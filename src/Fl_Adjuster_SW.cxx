//
// "$Id: Fl_Adjuster_SW.cxx,v 1.3 1999/05/06 05:52:11 carl Exp $"
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

#include <FL/Fl_Adjuster.H>

void Fl_Adjuster::attr(Attribute a, uchar c) {
  mstyle(&_style);
  ADJUSTER_STYLE->sbf |= bf(a);
  ADJUSTER_STYLE->adjuster(a)=c;
}

void Fl_Adjuster::fly_color(uchar c) { attr(FLY_COLOR, c); }
void Fl_Adjuster::fly_box(uchar c) { attr(FLY_BOX, c); }
