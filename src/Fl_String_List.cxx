#include <fltk/Fl_String_List.h>
#include <fltk/Fl_Item.h>

int Fl_String_List::children(const Fl_Menu_* group, const int* indexes, int level) {
  int n = Fl_List::children(group, 0,0);
  if (!level) {
    return children_ + n;
  } else if (*indexes < n) {
    return Fl_List::children(group, indexes, level);
  } else {
    return -1;
  }
}

Fl_Widget* Fl_String_List::child(const Fl_Menu_* group, const int* indexes, int level) {
  int n = *indexes - Fl_List::children(group, 0,0);
  if (n < 0) return Fl_List::child(group, indexes, level);
  if (n >= children_) return 0;
  static Fl_Widget* widget;
  if (!widget) {
    Fl_Group::current(0);
    widget = new Fl_Item();
  }
  widget->type(0);
  widget->label(array[n]);
  widget->clear_value();
  widget->w(0);
  return widget;
}

Fl_String_List::Fl_String_List(const char*const* a) {
  array = a;
  for (children_ = 0; a && a[children_]; children_++);
}

Fl_String_List::Fl_String_List(const char* s) {
  if (!s || !*s) {children_ = 0; return;}
  const char* temp[256];
  int n = 0;
  while (*s && n < 256) {
    temp[n] = s;
    while (*s++);
    n++;
  }
  children_ = n;
  const char** t = new const char* [n]; // this array is never freed...
  array = t;
  for (; n--;) t[n] = temp[n];
}
