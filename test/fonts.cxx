//
// "$Id: fonts.cxx,v 1.22 2001/02/21 06:15:45 clip Exp $"
//
// Font demo program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Check_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Fl_Window *form;

class FontDisplay : public Fl_Widget {
  void draw();
public:
  Fl_Font font; unsigned size; const char* encoding;
  FontDisplay(Fl_Boxtype B, int X, int Y, int W, int H, const char* L = 0) :
    Fl_Widget(X,Y,W,H,L) {box(B); font = 0; size = 14;}
};
void FontDisplay::draw() {
  draw_box();
// CET - FIXME - ??!?!  WTF?
//  int ix = x(), iy = y(), iw = w(), ih = h();
  int ix = 0, iy = 0, iw = w(), ih = h();
  box()->inset(ix, iy, iw, ih);
  fl_push_clip(ix, iy, iw, ih);
  fl_font(font, size); fl_encoding(encoding);
  fl_color(FL_BLACK);
  char buffer[32];
  for (int Y = 1; Y < 8; Y++) {
    for (int X = 0; X < 32; X++) buffer[X] = (32*Y+X);
//    fl_draw(buffer, 32, x()+3, y()+3+size*Y);
    fl_draw(buffer, 32, 3, 3+fl_height()*Y);
  }
  fl_font(FL_HELVETICA,10);
  fl_pop_clip();
}

FontDisplay *textobj;

Fl_Hold_Browser *fontobj, *sizeobj, *encobj;

Fl_Font* fonts; // list returned by fltk

Fl_Group *button_group;
Fl_Check_Button* bold_button, *italic_button;
Fl_Box* id_box;

int pickedsize = 14;

void font_cb(Fl_Widget *, long) {
  int fn = fontobj->value();
//printf("font: %d    name: %s   bigname: %s\n", fn, fonts[fn]->name(), fonts[fn]->system_name());

// CET - FIXME - new browser code has value starting from 0!
//  if (!fn) return;
//  fn--;

  Fl_Font f = fonts[fn];
  if (f->bold() == f) bold_button->deactivate();
  else bold_button->activate();
  if (f->italic() == f) italic_button->deactivate();
  else italic_button->activate();
  if (bold_button->value()) f = f->bold();
  if (italic_button->value()) f = f->italic();
  textobj->font = f;

// CET - FIXME - new browser code has value starting from 0!
  int pickedencoding = encobj->value();
  encobj->clear();
  const char** encodings; int ne = f->encodings(encodings);
  if (!ne) {
    textobj->encoding = fl_encoding();
  } else {
    pickedencoding = 0;
    for (int i = 0; i < ne; i++) {
      encobj->add(encodings[i]);
      if (!strcmp(encodings[i], fl_encoding())) pickedencoding = i;
    }
    textobj->encoding = encodings[pickedencoding];
// CET - FIXME - new browser code has value starting from 0!
    encobj->value(pickedencoding);
  }

  sizeobj->clear();
  int *s; int n = f->sizes(s);
  if (!n) {
    // no sizes (this only happens on X)
    fl_font(f, pickedsize);
    textobj->size = fl_height();
  } else if (s[0] == 0) {
    // many sizes;
    int j = 1;
    for (int i = 1; i<64 || i<s[n-1]; i++) {
      char buf[20];
      if (j < n && i==s[j]) {sprintf(buf,"@b%d",i); j++;}
      else sprintf(buf,"%d",i);
      sizeobj->add(buf);
    }
    sizeobj->value(pickedsize-1);
    textobj->size = pickedsize;
  } else {
    // some sizes
    int w = 0;
    for (int i = 0; i < n; i++) {
      if (s[i]<=pickedsize) w = i;
      char buf[20];
      sprintf(buf,"@b%d",s[i]);
      sizeobj->add(buf);
    }
    sizeobj->value(w);
    textobj->size = s[w];
  }
  encobj->redraw();
  sizeobj->redraw();
  textobj->redraw();
  id_box->label(textobj->font->system_name());
  id_box->redraw();
  button_group->redraw();
}

void encoding_cb(Fl_Widget *, long) {
  int i = encobj->value();
// CET - FIXME - new browser code has value starting from 0!
//  if (!i) return;
  textobj->encoding = encobj->text(i);
  textobj->redraw();
  id_box->redraw();
}

void size_cb(Fl_Widget *, long) {
  int i = sizeobj->value();
// CET - FIXME - new browser code has value starting from 0!
//  if (!i) return;
  const char *c = sizeobj->text(i);
  while (*c < '0' || *c > '9') c++;
  pickedsize = atoi(c);
  textobj->size = pickedsize;
  textobj->redraw();
  id_box->redraw();
}

void create_the_forms() {
  form = new Fl_Double_Window(550,390);

  textobj = new FontDisplay(FL_ENGRAVED_BOX,10,10,530,160);
  textobj->clear_flag(FL_ALIGN_MASK);
  textobj->set_flag(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_CLIP);
  id_box = new Fl_Box(10, 172, 530, 15);
  id_box->box(FL_ENGRAVED_BOX);
  id_box->label_size(10);
  id_box->set_flag(FL_ALIGN_INSIDE|FL_ALIGN_CLIP);
  button_group = new Fl_Group(10, 190, 140, 20);
  bold_button = new Fl_Check_Button(0, 0, 70, 20, "Bold");
  bold_button->callback(font_cb, 1);
  italic_button = new Fl_Check_Button(70, 0, 70, 20, "Italic");
  italic_button->callback(font_cb, 1);
  button_group->end();
  fontobj = new Fl_Hold_Browser(10, 210, 280, 170);
  fontobj->when(FL_WHEN_CHANGED);
  fontobj->callback(font_cb);
  form->resizable(fontobj);
  encobj = new Fl_Hold_Browser(300, 210, 100, 170);
  encobj->when(FL_WHEN_CHANGED);
  encobj->callback(encoding_cb, 1);
  sizeobj = new Fl_Hold_Browser(410, 210, 130, 170);
  sizeobj->when(FL_WHEN_CHANGED);
  sizeobj->callback(size_cb);
  form->end();
}

#include <FL/fl_ask.H>

int main(int argc, char **argv) {
  create_the_forms();
  int numfonts = fl_list_fonts(fonts);
  for (int i = 0; i < numfonts; i++) fontobj->add(fonts[i]->name());
// CET - FIXME - new browser code has value starting from 0!
  fontobj->value(0);
  font_cb(fontobj,0);
  form->show(argc,argv);
  return Fl::run();
}

//
// End of "$Id: fonts.cxx,v 1.22 2001/02/21 06:15:45 clip Exp $".
//
