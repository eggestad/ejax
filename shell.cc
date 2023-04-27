// Copyright 2012 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <assert.h>
#include <fcntl.h>
#include <include/libplatform/libplatform.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ulimit.h>


/* Standard readline include files. */
#include <readline/readline.h>
#include <readline/history.h>

#ifdef VER_8_2
#include "include/v8.h"
#else
#include "include/v8-context.h"
#include "include/v8-exception.h"
#include "include/v8-initialization.h"
#include "include/v8-isolate.h"
#include "include/v8-local-handle.h"
#include "include/v8-script.h"
#include "include/v8-template.h"
#endif
#include "src/debug/interface-types.h"
#include "src/debug/debug-interface.h"

#include <iostream>

#include "base-functions.h"
#include "shell.h"
#include "utils.h"

#include "ejax/include/ejax/logging.h"

/**
 * This sample program shows how to implement a simple javascript shell
 * based on V8.  This includes initializing V8 with command line options,
 * creating global functions, compiling and executing strings.
 *
 * For a more sophisticated shell, consider using the debug shell D8.
 */

using namespace std;

v8::Local<v8::Context> CreateShellContext(v8::Isolate* isolate);
void RunShell(v8::Local<v8::Context> context, v8::Platform* platform);
int RunMain(v8::Isolate* isolate, v8::Platform* platform, int argc,
            char* argv[]);

void ReportException(v8::Isolate* isolate, v8::TryCatch* handler);


static bool run_shell;

   


int main(int argc, char* argv[]) {
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();
  v8::V8::SetFlagsFromCommandLine(&argc, argv, true);
  v8::Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate* isolate = v8::Isolate::New(create_params);
  run_shell = (argc == 1);
  int result;
  {
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = CreateShellContext(isolate);
    if (context.IsEmpty()) {
      fprintf(stderr, "Error creating context\n");
      return 1;
    }
    v8::Context::Scope context_scope(context);
    result = RunMain(isolate, platform.get(), argc, argv);

    printf("past runmain rc =%d\n", result);

    
  v8::Local<v8::String> file_name =
     v8::String::NewFromUtf8Literal(isolate, "unnamed");
  // Create a string containing the JavaScript source code.
  v8::Local<v8::String>  source ;
  // "function foo() {}; console.info('hei x hei'); var match = 0;if(arguments[0] == arguments[1]) { match = 1; } return match; }"
  source = v8::String::NewFromUtf8Literal(isolate, "function foo() { console.log('hei x hei'); var match = 0;if(arguments[0] == arguments[1]) { match = 1; } return match; };"	 );
  
  bool success = ExecuteString(isolate, source, file_name, false, true);
   
    if (run_shell) RunShell(context, platform.get());
  }
  isolate->Dispose();
  v8::V8::Dispose();
#ifdef VER_8_2
  v8::V8::ShutdownPlatform();
#else
  v8::V8::DisposePlatform();
#endif
  delete create_params.array_buffer_allocator;
  return result;
}


class MyConsole : public v8::debug::ConsoleDelegate {

   v8::Isolate *isolate;
   
public:

   MyConsole(v8::Isolate *isolate) {
      this->isolate = isolate;
   }
   virtual void Log(const v8::debug::ConsoleCallArguments& args,
		    const v8::debug::ConsoleContext& context) {
      //      printf("in log\n");
      bool first = true;
      for (int i = 0; i < args.Length(); i++) {
	 v8::HandleScope handle_scope(isolate);
	 if (first) {
	    first = false;
	 } else {
	    printf(" ");
	 }
	 v8::String::Utf8Value str(isolate, args[i]);
	 const char* cstr = ToCString(str);
	 printf("%s", cstr);
      }
      printf("\n");
      fflush(stdout);

   }
};

void GetPoint0(const v8::FunctionCallbackInfo<v8::Value>& args) {
  args.GetReturnValue().Set(
      v8::String::NewFromUtf8(args.GetIsolate(), v8::V8::GetVersion())
          .ToLocalChecked());
}

