//
// "$Id: fl_ask.cxx,v 1.23 2001/07/23 09:50:05 spitzak Exp $"
//
// Standard dialog functions for the Fast Light Tool Kit (FLTK).
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

// Implementation of fl_message, fl_ask, fl_choice, fl_input
// The three-message fl_show_x functions are for forms compatibility
// mostly.  In most cases it is easier to get a multi-line message
// by putting newlines in the message.

#include <fltk/Fl.h>
#include <fltk/fl_draw.h>
#include <fltk/fl_ask.h>
#include <fltk/Fl_Box.h>
#include <fltk/Fl_Button.h>
#include <fltk/Fl_Return_Button.h>
#include <fltk/Fl_Window.h>
#include <fltk/Fl_Input.h>
#include <fltk/Fl_Secret_Input.h>

#include <fltk/vsnprintf.h>
#include <string.h>

static void m_revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
}
static Fl_Named_Style m_style("Message", m_revert, &fl_message_style);
Fl_Named_Style* fl_message_style = &m_style;

static void i_revert(Fl_Style* s) {
  s->box = FL_THIN_UP_BOX;
  s->label_font = FL_TIMES_BOLD;
  s->label_size = 34;
  s->color = FL_WHITE;
  s->label_color = FL_BLUE;
}
static Fl_Named_Style i_style("Icon", i_revert, &fl_icon_style);
Fl_Named_Style* fl_icon_style = &i_style;

static int button_number;
static void set_button_number(Fl_Widget* w, long a) {
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

static Fl_Input *input;

static int innards(
  const char* iconlabel,
  const char *istr, int itype,
  const char* fmt, va_list ap,
  const char *b0,
  const char *b1,
  const char *b2)
{
  Fl_Window window(3*BORDER_W+ICON_W+INPUT_W, 3*BORDER_H+ICON_H+BUTTON_H);

  // This keeps the icon from resizing.
  Fl_Group ig(BORDER_W, BORDER_H, ICON_W, ICON_H);

  Fl_Box icon(0, 0, ICON_W, ICON_H);
  icon.style(fl_icon_style);
  icon.label(iconlabel);

  ig.end();

  Fl_Box message(2*BORDER_W+ICON_W, 0, INPUT_W, 2*BORDER_H+ICON_H);
  message.set_flag(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
  message.style(fl_message_style);

  if (input) {delete input; input = 0;}
  if (istr) {
    input = new Fl_Input(2*BORDER_W+ICON_W, 0, INPUT_W, 0);
    input->h(input->text_size()+10);
    input->y(BORDER_H+ICON_H-input->h());
    message.h(input->y());
    input->type(itype);
    input->value(istr);
    window.focus(input);
  }

  window.resizable(message);
//  w->size_range(window.w(), window.h(), 0, window.h());

  char buffer[1024];
  if (!strcmp(fmt,"%s")) {
    message.label(va_arg(ap, const char*));
  } else {
    vsnprintf(buffer, 1024, fmt, ap);
    message.label(buffer);
  }

  const char* blabels[3];
  blabels[0] = b0;
  blabels[1] = b1;
  blabels[2] = b2;
  int default_button = b1 ? 1 : 0;

  int i;
  for (i = 0; i <= 2; i++)
    if (blabels[i] && blabels[i][0] == '*') {blabels[i]++; default_button = i;}

  for (i = 3; i--;) if (blabels[i]) {
    Fl_Button* button;
    if (i == default_button) {
      button = new Fl_Return_Button(
	3*BORDER_W+ICON_W+INPUT_W-(BUTTON_W+BORDER_W)*(i+1),
	2*BORDER_H+ICON_H, BUTTON_W, BUTTON_H, blabels[i]);
      window.hotspot(button);
      if (!input) window.focus(button);
    } else {
      button = new Fl_Button(
	3*BORDER_W+ICON_W+INPUT_W-(BUTTON_W+BORDER_W)*(i+1),
	2*BORDER_H+ICON_H, BUTTON_W, BUTTON_H, blabels[i]);
    }
    button->callback(set_button_number, i);
  }

  window.end();

  button_number = 0;
  window.exec();
  if (input) input->parent()->remove(input); // don't destroy it yet
  return button_number;
}

// pointers you can use to change fltk to a foreign language:
const char* fl_no = "No";
const char* fl_yes= "Yes";
const char* fl_ok = "OK";
const char* fl_cancel= "Cancel";

// fltk functions:

void fl_message(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  innards("i", 0, 0, fmt, ap, fl_ok, 0, 0);
  va_end(ap);
}

void fl_alert(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  innards("!", 0, 0, fmt, ap, fl_ok, 0, 0);
  va_end(ap);
}

int fl_ask(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int r = innards("?", 0, 0, fmt, ap, fl_no, fl_yes, 0);
  va_end(ap);
  return r;
}

int fl_choice(const char*fmt,const char *b0,const char *b1,const char *b2,...){
  va_list ap;
  va_start(ap, b2);
  int r = innards("?", 0, 0, fmt, ap, b0, b1, b2);
  va_end(ap);
  return r;
}

static const char* input_innards(const char* fmt, va_list ap,
				 const char* defstr, uchar type) {
  int r = innards("?", defstr ? defstr : "", type,
		  fmt, ap, fl_cancel, fl_ok, 0);
  return r ? input->value() : 0;
}

const char* fl_input(const char *fmt, const char *defstr, ...) {
  va_list ap;
  va_start(ap, defstr);
  const char* r = input_innards(fmt, ap, defstr, FL_NORMAL_INPUT);
  va_end(ap);
  return r;
}

const char *fl_password(const char *fmt, const char *defstr, ...) {
  va_list ap;
  va_start(ap, defstr);
  const char* r = input_innards(fmt, ap, defstr, FL_SECRET_INPUT);
  va_end(ap);
  return r;
}

//
// End of "$Id: fl_ask.cxx,v 1.23 2001/07/23 09:50:05 spitzak Exp $".
//
