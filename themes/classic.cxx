//
// "$Id: classic.cxx,v 1.30 2002/01/20 10:12:48 spitzak Exp $"
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

#include <fltk/Fl.h>
#include <fltk/Fl_Widget.h>
#include <fltk/Fl_Check_Button.h>
#include <fltk/Fl_Scrollbar.h>
#include <fltk/Fl_Input.h>
#include <fltk/Fl_Output.h>
#include <fltk/Fl_Style.h>
#include <stdio.h>
#include <string.h>

static void choice_glyph(const Fl_Widget* widget, int,
			 int x,int y,int w,int h, Fl_Flags f)
{
  int H = 7;
  FL_THIN_UP_BOX->draw(x, y+(h-H)/2, w-4, H, widget->get_box_color(f), f);
}

// Disable the engraving of inactive labels:
class Motif_Labeltype : public Fl_Labeltype_ {
public:
  void draw(const char* label,
	    int X, int Y, int W, int H,
	    Fl_Color c, Fl_Flags f) const
    {
      if (f & FL_INACTIVE) {
	c = fl_inactive(c);
	f &= ~FL_INACTIVE;
      }
      FL_NORMAL_LABEL->draw(label, X,Y,W,H,c,f);
    }
  Motif_Labeltype(const char*n) : Fl_Labeltype_(n) {}
};
static const Motif_Labeltype motif_label(0);

static Fl_Frame_Box always_up_box(0, "AAAAWUJJUTNN");
static const Fl_Highlight_Box menu_title_box(0, &always_up_box);

extern const Fl_Frame_Box classic_down_box;
static const Fl_Frame_Box classic_up_box(0, "AAAAWUJJUTNN",&classic_down_box);
const Fl_Frame_Box classic_down_box(0, "NNTUJJUWAAAA",&classic_up_box);

extern "C" int fltk_plugin() {

  Fl_Widget::default_style->button_box = &classic_up_box;
  Fl_Widget::default_style->box = &classic_down_box;
  Fl_Widget::default_style->highlight_color = 0;
  Fl_Widget::default_style->label_size = 14;
  Fl_Widget::default_style->text_size = 14;
  Fl_Widget::default_style->selection_color = FL_DARK2;
  Fl_Widget::default_style->selection_text_color = 0;

  Fl_Style* s;
  if ((s = Fl_Style::find("button"))) {
    s->box = &classic_up_box;
  }
  if ((s = Fl_Style::find("menu"))) {
    s->selection_color = FL_WHITE;
    s->selection_text_color = 0;
    s->color = FL_GRAY;
  }
  if ((s = Fl_Style::find("item"))) {
    s->text_color = FL_BLACK;
  }
  if ((s = Fl_Style::find("menu_bar"))) {
    s->button_box = &menu_title_box;
    s->selection_color = FL_GRAY;
  }
  if ((s = Fl_Style::find("check_button"))) {
    s->text_color = FL_BLACK;
    //s->button_color = FL_GRAY;
  }
  if ((s = Fl_Style::find("output"))) {
    s->color = FL_GRAY;
  }
  if ((s = Fl_Style::find("choice"))) {
    s->glyph = choice_glyph;
  }
  return 0;
}

//
// End of "$Id: classic.cxx,v 1.30 2002/01/20 10:12:48 spitzak Exp $".
//
