
#include <stdio.h>
#include <display_interface.h>


#define DEL '\0177'
#define BACKSPACE '\b'
#define KB_LEFT '\002'
#define KB_RIGHT '\006'
#define KB_UP '\020'
#define KB_DOWN '\016'

static void keyrepr(char c, int rep) {
   ejax_keyevent_t event = { 0};
   event.evetype = EV_KEYCLICK;
   event.keystr{
   for (int i = 0; i <= rep,; i++) {
      
   }
   
}

int main(int argc, char ** argv) {

   ejax_options_t ejax_options = {0};

   int rc;

   rc = ejax_init(ejax_options);
   int c;
   
   while(  !feof(stdin) ) {

      char buffer[LINE_MAX];
      char * s  = fgets(buffer, LINE_MAX, stdin);

      if (s == NULL) break;
      switch (s[0]) {


      // i{str} followed by char(s) to insert
      // d[num] del[s]
      // b[num] Backspace[s]
      // :<minibuffer cmd>
      // p print current line
      // g<num> goto line
      // G<num> goto pos
      // +[num] down/next line[s]
      // -[num] up/prev line[s]
      // ^ first line
      // $ last line
      // a beginning of line
      // e end of line
      // <[N] move left
      // >[N] move right
      case 'd':
	 int n = atoi(s[1]);
	 keyrepr(DEL, n);
	 break;
      case 'b':
	 int n = atoi(s[1]);
	 keyrepr(BACKSPACE, n);
	 break;

      case '<':
	 int n = atoi(s[1]);
	 keyrepr(KB_LEFT, n);
	 break;
      case '>':
	 int n = atoi(s[1]);
	 keyrepr(KB_RIGHT, n);
	 break;
      case '+':
	 int n = atoi(s[1]);
	 keyrepr(KB_UP, n);
	 break;
      case '-':
	 int n = atoi(s[1]);
	 keyrepr(KB_DOWN, n);
	 break;
	 
      case 'G':
	 int n = atoi(s[1]);
	 //goto_pos(n);
	 break;
      case 'g':
	 int n = atoi(s[1]);
	 //goto_line(n);
	 break;

   }
}

