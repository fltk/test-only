//
// "$Id: line_style.cxx,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $"
//
// WIN32 GDI printing device for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl.H>
#include "../win/Fl_Win_Display.H"
extern Fl_Win_Display fl_disp;

void Fl_GDI_Printer::sty(int style, int width, char *dashes, int vex){
  if(!(width)){
    width=1;
    if(!style && !dashes)
      style |= FL_CAP_SQUARE | FL_JOIN_MITER; //adjustment for system drawings
  }
  if(!dashes || !(*dashes)){
    if((style & 0xff) && ((style & 0xf00)==FL_CAP_SQUARE)) // square caps do not seem to work with dasges, not sure why
      style = (style & ~0xf00)|FL_CAP_FLAT;
    fl_disp.line_style(style, width*vex);
  }else{

    // Following is shameless copy from original fl_line_style with modifications.
    // Has to be changed to avoid code redundance...

    static DWORD Cap[4]= {PS_ENDCAP_FLAT, PS_ENDCAP_FLAT, PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE};
    static DWORD Join[4]={PS_JOIN_ROUND, PS_JOIN_MITER, PS_JOIN_ROUND, PS_JOIN_BEVEL};
    int s1 = PS_GEOMETRIC | Cap[(style>>8)&3] | Join[(style>>12)&3];
    DWORD a[16]; int n = 0;
    s1 |= PS_USERSTYLE;
    for (n = 0; n < 16 && *dashes; n++) a[n] = vex * *dashes++;
    if ((style || n) && !width) width = 1; // fix cards that do nothing for 0?
    LOGBRUSH penbrush = {BS_SOLID,fl_RGB(),0}; // can this be fl_brush()?
    HPEN newpen = ExtCreatePen(s1, vex * width, &penbrush, n, n ? a : 0);
    if (!newpen) {
      Fl::error("Fl_GDI_Printer::line_style(): Could not create GDI pen object.");
      return;
    }
    HPEN oldpen = (HPEN)SelectObject(fl_gc, newpen);
    DeleteObject(oldpen);
    fl_current_xmap->pen = newpen;
  }
}


void Fl_GDI_Printer::line_style(int style, int width, char * dashes){
   sty(style_ = style, width_ = width, dashes_ = dashes);
};


