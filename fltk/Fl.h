//
// "$Id: Fl.h,v 1.9 2001/12/06 18:23:43 spitzak Exp $"
//
// Main header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_H
#define Fl_H

#include "Enumerations.h"
#include "Fl_Boxtype.h"
#include "Fl_Style.h"

// These header files are only needed for the obsolete functions:
#include "Fl_Color.h"

class FL_API Fl_Widget;
class FL_API Fl_Window;
struct Fl_Screen_Info {
  int x, y, w, h;
  int width;
  int height;
  int depth;
  int width_mm;
  int height_mm;
};

typedef void (*Fl_Timeout_Handler)(void*);

class FL_API Fl {
  Fl() {}; // no constructor!

public: // should be private!

  static int damage_;
  static int e_type;
  static int e_x;
  static int e_y;
  static int e_dx;
  static int e_dy;
  static int e_x_root;
  static int e_y_root;
  static int e_state;
  static int e_clicks;
  static int e_is_click;
  static int e_keysym;
  static int e_length;
  static char* e_text;
  static int compose_state;
  static Fl_Widget* belowmouse_;
  static Fl_Widget* pushed_;
  static Fl_Widget* focus_;
  static Fl_Widget* modal_;
  static bool grab_;
  static bool exit_modal_;

  static void damage(int x) {damage_ = x;}

  static void (*idle)();

  static const char* scheme_;

public:

  static double version();

  // argument parsers:
  static int arg(int, char**, int&);
  static int args(int, char**, int&, int (*)(int,char**,int&) = 0);
  static const char* const help;
  static void args(int, char**);

  // themes & schemes:
  static void enable_themes(int);
  static int theme(const char*);
  static void enable_schemes(int);
  static int scheme(const char*);
  static const char* scheme() {return scheme_;}
  static int reload_scheme();
  static void startup();

  // things called by initialization:
  static void display(const char*);
  static int visual(int);
  static void own_colormap();

  // execution:
  static int wait();
  static int wait(double time);
  static int check();
  static int ready();
  static int run();

  static void modal(Fl_Widget*, bool grab = false);
  static Fl_Widget* modal() {return modal_;}
  static bool grab() {return grab_;}
  static void exit_modal() {exit_modal_ = true;}
  static bool exit_modal_flag() {return exit_modal_;}

  static void add_timeout(double t,Fl_Timeout_Handler,void* v = 0);
  static void repeat_timeout(double t, Fl_Timeout_Handler,void* = 0);
  static bool has_timeout(Fl_Timeout_Handler, void* = 0);
  static void remove_timeout(Fl_Timeout_Handler, void* = 0);
  static void add_check(Fl_Timeout_Handler, void* = 0);
  static bool has_check(Fl_Timeout_Handler, void* = 0);
  static void remove_check(Fl_Timeout_Handler, void* = 0);
  static void add_fd(int fd, int when, void (*cb)(int,void*),void* =0);
  static void add_fd(int fd, void (*cb)(int, void*), void* = 0);
  static void remove_fd(int, int when = -1);
  static void set_idle(void (*cb)()) {idle = cb;}
  static void add_idle(void (*cb)(void*), void* = 0);
  static bool has_idle(void (*cb)(void*), void* = 0);
  static void remove_idle(void (*cb)(void*), void* = 0);
  static int damage() {return damage_;}
  static void redraw();
  static void flush();
  static void (*warning)(const char*, ...);
  static void (*error)(const char*, ...);
  static void (*fatal)(const char*, ...);

  // list of top-level shown() windows:
  static Fl_Window* first_window();
  static void first_window(Fl_Window*);
  static Fl_Window* next_window(const Fl_Window*);

  // event information:
  static int event()		{return e_type;}
  static int event_x()		{return e_x;}
  static int event_y()		{return e_y;}
  static int event_dx()		{return e_dx;}
  static int event_dy()		{return e_dy;}
  static int event_x_root()	{return e_x_root;}
  static int event_y_root()	{return e_y_root;}
  static int event_clicks()	{return e_clicks;}
  static void event_clicks(int i) {e_clicks = i;}
  static bool event_is_click()	{return e_is_click != 0;}
  static void event_is_click(bool) {e_is_click = 0;} // only false works!
  static int event_button()	{return e_keysym-FL_Button(0);}
  static int event_state()	{return e_state;}
  static bool event_state(int i) {return (e_state&i) != 0;}
  static int event_key()	{return e_keysym;}
  static bool event_key(int);
  static const char* event_text() {return e_text;}
  static int event_length() {return e_length;}

