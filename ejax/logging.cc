#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <sys/time.h>

#define USE_GLIB_LOG

#include "ejax/logging.h"


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



#ifdef USE_GLIB_LOG

#define KEY_PRIORITY "PRIORITY"
#define KEY_CODE_FILE "CODE_FILE"
#define KEY_CODE_LINE "CODE_LINE"
#define KEY_CODE_FUNC "CODE_FUNC"
#define KEY_MESSAGE "MESSAGE"
#define KEY_GLIB_DOMAIN "GLIB_DOMAIN"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int unix_socket = -1;


extern "C" GLogWriterOutput my_log_writer (GLogLevelFlags log_level,
				const GLogField *fields,
				gsize n_fields,
				gpointer user_data) {

   // run: while [ true ]; do rm logsocket; nc -l -U logsocket ; done
   // in a window to get 
   if (unix_socket < 0) {
      unix_socket = socket(AF_UNIX, SOCK_STREAM, 0);
      struct sockaddr_un unaddr;
      unaddr.sun_family = AF_UNIX;
      strcpy(unaddr.sun_path, "/home/terje/logsocket");
      
      int rc = connect( unix_socket, (const struct sockaddr *) &unaddr, sizeof(struct sockaddr_un));
      if (rc != 0) {
	 
	 close(unix_socket);
	 unix_socket = -1;
      }
   }


   //printf("  %d:: \n", n_fields);
   const char * loglevelstr = "N/A";
   const char * filenamestr = "N/A";
   const char *  linenumstr = "N/A";
   const char * functionstr = "N/A";
   const char * msg = "N/A";
   
   for ( int i = 0; i < n_fields; i++) {
      const char * key = fields[i].key;
      const char * value = (const char*) fields[i].value;
      
      //printf("KV: [%s]: %s (%d) \n", key, value, fields[i].length);
      if (g_strcmp0(key, KEY_PRIORITY) == 0) {
	 if (g_strcmp0(value, "7") == 0) loglevelstr = "DEBUG";
	 if (g_strcmp0(value, "6") == 0) loglevelstr = "INFO ";	 
	 if (g_strcmp0(value, "5") == 0) loglevelstr = " MSG ";
	 if (g_strcmp0(value, "4") == 0) loglevelstr = "WARN ";	 
	 if (g_strcmp0(value, "3") == 0) loglevelstr = "CRITI";
	 if (g_strcmp0(value, "2") == 0) loglevelstr = "ERROR";	 
      };
      if (g_strcmp0(key, KEY_CODE_FILE) == 0) filenamestr = value;
      if (g_strcmp0(key, KEY_CODE_LINE) == 0) linenumstr = value;
      if (g_strcmp0(key, KEY_CODE_FUNC) == 0) functionstr = value;
      //      if (strcmp(key, 
      if (g_strcmp0(key, KEY_MESSAGE) == 0) msg = value;
   }
   

   size_t maxlen = strlen(msg) + 200;
   {
      char buffer[maxlen];
      GDateTime *dt = g_date_time_new_now_local();
      int l = snprintf(buffer, maxlen, "%s [%s] %s:%s (%s) - %s \n",
	       g_date_time_format(dt, "%F %T:%f"),
	       loglevelstr,
	       filenamestr, linenumstr, functionstr,
	       msg);
      g_date_time_unref(dt);
      fputs(buffer, stderr);
      //printf("maxlen %d l %d \n", maxlen, l);
      if (unix_socket > -1) write(unix_socket, buffer, l+1);
      
   }
      
   return G_LOG_WRITER_UNHANDLED;
};

extern "C" void ejax_log_init() {
   g_log_set_writer_func(my_log_writer, NULL, NULL);
};
#else
extern "C" void ejax_log_init() {};

#endif
