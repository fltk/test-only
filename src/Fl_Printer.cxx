//
// "$Id$"
//
// Preferences methods for the Fast Light Tool Kit (FLTK).
//
// Copyright 2002-2004 by Matthias Melcher.
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


#include <FL/Fl_Printer.H>
#include <FL/Fl.H>
#include <stdio.h>
#include <stdlib.h>

//const int Fl_Printer::NO_PAGE_FORMATS=30;




//const char * Fl_Printer::format_names = fl_printer_format_names_en;
const int Fl_Printer::page_formats[NO_PAGE_FORMATS][2]={

  // A* // index(Ai) = i
  {2384, 3370}, //A0
  {1684, 2384}, //A1
  {1191, 1684}, //A2
  {842, 1191},  //A3
  {595, 842},   //A4
  {420, 595},   //A5
 {297, 420},   //A6
  {210, 297},   //A7
  {148, 210},   //A8
  {105, 148},   //A9

  // B* // index(Bi) = i+10
  {2920, 4127}, //B0
  {2064, 2920}, //B1
  {1460, 2064}, //B2
  {1032, 1460}, //B3
  {729, 1032},  //B4
  {516, 729},   //B5
  {316, 516},   //B6
  {258, 516},   //B7
  {181, 258},   //B8
  {127, 181},   //B9
  {91,127},     //B10

  // others (look at Fl_Printer.H} //
  {462, 649},
  {312, 623},
  {541, 719},
  {595, 935},
  {1224, 790},
  {612, 1009},
  {612, 790},
  {791, 1224},
  {297, 683}

};

const char * Fl_Printer::doc_info_ = "Fltk Application";

void Fl_Printer::place(double x, double y, double w, double h, double tx, double ty, double tw, double th, int align){

  double dx=0;
  double s = tw/w;
  double dy=(th - s*h)/2;
  if((th/h)<s){
    s=th/h;
    dy=0;
    dx=(tw- s*w)/2;
  }
  if(align & 3)
    if(align & FL_ALIGN_TOP)
      dy=0;
    else
      dy *=2;
  if(align & 12)
    if(align & FL_ALIGN_LEFT)
      dx=0;
    else
      dx *=2;
  place(x, y, tx + dx, ty + dy, s);

}

void Fl_Printer::alpha_background(Fl_Color c){
	Fl::get_color(c, bg_r, bg_g, bg_b);
}

Fl_Printer::Fl_Printer(){
	Fl::get_color(FL_GRAY, bg_r, bg_g, bg_b);
}


//
// End of "$Id$"
//


