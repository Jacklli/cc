
#include "lexer.h"
#include <iostream>
#include <fstream>

lexer::lexer(std::string& srcName)
 :srcName(srcName) {
    setUpLexer();
    fetchContent();
}

lexer::~lexer() {
    delete []base;
}

bool lexer::setUpLexer() {
    for (int i=0; i<ALL_CHAR_NUMBER; i++) {
        if(isLetter(i)) {
            scaners[i] = std::bind(&lexer::scanIdentifier, this); 
        } else if (isDigit(i)) {
//            scaners[i] = std::bind(&lexer::scanNumericLiteral, this);
            std::cout<< "hello world"<<std::endl;
        } else {
            std::cout<< "hello world"<<std::endl;
//            scaners[i] = std::bind(&lexer::scanBadChar, this);
        }

/*
        scaners[END_OF_FILE] = std::bind(&lexer::scanEOF, this);
        scaners['\''] = std::bind(&lexer::scanCharLiteral, this);       // wide chars/strings are parsed in scanIdentifier(void)
        scaners['"']  = std::bind(&lexer::scanStringLiteral, this);
        scaners['+']  = std::bind(&lexer::scanPlus, this);
        scaners['-']  = std::bind(&lexer::scanMinus, this);
        scaners['*']  = std::bind(&lexer::scanStar, this);
        scaners['/']  = std::bind(&lexer::scanSlash, this);
        scaners['%']  = std::bind(&lexer::scanPercent, this);
        scaners['<']  = std::bind(&lexer::scanLess, this);
        scaners['>']  = std::bind(&lexer::scanGreat, this);
        scaners['!']  = std::bind(&lexer::scanExclamation, this);
        scaners['=']  = std::bind(&lexer::scanEqual, this);
        scaners['|']  = std::bind(&lexer::scanBar, this);
        scaners['&']  = std::bind(&lexer::scanAmpersand, this);
        scaners['^']  = std::bind(&lexer::scanCaret, this);
        scaners['.']  = std::bind(&lexer::scanDot, this);

        scaners['{']  = std::bind(&lexer::scanLBRACE, this);
        scaners['}']  = std::bind(&lexer::scanRBRACE, this);
        scaners['[']  = std::bind(&lexer::scanLBRACKET, this);
        scaners[']']  = std::bind(&lexer::scanRBRACKET, this);
        scaners['(']  = std::bind(&lexer::scanLPAREN, this);
        scaners[')']  = std::bind(&lexer::scanRPAREN, this);
        scaners[',']  = std::bind(&lexer::scanCOMMA, this);
        scaners[';']  = std::bind(&lexer::scanSEMICOLON, this);
        scaners['~']  = std::bind(&lexer::scanCOMP, this);
        scaners['?']  = std::bind(&lexer::scanQUESTION, this);
        scaners[':']  = std::bind(&lexer::scanCOLON, this);
*/
    }
    return true;
}

bool lexer::fetchContent() {
    std::filebuf *pbuf = NULL;
    std::ifstream filestr;
    base = NULL, size = 0;  // data member of class lexer
    filestr.open(srcName, std::ios::binary);
    pbuf = filestr.rdbuf();

    size = pbuf->pubseekoff(0, std::ios::end, std::ios::in);
    pbuf->pubseekpos(0, std::ios::in);
    base = new char[size];
    pbuf->sgetn(base, size);
    cursor = base;
    filestr.close();

//    std::cout.write(base, size);
    return true;
}

// return keyword or TK_ID
int findKeyword(char *str, int len) {
    struct keyword *p = NULL;

    //index is 0 when "__int64", see keyword.h      static struct keyword keywords_[]
    int index = 0;

    if (*str != '_')
        index = toUpper(*str) - 'A' + 1;

    p = &keywords[index];
    while (p->name) {
        if (p->len == len && strncmp(str, p->name, len) == 0)
            break;
	p++;
    }
    return p->tok;
}

