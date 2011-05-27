//
// "$Id$"
//
// Main header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_H
#  define Fl_H

#include <fltk3/run.h>
#include <fltk3/Widget.h>

#ifdef FLTK_HAVE_CAIRO
# include <FL/Fl_Cairo.H>
#endif

#  include "fl_utf8.h"
#  include "Enumerations.H"

#  ifdef check
#    undef check
#  endif

class Fl_Widget;
class Fl_Window;
class Fl_Image;
struct Fl_Label;

typedef void (Fl_Label_Draw_F)(const Fl_Label *label, int x, int y, int w, int h, Fl_Align align);
typedef void (Fl_Label_Measure_F)(const Fl_Label *label, int &width, int &height);
typedef void (Fl_Box_Draw_F)(int x, int y, int w, int h, Fl_Color color);
typedef void (*Fl_Timeout_Handler)(void *data);
typedef void (*Fl_Awake_Handler)(void *data);
typedef void (*Fl_Idle_Handler)(void *data);
typedef void (*Fl_Old_Idle_Handler)();
typedef void (*Fl_FD_Handler)(int fd, void *data);
typedef int (*Fl_Event_Handler)(int event);
typedef void (*Fl_Abort_Handler)(const char *format,...);
typedef void (*Fl_Atclose_Handler)(Fl_Window *window, void *data);
typedef int (*Fl_Args_Handler)(int argc, char **argv, int &i);
typedef int (*Fl_Event_Dispatch)(int event, Fl_Window *w);

class FL_EXPORT Fl {
#if 0 // FIXME: 123  
  Fl() {};
#endif
public: // should be private!
#if 0 // FIXME: 123
  static void damage(int d) {damage_ = d;}
  typedef enum {
    OPTION_ARROW_FOCUS = 0,
    OPTION_VISIBLE_FOCUS,
    OPTION_DND_TEXT,
    OPTION_SHOW_TOOLTIPS,
    OPTION_LAST
  } Fl_Option;
  static bool option(Fl_Option opt);
  static void option(Fl_Option opt, bool val);
  static void (*idle)();
  static int add_awake_handler_(Fl_Awake_Handler, void*);
  static int get_awake_handler_(Fl_Awake_Handler&, void*&);
  static double version();
#endif
  
  static int arg(int argc, char **argv, int& i) {
    return fltk3::arg(argc, argv, i);
  }
  
  static int args(int argc, char **argv, int& i, Fl_Args_Handler cb = 0) {
    return fltk3::args(argc, argv, i, cb);
  }
  
  static void args(int argc, char **argv) {
    fltk3::args(argc, argv);
  }
  
#if 0 // FIXME: 123
  static const char* const help;
  static void display(const char*);
#endif
  
  static int visual(int m) {
    return fltk3::visual(m);
  }
  
#if 0 // FIXME: 123
  static int gl_visual(int, int *alist=0); // platform dependent
  static void own_colormap();
  static void get_system_colors();
  static void foreground(uchar, uchar, uchar);
  static void background(uchar, uchar, uchar);
  static void background2(uchar, uchar, uchar);
  static int scheme(const char*);
  static const char* scheme() {return scheme_;}
  static int reload_scheme(); // platform dependent
  static int scrollbar_size();
  static void scrollbar_size(int W);
#endif
  
  static int wait() {
    return fltk3::wait(); 
  }
  
  static double wait(double time) {
    return fltk3::wait(time); 
  }
  
  static int check() {
    return fltk3::check(); 
  }
  
  static int ready() {
    return fltk3::ready(); 
  }
  
  static int run() { 
    return fltk3::run(); 
  }

