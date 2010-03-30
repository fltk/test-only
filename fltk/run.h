// "$Id: run.h 6233 2008-09-14 07:54:06Z spitzak $"
//

#ifndef fltk2_run_h
#define fltk2_run_h

#include "FL_API.h"
#include <fltk3/run.h>

#ifdef check
# undef check
#endif

namespace fltk {

#if 0
FL_API void display(const char*);
FL_API int arg(int, char**, int&);
FL_API int args(int, char**, int&, int (*)(int,char**,int&) = 0);
extern FL_API const char* const help;
FL_API void args(int, char**);
FL_API bool enable_tablet_events();

FL_API int wait();
FL_API int wait(float time);
FL_API int check();
FL_API int ready();
#endif
  
  FL_API int run() { return fltk3::run(); }
  
#if 0
FL_API void flush();
FL_API void redraw();
extern FL_API int damage_;
inline void damage(int d) {damage_ = d;}
inline int damage() {return damage_;}

/*! Type of function passed to add_timeout(), add_check(), and add_idle() */
typedef void (*TimeoutHandler)(void*);

FL_API double get_time_secs();

FL_API void add_timeout(float t, TimeoutHandler, void* v = 0);
FL_API void repeat_timeout(float t, TimeoutHandler,void* = 0);
FL_API bool has_timeout(TimeoutHandler, void* = 0);
FL_API void remove_timeout(TimeoutHandler, void* = 0);

FL_API void add_check(TimeoutHandler, void* = 0);
FL_API bool has_check(TimeoutHandler, void* = 0);
FL_API void remove_check(TimeoutHandler, void* = 0);

FL_API void add_idle(TimeoutHandler, void* = 0);
FL_API bool has_idle(TimeoutHandler, void* = 0);
FL_API void remove_idle(TimeoutHandler, void* = 0);

// For back-compatability only:
extern FL_API void (*idle)();
inline void set_idle(void (*cb)()) {idle = cb;}

/*! Type of function passed to add_fd() */
typedef void (*FileHandler)(int fd, void*);
enum {READ = 1, WRITE = 4, EXCEPT = 8};
FL_API void add_fd(int fd, int when, FileHandler, void* =0);
FL_API void add_fd(int fd, FileHandler, void* = 0);
FL_API void remove_fd(int, int when = -1);

FL_API void lock();
FL_API void unlock();
FL_API void awake(void* message = 0);
FL_API void* thread_message();
FL_API bool in_main_thread();
#endif
  
}

#endif
