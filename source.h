
#ifndef __SOURCE_H_
#define __SOURCE_H_

class source {
  public:
  private:
// static information while parsing
    std::string filename;
    unsigned char *base;
    unsigned char *cursor;
    unsigned char *lineHead;
    lin line;
    void *fileMapping;
    unsigned long size;
    
// dynamic information while parsing
    int ppline;
    int line;
    int col;
};


#endif
