#include <windows.h>
#include <process.h>

// these pointers are in run.cxx:
extern void (*fl_lock_function)();
extern void (*fl_unlock_function)();
static void init_function();
static void (*init_or_lock_function)() = init_function;

static CRITICAL_SECTION cs;

static void unlock_function() {
  LeaveCriticalSection(&cs);
}

static void lock_function() {
  EnterCriticalSection(&cs);
}

static DWORD main_thread;

static void init_function() {
  InitializeCriticalSection(&cs);
  lock_function();
  fl_lock_function = init_or_lock_function = lock_function;
  fl_unlock_function = unlock_function;
  main_thread = GetCurrentThreadId();
}

//static void (*init_or_lock_function)() = init_function;

void fltk::lock() {init_or_lock_function();}

void fltk::unlock() {fl_unlock_function();}

extern UINT fl_wake_msg;
void fltk::awake(void* msg) {
  if (!in_main_thread())
    PostThreadMessage( main_thread, fl_wake_msg, (WPARAM)msg, 0);
}
