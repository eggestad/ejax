

#include <stdio.h>
#include <stdlib.h>
#include <display_interface.h>

/* Standard readline include files. */
#include <readline/readline.h>
#include <readline/history.h>


ejax_display_handle_t displayHandle;


/* A static variable for holding the line. */
static char *line_read = (char *)NULL;

void process_command(const char *);
/* Read a string, and return a pointer to it.
   Returns NULL on EOF. */
char *
rl_gets ()
{
  /* If the buffer has already been allocated,
     return the memory to the free pool. */
  if (line_read)
    {
      free (line_read);
      line_read = (char *)NULL;
    }

  /* Get a line from the user. */
  line_read = readline ("> ");

  /* If the line has any text in it,
     save it on the history. */
  if (line_read && *line_read)
    add_history (line_read);

  return (line_read);
}

char * getHistoryFile () {
   static char histfile[PATH_MAX]  = { 0 };
   if (histfile[0] != '\0') return histfile;
   snprintf (histfile, PATH_MAX-1,  "%s/%s", getenv("HOME"), ".ejax/shell2.hist");
   return histfile;
}
int main(int argc, char ** argv) {

   ejax_options_t ejax_options = {0};

   int rc;

   rc = ejax_init(ejax_options);
   ejax_display_t display = { 0};
   strcpy( display.drivername, "ejax commandline");
   displayHandle = ejax_register_display(display);

   const char * cline;
   while ( (cline = rl_gets() ) != NULL) {
      process_command(cline);

   }
}

