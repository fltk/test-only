//
// "$Id: Fl_Window.H,v 1.33 2000/02/14 11:32:36 bill Exp $"
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

#include "Fl_Group.H"

class FL_API Fl_Window : public Fl_Group {
public:

  Fl_Window(int,int,int,int, const char* = 0);
  Fl_Window(int,int, const char* = 0);
  virtual ~Fl_Window();

  virtual int handle(int);

  void clear_border()	{set_flag(FL_NOBORDER);}
  int border() const	{return !(flags() & FL_NOBORDER);}
  void set_modal()	{set_flag(FL_MODAL);}
  int modal() const	{return flags() & FL_MODAL;}
  void set_non_modal()	{set_flag(FL_NON_MODAL);}
  int non_modal() const	{return flags() & (FL_NON_MODAL|FL_MODAL);}

  void move(int x, int y);
  void hotspot(int x, int y, int offscreen = 0);
  void hotspot(const Fl_Widget*, int offscreen = 0);
  void hotspot(const Fl_Widget& p, int offscreen = 0) {hotspot(&p,offscreen);}
  void free_position()	{clear_flag(FL_FORCE_POSITION);}
  void size_range(int a, int b, int c=0, int d=0, int e=0, int f=0)
    { minw=a; minh=b; maxw=c; maxh=d; dw=e; dh=f; size_range_(); }

  const char* label() const	{return Fl_Widget::label();}
  const char* iconlabel() const	{return iconlabel_;}
  void label(const char*);
  void iconlabel(const char*);
  void label(const char* label, const char* iconlabel);
  const char* xclass() const	{return xclass_;}
  void xclass(const char* c)	{xclass_ = c;}
  const void* icon() const	{return icon_;}
  void icon(const void * ic)	{icon_ = ic;}

  int shown() const {return i != 0;}
  void show();
  void show(int, char**);
  void show(const Fl_Window* parent);
  int exec(const Fl_Window* parent = 0);
  void show_inside(const Fl_Window* parent);
  void hide();
  void fullscreen();
  void fullscreen_off(int,int,int,int);
  void iconize();

  int x_root() const ;
  int y_root() const ;

  static Fl_Window *current() {return current_;}
  void make_current();

  void cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE);
  static void default_callback(Fl_Window*, void* v);

  virtual void layout();
  virtual void flush();
  virtual void draw();
  virtual void draw_n_clip();

protected:

  virtual void create();
  virtual void destroy();

private:

  friend class Fl_X; Fl_X *i; // points at the system-specific stuff
  const char* iconlabel_;
  const char* xclass_;
  const void* icon_;
  // size_range stuff:
  short minw, minh, maxw, maxh;
  unsigned char dw, dh, size_range_set;
  void size_range_();
  // values for flags():
  enum {
    FL_MODAL		= 0x80000000,
    FL_NOBORDER 	= 0x40000000,
    FL_FORCE_POSITION	= 0x20000000,
    FL_NON_MODAL 	= 0x10000000
  };
  static Fl_Window *current_;
  void _Fl_Window(); // constructor innards
};

#endif

//
// End of "$Id: Fl_Window.H,v 1.33 2000/02/14 11:32:36 bill Exp $".
//
