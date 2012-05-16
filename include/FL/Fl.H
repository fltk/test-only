//
// "$Id$"
//
// Main header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper started
//  - function pointers missing (see FIXME)
//  - callbacks not verified
//
// Copyright 1998-2011 by Bill Spitzak and others.
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
typedef int  (*Fl_Event_Handler)(int event);
typedef void (*Fl_Abort_Handler)(const char *format,...);
typedef void (*Fl_Atclose_Handler)(Fl_Window *window, void *data);
typedef int  (*Fl_Args_Handler)(int argc, char **argv, int &i);
typedef int  (*Fl_Event_Dispatch)(int event, Fl_Window *w);

class FL_EXPORT Fl {
  
  Fl() { /* empty */ };

public: // should be private!
  
#if 0 // FIXME: 123
  // these have not been wrapped yet!
  static void (*idle)();
  static int get_awake_handler_(Fl_Awake_Handler &h, void *&d) {}
  static void (*warning)(const char*, ...);
  static void (*error)(const char*, ...);
  static void (*atclose)(Fl_Window*,void*);
  static void watch_widget_pointer(Fl_Widget *&w);
  static void release_widget_pointer(Fl_Widget *&w);
  static void clear_widget_pointer(Fl_Widget const *w);
#endif

  static void fatal(const char* format, ...) { /* FIXME: 123 */ }

  static void damage(int d) {
    fltk3::damage(d);
  }
  
  typedef enum {
    OPTION_ARROW_FOCUS = 0,
    OPTION_VISIBLE_FOCUS,
    OPTION_DND_TEXT,
    OPTION_SHOW_TOOLTIPS,
    OPTION_LAST
  } Fl_Option;
  
  static bool option(Fl_Option opt) {
    return fltk3::option((fltk3::Option)opt);
  }
  
  static void option(Fl_Option opt, bool val) {
    fltk3::option((fltk3::Option)opt, val);
  }
  
  static int add_awake_handler_(Fl_Awake_Handler h, void *d) {
    return fltk3::add_awake_handler_((fltk3::AwakeHandler)h, d);
  }
  
  static double version() {
    return fltk3::version();
  }
  
  static int arg(int argc, char **argv, int& i) {
    return fltk3::arg(argc, argv, i);
  }
  
  static int args(int argc, char **argv, int& i, Fl_Args_Handler cb = 0) {
    return fltk3::args(argc, argv, i, cb);
  }
  
  static void args(int argc, char **argv) {
    fltk3::args(argc, argv);
  }
  
  static const char* const help_text() {
    return fltk3::help;
  }

  static void display(const char *d) {
    fltk3::display(d);
  }
  
  static int visual(int m) {
    return fltk3::visual(m);
  }
  
  static int gl_visual(int a, int *b=0) {
    return fltk3::gl_visual(a, b);
  }
  
  static void own_colormap() {
    fltk3::own_colormap();
  }
  
  static void get_system_colors() {
    fltk3::get_system_colors();
  }
  
  static void foreground(uchar r, uchar g, uchar b) {
    fltk3::foreground(r, g, b);
  }
  
  static void background(uchar r, uchar g, uchar b) {
    fltk3::background(r, g, b);
  }
  
  static void background2(uchar r, uchar g, uchar b) {
    fltk3::background2(r, g, b);
  }
  
  static int scheme(const char *s) {
    return fltk3::scheme(s);
  }
  
  static const char* scheme() {
    return fltk3::scheme();
  }
  
  static int reload_scheme() {
    return fltk3::reload_scheme();
  }
  
  static int scrollbar_size() {
    return fltk3::scrollbar_size();
  }
  
