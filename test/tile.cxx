//
// "$Id: tile.cxx,v 1.8 2001/07/23 09:50:06 spitzak Exp $"
//
// Tile test program for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl.h>
#include <fltk/Fl_Double_Window.h>
#include <fltk/Fl_Tile.h>
#include <fltk/Fl_Box.h>
#include <fltk/Fl_Button.h>

int main(int argc, char** argv) {
  Fl_Window window(320,320);
  window.box(FL_FLAT_BOX);
  window.resizable(window);
  Fl_Tile tile(10,10,300,300);
  Fl_Group box0(0,0,150,150,"0");
  box0.box(FL_DOWN_BOX);
  box0.color(9);
  box0.label_size(36);
  box0.set_flag(FL_ALIGN_CLIP);

  Fl_Button but(20, 20, 100, 30, "Button");
  box0.end();

  Fl_Window w1(150,0,150,150,"1");
  w1.box(FL_NO_BOX);
  Fl_Box box1(0,0,150,150,"1\nThis is a\nchild\nX window");
  box1.box(FL_DOWN_BOX);
  box1.color(19);
  box1.label_size(18);
  box1.set_flag(FL_ALIGN_CLIP);
  w1.resizable(box1);
  w1.end();

  //  Fl_Tile tile2(0,150,150,150);
  Fl_Box box2a(0,150,70,150,"2a");
  box2a.box(FL_DOWN_BOX);
  box2a.color(12);
  box2a.label_size(36);
  box2a.set_flag(FL_ALIGN_CLIP);
  Fl_Box box2b(70,150,80,150,"2b");
  box2b.box(FL_DOWN_BOX);
  box2b.color(13);
  box2b.label_size(36);
  box2b.set_flag(FL_ALIGN_CLIP);
  //tile2.end();

  //Fl_Tile tile3(150,150,150,150);
  Fl_Box box3a(150,150,150,70,"3a");
  box3a.box(FL_DOWN_BOX);
  box3a.color(12);
  box3a.label_size(36);
  box3a.set_flag(FL_ALIGN_CLIP);
  Fl_Box box3b(150,150+70,150,80,"3b");
  box3b.box(FL_DOWN_BOX);
  box3b.color(13);
  box3b.label_size(36);
  box3b.set_flag(FL_ALIGN_CLIP);
  //tile3.end();
  
  Fl_Box r(10,0,300-10,300-10);
  tile.resizable(r);
  //  r.box(FL_BORDER_FRAME);

  tile.end();
  window.end();
  window.show(argc,argv);
  w1.show();
  return Fl::run();
}

//
// End of "$Id: tile.cxx,v 1.8 2001/07/23 09:50:06 spitzak Exp $".
//
