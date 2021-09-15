#if !defined BUILDING_NODE_EXTENSION
	#define BUILDING_NODE_EXTENSION
#endif
#ifndef SHMOBJECT_H
#define SHMOBJECT_H

#include <node.h>
#include <string.h>
#include <node_object_wrap.h>

using node::ObjectWrap;
using v8::Object;
using v8::Local;
using v8::Isolate;
using v8::Persistent;
using v8::Value;
using v8::Function;
using v8::FunctionCallbackInfo;

extern "C" {
	//SHARED MEMORY
	char *shm_err();
	void shm_setDbg( int on );
	
	void shmop_init();
	void shmop_deinit();
	int shmop_open (int key, char* flags, int mode, int size);
	unsigned char* shmop_read (int shmid, int start, int count);
	void shmop_close (int shmid);
	int shmop_size (int shmid);
	int shmop_write (int shmid, unsigned char * data, int offset, int data_len);
	int shmop_delete (int shmid);
}

class SHMobject : public node::ObjectWrap {
 public:
  static void Init(Isolate* isolate);
  static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);
  double Val() const { return val_; }

 private:
	 explicit SHMobject(double val = 0) { val_ = val; };
  ~SHMobject();

  static Persistent<Function> constructor;

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  double val_;
};

#endif
