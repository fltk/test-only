#include <FL/Fl_String_List.H>
#include <FL/Fl_Item.H>

int Fl_String_List::children(const Fl_Group* group) {
  return children_ + Fl_List::children(group);
}

Fl_Widget* Fl_String_List::child(const Fl_Group* group, int i) {
  if (i >= children_) return Fl_List::child(group, i-children_);
  static Fl_Widget* widget;
  if (!widget) {
    Fl_Group::current(0);
    widget = new Fl_Item();
  }
  widget->label(array[i]);
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
