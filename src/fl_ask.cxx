//
// "$Id: fl_ask.cxx,v 1.27 2002/12/10 02:00:56 easysw Exp $"
//
// Standard dialog functions for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

// Implementation of message, ask, choice, input
// The three-message show_x functions are for forms compatibility
// mostly.  In most cases it is easier to get a multi-line message
// by putting newlines in the message.

#include <fltk/events.h>
#include <fltk/draw.h>
#include <fltk/ask.h>
#include <fltk/Button.h>
#include <fltk/ReturnButton.h>
#include <fltk/Window.h>
#include <fltk/Input.h>
#include <fltk/SecretInput.h>
#include <fltk/vsnprintf.h>
#include <string.h>
using namespace fltk;

static void m_revert(Style* s) {
  s->box = NO_BOX;
}
static NamedStyle m_style("Message", m_revert, &message_style);
NamedStyle* fltk::message_style = &m_style;

static void i_revert(Style* s) {
  s->box = THIN_UP_BOX;
  s->labelfont = TIMES_BOLD;
  s->labelsize = 34;
  s->color = WHITE;
  s->labelcolor = BLUE;
}
static NamedStyle i_style("Icon", i_revert, &fltk::icon_style);
NamedStyle* fltk::icon_style = &i_style;

static int button_number;
static void set_button_number(Widget* w, long a) {
  button_number = a;
  w->window()->hide();
}

#define ICON_W 50
#define ICON_H 50
#define BORDER_W 10
#define BORDER_H 10
#define INPUT_W 270
#define BUTTON_W 75
#define BUTTON_H 21

static Input *textfield;

// In this call the buttons are in backwards order from right to left,
// this is because "no" (0) is on the right.
static int innards(
  const char* iconlabel,
  const char *istr, int itype,
  const char* fmt, va_list ap,
  const char *b0,
  const char *b1,
  const char *b2)
{
  Window window(3*BORDER_W+ICON_W+INPUT_W, 3*BORDER_H+ICON_H+BUTTON_H);
  window.begin();

  // This keeps the icon from resizing.
  Group ig(BORDER_W, BORDER_H, ICON_W, ICON_H);
  ig.begin();
  Widget icon(0, 0, ICON_W, ICON_H);
  icon.style(icon_style);
  icon.label(iconlabel);
  ig.end();

  Widget message(2*BORDER_W+ICON_W, 0, INPUT_W, 2*BORDER_H+ICON_H);
  message.set_flag(ALIGN_LEFT|ALIGN_INSIDE|ALIGN_WRAP);
  message.style(message_style);

  if (textfield) {delete textfield; textfield = 0;}
  if (istr) {
    textfield = new Input(2*BORDER_W+ICON_W, 0, INPUT_W, 0);
    textfield->h(int(textfield->textsize())+10);
    textfield->y(BORDER_H+ICON_H-textfield->h());
    message.h(textfield->y());
    textfield->type(itype);
    textfield->value(istr);
    window.focus(textfield);
  }

  window.resizable(message);
//  w->size_range(window.w(), window.h(), 0, window.h());

  char buffer[1024];
  if (!strcmp(fmt,"%s")) {
    message.label(va_arg(ap, const char*));
  } else if (!strchr(fmt, '%')) {
    message.label(fmt);
  } else {
    vsnprintf(buffer, 1024, fmt, ap);
    message.label(buffer);
  }

  window.end();
  window.layout();
  setfont(message.labelfont(), message.labelsize());
  int w = 800; int h; measure(message.label(), w, h, 0); w+=6; h+=6;
  w -= message.w(); if (w < 0) w = 0;
  h -= message.h(); if (h < 0) h = 0;
  window.size(window.w()+w, window.h()+h);
  window.layout();

  window.begin();

  const char* blabels[3];
  blabels[0] = b0;
  blabels[1] = b1;
  blabels[2] = b2;

  // Button with '*' is default, if none the left-most one is:
  int default_button = 0;
  int i;
  for (i = 2; i >= 0; i--) if (blabels[i]) {
    if (!default_button) default_button = i;
    if (blabels[i][0] == '*') {blabels[i]++; default_button = i;}
  }

  for (i = 3; i--;) if (blabels[i]) {
    Button* button;
    if (i == default_button) {
      button = new ReturnButton(
	window.w()-(BUTTON_W+BORDER_W)*(i+1),
	window.h()-(BORDER_H+BUTTON_H), BUTTON_W, BUTTON_H, blabels[i]);
      window.hotspot(button);
      if (!textfield) window.focus(button);
    } else {
      button = new Button(
	window.w()-(BUTTON_W+BORDER_W)*(i+1),
	window.h()-(BORDER_H+BUTTON_H), BUTTON_W, BUTTON_H, blabels[i]);
    }
    button->callback(set_button_number, i);
  }

  window.end();
  button_number = 0;
  window.exec();
  if (textfield)
    textfield->parent()->remove(textfield); // don't destroy it yet
  return button_number;
}

// pointers you can use to change fltk to a foreign language:
const char* fltk::no = "No";
const char* fltk::yes= "Yes";
const char* fltk::ok = "OK";
const char* fltk::cancel= "Cancel";

void fltk::message(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  innards("i", 0, 0, fmt, ap, ok, 0, 0);
  va_end(ap);
}

void fltk::alert(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  innards("!", 0, 0, fmt, ap, ok, 0, 0);
  va_end(ap);
}

int fltk::ask(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int r = innards("?", 0, 0, fmt, ap, no, yes, 0);
  va_end(ap);
  return r;
}

int fltk::choice(const char*fmt,const char *b0,const char *b1,const char *b2,...){
  va_list ap;
  va_start(ap, b2);
  int r = innards("?", 0, 0, fmt, ap, b2, b1, b0);
  va_end(ap);
  return 2-r;
}

static const char* input_innards(const char* fmt, va_list ap,
				 const char* defstr, uchar type) {
  int r = innards("?", defstr ? defstr : "", type,
		  fmt, ap, cancel, ok, 0);
  return r ? textfield->value() : 0;
}

const char* fltk::input(const char *fmt, const char *defstr, ...) {
  va_list ap;
  va_start(ap, defstr);
  const char* r = input_innards(fmt, ap, defstr, Input::NORMAL);
  va_end(ap);
  return r;
}

const char *fltk::password(const char *fmt, const char *defstr, ...) {
  va_list ap;
  va_start(ap, defstr);
  const char* r = input_innards(fmt, ap, defstr, Input::SECRET);
  va_end(ap);
  return r;
}

//
// End of "$Id: fl_ask.cxx,v 1.27 2002/12/10 02:00:56 easysw Exp $".
//
