//
// "$Id: line_style.cxx,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $"
//
// Postscript line style  drawing implementation for the Fast Light Tool Kit (FLTK).
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
#include <math.h>
#include <string.h>

#include <FL/Fl_PS_Printer.H>
#include <FL/Fl.H>
 




static int dashes_flat[5][7]={
  {-1,0,0,0,0,0,0},
  {3,1,-1,0,0,0,0},
  {1,1,-1,0,0,0,0},
  {3,1,1,1,-1,0,0},
  {3,1,1,1,1,1,-1}
};


//yeah, hack...
static double dashes_cap[5][7]={
  {-1,0,0,0,0,0,0},
  {2,2,-1,0,0,0,0},
  {0.01,1.99,-1,0,0,0,0},
  {2,2,0.01,1.99,-1,0,0},
  {2,2,0.01,1.99,0.01,1.99,-1}
};





void Fl_PS_Printer::line_style(int style, int width, char* dashes){
  //line_styled_=1;
 
  linewidth_=width;
  linestyle_=style;
  //dashes_= dashes;
  if(dashes){
    if(dashes != linedash_)
      strcpy(linedash_,dashes);

  }else
    linedash_[0]=0;
  char width0 = 0;
  if(!width){
    width=1; //for screen drawing compatability
    width0=1;
  }

  fprintf(output, "%i setlinewidth\n", width);

  if(!style && (!dashes || !(*dashes)) && width0) //system lines
    style = FL_CAP_SQUARE;

  int cap = (style &0xf00) >> 8;
  if(cap) cap--;
  fprintf(output,"%i setlinecap\n", cap);

  int join = (style & 0xf000) >> 12;

  if(join) join--;
  fprintf(output,"%i setlinejoin\n", join);
  
  
  fprintf(output, "[");
  if(dashes && *dashes){
    while(*dashes){
      fprintf(output, "%i ", *dashes);
      dashes++;
    }
  }else{
    int * ds; 
    if(style & 0x200){ // round and square caps, dash length need to be adjusted
        double *dt = dashes_cap[style & 0xff];
        while (*dt >= 0){
          fprintf(output, "%g ",width * (*dt));
          dt++;
        }
      }else{

        ds = dashes_flat[style & 0xff];
        while (*ds >= 0){
          fprintf(output, "%i ",width * (*ds));
        ds++;
      }
    }
  }
  fprintf(output, "] 0 setdash\n");
};











  







