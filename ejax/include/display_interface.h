
#ifndef _EJAX_DISPLAY_H
#define _EJAX_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif
   
#include <limits.h>
   
   
   typedef ejax_buffer_ref void *;

   void ejax_register_function(const char * fname, func_XXX_t funcptr, const char * description);
   // properties are to be considered volaitile on the driver, the core and vm shall
   // always call get when accessing the property
   void ejax_register_property(const char * prop_name, get_XXX_t getptr, set_XXX_t setptr, const chat * description);



#define BUFFER_STATUS_MODIFIED         (1 <<  0);
#define BUFFER_STATUS_READ_ONLY        (1 <<  1);
#define BUFFER_STATUS_OVERWRITE        (1 <<  2);
   
#define BUFFER_STATUS_MODE_CHANGED     (1 << 10);

#define BUFFER_STATUS_VM_BUSY          (1 << 21);
   
   
   typedef struct {
      
      char drivername[NAME_MAX];
      short ver[3];
      
      void (*buffer_changed) ( ejax_buffer_ref new_buffer , char * filename);
      void (*buffer_context_changed) ( int64_t offset, int64_t num_char_changed, int64_t new_length );
      void (*buffer_status_changed)  ( int64_t flags, char * status_line );
   
   } ejax_display_t;
   
   extern ejax_display_t * display_initialize(ejax_buffer_ref buf_ref);
   
   
#ifdef __cplusplus
}
#endif

#endif
