// This uses a null-terminated list of array of const char* pointers
// to the values for label().  It is pretty much impossible to describe
// a menu in less space than this.

#ifndef Fl_String_List_H
#define Fl_String_List_H

#include <FL/Fl_Group.H>

class Fl_String_List : public Fl_List {
  const char* s;
  int children_;
public:
  virtual int children(const Fl_Group*);
  virtual Fl_Widget* child(const Fl_Group*, int n);
  Fl_String_List(const char* S) : s(S), children_(0) {}
  void set(const char* t) {s = t; children_ = 0;}
};

#endif
