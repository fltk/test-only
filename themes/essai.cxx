//
// "$Id: essai.cxx,v 1.33 2002/02/10 22:57:50 spitzak Exp $"
//
// Theme plugin file for FLTK
//
// Copyright 1999 Bill Spitzak and others.
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

// essai.cxx

// Demonstration of a plugin that replaces the boxes such that a pixmap
// is used to draw everything.

#include <fltk/Fl.h>
#include <fltk/Fl_Widget.h>
#include <fltk/Fl_Window.h>
#include <fltk/Fl_Check_Button.h>
#include <fltk/Fl_Scrollbar.h>
#include <fltk/Fl_Input.h>
#include <fltk/Fl_Output.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Style.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fltk/Fl_Boxtype.h>
#include <fltk/Fl_Shared_Image.h>
#include <fltk/Fl_Tiled_Image.h>

class Fl_Image_Box : public Fl_Boxtype_ {
  Fl_Flags mask;
public:
  void draw(const int,int,int,int, Fl_Color, Fl_Flags) const;
  Fl_Tiled_Image* normal_img;
  Fl_Tiled_Image* down_img;
  Fl_Tiled_Image* highlight_img;
  Fl_Image_Box(const char*, const char*, const char*, Fl_Flags = 0);
};

void Fl_Image_Box::draw(int x, int y, int w, int h,
			Fl_Color color, Fl_Flags flags) const
{
  Fl_Tiled_Image* img;

  if (flags&FL_VALUE) img = down_img;
  else if (flags&FL_HIGHLIGHT) img = highlight_img;
  else img = normal_img;

  fl_up_box.draw(x,y,w,h,0,(flags|FL_INACTIVE)&(~mask));
  if (!(flags&FL_INACTIVE)) {
    fl_up_box.inset(x,y,w,h);
    img->draw(x,y,w,h);
  }
}

Fl_Image_Box::Fl_Image_Box(const char* normal_b, const char* down_b, const char* highlight_b, Fl_Flags m) :
Fl_Boxtype_(0), mask(m) {
  dx_ = fl_up_box.dx();
  dy_ = fl_up_box.dy();
  dw_ = fl_up_box.dw();
  dh_ = fl_up_box.dh();
  fills_rectangle_ = true;
  char buffer[256];
  normal_img =
    new Fl_Tiled_Image(Fl_JPEG_Image::get(fl_find_config_file(buffer,256,normal_b)));
  down_img =
    new Fl_Tiled_Image(Fl_JPEG_Image::get(fl_find_config_file(buffer,256,down_b)));
  highlight_img =
    new Fl_Tiled_Image(Fl_JPEG_Image::get(fl_find_config_file(buffer,256,highlight_b)));
}

class Fl_Image_NoBorderBox : public Fl_Image_Box {
  void draw(int,int,int,int, Fl_Color, Fl_Flags) const;
public:
  Fl_Image_NoBorderBox(const char*a, const char*b, const char*c, Fl_Flags m = 0) : Fl_Image_Box(a,b,c,m) {dx_=dy_=dw_=dh_=0;}
};

void Fl_Image_NoBorderBox::draw(int x, int y, int w, int h,
                                Fl_Color color, Fl_Flags flags) const
{
  if (flags&(FL_SELECTED|FL_HIGHLIGHT)) {
    Fl_Image_Box::draw(x,y,w,h, color, flags);
    return;
  }
  if (!(flags&FL_INACTIVE)) {
    normal_img->draw(x, y, w, h);
  }
}

extern "C" bool fltk_theme()
{

  //  fl_background(0xD0D0E000); // it would be nice to figure out color from image
  Fl_Boxtype flat1 = new Fl_Image_NoBorderBox("bg.jpeg", "bg2.jpeg", "bg3.jpeg");
  Fl_Boxtype flat2 = new Fl_Image_NoBorderBox("bg2.jpeg", "bg3.jpeg", "bg3.jpeg", FL_VALUE);
  Fl_Boxtype box1 = new Fl_Image_Box("bg2.jpeg", "bg3.jpeg", "bg3.jpeg");
  //Fl_Boxtype box2 = new Fl_Image_Box("bg.jpeg", "bg.jpeg", "bg.jpeg");
  Fl_Boxtype box3 = new Fl_Image_Box("bg2.jpeg", "bg3.jpeg", "bg3.jpeg", FL_VALUE);
  Fl_Widget::default_style->box = box1;
  Fl_Widget::default_style->button_box = box3;
  Fl_Widget::default_style->highlight_color = FL_LIGHT2;
  Fl_Style* s;
  if ((s = Fl_Style::find("window"))) {
    s->box = flat1;
  }
  if ((s = Fl_Style::find("group"))) {
    s->box = FL_NO_BOX;
  }
  if ((s = Fl_Style::find("menu"))) {
    s->selection_text_color = FL_BLACK;
    s->box = flat1;
    s->button_box = flat2;
  }
  if ((s = Fl_Style::find("menu bar"))) {
    s->highlight_color = FL_GRAY;
    s->highlight_label_color = FL_BLACK;
    s->box = flat2;
    s->selection_text_color = FL_BLACK;
    s->button_box = flat2;
  }
  if ((s = Fl_Style::find("highlight button"))) {
    s->box = flat2;
  }
  if ((s = Fl_Style::find("button"))) {
    s->selection_text_color = FL_BLACK;
    s->box = box3;
  }
  if ((s = Fl_Style::find("tabs"))) {
    s->box = box1;
  }
  if ((s = Fl_Style::find("light button"))) {
    s->box = box3;
  }
  return true;
}

//
// End of "$Id: essai.cxx,v 1.33 2002/02/10 22:57:50 spitzak Exp $".
//