void GetPoint(const v8::FunctionCallbackInfo<v8::Value>& args) {
  args.GetReturnValue().Set(
      v8::String::NewFromUtf8(args.GetIsolate(), "point is at 0")
          .ToLocalChecked());

  v8::Isolate * isolate = args.GetIsolate();
  v8::Local< v8::Context > context = isolate->GetCurrentContext ();

  v8::Handle<v8::Object> global = context->Global();
  v8::Local< v8::String> key =v8::String::NewFromUtf8(isolate, "foo").ToLocalChecked();

  v8::Local<v8::Value> foo_value = global->Get(context, key).ToLocalChecked();;



  //v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(foo_value);
  //  printf("::%d\n", foo_value.As<v8::Object>()->IsEmpty());

  if (foo_value->IsFunction()) {

  } else {
     Error( "::foo is not a function");
  }

  //v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
  //v8::Handle<v8::Value> fargs[2];

  
  //  int final_result;
  
  //  fargs[0] = v8::String::NewFromUtf8(isolate,"1");
  //  fargs[1] = v8::String::NewFromUtf8(isolate,"2");

  
  //  js_result

  //  v8::MaybeLocal<v8::Value> js_result = func->Call(context, global, 0, nullptr);
  v8::MaybeLocal<v8::Value> js_result = foo_value.As<v8::Object>()->CallAsFunction(context, context->Global(), 0, nullptr);

  
  //  v8::String::Utf8Value utf8Value(isolate, js_result);
  //cout << utf8Value << endl;
  /*
  if (js_result.IsEmpty() ) {
     args.GetReturnValue().Set(
			       v8::String::NewFromUtf8(isolate,"ok")
			       .ToLocalChecked());
  } else {
     v8::String::Utf8Value ascii(isolate, js_result.ToLocalChecked());  
     int final_result = atoi(*ascii);
     args.GetReturnValue().Set(js_result.ToLocalChecked());
     }*/
  args.GetReturnValue().Set(
      v8::String::NewFromUtf8(args.GetIsolate(), v8::V8::GetVersion())
          .ToLocalChecked());
}


// Creates a new execution environment containing the built-in
// functions.
v8::Local<v8::Context> CreateShellContext(v8::Isolate* isolate) {
   Info( "creating context: %s:%d", __FILE__, __LINE__);



   // Create a template for the global object.
   v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

   addBaseFunctions(isolate, global);
   
   global->Set(v8::String::NewFromUtf8Literal(isolate, "getPoint"),
	       v8::FunctionTemplate::New(isolate, GetPoint));

   global->Set(v8::String::NewFromUtf8Literal(isolate, "x"),
	       v8::FunctionTemplate::New(isolate, GetPoint));

   MyConsole * myConsole = new MyConsole(isolate);

   v8::debug::SetConsoleDelegate(isolate, myConsole);

   v8::Local<v8::ObjectTemplate> ejax = v8::ObjectTemplate::New(isolate);

   ejax->Set(isolate, "getPoint", v8::FunctionTemplate::New(isolate, GetPoint));

   global->Set(isolate, "ejax", ejax);
  
   auto context = v8::Context::New(isolate, NULL, global);

   //v8::Local<v8::Value> name = v8::String::NewFromUtf8Literal(isolate, "(origin)");
   //v8::ScriptOrigin origin( name);
  
   // Compile the source code.
   //script = v8::Script::Compile(context, source, &origin).ToLocalChecked();

   // Run the script to get the result.
   //result = script->Run(context).ToLocalChecked();
   return context;
}




// Process remaining command line arguments and execute files
int RunMain(v8::Isolate* isolate, v8::Platform* platform, int argc,
            char* argv[]) {
   Info( "RunMain start");
  for (int i = 1; i < argc; i++) {
    const char* str = argv[i];
    if (strcmp(str, "--shell") == 0) {
      run_shell = true;
    } else if (strcmp(str, "-f") == 0) {
      // Ignore any -f flags for compatibility with the other stand-
      // alone JavaScript engines.
      continue;
    } else if (strncmp(str, "--", 2) == 0) {
      fprintf(stderr,
              "Warning: unknown flag %s.\nTry --help for options\n", str);
    } else if (strcmp(str, "-e") == 0 && i + 1 < argc) {
      // Execute argument given to -e option directly.
      v8::Local<v8::String> file_name =
          v8::String::NewFromUtf8Literal(isolate, "unnamed");
      v8::Local<v8::String> source;
      if (!v8::String::NewFromUtf8(isolate, argv[++i]).ToLocal(&source)) {
        return 1;
      }
      bool success = ExecuteString(isolate, source, file_name, false, true);
      while (v8::platform::PumpMessageLoop(platform, isolate)) continue;
      if (!success) return 1;
    } else {
      // Use all other arguments as names of files to load and run.
      v8::Local<v8::String> file_name =
          v8::String::NewFromUtf8(isolate, str).ToLocalChecked();
      v8::Local<v8::String> source;
      if (!ReadFile(isolate, str).ToLocal(&source)) {
        fprintf(stderr, "Error reading '%s'\n", str);
        continue;
      }
      bool success = ExecuteString(isolate, source, file_name, false, true);
      while (auto rx = v8::platform::PumpMessageLoop(platform, isolate)) {
	 printf("33\n");
	 continue;
      }
      if (!success) return 1;
    }
  }
  return 0;
}
 	
