//
// "$Id: image.cxx,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $"
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

#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Pixmap.H>

#include "../win/Fl_Win_Display.H"
extern Fl_Win_Display fl_disp;





void Fl_GDI_Printer::draw_image(const uchar* data, int x, int y, int w, int h, int delta, int ldelta){
  fl_disp.draw_image(data, x, y, w, h, delta, ldelta);
};

void Fl_GDI_Printer::draw_image_mono(const uchar* data, int x, int y, int w,int h, int delta, int ld){
  fl_disp.draw_image_mono(data, x, y, w, h, delta, ld);
};

void Fl_GDI_Printer::draw_image(Fl_Draw_Image_Cb cb, void* data, int x, int y, int w, int h, int delta){
  fl_disp.draw_image(cb,data, x, y, w, h, delta);
};

void Fl_GDI_Printer::draw_image_mono(Fl_Draw_Image_Cb cb, void* data, int x, int y, int w, int h, int delta){
  fl_disp.draw_image_mono(cb, data, x, y, w, h, delta);
};



void Fl_GDI_Printer::draw(Fl_RGB_Image * rgb,int XP, int YP, int WP, int HP, int cx, int cy){
  check_image_cache(rgb); // we have to do thet before calling display device
  fl_disp.draw(rgb, XP, YP, WP, HP, cx, cy);
  rgb->uncache(); // display has wrong device descriptor;
}

void Fl_GDI_Printer::draw(Fl_Bitmap * bitmap,int XP, int YP, int WP, int HP, int cx, int cy){

  check_image_cache(bitmap); // we have to do thet before calling display device
  fl_disp.draw(bitmap, XP, YP, WP, HP, cx, cy);
  bitmap->uncache(); // display has wrong device descriptor;
}

void Fl_GDI_Printer::draw(Fl_Pixmap * pxm,int XP, int YP, int WP, int HP, int cx, int cy){
  check_image_cache(pxm);
  fl_disp.draw(pxm, XP, YP, WP, HP, cx, cy);
  pxm->uncache(); // display has wrong device descriptor;
}

//
// End of "$Id: image.cxx,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $"
//
