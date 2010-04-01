//
// "$Id: Group.h 5915 2007-06-19 17:49:29Z spitzak $"
//

#ifndef fltk2_Group_h
#define fltk2_Group_h

#include "Widget.h"
#include <fltk3/Group.h>

namespace fltk {

class FL_API Group : public Widget {
  
protected:
  Group() { }
  
public:
#if 0
  int children() const {return children_;}
  Widget* child(int n) const {return array_[n];}

  void draw();
  void layout();
  int handle(int);
#endif
  
  void begin() { ((fltk3::Group*)_p)->begin(); }
  void end() { ((fltk3::Group*)_p)->end(); }
  
#if 0
  static Group *current() {return current_;}
  static void current(Group *g) {current_ = g;}

  int find(const Widget*) const;
  int find(const Widget& o) const {return find(&o);}
#endif
  
  Group(int x, int y, int w, int h, const char *label=0, bool doBegin=false) {
    _p = new fltk3::Group(x, y, w, h, label);
    _p->wrapper(this);
    if (doBegin) begin();
  }
  
#if 0
  virtual ~Group();
  void add(Widget&);
  void add(Widget* o) {add(*o);}
  void insert(Widget&, int index);
  void insert(Widget& o, Widget* before) {insert(o,find(before));}
  void remove(int index);
  void remove(Widget& o) {remove(find(o));}
  void remove(Widget* o) {remove(find(*o));}
  void remove_all();
  void replace(int index, Widget&);
  void replace(Widget& old, Widget& o) {replace(find(old),o);}
  void swap(int indexA, int indexB);
  void clear();
#endif
  
  void resizable(Widget& o) { ((fltk3::Group*)_p)->resizable(o.fltk3Widget()); }
  void resizable(Widget* o) { ((fltk3::Group*)_p)->resizable(o->fltk3Widget()); }
  Widget* resizable() const { return (Widget*)(((fltk3::Group*)_p)->resizable()->wrapper()); }
  
#if 0
  void add_resizable(Widget& o) {resizable_ = &o; add(o);}
  void init_sizes();

  void focus_index(int v) {focus_index_ = v;}
  void set_focus(Widget* w) {focus_index_ = find(w);}
  int  focus_index() const {return focus_index_;}
  static int navigation_key();

  // back compatability function:
  friend FL_FORMS_API void end_group(); // forms emulation function
  void fix_old_positions();

  Flags resize_align() const {return resize_align_;}
  void resize_align(Flags f) {resize_align_ = f;}

protected:

  void draw_child(Widget&) const;
  void update_child(Widget&) const;
  void draw_outside_label(Widget&) const ;
  int initial_w, initial_h;
  int* sizes();
  void layout(const Rectangle&, int layout_damage);

private:

  int children_;
  int focus_index_;
  Widget** array_;
  Widget* resizable_;
  Flags resize_align_;
  int *sizes_; // remembered initial sizes of children

  static Group *current_;
#endif
  
};

}

#endif

//
// End of "$Id: Group.h 5915 2007-06-19 17:49:29Z spitzak $".
//
