//
// "$Id: fl_windows.cxx,v 1.14 1999/11/19 15:11:57 carl Exp $"
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

static int engraved_data[2][3] = {{1,1,FL_LIGHT3},{0,0,0}};
static int embossed_data[2][3] = {{-1,-1,FL_LIGHT3},{0,0,0}};

static const Fl_Engraved_Label win98_engraved_label(
  "windows engraved", engraved_data);

static const Fl_Engraved_Label win98_embossed_label(
  "windows embossed", embossed_data);

class Win98_Label : public Fl_Engraved_Label {
  void draw(const char*, int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
public:
  Win98_Label(const char* n, const int p[][3]) : Fl_Engraved_Label(n,p) {}
};

void Win98_Label::draw(const char* label,
		       int X, int Y, int W, int H,
		       Fl_Color fill, Fl_Flags f) const
{
  if (f&FL_INACTIVE) Fl_Engraved_Label::draw(label, X, Y, W, H, fill, f);
  else Fl_Labeltype_::draw(label, X, Y, W, H, fill, f);
}

static const Win98_Label win98_label("windows", engraved_data);

static const Fl_Highlight_Box win98_menu_title_box(
  "win98 menu title", FL_THIN_UP_BOX);

static const Fl_Frame_Box win98_menu_window_box(
  "win98 menu window", "2AARRMMUU", FL_DOWN_BOX);

int fl_windows() {
  Fl_Style::revert(); // revert to FLTK default styles

  Fl_Style::draw_boxes_inactive = 0;
  Fl_Style::inactive_color_weight = 0.30f;

  Fl_Widget::default_style.set_selection_color(FL_GRAY);
  Fl_Widget::default_style.set_highlight_color(0);
  Fl_Widget::default_style.set_label_type(&win98_label);
  Fl_Widget::default_style.set_label_size(12);

  Fl_Style* s;
  if ((s = Fl_Style::find("menu window"))) {
    s->set_box(&win98_menu_window_box);
    s->set_leading(6);
  }

  if ((s = Fl_Style::find("menu title"))) {
    s->set_box(&win98_menu_title_box);
    s->set_glyph_box(FL_NO_BOX);
  }

  if ((s = Fl_Style::find("menu item"))) {
    s->set_glyph_box(FL_NO_BOX);
  }

  if ((s = Fl_Style::find("menu bar"))) {
    s->set_highlight_color(FL_GRAY); // needed for title highlighting
  }

  if ((s = Fl_Style::find("input"))) {
    s->set_box(FL_DOWN_BOX);
  }

  if ((s = Fl_Style::find("slider"))) {
//    s->set_glyph_box(&win98_menu_window_box);
    s->set_box(FL_DOWN_BOX);
  }

  if ((s = Fl_Style::find("value slider"))) {
//    s->set_glyph_box(&win98_menu_window_box);
    s->set_box(FL_DOWN_BOX);
  }

  if ((s = Fl_Style::find("scrollbar"))) {
    s->set_glyph_box(&win98_menu_window_box);
  }

  if ((s = Fl_Style::find("output"))) {
    s->set_box(FL_DOWN_BOX);
  }

  if ((s = Fl_Style::find("counter"))) {
    s->set_box(FL_DOWN_BOX);
  }

// use FLTK default Windows colors
  if ((s = Fl_Style::find("output"))) {
    s->set_color(FL_WHITE);
  }

  if ((s = Fl_Style::find("counter"))) {
    s->set_color(FL_WHITE);
  }

  if ((s = Fl_Style::find("scrollbar"))) {
    s->set_color(52);
  }

  if ((s = Fl_Style::find("highlight button"))) {
    s->set_highlight_color(FL_GRAY);
  }

  if ((s = Fl_Style::find("menu title"))) {
    s->set_selection_color(FL_GRAY);
    s->set_selection_text_color(FL_BLACK);
    s->set_highlight_color(FL_GRAY);
  }

  Fl::redraw();

  return 0;
}

//
// End of "$Id: fl_windows.cxx,v 1.14 1999/11/19 15:11:57 carl Exp $".
//
