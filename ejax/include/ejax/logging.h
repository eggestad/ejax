#ifndef _EJAX_DEBUG_H



#define _EJAX_DEBUG_H

#define EJAXLOG_FATAL     0
#define EJAXLOG_ERROR     1
#define EJAXLOG_WARNING   2
#define EJAXLOG_ALERT     3
#define EJAXLOG_INFO      4
#define EJAXLOG_DEBUG     5
#define EJAXLOG_DEBUG1    6
#define EJAXLOG_DEBUG2    7
#define EJAXLOG_DEBUG3    8
#define EJAXLOG_DEBUG4    9

#ifdef __GNUC__
/* gcc hack in order to get wrong arg type in mwlog() */
#define FORMAT_PRINTF __attribute__ ((format (printf, 2, 3)))
#else
#define FORMAT_PRINTF 
#endif

#define DEBUG(m...)    ejaxlog(EJAXLOG_DEBUG, m)
/** Info level log. */
#define Info(m...)    ejaxlog(EJAXLOG_INFO, m)
/** Warning level log */
#define Warning(m...) ejaxlog(EJAXLOG_WARNING, m)
/** Error level log */
#define Error(m...)   ejaxlog(EJAXLOG_ERROR, m)

/** Fatal leve log and call abort() */
#define Fatal(m...)   do { ejaxlog(EJAXLOG_FATAL, m); abort(); } while (0)

#ifdef	__cplusplus
extern "C" {
#endif
   
   
   void ejaxlog(int level, const char * format, ...) FORMAT_PRINTF;
   


#ifdef	__cplusplus
}
#endif
#endif //_EJAX_DEBUG_H
