/
// "$Id$
/
// Postscript arc(s) implementation for the Fast Light Tool Kit (FLTK)
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


#include <stdio.h
#include <math.h
#include <string.h

#include <FL/Fl_PS_Printer.H
#include <FL/Fl.H




void Fl_PS_Printer::arc(int x, int y, int w, int h, double a1, double a2) 


  fprintf(output, "GS\n")
  //fprintf(output, "BP\n")
  begin_line()
  fprintf(output, "%g %g TR\n", x + w/2.0 -0.5 , y + h/2.0 - 0.5)
  fprintf(output, "%g %g SC\n", (w-1)/2.0 , (h-1)/2.0 )
  arc(0,0,1,a2,a1)
//  fprintf(output, "0 0 1 %g %g arc\n" , -a1 , -a2)
  fprintf(output, "%g %g SC\n", 2.0/(w-1) , 2.0/(h-1) )
  fprintf(output, "%g %g TR\n", -x - w/2.0 +0.5 , -y - h/2.0 +0.5)
  end_line()


//  fprintf(output, "%g setlinewidth\n",  2/sqrt(w*h))
  //fprintf(output, "ELP\n")
//  fprintf(output, 2.0/w , 2.0/w , " SC\n"

//  fprintf(output, (-x - w/2.0) , (-y - h/2)  , " TR\n"
  fprintf(output, "GR\n")


void Fl_PS_Printer::pie(int x, int y, int w, int h, double a1, double a2) 

  fprintf(output, "GS\n")
  fprintf(output, "%g %g TR\n", x + w/2.0 -0.5 , y + h/2.0 - 0.5)
  fprintf(output, "%g %g SC\n", (w-1)/2.0 , (h-1)/2.0 )
  begin_polygon()
  vertex(0,0)
  arc(0.0,0.0, 1, a2, a1)
  end_polygon()
  fprintf(output, "GR\n")





/
// End of "$Id$"
/









 







