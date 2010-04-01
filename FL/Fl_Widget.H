//
// "$Id$"
//
// Widget header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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

/* \file
   Fl_Widget, Fl_Label classes . */

#ifndef Fltk1_Widget_H
#define Fltk1_Widget_H

#include <fltk3/Wrapper.h>
#include <fltk3/Widget.h>
#include <fltk3/Group.h>

#include "Enumerations.H"

class Fl_Widget;
class Fl_Window;
class Fl_Group;
class Fl_Image;

/** Default callback type definition for all fltk widgets (by far the most used) */
typedef void (Fl_Callback )(Fl_Widget*, void*);
/** Default callback type pointer definition for all fltk widgets */
typedef Fl_Callback* Fl_Callback_p; // needed for BORLAND
/** One parameter callback type definition passing only the widget */
typedef void (Fl_Callback0)(Fl_Widget*);
/** Callback type definition passing the widget and a long data value */
typedef void (Fl_Callback1)(Fl_Widget*, long);

#if 0

/** This struct stores all information for a text or mixed graphics label.

    \todo For FLTK 1.3, the Fl_Label type will become a widget by itself. That way
          we will be avoiding a lot of code duplication by handling labels in 
          a similar fashion to widgets containing text. We also provide an easy
          interface for very complex labels, containing html or vector graphics.
 */
struct FL_EXPORT Fl_Label {
  /** label text */
  const char* value;
  /** optional image for an active label */
  Fl_Image* image;
  /** optional image for a deactivated label */
  Fl_Image* deimage;
  /** label font used in text */
  Fl_Font font;
  /** size of label font */
  Fl_Fontsize size;
  /** text color */
  Fl_Color color;
  /** alignment of label */
  Fl_Align align_;
  /** type of label. \see Fl_Labeltype */
  uchar type;

  /** Draws the label aligned to the given box */
  void draw(int,int,int,int, Fl_Align) const ;
  void measure(int &w, int &h) const ;
};

#endif

class FL_EXPORT Fl_Widget : public fltk3::Wrapper 
{
  friend class Fl_Group;

  Fl_Widget(const Fl_Widget &);
  Fl_Widget& operator=(const Fl_Widget &);

protected:

  Fl_Widget() { }
  Fl_Widget(int x, int y, int w, int h, const char *label=0L) { 
    _p = new fltk3::Widget(x, y, w, h, label); 
    _p->wrapper(this);
  }
  
  void x(int v) { _p->x(v); }
  void y(int v) { _p->y(v); }
  void w(int v) { _p->w(v); }
  void h(int v) { _p->h(v); }
  unsigned int flags() const { return _p->flags(); }
  void set_flag(unsigned int c) { _p->set_flag(c); }
  void clear_flag(unsigned int c) { _p->clear_flag(c); }
  enum {
    INACTIVE        = fltk3::Widget::INACTIVE,
    INVISIBLE       = fltk3::Widget::INVISIBLE,
    OUTPUT          = fltk3::Widget::OUTPUT,
    NOBORDER        = fltk3::Widget::NOBORDER,
    FORCE_POSITION  = fltk3::Widget::FORCE_POSITION,
    NON_MODAL       = fltk3::Widget::NON_MODAL,
    SHORTCUT_LABEL  = fltk3::Widget::SHORTCUT_LABEL,
    CHANGED         = fltk3::Widget::CHANGED,
    OVERRIDE        = fltk3::Widget::OVERRIDE,
    VISIBLE_FOCUS   = fltk3::Widget::VISIBLE_FOCUS,
    COPIED_LABEL    = fltk3::Widget::COPIED_LABEL,
    CLIP_CHILDREN   = fltk3::Widget::CLIP_CHILDREN,
    MENU_WINDOW     = fltk3::Widget::MENU_WINDOW,
    TOOLTIP_WINDOW  = fltk3::Widget::TOOLTIP_WINDOW,
    MODAL           = fltk3::Widget::MODAL,
    NO_OVERLAY      = fltk3::Widget::NO_OVERLAY,
    GROUP_RELATIVE  = fltk3::Widget::GROUP_RELATIVE,
    // (space for more flags)
    USERFLAG3       = fltk3::Widget::USERFLAG3,
    USERFLAG2       = fltk3::Widget::USERFLAG2,
    USERFLAG1       = fltk3::Widget::USERFLAG1
  };
  void draw_box() const { _p->draw_box(); }
  void draw_box(Fl_Boxtype t, Fl_Color c) const { _p->draw_box((fltk3::Boxtype)t, c); }
  void draw_box(Fl_Boxtype t, int x,int y,int w,int h, Fl_Color c) const { _p->draw_box((fltk3::Boxtype)t, x, y, w, h, c); }
  void draw_focus() { _p->draw_focus(); }
  void draw_focus(Fl_Boxtype t, int x,int y,int w,int h) const { _p->draw_focus((fltk3::Boxtype)t, x, y, w, h); }
  void draw_label() const { _p->draw_label(); }
  void draw_label(int x, int y, int w, int h) const { _p->draw_label(x, y, w, h); }

public:

