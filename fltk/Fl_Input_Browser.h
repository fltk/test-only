//
// "$Id: Fl_Input_Browser.h,v 1.1 2001/07/23 09:50:04 spitzak Exp $"
//
// Combo box header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Input_Browser_H
#define Fl_Input_Browser_H

#include <fltk/Fl_Menu_.h>
#include <fltk/Fl_Input.h>

// values for type()
#define FL_NORMAL_INPUT_BROWSER      0
#define FL_NONEDITABLE_INPUT_BROWSER 1
#define FL_INDENTED_INPUT_BROWSER    2

class FL_API Fl_Input_Browser : public Fl_Menu_ {
public:
  Fl_Input_Browser(int,int,int,int,const char * = 0);
  ~Fl_Input_Browser() { input->parent(0); delete input; }
  static Fl_Named_Style* default_style;

  virtual int handle(int);

  void maxw(int i) { maxw_ = i; }
  void maxh(int i) { maxh_ = i; }
  int maxw() { return maxw_; }
  int maxh() { return maxh_; }

  void value(const char *v) { input->value(v); }
  const char *value() { return input->value(); }

protected:
  virtual void draw();

  Fl_Input *input;
  int minw_, minh_, maxw_, maxh_;
  int over_now, over_last;
};

#endif

//
// End of "$Id: Fl_Input_Browser.h,v 1.1 2001/07/23 09:50:04 spitzak Exp $".
//
