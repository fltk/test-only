//
// "$Id: arci.cxx,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $"
//
// Carbon src (integer) drawing functions for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//


#include <FL/fl_draw.H>
#include <FL/x.H>
#include "Fl_Carbon_Display.H"

void Fl_Carbon_Display::arc(int x,int y,int w,int h,double a1,double a2) {
  if (w <= 0 || h <= 0) return;
  Rect r; r.left=x; r.right=x+w; r.top=y; r.bottom=y+h;
  a1 = a2-a1; a2 = 450-a2;
  FrameArc(&r, (short int)a2, (short int)a1);
}

void Fl_Carbon_Display::pie(int x,int y,int w,int h,double a1,double a2) {
  if (w <= 0 || h <= 0) return;
  Rect r; r.left=x; r.right=x+w; r.top=y; r.bottom=y+h;
  a1 = a2-a1; a2 = 450-a2;
  PaintArc(&r, (short int)a2, (short int)a1);
}

//
// End of "$Id: arci.cxx,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $".
//
