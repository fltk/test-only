#include <fltk/StringList.h>
#include <fltk/Item.h>
using namespace fltk;

int StringList::children(const Menu* group, const int* indexes, int level) {
  // Get number of widget children:
  int n = List::children(group, 0,0);	
  if (!level) {
    // top level has widgets + all strings
    return children_ + n;
  } else if (*indexes < n) {
    // return children counts for widgets:
    return List::children(group, indexes, level);
  } else {
    // the strings have no children:
    return -1;
  }
}

Widget* StringList::child(const Menu* group, const int* indexes, int level) {
  // Return children for any widgets:
  int n = *indexes - List::children(group, 0,0);
  if (n < 0) return List::child(group, indexes, level);
  // Construct reusable widget for string and return it:
  static Widget* widget;
  if (!widget) {
    Group::current(0);
    widget = new Item();
  }
  widget->type(0);
  widget->label(array[n]);
  widget->clear_value();
  widget->w(0);
  return widget;
}

StringList::StringList(const char*const* a) {
  array = a;
  for (children_ = 0; a && a[children_]; children_++);
}

StringList::StringList(const char* s) {
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
