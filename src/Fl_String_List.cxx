#include <FL/Fl_String_List.H>
#include <FL/Fl_Item.H>

int Fl_String_List::children(const Fl_Group*) {
  if (!children_) {
    const char* p = s; if (!p) return 0;
    int i = 0;
    while (*p) {while (*p++); i++;}
    children_ = i;
  }
  return children_;
}

Fl_Widget* Fl_String_List::child(const Fl_Group*, int i) {
  const char* p = s;
  while (i--) {while (*p++);}
  static Fl_Widget* widget;
  if (!widget) {
    Fl_Group::current(0);
    widget = new Fl_Item();
  }
  widget->label(p);
  widget->user_data((void*)p);
  return widget;
}
