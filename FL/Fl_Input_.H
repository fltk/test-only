//
// "$Id$"
//

#ifndef Fltk1_Input__H
#define Fltk1_Input__H

#include <fltk3/Input_.H>

#include "Fl_Widget.H"

#define FL_NORMAL_INPUT		0
#define FL_FLOAT_INPUT		1
#define FL_INT_INPUT		2
#define FL_HIDDEN_INPUT		3
#define FL_MULTILINE_INPUT	4
#define FL_SECRET_INPUT		5
#define FL_INPUT_TYPE		7
#define FL_INPUT_READONLY	8
#define FL_NORMAL_OUTPUT	(FL_NORMAL_INPUT | FL_INPUT_READONLY)
#define FL_MULTILINE_OUTPUT	(FL_MULTILINE_INPUT | FL_INPUT_READONLY)
#define FL_INPUT_WRAP		16
#define FL_MULTILINE_INPUT_WRAP	(FL_MULTILINE_INPUT | FL_INPUT_WRAP)
#define FL_MULTILINE_OUTPUT_WRAP (FL_MULTILINE_INPUT | FL_INPUT_READONLY | FL_INPUT_WRAP)

class FL_EXPORT Fl_Input_ : public Fl_Widget {

protected:
  Fl_Input_() {}
  
#if 0 // fltk123: 
  int word_start(int i) const;
  int word_end(int i) const;
  int line_start(int i) const;
  int line_end(int i) const;
  void drawtext(int, int, int, int);
  int up_down_position(int, int keepmark=0);
  void handle_mouse(int, int, int, int, int keepmark=0);
  int handletext(int e, int, int, int, int);
  void maybe_do_callback();
  int xscroll() const {return xscroll_;}
  int yscroll() const {return yscroll_;}
  void yscroll(int y) { yscroll_ = y; damage(FL_DAMAGE_EXPOSE);}
  int linesPerPage();
#endif

public:
  Fl_Input_(int x, int y, int w, int h, const char *l = 0) {
    _p = new fltk3::Input_(x, y, w, h, l);
    _p->wrapper(this);
  }
#if 0 // fltk123: 
  ~Fl_Input_();
  void resize(int, int, int, int);
#endif
  int value(const char *v) { return ((fltk3::Input_*)_p)->value(v); }
  int value(const char *v, int a) { return ((fltk3::Input_*)_p)->value(v, a); }
  int static_value(const char *v)  { return ((fltk3::Input_*)_p)->static_value(v); }
  int static_value(const char *v, int a)  { return ((fltk3::Input_*)_p)->static_value(v, a); }
  const char* value() const  { return ((fltk3::Input_*)_p)->value(); }
#if 0 // fltk123: 
  Fl_Char index(int i) const;
  int size() const {return size_;}
  void size(int W, int H) { fltk3::Widget::size(W, H); }
  int maximum_size() const {return maximum_size_;}
  void maximum_size(int m) {maximum_size_ = m;}
  int position() const {return position_;}
  int mark() const {return mark_;}
  int position(int p, int m);
  int position(int p) {return position(p, p);}
  int mark(int m) {return position(position(), m);}
  int replace(int, int, const char*, int=0);
  int cut() {return replace(position(), mark(), 0);}
  int cut(int n) {return replace(position(), position()+n, 0);}
  int cut(int a, int b) {return replace(a, b, 0);}
  int insert(const char* t, int l=0){return replace(position_, mark_, t, l);}
  int copy(int clipboard);
  int undo();
  int copy_cuts();
  int shortcut() const {return shortcut_;}
  void shortcut(int s) {shortcut_ = s;}
  Fl_Font textfont() const {return textfont_;}
  void textfont(Fl_Font s) {textfont_ = s;}
  Fl_Fontsize textsize() const {return textsize_;}
  void textsize(Fl_Fontsize s) {textsize_ = s;}
  Fl_Color textcolor() const {return textcolor_;}
  void textcolor(Fl_Color n) {textcolor_ = n;}
  Fl_Color cursor_color() const {return cursor_color_;}
  void cursor_color(Fl_Color n) {cursor_color_ = n;}
  int input_type() const {return type() & FL_INPUT_TYPE; }
  void input_type(int t) { type((uchar)(t | readonly())); }
  int readonly() const { return type() & FL_INPUT_READONLY; }
  void readonly(int b) { if (b) type((uchar)(type() | FL_INPUT_READONLY));
                         else type((uchar)(type() & ~FL_INPUT_READONLY)); }
  int wrap() const { return type() & FL_INPUT_WRAP; }
  void wrap(int b) { if (b) type((uchar)(type() | FL_INPUT_WRAP));
                         else type((uchar)(type() & ~FL_INPUT_WRAP)); }
#endif
};

#endif 

//
// End of "$Id$".
//
