//
// "$Id: motif.cxx,v 1.18 2002/03/06 08:50:45 spitzak Exp $"
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

// Motif L&F theme plugin for fltk.  A good example of the capabilities
// of theme plugins.

#include <fltk/Fl.h>
#include <fltk/Fl_Widget.h>
#include <fltk/Fl_Check_Button.h>
#include <fltk/Fl_Scrollbar.h>
#include <fltk/Fl_Input.h>
#include <fltk/Fl_Output.h>
#include <fltk/Fl_Style.h>
#include <fltk/fl_draw.h>

// some new boxtypes (look familiar?)
static const Fl_Frame_Box
thick_motif_down_box("motif thick down", "HHVVHHVVHHVV");

static const Fl_Frame_Box
thick_motif_up_box("motif thick up", "VVHHVVHHVVHH", &thick_motif_down_box);

static const Fl_Highlight_Box
thick_motif_highlight_box("motif thick highlight", &thick_motif_up_box);

static const Fl_Highlight_Box
thick_motif_menu_box("motif thick menu", &thick_motif_up_box);

static const Fl_Frame_Box
thin_motif_down_box("motif down", "HHVVHHVV");

static const Fl_Frame_Box
thin_motif_up_box("motif up","VVHHVVHH",&thin_motif_down_box);

static const Fl_Highlight_Box
thin_motif_highlight_box("motif thin highlight", &thin_motif_up_box);

static const Fl_Frame_Box
thin_motif_always_up_box("motif up","VVHHVVHH");

static const Fl_Highlight_Box
thin_motif_menu_box("motif thin menu", &thin_motif_always_up_box);

static void motif_check_glyph(const Fl_Widget* widget, int t,
			int x, int y, int w, int h, Fl_Flags f)
{
  Fl_Color fc = fl_inactive(widget->text_color(), f);
  Fl_Color bc = widget->color();
  if (widget->type()==Fl_Button::RADIO) {
    w = (w-1)|1; h = (h-1)|1;
    int x1 = x+w/2;
    int y1 = y+h/2;
    Fl_Color light = FL_LIGHT2, dark = FL_DARK3;
    if (f&FL_VALUE) { light = FL_DARK3; dark = FL_LIGHT2; }
//  if (f&FL_INACTIVE)
//    { light = fl_inactive(light); dark = fl_inactive(dark); }
    fl_color((f & FL_VALUE) ? fc : bc);
    fl_vertex(x+3,y1); fl_vertex(x1+1,y+2); fl_vertex(x+w-3,y1); fl_vertex(x1,y+h-4);
    fl_fill();

    fl_color(light); fl_line(x, y1, x1, y); fl_line(x1, y, x+w-1, y1);
    fl_color(light); fl_line(x+1, y1, x1, y+1); fl_line(x1, y+1, x+w-2, y1);
    fl_color(light); fl_line(x+2, y1, x1, y+2); fl_line(x1, y+2, x+w-3, y1);

    fl_color(dark); fl_line(x, y1, x1, y+h-1); fl_line(x1, y+h-1, x+w-1, y1);
    fl_color(dark); fl_line(x+1, y1, x1, y+h-2); fl_line(x1, y+h-2, x+w-2, y1);
    fl_color(dark); fl_line(x+2, y1, x1, y+h-3); fl_line(x1, y+h-3, x+w-3, y1);
  } else {
    x += 2; y += 2; w -= 4; h -= 4; // fudge factor
    thin_motif_up_box.draw(x, y, w, h,
			   (f & FL_VALUE) ? fc : bc, f);
  }
}