  fltk3::Widget *fltk3Widget() { return (fltk3::Widget*)_p; }
  
  virtual ~Fl_Widget() { }
  virtual void draw() { _p->draw(); }
  virtual int handle(int event) { return _p->handle(event); }
  Fl_Group* parent() const { return (Fl_Group*)_p->parent()->wrapper(); }
  void parent(Fl_Group* p) { _p->parent((fltk3::Group*)(((Fl_Widget*)p)->_p)); }
  uchar type() const { return _p->type(); }
  void type(uchar t) { _p->type(t); }  
  int x() const { return _p->x(); }
  int y() const { return _p->y(); }
  int w() const { return _p->w(); }
  int h() const { return _p->h(); }
  virtual void resize(int x, int y, int w, int h) { _p->resize(x, y, w, h); }
  int damage_resize(int x, int y, int w, int h) { return _p->damage_resize(x, y, w, h); }
  void position(int X,int Y) { _p->position(X, Y); }
  void size(int W,int H) { _p->size(W, H); }
  
  Fl_Align align() const { return (Fl_Align)_p->align(); }
  void align(Fl_Align a) { _p->align((fltk3::Align)a); }
  
  Fl_Boxtype box() const { return (Fl_Boxtype)_p->box(); }
  void box(Fl_Boxtype new_box) { _p->box((fltk3::Boxtype)new_box); }
  
  Fl_Color color() const { return _p->color(); }
  void color(Fl_Color bg) { _p->color(bg); }
  Fl_Color selection_color() const { return _p->selection_color(); }
  void selection_color(Fl_Color a) { _p->selection_color(a); }
  void color(Fl_Color bg, Fl_Color sel) { _p->color(bg, sel); }
  
  const char* label() const { return _p->label(); }
  void label(const char* text) { _p->label(text); }
  void copy_label(const char *new_label) { _p->copy_label(new_label); }
  void label(Fl_Labeltype a, const char* b) { _p->label((fltk3::Labeltype)a, b); }
  Fl_Labeltype labeltype() const { return (Fl_Labeltype)_p->labeltype(); }
  void labeltype(Fl_Labeltype a) { _p->labeltype((fltk3::Labeltype)a); }
  Fl_Color labelcolor() const { return _p->labelcolor(); }
  void labelcolor(Fl_Color c) { _p->labelcolor(c); }
  Fl_Font labelfont() const { return _p->labelfont(); }
  void labelfont(Fl_Font f) { _p->labelfont(f); }
  Fl_Fontsize labelsize() const { return _p->labelsize(); }
  void labelsize(Fl_Fontsize pix) { _p->labelsize(pix); }
  
#if 0
  Fl_Image* image() {return label_.image;}
  void image(Fl_Image* img) {label_.image=img;}
  void image(Fl_Image& img) {label_.image=&img;}
  Fl_Image* deimage() {return label_.deimage;}
  void deimage(Fl_Image* img) {label_.deimage=img;}
  void deimage(Fl_Image& img) {label_.deimage=&img;}
#endif
  
