//
// "$Id: essai.cxx,v 1.19 1999/12/16 03:14:05 vincent Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Style.H>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <FL/Fl_Boxtype.H>
#include <FL/Fl_Shared_Image.H>

class Fl_Image_Box : public Fl_Boxtype_ {
  Fl_Flags mask;
  void inset(int& x,int& y,int& w,int& h) const {fl_up_box.inset(x,y,w,h);}
  int fills_rectangle() const {return true;}
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags) const;
  Fl_Shared_Image* normal_img;
  Fl_Shared_Image* down_img;
  Fl_Shared_Image* highlight_img;
  Fl_Image_Box(char*, char*, char*, Fl_Flags = 0);
};

void Fl_Image_Box::draw(int x, int y, int w, int h,
			Fl_Color fill, Fl_Flags flags) const
{
  Fl_Shared_Image* img;

  if (flags&FL_VALUE) img = down_img;
  else if (flags&FL_HIGHLIGHT) img = highlight_img;
  else img = normal_img;

  fl_up_box.draw(x,y,w,h,fill,(flags|FL_FRAME_ONLY)&(~mask));
  if (!(flags&FL_FRAME_ONLY)) {
    fl_up_box.inset(x,y,w,h);
    img->draw_tiled(x,y,w,h, -w/2, -h/2);
    if ((flags & FL_FOCUSED) && h > 4 && w > 4) {
      fl_color(FL_BLACK);
      fl_line_style(FL_DOT);
      fl_rect(x+1,y+1,w-2,h-2);
      fl_line_style(0);
    }
  }
}

Fl_Image_Box::Fl_Image_Box(char* normal_b, char* down_b, char* highlight_b, Fl_Flags m) :
Fl_Boxtype_(0), mask(m) {
  normal_img = Fl_JPEG_Image::get(fl_find_config_file(normal_b));
  down_img = Fl_JPEG_Image::get(fl_find_config_file(down_b));
  highlight_img = Fl_JPEG_Image::get(fl_find_config_file(highlight_b));
}

class Fl_Image_NoBorderBox : public Fl_Image_Box {
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags) const;
public:
  Fl_Image_NoBorderBox(char*a, char*b, char*c, Fl_Flags m = 0) : Fl_Image_Box(a,b,c,m) {}
};

void Fl_Image_NoBorderBox::draw(int x, int y, int w, int h,
				Fl_Color fill, Fl_Flags flags) const
{
  if (flags&(FL_VALUE|FL_HIGHLIGHT)) {
    Fl_Image_Box::draw(x,y,w,h,fill,flags);
    return;
  }
  Fl_Shared_Image* img;
  img = normal_img;
  if (!(flags&FL_FRAME_ONLY)) {
    img->draw_tiled(x, y, w, h, -w/2, -h/2);
    if ((flags & FL_FOCUSED) && h > 4 && w > 4) {
      fl_color(FL_BLACK);
      fl_line_style(FL_DOT);
      fl_rect(x+1,y+1,w-2,h-2);
      fl_line_style(0);
    }
  }
}

extern "C"
int fltk_theme(int, char**) {
  Fl_Style::revert();

  //  fl_background(0xD0D0E000); // it would be nice to figure out color from image
  Fl_Boxtype flat1 = new Fl_Image_NoBorderBox("themes/bg.jpeg", "themes/bg2.jpeg", "themes/bg3.jpeg");
  Fl_Boxtype flat2 = new Fl_Image_NoBorderBox("themes/bg2.jpeg", "themes/bg3.jpeg", "themes/bg3.jpeg", FL_VALUE);
  Fl_Boxtype box1 = new Fl_Image_Box("themes/bg2.jpeg", "themes/bg3.jpeg", "themes/bg3.jpeg");
  //Fl_Boxtype box2 = new Fl_Image_Box("themes/bg.jpeg", "themes/bg.jpeg", "themes/bg.jpeg");
  Fl_Boxtype box3 = new Fl_Image_Box("themes/bg2.jpeg", "themes/bg3.jpeg", "themes/bg3.jpeg", FL_VALUE);
  Fl_Widget::default_style->set_box(box1);
  Fl_Widget::default_style->set_glyph_box(box1);
  Fl_Widget::default_style->set_highlight_color(FL_LIGHT2);
  Fl_Style* s;
  if ((s = Fl_Style::find("window"))) {
    s->set_box(flat1);
  }
  if ((s = Fl_Style::find("menu item"))) {
    s->set_selection_text_color(FL_BLACK);
    s->set_box(flat2);
  }
  if ((s = Fl_Style::find("menu bar"))) {
    s->set_highlight_color(FL_GRAY); // required for highlighting
    s->set_box(flat2);
  }
  if ((s = Fl_Style::find("menu title"))) {
    s->set_highlight_color(FL_GRAY); // required for highlighting
    s->set_highlight_label_color(FL_BLACK);
    s->set_selection_text_color(FL_BLACK);
    s->set_box(flat2);
  }
  if ((s = Fl_Style::find("highlight button"))) {
    s->set_box(flat1);
  }
  if ((s = Fl_Style::find("button"))) {
    s->set_selection_text_color(FL_BLACK);
  }
  if ((s = Fl_Style::find("tabs"))) {
    s->set_box(box1);
  }
  if ((s = Fl_Style::find("light button"))) {
    s->set_box(box3);
  }
  return 0;
}

//
// End of "$Id: essai.cxx,v 1.19 1999/12/16 03:14:05 vincent Exp $".
//
