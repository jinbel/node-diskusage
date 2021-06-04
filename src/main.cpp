#include "diskusage.h"

#include <napi.h>
#include <stdexcept>
#include <sstream>

Napi::Object convertDiskUsage(const Napi::CallbackInfo& info, const DiskUsage& usage) {
  Napi::Env env = info.Env();
  Napi::Object obj = Napi::Object::New(env);
  obj.Set(Napi::String::New(env, "available"), Napi::Number::New(env, usage.available));
  obj.Set(Napi::String::New(env, "free"), Napi::Number::New(env, usage.free));
  obj.Set(Napi::String::New(env, "total"), Napi::Number::New(env, usage.total));

  return obj;
}

std::string convertSystemError(const SystemError& error)
{
  std::stringstream ss;
  ss << error.syscall() << error.message() << error.path() << std::string(strerror(errno));
  std::string msg;
  ss >> msg;
  return msg;
}


Napi::Value getDiskUsage(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
      Napi::TypeError::New(env, "Wrong number of arguments")
          .ThrowAsJavaScriptException();
      return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string path = info[0].As<Napi::String>().Utf8Value();


  try {
    DiskUsage result = GetDiskUsage(path.c_str());
    return convertDiskUsage(info, result);
  }
  catch (const SystemError &error) {
    Napi::Error::New(env, convertSystemError(error)).ThrowAsJavaScriptException();
  }
  catch (const std::exception &ex) {
    Napi::Error::New(env, ex.what()).ThrowAsJavaScriptException();
  }
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "getDiskUsage"),
              Napi::Function::New(env, getDiskUsage));
  return exports;
}

NODE_API_MODULE(diskusage, Init)
