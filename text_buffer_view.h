
#ifndef TEXT_BUFFER_VIEW_H
#define TEXT_BUFFER_VIEW_H

typedef unsigned long Point;

namespace ejax {


   class TextBufferView {

      TextBuffer buffer;
      Point point;
      long currentLine;


   public:

   TextBufferView( TextBuffer buf): point(0) {
	 buffer = buf;
	 currentLine = 1;
      };

       
      void insertText(std::string str);
      //void insertFullLine(std::string str, ulong linenum);
      void insertChar(char c);
      void deleteChar();
      void backspace();
      Point getPoint();
      Point setPoint(Point abspos);
      void movePoint(long rel);
   };

   typedef struct TextBufferView TextBufferView;
}

#endif  // TEXT_BUFFER_VIEW_H
