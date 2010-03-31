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

#if 0

/** Default callback type definition for all fltk widgets (by far the most used) */
typedef void (Fl_Callback )(Fl_Widget*, void*);
/** Default callback type pointer definition for all fltk widgets */
typedef Fl_Callback* Fl_Callback_p; // needed for BORLAND
/** One parameter callback type definition passing only the widget */
typedef void (Fl_Callback0)(Fl_Widget*);
/** Callback type definition passing the widget and a long data value */
typedef void (Fl_Callback1)(Fl_Widget*, long);

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
        INACTIVE        = 1<<0,   ///< the widget can't receive focus, and is disabled but potentially visible
        INVISIBLE       = 1<<1,   ///< the widget is not drawn but can receive events
        OUTPUT          = 1<<2,   ///< for output only
        NOBORDER        = 1<<3,   ///< don't draw a decoration (Fl_Window)
        FORCE_POSITION  = 1<<4,   ///< don't let the window manager position the window (Fl_Window)
        NON_MODAL       = 1<<5,   ///< thisis a hovering toolbar window (Fl_Window)
        SHORTCUT_LABEL  = 1<<6,   ///< the label contains a shortcut we need to draw
        CHANGED         = 1<<7,   ///< the widget value changed
        OVERRIDE        = 1<<8,   ///< position window on top (Fl_Window)
        VISIBLE_FOCUS   = 1<<9,   ///< accepts keyboard focus navigation if the widget can have the focus
        COPIED_LABEL    = 1<<10,  ///< the widget label is internally copied, its destruction is handled by the widget
        CLIP_CHILDREN   = 1<<11,  ///< all drawing within this widget will be clipped (Fl_Group)
        MENU_WINDOW     = 1<<12,  ///< a temporary popup window, dismissed by clicking outside (Fl_Window)
        TOOLTIP_WINDOW  = 1<<13,  ///< a temporary popup, transparent to events, and dismissed easily (Fl_Window)
        MODAL           = 1<<14,  ///< a window blocking input to all other winows (Fl_Window)
        NO_OVERLAY      = 1<<15,  ///< window not using a hardware overlay plane (Fl_Menu_Window)
        GROUP_RELATIVE  = 1<<16,  ///< position this idget relative to the parent group, not to the window
        // (space for more flags)
        USERFLAG3       = 1<<29,  ///< reserved for 3rd party extensions
        USERFLAG2       = 1<<30,  ///< reserved for 3rd party extensions
        USERFLAG1       = 1<<31   ///< reserved for 3rd party extensions
  };
  void draw_box() const { _p->draw_box(); }
  void draw_box(Fl_Boxtype t, Fl_Color c) const { _p->draw_box(t, c); }
  void draw_box(Fl_Boxtype t, int x,int y,int w,int h, Fl_Color c) const { _p->draw_box(t, x, y, w, h, c); }
  void draw_focus() { _p->draw_focus(); }
  void draw_focus(Fl_Boxtype t, int x,int y,int w,int h) const { _p->draw_focus(t, x, y, w, h); }
  void draw_label() const { _p->draw_label(); }
  void draw_label(int x, int y, int w, int h) const { _p->draw_label(x, y, w, h); }

public:

  virtual ~Fl_Widget() { }
  virtual void draw() { _p->draw(); }
  virtual int handle(int event) { return _p->handle(event); }
  Fl_Group* parent() const { return (Fl_Group*)_p->parent()->wrapper(); }
  void parent(Fl_Group* p) { _p->parent((fltk3::Group*)(((Fl_Widget*)p)->_p)); }
  uchar type() const { return _p->type(); }
  void type(uchar t) { _p->type(t); }
  
#if 0
  int x() const {return x_;}
  int y() const {return y_;}
  int w() const {return w_;}
  int h() const {return h_;}
  virtual void resize(int x, int y, int w, int h);
  int damage_resize(int,int,int,int);
  void position(int X,int Y) {resize(X,Y,w_,h_);}
  void size(int W,int H) {resize(x_,y_,W,H);}
  Fl_Align align() const {return label_.align_;}
  void align(Fl_Align alignment) {label_.align_ = alignment;}
#endif
  
  Fl_Boxtype box() const { return _p->box(); }
  void box(Fl_Boxtype new_box) { _p->box(new_box); }
  
#if 0
  Fl_Color color() const {return color_;}
  void color(Fl_Color bg) {color_ = bg;}
  Fl_Color selection_color() const {return color2_;}
  void selection_color(Fl_Color a) {color2_ = a;}
  void color(Fl_Color bg, Fl_Color sel) {color_=bg; color2_=sel;}
  const char* label() const {return label_.value;}
  void label(const char* text);
  void copy_label(const char *new_label);
  void label(Fl_Labeltype a, const char* b) {label_.type = a; label_.value = b;}
#endif
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
  const char *tooltip() const {return tooltip_;}
  void tooltip(const char *t);
  Fl_Callback_p callback() const {return callback_;}
  void callback(Fl_Callback* cb, void* p) {callback_=cb; user_data_=p;}
  void callback(Fl_Callback* cb) {callback_=cb;}
  void callback(Fl_Callback0*cb) {callback_=(Fl_Callback*)cb;}
  void callback(Fl_Callback1*cb, long p=0) {callback_=(Fl_Callback*)cb; user_data_=(void*)p;}
  void* user_data() const {return user_data_;}
  void user_data(void* v) {user_data_ = v;}
  long argument() const {return (long)user_data_;}
  void argument(long v) {user_data_ = (void*)v;}
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
