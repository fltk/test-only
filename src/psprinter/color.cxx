//
// "$Id: color.cxx,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $"
//
// Postscript color implementation for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#include <stdio.h>

#include <FL/Fl_PS_Printer.H>
#include <FL/Fl.H>
 

//////////////////////////////  setting background for alpha /////////////////////////////////

void Fl_PS_Printer::bg_color(Fl_Color bg){bg_=bg;};

//////////////////////////////// Primitives: Colors  ////////////////////////////////////////////

void Fl_PS_Printer::color(Fl_Color c) {
  //colored_=1;
  color_=c;
  Fl::get_color(c, cr_, cg_, cb_);
  if (cr_==cg_ && cg_==cb_) {
    double gray = cr_/255.0;
    fprintf(output, "%g GL\n", gray);

  } else {
    double fr, fg, fb;
    fr = cr_/255.0;
    fg = cg_/255.0;
    fb = cb_/255.0;
    fprintf(output,"%g %g %g SRGB\n", fr , fg , fb);
  }
}

void Fl_PS_Printer::color(unsigned char r, unsigned char g, unsigned char b) {

  //colored_=1;
  cr_=r;cg_=g;cb_=b;
  if (r==g && g==b) {
    double gray = r/255.0;
    fprintf(output, "%g GL\n", gray);
  } else {
    double fr, fg, fb;
    fr = r/255.0;
    fg = g/255.0;
    fb = b/255.0;
    fprintf(output, "%g %g %g SRGB\n", fr , fg , fb);
  }
}
//
// End of "$Id: color.cxx,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $"
//
