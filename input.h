#ifndef __INPUT_H_
#define __INPUT_H_

// coordinate
typedef struct coord
{
        char *filename;
        int ppline;                     // see function ScanPPLine(void), the line number in *.c or other current included file
        int line;                       // the line number in the original *.i
        int col;                        // column 
} *Coord;

class Input {

  public:
      Input();
      void ReadSourceFile();
      void CloseSourceFile(void);

  private:
      char *filename;
      unsigned char *base;    // points to heap memory for the file.
      unsigned char *cursor;
      unsigned char *lineHead;
      int line;
      void* file;             // file handle returned by  fopen() / CreateFileA() / open()
      void* fileMapping;      // handle returned by CreateFileMapping() on Win32
      unsigned long size;     // file size

}





#endif
