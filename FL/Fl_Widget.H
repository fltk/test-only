//
// "$Id$"
//
// Widget header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#ifndef Fl_Widget_H
#define Fl_Widget_H

#include <fltk3/Wrapper.h>
#include <fltk3/Widget.h>

#include "Enumerations.H"
#include "Fl_Image.H"

#ifdef _WIN64
#ifdef __GNUC__
#include <stdint.h>
#else
#include <stddef.h>  // M$VC
#endif
typedef intptr_t fl_intptr_t;
typedef uintptr_t fl_uintptr_t;
#else
typedef long fl_intptr_t;
typedef unsigned long fl_uintptr_t;
#endif

class Fl_Widget;
class Fl_Window;
class Fl_Group;
class Fl_Image;

typedef void (Fl_Callback )(Fl_Widget*, void*);
typedef Fl_Callback* Fl_Callback_p; // needed for BORLAND
typedef void (Fl_Callback0)(Fl_Widget*);
typedef void (Fl_Callback1)(Fl_Widget*, long);

struct FL_EXPORT Fl_Label {
  const char* value;
  Fl_Image* image;
  Fl_Image* deimage;
  Fl_Font font;
  Fl_Fontsize size;
  Fl_Color color;
  Fl_Align align_;
  uchar type;
  void draw(int,int,int,int, Fl_Align) const ;
  void measure(int &w, int &h) const ;
};


namespace fltk3 {
  inline Fl_Widget *_3to1_widget(Widget *w) { return (Fl_Widget*)w->wrapper(); }
  inline Fl_Window *_3to1_window(Widget *w) { return (Fl_Window*)w->wrapper(); }
  inline Fl_Group *_3to1_group(Widget *w) { return (Fl_Group*)w->wrapper(); }
  //inline Widget *_1to3_widget(Fl_Widget *w) { return ((fltk3::Widget*)_p); }
}


class FL_EXPORT Fl_Widget : public fltk3::WidgetWrapper {
  
  friend class Fl_Group;

private:
  
  Fl_Widget(const Fl_Widget &); /* make unavailable */
  
  Fl_Widget& operator=(const Fl_Widget &); /* make unavailable */

protected:
  
  Fl_Widget() { /*blank */ }

protected:
  
  void x(int v) {
    ((fltk3::Widget*)_p)->x(v);
  }
  
  void y(int v) {
    ((fltk3::Widget*)_p)->y(v);
  }
  
  void w(int v) {
    ((fltk3::Widget*)_p)->w(v);
  }
  
  void h(int v) {
    ((fltk3::Widget*)_p)->h(v);
  }
  
  unsigned int flags() const {
    return ((fltk3::Widget*)_p)->flags();
  }
  
  void set_flag(unsigned int c) {
    ((fltk3::Widget*)_p)->set_flag(c);
  }
  
  void clear_flag(unsigned int c) {
    ((fltk3::Widget*)_p)->clear_flag(c);
  }
  
  enum {
        INACTIVE        = 1<<0,
        INVISIBLE       = 1<<1,
        OUTPUT          = 1<<2,
        NOBORDER        = 1<<3,
        FORCE_POSITION  = 1<<4,
        NON_MODAL       = 1<<5,
        SHORTCUT_LABEL  = 1<<6,
        CHANGED         = 1<<7,
        OVERRIDE        = 1<<8,
        VISIBLE_FOCUS   = 1<<9,
        COPIED_LABEL    = 1<<10,
        CLIP_CHILDREN   = 1<<11,
        MENU_WINDOW     = 1<<12,
        TOOLTIP_WINDOW  = 1<<13,
        MODAL           = 1<<14,
        NO_OVERLAY      = 1<<15,
        GROUP_RELATIVE  = 1<<16,
        COPIED_TOOLTIP  = 1<<17,
        USERFLAG3       = 1<<29,
        USERFLAG2       = 1<<30,
        USERFLAG1       = 1<<31
  };
  
  void draw_box() const {
    ((fltk3::Widget*)_p)->draw_box();
  }
  
  void draw_box(Fl_Boxtype t, Fl_Color c) const {
    ((fltk3::Widget*)_p)->draw_box(fltk3::_1to3_boxtype(t), fltk3::_1to3_color(c));
  }
  
  void draw_box(Fl_Boxtype t, int x,int y,int w,int h, Fl_Color c) const {
    ((fltk3::Widget*)_p)->draw_box(fltk3::_1to3_boxtype(t), x, y, w, h, fltk3::_1to3_color(c));
  }
  
