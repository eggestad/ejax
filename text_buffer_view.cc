

#include "text_buffer_view.h"
#include "text_buffer.h"

using namespace std;

namespace ejax {

   namespace {

    
      void TextBufferView::insertText(string str) {
	 this.buffer.InsertText(point, str);
      }

      void TextBuffer::deleteChar() {
	 this.buffer.deleteChar(point);
      }

      void TextBuffer::backSpace() {
	 this.buffer.deleteChar(point-1);
	 point--;
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
}
