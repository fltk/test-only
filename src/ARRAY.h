// Portable macro to make an automatic array of variable size. Uses
// the compiler or alloca() if possible, otherwise it uses a C++
// automatic pointer.

#ifndef fl_ALLOC_h
#define fl_ALLOC_h
#undef ARRAY

// Systems with compiler support for dynamic local arrays:
#if defined(__GNUC__)
# define ARRAY(type, name, n) type name[n]

// Win32 named alloca "_alloca":
#elif defined(_WIN32)
# include <malloc.h>
# define ARRAY(type, name, n) type*const name = (type*)_alloca(n*sizeof(type))

// Systems with alloca():
#elif defined(__DECCXX) || defined(__sgi)
# include <alloca.h>
# define ARRAY(type, name, n) type*const name = (type*)alloca(n*sizeof(type))

// Use C++ to make the array:
#else
template <class T> class Alloca {
  T* p;
public:
  Alloca(int n) {p = new T[n];}
  operator T*() {return p;}
  ~Alloca() {delete [] p;}
};
#define ARRAY(type, name, n) Alloca<type> name(n)

#endif
#endif

