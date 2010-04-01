// "$Id: Window.h 6150 2008-08-04 22:53:30Z spitzak $"
//

#ifndef fltk2_Window_h
#define fltk2_Window_h

#include "Group.h"
#include <fltk3/Window.h>

namespace fltk {

#if 0
// value for x,y to indicate window system places window
const int USEDEFAULT = ((int)0x80000000); // same as Win32 value

class CreatedWindow;
class Monitor;
#endif

class FL_API Window : public Group {
public:
  Window(int,int,int,int, const char* = 0, bool begin = false);
  
  Window(int w, int h, const char *title=0) {
    _p = new fltk3::Window(w, h, title);
    _p->wrapper(this);
  }
  
  virtual ~Window() { }

#if 0
  static NamedStyle* default_style;
  const char* label() const	{return Widget::label();}
  const char* iconlabel() const	{return iconlabel_;}
  void label(const char*);
  void iconlabel(const char*);
  void label(const char* label, const char* iconlabel);
  void copy_label(const char* c) {Widget::copy_label(c); label(label());}
  const void* icon() const	{return icon_;}
  void icon(const void * ic)	{icon_ = ic;}
  static const char* xclass()	{return xclass_;}
  static void xclass(const char* v) {xclass_ = v;}

  void border(bool set)	{set ? clear_flag(NOBORDER) : set_flag(NOBORDER);}
  /*! \deprecated compat. api only, please use Window::border(bool)*/
  void clear_border()    {set_flag(NOBORDER);} 
  bool border() const	{return !flag(NOBORDER);}
  void set_override()	{set_flag(NOBORDER|OVERRIDE);}
  bool override() const {return flag(OVERRIDE); }
  const Window* child_of() const {return child_of_;}
  void child_of(const Window* w);
  void set_modal()	{set_flag(MODAL);} // back compatability only!
  void set_non_modal()	{set_flag(NON_MODAL);} // back compatability only!
#endif
  
  bool double_buffer() const { return false; } // fltk123: sorry!
  void set_double_buffer() {} // fltk123: sorry!
  void clear_double_buffer() {} // fltk123: sorry!
  void free_backbuffer() {}  // fltk123: sorry!
  
#if 0
  virtual void draw_overlay();
  void redraw_overlay();
  void erase_overlay();

  void hotspot(int x, int y, bool offscreen = false);
  void hotspot(const Widget*, bool offscreen = false);
  void hotspot(const Widget& p, bool offscrn = false) {hotspot(&p,offscrn);}
  void size_range(int a, int b, int c=0, int d=0, int e=0, int f=0)
    { minw=(short)a; minh=(short)b; maxw=(short)c; maxh=(short)d; dw=(uchar)e; dh=(uchar)f; size_range_(); }
  bool get_size_range( int *min_w, int *min_h, int *max_w, int *max_h );

  bool shown() const {return i != 0;}
  void show();
#endif
  
  void show(int argc, char **argv) { ((fltk3::Window*)_p)->show(argc, argv); }
  
#if 0
  void show(const Window* parent);
  bool exec(const Window* parent = 0, bool grab = false);
  void make_exec_return(bool);
  void show_inside(const Window* parent);
  virtual void destroy();

  void iconize();
  bool iconic() const;

  void maximize();

  void fullscreen();
  void fullscreen(const Monitor&);
  void fullscreen_off(int,int,int,int);

  static void default_callback(Window*, void* v);

  virtual int handle(int);
  virtual void layout();
  void system_layout();
  virtual void flush();
  virtual void draw();

  static Window* first();
  static void first(Window*);
  Window* next();

  void borders( Rectangle *r ) const;

  static const Window* drawing_window() {return drawing_window_;}
  static const Window* drawing_window_;

  // fabien: used for my cairo experimentations,
  // not sure i'll keep that opaque backbuffer access :
  // at least it shouldn't stay public
  void* backbuffer() const;

protected:
  virtual void create();

private:

  friend class CreatedWindow;
  CreatedWindow *i; // points at the system-specific stuff
  const Window* child_of_;
  const char* iconlabel_;
  const void* icon_;
  // size_range stuff:
  short minw, minh, maxw, maxh;
  unsigned char dw, dh, size_range_set;
  void size_range_();
  // values for flags():
  enum {
    MODAL	    = 0x80000000,
    NOBORDER 	    = 0x40000000,
    OVERRIDE	    = 0x20000000,
    NON_MODAL	    = 0x10000000,
    DOUBLE	    = 0x08000000
  };
  static const char* xclass_;
  void _Window(); // constructor innards
#endif
};

}

#endif

//
// End of "$Id: Window.h 6150 2008-08-04 22:53:30Z spitzak $".
//
