/*
 The following is only used when building DLLs under WIN32
*/

#if defined(WIN32) && defined(FL_DLL)
#  ifdef FL_LIBRARY
#    define FL_API	__declspec(dllexport)
#  else
#    define FL_API	__declspec(dllimport)
#  endif /* FL_LIBRARY */
#else
#  define FL_API
#endif /* FL_DLL */
