//
// "$Id: Window.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $"
//
// Window widget. This must be the outermost group. You can also put
// them inside other widgets to use the system's window hierarchy.
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

#ifndef fltk_Window_h
#define fltk_Window_h

#include "Group.h"

namespace fltk {

// value for x,y to indicate window system places window
const int USEDEFAULT = ((int)0x80000000); // same as Win32 value

class CreatedWindow;

class FL_API Window : public Group {
public:

  Window(int,int,int,int, const char* = 0, bool begin = false);
  Window(int,int, const char* = 0);
  static NamedStyle* default_style;
  virtual ~Window();

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

  void clear_border()	{set_flag(NOBORDER);}
  bool border() const	{return !(flags() & NOBORDER);}
  void set_override()	{set_flag(NOBORDER|OVERRIDE);}
  bool override() const {return (flags()&OVERRIDE)!=0; }
  const Window* child_of() const {return child_of_;}
  void child_of(const Window* w);
  void set_modal()	{set_flag(MODAL);} // back compatability only!
  void set_non_modal()	{set_flag(NON_MODAL);} // back compatability only!

  void hotspot(int x, int y, bool offscreen = false);
  void hotspot(const Widget*, bool offscreen = false);
  void hotspot(const Widget& p, bool offscrn = false) {hotspot(&p,offscrn);}
  void size_range(int a, int b, int c=0, int d=0, int e=0, int f=0)
    { minw=a; minh=b; maxw=c; maxh=d; dw=e; dh=f; size_range_(); }

  bool shown() const {return i != 0;}
  void show();
  void show(int, char**);
  void show(const Window* parent);
  bool exec(const Window* parent = 0, bool grab = false);
  void show_inside(const Window* parent);
  void iconize();
  bool iconic() const;
  virtual void destroy();

  void fullscreen();
  void fullscreen_off(int,int,int,int);

  static const Window *current() {return current_;}
  void make_current() const;

  static void default_callback(Window*, void* v);

  virtual int handle(int);
  virtual void layout();
  virtual void flush();
  virtual void draw();

  static Window* first();
  static void first(Window*);
  Window* next();

protected:
  virtual void create();
  static const Window *current_;

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
    MODAL	= 0x80000000,
    NOBORDER 	= 0x40000000,
    OVERRIDE	= 0x20000000,
    NON_MODAL	= 0x10000000
  };
  static const char* xclass_;
  void _Window(); // constructor innards
};

}

#endif

//
// End of "$Id: Window.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $".
//
