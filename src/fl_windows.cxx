//
// "$Id: fl_windows.cxx,v 1.1 1999/11/08 22:21:57 carl Exp $"
//
// Theme plugin file for FLTK
//
// Copyright 1999 Bill Spitzak and others.
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

// Windows-98 style

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Style.H>
#include <stdio.h>
#include <string.h>

// a boxtype drawing function in fl_boxtype.cxx
extern void fl_flatx(Fl_Boxtype b, int x, int y, int w, int h,
                     Fl_Color c, Fl_Flags f);

const Fl_Boxtype_ win98_menu_title_box = {
  fl_flatx, 0, FL_THIN_DOWN_BOX, FL_THIN_UP_BOX, 1,1,2,2, true
};


int fl_windows() {
  Fl_Style::revert(); // revert to FLTK default styles

  Fl_Widget::default_style.set_selection_color(FL_GRAY);
  Fl_Widget::default_style.set_label_size(12);

  Fl_Style* s;
  if ((s = Fl_Style::find("menu title"))) {
    s->set_box(&win98_menu_title_box);
    s->set_selection_color(FL_GRAY);
    s->set_selection_text_color(FL_BLACK);
    s->set_highlight_color(FL_GRAY);
  }

  // highlighting must be on both the menu bar and menu title!
  if ((s = Fl_Style::find("menu bar"))) {
    s->set_highlight_color(FL_GRAY);
  }

  if ((s = Fl_Style::find("input"))) {
    s->set_box(FL_DOWN_BOX);
  }

  if ((s = Fl_Style::find("slider"))) {
    s->set_box(FL_DOWN_BOX);
  }

  if ((s = Fl_Style::find("value slider"))) {
    s->set_box(FL_DOWN_BOX);
  }

  if ((s = Fl_Style::find("output"))) {
    s->set_box(FL_DOWN_BOX);
    s->set_color(FL_WHITE);
  }

  if ((s = Fl_Style::find("counter"))) {
    s->set_box(FL_DOWN_BOX);
    s->set_color(FL_WHITE);
  }

  if ((s = Fl_Style::find("scroll bar"))) {
    s->set_color(52);
  }

  if ((s = Fl_Style::find("highlight button"))) {
    s->set_highlight_color(FL_GRAY);
  }

  static Fl_Boxtype_Definer win98_menu_title("win98 menu title", win98_menu_title_box);

  Fl::redraw();

  return 0;
}

//
// End of "$Id: fl_windows.cxx,v 1.1 1999/11/08 22:21:57 carl Exp $".
//
