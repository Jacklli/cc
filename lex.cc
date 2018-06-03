#include "lex.h"
#include "keyword.h"

#define CURSOR      (Input.cursor)
#define LINE        (Input.line)
#define LINEHEAD    (Input.lineHead)


int Lexer::GetNextToken(void) {
    int tok;

    PrevCoord = TokenCoord;
    SkipWhiteSpace();
    TokenCoord.line = LINE; // line number in the *.i for C compiler
    TokenCoord.col  = (int)(CURSOR - LINEHEAD + 1);
    // use function pointer table to avoid a large switch statement.
    tok = (*Scanners[*CURSOR])();
    return tok;
}

void Lexer::BeginPeekToken(void) {
    PeekPoint = CURSOR;
    PeekValue = TokenValue;
    PeekCoord = TokenCoord;
}

void Lexer::EndPeekToken(void) {
    CURSOR = PeekPoint;
    TokenValue = PeekValue;
    TokenCoord = PeekCoord;
}

/**
 * Scans preprocessing directive which specify the line number and filename such as:
 * # line 6 "C:\\Program Files\\Visual Stduio 6\\VC6\\Include\\stdio.h" or
 * # 6 "/usr/include/stdio.h"
 * Ignores other preprocessing directive.
 */
 /*****************************************************
        hello.c --->  hello.i           (On Linux)

        hello.c
                Line1:  #include <stdio.h>
                Line2:  int f(int n){
        hello.i
                Line655:        # 2 "hello.c" 2
  *****************************************************/

void Lexer::ScanPPLine(void) {
    int line = 0;

    CURSOR++;
    while (*CURSOR == ' ' || *CURSOR == '\t') {
        CURSOR++;
    }
    // # 2 "hello.c" 2              on Linux
    if (IsDigit(*CURSOR)) {
        goto read_line;
    }
    // # line 6 "C:\\Program Files\\Visual Stduio 6\\VC6\\Include\\stdio.h"         on Windows
    else if (strncmp(CURSOR, "line", 4) == 0) {
        CURSOR += 4;
        while (*CURSOR == ' ' || *CURSOR == '\t') {
            CURSOR++;
        }

read_line:
        // line number
        while (IsDigit(*CURSOR)) {
            line = 10 * line + *CURSOR - '0';
            CURSOR++;
        }
        TokenCoord.ppline = line - 1;
        // skip white space
        while (*CURSOR == ' ' || *CURSOR == '\t') {
            CURSOR++;
        }
        //get the filename: "hello.c " --->  hello.c
        TokenCoord.filename = ++CURSOR;
        while (*CURSOR != '"' && !IS_EOF(CURSOR)&& *CURSOR != '\n') {
            CURSOR++;
        }
        TokenCoord.filename = InternName(TokenCoord.filename, (char *)CURSOR - TokenCoord.filename);
    }
    while (*CURSOR != '\n' && !IS_EOF(CURSOR)) {
        CURSOR++;
    }
}

void Lexer::SkipWhiteSpace(void) {
    int ch;

again:
    ch = *CURSOR;
    while (ch == '\t' || ch == '\v' || ch == '\f' || ch == ' ' ||
         ch == '\r' || ch == '\n' || ch == '/'  || ch == '#') {
        switch (ch) {
            case '\n':
                TokenCoord.ppline++;
                LINE++;
                LINEHEAD = ++CURSOR;
                break;

            case '#':       //  # 2 "hello.c"               *.i files               generated by preprocesser
                ScanPPLine();
                break;

            case '/':       // comments
                /**************************************************
                    C Style comment or C++ style comment
                    On Linux, 
                    In fact , all the comments have been eaten by preprocesser.
                    The UCL compiler won't encounter any comments.
                 ***************************************************/
                if (CURSOR[1] != '/' && CURSOR[1] != '*')
                    return;
                CURSOR++;
                if (*CURSOR == '/') {
                    CURSOR++;
                    while (*CURSOR != '\n' && !IS_EOF(CURSOR)) {
                        CURSOR++;
                    }
                }
                else {
                   CURSOR += 1;
                   while (CURSOR[0] != '*' || CURSOR[1] != '/') {
                       if (*CURSOR == '\n') {
                           TokenCoord.ppline++;
                           LINE++;
                       }
                       else if (IS_EOF(CURSOR)|| IS_EOF(&CURSOR[1])) {
                           Error(&TokenCoord, "Comment is not closed");
                           return;
                       }
                       CURSOR++;
                   }
                   CURSOR += 2;
                }
                break;

            default:
                CURSOR++;
                break;
        }
        ch = *CURSOR;
    }

    if (ExtraWhiteSpace != NULL) {
        char *p;
        // ignore the unknown strings, that is , ExtraWhiteSpace.
        FOR_EACH_ITEM(char*, p, ExtraWhiteSpace)
            if (strncmp(CURSOR, p, strlen(p)) == 0) {
                CURSOR += strlen(p);
                goto again;
            }
        ENDFOR
    }
}

