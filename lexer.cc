
#include "lexer.h"

lexer::lexer() {
    for (int i=0; i<256; i++) {
        if(isLetter(i)) {
            scaners[i] = scanIdentifier;
        } else if (isDigit(i)) {
            scaners[i] = scanNumericLiteral;
        } else {
            scaners[i] = scanBadChar;
        }

        scaners[END_OF_FILE] = scanEOF;
        scaners['\''] = scanCharLiteral;       // wide chars/strings are parsed in scanIdentifier(void)
        scaners['"']  = scanStringLiteral;
        scaners['+']  = scanPlus;
        scaners['-']  = scanMinus;
        scaners['*']  = scanStar;
        scaners['/']  = scanSlash;
        scaners['%']  = scanPercent;
        scaners['<']  = scanLess;
        scaners['>']  = scanGreat;
        scaners['!']  = scanExclamation;
        scaners['=']  = scanEqual;
        scaners['|']  = scanBar;
        scaners['&']  = scanAmpersand;
        scaners['^']  = scanCaret;
        scaners['.']  = scanDot;

        scaners['{']  = scanLBRACE;
        scaners['}']  = scanRBRACE;
        scaners['[']  = scanLBRACKET;
        scaners[']']  = scanRBRACKET;
        scaners['(']  = scanLPAREN;
        scaners[')']  = scanRPAREN;
        scaners[',']  = scanCOMMA;
        scaners[';']  = scanSEMICOLON;
        scaners['~']  = scanCOMP;
        scaners['?']  = scanQUESTION;
        scaners[':']  = scanCOLON;
    }
}

tokenMap *lexer::scanIdentifier() {
    unsigned char *start = cursor;
    tokenMap *tkMap;

    if (*cursor == 'L') {     // special case :  wide char/string
        if (cursor[1] == '\'') {
            return scanCharLiteral();       // L'a' wide char
        }
        if (cursor[1] == '"') {
            return scanStringLiteral();     // L"wide string"
        }
    }
    // lettter(letter|digit)*
    cursor++;
    while (isLetterOrDigit(*cursor)) {
        cursor++;
    }

    tok = findKeyword((char *)start, (int)(cursor - start));
    if (tok == TK_ID) {
        TokenValue.p = InternName((char *)start, (int)(cursor - start));
    }

    return tkMap;
}
