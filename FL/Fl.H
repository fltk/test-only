//
// "$Id: Fl.H,v 1.11 1999/04/11 15:34:20 mike Exp $"
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

#define HAVE_STYLES 1

#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "Enumerations.H"
#ifndef Fl_Object
#define Fl_Object Fl_Widget
#endif

class Fl_Widget;
class Fl_Window;
struct Fl_Label;
typedef void (Fl_Label_Draw_F)(const Fl_Label*, int,int,int,int, Fl_Align);
typedef void (Fl_Label_Measure_F)(const Fl_Label*, int&, int&);
typedef void (Fl_Box_Draw_F)(int,int,int,int, Fl_Color);

class Fl {
  Fl() {}; // no constructor!

public: // should be private!

  static FL_EXPORT int e_x,e_y,e_x_root,e_y_root;
  static FL_EXPORT int e_state;
  static FL_EXPORT int e_clicks;
  static FL_EXPORT int e_is_click;
  static FL_EXPORT int e_keysym;
  static FL_EXPORT char* e_text;
  static FL_EXPORT int e_length;
  static FL_EXPORT const char *encoding;
  static FL_EXPORT Fl_Widget* belowmouse_;
  static FL_EXPORT Fl_Widget* pushed_;
  static FL_EXPORT Fl_Widget* focus_;
  static FL_EXPORT int damage_;
  static FL_EXPORT Fl_Widget* selection_owner_;
  static FL_EXPORT Fl_Window* modal_;
  static FL_EXPORT Fl_Window* grab_;

  static void damage(int x) {damage_ = x;}

  static FL_EXPORT void (*idle)();

public:

  // argument parsers:
  static FL_EXPORT int arg(int, char**, int&);
  static FL_EXPORT int args(int, char**, int&, int (*)(int,char**,int&) = 0);
  static FL_EXPORT const char* const help;
  static FL_EXPORT void args(int, char**);

  // things called by initialization:
  static FL_EXPORT void display(const char*);
  static FL_EXPORT int visual(int);
  static FL_EXPORT int gl_visual(int, int *alist=0);
  static FL_EXPORT void own_colormap();
  static FL_EXPORT void get_system_colors();
  static FL_EXPORT void foreground(uchar, uchar, uchar);
  static FL_EXPORT void background(uchar, uchar, uchar);
  static FL_EXPORT void background2(uchar, uchar, uchar);

  // execution:
  static FL_EXPORT int wait();
  static FL_EXPORT double wait(double time);
  static FL_EXPORT int check();
  static FL_EXPORT int ready();
  static FL_EXPORT int run();
  static FL_EXPORT Fl_Widget* readqueue();
  static FL_EXPORT void add_timeout(double t,void (*cb)(void*),void* = 0);
  static FL_EXPORT void remove_timeout(void (*cb)(void*), void* = 0);
  static FL_EXPORT void add_fd(int fd, int when, void (*cb)(int, void*), void* = 0);
  static FL_EXPORT void add_fd(int fd, void (*cb)(int, void*), void* = 0);
  static FL_EXPORT void remove_fd(int);
  static FL_EXPORT void add_idle(void (*cb)(void*), void* = 0);
  static FL_EXPORT void remove_idle(void (*cb)(void*), void* = 0);
  static FL_EXPORT int damage() {return damage_;}
  static FL_EXPORT void redraw();
  static FL_EXPORT void flush();
  static FL_EXPORT void (*warning)(const char*, ...);
  static FL_EXPORT void (*error)(const char*, ...);
  static FL_EXPORT void (*fatal)(const char*, ...);
  static FL_EXPORT Fl_Window* first_window();
  static FL_EXPORT Fl_Window* next_window(const Fl_Window*);
  static FL_EXPORT Fl_Window* modal() {return modal_;}
  static FL_EXPORT Fl_Window* grab() {return grab_;}
  static FL_EXPORT void grab(Fl_Window*);