  const char *tooltip() const { return _p->tooltip(); }
  void tooltip(const char *t) { _p->tooltip(t); }
  
  Fl_Callback_p callback() const { return (Fl_Callback_p)_p->callback(); }
  void callback(Fl_Callback *cb, void *p) { _p->callback( (fltk3::Callback_p)cb, p); }
  void callback(Fl_Callback *cb) { _p->callback( (fltk3::Callback_p)cb); }
  void callback(Fl_Callback0 *cb) { _p->callback( (fltk3::Callback_p)cb); }
  void callback(Fl_Callback1 *cb, long p=0) { _p->callback( (fltk3::Callback_p)cb, (void*)p); }
  void *user_data() const { return _p->user_data(); }
  void user_data(void* v) { _p->user_data(v); }
  long argument() const { return _p->argument(); }
  void argument(long v) { _p->argument(v); }
  
#if 0
  Fl_When when() const {return (Fl_When)when_;}
  void when(uchar i) {when_ = i;}
  unsigned int visible() const {return !(flags_&INVISIBLE);}
  int visible_r() const;
  virtual void show();
  virtual void hide();
  void set_visible() {flags_ &= ~INVISIBLE;}
  void clear_visible() {flags_ |= INVISIBLE;}
  unsigned int active() const {return !(flags_&INACTIVE);}
  int active_r() const;
  void activate();
  void deactivate();
  unsigned int output() const {return (flags_&OUTPUT);}
  void set_output() {flags_ |= OUTPUT;}
  void clear_output() {flags_ &= ~OUTPUT;}
  unsigned int takesevents() const {return !(flags_&(INACTIVE|INVISIBLE|OUTPUT));}
  unsigned int changed() const {return flags_&CHANGED;}
  void set_changed() {flags_ |= CHANGED;}
  void clear_changed() {flags_ &= ~CHANGED;}
  int take_focus();
  void set_visible_focus() { flags_ |= VISIBLE_FOCUS; }
  void clear_visible_focus() { flags_ &= ~VISIBLE_FOCUS; }
  void visible_focus(int v) { if (v) set_visible_focus(); else clear_visible_focus(); }
  unsigned int  visible_focus() { return flags_ & VISIBLE_FOCUS; }
  static void default_callback(Fl_Widget *cb, void *d);
  void do_callback() {do_callback(this,user_data_);}
  void do_callback(Fl_Widget* o,long arg) {do_callback(o,(void*)arg);}
  void do_callback(Fl_Widget* o,void* arg=0);
  int test_shortcut();
  static unsigned int label_shortcut(const char *t);
  static int test_shortcut(const char*);
  int contains(const Fl_Widget *w) const ;
  int inside(const Fl_Widget* w) const {return w ? w->contains(this) : 0;}
  void redraw();
  void redraw_label();
  uchar damage() const {return damage_;}
  void clear_damage(uchar c = 0) {damage_ = c;}
  void damage(uchar c);
  void damage(uchar c, int x, int y, int w, int h);
  void draw_label(int, int, int, int, Fl_Align) const;
  void measure_label(int& ww, int& hh) {label_.measure(ww, hh);}
  Fl_Window* window() const ;
  virtual Fl_Group* as_group() {return 0;}
  virtual Fl_Window* as_window() {return 0;}
  virtual class Fl_Gl_Window* as_gl_window() {return 0;}
  Fl_Color color2() const {return (Fl_Color)color2_;}
  void color2(unsigned a) {color2_ = a;}
#endif
  
};

#define FL_RESERVED_TYPE 100

#endif

//
// End of "$Id$".
//