int Lexer::ScanEscapeChar(int wide) {
    int v = 0, overflow = 0;

    CURSOR++;
    switch (*CURSOR++) {
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
            return *(CURSOR - 1);

        case 'x':               //              \xhh    hexical
            if (! IsHexDigit(*CURSOR)) {
                Error(&TokenCoord, "Expect hex digit");
                return 'x';
            }
            v = 0;
            while (IsHexDigit(*CURSOR)) {
                    /***************************************
                            Bug?    
                            if(v >> (WCharType->size * 8-4 )) 
                            (1) WCharType->size == 2
                                    0xABCD * 16 + value --> overflow
                                    0x0ABC is OK.
                            (2) WCharType->size == 4
                                    0x12345678  * 16 + value --> overflow
                                    0x01234567 is OK.
                     ***************************************/
                if (v >> (WCharType->size*8 - 4)) {
                            overflow = 1;
                }
                //  v= v * 16 + value,  value : 0-9  A-F
                if (IsDigit(*CURSOR)) {
                    v = (v << 4) + *CURSOR - '0';
                } else {
                    v = (v << 4) + ToUpper(*CURSOR) - 'A' + 10;
                }
                CURSOR++;
            }

            if (overflow || (! wide && v > 255)) {
                Warning(&TokenCoord, "Hexademical espace sequence overflow");
            }
            return v;

        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7': // \ddd octal
            v = *(CURSOR - 1) - '0';
            if (IsOctDigit(*CURSOR)) {
                v = (v << 3) + *CURSOR++ - '0';
                if (IsOctDigit(*CURSOR))
                    v = (v << 3) + *CURSOR++ - '0';
            }
            return v;

        default:
            Warning(&TokenCoord, "Unrecognized escape sequence:\\%c", *CURSOR);
            return *CURSOR;
        }
}

// return keyword or TK_ID
int Lexer::FindKeyword(char *str, int len) {
    struct keyword *p = NULL;
    //index is 0 when "__int64", see keyword.h      static struct keyword keywords_[]
    int index = 0;

    if (*str != '_')
        index = ToUpper(*str) - 'A' + 1;

    p = keywords[index];
    while (p->name) {
        if (p->len == len && strncmp(str, p->name, len) == 0)
            break;
        p++;
    }
    return p->tok;
}