static void motif_glyph(const Fl_Widget* widget, int t,
			int x, int y, int w, int h, Fl_Flags f)
{
  Fl_Color bc = widget->button_color();
  switch (t) {
    case FL_GLYPH_RIGHT_BUTTON:
    case FL_GLYPH_LEFT_BUTTON:
    case FL_GLYPH_UP_BUTTON:
    case FL_GLYPH_DOWN_BUTTON:
      // draw background of scroll buttons
      fl_color(widget->color());
      fl_rectf(x,y,w,h);
      goto JUMP1;
    case FL_GLYPH_RIGHT:
    case FL_GLYPH_LEFT:
    case FL_GLYPH_UP:
    case FL_GLYPH_DOWN:
      // menu fudge factor
      if (w > 10) {x += (w-10)/2; y += (w-10)/2; w = h = 10;}
//	x += 2; y += 2; w -= 4; h -= 4;
//	x += 4; y += 4; w -= 8; h -= 8;
    JUMP1:
      if (h > w) {y += (h-w)/2; h = w;}
      else if (w > h) {x += (w-h)/2; w = h;}
      {Fl_Color d1, d2, l1, l2;
      if (f&FL_VALUE) {
        d1 = FL_LIGHT2; d2 = FL_LIGHT2; l1 = FL_DARK3; l2 = FL_DARK3;
      } else{
        l1 = FL_LIGHT2; l2 = FL_LIGHT2; d1 = FL_DARK3; d2 = FL_DARK3;
      }

//       if (f&FL_INACTIVE) {
//         l1 = fl_inactive(l1); l2 = fl_inactive(l2);
//         d1 = fl_inactive(d1); d2 = fl_inactive(d2);
//       }

      switch (t) {
      case FL_GLYPH_RIGHT:
      case FL_GLYPH_RIGHT_BUTTON:
        fl_color(bc); fl_newpath(); fl_vertex(x, y); fl_vertex(x+w-1, y+h/2);
        fl_vertex(x,y+h-1); fl_closepath(); fl_fill();
        fl_color(l2); fl_line(x+2,y+2, x+w-3,y+h/2);
        fl_color(d2); fl_line(x+2,y+h-3, x+w-3,y+h/2);
        fl_color(l2); fl_line(x+1,y+h-2, x+1,y+1); fl_line(x+1,y+1, x+w-2,y+h/2);
        fl_color(d2); fl_line(x+1,y+h-2, x+w-2,y+h/2);
        fl_color(l1); fl_line(x,y+h-1, x,y); fl_line(x,y, x+w-1,y+h/2);
        fl_color(d1); fl_line(x,y+h-1, x+w-1,y+h/2);
        break;
      case FL_GLYPH_LEFT:
      case FL_GLYPH_LEFT_BUTTON:
        fl_color(bc); fl_newpath(); fl_vertex(x+w-1, y); fl_vertex(x+w-1, y+h-1);
        fl_vertex(x,y+h/2); fl_closepath(); fl_fill();
        fl_color(d2); fl_line(x+w-3,y+h-3, x+2,y+h/2);
        fl_color(l2); fl_line(x+w-3,y+2, x+2,y+h/2);
        fl_color(d2); fl_line(x+w-2,y+1, x+w-2,y+h-2); fl_line(x+w-2,y+h-2, x+1,y+h/2);
        fl_color(l2); fl_line(x+w-2,y+1, x+1,y+h/2);
        fl_color(d1); fl_line(x+w-1,y, x+w-1,y+h-1); fl_line(x+w-1,y+h-1, x,y+h/2);
        fl_color(l1); fl_line(x+w-1,y, x,y+h/2);
        break;
      case FL_GLYPH_DOWN:
      case FL_GLYPH_DOWN_BUTTON:
        fl_color(bc); fl_newpath(); fl_vertex(x, y); fl_vertex(x+w/2, y+h-1);
        fl_vertex(x+w-1,y); fl_closepath(); fl_fill();
        fl_color(l2); fl_line(x+2,y+2, x+w/2,y+h-3);
        fl_color(d2); fl_line(x+w-3,y+2, x+w/2,y+h-3);
        fl_color(l2); fl_line(x+w-2,y+1, x+1,y+1); fl_line(x+1,y+1, x+w/2,y+h-2);
        fl_color(d2); fl_line(x+w-2,y+1, x+w/2,y+h-2);
        fl_color(l1); fl_line(x+w-1,y, x,y); fl_line(x,y, x+w/2,y+h-1);
        fl_color(d1); fl_line(x+w-1,y, x+w/2,y+h-1);
        break;
      case FL_GLYPH_UP:
      case FL_GLYPH_UP_BUTTON:
        fl_color(bc); fl_newpath(); fl_vertex(x, y+h-1); fl_vertex(x+w-1, y+h-1);
        fl_vertex(x+w/2,y); fl_closepath(); fl_fill();
        fl_color(d2); fl_line(x+w-3,y+h-3, x+w/2,y+2);
        fl_color(l2); fl_line(x+2,y+h-3, x+w/2,y+2);
        fl_color(d2); fl_line(x+1,y+h-2, x+w-2,y+h-2); fl_line(x+w-2,y+h-2, x+w/2,y+1);
        fl_color(l2); fl_line(x+1,y+h-2, x+w/2,y+1);
        fl_color(d1); fl_line(x,y+h-1, x+w-1,y+h-1); fl_line(x+w-1,y+h-1, x+w/2,y);
        fl_color(l1); fl_line(x,y+h-1, x+w/2,y);
        break;
      }
      break;
    }
    default:
      Fl_Widget::default_glyph(widget, t, x,y,w,h, f);
  }
}