  static void scrollbar_size(int W) {
    fltk3::scrollbar_size(W);
  }
  
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
    return fltk3::_3to1_widget( fltk3::readqueue() );
  }
  
  static void add_timeout(double t, Fl_Timeout_Handler h, void *p = 0) {
    fltk3::add_timeout(t, (fltk3::TimeoutHandler)h, p);
  }
  
  static void repeat_timeout(double t, Fl_Timeout_Handler h, void *p = 0) {
    fltk3::repeat_timeout(t, (fltk3::TimeoutHandler)h, p);
  }
  
  static int has_timeout(Fl_Timeout_Handler h, void *p = 0) {
    return fltk3::has_timeout((fltk3::TimeoutHandler)h, p);
  }
  
  static void remove_timeout(Fl_Timeout_Handler h, void *p = 0) {
    fltk3::remove_timeout((fltk3::TimeoutHandler)h, p);
  }
  
  static void add_check(Fl_Timeout_Handler h, void *p = 0) {
    fltk3::add_check((fltk3::TimeoutHandler)h, p);
  }
  
  static int has_check(Fl_Timeout_Handler h, void *p = 0) {
    return fltk3::has_check((fltk3::TimeoutHandler)h, p);
  }
  
  static void remove_check(Fl_Timeout_Handler h, void *p = 0) {
    fltk3::remove_check((fltk3::TimeoutHandler)h, p);
  }
  
  static void add_fd(int fd, int when, Fl_FD_Handler cb, void *d = 0) {
    fltk3::add_fd(fd, when, (fltk3::FDHandler)cb, d);
  }
  
  static void add_fd(int fd, Fl_FD_Handler cb, void *d = 0) {
    fltk3::add_fd(fd, (fltk3::FDHandler)cb, d);
  }
  
  static void remove_fd(int fd, int when) {
    fltk3::remove_fd(fd, when);
  }
  
  static void remove_fd(int fd) {
    fltk3::remove_fd(fd);
  }
  
  static void add_idle(Fl_Idle_Handler cb, void* data = 0) {
    fltk3::add_idle((fltk3::IdleHandler)cb, data);
  }
  
  static int  has_idle(Fl_Idle_Handler cb, void* data = 0) {
    return fltk3::has_idle((fltk3::IdleHandler)cb, data);
  }
  
  static void remove_idle(Fl_Idle_Handler cb, void* data = 0) {
    fltk3::remove_idle((fltk3::IdleHandler)cb, data);
  }
  
  static int damage() {
    return fltk3::damage();
  }
  
  static void redraw() {
    fltk3::redraw();
  }
  
  static void flush() {
    fltk3::flush();
  }
      
  static Fl_Window* first_window() {
    return fltk3::_3to1_window( fltk3::first_window() );
  }
  
  static void first_window(Fl_Window *w) {
    fltk3::first_window( fltk3::_1to3_window(w) );
  }
  
  static Fl_Window* next_window(const Fl_Window *w) {
    return fltk3::_3to1_window( fltk3::next_window( fltk3::_1to3_window((Fl_Window*)w) ) );
  }
  
  static Fl_Window* modal() {
    return fltk3::_3to1_window( fltk3::modal() );
  }
  static Fl_Window* grab() {
    return fltk3::_3to1_window( fltk3::grab() );
  }
  
  static void grab(Fl_Window *w) {
    fltk3::grab( fltk3::_1to3_window(w) );
  }
  
  static int event() {
    return fltk3::event();
  }
  
  static int event_x() {
    int retval = fltk3::event_x();
    if (fltk3::e_widget && !fltk3::e_widget->as_window()) retval += fltk3::e_widget->dx_window();
    return retval;
  }
  
  static int event_y() {
    int retval = fltk3::event_y();
    if (fltk3::e_widget && !fltk3::e_widget->as_window()) retval += fltk3::e_widget->dy_window();
    return retval;
  }
  
  static int event_x_root() {
    return fltk3::event_x_root();
  }
  
  static int event_y_root() {
    return fltk3::event_y_root();
  }
  
  static int event_dx() {
    return fltk3::event_dx();
  }
  
  static int event_dy()	{
    return fltk3::event_dy();
  }
  
  static void get_mouse(int &x, int &y) {
    fltk3::get_mouse(x, y);
  }
  
  static int event_clicks() {
    return fltk3::event_clicks();
  }
  
  static void event_clicks(int i) {
    fltk3::event_clicks(i);
  }
  
  static int event_is_click() {
    return fltk3::event_is_click();
  }
  
  static void event_is_click(int i) {
    fltk3::event_is_click(i);
  }
  
  static int event_button() {
    return fltk3::event_button();
  }
  
  static int event_state() {
    return fltk3::event_state();
  }
  
  static int event_state(int i) {
    return fltk3::event_state(i);
  }
  
  static int event_key() {
    return fltk3::event_key();
  }
  
  static int event_original_key() {
    return fltk3::event_original_key();
  }
  
  static int event_key(int key) {
    return fltk3::event_key(key);
  }
  
  static int get_key(int key) {
    return fltk3::get_key(key);
  }
  
  static const char *event_text() {
    return fltk3::event_text();
  }
  
  static int event_length() {
    return fltk3::event_length();
  }
  
  static int compose(int &del) {
    return fltk3::compose(del);
  }
  
  static void compose_reset() {
    fltk3::compose_reset();
  }
  
  static int event_inside(int x, int y, int w, int h) {
    return fltk3::event_inside(x, y, w, h);
  }
  
  static int event_inside(const Fl_Widget *w) {
    return fltk3::event_inside( fltk3::_1to3_widget((Fl_Widget*)w) );
  }
  
  static int test_shortcut(unsigned int s) {
    return fltk3::test_shortcut(s);
  }
  
  static int handle(int e, Fl_Window *w) {
    return fltk3::handle( fltk3::_1to3_event((Fl_Event)e), fltk3::_1to3_window(w) );
  }
  
  static int handle_(int e, Fl_Window *w) {
    return fltk3::handle_( fltk3::_1to3_event((Fl_Event)e), fltk3::_1to3_window(w) );
  }
  
  static Fl_Widget* belowmouse() {
    return fltk3::_3to1_widget( fltk3::belowmouse() );
  }
  
  static void belowmouse(Fl_Widget *w) {
    fltk3::belowmouse(fltk3::_1to3_widget(w));
  }
  
  static Fl_Widget* pushed() {
    return fltk3::_3to1_widget( fltk3::pushed() );
  }
  
  static void pushed(Fl_Widget *w) {
    fltk3::pushed(fltk3::_1to3_widget(w));
  }
  
  static Fl_Widget* focus() {
    return fltk3::_3to1_widget( fltk3::focus() );
  }
    
  static void focus(Fl_Widget *w) {
    fltk3::focus(fltk3::_1to3_widget(w));
  }
  
  static void add_handler(Fl_Event_Handler h) {
    fltk3::add_handler((fltk3::EventHandler)h);
  }
  
  static void remove_handler(Fl_Event_Handler h) {
    fltk3::remove_handler((fltk3::EventHandler)h);
  }
  
  static void event_dispatch(Fl_Event_Dispatch d) {
    fltk3::event_dispatch((fltk3::EventDispatch)d);
  }
  
  static Fl_Event_Dispatch event_dispatch() {
    return 0L; // FIXME: 123 - (fltk3::EventDispatch)fltk3::event_dispatch();
  }
  
  static void copy(const char* stuff, int len, int destination = 0) {
    fltk3::copy(stuff, len, destination);
  }

  static void paste(Fl_Widget &receiver, int source) {
    fltk3::paste( *fltk3::_1to3_widget(&receiver), source);
  }
  
  static int dnd() {
    return fltk3::dnd();
  }
  
  static Fl_Widget* selection_owner() {
    return fltk3::_3to1_widget( fltk3::selection_owner() );
  }
  
  static void selection_owner(Fl_Widget *w) {
    fltk3::selection_owner(fltk3::_1to3_widget(w));
  }
  
  static void selection(Fl_Widget &owner, const char *t, int len) {
    fltk3::selection(*fltk3::_1to3_widget(&owner), t, len);
  }
  
  static void paste(Fl_Widget &receiver) {
    fltk3::paste(*fltk3::_1to3_widget(&receiver));
  }
  
  static int x() {
    return fltk3::x();
  }
  
  static int y() {
    return fltk3::y();
  }

  static int w() {
    return fltk3::w();
  }

  static int h() {
    return fltk3::h();
  }

  static int screen_count() {
    return fltk3::screen_count();
  }
  
  static void screen_xywh(int &X, int &Y, int &W, int &H) {
    fltk3::screen_xywh(X, Y, W, H);
  }
  
  static void screen_xywh(int &X, int &Y, int &W, int &H, int mx, int my) {
    fltk3::screen_xywh(X, Y, W, H, mx, my);
  }
  
  static void screen_xywh(int &X, int &Y, int &W, int &H, int n) {
    fltk3::screen_xywh(X, Y, W, H, n);
  }
  
  static void screen_xywh(int &X, int &Y, int &W, int &H, int mx, int my, int mw, int mh) {
    fltk3::screen_xywh(X, Y, W, H, mx, my, mw, mh);
  }
  
  static void screen_dpi(float &h, float &v, int n=0) {
    fltk3::screen_dpi(h, v, n);
  }
  
  static void set_color(Fl_Color c, uchar r, uchar g, uchar b) {
    fltk3::set_color(c, r, g, b);
  }
  
  static void set_color(Fl_Color i, unsigned c) {
    fltk3::set_color(i, c);
  }
  
  static unsigned get_color(Fl_Color i) {
    return fltk3::get_color(i);
  }
  
  static void get_color(Fl_Color i, uchar &red, uchar &green, uchar &blue) {
    fltk3::get_color(i, red, green, blue);
  }
  
  static void free_color(Fl_Color i, int overlay = 0) {
    fltk3::free_color(i, overlay);
  }
  
  static const char* get_font(Fl_Font f) {
    return fltk3::get_font(fltk3::_1to3_font(f));
  }
  
  static const char* get_font_name(Fl_Font f, int* attributes = 0) {
    return fltk3::get_font_name(fltk3::_1to3_font(f), attributes);
  }
  
  static int get_font_sizes(Fl_Font f, int*& sizep) {
    return fltk3::get_font_sizes(fltk3::_1to3_font(f), sizep);
  }
  
  static void set_font(Fl_Font f, const char *t) {
    fltk3::set_font(fltk3::_1to3_font(f), t);
  }
  
  static void set_font(Fl_Font a, Fl_Font b) {
    fltk3::set_font(fltk3::_1to3_font(a), fltk3::_1to3_font(b));
  }
  
  static Fl_Font set_fonts(const char *t = 0) {
    return fltk3::_3to1_font( fltk3::set_fonts(t) );
  }
  
  static void set_labeltype(Fl_Labeltype a, Fl_Label_Draw_F *b, Fl_Label_Measure_F *c) {
    fltk3::set_labeltype(fltk3::_1to3_labeltype(a), (fltk3::LabelDrawF*)b, (fltk3::LabelMeasureF*)c);
  }
  
  static void set_labeltype(Fl_Labeltype a, Fl_Labeltype b) {
    fltk3::set_labeltype(fltk3::_1to3_labeltype(a), fltk3::_1to3_labeltype(b));
  }
  
  static Fl_Box_Draw_F *get_boxtype(Fl_Boxtype b) {
    return 0; //(fltk3::BoxDrawF*)fltk3::get_boxtype(fltk3::_1to3_boxtype(b));
  }
  
  static void set_boxtype(Fl_Boxtype a, Fl_Box_Draw_F *b, uchar c, uchar d, uchar e, uchar f) {
    fltk3::set_boxtype(fltk3::_1to3_boxtype(a), (fltk3::BoxDrawF*)b, c, d, e, f);
  }
  
  static void set_boxtype(Fl_Boxtype a, Fl_Boxtype b) {
    fltk3::set_boxtype(fltk3::_1to3_boxtype(a), fltk3::_1to3_boxtype(b));
  }
  
  static int box_dx(Fl_Boxtype b) {
    return fltk3::box_dx(fltk3::_1to3_boxtype(b));
  }
  
  static int box_dy(Fl_Boxtype b) {
    return fltk3::box_dy(fltk3::_1to3_boxtype(b));
  }
  
  static int box_dw(Fl_Boxtype b) {
    return fltk3::box_dw(fltk3::_1to3_boxtype(b));
  }
  
  static int box_dh(Fl_Boxtype b) {
    return fltk3::box_dh(fltk3::_1to3_boxtype(b));
  }
  
  static int draw_box_active() {
    return fltk3::draw_box_active();
  }
  
  static void set_abort(Fl_Abort_Handler f) {
    fltk3::set_abort((fltk3::AbortHandler)f);
  }
  
  static void default_atclose(Fl_Window *w, void *d) {
    fltk3::default_atclose(fltk3::_1to3_window(w), d);
  }
  
  static void set_atclose(Fl_Atclose_Handler f) {
    fltk3::set_atclose((fltk3::AtcloseHandler)f);
  }
  
  static int event_shift() {
    return fltk3::event_shift();
  }
  
  static int event_ctrl() {
    return fltk3::event_ctrl();
  }
  
  static int event_command() {
    return fltk3::event_command();
  }
  
  static int event_alt() {
    return fltk3::event_alt();
  }
  
  static int event_buttons() {
    return fltk3::event_buttons();
  }
  
  static int event_button1() {
    return fltk3::event_button1();
  }
  
  static int event_button2() {
    return fltk3::event_button2();
  }
  
  static int event_button3() {
    return fltk3::event_button3();
  }
  
  static void set_idle(Fl_Old_Idle_Handler cb) {
    fltk3::set_idle((fltk3::OldIdleHandler)cb);
  }
  
  static void grab(Fl_Window& win) {
    fltk3::grab(*(fltk3::_1to3_window(&win)));
  }
  
  static void release() {
    fltk3::release();
  }
  
  static void visible_focus(int v) {
    fltk3::visible_focus(v);
  }
  
  static int visible_focus() {
    return fltk3::visible_focus();
  }
  
  static void dnd_text_ops(int v) {
    fltk3::dnd_text_ops(v);
  }
  
  static int dnd_text_ops() {
    return fltk3::dnd_text_ops();
  }
  
  static int lock() {
    return fltk3::lock();
  }
  
  static void unlock() {
    return fltk3::unlock();
  }
  
  static void awake(void* message = 0) {
    fltk3::awake(message);
  }
  
  static int awake(Fl_Awake_Handler cb, void* message = 0) {
    return fltk3::awake((fltk3::AwakeHandler)cb, message);
  }
  
  static void* thread_message() {
    return fltk3::thread_message();
  }
  
  static void delete_widget(Fl_Widget *w) {
    fltk3::delete_widget(fltk3::_1to3_widget(w));
  }
  
  static void do_widget_deletion() {
    fltk3::do_widget_deletion();
  }
    
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
};


class FL_EXPORT Fl_Widget_Tracker: public fltk3::WidgetTracker {
public:
  Fl_Widget_Tracker(Fl_Widget *wi) :
    fltk3::WidgetTracker(fltk3::_1to3_widget(wi))
  { }
  ~Fl_Widget_Tracker() { }
  Fl_Widget *widget() {
    return fltk3::_3to1_widget(fltk3::WidgetTracker::widget());
  }
  int deleted() {
    return fltk3::WidgetTracker::deleted();
  }
  int exists()  {
    return fltk3::WidgetTracker::exists();
  }
};


#endif // !Fl_H

//
// End of "$Id$".
//