  void draw_backdrop() const {
    ((fltk3::Widget*)_p)->draw_backdrop();
  }
  
  void draw_focus() {
    ((fltk3::Widget*)_p)->draw_focus();
  }
  
  void draw_focus(Fl_Boxtype t, int x,int y,int w,int h) const {
    ((fltk3::Widget*)_p)->draw_focus(fltk3::_1to3_boxtype(t), x, y, w, h);
  }
  
  void draw_label() const {
    ((fltk3::Widget*)_p)->draw_label();
  }
  
  void draw_label(int x, int y, int w, int h) const {
    ((fltk3::Widget*)_p)->draw_label(x, y, w, h);
  }
  
public:
  Fl_Widget(int x, int y, int w, int h, const char *label=0L) {
    _p = new fltk3::Widget(x, y, w, h, label);
    _p->wrapper(this);
  }

#if 0 // FIXME: 123
  virtual ~Fl_Widget();
  virtual void draw() = 0;
  virtual int handle(int event);
  virtual void resize(int x, int y, int w, int h);
  virtual void show();
  virtual void hide();
  virtual Fl_Group* as_group() {return 0;}
  virtual Fl_Window* as_window() {return 0;}
  virtual class Fl_Gl_Window* as_gl_window() {return 0;}
#endif
  
  Fl_Group *parent() const {
    return fltk3::_3to1_group( ((fltk3::Widget*)_p)->parent() );
  }
  
  void parent(Fl_Group* p) {
    ((fltk3::Widget*)_p)->parent( ((fltk3::Group*)((Fl_Widget*)p)->_p) );
  }
  
  Fl_Window* window() const {
    return fltk3::_3to1_window( ((fltk3::Widget*)_p)->parent() );
  }

  uchar type() const {
    return ((fltk3::Widget*)_p)->type();
  }
  
  void type(uchar t) {
    ((fltk3::Widget*)_p)->type(t);
  }
  
  int x() const {
    return ((fltk3::Widget*)_p)->x();
  }
  
  int y() const {
    return ((fltk3::Widget*)_p)->y();
  }
  
  int w() const {
    return ((fltk3::Widget*)_p)->w();
  }
  
  int h() const {
    return ((fltk3::Widget*)_p)->h();
  }
  
  int damage_resize(int,int,int,int);
  
  void position(int x, int y) {
    ((fltk3::Widget*)_p)->position(x, y);;
  }
  
  void size(int w, int h) {
    ((fltk3::Widget*)_p)->size(w, h);;
  }
  
  Fl_Align align() const {
    return fltk3::_3to1_align(((fltk3::Widget*)_p)->align());
  }
  
  void align(Fl_Align a) {
    ((fltk3::Widget*)_p)->align(fltk3::_1to3_align(a));
  }
  
  Fl_Boxtype box() const {
    return fltk3::_3to1_boxtype(((fltk3::Widget*)_p)->box());
  }
  
  void box(Fl_Boxtype new_box) {
    ((fltk3::Widget*)_p)->box( fltk3::_1to3_boxtype(new_box) );
  }
  
  Fl_Color color() const {
    return fltk3::_3to1_color( ((fltk3::Widget*)_p)->color() );
  }

  void color(Fl_Color bg) {
    ((fltk3::Widget*)_p)->color( fltk3::_1to3_color(bg) );
  }
  
  Fl_Color selection_color() const {
    return fltk3::_3to1_color( ((fltk3::Widget*)_p)->selection_color() );
  }
    
  void selection_color(Fl_Color a) {
    ((fltk3::Widget*)_p)->selection_color( fltk3::_1to3_color(a) );
  }
  
  void color(Fl_Color bg, Fl_Color sel) {
    ((fltk3::Widget*)_p)->color( fltk3::_1to3_color(bg), fltk3::_1to3_color(sel) );
  }
  
  const char* label() const {
    return ((fltk3::Widget*)_p)->label();
  }
  
  void label(const char* text) {
    ((fltk3::Widget*)_p)->label(text);
  }
  
  void copy_label(const char *new_label) {
    ((fltk3::Widget*)_p)->label(new_label);
  }
    
  void label(Fl_Labeltype a, const char* b) {
    ((fltk3::Widget*)_p)->label( fltk3::_1to3_labeltype(a), b );
  }
  
