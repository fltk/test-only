typedef void* Fl_Thread;

// Portable recursive mutex
class Fl_Mutex {
  void* m;

  Fl_Thread owner;	// Used to simulate recursive mutex if they are 
  int counter;		// not supported by the system

public:
  Fl_Mutex(bool locked);
  ~Fl_Mutex();
  void lock();
  void unlock();
};

int fl_create_thread(Fl_Thread&, void *(*__start_routine) (void *), void*);
