#if !defined BUILDING_NODE_EXTENSION
	#define BUILDING_NODE_EXTENSION
#endif
#include <node.h>
#include "SHMobject.h"

using namespace v8;


SHMobject::~SHMobject() {};

Persistent<Function> SHMobject::constructor;

void SHMobject::Init(Isolate* isolate) {

  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "SHMobject"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(isolate, tpl->GetFunction());
}

void SHMobject::New( const v8::FunctionCallbackInfo<Value>& args) {
 
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    if (args.IsConstructCall()) {
        double value = args[0]->IsUndefined() ?
            0 : args[0]->NumberValue(context).FromMaybe(0);
        SHMobject* obj = new SHMobject(value);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
    else {
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        Local<Object> instance =
            cons->NewInstance(context, argc, argv).ToLocalChecked();
        args.GetReturnValue().Set(instance);
    }
}

void SHMobject::NewInstance( const v8::FunctionCallbackInfo<Value>& args) {

  Isolate* isolate = args.GetIsolate();

  const unsigned argc = 1;
  Local<Value> argv[argc] = { args[0] };
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();

  args.GetReturnValue().Set(instance);
}
