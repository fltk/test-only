//
// "$Id: classic.cxx,v 1.17 2000/01/23 01:38:21 bill Exp $"
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

static void choice_glyph(int, int x,int y,int w,int h, Fl_Color bc, Fl_Color,
		  Fl_Flags f, Fl_Boxtype box)
{
  int H = 7;
  FL_THIN_UP_BOX->draw(x, y+(h-H)/2, w-4, H, bc, f);
}

static Fl_Frame_Box always_up_box(0,FL_UP_BOX->data);

static const Fl_Highlight_Box menu_title_box(0, &always_up_box);

extern "C" int fltk_theme(int, char**);

int fltk_theme(int, char**) {

  fl_up_box.data = always_up_box.data = "AAAAWUJJUTNN";
  fl_down_box.data = "NNTUJJUWAAAA";
  Fl_Style* s;
  if ((s = Fl_Style::find("menu_item"))) {
    s->box = FL_FLAT_BOX;
    s->selection_color = FL_WHITE;
    s->selection_text_color = 0;
    s->off_color = FL_GRAY;
  }
  if ((s = Fl_Style::find("menu_title"))) {
    s->box = &menu_title_box;
    s->selection_color = 0;
    s->selection_text_color = 0;
    s->off_color = FL_GRAY;
  }
  if ((s = Fl_Style::find("check_button"))) {
    s->selection_color = FL_BLACK;
    s->off_color = FL_GRAY;
  }
  Fl_Widget::default_style->highlight_color = 0;
  Fl_Widget::default_style->label_size = 15;
  Fl_Widget::default_style->text_size = 15;
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
  if ((s = Fl_Style::find("browser"))) {
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
  if ((s = Fl_Style::find("choice"))) {
    s->box = FL_UP_BOX;
    s->color = FL_GRAY;
    s->glyph = choice_glyph;
  }
  return 0;
}

//
// End of "$Id: classic.cxx,v 1.17 2000/01/23 01:38:21 bill Exp $".
//
