/* Stuff needed to compile DLL's on Windows. These symbols pollute all
   the header files. Make sure you put them on anything you want to go
   into a DLL!

   I also put here some typedefs that fltk uses everywhere.
*/

#ifndef FL_API

#if !defined(DOXYGEN) && defined(_WIN32) && defined(FL_SHARED)
#  ifdef FL_LIBRARY
#    define FL_API	__declspec(dllexport)
#  else
#    define FL_API	__declspec(dllimport)
#  endif
#  ifdef FL_IMAGES_LIBRARY
#    define FL_IMAGES_API	__declspec(dllexport)
#  else
#    define FL_IMAGES_API	__declspec(dllimport)
#  endif
#  ifdef FL_GLUT_LIBRARY
#    define FL_GLUT_API		__declspec(dllexport)
#  else
#    define FL_GLUT_API		__declspec(dllimport)
#  endif
#  ifdef FL_FORMS_LIBRARY
#    define FL_FORMS_API	__declspec(dllexport)
#  else
#    define FL_FORMS_API	__declspec(dllimport)
#  endif
#  ifdef FL_GL_LIBRARY
#    define FL_GL_API		__declspec(dllexport)
#  else
#    define FL_GL_API		__declspec(dllimport)
#  endif
#else
#  define FL_API
#  define FL_IMAGES_API
#  define FL_GLUT_API
#  define FL_FORMS_API
#  define FL_GL_API
#endif

typedef unsigned char	uchar;

#endif
