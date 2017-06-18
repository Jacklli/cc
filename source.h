
#ifndef __SOURCE_H_
#define __SOURCE_H_

#include "token.h"

// abstract for the input *.i files
class source {
  public:
    explicit(std::string& srcName);
    bool readSource();
    unsigned char *getCursor();
    unsigned char *getBase();
    unsgned int getLength();

  private:
    std::string fileName;
    unsigned char *base;        // points to heap memory for the file.
    unsigned char *cursor;
    unsigned char *lineHead;
    unsigned long size;
};

// abstract for coordination of each token got from *.i files
class tokCoord {
  public:
    explicit coord();
    bool setCurrSrcName(std::string& name);
    bool setCurrTokMap(tokenMap *tokmap);
    std::string getCurrSrcName();
    unsigned int getCurrPPLine();    // line numbers before pre-processed.
    unsigned int getCurrLine();      // line numbers after pre-processed.
    unsigned int getCurrCol();       // cols are the same between before pre-processed and after pre-processed.

  private:
    std::string srcName;
    tokenMap *tokmap;
    unsigned int ppline;
    unsigned int line;
    unsigned int col;
};


#endif
