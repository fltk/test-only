//
// "$Id$"
//
// Printing support for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper started
//
// Copyright 2010-2011 by Bill Spitzak and others.
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

#ifndef Fl_Paged_Device_H
#define Fl_Paged_Device_H

#include <fltk3/PagedDevice.h>
#include <FL/Fl_Device.H>
#include <FL/Fl_Window.H>

#define NO_PAGE_FORMATS 30 /* MSVC6 compilation fix */


class Fl_Paged_Device : public Fl_Surface_Device {
  
public:

  enum Page_Format {
    A0 = 0,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    A8,
    A9,
    B0,
    B1,
    B2,
    B3,
    B4,
    B5,
    B6,
    B7,
    B8,
    B9,
    B10,
    C5E,
    DLE,
    EXECUTIVE,
    FOLIO,
    LEDGER,
    LEGAL,
    LETTER,
    TABLOID,
    ENVELOPE,
    MEDIA = 0x1000
  };

  enum Page_Layout {
    PORTRAIT = 0,
    LANDSCAPE = 0x100,
    REVERSED = 0x200,
    ORIENTATION = 0x300
  };

  typedef struct {
    int width; 
    int height; 
    const char *name; 
  } page_format;

protected:
  
  Fl_Paged_Device() : Fl_Surface_Device(NULL) {};
  
public:
  
  const char *class_name() {
    return ((fltk3::PagedDevice*)_p)->class_name();
  }
  
  virtual int start_job(int pagecount, int *frompage = NULL, int *topage = NULL) {
    return ((fltk3::PagedDevice*)_p)->start_job(pagecount, frompage, topage);
  }
  
  virtual int start_page(void) {
    return ((fltk3::PagedDevice*)_p)->start_page();
  }
  
  virtual int printable_rect(int *w, int *h) {
    return ((fltk3::PagedDevice*)_p)->printable_rect(w, h);
  }
  
  virtual void margins(int *left, int *top, int *right, int *bottom) {
    ((fltk3::PagedDevice*)_p)->margins(left, top, right, bottom);
  }
  
  virtual void origin(int x, int y) {
    ((fltk3::PagedDevice*)_p)->origin(x, y);
  }
  
  virtual void origin(int *x, int *y) {
    ((fltk3::PagedDevice*)_p)->origin(x, y);
  }
  
  virtual void scale(float scale_x, float scale_y = 0.) {
    ((fltk3::PagedDevice*)_p)->scale(scale_x, scale_y);
  }
  
  virtual void rotate(float angle) {
    ((fltk3::PagedDevice*)_p)->rotate(angle);
  }
  
  virtual void translate(int x, int y) {
    ((fltk3::PagedDevice*)_p)->translate(x, y);
  }
  
  virtual void untranslate(void) {
    ((fltk3::PagedDevice*)_p)->untranslate();
  }
  
  virtual void print_widget(Fl_Widget* widget, int delta_x = 0, int delta_y = 0) {
    ((fltk3::PagedDevice*)_p)->print_widget(fltk3::_1to3_widget(widget), delta_x, delta_y);
  }
  
  void print_window(Fl_Window *win, int x_offset = 0, int y_offset = 0) {
    ((fltk3::PagedDevice*)_p)->print_window(fltk3::_1to3_window(win), x_offset, y_offset);
  }
  
  virtual void print_window_part(Fl_Window *win, int x, int y, int w, int h, int delta_x = 0, int delta_y = 0) {
    ((fltk3::PagedDevice*)_p)->print_window_part(fltk3::_1to3_window(win), x, y, w, h, delta_x, delta_y);
  }
  
  virtual int end_page (void) {
    return ((fltk3::PagedDevice*)_p)->end_page();
  }
  
  virtual void end_job (void) {
    ((fltk3::PagedDevice*)_p)->end_job();
  }
};


#endif // Fl_Paged_Device_H

//
// End of "$Id$"
//

