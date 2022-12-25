


bool ExecuteString(v8::Isolate* isolate, v8::Local<v8::String> source,
                   v8::Local<v8::Value> name, bool print_result,
                   bool report_exceptions);


v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const char* name);
