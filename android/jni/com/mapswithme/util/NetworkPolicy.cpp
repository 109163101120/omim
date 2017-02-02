#include "com/mapswithme/core/jni_helper.hpp"

namespace network_policy
{
bool GetNetworkPolicyStatus(JNIEnv * env, jobject obj)
{
  static const jmethodID networkPolicyCanUseMethod =
      jni::GetMethodID(env, obj, "сanUseNetwork", "()Z");
  return env->CallBooleanMethod(obj, networkPolicyCanUseMethod);
}
}  // namespace network_policy
