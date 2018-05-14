//
// MATLAB Compiler: 6.5 (R2017b)
// Date: Thu May  3 19:31:15 2018
// Arguments: "-B""macro_default""-W""cpplib:libMyAdd""-T""link:lib""c_matlab.m"
//

#ifndef __libMyAdd_h
#define __libMyAdd_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libMyAdd_C_API 
#define LIB_libMyAdd_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_libMyAdd_C_API 
bool MW_CALL_CONV libMyAddInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_libMyAdd_C_API 
bool MW_CALL_CONV libMyAddInitialize(void);

extern LIB_libMyAdd_C_API 
void MW_CALL_CONV libMyAddTerminate(void);

extern LIB_libMyAdd_C_API 
void MW_CALL_CONV libMyAddPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_libMyAdd_C_API 
bool MW_CALL_CONV mlxC_matlab(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_libMyAdd
#define PUBLIC_libMyAdd_CPP_API __declspec(dllexport)
#else
#define PUBLIC_libMyAdd_CPP_API __declspec(dllimport)
#endif

#define LIB_libMyAdd_CPP_API PUBLIC_libMyAdd_CPP_API

#else

#if !defined(LIB_libMyAdd_CPP_API)
#if defined(LIB_libMyAdd_C_API)
#define LIB_libMyAdd_CPP_API LIB_libMyAdd_C_API
#else
#define LIB_libMyAdd_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_libMyAdd_CPP_API void MW_CALL_CONV c_matlab(int nargout, mwArray& en_dicom_img, const mwArray& dicom_img);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