static void choice_glyph(const Fl_Widget* widget, int,
			 int x,int y,int w,int h, Fl_Flags f)
{
  Fl_Color color = widget->color();
  fl_color(color);
  fl_rectf(x,y,w,h);
  int H = h/3;
  y += (h-H)/2;
  h = H;
  widget->box()->draw(x,y,w,h,color,f);
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

// This is the function that is called by fltk when the plugin is loaded:
extern "C" bool fltk_theme()
{
  // On X fl_get_system_colors reads the XResource database for
  // some Motif-like things like *background:
  fl_get_system_colors();

  Fl_Style::draw_boxes_inactive = false;

  Fl_Widget::default_style->button_box = &thin_motif_up_box;
  Fl_Widget::default_style->box = &thin_motif_down_box;
  Fl_Widget::default_style->color = FL_GRAY;
  Fl_Widget::default_style->selection_color = FL_BLACK;
  Fl_Widget::default_style->selection_text_color = FL_GRAY;
  Fl_Widget::default_style->glyph = motif_glyph;
  Fl_Widget::default_style->label_type = &motif_label;

  Fl_Style* s;

  if ((s = Fl_Style::find("button"))) {
    s->box = &thin_motif_up_box;
  }

  if ((s = Fl_Style::find("menu"))) {
    s->button_box = &thin_motif_menu_box; // this does not work anymore...
    //s->leading = 0;
  }

  if ((s = Fl_Style::find("menu bar"))) {
    s->button_box = &thin_motif_menu_box;
  }

  if ((s = Fl_Style::find("menu button"))) {
    s->button_box = &thin_motif_menu_box;
  }

  if ((s = Fl_Style::find("choice"))) {
    s->box = &thin_motif_up_box;
    s->glyph = choice_glyph;
  }

  if ((s = Fl_Style::find("scrollbar"))) {
    s->box = &thin_motif_down_box;
    s->color = FL_DARK1;
  }

  if ((s = Fl_Style::find("slider"))) {
    s->box = &thin_motif_down_box;
    s->color = FL_DARK1;
  }

  if ((s = Fl_Style::find("value slider"))) {
    s->color = FL_DARK1;
  }

  if ((s = Fl_Style::find("highlight button"))) {
    s->box = &thin_motif_highlight_box;
    s->highlight_color = FL_GRAY;
  }

  if ((s = Fl_Style::find("check button"))) {
    s->button_color = FL_DARK1;
    s->glyph = motif_check_glyph;
  }

  if ((s = Fl_Style::find("radio button"))) {
    s->button_color = FL_DARK1;
  }

  return true;
}

//
// End of "$Id: motif.cxx,v 1.18 2002/03/06 08:50:45 spitzak Exp $"
//
