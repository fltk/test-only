//
// "$Id: fl_glyph_adjuster.cxx,v 1.1 1999/11/21 06:23:28 carl Exp $"
//
// Glyph drawing code for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Style.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Bitmap.H>

#include "fastarrow.h"
static Fl_Bitmap fastarrow(fastarrow_bits, fastarrow_width, fastarrow_height);
#include "mediumarrow.h"
static Fl_Bitmap mediumarrow(mediumarrow_bits, mediumarrow_width, mediumarrow_height);
#include "slowarrow.h"
static Fl_Bitmap slowarrow(slowarrow_bits, slowarrow_width, slowarrow_height);

void fl_glyph_adjuster(int t, int x,int y,int w,int h, Fl_Color bc, Fl_Color fc,
                       Fl_Flags f, Fl_Boxtype box)
{
  box->draw(x,y,w,h, bc, f);
  fl_color(fc);

  switch (t) {
    case FL_GLYPH_FASTARROW:
      fastarrow.draw(x, y, w, h, FL_ALIGN_CENTER); break;
    case FL_GLYPH_MEDIUMARROW:
      mediumarrow.draw(x, y, w, h, FL_ALIGN_CENTER); break;
    case FL_GLYPH_SLOWARROW:
      slowarrow.draw(x, y, w, h, FL_ALIGN_CENTER); break;
  }
}

//
// End of "$Id: fl_glyph_adjuster.cxx,v 1.1 1999/11/21 06:23:28 carl Exp $".
//
