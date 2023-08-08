

#include "text_buffer.h"
#include "types.h"
#include <cassert>
#include <iostream>
#include <stdexcept>


#include "ejax/include/ejax/logging.h"


using namespace std;

namespace ejax {

   TextBuffer::TextBuffer() : size(0), lines() {
      lines.push_back(TextBufferLine());
   };
// really needed???
   TextBuffer::TextBuffer(string s) : lines() {
      lines.push_back(TextBufferLine());
      insertText(0, s);
   };

   list<string> splitByNewLine(const string &s) {
      list<string> l(0);
      long off = 0;
      do {
         auto idx = s.find('\n', off);
         if (idx == s.npos) idx = s.size();
         l.push_back(s.substr(off, idx - off));
         off = idx + 1;
      } while (off < s.size());
      return l;
   }

   void TextBuffer::dump() {
      cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv"  <<endl;
      cout << "BUFFER DUMP: size: " << size << " linecount: " << lines.size() << endl;

      for (auto it = lines.begin(); it != lines.end(); ++it) {
         (*it).print();
      }
      cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"  <<endl;

   };

#if (0)
   vector<TextBufferLine>::iterator TextBuffer::getLineByPoint(Point point) {
	   // needs speedup

	   for (auto it = lines.begin(); it != lines.end(); it++) {
		   auto start = (*it).pos;
		   if (point < start) throw logic_error("buffer line pos and length corrupt?");
		   if (start + (*it).line.length() >=point)
			   return it;
	   }
	   throw out_of_range("point outside of buffer length");
   }
#else
   vector<TextBufferLine>::iterator TextBuffer::getLineByPoint(Point point) {
	   // still needs speedup, we should do a binary search for the line;
	   auto it = lines.end()-1;

	   cout << "end of buffer at " << it->lineNum << " pos "<< it->pos << " lines "<< lines.size()<<endl;
	   if (it->pos + it->line.size() < point)
	   	   throw out_of_range("point outside of buffer length");

	   it = lines.begin();
	   it += (point / this->meanLinelength );
	   do {
		   int startpos = it->pos;
		   int endpos = it->pos + it->line.size();
		   if (startpos <= point && endpos >= point) {
			   return it;
		   }
		   if (startpos > point) {
			   // we're below target line
			   it--;
		   } else if (endpos < point){
			   it++;
		   }

	   } while (1);


   }
#endif

   void TextBuffer::insertText(Point point, string str) {

      if (str.length() == 0) return;

      cout << "insert::'''" << str << "''' " << str.size() << endl;

      auto cursor = getLineByPoint(point);
      long firstLineNum = cursor->lineNum;

      cout << "inserting: linenum and point " << firstLineNum << " " << point << endl;

      auto newlines = splitByNewLine(str);
      cout << "inserting: " << newlines.size() << " lines" << endl;

      if (newlines.size() == 1) {// just insert
         cout << "inserting: no new lines\n";
         auto p = (*cursor).split(point);
         (*cursor).line = get<0>(p) + str + get<1>(p);
         recalc(cursor->lineNum);
         return;
      }

      cout << "inserting: splitting current line\n";

      // first split the current line into two

      auto p = (*cursor).split(point);

      // then append the first newline to the above line
      (*cursor).line = get<0>(p) + (*newlines.begin());
      cout << "inserting: text at start: " << (*cursor).line << endl;
      newlines.erase(newlines.begin());

      // then prepend the last line

      string last = *newlines.rbegin();
      TextBufferLine *bl = new TextBufferLine(last + get<1>(p), 0, 1);
      cursor = lines.insert(cursor+1, *bl);
      cout << "inserting: text at end: " << bl->line << endl;
      newlines.pop_back();

      // we've now done the first and last
      // the lines in the middle will just be inserted wholesale

      cout << "inserting: " << newlines.size() << " middle lines\n";
      for (string l: newlines) {

    	  cout << "'''" << l << "''' ins: 1" << endl;
    	  cout << cursor->line << cursor->lineNum << endl;
    	  TextBufferLine *bl = new TextBufferLine(l, 0, 1);
    	  bl->print();
    	  cout << " ins: 2\n";
    	  cursor = lines.insert(cursor+1, *bl);
    	  cout << " ins: 3\n";

      }
      recalc(0);
      //this->recalc(0);
   }

   void ejax::TextBuffer::deleteChar(Point point) {
      if (point >= size - 1) return;
      auto bufferLineIt = getLineByPoint(point);


      int off = point - bufferLineIt->pos;
      assert(off >= 0);

      int eolOff = bufferLineIt->line.size();
      assert(off <= eolOff);

      // if we delete EOL, merge with next line
      if (off == eolOff) {
         auto itnext = bufferLineIt;
         itnext++;

         bufferLineIt->line.append(itnext->line);
         lines.erase(itnext);
      } else {
    	  // just delete one char;
    	  bufferLineIt->line.erase(off, 1);
      }
      recalc(0);
   }


   void ejax::TextBuffer::recalc(long linestart) {
      printf("recalc lines %d %d\n", linestart, lines.size());
      long pos = 0;
      long lnum = 1;
      for (auto it = lines.begin(); it != lines.end(); it++) {
         it->pos = pos;
         it->lineNum = lnum++;
         pos += it->line.length() + 1;
         printf("line %d len %d pos %d \n", it->lineNum, it->line.length(), pos);
      }
      this->size = pos;
      //dump();
   }
}
