#ifndef __SCANNER_H_
#define __SCANNER_H_

#include "token.h"
#include <string>

#define END_OF_FILE 255

struct tokenMap {
  token tokn;
  union value {
    int i[2];
    float f;
    double d;
    std::string *p;
  };
};

#define isDigit(c)         (c >= '0' && c <= '9')
#define isOctDigit(c)      (c >= '0' && c <= '7')
#define isHexDigit(c)      (isDigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
#define isLetter(c)        ((c >= 'a' && c <= 'z') || (c == '_') || (c >= 'A' && c <= 'Z'))
#define isLetterOrDigit(c) (isLetter(c) || isDigit(c))
#define toUpper(c)                 (c & ~0x20)
#define HIGH_4BIT(v)       ((v) >> (8 * sizeof(int) - 4) & 0x0f)
#define HIGH_3BIT(v)       ((v) >> (8 * sizeof(int) - 3) & 0x07)
#define HIGH_1BIT(v)       ((v) >> (8 * sizeof(int) - 1) & 0x01)

typedef tokenMap *(*scanner)(void);

class scanner {
  public:
    explicit scanner(const std::string& srcName);
    scanner scaners[256];

  private:
    tokenMap *scanEOF();
    tokenMap *scanCharLiteral();
    tokenMap *scanStringLiteral();
    tokenMap *scanPlus();
    tokenMap *scanMinus();
    tokenMap *scanStar();
    tokenMap *scanSlash();
    tokenMap *scanPercent();
    tokenMap *scanLess();
    tokenMap *scanGreat();
    tokenMap *scanExclamation();
    tokenMap *scanEqual();
    tokenMap *scanBar();
    tokenMap *scanAmpersand();
    tokenMap *scanCaret();
    tokenMap *scanDot();

    tokenMap *scanLBRACE();
    tokenMap *scanRBRACE();
    tokenMap *scanLBRACKET();
    tokenMap *scanRBRACKET();
    tokenMap *scanLPAREN();
    tokenMap *scanRPAREN();
    tokenMap *scanCOMMA();
    tokenMap *scanSEMICOLON();
    tokenMap *scanCOMP();
    tokenMap *scanQUESTION();
    tokenMap *scanCOLON();

    tokenMap *scanIdentifier();
    tokenMap *scanPlus();
    tokenMap *scanStringLiteral();
    tokenMap *scanNumericLiteral();
    tokenMap *scanBadChar();

    unsigned char *cursor;
};

#endif