  static bool event_inside(int,int,int,int);
  static bool test_shortcut(int shortcut);
  static const char* key_name(int key);
  static bool compose(int &del);
  static void compose_reset() {compose_state = 0;}

  static bool get_key(int);
  static void get_mouse(int &,int &);

  // event destinations:
  static bool handle(int, Fl_Window*);
  static void add_handler(int (*h)(int));
  static Fl_Widget* belowmouse() {return belowmouse_;}
  static void belowmouse(Fl_Widget*);
  static void belowmouse(Fl_Widget& w) {belowmouse(&w);}
  static Fl_Widget* pushed()	{return pushed_;}
  static void pushed(Fl_Widget*);
  static void pushed(Fl_Widget& w) {pushed(&w);}
  static Fl_Widget* focus()	{return focus_;}
  static void focus(Fl_Widget*);
  static void focus(Fl_Widget& w) {focus(&w);}

  // cut/paste:
  static void copy(const char* stuff, int len, bool clipboard = false);
  static void paste(Fl_Widget &receiver, bool clipboard = false);
  static bool dnd();

  // screen size (any any other static information from system):
  static const Fl_Screen_Info& info();

#ifndef FLTK_2  // back-compatability section:
  // commented-out functions could not be emulated in fltk 2.0
  static int x() {return info().x;}
  static int y() {return info().y;}
  static int w() {return info().width;}
  static int h() {return info().height;}

  static void set_color(Fl_Color c,uchar r,uchar g,uchar b) {fl_set_color(c,fl_rgb(r,g,b));}
  static void set_color(Fl_Color c, Fl_Color v) {fl_set_color(c, v);}
  static unsigned get_color(Fl_Color c) {return fl_get_color(c);}
  static void get_color(Fl_Color c,uchar& r,uchar& g,uchar& b) {fl_get_color(c,r,g,b);}
  static void free_color(Fl_Color c, int = 0) {fl_free_color(c);}
  static void background(uchar r, uchar g, uchar b) {fl_background(fl_rgb(r,g,b));}
  //static void foreground(uchar r, uchar g, uchar b)

  //static const char* get_font(Fl_Font);
  //static const char* get_font_name(Fl_Font, int* attributes = 0);
  //static int get_font_sizes(Fl_Font, int*& sizep);
  //static void set_font(Fl_Font, const char*);
  //static void set_font(Fl_Font, Fl_Font);
  //static Fl_Font set_fonts(const char* = 0);
  //static void set_encoding(const char *e, const char *loc);

  //static void set_labeltype(Fl_Labeltype,y,z);
  //static void set_labeltype(Fl_Labeltype, Fl_Labeltype from);
  static void enable_symbols();

  //static void set_boxtype(Fl_Boxtype,Fl_Box_Draw_F*,uchar,uchar,uchar,uchar);
  //static void set_boxtype(Fl_Boxtype, Fl_Boxtype from);

  static int box_dx(Fl_Boxtype b) {return b->dx();}
  static int box_dy(Fl_Boxtype b) {return b->dy();}
  static int box_dw(Fl_Boxtype b) {return b->dw();}
  static int box_dh(Fl_Boxtype b) {return b->dh();}

  static void set_abort(void (*f)(const char*,...)) {fatal = f;}
  static bool event_shift() {return (e_state&FL_SHIFT)!=0;}
  static bool event_ctrl() {return (e_state&FL_CTRL)!=0;}
  static bool event_alt() {return (e_state&FL_ALT)!=0;}
  static bool event_buttons() {return (e_state&FL_BUTTONS)!=0;}
  static bool event_button1() {return (e_state&FL_BUTTON1)!=0;}
  static bool event_button2() {return (e_state&FL_BUTTON2)!=0;}
  static bool event_button3() {return (e_state&FL_BUTTON3)!=0;}

#endif

  // Multithreading support:
  static void lock();
  static void unlock();
  static void awake(void* message = 0);
  static void* thread_message();

};


FL_API const char* fl_find_config_file(const char* filename, bool create = false);
FL_API int fl_getconf(const char *key, char *value, int value_length);

#endif

//
// End of "$Id: Fl.h,v 1.9 2001/12/06 18:23:43 spitzak Exp $".
//
