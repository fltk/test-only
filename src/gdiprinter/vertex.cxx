/
// "$Id$
/
// Path drawings for WIN32 GDI printing device for the Fast Light Tool Kit (FLTK)
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
// Please report all bugs and problems to http://www.fltk.org/str.ph
/

#include <math.h
#include <FL/math.h

#include <FL/Fl_GDI_Printer.H
#include "../win/Fl_Win_Display.H
extern Fl_Win_Display fl_disp

///////////////// we need to re-implement path drawings (double) for sub-pixel placement of vertices ////////////



void Fl_GDI_Printer::loop(int x1, int y1, int x2 ,int y2, int x3, int y3)
  fl_disp.loop(x1, y1, x2, y2, x3, y3)
}


void Fl_GDI_Printer::loop(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4)
  fl_disp.loop(x1, y1, x2, y2, x3, y3, x4, y4)
}


void Fl_GDI_Printer::polygon(int x1, int y1, int x2 ,int y2, int x3, int y3)
  fl_disp.polygon(x1, y1, x2, y2, x3, y3)
}


void Fl_GDI_Printer::polygon(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4)
  fl_disp.polygon(x1, y1, x2, y2, x3, y3, x4, y4)
}


void Fl_GDI_Printer::begin_points()
  fl_disp.begin_points()
}



void Fl_GDI_Printer::transformed_vertex(double x, double y)
  fl_disp.transformed_vertex((max_res_) * x, (max_res_) * y)
}

struct matrix {double a, b, c, d, x, y;}
extern FL_EXPORT matrix * fl_matrix

void Fl_GDI_Printer::vertex(double x, double y)
         transformed_vertex(fl_transform_x(x,y), fl_transform_y(x,y))
}

extern int fl_what_loop
enum {LINE, LOOP, POLYGON, POINT_}

void Fl_GDI_Printer::circle(double x, double y, double r)

  double xt = fl_transform_x(x,y)
  double yt = fl_transform_y(x,y)


  double rx = r * (fl_matrix->c ? sqrt(fl_matrix->a*fl_matrix->a+fl_matrix->c*fl_matrix->c) : fabs(fl_matrix->a))
  double ry = r * (fl_matrix->b ? sqrt(fl_matrix->b*fl_matrix->b+fl_matrix->d*fl_matrix->d) : fabs(fl_matrix->d))
  double llx = xt-rx
  double w = xt + rx - llx
  double lly = yt - ry
  double h = yt + ry - lly

  SetWindowExtEx(gc_, WEx*(max_res_), WEy*(max_res_), 0)
  SetWindowOrgEx(gc_, WOx*(max_res_) ,  WOy*(max_res_) ,0)

  if (fl_what_loop==POLYGON) 
    SelectObject(fl_gc, fl_brush())
    Pie(fl_gc, (int)rint((max_res_)*llx), (int)rint((max_res_)*lly), (int)rint((max_res_)*(llx+w)), (int)rint((max_res_)*(lly+h)), 0, 0, 0, 0)
	  SetWindowExtEx(gc_, WEx, WEy, 0)
	  SetWindowOrgEx(gc_, WOx, WOy,0)
  }else
    sty(style_, width_, dashes_, max_res_)
    fl_pen()
    Arc(fl_gc, (int)rint((max_res_)*llx), (int)rint((max_res_)*lly), (int)rint((max_res_)*(llx+w)), (int)rint((max_res_)*(lly+h)), 0, 0, 0, 0)
	  SetWindowExtEx(gc_, WEx, WEy, 0)
	  SetWindowOrgEx(gc_, WOx, WOy,0)
    sty(style_, width_, dashes_)
  



void Fl_GDI_Printer::curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3)
  fl_disp.curve(x, y, x1, y1, x2, y2, x3, y3)
}

void Fl_GDI_Printer::arc(double x, double y, double r, double start, double a)
  ((Fl_Display *)(&fl_disp))->arc(x, y, r, start, a)
}

void Fl_GDI_Printer::begin_line()
  fl_disp.begin_line()
}

void Fl_GDI_Printer::begin_loop()
  fl_disp.begin_loop()


void Fl_GDI_Printer::begin_polygon()
  fl_disp.begin_polygon()



void Fl_GDI_Printer::begin_complex_polygon()
  fl_disp.begin_complex_polygon()




void Fl_GDI_Printer::gap()
  fl_disp.gap()


void Fl_GDI_Printer::end_line()
  set_subpixel()
  fl_disp.end_line()
  set_normal()
}


void Fl_GDI_Printer::end_polygon()
  set_subpixel()
  fl_disp.end_polygon()
  set_normal()
}


void Fl_GDI_Printer::end_loop()
  set_subpixel()
  fl_disp.end_loop()
  set_normal()
}



void Fl_GDI_Printer::end_points()
  set_subpixel()
  fl_disp.end_points()
  set_normal()
}

void Fl_GDI_Printer::end_complex_polygon()
  set_subpixel()
  fl_disp.end_complex_polygon()
  set_normal()
}


/
// End of "$Id$
/


