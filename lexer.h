#ifndef __LEXER_H_
#define __LEXER_H_

#include <list>
#include <string>

class scanner {
  public:
    explicit scanner(const std::string& srcName);

  private:
    const std::string srcName;
};

class lexer {
  public:
    explicit lexer(const std::string& srcName);

  private:
    scanner scaner;
    std::list<std::string> tokSeq;
};


#endif