  // event information:
  static FL_EXPORT int event_x()		{return e_x;}
  static FL_EXPORT int event_y()		{return e_y;}
  static FL_EXPORT int event_x_root()	{return e_x_root;}
  static FL_EXPORT int event_y_root()	{return e_y_root;}
  static FL_EXPORT void get_mouse(int &,int &);
  static FL_EXPORT int event_clicks()	{return e_clicks;}
  static FL_EXPORT void event_clicks(int i) {e_clicks = i;}
  static FL_EXPORT int event_is_click()	{return e_is_click;}
  static FL_EXPORT void event_is_click(int i) {e_is_click = i;} // only 0 works!
  static FL_EXPORT int event_button()	{return e_keysym-FL_Button;}
  static FL_EXPORT int event_state()	{return e_state;}
  static FL_EXPORT int event_state(int i) {return e_state&i;}
  static FL_EXPORT int event_key()	{return e_keysym;}
  static FL_EXPORT int event_key(int);
  static FL_EXPORT int get_key(int);
  static FL_EXPORT const char* event_text() {return e_text;}
  static FL_EXPORT int event_length() {return e_length;}
  static FL_EXPORT int event_inside(int,int,int,int);
  static FL_EXPORT int event_inside(const Fl_Widget*);
  static FL_EXPORT int test_shortcut(int);

  // event destinations:
  static FL_EXPORT int handle(int, Fl_Window*);
  static FL_EXPORT Fl_Widget* belowmouse() {return belowmouse_;}
  static FL_EXPORT void belowmouse(Fl_Widget*);
  static FL_EXPORT Fl_Widget* pushed()	{return pushed_;}
  static FL_EXPORT void pushed(Fl_Widget*);
  static FL_EXPORT Fl_Widget* focus()	{return focus_;}
  static FL_EXPORT void focus(Fl_Widget*);
  static FL_EXPORT void add_handler(int (*h)(int));

  // cut/paste:
  static FL_EXPORT Fl_Widget* selection_owner() {return selection_owner_;}
  static FL_EXPORT void selection_owner(Fl_Widget*);
  static FL_EXPORT void selection(Fl_Widget &owner, const char* stuff, int len);
  static FL_EXPORT void paste(Fl_Widget &receiver);

  // screen size:
#if defined(WIN32)
  static FL_EXPORT int x();
  static FL_EXPORT int y();
#else
  static FL_EXPORT int x() {return 0;}
  static FL_EXPORT int y() {return 0;}
#endif /* WIN32 */
  static FL_EXPORT int w();
  static FL_EXPORT int h();

  // color map:
  static FL_EXPORT void	set_color(Fl_Color, uchar, uchar, uchar);
  static FL_EXPORT void	set_color(Fl_Color, unsigned);
  static FL_EXPORT unsigned get_color(Fl_Color);
  static FL_EXPORT void	get_color(Fl_Color, uchar&, uchar&, uchar&);
  static FL_EXPORT void	free_color(Fl_Color, int overlay = 0);

  // fonts:
  static FL_EXPORT const char* get_font(Fl_Font);
  static FL_EXPORT const char* get_font_name(Fl_Font, int* attributes = 0);
  static FL_EXPORT int get_font_sizes(Fl_Font, int*& sizep);
  static FL_EXPORT void set_font(Fl_Font, const char*);
  static FL_EXPORT void set_font(Fl_Font, Fl_Font);
  static FL_EXPORT Fl_Font set_fonts(const char* = 0);
  static FL_EXPORT const char *set_encoding(const char *e, const char *loc)
    { if (strncmp(loc, setlocale(LC_ALL, NULL), 2) == 0) encoding=e;}

  // labeltypes:
  static FL_EXPORT void set_labeltype(Fl_Labeltype,Fl_Label_Draw_F*,Fl_Label_Measure_F*);
  static FL_EXPORT void set_labeltype(Fl_Labeltype, Fl_Labeltype from);
  static FL_EXPORT void enable_symbols();

  // boxtypes:
  static FL_EXPORT void set_boxtype(Fl_Boxtype, Fl_Box_Draw_F*,uchar,uchar,uchar,uchar);
  static FL_EXPORT void set_boxtype(Fl_Boxtype, Fl_Boxtype from);
  static FL_EXPORT int box_dx(Fl_Boxtype);
  static FL_EXPORT int box_dy(Fl_Boxtype);
  static FL_EXPORT int box_dw(Fl_Boxtype);
  static FL_EXPORT int box_dh(Fl_Boxtype);