  static Fl_Widget *readqueue() {
    return (Fl_Widget*)(fltk3::readqueue()->wrapper());
  }
  
#if 0 // FIXME: 123
  static void add_timeout(double t, Fl_Timeout_Handler,void* = 0); // platform dependent
  static void repeat_timeout(double t, Fl_Timeout_Handler, void* = 0); // platform dependent
  static int  has_timeout(Fl_Timeout_Handler, void* = 0);
  static void remove_timeout(Fl_Timeout_Handler, void* = 0);
  static void add_check(Fl_Timeout_Handler, void* = 0);
  static int  has_check(Fl_Timeout_Handler, void* = 0);
  static void remove_check(Fl_Timeout_Handler, void* = 0);
  static void add_fd(int fd, int when, Fl_FD_Handler cb, void* = 0); // platform dependent
  static void add_fd(int fd, Fl_FD_Handler cb, void* = 0); // platform dependent
  static void remove_fd(int, int when); // platform dependent
  static void remove_fd(int); // platform dependent
  static void add_idle(Fl_Idle_Handler cb, void* data = 0);
  static int  has_idle(Fl_Idle_Handler cb, void* data = 0);
  static void remove_idle(Fl_Idle_Handler cb, void* data = 0);
  static int damage() {return damage_;}
  static void redraw();
  static void flush();
  static void (*warning)(const char*, ...);
  static void (*error)(const char*, ...);
#endif
  
