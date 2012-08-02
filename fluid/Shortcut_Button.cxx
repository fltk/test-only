//
// "$Id$"
//
// Shortcut Button code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#include "Shortcut_Button.h"
#include "fluid.h"
#include "panel.h"
#include "Fl_Widget_Type.h"

#include <fltk3/run.h>
#include <fltk3/Menu_.h>
#include <fltk3/Button.h>
#include <fltk3/ValueInput.h>
#include <fltk3/TextDisplay.h>
#include <fltk3/draw.h>
#include "../src/fltk3/flstring.h"


////////////////////////////////////////////////////////////////
// Shortcut entry item in panel:

void Shortcut_Button::draw() {
  if (value()) draw_box(fltk3::DOWN_BOX, (fltk3::Color)9);
  else draw_box(fltk3::UP_BOX, fltk3::WHITE);
  fltk3::font(fltk3::HELVETICA,14); fltk3::color(fltk3::FOREGROUND_COLOR);
  if (use_FL_COMMAND && (svalue & (fltk3::CTRL|fltk3::META))) {
    char buf[1024];
    fl_snprintf(buf, 1023, "Command+%s", fltk3::shortcut_label(svalue&~(fltk3::CTRL|fltk3::META)));
    fltk3::draw(buf,6,0,w(),h(),fltk3::ALIGN_LEFT);
  } else {
    fltk3::draw(fltk3::shortcut_label(svalue),6,0,w(),h(),fltk3::ALIGN_LEFT);
  }
}

int Shortcut_Button::handle(int e) {
  when(0); type(fltk3::TOGGLE_BUTTON);
  if (e == fltk3::KEYBOARD) {
    if (!value()) return 0;
    unsigned v = fltk3::event_text()[0];
    if ( (v > 32 && v < 0x7f) || (v > 0xa0 && v <= 0xff) ) {
      if (isupper(v)) {
        v = tolower(v);
        v |= fltk3::SHIFT;
      }
      v = v | (fltk3::event_state()&(fltk3::META|fltk3::ALT|fltk3::CTRL));
    } else {
      v = (fltk3::event_state()&(fltk3::META|fltk3::ALT|fltk3::CTRL|fltk3::SHIFT)) | fltk3::event_key();
      if (v == fltk3::BackSpaceKey && svalue) v = 0;
    }
    if (v != svalue) {svalue = v; set_changed(); redraw(); do_callback(); }
    return 1;
  } else if (e == fltk3::UNFOCUS) {
    int c = changed(); value(0); if (c) set_changed();
    return 1;
  } else if (e == fltk3::FOCUS) {
    return 1;
  } else {
    int r = Button::handle(e);
    if (e == fltk3::RELEASE && value() && fltk3::focus() != this) take_focus();
    return r;
  }
}

void shortcut_in_cb(Shortcut_Button* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_button())
      i->svalue = ((fltk3::Button*)(Fl_Panel::selected_widget()->o))->shortcut();
    else if (Fl_Panel::selected_type()->is_input())
      i->svalue = ((fltk3::Input_*)(Fl_Panel::selected_widget()->o))->shortcut();
    else if (Fl_Panel::selected_type()->is_value_input())
      i->svalue = ((fltk3::ValueInput*)(Fl_Panel::selected_widget()->o))->shortcut();
    else if (Fl_Panel::selected_type()->is_text_display())
      i->svalue = ((fltk3::TextDisplay*)(Fl_Panel::selected_widget()->o))->shortcut();
    else {
      i->hide();
      return;
    }
    i->show();
    i->redraw();
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next)
      if (o->selected && o->is_button()) {
	fltk3::Button* b = (fltk3::Button*)(((Fl_Widget_Type*)o)->o);
        if (b->shortcut()!=i->svalue) mod = 1;
	b->shortcut(i->svalue);
	if (o->is_menu_item()) ((Fl_Widget_Type*)o)->redraw();
      } else if (o->selected && o->is_input()) {
	fltk3::Input_* b = (fltk3::Input_*)(((Fl_Widget_Type*)o)->o);
        if (b->shortcut()!=i->svalue) mod = 1;
	b->shortcut(i->svalue);
      } else if (o->selected && o->is_value_input()) {
	fltk3::ValueInput* b = (fltk3::ValueInput*)(((Fl_Widget_Type*)o)->o);
        if (b->shortcut()!=i->svalue) mod = 1;
	b->shortcut(i->svalue);
      } else if (o->selected && o->is_text_display()) {
	fltk3::TextDisplay* b = (fltk3::TextDisplay*)(((Fl_Widget_Type*)o)->o);
        if (b->shortcut()!=i->svalue) mod = 1;
	b->shortcut(i->svalue);
      }
    if (mod) set_modflag(1);
  }
}

//
// End of "$Id$".
//