/* A static variable for holding the line. */
static char *line_read = (char *)NULL;

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

// The read-eval-execute loop of the shell.
void RunShell(v8::Local<v8::Context> context, v8::Platform* platform) {
   Info( "RunShell V8 version %s [sample shell]", v8::V8::GetVersion());
  static const int kBufferSize = 256;
  // Enter the execution environment before evaluating any code.
  v8::Context::Scope context_scope(context);
  v8::Local<v8::String> name(
      v8::String::NewFromUtf8Literal(context->GetIsolate(), "(shell)"));
  read_history(getHistoryFile());

  
  Info( "RunShell Starting main loop");
  while (true) {
     //cout << "shell loop begin\n";
     //    char buffer[kBufferSize];
     //fprintf(stderr, "> ");
     char* str = rl_gets() ; //fgets(buffer, kBufferSize, stdin);
     if (str == NULL) break;
     write_history(getHistoryFile());
     v8::HandleScope handle_scope(context->GetIsolate());
     ExecuteString(
		   context->GetIsolate(),
		   v8::String::NewFromUtf8(context->GetIsolate(), str).ToLocalChecked(),
		   name, true, true);
     do {
	auto rc = v8::platform::PumpMessageLoop(platform, context->GetIsolate()) ;
	//cout << "pump rc " << rc << "\n";
	
	if (!rc) break;
     } while(true);
  }
}


// Executes a string within the current v8 context.
bool ExecuteString(v8::Isolate* isolate, v8::Local<v8::String> source,
                   v8::Local<v8::Value> name, bool print_result,
                   bool report_exceptions) {
  v8::HandleScope handle_scope(isolate);
  v8::TryCatch try_catch(isolate);
  v8::ScriptOrigin origin(isolate, name);
  v8::Local<v8::Context> context(isolate->GetCurrentContext());
  v8::Local<v8::Script> script;
  if (!v8::Script::Compile(context, source, &origin).ToLocal(&script)) {
    // Print errors that happened during compilation.
    if (report_exceptions)
      ReportException(isolate, &try_catch);
    return false;
  } else {
    v8::Local<v8::Value> result;
    if (!script->Run(context).ToLocal(&result)) {
      assert(try_catch.HasCaught());
      // Print errors that happened during execution.
      if (report_exceptions)
        ReportException(isolate, &try_catch);
      return false;
    } else {
      assert(!try_catch.HasCaught());
      if (print_result && !result->IsUndefined()) {
        // If all went well and the result wasn't undefined then print
        // the returned value.
        v8::String::Utf8Value str(isolate, result);
        const char* cstr = ToCString(str);
        DEBUG("%s", cstr);
	printf("%s\n", cstr);
      }
      if (print_result && result->IsUndefined()) {
	 printf("(undefined)\n");
      }

      return true;
    }
  }
}


void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch) {
  v8::HandleScope handle_scope(isolate);
  v8::String::Utf8Value exception(isolate, try_catch->Exception());
  const char* exception_string = ToCString(exception);
  v8::Local<v8::Message> message = try_catch->Message();
  if (message.IsEmpty()) {
    // V8 didn't provide any extra information about this error; just
    // print the exception.
    fprintf(stderr, "%s\n", exception_string);
  } else {
    // Print (filename):(line number): (message).
    v8::String::Utf8Value filename(isolate,
                                   message->GetScriptOrigin().ResourceName());
    v8::Local<v8::Context> context(isolate->GetCurrentContext());
    const char* filename_string = ToCString(filename);
    int linenum = message->GetLineNumber(context).FromJust();
    fprintf(stderr, "%s:%i: %s\n", filename_string, linenum, exception_string);
    // Print line of source code.
    v8::String::Utf8Value sourceline(
        isolate, message->GetSourceLine(context).ToLocalChecked());
    const char* sourceline_string = ToCString(sourceline);
    fprintf(stderr, "%s\n", sourceline_string);
    // Print wavy underline (GetUnderline is deprecated).
    int start = message->GetStartColumn(context).FromJust();
    for (int i = 0; i < start; i++) {
      fprintf(stderr, " ");
    }
    int end = message->GetEndColumn(context).FromJust();
    for (int i = start; i < end; i++) {
      fprintf(stderr, "^");
    }
    fprintf(stderr, "\n");
    v8::Local<v8::Value> stack_trace_string;
    if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
        stack_trace_string->IsString() &&
        stack_trace_string.As<v8::String>()->Length() > 0) {
      v8::String::Utf8Value stack_trace(isolate, stack_trace_string);
      const char* err = ToCString(stack_trace);
      fprintf(stderr, "%s\n", err);
    }
  }
}
