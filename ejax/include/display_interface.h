
#ifndef _EJAX_DISPLAY_H
#define _EJAX_DISPLAY_H

#include <limits.h>
#include <stdint.h>

#define EV_KEYPRESS    0x1
#define EV_KEYRELEASE  0x2
#define EV_KEYCLICK    0x10
#define EV_KEYDLBCLICK 0x11
#define EV_KEYTRICLICK 0x12

#define EV_MOUSE_MOVE        0x100
#define EV_MOUSE_BTNPRESS    0x201
#define EV_MOUSE_BTNRELEASE  0x202
#define EV_MOUSE_BTNCLICK    0x210
#define EV_MOUSE_BTNDBLCLICK 0x211
#define EV_MOUSE_BTNTRICLICK 0x213


#define BUFFER_STATUS_MODIFIED         (1 <<  0);
#define BUFFER_STATUS_READ_ONLY        (1 <<  1);
#define BUFFER_STATUS_OVERWRITE        (1 <<  2);
   
#define BUFFER_STATUS_MODE_CHANGED     (1 << 10);

#define BUFFER_STATUS_VM_BUSY          (1 << 21);


#ifdef __cplusplus
extern "C" {
#endif

   
   typedef struct {
      
      char drivername[NAME_MAX];
      short ver[3];
      
      //void (*buffer_changed) ( ejax_buffer_ref new_buffer , const char * filename);
      void (*buffer_context_changed) ( int64_t offset, int64_t num_char_changed, int64_t new_length );
      void (*buffer_view_changed) ( int64_t line, int64_t num_line_changed, int64_t new_total_lines );

      void (*buffer_status_changed)  ( int64_t flags, const char * status_line );
      void (*vm_busy)(void);
      void (*vm_ready)(void);
      void (*vm_exception)(const char *);

      void (*buffer_dirty)(void);
      void (*buffer_saved)(void);

      void (*minibuffer_focus)(const char * text);
      void (*minibuffer_update)(const char * text);
      //      void (*minibuffer_focus)(void);
      
   } ejax_display_t;

   typedef uint64_t ejax_display_handle_t;
   
   ejax_display_handle_t ejax_register_display(ejax_display_t display) ;
   
   //   extern ejax_display_t * display_initialize(ejax_buffer_ref buf_ref);
   
   
   typedef struct {
      int use_subprocess;

   } ejax_options_t;


   int ejax_init(ejax_options_t options);


   
   typedef struct  {
      int evtype;
      int keycode;
      int keysym;
      char keystr[8]; // this may be UTF-8
   } ejax_keyevent_t ;

   
   typedef struct  {
      int evtype;
      int btn_number;
      long x;
      long y;
   } ejax_mouseevent_t ;

   void ejax_send_key_event( ejax_display_handle_t, ejax_keyevent_t  );
   void ejax_send_mouse_event( ejax_display_handle_t, ejax_keyevent_t  );
   


   
   //   typedef ejax_buffer_ref void *;

   //   void ejax_register_function(const char * fname, func_XXX_t funcptr, const char * description);
   // properties are to be considered volaitile on the driver, the core and vm shall
   // always call get when accessing the property
   //void ejax_register_property(const char * prop_name, get_XXX_t getptr, set_XXX_t setptr, const chat * description);



   
#ifdef __cplusplus
}
#endif

#endif
