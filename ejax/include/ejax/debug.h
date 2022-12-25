#ifndef _EJAX_DEBUG_H



#define _EJAX_DEBUG_H



#ifdef __GNUC__
/* gcc hack in order to get wrong arg type in mwlog() */
#define FORMAT_PRINTF __attribute__ ((format (printf, 2, 3)))
#else
#define FORMAT_PRINTF 
#endif

#ifdef	__cplusplus
extern "C" {
#endif
   
   
   void ejaxlog(int level, const char * format, ...) FORMAT_PRINTF;
   


#ifdef	__cplusplus
}
#endif
#endif //_EJAX_DEBUG_H
