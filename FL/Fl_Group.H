//
// "$Id$"
//
// Group compatibility header file for the Fast Light Tool Kit (FLTK).
//

#ifndef Fltk1_Group_H
#define Fltk1_Group_H

#include <fltk3/Group.h>
#include "Fl_Widget.H"

class FL_EXPORT Fl_Group : public Fl_Widget {
  
  Fl_Group(const Fl_Group&);
  Fl_Group& operator=(const Fl_Group&);
  
protected:
  Fl_Group() { }
  
#if 0
  void draw();
  void draw_child(Fl_Widget& widget) const;
  void draw_children();
  void draw_outside_label(const Fl_Widget& widget) const ;
  void update_child(Fl_Widget& widget) const;
  int  *sizes();
#endif

public:

#if 0
  int handle(int);
#endif
  
  void begin() { ((fltk3::Group*)_p)->begin(); }
  void end() { ((fltk3::Group*)_p)->end(); }
  
#if 0
  static Fl_Group *current();
  static void current(Fl_Group *g);
  int children() const {return children_;}
  Fl_Widget* child(int n) const {return array()[n];}
  int find(const Fl_Widget*) const;
  int find(const Fl_Widget& o) const {return find(&o);}
  Fl_Widget* const* array() const;
  void resize(int,int,int,int);
#endif 
  
  Fl_Group(int x, int y, int w, int h, const char *label = 0) {
    _p = new fltk3::Group(x, y, w, h, label); 
    _p->wrapper(this);
    begin();
  }
  
  virtual ~Fl_Group() {
  }
  
#if 0
  void add(Fl_Widget&);
  void add(Fl_Widget* o) {add(*o);}
  void insert(Fl_Widget&, int i);
  void insert(Fl_Widget& o, Fl_Widget* before) {insert(o,find(before));}
  void remove(Fl_Widget&);
  void remove(Fl_Widget* o) {remove(*o);}
  void clear();
#endif
  
  void resizable(Fl_Widget& o) { ((fltk3::Group*)_p)->resizable(o.fltk3Widget()); }
  void resizable(Fl_Widget* o) { ((fltk3::Group*)_p)->resizable(o->fltk3Widget()); }
  
#if 0
  Fl_Widget* resizable() const {return resizable_;}
  void add_resizable(Fl_Widget& o) {resizable_ = &o; add(o);}
  void init_sizes();
  void clip_children(int c) { if (c) set_flag(CLIP_CHILDREN); else clear_flag(CLIP_CHILDREN); }
  unsigned int clip_children() { return (flags() & CLIP_CHILDREN) != 0; }
  virtual Fl_Group* as_group() { return this; }
  void focus(Fl_Widget* W) {W->take_focus();}
  Fl_Widget* & _ddfdesign_kludge() {return resizable_;}
  void forms_end();
#endif
};

#if 0
class FL_EXPORT Fl_End {
public:
  /** All it does is calling Fl_Group::current()->end() */
  Fl_End() {Fl_Group::current()->end();}
};
#endif

#endif

//
// End of "$Id$".
//
