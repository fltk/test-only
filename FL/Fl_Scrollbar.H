//
// "$Id: Fl_Scrollbar.H,v 1.17 1999/11/20 04:42:31 vincent Exp $"
//
// Scroll bar header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Scrollbar_H
#define Fl_Scrollbar_H

#include "Fl_Slider.H"

class FL_API Fl_Scrollbar : public Fl_Slider {

public:

  Fl_Scrollbar(int x,int y,int w,int h, const char *l = 0);

  int value() {return int(Fl_Slider::value());}
  int value(int position, int size, int top, int total);

  int linesize() const {return linesize_;}
  void linesize(int i) {linesize_ = i;}

  int pagesize() const {return pagesize_;}
  void pagesize(int i) {pagesize_ = i;}

private:

  static Fl_Style* default_style;
  int linesize_;
  int pagesize_;
  int pushed_, last_, highlight_;
  void draw();
  int handle(int);
  static void timeout_cb(void*);
  void increment_cb();

};

#endif

//
// End of "$Id: Fl_Scrollbar.H,v 1.17 1999/11/20 04:42:31 vincent Exp $".
//
