//
// "$Id: motif.cxx,v 1.9 2000/01/10 06:31:32 bill Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Style.H>
#include <FL/fl_draw.H>

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

static void motif_glyph(int t, int x, int y, int w, int h, Fl_Color bc, Fl_Color fc,
                Fl_Flags f, Fl_Boxtype box)
{
  switch (t) {
    case FL_GLYPH_RADIO: {
      w = (w-1)|1; h = (h-1)|1;
      int x1 = x+w/2;
      int y1 = y+h/2;
      Fl_Color light = FL_LIGHT2, dark = FL_DARK3;
      if (f&FL_VALUE) { light = FL_DARK3; dark = FL_LIGHT2; }

      if (f&FL_INACTIVE)
        { fc = fl_inactive(fc); light = fl_inactive(light); dark = fl_inactive(dark); }
      fl_color( box == FL_NO_BOX ? bc : fc); // hack! for color
      fl_polygon(x+3,y1, x1,y+3, x+w-4,y1, x1,y+h-4);

      fl_color(light); fl_line(x, y1, x1, y, x+w-1, y1);
      fl_color(light); fl_line(x+1, y1, x1, y+1, x+w-2, y1);
      fl_color(light); fl_line(x+2, y1, x1, y+2, x+w-3, y1);

      fl_color(dark); fl_line(x, y1, x1, y+h-1, x+w-1, y1);
      fl_color(dark); fl_line(x+1, y1, x1, y+h-2, x+w-2, y1);
      fl_color(dark); fl_line(x+2, y1, x1, y+h-3, x+w-3, y1);
      break;
    }
    case FL_GLYPH_CHECK:
      x += 2; y += 2; w -= 4; h -= 4; // fudge factor
      thin_motif_up_box.draw(x, y, w, h, box == FL_NO_BOX ? bc : fc, f); // hack! for color
      break;
    case FL_GLYPH_RIGHT:
    case FL_GLYPH_LEFT:
    case FL_GLYPH_UP:
    case FL_GLYPH_DOWN: {
      if (box == FL_NO_BOX) { x += 4; y += 4; w -= 8; h -= 8; } // menu fudge factor
      Fl_Color d1, d2, l1, l2;
      if (f&FL_VALUE) {
        d1 = FL_LIGHT2; d2 = FL_LIGHT2; l1 = FL_DARK3; l2 = FL_DARK3;
      } else{
        l1 = FL_LIGHT2; l2 = FL_LIGHT2; d1 = FL_DARK3; d2 = FL_DARK3;
      }

      if (f&FL_INACTIVE) {
        l1 = fl_inactive(l1); l2 = fl_inactive(l2);
        d1 = fl_inactive(d1); d2 = fl_inactive(d2);
      }

      if (t == FL_GLYPH_RIGHT) {
        fl_color(bc); fl_polygon(x,y, x+w-1,y+h/2, x,y+h-1);
        fl_color(l2); fl_line(x+2,y+2, x+w-3,y+h/2);
        fl_color(d2); fl_line(x+2,y+h-3, x+w-3,y+h/2);
        fl_color(l2); fl_line(x+1,y+h-2, x+1,y+1, x+w-2,y+h/2);
        fl_color(d2); fl_line(x+1,y+h-2, x+w-2,y+h/2);
        fl_color(l1); fl_line(x,y+h-1, x,y, x+w-1,y+h/2);
        fl_color(d1); fl_line(x,y+h-1, x+w-1,y+h/2);
      } else if (t == FL_GLYPH_LEFT) {
        fl_color(bc); fl_polygon(x+w-1,y, x+w-1,y+h-1, x,y+h/2);
        fl_color(d2); fl_line(x+w-3,y+h-3, x+2,y+h/2);
        fl_color(l2); fl_line(x+w-3,y+2, x+2,y+h/2);
        fl_color(d2); fl_line(x+w-2,y+1, x+w-2,y+h-2, x+1,y+h/2);
        fl_color(l2); fl_line(x+w-2,y+1, x+1,y+h/2);
        fl_color(d1); fl_line(x+w-1,y, x+w-1,y+h-1, x,y+h/2);
        fl_color(l1); fl_line(x+w-1,y, x,y+h/2);
      } else if (t == FL_GLYPH_DOWN) {
        fl_color(bc); fl_polygon(x,y, x+w/2,y+h-1, x+w-1,y);
        fl_color(l2); fl_line(x+2,y+2, x+w/2,y+h-3);
        fl_color(d2); fl_line(x+w-3,y+2, x+w/2,y+h-3);
        fl_color(l2); fl_line(x+w-2,y+1, x+1,y+1, x+w/2,y+h-2);
        fl_color(d2); fl_line(x+w-2,y+1, x+w/2,y+h-2);
        fl_color(l1); fl_line(x+w-1,y, x,y, x+w/2,y+h-1);
        fl_color(d1); fl_line(x+w-1,y, x+w/2,y+h-1);
      } else { // UP
        fl_color(bc); fl_polygon(x,y+h-1, x+w-1,y+h-1, x+w/2,y);
        fl_color(d2); fl_line(x+w-3,y+h-3, x+w/2,y+2);
        fl_color(l2); fl_line(x+2,y+h-3, x+w/2,y+2);
        fl_color(d2); fl_line(x+1,y+h-2, x+w-2,y+h-2, x+w/2,y+1);
        fl_color(l2); fl_line(x+1,y+h-2, x+w/2,y+1);
        fl_color(d1); fl_line(x,y+h-1, x+w-1,y+h-1, x+w/2,y);
        fl_color(l1); fl_line(x,y+h-1, x+w/2,y);
      }
      break;
    }
    default:
      box->draw(x,y,w,h, bc, f);
  }
}

