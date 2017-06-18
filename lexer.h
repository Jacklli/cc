#ifndef __LEXER_H_
#define __LEXER_H_

#include "token.h"
#include "scanner.h"
#include "source.h"

#include <list>
#include <string>


class lexer {
  public:
    explicit lexer(const std::string& srcName);
    tokenMap *getNextToken();    // this function combines three classes: "scanner, source, tokCoord"

  private:
    scanner scaner;
    source src;
    tokCoord coord;
};


#endif
