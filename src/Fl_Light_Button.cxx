/
// "$Id$
/
// Lighted button widget for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

// Subclass of Fl_Button where the "box" indicates whether it i
// pushed or not, and the "down box" is drawn small and square o
// the left to indicate the current state

// The default down_box of zero draws a rectangle designed to loo
// just like Flame's buttons

#include <FL/Fl.H
#include <FL/Fl_Light_Button.H
#include <FL/fl_draw.H>
#include <FL/Fl_Style.H>



void fl_draw_check(int x, int y, int w,int h)
      int tx = x + 3
      int tw = w - 6
      int d1 = tw/3
      int d2 = tw-d1
      int ty = y + (w+d2)/2-d1-2
      for (int n = 0; n < 3; n++, ty++) 
        fl_line(tx, ty, tx+d1, ty+d1)
        fl_line(tx+d1, ty+d1, tx+tw-1, ty+d1-d2+1)
      


/

void Fl_Light_Button::draw() 

  if (box()) draw_box(this==Fl::pushed() ? fl_down(box()) : box(), color())

  int W  = labelsize()
  int dy = (h() - W) / 2
  int dx = Fl::box_dx(box()) + 2

  // if (dy < 0) dy = 0;         // neg. offset o.k. for vertical centerin

  draw_box(down_box(), x()+dx, y()+dy, W, W, background())
	if (value()) 
	   fl_color(value() ? (active_r() ? selection_color() : fl_inactive(selection_color())) : color())
     symbol_.draw(x()+ dx + Fl::box_dx(down_box()), y()+ dy + Fl::box_dy(down_box()), W - Fl::box_dw(down_box()), W - Fl::box_dh(down_box()))
	
  draw_label(x()+W+2*dx, y(), w()-W-2*dx, h())
  if (Fl::focus() == this) draw_focus()


*

void Fl_Light_Button::draw() {
 
  if (box()) draw_box(this==Fl::pushed() ? fl_down(box()) : box(), color())
  Fl_Color col = value() ? (active_r() ? selection_color() 
                            fl_inactive(selection_color())) : color()
  int W
  int dx, dy

  W  = labelsize()
  dx = Fl::box_dx(box()) + 2
  dy = (h() - W) / 2
  // if (dy < 0) dy = 0;         // neg. offset o.k. for vertical centerin


  Fl_Boxtype d_box = down_box()
  if (d_box) 
    // draw other down_box() styles
     
      if(d_box == FL_DOWN_BOX ||
         d_box == FL_UP_BOX ||
         d_box == _FL_PLASTIC_DOWN_BOX |
         d_box == FL_PLASTIC_UP_BOX)
        // Check box..
        draw_box(down_box(), x()+dx, y()+dy, W, W, FL_BACKGROUND2_COLOR)
	      if (value()) 
	        fl_color(col)
	        int tx = x() + dx + 3
	        int tw = W - 6
	        int d1 = tw/3
	        int d2 = tw-d1
	        int ty = y() + dy + (W+d2)/2-d1-2
	        for (int n = 0; n < 3; n++, ty++) 
	          fl_line(tx, ty, tx+d1, ty+d1)
	          fl_line(tx+d1, ty+d1, tx+tw-1, ty+d1-d2+1)
	        
	      
      }else if(d_box == _FL_ROUND_DOWN_BOX ||
        d_box == _FL_ROUND_UP_BOX)
        // Radio button..
        draw_box(down_box(), x()+dx, y()+dy, W, W, FL_BACKGROUND2_COLOR)
	      if (value()) 
	        fl_color(col)
	        int tW = (W - Fl::box_dw(down_box())) / 2 + 1
	        if ((W - tW) & 1) tW++; // Make sure difference is even to cente
	        int tdx = dx + (W - tW) / 2
	        int tdy = dy + (W - tW) / 2

	        switch (tW) 
	        // Larger circles draw fine..
	          default 
              fl_pie(x() + tdx, y() + tdy, tW, tW, 0.0, 360.0)
	            break

            // Small circles don't draw well on many systems..
	          case 6 
	            fl_rectf(x() + tdx + 2, y() + tdy, tW - 4, tW)
	            fl_rectf(x() + tdx + 1, y() + tdy + 1, tW - 2, tW - 2)
	            fl_rectf(x() + tdx, y() + tdy + 2, tW, tW - 4)
	            break

	          case 5 
	          case 4 
	          case 3 
	            fl_rectf(x() + tdx + 1, y() + tdy, tW - 2, tW)
	            fl_rectf(x() + tdx, y() + tdy + 1, tW, tW - 2)
	            break

      	    case 2 
	          case 1 
	            fl_rectf(x() + tdx, y() + tdy, tW, tW)
	            break
	        
	      
        
      }else
        if(value()) // new code: it allows define the glyph by a pair of boxtype
          draw_box(fl_down(down_box()), x()+dx, y()+dy, W, W, col)
        els
          draw_box(down_box(), x()+dx, y()+dy, W, W, col)
      }
    
  } else 
    // if down_box() is zero, draw light button style
    int hh = h()-2*dy - 2
    int ww = W/2+1
    int xx = dx
    if (w()<ww+2*xx) xx = (w()-ww)/2
    if (Fl::scheme()) 
      col = active_r() ? selection_color() : fl_inactive(selection_color())
      fl_color(value() ? col : fl_color_average(col, FL_BLACK, 0.5f))
      fl_pie(x()+xx, y()+dy+1, ww, hh, 0, 360)
    } else 
      draw_box(FL_THIN_DOWN_BOX, x()+xx, y()+dy+1, ww, hh, col)
    
    dx = (ww + 2 * dx - W) / 2
  
  draw_label(x()+W+2*dx, y(), w()-W-2*dx, h())
  if (Fl::focus() == this) draw_focus()



int Fl_Light_Button::handle(int event) 
  switch (event) 
  case FL_RELEASE
    if (box()) redraw()
  default
    return Fl_Button::handle(event)
  


static void rectf(int x, int y, int w, int h){fl_rectf(x,y,w,h);}

Fl_Light_Button::Fl_Light_Button(int X, int Y, int W, int H, const char* l
: Fl_Button(X, Y, W, H, l)
  type(FL_TOGGLE_BUTTON)
  style_ = default_style();
  align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE)
}



Fl_Button::Style * Fl_Light_Button::default_style(){
  static Style * s = 0;
  if(!s){ // not yet initialized
    s = new Style(Fl_Button::default_style(), Style::ALL & ~Style::SELECTION_COLOR & ~Style::DOWN_BOX);
    s->selection_color(FL_RED);
    s->down_box(0);
  }
  return s;
}






/
// End of "$Id$"
/
