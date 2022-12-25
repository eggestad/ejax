
const char* ToCString(const v8::String::Utf8Value& value) ;


v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const char* name);
