/**
 * \file   BrefDLL.h
 * \author Vincent Giersch
 * \date   Tue Mar 20 16:21:00 2012
 */

#ifndef BREF_API_DETAIL_BREFDLL_H_
#define BREF_API_DETAIL_BREFDLL_H_

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define BREF_DLL __attribute__ ((dllexport))
  #else
    #define BREF_DLL __declspec(dllexport)
  #endif
  #pragma warning(disable: 4251)
#else
  #if __GNUC__ >= 4
    #define BREF_DLL __attribute__ ((visibility ("default")))
  #else
    #define BREF_DLL
  #endif
#endif

#endif /* !BREF_API_DETAIL_BREFDLL_H_ */