#ifndef __SCANNER_H_
#define __SCANNER_H_

#include "token.h"
#include <string>

#define END_OF_FILE 255

struct tokenmap {
  token tokn;
  union value {
    int i[2];
    float f;
    double d;
    void *p;
  };
};


#define IsDigit(c)         (c >= '0' && c <= '9')
#define IsOctDigit(c)      (c >= '0' && c <= '7')
#define IsHexDigit(c)      (IsDigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
#define IsLetter(c)        ((c >= 'a' && c <= 'z') || (c == '_') || (c >= 'A' && c <= 'Z'))
#define IsLetterOrDigit(c) (IsLetter(c) || IsDigit(c))
#define ToUpper(c)                 (c & ~0x20)
#define HIGH_4BIT(v)       ((v) >> (8 * sizeof(int) - 4) & 0x0f)
#define HIGH_3BIT(v)       ((v) >> (8 * sizeof(int) - 3) & 0x07)
#define HIGH_1BIT(v)       ((v) >> (8 * sizeof(int) - 1) & 0x01)

typedef tokenmap *(*Scanner)(void);

tokenmap *ScanEOF();
tokenmap *ScanCharLiteral();
tokenmap *ScanStringLiteral();
tokenmap *ScanPlus();
tokenmap *ScanMinus();
tokenmap *ScanStar();
tokenmap *ScanSlash();
tokenmap *ScanPercent();
tokenmap *ScanLess();
tokenmap *ScanGreat();
tokenmap *ScanExclamation();
tokenmap *ScanEqual();
tokenmap *ScanBar();
tokenmap *ScanAmpersand();
tokenmap *ScanCaret();
tokenmap *ScanDot();

// defined in lexer.cc with macro form
tokenmap *ScanLBRACE();
tokenmap *ScanRBRACE();
tokenmap *ScanLBRACKET();
tokenmap *ScanRBRACKET();
tokenmap *ScanLPAREN();
tokenmap *ScanRPAREN();
tokenmap *ScanCOMMA();
tokenmap *ScanSEMICOLON();
tokenmap *ScanCOMP();
tokenmap *ScanQUESTION();
tokenmap *ScanCOLON();

#define SINGLE_CHAR_SCANNER(t) \
tokenmap *Scan##t(void) {      \
    return NULL;               \
}

SINGLE_CHAR_SCANNER(LBRACE)
SINGLE_CHAR_SCANNER(RBRACE)
SINGLE_CHAR_SCANNER(LBRACKET)
SINGLE_CHAR_SCANNER(RBRACKET)
SINGLE_CHAR_SCANNER(LPAREN)
SINGLE_CHAR_SCANNER(RPAREN)
SINGLE_CHAR_SCANNER(COMMA)
SINGLE_CHAR_SCANNER(SEMICOLON)
SINGLE_CHAR_SCANNER(COMP)
SINGLE_CHAR_SCANNER(QUESTION)
SINGLE_CHAR_SCANNER(COLON)

tokenmap *ScanIdentifier();
tokenmap *ScanPlus();
tokenmap *ScanStringLiteral();
tokenmap *ScanNumericLiteral();
tokenmap *ScanBadChar();

class scanner {
  public:
    explicit scanner(const std::string& srcName);

  private:
    Scanner scaners[255];
    const std::string srcName;
};


#endif
