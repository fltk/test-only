//
// "$Id: fl_ask.cxx,v 1.22 2001/02/23 05:14:18 clip Exp $"
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <config.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include <FL/fl_ask.H>

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
static Fl_Window *window;
static Fl_Box *message;
static Fl_Widget *icon;
static Fl_Button *button[3];
static Fl_Input *input;
static const char *iconlabel = "?";

static void m_revert(Fl_Style* s) {
  s->box = FL_NO_BOX;
}

Fl_Named_Style* fl_message_style =
  new Fl_Named_Style("Message", m_revert, &fl_message_style);

static void i_revert(Fl_Style* s) {
  s->box = FL_THIN_UP_BOX;
  s->label_font = FL_TIMES_BOLD;
  s->label_size = 34;
  s->color = FL_WHITE;
  s->label_color = FL_BLUE;
}

static Fl_Named_Style* icon_style =
  new Fl_Named_Style("Message Icon", i_revert, &icon_style);

static int button_number;
static void set_button_number(Fl_Widget* w, long a) {
  button_number = a;
  w->window()->hide();
}

static Fl_Widget* user_icon = 0;
void fl_message_icon(Fl_Widget *w) { user_icon = w; }

static Fl_Window *makeform(const char *istr, int itype, int default_button) {
  if (user_icon) {
    icon = user_icon;
    icon->position(10, 10);
  } else {
    icon = new Fl_Box(10, 10, 50, 50);
    icon_style->parent = icon->style();
    icon->copy_style(icon_style);
    icon->label(iconlabel);
  }
  if (window) delete window;
  Fl_Window *w = window = new Fl_Window(10+icon->w()+10+340+10,10+icon->h()+10+25+10);
  // This keeps the icon from resizing.
  Fl_Group *ig = new Fl_Group(icon->x(), icon->y(), icon->w(), icon->h());
  icon->position(0, 0);
  ig->add(icon);
  ig->end();
  message = new Fl_Box(10+icon->w()+10, 0, 340, 10+icon->h()+10);
  message->set_flag(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
  fl_message_style->parent = message->style();
  message->copy_style(fl_message_style);
  if (istr) {
    input = new Fl_Input(message->x(), 0, message->w(), 0);
    input->h(fl_height(input->text_font(), input->text_size())+10);
    input->y(10+icon->h()-input->h());
    input->type(itype);
    input->value(istr);
    message->h(input->y());
  }

  for (int i = 0; i <= 2; i++) {
    if (i == default_button)
      button[i] = new Fl_Return_Button(w->w()-100*(i+1), 10+icon->h()+10, 90, 25);
    else
      button[i] = new Fl_Button(w->w()-100*(i+1), 10+icon->h()+10, 90, 25);
    button[i]->callback(set_button_number, i);
  }

  w->resizable(message);
//  w->size_range(w->w(), w->h(), 0, w->h());
  w->end();
  w->set_modal();
  return w;
}

#if !HAVE_VSNPRINTF || defined(__hpux)
extern "C" {
int vsnprintf(char* str, size_t size, const char* fmt, va_list ap);
}
#endif

static int innards(const char *istr, int itype, const char* fmt, va_list ap,
  const char *b0,
  const char *b1,
  const char *b2)
{
  int default_button = b1 ? 1 : 0;
  if (b0 && *b0 == '*') { b0++; default_button = 0; }
  if (b1 && *b1 == '*') { b1++; default_button = 1; }
  if (b2 && *b2 == '*') { b2++; default_button = 2; }
  makeform(istr, itype, default_button);
  char buffer[1024];
  if (!strcmp(fmt,"%s")) {
    message->label(va_arg(ap, const char*));
  } else {
    vsnprintf(buffer, 1024, fmt, ap);
    message->label(buffer);
  }
  if (b0) button[0]->label(b0);
  else button[0]->hide();
  if (b1) button[1]->label(b1);
  else button[1]->hide();
  if (b2) button[2]->label(b2);
  else button[2]->hide();
  button_number = 0;
  window->focus(istr ? (Fl_Widget *)input : (Fl_Widget *)button[default_button]);
  window->hotspot(button[default_button]);
  window->exec();
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
  iconlabel = "i";
  innards(0, 0, fmt, ap, fl_ok, 0, 0);
  va_end(ap);
}

void fl_alert(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  iconlabel = "!";
  innards(0, 0, fmt, ap, fl_ok, 0, 0);
  va_end(ap);
}

int fl_ask(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  iconlabel = "?";
  int r = innards(0, 0, fmt, ap, fl_no, fl_yes, 0);
  va_end(ap);
  return r;
}

int fl_choice(const char*fmt,const char *b0,const char *b1,const char *b2,...){
  va_list ap;
  va_start(ap, b2);
  iconlabel = "?";
  int r = innards(0, 0, fmt, ap, b0, b1, b2);
  va_end(ap);
  return r;
}

static const char* input_innards(const char* fmt, va_list ap,
				 const char* defstr, uchar type) {
  int r = innards(defstr, type, fmt, ap, fl_cancel, fl_ok, 0);
  return r ? input->value() : 0;
}

const char* fl_input(const char *fmt, const char *defstr, ...) {
  va_list ap;
  va_start(ap, defstr);
  iconlabel = "?";
  const char* r = input_innards(fmt, ap, defstr, FL_NORMAL_INPUT);
  va_end(ap);
  return r;
}

const char *fl_password(const char *fmt, const char *defstr, ...) {
  va_list ap;
  va_start(ap, defstr);
  iconlabel = "?";
  const char* r = input_innards(fmt, ap, defstr ? defstr : "", FL_SECRET_INPUT);
  va_end(ap);
  return r;
}

//
// End of "$Id: fl_ask.cxx,v 1.22 2001/02/23 05:14:18 clip Exp $".
//
