//
// Created by terje on 7/11/23.
//
#include <stdio.h>
#include <stdlib.h>

#include <display_interface.h>

#define DEL '\177'
#define BACKSPACE '\b'
#define KB_LEFT '\002'
#define KB_RIGHT '\006'
#define KB_UP '\020'
#define KB_DOWN '\016'

extern ejax_display_handle_t displayHandle;


static void keyrepr(wchar_t  c, int rep) {
   ejax_keyevent_t event = { 0};
   event.evtype = EV_KEYCLICK;
   event.keystr[0] = c;


   for (int i = 0; i <= rep; i++) {
      printf("send key %");
      ejax_send_key_event(displayHandle, event);
   }

}
void process_command(const char * s) {

   int n ;
   const char * arg = s+1;

   if (s == NULL) return;
   switch (s[0]) {


         // i{str} followed by char(s) to insert
         // d[num] del[s]
         // b[num] Backspace[s]
         // n  insert newline
         // $ insert newline
         // :<minibuffer cmd>
         // p print current buffer line
         // g<num> goto line
         // #<num> goto pos
         // #[num]<command> // execute i/d/b/n/$ at pos

         // +[num] down/next line[s]
         // -[num] up/prev line[s]
         // ^ first line
         // $ last line
         // a beginning of line
         // e end of line
         // <[N] move left
         // >[N] move right

         // s save
         // o<filename> open

      case 'd':
         n = atoi(arg);
         keyrepr(DEL, n);
         break;
      case 'b':
         n = atoi(arg);
         keyrepr(BACKSPACE, n);
         break;

      case '<':
         n = atoi(arg);
         keyrepr(KB_LEFT, n);
         break;
      case '>':
         n = atoi(arg);
         keyrepr(KB_RIGHT, n);
         break;
      case '+':
         n = atoi(arg);
         keyrepr(KB_UP, n);
         break;
      case '-':
         n = atoi(arg);
         keyrepr(KB_DOWN, n);
         break;

      case 'G':
         n = atoi(arg);
         //goto_pos(n);
         break;
      case 'g':
         n = atoi(arg);
         //goto_line(n);
         break;
   }
}
