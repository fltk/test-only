//
// "$Id: Fl_Adjuster_SW.cxx,v 1.1 1999/03/14 06:46:25 carl Exp $"
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

void Fl_Adjuster::fly_color(uchar c) {
  wstyle();
  ADJUSTER_STYLE->sbf |= bf(FLY_COLOR);
  ADJUSTER_STYLE->adjuster(FLY_COLOR)=c;
}

void Fl_Adjuster::fly_box(uchar b) {
  wstyle();
  ADJUSTER_STYLE->sbf |= bf(FLY_BOX);
  ADJUSTER_STYLE->adjuster(FLY_BOX)=b;
}
