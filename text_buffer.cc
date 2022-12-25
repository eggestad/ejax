

#include <iostream>
#include <assert.h>
#include "text_buffer.h"
#include <ejax/logging.h>


using namespace std;

namespace ejax {

   namespace {

    

      list<string> splitByNewLine(string s) {
	 list<string> l(0);
	 long off = 0;
	 do {
	    auto idx = s.find("\n", off);
	    if (idx == s.npos) idx = s.size();
	    l.push_back(s.substr(off, idx-off));
	    off = idx+1;
	 } while(off < s.size());
	 return l;
      }
   }
   
   
   
   void TextBuffer::dump() {
      cout << "BUFFER  size: " << size << " linecount: " <<linecount
	   << " point: " << point << " currentLine: " << currentLine <<endl;
      
      for (auto it=lines.begin(); it != lines.end(); ++it) {
	    (*it).print();
      }
   };
      
   void TextBuffer::insertText(string str) {
      
      if (str.length() == 0) return;
      
      cout << "insert::'" << str << "'" << str.size() <<endl;
      cout << "inserting: linenum and point " << currentLine << " " << point << endl;
      
      auto cursor = lines.begin() + (currentLine-1);

      auto newlines = splitByNewLine(str);
      cout << "inserting: " << newlines.size() << " lines"  <<endl;
       
      if (newlines.size() == 1) { // just insert
	 cout << "inserting: no new lines\n";
	 
	 auto p =  (*cursor).split(this->point);
	 (*cursor).line =  get<0>(p) + str +  get<1>(p);
	 
      } else {
	 int insertedLines = 1;
	 cout << "inserting: splittin current line\n";
	 // first split the currentn line into two
	 auto p =  (*cursor).split(this->point);
	 // then append the first newline to the above line
	 (*cursor).line = get<0>(p) + (*newlines.begin());
	 cout << "inserting: text at start: "<< (*cursor).line << endl;
	 newlines.erase(newlines.begin());
	 
	 // then prepend the last line
	 cursor++;
	 string last = *newlines.rbegin();
	 TextBufferLine bl( last  + get<1>(p), 0, 1);
	 lines.insert(cursor, bl);
	 cout << "inserting: text at end: "<< bl.line << endl;
	 newlines.pop_back();
	 
	 // we've now doen the first and last
	 // the lines in the middel will jst be inserted wholesale
	 
	 
	 cout << "inserting: " << newlines.size() << " middle lines\n";
	 for (string l : newlines) {
	    cursor = lines.begin() + (currentLine);
	    currentLine++;
	    cout << l << " ins: 1\n";
	    TextBufferLine bl(l, 0, 1);
	    bl.print();
	    cout  << " ins: 2\n";
	    lines.insert(cursor, bl);
	    cout << " ins: 3\n";
	    insertedLines++;
	 }

      } 
      point += str.length();
      this->recalc(0);	
       
 
   }

   void TextBuffer::deleteChar() {
      if (point >= size-1) return;
      auto it = lines.begin();
      it += currentLine -1;
      int off = point - (*it).pos;
      assert (off >= 0);
      int eolOff = (*it).line.size();
      assert (off <= eolOff);
      if (off == eolOff) {
	 auto itnext = lines.begin();
	 itnext  += currentLine ;
	 (*it).line = (*it).line + (*itnext).line;
	 lines.erase(itnext);
      } else {
	 (*it).line = (*it).line.substr(0,off) + (*it).line.substr(off+1);
      }
      recalc(0);
   }
 
   Point TextBuffer::getPoint() {
      return this->point;
   }

   Point TextBuffer::setPoint(Point  newPoint) {
      if (newPoint > size-1) newPoint = size-1;
      this->point = newPoint;
      long lnum= 1;
      for (TextBufferLine bl : lines) {
	 if (point >= bl.pos and point <= bl.pos + bl.line.size() + 1) currentLine = lnum;
	 lnum++;
      }
      return this->point;
   }
}
