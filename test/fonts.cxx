//
// "$Id$"
//
// Font demo program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <fltk3/run.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/TiledGroup.h>
#include <fltk3/HoldBrowser.h>
#include <fltk3/draw.h>
#include <fltk3/Box.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

fltk3::DoubleWindow *form;
fltk3::TiledGroup *tile;

class FontDisplay : public fltk3::Widget {
  void draw();
public:
  int font, size;
  FontDisplay(fltk3::Boxtype B, int X, int Y, int W, int H, const char* L = 0) :
    fltk3::Widget(X,Y,W,H,L) {box(B); font = 0; size = 14;}
};
void FontDisplay::draw() {
  draw_box();
  fltk3::font((fltk3::Font)font, size);
  fltk3::color(fltk3::BLACK);
  fltk3::draw(label(), x()+3, y()+3, w()-6, h()-6, align());
}

FontDisplay *textobj;

fltk3::HoldBrowser *fontobj, *sizeobj;

int **sizes;
int *numsizes;
int pickedsize = 14;

void font_cb(fltk3::Widget *, long) {
  int fn = fontobj->value();
  if (!fn) return;
  fn--;
  textobj->font = fn;
  sizeobj->clear();
  int n = numsizes[fn];
  int *s = sizes[fn];
  if (!n) {
    // no sizes
  } else if (s[0] == 0) {
    // many sizes;
    int j = 1;
    for (int i = 1; i<64 || i<s[n-1]; i++) {
      char buf[20];
      if (j < n && i==s[j]) {sprintf(buf,"@b%d",i); j++;}
      else sprintf(buf,"%d",i);
      sizeobj->add(buf);
    }
    sizeobj->value(pickedsize);
  } else {
    // some sizes
    int w = 0;
    for (int i = 0; i < n; i++) {
      if (s[i]<=pickedsize) w = i;
      char buf[20];
      sprintf(buf,"@b%d",s[i]);
      sizeobj->add(buf);
    }
    sizeobj->value(w+1);
  }
  textobj->redraw();
}

void size_cb(fltk3::Widget *, long) {
  int i = sizeobj->value();
  if (!i) return;
  const char *c = sizeobj->text(i);
  while (*c < '0' || *c > '9') c++;
  pickedsize = atoi(c);
  textobj->size = pickedsize;
  textobj->redraw();
}

char label[0x1000];

void create_the_forms() {
  // create the sample string
  int n = 0;
  strcpy(label, "Hello, world!\n");
  int i = strlen(label);
  ulong c;
  for (c = ' '+1; c < 127; c++) {
    if (!(c&0x1f)) label[i++]='\n';
    if (c=='@') label[i++]=c;
    label[i++]=c;
  }
  label[i++] = '\n';
  for (c = 0xA1; c < 0x600; c += 9) {
    if (!(++n&(0x1f))) label[i++]='\n';
    i += fltk3::utf8encode((unsigned int)c, label + i);
  }
  label[i] = 0;

  // create the basic layout
  form = new fltk3::DoubleWindow(550,370);

  tile = new fltk3::TiledGroup(0, 0, 550, 370);

  fltk3::Group *textgroup = new fltk3::Group(0, 0, 550, 185);
  textgroup->box(fltk3::FLAT_BOX);
  textobj = new FontDisplay(fltk3::FRAME_BOX,10,10,530,170,label);
  textobj->align(fltk3::ALIGN_TOP|fltk3::ALIGN_LEFT|fltk3::ALIGN_INSIDE|fltk3::ALIGN_CLIP);
  textobj->color(9,47);
  textgroup->resizable(textobj);
  textgroup->end();

  fltk3::Group *fontgroup = new fltk3::Group(0, 185, 550, 185);
  fontgroup->box(fltk3::FLAT_BOX);
  fontobj = new fltk3::HoldBrowser(10, 190, 390, 170);
  fontobj->box(fltk3::FRAME_BOX);
  fontobj->color(53,3);
  fontobj->callback(font_cb);
  sizeobj = new fltk3::HoldBrowser(410, 190, 130, 170);
  sizeobj->box(fltk3::FRAME_BOX);
  sizeobj->color(53,3);
  sizeobj->callback(size_cb);
  fontgroup->resizable(fontobj);
  fontgroup->end();

  tile->end();

  form->resizable(tile);
  form->end();
}

#include <fltk3/ask.h>

int main(int argc, char **argv) {
  fltk3::scheme(NULL);
  fltk3::args(argc, argv);
  fltk3::get_system_colors();
  create_the_forms();

// For the Unicode test, get all fonts...
//#ifdef __APPLE__
  int i = 0;
//#else
//  int i = fltk3::choice("Which fonts:","-*","iso8859","All");
//#endif
  int k = fltk3::set_fonts(i ? (i>1 ? "*" : 0) : "-*");
  sizes = new int*[k];
  numsizes = new int[k];
  for (i = 0; i < k; i++) {
    int t; const char *name = fltk3::get_font_name((fltk3::Font)i,&t);
    char buffer[128];
#if 1
    if (t) {
      char *p = buffer;
      if (t & fltk3::BOLD) {*p++ = '@'; *p++ = 'b';}
      if (t & fltk3::ITALIC) {*p++ = '@'; *p++ = 'i';}
	  *p++ = '@'; *p++ = '.'; // Suppress subsequent formatting - some MS fonts have '@' in their name
      strcpy(p,name);
      name = buffer;
    }
#else // this is neat, but really slow on some X servers:
    sprintf(buffer, "@F%d@.%s", i, name);
    name = buffer;
#endif
    fontobj->add(name);
    int *s; int n = fltk3::get_font_sizes((fltk3::Font)i, s);
    numsizes[i] = n;
    if (n) {
      sizes[i] = new int[n];
      for (int j=0; j<n; j++) sizes[i][j] = s[j];
    }
  }
  fontobj->value(1);
  font_cb(fontobj,0);
  form->show(argc,argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
