#ifndef TEXT_BUFFER_H_
#define TEXT_BUFFER_H_

#include <string>
#include <list>
#include <vector>
#include <iterator>
#include <variant>





namespace ejax {

   struct TextBufferLine {
      std::string line;
      int pos;
      int lineNum;

   TextBufferLine(): line(""), pos(0), lineNum(1) {line = std::string("");};
   TextBufferLine(std::string s, int ln, long p): line(s), pos(p), lineNum(ln) {};

      void print() {
	 printf("BL: pos:%6d line: %5d  (%3d)::\e[93m%s\e[m:: \n",
		pos, lineNum,  line.length(), line.c_str());
      }

      std::pair<std::string, std::string> split(Point point) {
	 std::cout <<"split: point pos:  " << point  << " " <<  pos << std::endl;
	 int offset = point - pos;
	 std::cout << "split: off lsize "<< offset << " " << line.size() << std::endl;
	 auto pair = std::make_pair(line.substr(0, offset), line.substr(offset));

	 std::cout << "split: " << std::get<0>(pair) << " - " <<  std::get<1>(pair) << std::endl;
	 std::cout << "split: " << std::get<0>(pair).size() << " - " <<  std::get<1>(pair).size() << std::endl;
	 
	 return std::make_pair(line.substr(0, offset), line.substr(offset));
      }
   };

   typedef TextBufferLine TextBufferLine;
 

   class TextBuffer {
   
      size_t size;
      size_t linecount;
      std::vector<TextBufferLine> lines;
      long currentLine;

   public:
      void recalc(long linestart) {
	 printf("recalc %d %d\n", linestart, lines.size());
	 long pos = 0;
	 long lnum= 1;
	 for (auto it=lines.begin(); it != lines.end(); it++) {
	    if (point >= pos and point <= pos + (*it).line.size() + 1) currentLine = lnum;
	    (*it).pos = pos;
	    (*it).lineNum = lnum++;
	    pos +=(*it).line.length() + 1;
	    
	 }
	 size= pos;
	 dump();
      }
	    
      
   public:
   TextBuffer(): size(0), linecount(1), lines() {
	 lines.push_back(TextBufferLine());
	 currentLine = 1;
      };

   TextBuffer(std::string s): point(0), lines() {
	 lines.push_back(TextBufferLine());
	 currentLine = 1;
	 insertText(s);
      };
      
      void insertText(long pos, std::string str);
      //void insertFullLine(std::string str, ulong linenum);
      void insertChar(long pos, char c);
      void deleteChar(long pos);
      void backspace(long pos);
      
      void dump();
	 

   };
   typedef struct TextBuffer TextBuffer;

}

#endif  // TEXT_BUFFER_H_

