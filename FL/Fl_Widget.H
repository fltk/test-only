//
// "$Id: Fl_Widget.H,v 1.15 1999/03/31 14:52:24 mike Exp $"
//
// Widget header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef Fl_Widget_H
#define Fl_Widget_H

#include "Enumerations.H"

#define WIDGET_STYLE ((Fl_Widget::Style*)_style)

class Fl_Widget;
class Fl_Window;

typedef void (Fl_Callback )(Fl_Widget*, void*);
typedef Fl_Callback* Fl_Callback_p; // needed for BORLAND
typedef void (Fl_Callback0)(Fl_Widget*);
typedef void (Fl_Callback1)(Fl_Widget*, long);

struct Fl_Label {
  const char* value;
  uchar type;
  uchar font;
  uchar size;
  uchar color;
  void draw(int,int,int,int, Fl_Align) const ;
  void measure(int&, int&) const ;
};

class Fl_Widget {
  friend class Fl_Group;

public:
  enum Attribute {
    LABELCOLOR = 0,
    LABELSIZE,
    LABELTYPE,
    LABELFONT,
    COLOR,
    COLOR2,
    COLOR3,
    BOX,
    WA_END
  };

  struct Style {
    uchar widget_[WA_END];
    uchar sbf;
    
    // This is just a kludge used for one particular situation--
    // generally determined by whether style() == 0
    uchar not_dynamic;

    Style();
    uchar& widget(Attribute a) { return widget_[a]; }
  };

  enum {INACTIVE=1, INVISIBLE=2, OUTPUT=4, SHORTCUT_LABEL=64, CHANGED=128 };

  virtual ~Fl_Widget();

  virtual void loadstyle() const;

  // These next few functions are mainly used for FLUID

  // returns pointer to the style structure
  const Style* style() const { if (_style) return _style; return default_style(); }

  // use the default style instead of any forced attributes
  void use_default_style() { 
    if (_style && !_style->not_dynamic) delete _style;
    _style = 0;
  }

  // are we using default style?
  int is_using_default_style() { return _style ? 0 : 1; }  

  // after doing this setting attributes modifies the default style
  void modify_default_style() {
    if (_style && !_style->not_dynamic) delete _style;
    _style = default_style();
    _style->not_dynamic = 1;
  }

  // has this attribute been forced to a particular value?
  int is_style_forced(Attribute a) const
    { if (!_style) return 0; return ((Style*)_style)->sbf & bf(a); }

  // copy the style of another widget
  void copy_style(Fl_Widget* o)
    { mstyle(&_style); *((Style*)_style) = *((Style*)o->style()); }

  const char *tooltip() const { return tooltip_; }
  void tooltip(const char *t);
  uchar bf(uchar a) const {return (uchar)1 << a;}
  unsigned short flags() const {return flags_;}
  virtual void draw() = 0;
  virtual int handle(int);
  Fl_Widget* parent() const {return parent_;}
  void parent(Fl_Widget* w) {parent_ = w;} // for hacks only

  uchar type() const {return type_;}
  void type(uchar t) {type_ = t;}

  int x() const {return x_;}
  int y() const {return y_;}
  int w() const {return w_;}
  int h() const {return h_;}
  virtual void resize(int,int,int,int);
  int damage_resize(int,int,int,int);
  void position(int X,int Y) {resize(X,Y,w_,h_);}
  void size(int W,int H) {resize(x_,y_,W,H);}

  Fl_Align align() const {return (Fl_Align)align_;}

  void align(uchar a) {align_ = a;}

  Fl_Boxtype box() const;
  Fl_Color color() const;
  Fl_Color selection_color() const;
  Fl_Color color3() const;
  Fl_Labeltype labeltype() const;
  Fl_Color labelcolor() const;
  Fl_Font labelfont() const;
  uchar labelsize() const;

  void box(Fl_Boxtype a);
  void color(uchar a);
  void selection_color(uchar a);
  void color3(uchar a);
  void color(uchar a, uchar b);
  void labeltype(Fl_Labeltype a);
  void labelcolor(uchar a);
  void labelfont(uchar a);
  void labelsize(uchar a);

  const char* label() const {return label_;}
  void label(const char* a) {label_=a;}
  void label(Fl_Labeltype a,const char* b);

