
#include "include/v8.h"
#include "shell.h"
#include "utils.h"

// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
void Print(const v8::FunctionCallbackInfo<v8::Value>& args) {
   bool first = true;
   for (int i = 0; i < args.Length(); i++) {
      v8::HandleScope handle_scope(args.GetIsolate());
      if (first) {
	 first = false;
      } else {
	 printf(" ");
      }
      v8::String::Utf8Value str(args.GetIsolate(), args[i]);
      const char* cstr = ToCString(str);
      printf("%s", cstr);
      printf(" hdl: %d\n", v8::HandleScope::NumberOfHandles (args.GetIsolate()));

   }
   printf("\n");
   printf(" hdl: %d\n", v8::HandleScope::NumberOfHandles (args.GetIsolate()));
   fflush(stdout);
}


// The callback that is invoked by v8 whenever the JavaScript 'read'
// function is called.  This function loads the content of the file named in
// the argument into a JavaScript string.
void Read(const v8::FunctionCallbackInfo<v8::Value>& args) {
   if (args.Length() != 1) {
      args.GetIsolate()->ThrowException(
					v8::String::NewFromUtf8Literal(args.GetIsolate(), "Bad parameters"));
      return;
   }
   v8::String::Utf8Value file(args.GetIsolate(), args[0]);
   if (*file == NULL) {
      args.GetIsolate()->ThrowException(v8::String::NewFromUtf8Literal(
								       args.GetIsolate(), "Error loading file"));
      return;
   }
   v8::Local<v8::String> source;
   if (!ReadFile(args.GetIsolate(), *file).ToLocal(&source)) {
      args.GetIsolate()->ThrowException(v8::String::NewFromUtf8Literal(
								       args.GetIsolate(), "Error loading file"));
      return;
   }

   args.GetReturnValue().Set(source);
}



// The callback that is invoked by v8 whenever the JavaScript 'load'
// function is called.  Loads, compiles and executes its argument
// JavaScript file.
void Load(const v8::FunctionCallbackInfo<v8::Value>& args) {
   for (int i = 0; i < args.Length(); i++) {
      v8::HandleScope handle_scope(args.GetIsolate());
      v8::String::Utf8Value file(args.GetIsolate(), args[i]);
      if (*file == NULL) {
	 args.GetIsolate()->ThrowException(v8::String::NewFromUtf8Literal(
									  args.GetIsolate(), "Error loading file"));
	 return;
      }
      v8::Local<v8::String> source;
      if (!ReadFile(args.GetIsolate(), *file).ToLocal(&source)) {
	 args.GetIsolate()->ThrowException(v8::String::NewFromUtf8Literal(
									  args.GetIsolate(), "Error loading file"));
	 return;
      }
      if (!ExecuteString(args.GetIsolate(), source, args[i], false, false)) {
	 args.GetIsolate()->ThrowException(v8::String::NewFromUtf8Literal(
									  args.GetIsolate(), "Error executing file"));
	 return;
      }
   }
}


// The callback that is invoked by v8 whenever the JavaScript 'quit'
// function is called.  Quits.
void Quit(const v8::FunctionCallbackInfo<v8::Value>& args) {
   // If not arguments are given args[0] will yield undefined which
   // converts to the integer value 0.
   int exit_code =
      args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
   fflush(stdout);
   fflush(stderr);
   exit(exit_code);
}


void Version(const v8::FunctionCallbackInfo<v8::Value>& args) {
   args.GetReturnValue().Set(
			     v8::String::NewFromUtf8(args.GetIsolate(), v8::V8::GetVersion())
			     .ToLocalChecked());
}

void addBaseFunctions(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> global ) {

   // Bind the global 'print' function to the C++ Print callback.
   global->Set(v8::String::NewFromUtf8Literal(isolate, "print"),
	       v8::FunctionTemplate::New(isolate, Print));
   // Bind the global 'read' function to the C++ Read callback.
   global->Set(v8::String::NewFromUtf8Literal(isolate, "read"),
	       v8::FunctionTemplate::New(isolate, Read));
   // Bind the global 'load' function to the C++ Load callback.
   global->Set(v8::String::NewFromUtf8Literal(isolate, "load"),
	       v8::FunctionTemplate::New(isolate, Load));
   // Bind the 'quit' function
   global->Set(v8::String::NewFromUtf8Literal(isolate, "quit"),
	       v8::FunctionTemplate::New(isolate, Quit));
   // Bind the 'version' function
   global->Set(v8::String::NewFromUtf8Literal(isolate, "version"),
	       v8::FunctionTemplate::New(isolate, Version));
}
