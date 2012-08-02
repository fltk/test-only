//
// "$Id$"
//
// Widget type header file for the Fast Light Tool Kit (FLTK).
//
// Type for creating all subclasses of fltk3::Widget
// This should have the widget pointer in it, but it is still in the
// Fl_Type base class.
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#ifndef FLUID3_WIDGET_TYPE_H
#define FLUID3_WIDGET_TYPE_H

#include "Fl_Type.h"


namespace fltk3 {
  struct MenuItem;
}
class Fluid_Image;


#define NUM_EXTRA_CODE 4


class Fl_Widget_Type : public Fl_Type {
  virtual fltk3::Widget *widget(int,int,int,int) = 0;
  virtual Fl_Widget_Type *_make() = 0; // virtual constructor
  virtual void setlabel(const char *);
  
  const char *extra_code_[NUM_EXTRA_CODE];
  const char *subclass_;
  const char *tooltip_;
  const char *image_name_;
  const char *inactive_name_;
  uchar hotspot_;
  
protected:
  
  void write_static();
  void write_code1();
  void write_widget_code();
  void write_extra_code();
  void write_block_close();
  void write_code2();
  void write_color(const char*, fltk3::Color);
  fltk3::Widget *live_widget;
  
public:
  static int default_size;
  
  const char *xclass; // junk string, used for shortcut
  fltk3::Widget *o;
  int public_;
  
  Fluid_Image *image;
  void setimage(Fluid_Image *);
  Fluid_Image *inactive;
  void setinactive(Fluid_Image *);
  
  Fl_Widget_Type();
  Fl_Type *make();
  void open();
  virtual const char *include_path() { return "fltk3"; }
  
  const char *extra_code(int n) const {return extra_code_[n];}
  void extra_code(int n,const char *);
  const char *subclass() const {return subclass_;}
  void subclass(const char *);
  const char *tooltip() const {return tooltip_;}
  void tooltip(const char *);
  const char *image_name() const {return image_name_;}
  void image_name(const char *);
  const char *inactive_name() const {return inactive_name_;}
  void inactive_name(const char *);
  uchar hotspot() const {return hotspot_;}
  void hotspot(uchar v) {hotspot_ = v;}
  uchar resizable() const;
  void resizable(uchar v);
  
  virtual int textstuff(int what, fltk3::Font &, int &, fltk3::Color &);
  virtual fltk3::MenuItem *subtypes();
  
  virtual int is_widget() const;
  virtual int is_public() const;
  
  virtual void write_properties();
  virtual char read_property(const char *);
  
  virtual fltk3::Widget *enter_live_mode(int top=0);
  virtual void leave_live_mode();
  virtual void copy_properties();
  
  virtual void ideal_size(int &w, int &h);
  virtual void ideal_spacing(int &x, int &y);
  
  ~Fl_Widget_Type();
  void redraw();
};


#endif

//
// End of "$Id$".
//
