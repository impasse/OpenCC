#include "../opencc.h"
#include "jni.h"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

const char* className = "com/mokahr/neon/engine/OpenCC";

class OpenCC {
private:
  opencc_t instance;

public:
  OpenCC(const char* configFileName) { instance = opencc_open(configFileName); }

  ~OpenCC() { opencc_close(instance); }

  string convert(const char* input, size_t length) {
    char* tmp = opencc_convert_utf8(instance, input, length);
    string str = string(tmp);
    opencc_convert_utf8_free(tmp);
    return str;
  }
};


jstring convert(JNIEnv* env, jobject obj, jstring config, jstring input) {
  jboolean isCopy = JNI_FALSE;
  const char* ip = env->GetStringUTFChars(input, &isCopy);
  const char* cf = env->GetStringUTFChars(config, &isCopy);
  jsize len = env->GetStringUTFLength(input);
  string result = OpenCC(cf).convert(ip, len);
  env->ReleaseStringUTFChars(config, cf);
  env->ReleaseStringUTFChars(input, ip);
  return env->NewStringUTF(result.c_str());
}

static JNINativeMethod methods[] = {{"convert",
                                     "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;",
                                     reinterpret_cast<void*>(convert)}};

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  JNIEnv* env = NULL;
  if (JNI_OK != vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_4)) {
    cout << "JNI_OnLoad could not get JNI env" << endl;
    return JNI_ERR;
  }

  jclass clazz = env->FindClass(className);

  if (env->RegisterNatives(clazz, methods,
                           sizeof(methods) / sizeof((methods)[0])) < 0) {
    cout << "RegisterNatives error" << endl;
    return JNI_ERR;
  }

  return JNI_VERSION_1_4;
}
