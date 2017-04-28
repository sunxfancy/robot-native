// main.cpp
#include <node.h>

namespace robot {

using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void MyFunction(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "hello world"));
}

void CreateFunction(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, MyFunction);
    Local<Function> fn = tpl->GetFunction();

    // omit this to make it anonymous
    fn->SetName(String::NewFromUtf8(isolate, "theFunction"));

    args.GetReturnValue().Set(fn);
}

extern void DetectFace(const FunctionCallbackInfo<Value>& args);
extern void DetectDetail(const FunctionCallbackInfo<Value>& args);

void Init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "createFunction", CreateFunction);
    NODE_SET_METHOD(exports, "detectFace", DetectFace);
    NODE_SET_METHOD(exports, "detectDetail", DetectFace);
}

NODE_MODULE(addon, Init)

}  // namespace robot