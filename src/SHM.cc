#if !defined BUILDING_NODE_EXTENSION
	#define BUILDING_NODE_EXTENSION
#endif
#include <node.h>
#include "node_buffer.h"
#include "SHMobject.h"

using namespace node;
using namespace v8;


void GetErrMsg(const FunctionCallbackInfo<Value>& args) {

	Isolate* isolate = args.GetIsolate();
	HandleScope scope(isolate);

	int activateDebug = (args[0]->Int32Value());	//IntegerValue();
	shm_setDbg(activateDebug);

	args.GetReturnValue().Set(Exception::Error(String::NewFromUtf8(isolate, shm_err())));

}

//API:	
//	int shmid  .... int shmop_open (int key, char* flags, int mode, int size)	
//	unsigned char*  shmop_read (int shmid, int start, int length)
//	int  shmop_close (int shmid)
//	int  shmop_size (int shmid)
//	int  shmop_write (int shmid, char * data, int offset, int data_len)
//	int shmop_delete (int shmid)
//

void ShmOpen(const FunctionCallbackInfo<Value>& args) {
//	int shmid  .... int shmop_open (int key, char* flags, int mode, int size)	
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  char *flags = NULL;
  int key = args[0]->Int32Value();	//IntegerValue();
  String::Utf8Value sflags(args[1]->ToString());;
  flags = new char[(sflags.length()+1)];
  memcpy(flags,*sflags,sflags.length()+1);
  int mode = args[2]->Int32Value();	//IntegerValue();
  int size = args[3]->Int32Value();	//IntegerValue();

  int retval = shmop_open (key, flags, mode, size);
  args.GetReturnValue().Set(Number::New(isolate, retval));

}

void ShmRead(const  FunctionCallbackInfo<Value>& args) {
//	unsigned char*  shmop_read (int shmid, int start, int length)
//  HandleScope scope;
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  char *sdata;
  int shmid = args[0]->Int32Value();	//IntegerValue();
  int start = args[1]->Int32Value();	//IntegerValue();
  int length = args[2]->Int32Value();	//IntegerValue();
  
  sdata = (char *)shmop_read (shmid, start, length);
  if (sdata > 0) 
  {
	  Local<Value> b =  Encode(sdata, length, BINARY);
	  args.GetReturnValue().Set(b);
  }
  else 
  {
	 args.GetReturnValue().Set(Exception::Error(String::NewFromUtf8(isolate, "SHM read error")));
  }
}

void ShmClose(const  FunctionCallbackInfo<Value>& args) {
//	int  shmop_close (int shmid)
  Isolate* isolate = args.GetIsolate();
  //HandleScope scope;
  int shmid = args[0]->Int32Value();	//IntegerValue();
  
  shmop_close (shmid);
  args.GetReturnValue().Set(Number::New(isolate, 1));
}

void ShmSize(const FunctionCallbackInfo<Value>& args) {
//	int  shmop_size (int shmid)
  //HandleScope scope;
  Isolate* isolate = args.GetIsolate();
  int shmid = args[0]->Int32Value();	//IntegerValue();
  
  int retval = shmop_size (shmid);
  args.GetReturnValue().Set(Number::New(isolate,retval));
}

void ShmWrite(const FunctionCallbackInfo<Value>& args) {
//	int  shmop_write (int shmid, char * data, int offset, int data_len)
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  int shmid = args[0]->Int32Value();	//IntegerValue();
  //Local<String> sdata = args[1]->ToString();
  Local<Value> buffer_v = args[1];
  if (!Buffer::HasInstance(buffer_v)) {
	  args.GetReturnValue().Set(Exception::TypeError(String::NewFromUtf8(isolate, "Argument should be a buffer")));
	  return;
  }
  Local<Object> buffer_obj = buffer_v->ToObject();
  char *buffer_data = Buffer::Data(buffer_obj);
  int offset = args[2]->Int32Value();	//IntegerValue();
  int data_len = args[3]->Int32Value();	//IntegerValue();
  
  int retval = shmop_write (shmid, (unsigned char *)buffer_data, offset, data_len);
  args.GetReturnValue().Set(Number::New(isolate,retval));
}

void ShmDelete(const FunctionCallbackInfo<Value>& args) {
//	int shmop_delete (int shmid)
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  int shmid = args[0]->Int32Value();	//IntegerValue();
  
  int retval = shmop_delete (shmid);
  args.GetReturnValue().Set(Number::New(isolate,retval));
}

void Initialize(Local<Object> exports, Local<Object> module)
{
  SHMobject::Init(exports->GetIsolate());

  NODE_SET_METHOD(exports, "getErrMsg", GetErrMsg);
  NODE_SET_METHOD(exports, "openSHM", ShmOpen);
  NODE_SET_METHOD(exports, "readSHM", ShmRead);
  NODE_SET_METHOD(exports, "closeSHM", ShmClose);
  NODE_SET_METHOD(exports, "sizeSHM", ShmSize);
  NODE_SET_METHOD(exports, "writeSHM", ShmWrite);
  NODE_SET_METHOD(exports, "deleteSHM", ShmDelete);

}

NODE_MODULE(shm, Initialize)	//name of module in NODE !!! =  "shm"
