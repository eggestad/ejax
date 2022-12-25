#include <stdio.h>

#include "ejax/debug.h"


#define LOG_MSG_MAX (64*1024)
static char mesg[LOG_MSG_MAX];

static FILE * logfile = NULL;

void _mw_vlogf(int level, const char * format, va_list ap)
{

   if (logfile == NULL)  logfile = fopen("ejax-debug.log");
   
   //   int rc = vsnprintf(mesg, LOG_MSG_MAX, format, ap);
   int rc = vfprintf(logfile, format, ap);
}
   
void mwlog(int level, const char * format, ...)
{

   
   va_list ap;
   
   va_start(ap, format);
   
   _mw_vlogf(level, format, ap);
   
   va_end(ap);

  return ;
};
