
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
            //scaners[i] = scanIdentifier;
            scaners[i] = std::bind(&lexer::scanIdentifier, this); 
        } else if (isDigit(i)) {
            scaners[i] = std::bind(&lexer::scanNumericLiteral, this);
        } else {
            scaners[i] = std::bind(&lexer::scanBadChar, this);
        }

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

tokenMap *lexer::scanIdentifier() {
    unsigned int tok = 0;
    char *start = cursor;
    tokenMap *tkMap;

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
        tkMap->value.p = new std::string(start, static_cast<int>(cursor - start));
    }

    return tkMap;
}


