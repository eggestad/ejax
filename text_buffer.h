#ifndef TEXT_BUFFER_H
#define TEXT_BUFFER_H
#include "types.h"

#include <list>
#include <string>
#include <variant>
#include <vector>
#include <iostream>


namespace ejax {

   class BufferView {
   };
   
   /*
    * Text Buffer Line
    */
   struct TextBufferLine {
      std::string line;
      int pos;
      int lineNum;

      TextBufferLine() :  pos(0), lineNum(1) { line = std::string(""); };
      //TextBufferLine(std::string s, int ln, long p) : line(s), pos(p), lineNum(ln){};
      TextBufferLine(std::string s, int ln, long p) {
    	  line = s;
    	  pos = p;
    	  lineNum = ln;
      };


      void print() {
         printf("BL: pos:%6d line: %5d  (%3d)::\e[93m%s\e[m:: %p\n",
                pos, lineNum, line.length(), line.c_str(), this);
      }

      std::pair<std::string, std::string> split(Point point) {
         std::cout << "split: point pos:  " << point << " " << pos << std::endl;
         int offset = point - pos;
         std::cout << "split: off lsize " << offset << " " << line.size() << std::endl;
         auto pair = std::make_pair(line.substr(0, offset), line.substr(offset));

         std::cout << "split: " << std::get<0>(pair) << " - " << std::get<1>(pair) << std::endl;
         std::cout << "split: " << std::get<0>(pair).size() << " - " << std::get<1>(pair).size() << std::endl;

         return std::make_pair(line.substr(0, offset), line.substr(offset));
      }
   };

   typedef TextBufferLine TextBufferLine;

   /*
    * Text buffer
    */
   class TextBuffer {
	   std::string name;
	   std::string fileurl;

	   size_t size;
	   int meanLinelength = 20; // C code

	   std::vector<TextBufferLine>::iterator getLineByPoint(Point point) ;

   public:
      std::vector<TextBufferLine> lines;
      size_t getSize() {
         return size;
      }

      void recalc(long linestart);
      void registerView(BufferView *);
      
      TextBuffer();

      TextBuffer(std::string s); // Really necessary?

      void insertText(Point pos, std::string str);
      //void insertFullLine(std::string str, ulong linenum);
      void insertChar(Point pos, char c);
      void backspace(Point pos);

      void dump();


      void deleteChar(Point point);

      std::string asString();
      std::string asString(Point start, long len);

      void writeToFile(std::string fullpath);

      const std::string& getFileurl() const {
    	  return fileurl;
      }

      void setFileurl(const std::string &fileurl) {
    	  this->fileurl = fileurl;
      }

      const std::string& getName() const {
    	  return name;
      }

      void setName(const std::string &name) {
    	  this->name = name;
      }
   };
   typedef struct TextBuffer TextBuffer;

}// namespace ejax

#endif// TEXT_BUFFER_H
