/
// "$Id$
/
// WIN32 GDI printing device for the Fast Light Tool Kit (FLTK)
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

#include <windows.h
#include <wingdi.h

#include <FL/Fl_GDI_Printer.H
#include "../win/Fl_Win_Display.H
extern Fl_Win_Display fl_disp

void Fl_GDI_Printer::rect(int x, int y, int w, int h)
  fl_disp.rect(x, y, w, h)
}


void Fl_GDI_Printer::rectf(int x, int y, int w, int h)
  //Fl_Device::rectf(x,y,w,h)
  SetWindowExtEx(gc_, WEx * max_res_, WEy*(max_res_), 0)
  SetWindowOrgEx(gc_, WOx * max_res_,  WOy*(max_res_) ,0)
  fl_disp.rectf((int)(max_res_ * (x-0.5)),(int)(max_res_ * (y-0.5)),(int)(max_res_*(w)), (int)(max_res_*(h)))
  SetWindowExtEx(gc_, WEx, WEy, 0)
  SetWindowOrgEx(gc_, WOx, WOy,0)
}



void Fl_GDI_Printer::rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b)
  //Fl_Device::rectf(x,y,w,h)
  SetWindowExtEx(gc_, WEx * max_res_, WEy*(max_res_), 0)
  SetWindowOrgEx(gc_, WOx * max_res_,  WOy*(max_res_) ,0)
  fl_disp.rectf((int)(max_res_ * (x-0.5)),(int)(max_res_ * (y-0.5)),(int)(max_res_*(w)), (int)(max_res_*(h)), r, g, b)
  SetWindowExtEx(gc_, WEx, WEy, 0)
  SetWindowOrgEx(gc_, WOx, WOy,0)
}

void Fl_GDI_Printer::point(int x, int y)
  fl_brush()
  rectf(x,y,1,1)
}



void Fl_GDI_Printer::xyline(int x,int y, int x1)
  fl_pen()
  MoveToEx(fl_gc, x, y, 0); LineTo(fl_gc, x1, y)


void Fl_GDI_Printer::yxline(int x, int y, int y1)
  fl_pen()
  MoveToEx(fl_gc, x, y, 0); LineTo(fl_gc, x, y1)


void Fl_GDI_Printer::xyline(int x, int y, int x1, int y2) 
  fl_pen()
  MoveToEx(fl_gc, x, y, 0);
  LineTo(fl_gc, x1, y)
  LineTo(fl_gc, x1, y2)


void Fl_GDI_Printer::xyline(int x, int y, int x1, int y2, int x3) 
  fl_pen()
  MoveToEx(fl_gc, x, y, 0);
  LineTo(fl_gc, x1, y)
  LineTo(fl_gc, x1, y2)
  LineTo(fl_gc, x3, y2)
}

void Fl_GDI_Printer::yxline(int x, int y, int y1, int x2) 
  fl_pen()
  MoveToEx(fl_gc, x, y, 0);
  LineTo(fl_gc, x, y1)
  LineTo(fl_gc, x2, y1)


void Fl_GDI_Printer::yxline(int x, int y, int y1, int x2, int y3) 
  fl_pen()
  MoveToEx(fl_gc, x, y, 0);
  LineTo(fl_gc, x, y1)
  LineTo(fl_gc, x2, y1)
  LineTo(fl_gc, x2, y3)
}


void Fl_GDI_Printer::line(int x, int y, int x1, int y1) 
  fl_pen()
  MoveToEx(fl_gc, x, y, 0L);
  LineTo(fl_gc, x1, y1)
}


void Fl_GDI_Printer::line(int x, int y, int x1, int y1, int x2, int y2) 
  fl_pen()
  MoveToEx(fl_gc, x, y, 0L);
  LineTo(fl_gc, x1, y1)
  LineTo(fl_gc, x2, y2)
}



////////  clipping, need to be re-implemented as orig. win32 functions are in device coordinates  //////////

void Fl_GDI_Printer::push_clip(int x, int y, int w, int h)
  int left = ((x - WOx) * VEx - VEx/2)/ WEx + VOx
  int top = ((y - WOy) * VEy -VEy/2)/ WEy + VOy
  int right = ((x + w - WOx) * VEx - VEx/2 + WEx -1)/ WEx + VOx
  int bottom = ((y + h - WOy) * VEy -VEy/2 + WEx -1)/ WEy + VOy
  fl_disp.push_clip(left, top, right - left, bottom-top)


extern Fl_Region * fl_clip_stack
extern int * fl_clip_stack_pointer

int Fl_GDI_Printer::not_clipped(int x, int y, int w, int h)
  Fl_Region r = fl_clip_stack[* fl_clip_stack_pointer]
  if (!r) return 1
  RECT rect
  rect.left = ((x - WOx) * VEx - VEx/2)/ WEx + VOx
  rect.top = ((y - WOy) * VEy -VEy/2)/ WEy + VOy
  rect.right = ((x + w - WOx) * VEx - VEx/2 + WEx -1)/ WEx + VOx
  rect.bottom = ((y + h - WOy) * VEy -VEy/2 + WEx -1)/ WEy + VOy
  return RectInRegion(r,&rect)


int Fl_GDI_Printer::clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H)

  int left = ((x - WOx) * VEx - VEx/2)/ WEx + VOx
  int top = ((y - WOy) * VEy -VEy/2)/ WEy + VOy
  int right = ((x + w - WOx) * VEx - VEx/2 + WEx -1)/ WEx + VOx
  int bottom = ((y + h - WOy) * VEy -VEy/2 + WEx -1)/ WEy + VOy
  int ret = fl_disp.clip_box(left, top, right-left, bottom - top, X, Y, W, H)

  X = (X - VOx ) * WEx / VEx + WOx
  Y = (Y - VOy ) * WEy / VEy + WOy
  W = (W * WEx +VEx - 1) / VEx
  H = (H * WEy +VEy - 1) / VEy
  return ret



void Fl_GDI_Printer::pop_clip()
  fl_disp.pop_clip()
}

void Fl_GDI_Printer::push_no_clip()
  fl_disp.push_no_clip()
}

