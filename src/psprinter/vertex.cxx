/
// "$Id$
/
// Postscript path drawing implementation for the Fast Light Tool Kit (FLTK)
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


struct matrix {double a, b, c, d, x, y;}
//extern matrix  * fl_matrix()

extern matrix * fl_matrix

void Fl_PS_Printer::concat()
  fprintf(output,"[%g %g %g %g %g %g] CT\n", fl_matrix->a , fl_matrix->b , fl_matrix->c , fl_matrix->d , fl_matrix->x , fl_matrix->y)


void Fl_PS_Printer::reconcat()
  fprintf(output, "[%g %g %g %g %g %g] RCT\n" , fl_matrix->a , fl_matrix->b , fl_matrix->c , fl_matrix->d , fl_matrix->x , fl_matrix->y)


/
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


*
/////////////////  transformed (double) drawings ///////////////////////////////


void Fl_PS_Printer::begin_points()
  fprintf(output, "GS\n")
  concat()

  fprintf(output, "BP\n")
  gap_=1
  shape_=POINTS
}

void Fl_PS_Printer::begin_line()
  fprintf(output, "GS\n")
  concat()
  fprintf(output, "BP\n")
  gap_=1
  shape_=LINE
}

void Fl_PS_Printer::begin_loop()
  fprintf(output, "GS\n")
  concat()
  fprintf(output, "BP\n")
  gap_=1
  shape_=LOOP
}

void Fl_PS_Printer::begin_polygon()
  fprintf(output, "GS\n")
  concat()
  fprintf(output, "BP\n")
  gap_=1
  shape_=POLYGON
}

void Fl_PS_Printer::vertex(double x, double y)
  if(shape_==POINTS)
    fprintf(output,"%g %g MT\n", x , y)
    gap_=1
    return
  
  if(gap_)
    fprintf(output,"%g %g MT\n", x , y)
    gap_=0
  }els
    fprintf(output, "%g %g LT\n", x , y)
}

void Fl_PS_Printer::curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3)
  if(shape_==NONE) return
    if(gap_
      fprintf(output,"%g %g MT\n", x , y)
    els
      fprintf(output, "%g %g LT\n", x , y)
    gap_=0

  fprintf(output, "%g %g %g %g %g %g curveto \n", x1 , y1 , x2 , y2 , x3 , y3)
}


void Fl_PS_Printer::circle(double x, double y, double r)
  if(shape_==NONE)
    fprintf(output, "GS\n")
    concat()
//    fprintf(output, "BP\n")
    fprintf(output,"%g %g %g 0 360 arc\n", x , y , r)
    reconcat()
//    fprintf(output, "ELP\n")
    fprintf(output, "GR\n")
  }els

    fprintf(output, "%g %g %g 0 360 arc\n", x , y , r)

}





void Fl_PS_Printer::arc(double x, double y, double r, double start, double a)
  if(shape_==NONE) return
  gap_=0
  if(start>a
    fprintf(output, "%g %g %g %g %g arc\n", x , y , r , -start, -a)
  els
    fprintf(output, "%g %g %g %g %g arcn\n", x , y , r , -start, -a)

}

void Fl_PS_Printer::end_points()
  gap_=1
  reconcat()
  fprintf(output, "ELP\n"); //?
  fprintf(output, "GR\n")
  shape_=NONE


void Fl_PS_Printer::end_line()
  gap_=1
  reconcat()
  fprintf(output, "ELP\n")
  fprintf(output, "GR\n")
  shape_=NONE

void Fl_PS_Printer::end_loop()
  gap_=1
  reconcat()
  fprintf(output, "ECP\n")
  fprintf(output, "GR\n")
  shape_=NONE


void Fl_PS_Printer::end_polygon()

  gap_=1
  reconcat()
  fprintf(output, "EFP\n")
  fprintf(output, "GR\n")
  shape_=NONE


void Fl_PS_Printer::transformed_vertex(double x, double y)
  reconcat()
  if(gap_)
    fprintf(output, "%g %g MT\n", x , y)
    gap_=0
  }els
    fprintf(output, "%g %g LT\n", x , y)
  concat()
}


/
// End of "$Id$
/







 







