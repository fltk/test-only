/
// "$Id$
/
// X-specific color definitions for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2001 by Bill Spitzak and others
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

#include <config.h
#include <FL/Enumerations.H

// one of these for each color in fltk's "colormap"
// if overlays are enabled, another one for the overla
struct Fl_XColor 
  unsigned char r,g,b;	// actual color used by 
  unsigned char mapped;	// true when XAllocColor don
  unsigned long pixel;	// the X pixel to us
}
extern Fl_XColor fl_xmap[/*overlay*/][256]

// mask & shifts to produce xcolor for truecolor visuals
extern unsigned char fl_redmask, fl_greenmask, fl_bluemask
extern int fl_redshift, fl_greenshift, fl_blueshift, fl_extrashift

/
// End of "$Id$"
/