int lexer::scanEscapeChar(int wide) {
    int v = 0, overflow = 0;

    cursor++;
    switch (*cursor++) {
    case 'a':
        return '\a';

    case 'b':
        return '\b';

    case 'f':
        return '\f';

    case 'n':
        return '\n';

    case 'r':
        return '\r';

    case 't':
        return '\t';

    case 'v':
        return '\v';

    case '\'':
    case '"':
    case '\\':
    case '\?':
        return *(cursor - 1);

    case 'x':               //              \xhh    hexical
        if (! isHexDigit(*cursor)) {
            std::cout << "Expect hex digit" << std::endl;
            return 'x';
        }    
        v = 0; 
        while (isHexDigit(*cursor)) {
	    /***************************************
	    Bug?    
	    if(v >> (WCharType->size * 8-4 )) 
	    (1) WCharType->size == 2 
	    0xABCD * 16 + value --> overflow
	    0x0ABC is OK.
	    (2) WCharType->size == 4 
	    0x12345678  * 16 + value --> overflow
	    0x01234567 is OK.
	    ****************************************/
	    if (v >> (WCharType->size*8 - 4)) {
	        overflow = 1;
	    }
	    //  v= v * 16 + value,  value : 0-9  A-F
	    if (isDigit(*cursor)) {
	        v = (v << 4) + *cursor - '0';
	    }
	    else {
	        v = (v << 4) + toUpper(*cursor) - 'A' + 10;
	    }
	    cursor++;
	}
	if (overflow || (! wide && v > 255)) {
	    std::cout << "Hexademical espace sequence overflow" << std::endl;
        }
        return v;

    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7': // \ddd octal
        v = *(cursor - 1) - '0';
        if (isOctDigit(*cursor)) {
	    v = (v << 3) + *cursor++ - '0';
	    if (isOctDigit(*cursor))
                v = (v << 3) + *cursor++ - '0';
	}
	return v;

    default:
        std::cout << "Unrecognized escape sequene " <<  *cursor << std::endl;
        return *cursor;
    }
}
                                        
bool lexer::scanCharLiteral() {
    size_t n = 0;
    int count = 0;
    int wide = 0;
    unsigned short ch = 0;

    if (*cursor == 'L') {    // wide char  L'a' L'\t'
	cursor++;
	wide = 1;
    }
    cursor++;               // skip \'
    if (*cursor == '\'') {
	std::cout << "empty character constant" << std::endl;
    } else if (*cursor == '\n' || IS_EOF(cursor)) {
	std::cout << "missing terminating character" << std::endl;
    } else {
	if(*cursor == '\\') {
	    ch = (unsigned short) scanEscapeChar(wide);
	} else {
	    if(wide) {
	        n = mbrtowc(&ch, cursor, MB_CUR_MAX, 0);
	        if(n > 0) {
	            cursor += n;
	        }
		// PRINT_DEBUG_INFO(("%x %x",n,ch));
	    } else {
	        ch = *cursor;
	        cursor++;
	    }
	}
	while (*cursor != '\'') {      // L'abc',  skip the redundant characters
	    if (*cursor == '\n' || IS_EOF(cursor))
	        break;
	    cursor++;
	    count++;
	}
    }


    if (*cursor != '\'') {
        std::cout << "missing terminating ' character";
        goto end_char;
    }
    cursor++;
    if (count > 0) {
        std::cout << "Two many characters" << std::endl;
    }

end_char:
    tokMap.value.i[0] = ch;
    tokMap.value.i[1] = 0;
}

bool lexer::scanIdentifier() {
    unsigned int tok = 0;
    char *start = cursor;

    if (*cursor == 'L') {     // special case :  wide char/string
        if (cursor[1] == '\'') {
            return scanCharLiteral();       // L'a' wide char
        }
        if (cursor[1] == '"') {
            return scanStringLiteral();     // L"wide string"
        }
    }
    // letter(letter|digit)*
    cursor++;
    while (isLetterOrDigit(*cursor)) {
        cursor++;
    }

    tok = findKeyword((char *)start, (int)(cursor - start));
    if (tok == TK_ID) {
        tokMap.value.p = new std::string(start, static_cast<int>(cursor - start));
    }
}


