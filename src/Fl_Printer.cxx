/
// "$Id$
/
// Preferences methods for the Fast Light Tool Kit (FLTK)
/
// Copyright 2002-2004 by Matthias Melcher
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


#include <FL/Fl_Printer.H
#include <FL/Fl.H
#include <stdio.h
#include <stdlib.h

//const int Fl_Printer::NO_PAGE_FORMATS=30




//const char * Fl_Printer::format_names = fl_printer_format_names_en
const int Fl_Printer::page_formats[NO_PAGE_FORMATS][2]=

  // A* // index(Ai) = 
  {2384, 3370}, //A
  {1684, 2384}, //A
  {1191, 1684}, //A
  {842, 1191},  //A
  {595, 842},   //A
  {420, 595},   //A
 {297, 420},   //A
  {210, 297},   //A
  {148, 210},   //A
  {105, 148},   //A

  // B* // index(Bi) = i+1
  {2920, 4127}, //B
  {2064, 2920}, //B
  {1460, 2064}, //B
  {1032, 1460}, //B
  {729, 1032},  //B
  {516, 729},   //B
  {316, 516},   //B
  {258, 516},   //B
  {181, 258},   //B
  {127, 181},   //B
  {91,127},     //B1

  // others (look at Fl_Printer.H} /
  {462, 649}
  {312, 623}
  {541, 719}
  {595, 935}
  {1224, 790}
  {612, 1009}
  {612, 790}
  {791, 1224}
  {297, 683

}

const char * Fl_Printer::doc_info_ = "Fltk Application"

void Fl_Printer::place(double x, double y, double w, double h, double tx, double ty, double tw, double th, int align)

  double dx=0
  double s = tw/w
  double dy=(th - s*h)/2
  if((th/h)<s)
    s=th/h
    dy=0
    dx=(tw- s*w)/2
  
  if(align & 3
    if(align & FL_ALIGN_TOP
      dy=0
    els
      dy *=2
  if(align & 12
    if(align & FL_ALIGN_LEFT
      dx=0
    els
      dx *=2
  place(x, y, tx + dx, ty + dy, s)



void Fl_Printer::alpha_background(Fl_Color c)
	Fl::get_color(c, bg_r, bg_g, bg_b)


Fl_Printer::Fl_Printer()
	Fl::get_color(FL_GRAY, bg_r, bg_g, bg_b)



/
// End of "$Id$
/


