// This object allows a compact list of const char* pointers to describe
// the items in a browser or menu.  It is pretty much impossible to describe
// a menu in less space than this.

#ifndef Fl_String_List_H
#define Fl_String_List_H

#include <FL/Fl_Menu_.H>

class FL_API Fl_String_List : public Fl_List {
  const char* const * array;
  int children_;
public:
  virtual int children(const Fl_Group*, const int* indexes, int level);
  virtual Fl_Widget* child(const Fl_Group*, const int* indexes, int level);
  Fl_String_List(const char*const* a, int n) : array(a), children_(n) {}
  Fl_String_List(const char*const* a);
  Fl_String_List(const char* S);
};

#endif