  // back compatability:
  static FL_EXPORT void set_abort(void (*f)(const char*,...)) {fatal = f;}
  static FL_EXPORT void (*atclose)(Fl_Window*,void*);
  static FL_EXPORT void default_atclose(Fl_Window*,void*);
  static FL_EXPORT void set_atclose(void (*f)(Fl_Window*,void*)) {atclose = f;}
  static FL_EXPORT int event_shift() {return e_state&FL_SHIFT;}
  static FL_EXPORT int event_ctrl() {return e_state&FL_CTRL;}
  static FL_EXPORT int event_alt() {return e_state&FL_ALT;}
  static FL_EXPORT int event_buttons() {return e_state&0x7f000000;}
  static FL_EXPORT int event_button1() {return e_state&FL_BUTTON1;}
  static FL_EXPORT int event_button2() {return e_state&FL_BUTTON2;}
  static FL_EXPORT int event_button3() {return e_state&FL_BUTTON3;}
  static FL_EXPORT void set_idle(void (*cb)()) {idle = cb;}
  static FL_EXPORT void grab(Fl_Window&w) {grab(&w);}
  static FL_EXPORT void release() {grab(0);}

  // current style
  static FL_EXPORT const char *_style;
  static FL_EXPORT Fl_Widget_Style _widget_style;

  // get and set style
  static FL_EXPORT const char *style();
  static FL_EXPORT void style(const char *s);
  static FL_EXPORT Fl_Widget_Style widget_style() { return _widget_style; }
  static FL_EXPORT void widget_style(Fl_Widget_Style s) { _widget_style = s; }

  // This defines current style options for FLTK.

  struct Attribute {
    const char* key;
    int which;
  };

  // Widget drawing style

  // styles loaded
  static FL_EXPORT int s_menu_item;
  static FL_EXPORT int s_browser;
  static FL_EXPORT int s_button;
  static FL_EXPORT int s_chart;
  static FL_EXPORT int s_light_button;
  static FL_EXPORT int s_check_button;
  static FL_EXPORT int s_choice;
  static FL_EXPORT int s_menu_;
  static FL_EXPORT int s_menu_bar;
  static FL_EXPORT int s_menu_button;
  static FL_EXPORT int s_widget;
  static FL_EXPORT int s_window;
  static FL_EXPORT int s_slider;
  static FL_EXPORT int s_return_button;
  static FL_EXPORT int s_round_button;
  static FL_EXPORT int s_fly_button;
  static FL_EXPORT int s_scrollbar;
  static FL_EXPORT int s_input;
  static FL_EXPORT int s_output;
  static FL_EXPORT int s_adjuster;
  static FL_EXPORT int s_counter;
  static FL_EXPORT int s_roller;
  static FL_EXPORT int s_tooltip;
  static FL_EXPORT int s_background;
  static FL_EXPORT int s_mi_button; // hack for FLUID

  // force reloading of styles from config file
  static FL_EXPORT void load_styles(int);

  // load a bunch of attributes
  static FL_EXPORT void load_attributes(const char *section, uchar array[], Attribute *attrib);

  // get the string value of a key from the global or user's config file
  static FL_EXPORT int find(const char *key, char *value, int value_length, int sf = 0);

  // get the long value of a key from the global or user's config file
  static FL_EXPORT int find(const char *key, long &lvalue, int sf = 0);

  // get the int value of a key from the global or user's config file
  static FL_EXPORT int find(const char *key, int &ivalue, int sf = 0);

  // get the uchar value of a key from the global or user's config file
  static FL_EXPORT int find(const char *key, uchar &ucvalue, int sf = 0);

  // get the boolean value of a key from the global or user's config file
  static FL_EXPORT int find_boolean(const char *key, int &bvalue, int sf = 0);
};

#endif

//
// End of "$Id: Fl.H,v 1.11 1999/04/11 15:34:20 mike Exp $".
//
