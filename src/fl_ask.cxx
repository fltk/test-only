//
// "$Id: fl_ask.cxx,v 1.19 2000/08/10 09:24:32 spitzak Exp $"
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

#include <FL/fl_ask.H>

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
static Fl_Window *window;
static Fl_Box *message;
static Fl_Box *icon;
static Fl_Button *button[3];
static Fl_Input *input;
static char *iconlabel = "?";

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

static Fl_Window *makeform() {
  if (window) {
    window->size(410,105);
    return window;
  }
  Fl_Window *w = window = new Fl_Window(410,105);
  message = new Fl_Box(60, 0, 340, 70);
  message->set_flag(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
  fl_message_style->parent = message->style();
  message->copy_style(fl_message_style);
  input = new Fl_Input(60,32,340,30);
  input->hide();
  icon = new Fl_Box(10, 10, 50, 50);
  icon_style->parent = icon->style();
  icon->copy_style(icon_style);
  button[2] = new Fl_Button(110, 70, 90, 25);
  button[1] = new Fl_Return_Button(210, 70, 90, 25);
  (button[0] = new Fl_Button(310, 70, 90, 25))->shortcut(FL_Escape);
  w->resizable(new Fl_Box(60,10,110-60,22));
  w->end();
  w->set_modal();
  return w;
}

#if !HAVE_VSNPRINTF || defined(__hpux)
extern "C" {
int vsnprintf(char* str, size_t size, const char* fmt, va_list ap);
}
#endif

static int innards(const char* fmt, va_list ap,
  const char *b0,
  const char *b1,
  const char *b2)
{
  makeform();
  char buffer[1024];
  if (!strcmp(fmt,"%s")) {
    message->label(va_arg(ap, const char*));
  } else {
    vsnprintf(buffer, 1024, fmt, ap);
    message->label(buffer);
  }
  if (b0) {button[0]->show();button[0]->label(b0);button[1]->position(210,70);}
  else {button[0]->hide(); button[1]->position(310,70);}
  if (b1) {button[1]->show(); button[1]->label(b1);}
  else button[1]->hide();
  if (b2) {button[2]->show(); button[2]->label(b2);}
  else button[2]->hide();
  const char* prev_icon_label = icon->label();
  if (!prev_icon_label) icon->label(iconlabel);
  input->h(input->text_size()*2);
  window->focus(input->visible() ? (Fl_Widget*)input : (Fl_Widget*)button[1]);
  window->hotspot(button[0]);
  window->show();
  int r;
  for (;;) {
    Fl_Widget *o = Fl::readqueue();
    if (!o) Fl::wait();
    else if (o == button[0]) {r = 0; break;}
    else if (o == button[1]) {r = 1; break;}
    else if (o == button[2]) {r = 2; break;}
    else if (o == window) {r = 0; break;}
  }
  window->hide();
  icon->label(prev_icon_label);
  return r;
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
  innards(fmt, ap, 0, fl_ok, 0);
  va_end(ap);
  iconlabel = "?";
}

void fl_alert(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  iconlabel = "!";
  innards(fmt, ap, 0, fl_ok, 0);
  va_end(ap);
  iconlabel = "?";
}

int fl_ask(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int r = innards(fmt, ap, fl_no, fl_yes, 0);
  va_end(ap);
  return r;
}

int fl_choice(const char*fmt,const char *b0,const char *b1,const char *b2,...){
  va_list ap;
  va_start(ap, b2);
  int r = innards(fmt, ap, b0, b1, b2);
  va_end(ap);
  return r;
}

Fl_Widget *fl_message_icon() {makeform(); return icon;}

static const char* input_innards(const char* fmt, va_list ap,
				 const char* defstr, uchar type) {
  makeform();
  message->size(340, 40);
  input->type(type);
  input->show();
  input->value(defstr);
  int r = innards(fmt, ap, fl_cancel, fl_ok, 0);
  input->hide();
  message->size(340, 70);
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
// End of "$Id: fl_ask.cxx,v 1.19 2000/08/10 09:24:32 spitzak Exp $".
//
