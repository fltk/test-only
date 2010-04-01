//
// "$Id$"
//
// Main compatibility header file for the Fast Light Tool Kit (FLTK).
//

#ifndef Fltk1_H
#define Fltk1_H

#include <fltk3/run.h>

#if 0

#ifdef HAVE_CAIRO
# include <FL/Fl_Cairo.H>
#endif

#  include "fl_utf8.h"
#  include "Enumerations.H"
#  ifndef Fl_Object
#    define Fl_Object Fl_Widget	/**< for back compatibility - use Fl_Widget! */
#  endif

#endif

#  ifdef check
#    undef check
#  endif


class Fl_Widget;
class Fl_Window;
class Fl_Image;
struct Fl_Label;

#if 0

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

#endif

class FL_EXPORT Fl {
  Fl() {};

public: // should be private!
  
#if 0  

  static void damage(int d) {damage_ = d;}
  static void (*idle)();
  static int add_awake_handler_(Fl_Awake_Handler, void*);
  static int get_awake_handler_(Fl_Awake_Handler&, void*&);
  static double version();
  static int arg(int, char**, int&);
  static int args(int, char**, int&, Fl_Args_Handler ah = 0);
#endif
  
  static void args(int a, char **b) { fltk3::args(a, b); }
  
#if 0
  static void display(const char*);
  static int visual(int);
  static int gl_visual(int, int *alist=0); // platform dependent
  static void own_colormap();
#endif
  
  static void get_system_colors() { fltk3::get_system_colors(); }
  
#if 0
  static void foreground(uchar, uchar, uchar);
  static void background(uchar, uchar, uchar);
  static void background2(uchar, uchar, uchar);
  static int scheme(const char*);
  static const char* scheme() {return scheme_;}
  static int reload_scheme(); // platform dependent
  static int scrollbar_size();
  static void scrollbar_size(int W);
  static int wait();
  static double wait(double time);
  static int check();
  static int ready();
#endif  
  
  static int run() { return fltk3::run(); }
  
#if 0
  static Fl_Widget* readqueue();
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
  static void (*fatal)(const char*, ...);
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
  static void compose_reset() {compose_state = 0;}
  static int event_inside(int,int,int,int);
  static int event_inside(const Fl_Widget*);
  static int test_shortcut(Fl_Shortcut);
  static int handle(int, Fl_Window*);
  static Fl_Widget* belowmouse() {return belowmouse_;}
  static void belowmouse(Fl_Widget*);
  static Fl_Widget* pushed()	{return pushed_;}
  static void pushed(Fl_Widget*);
  static Fl_Widget* focus()	{return focus_;}
  static void focus(Fl_Widget*);
  static void add_handler(Fl_Event_Handler h);
  static void remove_handler(Fl_Event_Handler h);
  static void copy(const char* stuff, int len, int clipboard = 0); // platform dependent
  static void paste(Fl_Widget &receiver, int clipboard /*=0*/); // platform dependent
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
  static void screen_xywh(int &X, int &Y, int &W, int &H) {
    screen_xywh(X, Y, W, H, e_x_root, e_y_root);
  }
  static void screen_xywh(int &X, int &Y, int &W, int &H, int mx, int my);
  static void screen_xywh(int &X, int &Y, int &W, int &H, int n);
  static void	set_color(Fl_Color, uchar, uchar, uchar);
  static void	set_color(Fl_Color, unsigned); // platform dependent
  static Fl_Color get_color(Fl_Color);
  static void	get_color(Fl_Color, uchar&, uchar&, uchar&);
  static void	free_color(Fl_Color, int overlay = 0); // platform dependent
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
  static void grab(Fl_Window&win) {grab(&win);}
  static void release() {grab(0);}
  static void visible_focus(int v) { visible_focus_ = v; }
  static int  visible_focus() { return visible_focus_; }
  static void dnd_text_ops(int v) { dnd_text_ops_ = v; }
  static int  dnd_text_ops() { return dnd_text_ops_; }
  static void lock();
  static void unlock();
  static void awake(void* message = 0);
  static int awake(Fl_Awake_Handler cb, void* message = 0);
  static void* thread_message(); // platform dependent
  static void delete_widget(Fl_Widget *w);
  static void do_widget_deletion();
  static void watch_widget_pointer(Fl_Widget *&w);
  static void release_widget_pointer(Fl_Widget *&w);
  static void clear_widget_pointer(Fl_Widget const *w);

#ifdef HAVE_CAIRO
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
  /** @} */ 

#endif // HAVE_CAIRO
#endif
};

#if 0
/**
  This class should be used to control safe widget deletion.

  You can use an Fl_Widget_Tracker object to watch another widget, if you
  need to know, if this widget has been deleted during a callback.

  This simplifies the use of the "safe widget deletion" methods
  Fl::watch_widget_pointer() and Fl::release_widget_pointer() and
  makes their use more reliable, because the destructor autmatically
  releases the widget pointer from the widget watch list.

  It is intended to be used as an automatic (local/stack) variable,
  such that the automatic destructor is called when the object's
  scope is left. This ensures that no stale widget pointers are
  left in the widget watch list (see example below).
  
  You can also create Fl_Widget_Tracker objects with \c new, but then it
  is your responsibility to delete the object (and thus remove the
  widget pointer from the watch list) when it is not needed any more.

  Example:

  \code
    int MyClass::handle (int event) {

      if (...) {
	Fl_Widget_Tracker wp(this);	// watch myself
	do_callback();			// call the callback

	if (wp.deleted()) return 1;	// exit, if deleted

	// Now we are sure that the widget has not been deleted.
	// It is safe to access the widget

	clear_changed();		// access the widget
      }
    }
  \endcode

*/
class FL_EXPORT Fl_Widget_Tracker {

  Fl_Widget* wp_;

public:

  Fl_Widget_Tracker(Fl_Widget *wi);
  ~Fl_Widget_Tracker();

  /**
    Returns a pointer to the watched widget.

    This pointer is \c NULL, if the widget has been deleted.
  */
  Fl_Widget *widget() {return wp_;}

  /**
    Returns 1, if the watched widget has been deleted.

    This is a convenience method. You can also use something like

    <tt>  if (wp.widget() == 0) // ...</tt>

    where \p wp is an Fl_Widget_Tracker object.
  */
  int deleted() {return wp_ == 0;}

  /**
    Returns 1, if the watched widget exists (has not been deleted).

    This is a convenience method. You can also use something like

    <tt>  if (wp.widget() != 0) // ...</tt>

    where \p wp is an Fl_Widget_Tracker object.
  */
  int exists()  {return wp_ != 0;}

};

 /** \defgroup  fl_unicode  Unicode and UTF-8 functions
	fl global Unicode and UTF-8 handling functions
     @{ */
 /** @} */

#endif

#endif // !Fl_H

//
// End of "$Id$".
//
