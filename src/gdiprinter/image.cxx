//
// "$Id$"
//
// WIN32 GDI printing device for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
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

struct _img_data{
	void * img;
	int cx;
	int cy;
	uchar bg_r, bg_g, bg_b;
};




void Fl_GDI_Printer::alphamix(void * data,int x, int y, int w, uchar * buf){
	_img_data * d = (_img_data *) data;
  Fl_RGB_Image * img = (Fl_RGB_Image *)d->img;
	int l = img->ld();
	if(!l) l = img->w() * img->d();
	const uchar * from  = img->array + (y + d->cy) * l + (x + d->cx) * img->d();
	w++;
	for(;w ;w--){
		int a = from[3];
		* buf++ = (uchar)((a * (*from) + (0xff - a) * d->bg_r)/0xff);
		* buf++ = (uchar)((a * from[1] + (0xff - a) * d->bg_g)/0xff);
		* buf++ = (uchar)((a * from[2] + (0xff - a) * d->bg_b)/0xff);
		from += img->d();
	};
};

void Fl_GDI_Printer::mono_alphamix(void * data,int x, int y, int w, uchar * buf){
	_img_data * d = (_img_data *) data;
  Fl_RGB_Image * img = (Fl_RGB_Image *)d->img;
	int l = img->ld();
	if(!l) l = img->w() * img->d();
	const uchar * from  = img->array + (y + d->cy) * l + (x + d->cx) * img->d();
	w++;
	for(;w ;w--){
		int a = from[1];
		* buf++ = (uchar)((a * (*from) + (0xff - a) * d->bg_r)/0xff);
		* buf++ = (uchar)((a * (*from) + (0xff - a) * d->bg_g)/0xff);
		* buf++ = (uchar)((a * (*from) + (0xff - a) * d->bg_b)/0xff);
		from += img->d();
	};
};





void Fl_GDI_Printer::draw_image(const uchar* data, int x, int y, int w, int h, int delta, int ldelta){
  Fl_Offscreen id = fl_create_offscreen(w, h);
  fl_begin_offscreen(id);
  fl_disp.draw_image(data, 0, 0, w, h, delta, ldelta);
  fl_end_offscreen();
  fl_copy_offscreen(x, y, w, h, id, 0, 0);
  fl_delete_offscreen(id);
};

void Fl_GDI_Printer::draw_image_mono(const uchar* data, int x, int y, int w,int h, int delta, int ld){
  Fl_Offscreen id = fl_create_offscreen(w, h);
  fl_begin_offscreen(id);
  fl_disp.draw_image_mono(data, 0, 0, w, h, delta, ld);
  fl_end_offscreen();
  fl_copy_offscreen(x, y, w, h, id, 0, 0);
  fl_delete_offscreen(id);
};

void Fl_GDI_Printer::draw_image(Fl_Draw_Image_Cb cb, void* data, int x, int y, int w, int h, int delta){
  Fl_Offscreen id = fl_create_offscreen(w, h);
  fl_begin_offscreen(id);
  fl_disp.draw_image(cb, data, 0, 0, w, h, delta);
  fl_end_offscreen();
  fl_copy_offscreen(x, y, w, h, id, 0, 0);
  fl_delete_offscreen(id);
};

void Fl_GDI_Printer::draw_image_mono(Fl_Draw_Image_Cb cb, void* data, int x, int y, int w, int h, int delta){
  Fl_Offscreen id = fl_create_offscreen(w, h);
  fl_begin_offscreen(id);
  fl_disp.draw_image_mono(cb, data, 0, 0, w, h, delta);
  fl_end_offscreen();
  fl_copy_offscreen(x, y, w, h, id, 0, 0);
  fl_delete_offscreen(id);
};



void Fl_GDI_Printer::draw(Fl_RGB_Image * rgb,int XP, int YP, int WP, int HP, int cx, int cy){


  if(rgb->d() >3){
    _img_data  d = {rgb, cx, cy, bg_r, bg_g, bg_b};
    draw_image(&alphamix, &d, XP, YP, WP, HP,3);
  }else if(rgb->d()==2){ // mono alphablended image
    _img_data  d = {rgb, cx, cy, bg_r, bg_g, bg_b};
    draw_image(&alphamix, &d, XP, YP, WP, HP,3);
  }else{
    int ld = rgb->ld();
    if(!ld) ld = rgb->w() * rgb->d();
    fl_draw_image(rgb->array + cy*ld + cx * rgb->d(), XP, YP, WP, HP, rgb->d(), rgb->ld());
  }
}


static void bitmap_mix(void * data,int x, int y, int w, uchar * buf){
  _img_data * d = (_img_data *) data;
  Fl_Bitmap * img = (Fl_Bitmap *)d->img;
	int l = (img->w()+7)/8;
  const uchar * from = img->array + l*(y + d->cy) + (x + d->cx)/8;
  int si = x % 8;
  uchar mask = 1 << si;
  COLORREF  cref = fl_RGB();
  uchar r = GetRValue(cref);
  uchar g = GetGValue(cref);
  uchar b = GetBValue(cref);
  w++;
  for(; w; w--){
    if(*from & mask){
     * buf++ = r;
     * buf++ = g;
     * buf++ = b;
    }else{
     * buf++ = d->bg_r;
     * buf++ = d->bg_g;
     * buf++ = d->bg_b;
    };
    if(mask ==128){
      from++;
      mask = 1;
    }else
      mask <<=1;
  };
};



void Fl_GDI_Printer::draw(Fl_Bitmap * img,int XP, int YP, int WP, int HP, int cx, int cy){
    _img_data  d = {img, cx, cy, bg_r, bg_g, bg_b};
    draw_image(&bitmap_mix, &d, XP, YP, WP, HP,3);

}

void Fl_GDI_Printer::draw(Fl_Pixmap * img,int XP, int YP, int WP, int HP, int cx, int cy){

  Fl_Offscreen id = fl_create_offscreen(img->w(), img->h());
  fl_begin_offscreen(id);
  fl_draw_pixmap(img->data(),0,0,bg_r_,bg_g_,bg_b_);
  fl_end_offscreen();
  fl_copy_offscreen(XP, YP, WP, HP, id, cx, cy);
  fl_delete_offscreen(id);


}

//
// End of "$Id$"
//
