#ifndef __SCANNER_H_
#define __SCANNER_H_

#include "token.h"
#include <string>
#include <functional>

#define END_OF_FILE 255
#define ALL_CHAR_NUMBER 256

#define	IS_EOF(cur)	   (static_cast<int>(*(cur)) == END_OF_FILE && ((cur) - base) == size)

#define isDigit(c)         (c >= '0' && c <= '9')
#define isOctDigit(c)      (c >= '0' && c <= '7')
#define isHexDigit(c)      (isDigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
#define isLetter(c)        ((c >= 'a' && c <= 'z') || (c == '_') || (c >= 'A' && c <= 'Z'))
#define isLetterOrDigit(c) (isLetter(c) || isDigit(c))
#define toUpper(c)                 (c & ~0x20)
#define HIGH_4BIT(v)       ((v) >> (8 * sizeof(int) - 4) & 0x0f)
#define HIGH_3BIT(v)       ((v) >> (8 * sizeof(int) - 3) & 0x07)
#define HIGH_1BIT(v)       ((v) >> (8 * sizeof(int) - 1) & 0x01)

typedef std::function <bool ()> scanner;

class lexer {
  public:
    explicit lexer(std::string& srcName);
    ~lexer();
    bool getNextToken();

  private:
    bool setUpLexer();

    int findKeyword(char *str, int len);

    int scanEscapeChar(int wide);
    bool scanEOF();
    bool scanCharLiteral();
    bool scanStringLiteral();
    
    bool scanIdentifier();

    bool scanPlus();
    bool scanMinus();
    bool scanStar();
    bool scanSlash();
    bool scanPercent();
    bool scanLess();
    bool scanGreat();
    bool scanExclamation();
    bool scanEqual();
    bool scanBar();
    bool scanAmpersand();
    bool scanCaret();
    bool scanDot();

    bool scanLBRACE();
    bool scanRBRACE();
    bool scanLBRACKET();
    bool scanRBRACKET();
    bool scanLPAREN();
    bool scanRPAREN();
    bool scanCOMMA();
    bool scanSEMICOLON();
    bool scanCOMP();
    bool scanQUESTION();
    bool scanCOLON();

    bool scanNumericLiteral();
    bool scanBadChar();
    
    scanner scaners[ALL_CHAR_NUMBER];

    // source file contents related
    bool fetchContent();
    std::string srcName;
    char *base;
    unsigned long size;
    char *cursor;

    // coordination details
    tokenMap tokMap;
    unsigned int ppline;
    unsigned int line;
    unsigned int col;
};

#endif
