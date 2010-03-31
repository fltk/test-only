//
// "$Id$"
//
// Window compatibility header file for the Fast Light Tool Kit (FLTK).
//

#ifndef Fltk1_Window_H
#define Fltk1_Window_H

#include <fltk3/Window.h>
#include "Fl_Group.H"

#if 0

#define FL_WINDOW 0xF0		///< window type id all subclasses have type() >= this
#define FL_DOUBLE_WINDOW 0xF1   ///< double window type id

class Fl_X;

/**
  This widget produces an actual window.  This can either be a main
  window, with a border and title and all the window management controls,
  or a "subwindow" inside a window.  This is controlled by whether or not
  the window has a parent().

  Once you create a window, you usually add children Fl_Widget
  's to it by using window->add(child) for each new widget.
  See Fl_Group for more information on how to add and remove children.

  There are several subclasses of Fl_Window that provide
  double-buffering, overlay, menu, and OpenGL support.

  The window's callback is done if the user tries to close a window
  using the window manager and Fl::modal() is zero or equal to the
  window. Fl_Window has a default callback that calls Fl_Window::hide().
*/
#endif

class FL_EXPORT Fl_Window : public Fl_Group {
  
#if 0
  // unimplemented copy ctor and assignment operator
  Fl_Window(const Fl_Window&);
  Fl_Window& operator=(const Fl_Window&);

protected:

  virtual void draw();
  virtual void flush();
  void force_position(int force) {
    if (force) set_flag(FORCE_POSITION);
    else clear_flag(FORCE_POSITION);
  }
  int force_position() const { return ((flags() & FORCE_POSITION)?1:0); }
  
#endif
  
public:

  Fl_Window(int w, int h, const char* title= 0) {
    _p = new fltk3::Window(w, h, title); 
    _p->wrapper(this);
    begin();
  }
  
  Fl_Window(int x, int y, int w, int h, const char* title = 0) {
    _p = new fltk3::Window(x, y, w, h, title); 
    _p->wrapper(this);
    begin();
  }
  
  virtual ~Fl_Window() { }

#if 0
  virtual int handle(int);
  virtual void resize(int,int,int,int);
  void border(int b);
  void clear_border()	{set_flag(NOBORDER);}
  unsigned int border() const	{return !(flags() & NOBORDER);}
  void set_override()	{set_flag(NOBORDER|OVERRIDE);}
  unsigned int override() const  { return flags()&OVERRIDE; }
  void set_modal()	{set_flag(MODAL);}
  unsigned int modal() const	{return flags() & MODAL;}
  void set_non_modal()	{set_flag(NON_MODAL);}
  unsigned int non_modal() const {return flags() & (NON_MODAL|MODAL);}
  void set_menu_window()	{set_flag(MENU_WINDOW);}
  unsigned int menu_window() const {return flags() & MENU_WINDOW;}
  void set_tooltip_window()	{ set_flag(TOOLTIP_WINDOW);
				  clear_flag(MENU_WINDOW); }
  unsigned int tooltip_window() const {return flags() & TOOLTIP_WINDOW;}
  void hotspot(int x, int y, int offscreen = 0);
  void hotspot(const Fl_Widget*, int offscreen = 0);
  void hotspot(const Fl_Widget& p, int offscreen = 0) {hotspot(&p,offscreen);}
  void free_position()	{clear_flag(FORCE_POSITION);}
  void size_range(int a, int b, int c=0, int d=0, int e=0, int f=0, int g=0) {
    minw=a; minh=b; maxw=c; maxh=d; dw=e; dh=f; aspect=g; size_range_();}
  const char* label() const	{return Fl_Widget::label();}
  const char* iconlabel() const	{return iconlabel_;}
  void label(const char*);
  void iconlabel(const char*);
  void label(const char* label, const char* iconlabel); // platform dependent 
  void copy_label(const char* a);
  const char* xclass() const	{return xclass_;}
  void xclass(const char* c)	{xclass_ = c;}
  const void* icon() const	{return icon_;}
  void icon(const void * ic)	{icon_ = ic;}
  int shown() {return i != 0;}
  virtual void show();
  virtual void hide();
#endif
  
  void show(int argc, char **argv) { ((fltk3::Window*)_p)->show(argc, argv); }
  
#if 0
  void fullscreen();
  void fullscreen_off(int,int,int,int);
  void iconize();
  int x_root() const ;
  int y_root() const ;
 static Fl_Window *current();
  void make_current();
  virtual Fl_Window* as_window() { return this; }
  void cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE); // platform dependent
  void default_cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE);
  static void default_callback(Fl_Window*, void* v);
#endif
};


#endif

//
// End of "$Id$".
//