  Fl_Callback* callback() const {return callback_;}
  void callback(Fl_Callback* c, void* p) {callback_=c; user_data_=p;}
  void callback(Fl_Callback* c) {callback_=c;}
  void callback(Fl_Callback0*c) {callback_=(Fl_Callback*)c;}
  void callback(Fl_Callback1*c, long p=0) {callback_=(Fl_Callback*)c; user_data_=(void*)p;}
  void* user_data() const {return user_data_;}
  void user_data(void* v) {user_data_ = v;}
  long argument() const {return (long)user_data_;}
  void argument(long v) {user_data_ = (void*)v;}
  Fl_When when() const {return (Fl_When)when_;}
  void when(uchar i) {when_ = i;}

  int visible() const {return !(flags_&INVISIBLE);}
  int visible_r() const;
  void show();
  void hide();
  void set_visible() {flags_ &= ~INVISIBLE;}
  void clear_visible() {flags_ |= INVISIBLE;}
  int active() const {return !(flags_&INACTIVE);}
  int active_r() const;
  void activate();
  void deactivate();
  int output() const {return (flags_&OUTPUT);}
  void set_output() {flags_ |= OUTPUT;}
  void clear_output() {flags_ &= ~OUTPUT;}
  int takesevents() const {return !(flags_&(INACTIVE|INVISIBLE|OUTPUT));}
  int changed() const {return flags_&CHANGED;}
  void set_changed() {flags_ |= CHANGED;}
  void clear_changed() {flags_ &= ~CHANGED;}
  int take_focus();

  static void default_callback(Fl_Widget*, void*);
  void do_callback() {callback_(this,user_data_);}
  void do_callback(Fl_Widget* o,void* arg=0) {callback_(o,arg);}
  void do_callback(Fl_Widget* o,long arg) {callback_(o,(void*)arg);}
  int test_shortcut();
  static int test_shortcut(const char*);
  int contains(const Fl_Widget*) const ;
  int inside(const Fl_Widget* o) const {return o ? o->contains(this) : 0;}

  void redraw();
  uchar damage() const {return damage_;}
  void clear_damage(uchar c = 0) {damage_ = c;}
  void damage(uchar c);
  void damage(uchar c,int,int,int,int);
  void measure_label(int&, int&);
  void draw_label(int, int, int, int, Fl_Color, Fl_Align) const;

  Fl_Window* window() const ;

  // back compatability only:
  Fl_Color color2() const;
  void color2(uchar a);

protected:
  static Fl_Widget::Style* _default_style;
  Style* _style;

  Fl_Widget(int,int,int,int,const char* =0);

  virtual Fl_Widget::Style* default_style() const { mstyle(&_default_style); return _default_style; }
  virtual void mstyle(Fl_Widget::Style** s) const { if (!(*s)) (*s) = new Style; }

  void x(int v) {x_ = v;}
  void y(int v) {y_ = v;}
  void w(int v) {w_ = v;}
  void h(int v) {h_ = v;}

  void set_flag(unsigned short c) {flags_ |= c;}
  void clear_flag(unsigned c) {flags_ &= ~c;}

  void draw_box() const;
  void draw_box(Fl_Boxtype, Fl_Color) const;
  void draw_box(Fl_Boxtype, int,int,int,int, Fl_Color) const;
  void draw_label() const;
  void draw_label(Fl_Color) const;
  void draw_label(int, int, int, int, Fl_Color) const;

private:
  Fl_Widget* parent_;
  Fl_Callback* callback_;
  const char* label_;
  void* user_data_;
  const char *tooltip_;
  short x_,y_,w_,h_;
  uchar flags_;
  uchar type_;
  uchar damage_;
  uchar align_;
  uchar when_;

  // "de-implement" the copy constructors:
  Fl_Widget & operator=(const Fl_Widget &);
  Fl_Widget(const Fl_Widget &);
};

// reserved type numbers (necessary for my cheapo RTTI) start here.
// grep the header files for "RESERVED_TYPE" to find the next available
// number.
#define FL_RESERVED_TYPE 100

#endif

//
// End of "$Id: Fl_Widget.H,v 1.15 1999/03/31 14:52:24 mike Exp $".
//
