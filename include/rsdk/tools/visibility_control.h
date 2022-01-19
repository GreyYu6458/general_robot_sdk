#ifndef _HUI_VISIBILITY_CONTROL_H_
#define _HUI_VISIBILITY_CONTROL_H_

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define HUI_EXPORT __attribute__ ((dllexport))
    #define HUI_IMPORT __attribute__ ((dllimport))
  #else
    #define HUI_EXPORT __declspec(dllexport)
    #define HUI_IMPORT __declspec(dllimport)
  #endif
  #ifdef HUI_LIBRARY
    #define HUI_PUBLIC HUI_EXPORT
  #else
    #define HUI_PUBLIC HUI_IMPORT
  #endif
  #define HUI_PUBLIC_TYPE HUI_PUBLIC
  #define HUI_LOCAL
#else
  #define HUI_EXPORT __attribute__ ((visibility("default")))
  #define HUI_IMPORT
  #if __GNUC__ >= 4
    #define HUI_PUBLIC __attribute__ ((visibility("default")))
    #define HUI_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define HUI_PUBLIC
    #define HUI_LOCAL
  #endif
  #define HUI_PUBLIC_TYPE
#endif

#endif
