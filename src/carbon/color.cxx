/
// "$Id$
/
// MacOS color functions for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2003 by Bill Spitzak and others
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

// The fltk "colormap".  This allows ui colors to be stored in 8-bi
// locations, and provides a level of indirection so that global colo
// changes can be made.  Not to be confused with the X colormap, whic
// I try to hide completely

// MacOS - matt: the macintosh port does not support colormap

#include <config.h
#include <FL/Fl.H
#include <FL/x.H
#include <FL/fl_draw.H

#include "Fl_Carbon_Display.H



// Translations to mac data structures
Fl_XMap fl_xmap[256]

Fl_XMap* fl_current_xmap

Fl_Color fl_color_

void Fl_Carbon_Display::color(Fl_Color i) 
  fl_color_ = i
  int index
  uchar r, g, b
  if (i & 0xFFFFFF00) 
    // translate rgb colors into color inde
    r = i>>24
    g = i>>16
    b = i>> 8
  } else 
    // translate index into rgb
    index = i
    unsigned c = fl_cmap[i]
    r = c>>24
    g = c>>16
    b = c>> 8
  
  RGBColor rgb;
  rgb.red   = (r<<8)|r
  rgb.green = (g<<8)|g
  rgb.blue  = (b<<8)|b
  RGBForeColor(&rgb)


void Fl_Carbon_Display::color(uchar r, uchar g, uchar b) 
  RGBColor rgb;
  fl_color_ = fl_rgb_color(r, g, b)
  rgb.red   = (r<<8)|r
  rgb.green = (g<<8)|g
  rgb.blue  = (b<<8)|b
  RGBForeColor(&rgb)


void Fl::set_color(Fl_Color i, unsigned c) 
  if (fl_cmap[i] != c) 
    fl_cmap[i] = c
  


/
// End of "$Id$"
/