static void choice_glyph(int/*t*/, int x,int y,int w,int h, Fl_Color bc, Fl_Color,
		  Fl_Flags f, Fl_Boxtype box)
{
  int H = h/2;
  int Y = y + (h-H)/2;
  box->draw(x,Y,w,H, bc, f);
}

extern "C" int fltk_theme(int, char**)
{
  fl_up_box.data = thin_motif_up_box.data;
  fl_down_box.data = thin_motif_down_box.data;
  Fl_Style::draw_boxes_inactive = 0;

  Fl_Widget::default_style->box = &thin_motif_up_box;
  Fl_Widget::default_style->selection_color = FL_DARK1;
  Fl_Widget::default_style->glyph_box = &thin_motif_up_box;
  //Fl_Widget::default_style->highlight_color = 0;

  Fl_Style* s;

  if ((s = Fl_Style::find("menu item"))) {
    s->box = &thin_motif_menu_box;
    s->glyph = motif_glyph;
    s->glyph_box = FL_NO_BOX;
    s->selection_color = FL_GRAY;
    s->selection_text_color = FL_BLACK;
    s->off_color = FL_GRAY;
  }

  if ((s = Fl_Style::find("menu window"))) {
    s->leading = 0;
  }

  if ((s = Fl_Style::find("menu title"))) {
    s->box = &thin_motif_menu_box;
    s->glyph = motif_glyph;
    s->glyph_box = FL_NO_BOX;
    s->selection_color = FL_GRAY;
    s->selection_text_color = FL_BLACK;
    s->off_color = FL_GRAY;
  }

  if ((s = Fl_Style::find("check button"))) {
    s->glyph = motif_glyph;
    s->selection_color = FL_DARK1;
    s->off_color = FL_GRAY;
  }

  if ((s = Fl_Style::find("scrollbar"))) {
    s->glyph = motif_glyph;
    s->box = &thin_motif_down_box;
    s->color = FL_DARK1;
    s->selection_color = FL_DARK1;
  }

  if ((s = Fl_Style::find("slider"))) {
    s->box = &thin_motif_down_box;
    s->color = FL_DARK1;
    s->selection_color = FL_DARK1;
  }

  if ((s = Fl_Style::find("value slider"))) {
    s->box = &thin_motif_down_box;
    s->color = FL_DARK1;
    s->selection_color = FL_DARK1;
  }

  if ((s = Fl_Style::find("input"))) {
    s->color = FL_GRAY;
    s->selection_color = FL_BLACK;
    s->selection_text_color = FL_GRAY;
    s->box = &thin_motif_down_box;
  }

  if ((s = Fl_Style::find("output"))) {
    s->color = FL_GRAY;
    s->selection_color = FL_BLACK;
    s->selection_text_color = FL_GRAY;
    s->box = &thin_motif_down_box;
  }

  if ((s = Fl_Style::find("counter"))) {
    s->box = &thin_motif_down_box;
  }

  if ((s = Fl_Style::find("highlight button"))) {
    s->box = &thin_motif_highlight_box;
    s->highlight_color = FL_GRAY;
  }

  if ((s = Fl_Style::find("menu button"))) {
    s->glyph = motif_glyph;
  }

  if ((s = Fl_Style::find("choice"))) {
    s->glyph = choice_glyph;
    s->glyph_box = FL_UP_BOX;
  }

  if ((s = Fl_Style::find("browser"))) {
    s->color = FL_GRAY;
    s->selection_color = FL_BLACK;
    s->selection_text_color = FL_GRAY;
    s->box = &thin_motif_down_box;
  }
  return 0;
}

//
// End of "$Id: motif.cxx,v 1.9 2000/01/10 06:31:32 bill Exp $"
//
