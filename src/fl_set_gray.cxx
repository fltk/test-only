//
// "$Id: fl_set_gray.cxx,v 1.6 1999/04/07 00:46:57 carl Exp $"
//
// Background (gray) color routines for the Fast Light Tool Kit (FLTK).
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

// -fg, -bg, and -bg2 switches

#include <FL/Fl.H>

void Fl::background(uchar r, uchar g, uchar b) {
  // replace the gray ramp so that color 47 is this color
  int i;
  for (i = 32; i <= 47; i++) {
    int m = (i-32)*255/23;
    Fl::set_color((Fl_Color)i,r*m/166,g*m/166,b*m/166);
  }
  for (; i < 56; i++) {
    int m = 255-(i-32)*255/23;
    Fl::set_color((Fl_Color)i,255-(255-r)*m/89,255-(255-g)*m/89,255-(255-b)*m/89);
  }
}

void Fl::foreground(uchar r, uchar g, uchar b) {
  Fl::set_color(FL_BLACK,r,g,b);
}

void Fl::background2(uchar r, uchar g, uchar b) {
  Fl::set_color(FL_WHITE,r,g,b);
}

//
// End of "$Id: fl_set_gray.cxx,v 1.6 1999/04/07 00:46:57 carl Exp $".
//
