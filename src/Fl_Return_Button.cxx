//
// "$Id: Fl_Return_Button.cxx,v 1.18 1999/11/22 09:00:19 bill Exp $"
//
// Return button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

#include <FL/Fl.H>
#include <FL/Fl_Return_Button.H>
#include <FL/fl_draw.H>

Fl_Return_Button::Fl_Return_Button(int x,int y,int w,int h,const char *l)
  : Fl_Button(x,y,w,h,l)
{
  style(default_style);
}

// this is public so fl_draw_symbol can call it:
void fl_glyph_return(int, int x,int y,int w,int h, Fl_Color, Fl_Color fc,
                     Fl_Flags f, Fl_Boxtype)
{
  int size = w; if (h<size) size = h;
  int d = (size+2)/4; if (d<3) d = 3;
  int t = (size+9)/12; if (t<1) t = 1;
  int x0 = x+(w-2*d-2*t-1)/2;
  int x1 = x0+d;
  int y0 = y+h/2;
  fl_color(fl_inactive(fc, f));

  fl_polygon(x0,y0, x1,y0+d, x1, y0-d);
  fl_polygon(x1,y0+t, x1+d,y0+t, x1+d,y0-t, x1,y0-t);
  fl_polygon(x1+d,y0-d, x1+d+2*t,y0-d, x1+d+2*t,y0+t, x1+d,y0+t);
}

void Fl_Return_Button::draw() {
  Fl_Color lc = draw_button();
  int W = h();
  if (w()/3 < W) W = w()/3;
  draw_glyph(0, x()+w()-W-4, y(), W, h(),
	     Fl::belowmouse()==this ? FL_HIGHLIGHT : 0);
  draw_button_label(x(), y(), w()-W+4, h(), lc);
}

int Fl_Return_Button::handle(int event) {
  if (event == FL_SHORTCUT &&
      (Fl::event_key() == FL_Enter || Fl::event_key() == FL_KP_Enter)) {
    do_callback();
    return 1;
  } else
    return Fl_Button::handle(event);
}

static void revert(Fl_Style* s) {
  s->glyph_box = FL_NO_BOX;
  s->glyph = fl_glyph_return;
}

Fl_Style* Fl_Return_Button::default_style = new Fl_Named_Style("Return_Button", revert, &Fl_Return_Button::default_style);

//
// End of "$Id: Fl_Return_Button.cxx,v 1.18 1999/11/22 09:00:19 bill Exp $".
//
