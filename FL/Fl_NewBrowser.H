//
// "$Id: Fl_NewBrowser.H,v 1.2 2000/05/11 22:03:25 bill Exp $"
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
  Fl_End endgroup;

public:

  Fl_Widget* goto_top();
  Fl_Widget* goto_mark(int n);
  Fl_Widget* goto_position(int y);
  Fl_Widget* goto_number(int y);

  Fl_Widget* forward();
  Fl_Widget* backward();

  int at_mark(int n);
  void set_mark(int n);

  int item_level() const {return item_level_;}
  int item_position() const {return item_position_;}
  int item_height() const {return item_height_;}
  int item_number() const {return item_number_;}

  void set_focus();

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

  enum { // predefined marks
    FOCUS = 0,
    FIRST_VISIBLE,
    REDRAW_0,
    REDRAW_1,
    USER_0,
    USER_1,
    NUMMARKS
  };

  int xposition() const {return xposition_;}
  void xposition(int);
  int yposition() const {return yposition_;}
  void yposition(int);

private:

  int xposition_, yposition_;
  int scrolldx, scrolldy;
  static void hscrollbar_cb(Fl_Widget*, void*);
  static void scrollbar_cb(Fl_Widget*, void*);
  void draw_clip(int,int,int,int);
  static void draw_clip_cb(void*,int,int,int,int);

  // Mark and current item storage:
  int item_level_;
  int item_position_;
  int item_height_;
  int item_number_;
  struct Data;
  Data* data;
  int mark_position[NUMMARKS];
  int mark_number[NUMMARKS];
  int levels;
  void set_level(int);

};

#define FL_NORMAL_BROWSER Fl_Browser::BOTH
#define FL_MULTI_BROWSER (Fl_Browser::BOTH|Fl_Browser::MULTI_BROWSER)

#ifndef FLTK_2
#define FL_SELECT_BROWSER Fl_Browser::BOTH
#define FL_HOLD_BROWSER Fl_Browser::BOTH
#endif

#endif

//
// End of "$Id: Fl_NewBrowser.H,v 1.2 2000/05/11 22:03:25 bill Exp $".
//
