//
// "$Id: Fl_NewBrowser.H,v 1.1 2000/04/24 08:31:21 bill Exp $"
//
// Copyright 1998-2000 by Bill Spitzak and others.
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

// This file is temporary, it will completely replace the current
// Fl_Browser.H and Fl_Browser_.H

#ifndef Fl_Browser_H
#define Fl_Browser_H

#define Fl_Browser Fl_NewBrowser

#include "Fl_Menu_.H"
#include "Fl_Scrollbar.H"

class FL_API Fl_Browser : public Fl_Menu_ {

  int xposition_, yposition_;
  int scrolldx, scrolldy;
  static void hscrollbar_cb(Fl_Widget*, void*);
  static void scrollbar_cb(Fl_Widget*, void*);
  void draw_clip(int,int,int,int);
  static void draw_clip_cb(void*,int,int,int,int);
  Fl_End endgroup;

  struct Data;
  Data* data;
  int levels;

  int level;
  int item_index;
  int first_visible_position;
  int first_visible_level;
  int first_visible_index;
  void set_level(int);
  int measure(Fl_Widget*);
  Fl_Widget* goto_top();
  Fl_Widget* goto_first_visible();
  void set_first_visible();
  Fl_Widget* forward();
  Fl_Widget* backward();
  int goto_position(int y);
  Fl_Widget* goto_focus();
  void set_focus();

public:

  void bbox(int&,int&,int&,int&);
  Fl_Scrollbar scrollbar;
  Fl_Scrollbar hscrollbar;

  int handle(int);
  void layout();
  void draw();

  Fl_Browser(int X,int Y,int W,int H,const char*l=0);
  ~Fl_Browser();

  enum { // values for type()
    HORIZONTAL = 1,
    VERTICAL = 2,
    BOTH = 3,
    ALWAYS_ON = 4,
    HORIZONTAL_ALWAYS = 5,
    VERTICAL_ALWAYS = 6,
    BOTH_ALWAYS = 7,
    MULTI_BROWSER = 8
  };

  int xposition() const {return xposition_;}
  void xposition(int);
  int yposition() const {return yposition_;}
  void yposition(int);
};

#define FL_NORMAL_BROWSER Fl_Browser::BOTH
#define FL_MULTI_BROWSER (Fl_Browser::BOTH|Fl_Browser::MULTI_BROWSER)

#ifndef FLTK_2
#define FL_SELECT_BROWSER Fl_Browser::BOTH
#define FL_HOLD_BROWSER Fl_Browser::BOTH
#endif

#endif

//
// End of "$Id: Fl_NewBrowser.H,v 1.1 2000/04/24 08:31:21 bill Exp $".
//