  Fl_Color labelcolor() const {
    return fltk3::_3to1_color( ((fltk3::Widget*)_p)->labelcolor() );
  }
  
  void labelcolor(Fl_Color c) {
    ((fltk3::Widget*)_p)->labelcolor( fltk3::_1to3_color(c) );
  }

  Fl_Labeltype labeltype() const {
    return fltk3::_3to1_labeltype( ((fltk3::Widget*)_p)->labeltype() );
  }
  
  void labeltype(Fl_Labeltype a) {
    ((fltk3::Widget*)_p)->labeltype( fltk3::_1to3_labeltype(a) );
  }
  
  
  Fl_Font labelfont() const {
    return fltk3::_3to1_font( ((fltk3::Widget*)_p)->labelfont() );
  }
  
  void labelfont(Fl_Font f) {
    ((fltk3::Widget*)_p)->labelfont( fltk3::_1to3_font(f) );
  }
  
  Fl_Fontsize labelsize() const {
    return fltk3::_3to1_fontsize( ((fltk3::Widget*)_p)->labelsize() );
  }
  
  void labelsize(Fl_Fontsize pix) {
    ((fltk3::Widget*)_p)->labelsize( fltk3::_1to3_fontsize(pix) );
  }

  Fl_Image* image() {
    return fltk3::_3to1_image(((fltk3::Widget*)_p)->image());
  }
  
  const Fl_Image* image() const {
    return fltk3::_3to1_image(((fltk3::Widget*)_p)->image());
  }
  
  void image(Fl_Image* img) {
    ((fltk3::Widget*)_p)->image( (fltk3::Image*)(img->_p) );
  }
  
  void image(Fl_Image& img) {
    ((fltk3::Widget*)_p)->image( (fltk3::Image&)(img._p) );
  }
  
  Fl_Image* deimage() {
    return fltk3::_3to1_image(((fltk3::Widget*)_p)->deimage());
  }
  
  const Fl_Image* deimage() const {
    return fltk3::_3to1_image(((fltk3::Widget*)_p)->deimage());
  }
  
  void deimage(Fl_Image* img) {
    ((fltk3::Widget*)_p)->deimage( (fltk3::Image*)(img->_p) );
  }
  
  void deimage(Fl_Image& img) {
    ((fltk3::Widget*)_p)->deimage( (fltk3::Image&)(img._p) );
  }
  
  const char *tooltip() const {
    return ((fltk3::Widget*)_p)->tooltip();
  }
  
  void tooltip(const char *text) {
    ((fltk3::Widget*)_p)->tooltip(text);
  }
  
  void copy_tooltip(const char *text) {
    ((fltk3::Widget*)_p)->copy_tooltip(text);
  }
  
  Fl_Callback_p callback() const {
    return (Fl_Callback_p)((fltk3::Widget*)_p)->callback();
  }
  
  void callback(Fl_Callback* cb, void* p) {
    ((fltk3::Widget*)_p)->callback( (fltk3::Callback*)cb, p );
  }
  
  void callback(Fl_Callback* cb) {
    ((fltk3::Widget*)_p)->callback( (fltk3::Callback*)cb );
  }
  
  void callback(Fl_Callback0*cb) {
    ((fltk3::Widget*)_p)->callback( (fltk3::Callback0*)cb );
  }
  
  void callback(Fl_Callback1*cb, long p=0) {
    ((fltk3::Widget*)_p)->callback( (fltk3::Callback1*)cb, p );
  }

  void *user_data() const {
    return ((fltk3::Widget*)_p)->user_data();
  }
  
  void user_data(void *v) {
    ((fltk3::Widget*)_p)->user_data(v);
  }
  
  long argument() const {
    return ((fltk3::Widget*)_p)->argument();
  }
  
  void argument(long v) {
    ((fltk3::Widget*)_p)->argument(v);
  }
  
  Fl_When when() const {
    return fltk3::_3to1_when( ((fltk3::Widget*)_p)->when() );
  }
  
  void when(uchar i) {
    ((fltk3::Widget*)_p)->when( fltk3::_1to3_when((Fl_When)i) );
  }
  
  unsigned int visible() const {
    return ((fltk3::Widget*)_p)->visible();
  }    
  
  int visible_r() const {
    return ((fltk3::Widget*)_p)->visible_r();
  }
  
  void set_visible() {
    ((fltk3::Widget*)_p)->set_visible();
  }
  
