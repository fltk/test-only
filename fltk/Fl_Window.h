//
// "$Id: Fl_Window.h,v 1.5 2002/07/15 05:55:37 spitzak Exp $"
//
// Window header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Window_H
#define Fl_Window_H

#include "Fl_Group.h"

// value for x,y to indicate window system places window
#define  FL_USEDEFAULT ((int)0x80000000) // same as Win32 value

class FL_API Fl_Window : public Fl_Group {
public:

  Fl_Window(int,int,int,int, const char* = 0);
  Fl_Window(int,int, const char* = 0);
  static Fl_Named_Style* default_style;
  virtual ~Fl_Window();

  const char* label() const	{return Fl_Widget::label();}
  const char* iconlabel() const	{return iconlabel_;}
  void label(const char*);
  void iconlabel(const char*);
  void label(const char* label, const char* iconlabel);
  void copy_label(const char* c) {Fl_Widget::copy_label(c); label(label());}
  const void* icon() const	{return icon_;}
  void icon(const void * ic)	{icon_ = ic;}
  static const char* xclass()	{return xclass_;}
  static void xclass(const char* v) {xclass_ = v;}

  void clear_border()	{set_flag(FL_NOBORDER);}
  bool border() const	{return !(flags() & FL_NOBORDER);}
  void set_override()	{set_flag(FL_NOBORDER|FL_OVERRIDE);}
  bool override() const {return (flags()&FL_OVERRIDE)!=0; }
  const Fl_Window* child_of() const {return child_of_;}
  void child_of(const Fl_Window* w);
  void set_modal()	{set_flag(FL_MODAL);} // back compatability only!
  void set_non_modal()	{set_flag(FL_NON_MODAL);} // back compatability only!

  void hotspot(int x, int y, bool offscreen = false);
  void hotspot(const Fl_Widget*, bool offscreen = false);
  void hotspot(const Fl_Widget& p, bool offscrn = false) {hotspot(&p,offscrn);}
  void size_range(int a, int b, int c=0, int d=0, int e=0, int f=0)
    { minw=a; minh=b; maxw=c; maxh=d; dw=e; dh=f; size_range_(); }

  bool shown() const {return i != 0;}
  void show();
  void show(int, char**);
  void show(const Fl_Window* parent);
  bool exec(const Fl_Window* parent = 0, bool grab = false);
  void show_inside(const Fl_Window* parent);
  void iconize();
  bool iconic() const;
  virtual void destroy();

  void fullscreen();
  void fullscreen_off(int,int,int,int);

  static const Fl_Window *current() {return current_;}
  void make_current() const;

  void cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE);
  static void default_callback(Fl_Window*, void* v);

  virtual int handle(int);
  virtual void layout();
  virtual void flush();
  virtual void draw();

protected:
  virtual void create();
  static const Fl_Window *current_;

private:

  friend class Fl_X; Fl_X *i; // points at the system-specific stuff
  const Fl_Window* child_of_;
  const char* iconlabel_;
  const void* icon_;
  // size_range stuff:
  short minw, minh, maxw, maxh;
  unsigned char dw, dh, size_range_set;
  void size_range_();
  // values for flags():
  enum {
    FL_MODAL		= 0x80000000,
    FL_NOBORDER 	= 0x40000000,
    FL_OVERRIDE         = 0x20000000,
    FL_NON_MODAL	= 0x10000000
  };
  static const char* xclass_;
  void _Fl_Window(); // constructor innards
};

#endif

//
// End of "$Id: Fl_Window.h,v 1.5 2002/07/15 05:55:37 spitzak Exp $".
//
