//
// "$Id: windows.cxx,v 1.7 2000/01/07 08:50:54 bill Exp $"
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

// Theme plugin for fltk to more accurately emulate the Windows 98 GUI
// than the fltk default does.  Due to popular demand (ugh) some of this
// may be moved to the default...

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Style.H>

////////////////////////////////////////////////////////////////
// Different box type used by win98 sometimes:

static const Fl_Frame_Box
win98_menu_window_box("win98 menu window", "2AARRMMUU", FL_DOWN_BOX);

////////////////////////////////////////////////////////////////
// Draw inactive labels as a thin engraved look:

static int engraved_data[2][3] = {{1,1,FL_LIGHT3},{0,0,0}};
// static int embossed_data[2][3] = {{-1,-1,FL_LIGHT3},{0,0,0}};

static const Fl_Engraved_Label
win98_engraved_label("windows engraved", engraved_data);

// static const Fl_Engraved_Label
// win98_embossed_label("windows embossed", embossed_data);

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

////////////////////////////////////////////////////////////////
// This glyph function just makes the inactive engraved look by calling
// the original function twice with different colors.
// Pretty nasty, but it works (WAS: I agree)

static void
inset_glyph(int t, int x, int y, int w, int h, Fl_Color bc, Fl_Color fc,
	    Fl_Flags f, Fl_Boxtype box, Fl_Glyph function)
{
  // Draw active widgets and slider thumbs or check or radio buttons normally:
  if (t == FL_GLYPH_VSLIDER || t == FL_GLYPH_HSLIDER ||
      t == FL_GLYPH_VNSLIDER || t == FL_GLYPH_HNSLIDER ||
      t == FL_GLYPH_CHECK || t == FL_GLYPH_RADIO)
  {
    function(t, x, y, w, h, bc, fc, f, box);
    return;
  }

  // draw the box once:
  if (box != FL_NO_BOX) {
    box->draw(x,y,w,h,bc,f);
    box->inset(x,y,w,h);
  }

  // Draw active widgets normally (this is called here becuase the above
  // inset call is bypassed in the normal fltk code and using it shrinks
  // the arrows just about the right amount to make them match win98:
  if (!(f & FL_INACTIVE)) {
    function(t, x, y, w, h, bc, fc, f, FL_NO_BOX);
    return;
  }

  f &= ~FL_INACTIVE;
  function(t, x+1, y+1, w, h, bc, FL_LIGHT3, f, FL_NO_BOX);
  function(t, x,   y,   w, h, bc, fc, f, FL_NO_BOX);
}

static Fl_Glyph fltk_glyph = 0;

static void
windows_glyph(int t, int x, int y, int w, int h, Fl_Color bc, Fl_Color fc,
              Fl_Flags f, Fl_Boxtype box)
{
  inset_glyph(t,x,y,w,h,bc,fc,f,box,fltk_glyph);
}

static Fl_Glyph return_glyph = 0;

static void
my_return_glyph(int t, int x, int y, int w, int h, Fl_Color bc, Fl_Color fc,
                Fl_Flags f, Fl_Boxtype box)
{
  inset_glyph(t,x,y,w,h,bc,fc,f,box,return_glyph);
}

static Fl_Glyph adjuster_glyph = 0;

static void
my_adjuster_glyph(int t, int x, int y, int w, int h, Fl_Color bc, Fl_Color fc,
                  Fl_Flags f, Fl_Boxtype box)
{
  inset_glyph(t,x,y,w,h,bc,fc,f,box,adjuster_glyph);
}

static Fl_Glyph counter_glyph = 0;

static void
my_counter_glyph(int t, int x, int y, int w, int h, Fl_Color bc, Fl_Color fc,
                 Fl_Flags f, Fl_Boxtype box)
{
  inset_glyph(t,x,y,w,h,bc,fc,f,box,counter_glyph);
}

////////////////////////////////////////////////////////////////

extern "C" int fltk_theme(int, char**)
{
  Fl_Style::draw_boxes_inactive = 0;
  Fl_Style::inactive_color_weight = 0.30f;
  Fl_Style::inactive_menu_hack = 1;

  // More accurate copy of the colors on the edges of boxes, from Win98
  // Fltk by default uses colors picked by Bill for aestetic reasons:
  fl_up_box.data = "2AAXXIIUU";
  fl_down_box.data = "2XXIIUUAA";

  Fl_Widget::default_style->set_label_type(&win98_label);

  Fl_Style* s;
  if ((s = Fl_Style::find("menu window"))) {
    s->set_box(&win98_menu_window_box);
    s->set_leading(6);
  }

  if ((s = Fl_Style::find("menu title"))) {
    s->set_box(FL_HIGHLIGHT_BOX);
    fltk_glyph = s->glyph;
    s->set_glyph_box(FL_NO_BOX);
    s->set_selection_color(FL_GRAY);
    s->set_selection_text_color(FL_BLACK);
    s->set_highlight_color(FL_GRAY);
  }

  if ((s = Fl_Style::find("menu item"))) {
    fltk_glyph = s->glyph;
    s->set_glyph(windows_glyph);
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
    fltk_glyph = s->glyph;
    s->set_glyph(windows_glyph);
    s->set_glyph_box(&win98_menu_window_box);
    s->set_color(52);
  }

  if ((s = Fl_Style::find("output"))) {
    s->set_color(FL_WHITE);
    s->set_box(FL_DOWN_BOX);
  }

  if ((s = Fl_Style::find("counter"))) {
    s->set_color(FL_WHITE);
    s->set_box(FL_DOWN_BOX);
  }

  if ((s = Fl_Style::find("highlight button"))) {
    s->set_highlight_color(FL_GRAY);
  }

  if ((s = Fl_Style::find("check button"))) {
    fltk_glyph = s->glyph;
    s->set_glyph(windows_glyph);
  }

  if ((s = Fl_Style::find("return button"))) {
    return_glyph = s->glyph;
    s->set_glyph(my_return_glyph);
  }

  if ((s = Fl_Style::find("menu button"))) {
    s->set_glyph(windows_glyph);
  }

  if ((s = Fl_Style::find("adjuster"))) {
    adjuster_glyph = s->glyph;
    s->set_glyph(my_adjuster_glyph);
  }

  if ((s = Fl_Style::find("counter"))) {
    counter_glyph = s->glyph;
    s->set_glyph(my_counter_glyph);
  }

  return 0;
}

//
// End of "$Id: windows.cxx,v 1.7 2000/01/07 08:50:54 bill Exp $"
//
