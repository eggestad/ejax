

#include "text_buffer_view.h"
#include "text_buffer.h"

using namespace std;

namespace ejax {


   void TextBufferView::insertText(string str) {
      this->buffer->insertText(point, str);
   }

   void TextBufferView::deleteChar() {
      this->buffer->deleteChar(point);
   }

   void TextBufferView::backSpace() {
      this->buffer->deleteChar(point-1);
      point--;
   }

   Point TextBufferView::getPoint() {
	   return this->point;
   }
   Point TextBufferView::getPointMin() {
	   return 1;
   }
   Point TextBufferView::getPointMax() {
	   return this->buffer->getSize()+1;
   }

   Point TextBufferView::setPoint(Point  newPoint) {
      auto size = buffer->getSize();
      if (newPoint > size-1) newPoint = size-1;
      this->point = newPoint;
      long lnum= 1;
      for (TextBufferLine bl : buffer->lines) {
         if (point >= bl.pos and point <= bl.pos + bl.line.size() + 1) currentLine = lnum;
         lnum++;
      }
      return this->point;
   }
}