  static void fatal(const char*, ...) {
    // FIXME: 123 - this is a function pointer. Not sure if we can wrap this
    // FIXME: 123 - also, to wrap the varargs, we need the appropriate va function in fltk3
  }
  
#if 0 // FIXME: 123
  static Fl_Window* first_window();
  static void first_window(Fl_Window*);
  static Fl_Window* next_window(const Fl_Window*);
  static Fl_Window* modal() {return modal_;}
  static Fl_Window* grab() {return grab_;}
  static void grab(Fl_Window*); // platform dependent
  static int event()		{return e_number;}
  static int event_x()	{return e_x;}
  static int event_y()	{return e_y;}
  static int event_x_root()	{return e_x_root;}
  static int event_y_root()	{return e_y_root;}
  static int event_dx()	{return e_dx;}
  static int event_dy()	{return e_dy;}
  static void get_mouse(int &,int &); // platform dependent
  static int event_clicks()	{return e_clicks;}
  static void event_clicks(int i) {e_clicks = i;}
  static int event_is_click()	{return e_is_click;}
  static void event_is_click(int i) {e_is_click = i;}
  static int event_button()	{return e_keysym-FL_Button;}
  static int event_state()	{return e_state;}
  static int event_state(int i) {return e_state&i;}
  static int event_key()	{return e_keysym;}
  static int event_original_key(){return e_original_keysym;}
  static int event_key(int key);
  static int get_key(int key); // platform dependent
  static const char* event_text() {return e_text;}
  static int event_length() {return e_length;}
  static int compose(int &del);
  static void compose_reset();
  static int event_inside(int,int,int,int);
  static int event_inside(const Fl_Widget*);
  static int test_shortcut(Fl_Shortcut);
  static int handle(int, Fl_Window*);
  static int handle_(int, Fl_Window*);
  static Fl_Widget* belowmouse() {return belowmouse_;}
  static void belowmouse(Fl_Widget*);
  static Fl_Widget* pushed()	{return pushed_;}
  static void pushed(Fl_Widget*);
  static Fl_Widget* focus()	{return focus_;}
  static void focus(Fl_Widget*);
  static void add_handler(Fl_Event_Handler h);
  static void remove_handler(Fl_Event_Handler h);
  static void event_dispatch(Fl_Event_Dispatch d);
  static Fl_Event_Dispatch event_dispatch();
  static void copy(const char* stuff, int len, int destination = 0); // platform dependent
  static void paste(Fl_Widget &receiver, int source /*=0*/); // platform dependent
  static int dnd(); // platform dependent
  static Fl_Widget* selection_owner() {return selection_owner_;}
  static void selection_owner(Fl_Widget*);
  static void selection(Fl_Widget &owner, const char*, int len);
  static void paste(Fl_Widget &receiver);
  static int x(); // platform dependent
  static int y(); // platform dependent
  static int w(); // platform dependent
  static int h(); // platform dependent
  static int screen_count();
  static void screen_xywh(int &X, int &Y, int &W, int &H) {    screen_xywh(X, Y, W, H, e_x_root, e_y_root);  }
  static void screen_xywh(int &X, int &Y, int &W, int &H, int mx, int my);
  static void screen_xywh(int &X, int &Y, int &W, int &H, int n); 
  static void screen_xywh(int &X, int &Y, int &W, int &H, int mx, int my, int mw, int mh);
  static void screen_dpi(float &h, float &v, int n=0);
  static void	set_color(Fl_Color, uchar, uchar, uchar);
  static void	set_color(Fl_Color i, unsigned c); // platform dependent
  static unsigned get_color(Fl_Color i);
  static void	get_color(Fl_Color i, uchar &red, uchar &green, uchar &blue);
  static void	free_color(Fl_Color i, int overlay = 0); // platform dependent
  static const char* get_font(Fl_Font);
  static const char* get_font_name(Fl_Font, int* attributes = 0);
  static int get_font_sizes(Fl_Font, int*& sizep);
  static void set_font(Fl_Font, const char*);
  static void set_font(Fl_Font, Fl_Font);
  static Fl_Font set_fonts(const char* = 0); // platform dependent
  static void set_labeltype(Fl_Labeltype,Fl_Label_Draw_F*,Fl_Label_Measure_F*);
  static void set_labeltype(Fl_Labeltype, Fl_Labeltype from); // is it defined ?
  static Fl_Box_Draw_F *get_boxtype(Fl_Boxtype);
  static void set_boxtype(Fl_Boxtype, Fl_Box_Draw_F*,uchar,uchar,uchar,uchar);
  static void set_boxtype(Fl_Boxtype, Fl_Boxtype from);
  static int box_dx(Fl_Boxtype);
  static int box_dy(Fl_Boxtype);
  static int box_dw(Fl_Boxtype);
  static int box_dh(Fl_Boxtype);
  static int draw_box_active();
  static void set_abort(Fl_Abort_Handler f) {fatal = f;}
  static void (*atclose)(Fl_Window*,void*);
  static void default_atclose(Fl_Window*,void*);
  static void set_atclose(Fl_Atclose_Handler f) {atclose = f;}
  static int event_shift() {return e_state&FL_SHIFT;}
  static int event_ctrl() {return e_state&FL_CTRL;}
  static int event_command() {return e_state&FL_COMMAND;}
  static int event_alt() {return e_state&FL_ALT;}
  static int event_buttons() {return e_state&0x7f000000;}
  static int event_button1() {return e_state&FL_BUTTON1;}
  static int event_button2() {return e_state&FL_BUTTON2;}
  static int event_button3() {return e_state&FL_BUTTON3;}
  static void set_idle(Fl_Old_Idle_Handler cb) {idle = cb;}
  static void grab(Fl_Window& win) {grab(&win);}
  static void release() {grab(0);}
  static void visible_focus(int v) { option(OPTION_VISIBLE_FOCUS, (v!=0)); }
  static int  visible_focus() { return option(OPTION_VISIBLE_FOCUS); }
  static void dnd_text_ops(int v) { option(OPTION_DND_TEXT, (v!=0)); }
  static int  dnd_text_ops() { return option(OPTION_DND_TEXT); }
  static int lock();
  static void unlock();
  static void awake(void* message = 0);
  static int awake(Fl_Awake_Handler cb, void* message = 0);
  static void* thread_message(); // platform dependent
  static void delete_widget(Fl_Widget *w);
  static void do_widget_deletion();
  static void watch_widget_pointer(Fl_Widget *&w);
  static void release_widget_pointer(Fl_Widget *&w);
  static void clear_widget_pointer(Fl_Widget const *w);
#ifdef FLTK_HAVE_CAIRO
public:
  static cairo_t * cairo_make_current(Fl_Window* w);
  static void cairo_autolink_context(bool alink) {cairo_state_.autolink(alink);}
  static bool cairo_autolink_context() {return cairo_state_.autolink();}
  static cairo_t * cairo_cc() { return cairo_state_.cc(); }
  static void cairo_cc(cairo_t * c, bool own=false){ cairo_state_.cc(c, own); } 
private:
  static cairo_t * cairo_make_current(void* gc);
  static cairo_t * cairo_make_current(void* gc, int W, int H);
  static Fl_Cairo_State cairo_state_;
public:
#endif // FLTK_HAVE_CAIRO
#endif
};


#if 0 // FIXME: 123

class FL_EXPORT Fl_Widget_Tracker {
public:
  Fl_Widget_Tracker(Fl_Widget *wi);
  ~Fl_Widget_Tracker();
  Fl_Widget *widget() {return wp_;}
  int deleted() {return wp_ == 0;}
  int exists()  {return wp_ != 0;}
};

#endif

#endif // !Fl_H

//
// End of "$Id$".
//
