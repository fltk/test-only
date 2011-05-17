//
// "$Id$"
//
// Progress bar widget routines.
//
// Copyright 2000-2010 by Michael Sweet.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//
// Contents:

//
//   Fl_Progress::draw()        - Draw the check button.
//   Fl_Progress::Fl_Progress() - Construct a Fl_Progress widget.
//

//
// Include necessary header files...
//

#include <fltk3/run.h>
#include <fltk3/Progress.h>
#include <fltk3/draw.h>


//
// Fl_Progress is a progress bar widget based off fltk3::Widget that shows a
// standard progress bar...
//


//
// 'Fl_Progress::draw()' - Draw the progress bar.
//

/** Draws the progress bar. */
void Fl_Progress::draw()
{
  int	progress;	// Size of progress bar...
  int	bx, by, bw, bh;	// Box areas...
  int	tx, tw;		// Temporary X + width


  // Get the box borders...
  bx = fltk3::box_dx(box());
  by = fltk3::box_dy(box());
  bw = fltk3::box_dw(box());
  bh = fltk3::box_dh(box());

  tx = x() + bx;
  tw = w() - bw;

  // Draw the progress bar...
  if (maximum_ > minimum_)
    progress = (int)(w() * (value_ - minimum_) / (maximum_ - minimum_) + 0.5f);
  else
    progress = 0;

  // Draw the box and label...
  if (progress > 0) {
    fltk3::Color c = labelcolor();
    labelcolor(fltk3::contrast(labelcolor(), selection_color()));

    fl_push_clip(x(), y(), progress + bx, h());
      draw_box(box(), x(), y(), w(), h(), active_r() ? selection_color() : fltk3::inactive(selection_color()));
      draw_label(tx, y() + by, tw, h() - bh);
    fl_pop_clip();

    labelcolor(c);

    if (progress<w()) {
      fl_push_clip(tx + progress, y(), w() - progress, h());
        draw_box(box(), x(), y(), w(), h(), active_r() ? color() : fltk3::inactive(color()));
        draw_label(tx, y() + by, tw, h() - bh);
      fl_pop_clip();
    }
  } else {
    draw_box(box(), x(), y(), w(), h(), active_r() ? color() : fltk3::inactive(color()));
    draw_label(tx, y() + by, tw, h() - bh);
  }
}


/**  
    The constructor creates the progress bar using the position, size, and label.
    
    You can set the background color with color() and the
    progress bar color with selection_color(), or you can set both colors
    together with color(unsigned bg, unsigned sel).
    
    The default colors are fltk3::BACKGROUND2_COLOR and fltk3::YELLOW, resp.
*/
Fl_Progress::Fl_Progress(int X, int Y, int W, int H, const char* L)
: fltk3::Widget(X, Y, W, H, L) {
  align(fltk3::ALIGN_INSIDE);
  box(fltk3::DOWN_BOX);
  color(fltk3::BACKGROUND2_COLOR, fltk3::YELLOW);
  minimum(0.0f);
  maximum(100.0f);
  value(0.0f);
}


//
// End of "$Id$".
//
