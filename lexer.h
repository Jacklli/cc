#ifndef __LEXER_H_
#define __LEXER_H_

#include "token.h"

#include <list>
#include <string>

class scanner;
class tokenmap;


class lexer {
  public:
    explicit lexer(const std::string& srcName);
    std::string getNextToken();

  private:
    scanner *scaner;
    std::list<tokenmap> toknSeq;
};


#endif
