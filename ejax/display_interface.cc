#include <display_interface.h>
#include <iostream>

using namespace std;
//using namespace ejax;

int ejax_init(ejax_options_t options) {
   cout << "ejax init " << endl;

   return 0;
}

ejax_display_handle_t  ejax_register_display(ejax_display_t display) {

   cout << "ejax register display " << endl;

   return 99;
}



void ejax_send_key_event( ejax_keyevent_t  event ) {


   cout << "got key event" << endl;
}



