
#include "scanner.h"


/*******************************************************
        Only when *CURSOR is 0xFF,
        and
        CURSOR points to the position behind the last byte.
 *******************************************************/
#define IS_EOF(cur)      (*(cur) == END_OF_FILE && ((cur)-Input.base) == Input.size)

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
static void ScanPPLine(void) {

}
 

scanner::scanner(const std::string& srcName)
 :srcName(srcName) {
    for(int i=0; i<256; i++) {
        if(IsLetter(i)) {
            scaners[i] = ScanIdentifier;
        } else if (IsDigit(i)) {
            scaners[i] = ScanNumericLiteral;
        } else {
            scaners[i] = ScanBadChar;
        }

        scaners[END_OF_FILE] = ScanEOF;
        scaners['\''] = ScanCharLiteral;       // wide chars/strings are parsed in ScanIdentifier(void)
        scaners['"']  = ScanStringLiteral;
        scaners['+']  = ScanPlus;
        scaners['-']  = ScanMinus;
        scaners['*']  = ScanStar;
        scaners['/']  = ScanSlash;
        scaners['%']  = ScanPercent;
        scaners['<']  = ScanLess;
        scaners['>']  = ScanGreat;
        scaners['!']  = ScanExclamation;
        scaners['=']  = ScanEqual;
        scaners['|']  = ScanBar;
        scaners['&']  = ScanAmpersand;
        scaners['^']  = ScanCaret;
        scaners['.']  = ScanDot;
        // see Macro SINGLE_CHAR_SCANNER(t)
        scaners['{']  = ScanLBRACE;
        scaners['}']  = ScanRBRACE;
        scaners['[']  = ScanLBRACKET;
        scaners[']']  = ScanRBRACKET;
        scaners['(']  = ScanLPAREN;
        scaners[')']  = ScanRPAREN;
        scaners[',']  = ScanCOMMA;
        scaners[';']  = ScanSEMICOLON;
        scaners['~']  = ScanCOMP;
        scaners['?']  = ScanQUESTION;
        scaners[':']  = ScanCOLON;
    }
}

