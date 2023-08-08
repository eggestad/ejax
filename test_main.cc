#include "text_buffer.h"
#include "text_buffer_view.h"
#include <iostream>
#include <iterator>
#include <list>
#include <vector>


using namespace std;
using namespace ejax;



void foo_list() {
 int myints[] = {75,23,65,42,13};
   vector<int> l = vector({1,2,3,4,5});

   cout << ": (";
   for (auto it2 = l.begin(); it2 != l.end(); it2++) {
      cout << *it2 << ", ";
   }
   cout << ")." << endl;

   
   auto it =   l.begin();
   cout << *it << endl;
   it++;
   cout << *it << endl;
   l.push_back(7);
   l.push_back(8);
   l.push_back(9);
   cout << l.at(4) << l.at(6) << endl;
   cout << *it << endl;

   it = l.begin();
   l.insert(++it, 777);
   l.insert(++it, 787);
   it = l.begin() ;
   l.insert(it, 555);

   do {
      //l.erase(l.begin());
      l.pop_back();
      cout << ": (";
      for (auto it2 = l.begin(); it2 != l.end(); it2++) {
	 cout << *it2 << ", ";
      }
      cout  << ")."<< endl;
      
   } while(l.size() > 0) ;
   
   
}

void foo_split() {
   string line = "1234567890";
   int offset = 9;
   auto p = std::make_pair(line.substr(0, offset), line.substr(offset));
   cout << get<0>(p) << "|" << get<1>(p) << endl;
}

int main(int argc, char ** argv) {
   //foo_list();
   cout << "===================== doing create\n";
   TextBuffer buffer("00");
   TextBufferView buf(&buffer);
   buffer.dump();

   cout << "===================== doing insert simple\n"; 
   buf.insertText("X");
   buffer.dump();
   cout << "===================== doing insert leading newline\n"; 
   buf.insertText("\n11");
   buffer.dump();
   cout << "===================== doing insert 3 lines\n"; 
   buf.insertText("\n12\n34\n5: and some\n67\n89");
   buffer.dump();
   cout << "===================== doone insert\n"; 
      
   cout << buf.getPoint() << endl;
   buffer.dump();
   cout << "===================== doing insert middle\n";

   buf.setPoint(9);
   buf.insertText("####");
   buffer.dump();

   cout << "===================== doing delchar\n";
   buf.deleteChar();
   buf.deleteChar();
   buf.deleteChar();
   buf.deleteChar();
   buffer.dump();

 }
