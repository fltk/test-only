//
// "$Id$"
//
// Window header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Window_H
#define Fl_Window_H

#include <fltk3/Wrapper.h>
#include <fltk3/Window.h>

#include "Fl_Group.H"

#define FL_WINDOW 0xF0		///< window type id all subclasses have type() >= this
#define FL_DOUBLE_WINDOW 0xF1   ///< double window type id

class Fl_X;

class FL_EXPORT Fl_Window : public Fl_Group {

  Fl_Window(const Fl_Window&);
  Fl_Window& operator=(const Fl_Window&);

#if 0 // FIXME: 123

protected:
  
  virtual void draw();
  virtual void flush();
  void force_position(int force);
  int force_position() const { return ((flags() & FORCE_POSITION)?1:0); }
#endif
  
public:
  
  Fl_Window() {}
  
  Fl_Window(int w, int h, const char* label = 0) {
    _p = new fltk3::Window(w, h, label);
    _p->wrapper(this);
  }
  
  Fl_Window(int x, int y, int w, int h, const char* label = 0) {
    _p = new fltk3::Window(x, y, w, h, label);
    _p->wrapper(this);
  }
  
#if 0 // FIXME: 123
  virtual ~Fl_Window();
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
  void set_tooltip_window();
  unsigned int tooltip_window() const {return flags() & TOOLTIP_WINDOW;}
  void hotspot(int x, int y, int offscreen = 0);
  void hotspot(const Fl_Widget*, int offscreen = 0);
  void hotspot(const Fl_Widget& p, int offscreen = 0) {hotspot(&p,offscreen);}
  void free_position()	{clear_flag(FORCE_POSITION);}
  void size_range(int a, int b, int c=0, int d=0, int e=0, int f=0, int g=0);
  const char* label() const	{return Fl_Widget::label();}
  const char* iconlabel() const	{return iconlabel_;}
  void label(const char*);
  void iconlabel(const char*);
  void label(const char* label, const char* iconlabel); // platform dependent
  void copy_label(const char* a);
  static void default_xclass(const char*);
  static const char *default_xclass();
  const char* xclass() const;
  void xclass(const char* c);
  const void* icon() const;
  void icon(const void * ic);
  int shown() {return i != 0;}
  virtual void show();
  virtual void hide();
#endif
  
  void show(int argc, char **argv) {
    ((fltk3::Window*)_p)->show(argc, argv);
  }
  
#if 0 // FIXME: 123
  void fullscreen();
  void fullscreen_off(int,int,int,int);
  void iconize();
  int x_root() const ;
  int y_root() const ;
  static Fl_Window *current();
  void make_current();
  virtual Fl_Window* as_window() { return this; }
#endif
  
  void cursor(Fl_Cursor c, Fl_Color a=FL_BLACK, Fl_Color b=FL_WHITE) {
    ((fltk3::Window*)_p)->cursor( fltk3::_1to3_cursor(c), fltk3::_1to3_color(a), fltk3::_1to3_color(b));
  }
  
#if 0 // FIXME: 123
  void default_cursor(Fl_Cursor, Fl_Color=FL_BLACK, Fl_Color=FL_WHITE);
  static void default_callback(Fl_Window*, void* v);
  int decorated_w();
  int decorated_h();
#endif

};

#endif

//
// End of "$Id$".
//
