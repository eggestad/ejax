#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>

#include "ejax/debug.h"


#define LOG_MSG_MAX (64*1024)
static char mesg[LOG_MSG_MAX];

static FILE * logfile = NULL;

static int
sptime(char *b, int max)
{
  int rc;
  struct timeval tv;
  struct tm * now;
  if (b == NULL) return  -1;

  gettimeofday(&tv,NULL);
  struct tm localtm;
  now = localtime_r((time_t *) &tv.tv_sec, &localtm);
  
  rc = snprintf(b, max, "%4d%2.2d%2.2d %2.2d%2.2d%2.2d.%3.3ld ",
		now->tm_year+1900, now->tm_mon+1, now->tm_mday,
		now->tm_hour, now->tm_min, now->tm_sec, tv.tv_usec/1000);
  return rc;
}

static const char *levelheader[] = { "FATAL: ", 
			       "ERROR: ", 
			       "WARN : ", 
			       "ALERT: ", 
			       "Info : ", 
			       "Debug: ", 
			       "Dbg1 : ", 
			       "Dbg2 : ", 
			       "Dbg3 : ", 
			       "Dbg4 : ", 
			       NULL };

static void _vlogf(int level, const char * format, va_list ap)
{
   char timestamp[40];
   int rc = sptime(timestamp, 40);   
   
   if (logfile == NULL)  logfile = fopen("ejax-debug.log", "w");

   fprintf(logfile, "%s %s", levelheader[level], timestamp);

   //   int rc = vsnprintf(mesg, LOG_MSG_MAX, format, ap);
   rc = vfprintf(logfile, format, ap);
   fprintf(logfile, "\n");
   fflush(logfile);
}
   
void ejaxlog(int level, const char * format, ...)
{

   
   va_list ap;
   
   va_start(ap, format);
   
   _vlogf(level, format, ap);
   
   va_end(ap);

  return ;
};