static int ScanIntLiteral(unsigned char *start, int len, int base) {
    unsigned char *p =  start;
    unsigned char *end = start + len;
    unsigned int i[2] = {0, 0};
    int tok = TK_INTCONST;
    int d = 0;
    int carry0 = 0, carry1 = 0;
    int overflow = 0;

    while (p != end) {
        if (base == 16) {
            if ((*p >= 'A' && *p <= 'F') ||
                (*p >= 'a' && *p <= 'f')) {
                d = ToUpper(*p) - 'A' + 10;
            } else {
                d = *p - '0';
            }
        } else {
            d = *p - '0';
        }
        /***********************************
                treat i[1],i[0] as 64 bit integer.                      
         ***********************************/
        switch (base) {
            case 16:
                carry0 = HIGH_4BIT(i[0]);
                carry1 = HIGH_4BIT(i[1]);
                i[0] = i[0] << 4;
                i[1] = i[1] << 4;
                break;

            case 8:
                carry0 = HIGH_3BIT(i[0]);
                carry1 = HIGH_3BIT(i[1]);
                i[0] = i[0] << 3;
                i[1] = i[1] << 3;
                break;

            case 10:
                {
                    unsigned int t1, t2;
                    // number * 10 = number * 8 + number * 2 = (number << 3) + (number << 1)
                    carry0 = HIGH_3BIT(i[0]) + HIGH_1BIT(i[0]);
                    carry1 = HIGH_3BIT(i[1]) + HIGH_1BIT(i[1]);
                    t1 = i[0] << 3;
                    t2 = i[0] << 1;
                    
                    // In maths:  t1 + t2 > UINT_MAX
                    if (t1 > UINT_MAX - t2) {
                        carry0++;
                    }
                    i[0] = t1 + t2;
                    t1 = i[1] << 3;
                    t2 = i[1] << 1;
                    if (t1 > UINT_MAX - t2) {
                        carry1++;
                    }
                    i[1] = t1 + t2;
                }
                break;
        }
        
        // for decimal, i[0] + d maybe greater than UINT_MAX
        if (i[0] > UINT_MAX - d) {
            carry0 += i[0] - (UINT_MAX - d);
        }
        if (carry1 || (i[1] > UINT_MAX - carry0)) {
            overflow = 1;
        }
        i[0] += d;
        i[1] += carry0;
        p++;
    }
    /*****************************
            overflow != 0:
                    out of 64 bit bound
            i[1] != 0
                    out of 32 bit bound
     *****************************/
    if (overflow || i[1] != 0) {
        Warning(&TokenCoord, "Integer literal is too big");
    }

    TokenValue.i[1] = 0;
    TokenValue.i[0] = i[0];
    tok = TK_INTCONST;

    /******************************************
            12345678U
            12345678u
     ******************************************/
    if (*CURSOR == 'U' || *CURSOR == 'u') {
        CURSOR++;
        if (tok == TK_INTCONST) {
            tok = TK_UINTCONST;
        } else if (tok == TK_LLONGCONST) {
            tok = TK_ULLONGCONST;
        }
    }
    /******************************************
            12345678UL
            12345678L
     ******************************************/
    if (*CURSOR == 'L' || *CURSOR == 'l') {
        CURSOR++;
        if (tok == TK_INTCONST) {
            tok = TK_LONGCONST;
        } else if (tok == TK_UINTCONST) {
            tok = TK_ULONGCONST;
        }
        // LL  long long int
        if (*CURSOR == 'L' || *CURSOR == 'l') {
            CURSOR++;
            if (tok < TK_LLONGCONST) {
                tok = TK_LLONGCONST;
            }
        }
    }

    return tok;
}

/************************************************
        @start  points to the beginngin of the float
        @CURSOR points to the possible  \. or E or e
// .123         , 123.456
 ************************************************/
int Lexer::ScanFloatLiteral(unsigned char *start) {
    double d;
    /*****************************************
            Just check the optional fragment part and the 
            exponent part.
            The value of float number is determined by 
            strtod().
     *****************************************/
    if (*CURSOR == '.') {
        CURSOR++;
        while (IsDigit(*CURSOR)) {
            CURSOR++;
        }    
    }    
    if (*CURSOR == 'e' || *CURSOR == 'E') {
        CURSOR++;
        if (*CURSOR == '+' || *CURSOR == '-') {
            CURSOR++;
        }    
        if (! IsDigit(*CURSOR)) {
            Error(&TokenCoord, "Expect exponent value");
        } else {
            while (IsDigit(*CURSOR)) {
                CURSOR++;
            }
        }
    }

    errno = 0;
    d = strtod((char *)start, NULL);
    if (errno == ERANGE) {
        Warning(&TokenCoord, "Float literal overflow");
    }
    TokenValue.d = d;
    // single precision float:  123.456f
    if (*CURSOR == 'f' || *CURSOR == 'F') {
        CURSOR++;
        TokenValue.f = (float)d;
        return TK_FLOATCONST;
    }
    // long double
    else if (*CURSOR == 'L' || *CURSOR == 'l') {
        CURSOR++;
        return TK_LDOUBLECONST;
    } else {
        return TK_DOUBLECONST;
    }
}

