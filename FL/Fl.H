//
// "$Id: Fl.H,v 1.18 1999/09/19 04:58:52 vincent Exp $"
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

#include "Enumerations.H"
#include "Fl_Boxtype.H"

// These header files are only needed for the obsolete functions:
#include "Fl_Color.H"

class Fl_Widget;
class Fl_Window;

class Fl {
  Fl() {}; // no constructor!

public: // should be private!

  FL_IMPORT static int e_x,e_y,e_x_root,e_y_root;
  FL_IMPORT static int e_state;
  FL_IMPORT static int e_clicks;
  FL_IMPORT static int e_is_click;
  FL_IMPORT static int e_keysym;
  FL_IMPORT static char* e_text;
  FL_IMPORT static int e_length;
  FL_IMPORT static const char *encoding;
  FL_IMPORT static Fl_Widget* belowmouse_;
  FL_IMPORT static Fl_Widget* pushed_;
  FL_IMPORT static Fl_Widget* focus_;
  FL_IMPORT static int damage_;
  FL_IMPORT static Fl_Widget* selection_owner_;
  FL_IMPORT static Fl_Window* modal_;
  FL_IMPORT static Fl_Window* grab_;

  FL_IMPORT static void damage(int x) {damage_ = x;}

  FL_IMPORT static void (*idle)();

public:

  // argument parsers:
  FL_IMPORT static int arg(int, char**, int&);
  FL_IMPORT static int args(int, char**, int&, int (*)(int,char**,int&) = 0);
  FL_IMPORT static const char* const help;
  FL_IMPORT static void args(int, char**);

  // things called by initialization:
  FL_IMPORT static void display(const char*);
  FL_IMPORT static int visual(int);
  FL_IMPORT static int gl_visual(int, int *alist=0);
  FL_IMPORT static void own_colormap();

  // execution:
  FL_IMPORT static int wait();
  FL_IMPORT static double wait(double time);
  FL_IMPORT static int check();
  FL_IMPORT static int ready();
  FL_IMPORT static int run();
  FL_IMPORT static Fl_Widget* readqueue();
  FL_IMPORT static void add_timeout(double t,void (*cb)(void*),void* = 0);
  FL_IMPORT static void remove_timeout(void (*cb)(void*), void* = 0);
  FL_IMPORT static void add_fd(int fd, int when, void (*cb)(int,void*),void* =0);
  FL_IMPORT static void add_fd(int fd, void (*cb)(int, void*), void* = 0);
  FL_IMPORT static void remove_fd(int, int when);
  FL_IMPORT static void remove_fd(int);
  FL_IMPORT static void set_idle(void (*cb)()) {idle = cb;}
  FL_IMPORT static void add_idle(void (*cb)(void*), void* = 0);
  FL_IMPORT static void remove_idle(void (*cb)(void*), void* = 0);
  FL_IMPORT static int damage() {return damage_;}
  FL_IMPORT static void redraw();
  FL_IMPORT static void flush();
  FL_IMPORT static void (*warning)(const char*, ...);
  FL_IMPORT static void (*error)(const char*, ...);
  FL_IMPORT static void (*fatal)(const char*, ...);
  FL_IMPORT static Fl_Window* first_window();
  FL_IMPORT static Fl_Window* next_window(const Fl_Window*);
  FL_IMPORT static Fl_Window* modal() {return modal_;}
  FL_IMPORT static Fl_Window* grab() {return grab_;}
  FL_IMPORT static void grab(Fl_Window*);
  FL_IMPORT static void grab(Fl_Window&w) {grab(&w);}
  FL_IMPORT static void release() {grab(0);}

  // event information:
  FL_IMPORT static int event_x()		{return e_x;}
  FL_IMPORT static int event_y()		{return e_y;}
  FL_IMPORT static int event_x_root()	{return e_x_root;}
  FL_IMPORT static int event_y_root()	{return e_y_root;}
  FL_IMPORT static void get_mouse(int &,int &);
  FL_IMPORT static int event_clicks()	{return e_clicks;}
  FL_IMPORT static void event_clicks(int i) {e_clicks = i;}
  FL_IMPORT static int event_is_click()	{return e_is_click;}
  FL_IMPORT static void event_is_click(int i) {e_is_click = i;} // only 0 works!
  FL_IMPORT static int event_button()	{return e_keysym-FL_Button;}
  FL_IMPORT static int event_state()	{return e_state;}
  FL_IMPORT static int event_state(int i) {return e_state&i;}
  FL_IMPORT static int event_key()	{return e_keysym;}
  FL_IMPORT static int event_key(int);
  FL_IMPORT static int get_key(int);
  FL_IMPORT static const char* event_text() {return e_text;}
  FL_IMPORT static int event_length() {return e_length;}
  FL_IMPORT static int event_inside(int,int,int,int);
  FL_IMPORT static int event_inside(const Fl_Widget*);
  FL_IMPORT static int test_shortcut(int);

