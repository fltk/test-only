/
// "$Id$
/
// Postscript font drawing implementation for the Fast Light Tool Kit (FLTK)
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



#include <stdio.h
#include <math.h
#include <string.h



#include <FL/Fl_PS_Printer.H
#include <FL/Fl.H


// RK: CRITICAL -for font size

#ifdef __APPLE__
  #include "../carbon/Fl_Carbon_Display.H
  extern Fl_Carbon_Display fl_disp
#elif defined(WIN32
  #include "../win/Fl_Win_Display.H
  extern Fl_Win_Display fl_disp
#els
  #include "../xlib/Fl_Xlib_Display.H
  extern Fl_Xlib_Display fl_disp
#endif  // __APPLE



static const char *_fontNames[] = 
  "Helvetica"
  "Helvetica-Bold"
  "Helvetica-Oblique"
  "Helvetica-BoldOblique"
  "Courier"
  "Courier-Bold"
  "Courier-Oblique"
  "Courier-BoldOblique"
  "Times"
  "Times-Bold"
  "Times-Italic"

  "Times-BoldItalic"
  "Symbol"
  "Courier"
  "CourierBold"
  "ZapfDingbats
}
// TODO RK: CRITICAL: this is hacky/temporary implementation of fonts. All below should be replaced

void Fl_PS_Printer::font(int f, int s) 

  //fonted_=1
  if (f >= FL_FREE_FONT
    f = FL_COURIER
  fprintf(output, "/%s SF\n" , _fontNames[f])
  fprintf(output,"%i FS\n", s)
  fl_disp.font(f,s); //Dirty hack for font measurement ;-
  font_=f; size_=s
}



double Fl_PS_Printer::width(unsigned c)
  return fl_disp.width(c); //Dirty..


double Fl_PS_Printer::width(const char* s, int n){
  return fl_disp.width(s,n); //Very Dirty..

int Fl_PS_Printer::descent()
  return fl_disp.descent(); //A bit Dirty..

int Fl_PS_Printer::height()
  return fl_disp.height(); //Still Dirty..


/
// "$Id$
/






 







