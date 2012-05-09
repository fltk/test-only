//
// "$Id$"
//
// fltk3::PackedGroup test program for the Fast Light Tool Kit (FLTK).
//
// Rather crude test of the fltk3::PackedGroup object.
// Changing the type() of an fltk3::PackedGroup after it is displayed is not supported
// so I have to do a lot of resizing of things before that.
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
#include <fltk3/Button.h>
#include <fltk3/LightButton.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/ScrollGroup.h>
#include <fltk3/ValueSlider.h>
#include <fltk3/PackedGroup.h>

fltk3::PackedGroup *pack;
fltk3::ScrollGroup *scroll;

void type_cb(fltk3::LightButton*, long v) {
  for (int i = 0; i < pack->children(); i++) {
    fltk3::Widget* o = pack->child(i);
    o->resize(0,0,25,25);
  }
  pack->resize(0, 0, scroll->w(), scroll->h());
  pack->parent()->redraw();
  pack->type(uchar(v));
  pack->redraw();
}

void spacing_cb(fltk3::ValueSlider*o, long) {
  pack->spacing(int(o->value()));
  scroll->redraw();
}

int main(int argc, char **argv) {
  fltk3::DoubleWindow *w;
  {fltk3::DoubleWindow* o = new fltk3::DoubleWindow(360, 370);
    w = o;
    scroll = new fltk3::ScrollGroup(10,10,340,285);
    {fltk3::PackedGroup* o = new fltk3::PackedGroup(0, 0, 340, 285);
      pack = o;
      o->box(fltk3::DOWN_FRAME);
      o->clip_children(1);
      //o->box(fltk3::ENGRAVED_FRAME);
      new fltk3::Button(35, 35, 25, 25, "b1");
      new fltk3::Button(45, 45, 25, 25, "b2");
      new fltk3::Button(55, 55, 25, 25, "b3");
      new fltk3::Button(65, 65, 25, 25, "b4");
      new fltk3::Button(75, 75, 25, 25, "b5");
      new fltk3::Button(85, 85, 25, 25, "b6");
      new fltk3::Button(95, 95, 25, 25, "b7");
      new fltk3::Button(105, 105, 25, 25, "b8");
      new fltk3::Button(115, 115, 25, 25, "b9");
      new fltk3::Button(125, 125, 25, 25, "b10");
      new fltk3::Button(135, 135, 25, 25, "b11");
      new fltk3::Button(145, 145, 25, 25, "b12");
      new fltk3::Button(155, 155, 25, 25, "b13");
      new fltk3::Button(165, 165, 25, 25, "b14");
      new fltk3::Button(175, 175, 25, 25, "b15");
      new fltk3::Button(185, 185, 25, 25, "b16");
      new fltk3::Button(195, 195, 25, 25, "b17");
      new fltk3::Button(205, 205, 25, 25, "b18");
      new fltk3::Button(215, 215, 25, 25, "b19");
      new fltk3::Button(225, 225, 25, 25, "b20");
      new fltk3::Button(235, 235, 25, 25, "b21");
      new fltk3::Button(245, 245, 25, 25, "b22");
      new fltk3::Button(255, 255, 25, 25, "b23");
      new fltk3::Button(265, 265, 25, 25, "b24");
      o->end();
      w->resizable(o);
    }
    scroll->end();
    {fltk3::LightButton* o = new fltk3::LightButton(10, 305, 165, 25, "HORIZONTAL");
      o->type(fltk3::RADIO_BUTTON);
      o->callback((fltk3::Callback*)type_cb, (void*)(fltk3::PackedGroup::HORIZONTAL));
    }
    {fltk3::LightButton* o = new fltk3::LightButton(185, 305, 165, 25, "VERTICAL");
      o->type(fltk3::RADIO_BUTTON);
      o->value(1);
      o->callback((fltk3::Callback*)type_cb, (void*)(fltk3::PackedGroup::VERTICAL));
    }
    {fltk3::ValueSlider* o = new fltk3::ValueSlider(100, 335, 250, 25, "Spacing: ");
      o->align(fltk3::ALIGN_LEFT);
      o->type(fltk3::HORIZONTAL);
      o->range(0,30);
      o->step(1);
      o->callback((fltk3::Callback*)spacing_cb);
    }
    w->end();
  }
  w->show(argc, argv);
  return fltk3::run();
}

//
// End of "$Id$".
//
