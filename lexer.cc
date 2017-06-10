#include "lexer.h"
#include "token.h"
#include "keyword.h"

scanner::scanner(const std::string& srcName)
 :srcName(srcName) {

}

lexer::lexer(const std::string& srcName)
 :scaner(srcName) {
  
}

