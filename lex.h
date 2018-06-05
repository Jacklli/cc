#ifndef __LEX_H_
#define __LEX_H_

#include "input.h"
#include "alloc.h"

extern Heap CurrentHeap;

const char* TokenStrings[] =
{
#define TOKEN(k, s) s,
#include "token.h"
#undef  TOKEN
};

enum token
{
        TK_BEGIN,
#define TOKEN(k, s) k,
#include "token.h"
#undef  TOKEN

};

// Token Value
union value
{
        int i[2];
        float f;
        double d;
        void *p;
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


typedef int (*Scanner)(void);


class Lexer {

  public:
      Lexer();
      void SetupLexer(void);
      void BeginPeekToken(void);
      void EndPeekToken(void);
      int  GetNextToken(void);

  private:
      void ScanPPLine(void);
      int SkipWin32Declares(void);
      void SkipWhiteSpace(void);
      int ScanEscapeChar(int wide);
      int FindKeyword(char *str, int len);
      int ScanIntLiteral(unsigned char *start, int len, int base);
      int ScanFloatLiteral(unsigned char *start);
      int ScanNumericLiteral(void);
      int ScanCharLiteral(void);
      int ScanStringLiteral(void);
      int ScanIdentifier(void);
      int ScanPlus(void);
      int ScanMinus(void);
      int ScanStar(void);
      int ScanSlash(void);
      int ScanPercent(void);
      int ScanLess(void);
      int ScanGreat(void);
      int ScanExclamation(void);
      int ScanEqual(void);
      int ScanBar(void);
      int ScanAmpersand(void);
      int ScanCaret(void);
      int ScanDot(void);
      int ScanBadChar(void);
      int ScanEOF(void);
      
      unsigned char *CURSOR;
      unsigned char *PeekPoint;
      union value    PeekValue;
      struct coord   PeekCoord;
      union value  TokenValue;
      struct coord TokenCoord;
      struct coord PrevCoord;
      Scanner Scanners[256];
};










#endif