  // event destinations:
  FL_IMPORT static int handle(int, Fl_Window*);
  FL_IMPORT static Fl_Widget* belowmouse() {return belowmouse_;}
  FL_IMPORT static void belowmouse(Fl_Widget*);
  FL_IMPORT static Fl_Widget* pushed()	{return pushed_;}
  FL_IMPORT static void pushed(Fl_Widget*);
  FL_IMPORT static Fl_Widget* focus()	{return focus_;}
  FL_IMPORT static void focus(Fl_Widget*);
  FL_IMPORT static void add_handler(int (*h)(int));

  // cut/paste:
  FL_IMPORT static Fl_Widget* selection_owner() {return selection_owner_;}
  FL_IMPORT static void selection_owner(Fl_Widget*);
  FL_IMPORT static void selection(Fl_Widget &owner, const char* stuff, int len);
  FL_IMPORT static void paste(Fl_Widget &receiver);

  // screen size:
  FL_IMPORT static int x();
  FL_IMPORT static int y();
  FL_IMPORT static int w();
  FL_IMPORT static int h();

#ifndef FLTK_2  // back-compatability section:
  // commented-out functions could not be emulated in fltk 2.0

  FL_IMPORT static void set_color(Fl_Color c,uchar r,uchar g,uchar b) {fl_set_color(c,fl_rgb(r,g,b));}
  FL_IMPORT static void set_color(Fl_Color c, Fl_Color v) {fl_set_color(c, v);}
  FL_IMPORT static unsigned get_color(Fl_Color c) {return fl_get_color(c);}
  FL_IMPORT static void get_color(Fl_Color c,uchar& r,uchar& g,uchar& b) {fl_get_color(c,r,g,b);}
  FL_IMPORT static void free_color(Fl_Color c, int = 0) {fl_free_color(c);}

  FL_IMPORT static void background(uchar r, uchar g, uchar b) {fl_background(fl_rgb(r,g,b));}
  FL_IMPORT static void foreground(uchar r, uchar g, uchar b) {fl_foreground(fl_rgb(r,g,b));}

  //FL_IMPORT static const char* get_font(Fl_Font);
  //FL_IMPORT static const char* get_font_name(Fl_Font, int* attributes = 0);
  //FL_IMPORT static int get_font_sizes(Fl_Font, int*& sizep);
  //FL_IMPORT static void set_font(Fl_Font, const char*);
  //FL_IMPORT static void set_font(Fl_Font, Fl_Font);
  //FL_IMPORT static Fl_Font set_fonts(const char* = 0);
  //FL_IMPORT static void set_encoding(const char *e, const char *loc);

  //FL_IMPORT static void set_labeltype(Fl_Labeltype,y,z);
  //FL_IMPORT static void set_labeltype(Fl_Labeltype, Fl_Labeltype from);
  FL_IMPORT static void enable_symbols();

  //FL_IMPORT static void set_boxtype(Fl_Boxtype,Fl_Box_Draw_F*,uchar,uchar,uchar,uchar);
  //FL_IMPORT static void set_boxtype(Fl_Boxtype, Fl_Boxtype from);
  FL_IMPORT static int box_dx(Fl_Boxtype b) {return b->dx();}
  FL_IMPORT static int box_dy(Fl_Boxtype b) {return b->dy();}
  FL_IMPORT static int box_dw(Fl_Boxtype b) {return b->dw();}
  FL_IMPORT static int box_dh(Fl_Boxtype b) {return b->dh();}

  FL_IMPORT static void set_abort(void (*f)(const char*,...)) {fatal = f;}
  FL_IMPORT static int event_shift() {return e_state&FL_SHIFT;}
  FL_IMPORT static int event_ctrl() {return e_state&FL_CTRL;}
  FL_IMPORT static int event_alt() {return e_state&FL_ALT;}
  FL_IMPORT static int event_buttons() {return e_state&0x7f000000;}
  FL_IMPORT static int event_button1() {return e_state&FL_BUTTON1;}
  FL_IMPORT static int event_button2() {return e_state&FL_BUTTON2;}
  FL_IMPORT static int event_button3() {return e_state&FL_BUTTON3;}
#endif

};

#endif

//
// End of "$Id: Fl.H,v 1.18 1999/09/19 04:58:52 vincent Exp $".
//
