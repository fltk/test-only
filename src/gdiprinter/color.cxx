//
// "$Id: color.cxx,v 1.1.2.3 2004/05/12 22:13:39 rokan Exp $"
//
// Color implementation of WIN32 GDI printing device for the Fast Light Tool Kit (FLTK).
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
// Please report all bugs and problems to http://www.fltk.org/str.php
//

#include <FL/Fl_GDI_Printer.H>
#include <FL/Fl_Display.H>
#include "../win/Fl_Win_Display.H"

extern Fl_Win_Display fl_disp;

void Fl_GDI_Printer::color(Fl_Color c) {
  sty(0,0,0); // RK CRITICAL: temporary fix of broken line styles under windows
  fl_disp.color(c);
  sty(style_, width_, dashes_);
};

void Fl_GDI_Printer::color(unsigned char r, unsigned char g, unsigned char b){
   sty(0,0,0);
   fl_disp.color(r, g, b);
   sty(style_, width_, dashes_);
};



Fl_Color Fl_GDI_Printer::color(){
  return fl_disp.color();
};


//
// End of "$Id: color.cxx,v 1.1.2.3 2004/05/12 22:13:39 rokan Exp $"
//

