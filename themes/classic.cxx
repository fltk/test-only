//
// "$Id: classic.cxx,v 1.15 2000/01/10 06:31:32 bill Exp $"
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

// fltk classic appearance

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

extern "C" int fltk_theme(int, char**);

int fltk_theme(int, char**) {

  fl_up_box.data = "AAAAWUJJUTNN";
  fl_down_box.data = "NNTUJJUWAAAA";
  Fl_Style* s;
  if ((s = Fl_Style::find("menu_item"))) {
    s->box = FL_FLAT_BOX;
    s->highlight_color = FL_WHITE;
    s->highlight_label_color = 0;
    s->selection_color = FL_GRAY;
    s->off_color = FL_GRAY;
  }
  if ((s = Fl_Style::find("check_button"))) {
    s->selection_color = FL_BLACK;
    s->off_color = FL_GRAY;
  }
  Fl_Widget::default_style->highlight_color = 0;
  if ((s = Fl_Style::find("scrollbar"))) {
    s->box = FL_FLAT_BOX;
  }
  if ((s = Fl_Style::find("slider"))) {
    //s->box = FL_DOWN_BOX;
    s->glyph_box = FL_UP_BOX;
  }
  if ((s = Fl_Style::find("input"))) {
    s->box = FL_DOWN_BOX;
    s->selection_color = FL_DARK2;
    s->selection_text_color = FL_BLACK;
  }
  if ((s = Fl_Style::find("output"))) {
    s->box = FL_DOWN_BOX;
    s->color = FL_GRAY;
    s->selection_color = FL_DARK2;
    s->selection_text_color = FL_BLACK;
  }
  return 0;
}

//
// End of "$Id: classic.cxx,v 1.15 2000/01/10 06:31:32 bill Exp $".
//
