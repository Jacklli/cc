
#include "scanner.h"

// scan pre-processed line in *.i
static void scanPPLine(void) {

}
 

scanner::scanner() {
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