  void clear_visible() {
    ((fltk3::Widget*)_p)->clear_visible();
  }

  unsigned int active() const {
    return ((fltk3::Widget*)_p)->active();
  }
  
  int active_r() const {
    return ((fltk3::Widget*)_p)->active_r();
  }
    
  void activate() {
    ((fltk3::Widget*)_p)->activate();
  }
    
  void deactivate() {
    ((fltk3::Widget*)_p)->deactivate();
  }
    
  unsigned int output() const {
    return ((fltk3::Widget*)_p)->output();
  }

  void set_output() {
    ((fltk3::Widget*)_p)->set_output();
  }
    
  void clear_output() {
    ((fltk3::Widget*)_p)->clear_output();
  }
    
  unsigned int takesevents() const {
    return ((fltk3::Widget*)_p)->takesevents();
  }
  
  unsigned int changed() const {
    return ((fltk3::Widget*)_p)->changed();
  }
  
  void set_changed() {
    ((fltk3::Widget*)_p)->set_changed();
  }
  
  void clear_changed() {
    ((fltk3::Widget*)_p)->clear_changed();
  }
  
  int take_focus() {
    return ((fltk3::Widget*)_p)->take_focus();
  }
    
  void set_visible_focus() {
    ((fltk3::Widget*)_p)->set_visible_focus();
  }

  void clear_visible_focus() {
    ((fltk3::Widget*)_p)->clear_visible_focus();
  }

  void visible_focus(int v) {
    ((fltk3::Widget*)_p)->visible_focus(v);
  }
  
  unsigned int  visible_focus() {
    return ((fltk3::Widget*)_p)->visible_focus();
  }
    
  static void default_callback(Fl_Widget *cb, void *d) {
    fltk3::Widget::default_callback( (fltk3::Widget*)(cb->_p), d );
  }
  
  void do_callback() {
    ((fltk3::Widget*)_p)->do_callback();
  }
  
  void do_callback(Fl_Widget* o,long arg) {
    ((fltk3::Widget*)_p)->do_callback( (fltk3::Widget*)(o->_p), arg );
  }
  
  void do_callback(Fl_Widget* o,void* arg=0) {
    ((fltk3::Widget*)_p)->do_callback( (fltk3::Widget*)(o->_p), arg );
  }
    
  int test_shortcut() {
    return ((fltk3::Widget*)_p)->test_shortcut();
  }

  static unsigned int label_shortcut(const char *t) {
    return fltk3::Widget::label_shortcut(t);
  }

  static int test_shortcut(const char *s, const bool require_alt = false) {
    return fltk3::Widget::test_shortcut(s, require_alt);
  }
  
  int contains(const Fl_Widget *w) const {
    return ((fltk3::Widget*)_p)->contains((fltk3::Widget*)(w->_p));
  }

  int inside(const Fl_Widget* w) const {
    return ((fltk3::Widget*)_p)->inside((fltk3::Widget*)(w->_p));
  }
  
  void redraw() {
    ((fltk3::Widget*)_p)->redraw();
  }

  void redraw_label() {
    ((fltk3::Widget*)_p)->redraw_label();
  }

  uchar damage() const {
    return fltk3::_3to1_damage( ((fltk3::Widget*)_p)->damage() );
  }
    
  void clear_damage(uchar c = 0) {
    ((fltk3::Widget*)_p)->clear_damage();
  }
  
  void damage(uchar c) {
    ((fltk3::Widget*)_p)->damage( fltk3::_1to3_damage((Fl_Damage)c) );
  }
  
  void damage(uchar c, int x, int y, int w, int h) {
    ((fltk3::Widget*)_p)->damage( fltk3::_1to3_damage((Fl_Damage)c), x, y, w, h );
  }
  
  void draw_label(int x, int y, int w, int h, Fl_Align a) const {
    ((fltk3::Widget*)_p)->draw_label( x, y, w, h, fltk3::_1to3_align(a) );
  }
  
  void measure_label(int& ww, int& hh) const {
    ((fltk3::Widget*)_p)->measure_label( ww, hh );
  }
  
  Fl_Color color2() const {
    return fltk3::_3to1_color(((fltk3::Widget*)_p)->color2());
  }
  
  void color2(unsigned a) {
    ((fltk3::Widget*)_p)->color2( fltk3::_1to3_color(a) );
  }
};

#define FL_RESERVED_TYPE 100

#endif

//
// End of "$Id$".
//
