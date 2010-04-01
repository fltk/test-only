//
// "$Id: Input.h 4886 2006-03-30 09:55:32Z fabien $"
//

#ifndef fltk2_Input_h
#define fltk2_Input_h

#include <fltk3/Input.h>

#include "Widget.h"

namespace fltk {

class FL_API Input : public Widget {
public:
#if 0 // fltk123:
  enum { // values for type()
    NORMAL	= 0,
    FLOAT_INPUT = 1,
    INT_INPUT	= 2,
    SECRET	= 3,
    MULTILINE	= 4,
    WORDWRAP	= 5
  };
#endif
  Input(int x, int y, int w, int h, const char *l = 0) {
    _p = new fltk3::Input(x, y, w, h, l);
    _p->wrapper(this);
  }
#if 0 // fltk123:  
  ~Input();
  static NamedStyle* default_style;

  void draw();
  void draw(const Rectangle&);
  int handle(int);
  int handle(int event, const Rectangle&);
  bool handle_key();
#endif
  
  bool text(const char *v) { return ((fltk3::Input*)_p)->value(v); }
  bool text(const char *v, int a) { return ((fltk3::Input*)_p)->value(v, a); }
  bool static_text(const char *v) { return ((fltk3::Input*)_p)->static_value(v); }
  bool static_text(const char *v, int a) { return ((fltk3::Input*)_p)->static_value(v, a); }
  const char* text() const { return ((fltk3::Input*)_p)->value(); }
#if 0 // fltk123:
  char at(int i) const {return text_[i];}
#ifdef FLTK_1_WIDGET  // back-compatability section:
  char index(int i) const {return text_[i];}
#endif
#endif
  bool value(const char* v) {return text(v);} // DEPRECIATED
  bool value(const char* v, int n) {return text(v,n);} // DEPRECIATED
  bool static_value(const char* v) {return static_text(v);} // DEPRECIATED
  const char* value() const {return text();} // DEPRECIATED
#if 0 // fltk123:
  int size() const {return size_;}
  void reserve(int newsize);

  int position() const {return position_;}
  int mark() const {return mark_;}
  void position(int p, int m);
  void position(int p) {position(p, p);}
  void up_down_position(int position, bool extend);
  void mark(int m) { position(position(), m);}

  virtual bool replace(int, int, const char*, int);
  bool cut() {return replace(position(), mark(), 0, 0);}
  bool cut(int n) {return replace(position(), position()+n, 0, 0);}
  bool cut(int a, int b) {return replace(a, b, 0, 0);}
  bool insert(const char* t);
  bool insert(const char* t, int l){return replace(position_, mark_, t, l);}
  bool replace(int a, int b, char c) {return replace(a,b,&c,1);}
  bool copy(bool clipboard = true);
  bool undo();
  void maybe_do_callback();

  int word_start(int i) const;
  int word_end(int i) const;
  int line_start(int i) const;
  int line_end(int i) const;
  int mouse_position(const Rectangle&) const;
  int xscroll() const {return xscroll_;}
  int yscroll() const {return yscroll_;}

#endif
};

}

#endif

//
// End of "$Id: Input.h 4886 2006-03-30 09:55:32Z fabien $".
//
