
#ifndef __SOURCE_H_
#define __SOURCE_H_

class source {
  public:

  private:
    // static information
    std::string filename;
    unsigned char *base;        // points to heap memory for the file.
    unsigned char *cursor;
    unsigned char *lineHead;
    unsigned long size;
    
    // dynamic information
    int ppline; // line number in *.c
    int line;   // line number in *.i
    int col;
};


#endif
