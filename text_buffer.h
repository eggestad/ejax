#ifndef TEXT_BUFFER_H
#define TEXT_BUFFER_H
#include "types.h"

#include <list>
#include <string>
#include <variant>
#include <vector>
#include <iostream>

namespace ejax {

   struct TextBufferLine {
      std::string line;
      int pos;
      int lineNum;

      TextBufferLine() : line(""), pos(0), lineNum(1) { line = std::string(""); };
      TextBufferLine(std::string s, int ln, long p) : line(s), pos(p), lineNum(ln){};

      void print() {
         printf("BL: pos:%6d line: %5d  (%3d)::\e[93m%s\e[m:: \n",
                pos, lineNum, line.length(), line.c_str());
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


   class TextBuffer {

      size_t size;
      size_t linecount;
      long currentLine;

  public:
      std::vector<TextBufferLine> lines;
      size_t getSize() {
         return size;
      }

      void recalc(long linestart);


      TextBuffer();

      TextBuffer(std::string s);

      void insertText(Point pos, std::string str);
      //void insertFullLine(std::string str, ulong linenum);
      void insertChar(Point pos, char c);
      void backspace(Point pos);

      void dump();


      void deleteChar(Point point);
   };
   typedef struct TextBuffer TextBuffer;

}// namespace ejax

#endif// TEXT_BUFFER_H
