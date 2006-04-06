//
// "$Id$"
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

#include <fltk/draw.h>
#include <fltk/ask.h>
#include <fltk/Button.h>
#include <fltk/ReturnButton.h>
#include <fltk/Window.h>
#include <fltk/Input.h>
#include <fltk/SecretInput.h>
#include <fltk/string.h>

#ifdef WIN32
# include <windows.h>
#endif
#include <fltk/x.h>

using namespace fltk;

/*! \addtogroup dialogs

  FLTK provides a number of functions that create a popup window and
  Window::exec() it. These functions return when the user OK's or
  Cancels the window, returning the value the user chose.

  Currently these are all built using normal fltk widgets. In theory
  these should be services provided by the operating system or desktop
  environment, but so far no non-ugly scheme has been developed for
  that. Unfortunatly the space for these dialogs can often end up being
  the majority of an FLTK program's size.

*/

static void m_revert(Style* s) {
  s->box_ = NO_BOX;
}
static NamedStyle m_style("Message", m_revert, &message_style);
/*! This Style is used for the label area for all the popup windows.
  You can change the textfont() or textsize() to make them print
  differently.
*/
NamedStyle* fltk::message_style = &m_style;

static void i_revert(Style* s) {
  s->box_ = THIN_UP_BOX;
  s->labelfont_ = TIMES_BOLD;
  s->labelsize_ = 34;
  s->color_ = WHITE;
  s->textcolor_ = s->labelcolor_ = BLUE;
}
static NamedStyle i_style("Icon", i_revert, &fltk::icon_style);
/*! This Style is used for the 50x50 icon area on the left of all
  the popup windows. You can change the colors or font used here.
*/
NamedStyle* fltk::icon_style = &i_style;

static int button_number;
static void set_button_number(Widget* w, long a) {
  button_number = a;
  w->window()->make_exec_return(false);
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
  Group* saved_current_group = Group::current();
  load_theme();
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

  if (istr) {
    if (textfield)
      window.add(textfield);
    else
      textfield = new Input(2*BORDER_W+ICON_W, 0, INPUT_W, 0);
    textfield->h(int(textfield->textsize())+10);
    textfield->y(BORDER_H+ICON_H-textfield->h());
    textfield->w(INPUT_W);
    message.h(textfield->y());
    textfield->type(itype);
    textfield->text(istr);
    window.set_focus(textfield);
  }

  window.resizable(message);
  window.resize_align(ALIGN_RIGHT);
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
  int w = 800;
  int h = 0;
  measure(message.label(), w, h, message.flags());
  w+=6; h+=6;
  w -= message.w(); if (w < 0) w = 0;
  h -= message.h(); if (h < 0) h = 0;
  window.resize(window.x(),window.y(),window.w()+w, window.h()+h);
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
      if (!istr) window.set_focus(button);
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
  if (istr)
    window.remove(textfield); // don't destroy it yet
  Group::current(saved_current_group);
  return button_number;
}

/*! You can change this string to convert fltk to a foreign language. */
const char* fltk::no = "&No";
/*! You can change this string to convert fltk to a foreign language. */
const char* fltk::yes= "&Yes";
/*! You can change this string to convert fltk to a foreign language. */
const char* fltk::ok = "&OK";
/*! You can change this string to convert fltk to a foreign language. */
const char* fltk::cancel= "&Cancel";

// beep on dialogs state
static bool sBeepOnDlg = false;

/*! You get the state enable beep on default message dialog 
    (like ask, choice, input, ...) 
    by  using this function with true (default is false) */
bool fltk::beep_on_dlg() {return sBeepOnDlg;}

/*! You can enable beep on default message dialog 
    (like ask, choice, input, ...) 
    by  using this function with true (default is false) */
void fltk::beep_on_dlg(bool b) {sBeepOnDlg=b;}

/*!
  \image html fl_message.gif
  Displays a printf-style message in a pop-up box with an "OK" button,
  waits for the user to hit the button. The message will wrap to fit
  the window, or may be many lines by putting '\\n' characters into
  it. The enter key is a shortcut for the OK button.
*/
void fltk::message(const char *fmt, ...) {
  if (fltk::beep_on_dlg()) (fltk::beep(BEEP_MESSAGE));
  va_list ap;
  va_start(ap, fmt);
  innards("i", 0, 0, fmt, ap, ok, 0, 0);
  va_end(ap);

}

/*!
  \image html fl_alert.gif
  Same as fltk::message() except for the "!" symbol. 
*/
void fltk::alert(const char *fmt, ...) {
    if (fltk::beep_on_dlg()) (fltk::beep(fltk::BEEP_ERROR));
  va_list ap;
  va_start(ap, fmt);
  innards("!", 0, 0, fmt, ap, ok, 0, 0);
  va_end(ap);
}

/*!
  \image html fl_ask.gif
  Displays a printf-style message in a pop-up box with an "Yes" and
  "No" button and waits for the user to hit a button. The return value
  is 1 if the user hits Yes, 0 if they pick No. The enter key is a
  shortcut for Yes and ESC is a shortcut for No.
*/
int fltk::ask(const char *fmt, ...) {
  if (fltk::beep_on_dlg()) (fltk::beep(BEEP_QUESTION));
  va_list ap;
  va_start(ap, fmt);
  int r = innards("?", 0, 0, fmt, ap, no, yes, 0);
  va_end(ap);
  return r;
}

/*!
  \image html fl_choice.gif
  Shows the message with three buttons below it marked with the
  strings b0, b1, and b2. Returns 0, 1, or 2 depending on which button
  is hit. If one of the strings begins with the special character '*'
  then the associated button will be the default which is selected
  when the enter key is pressed. ESC is a shortcut for b2.
*/
int fltk::choice(const char*fmt,const char *b0,const char *b1,const char *b2,...){
    if (fltk::beep_on_dlg()) (fltk::beep(fltk::BEEP_QUESTION));
  va_list ap;
  va_start(ap, b2);
  int r = innards("?", 0, 0, fmt, ap, b2, b1, b0);
  va_end(ap);
  return 2-r;
}

/*! Same as choice() except a "!" icon is used instead of a "?" */
int fltk::choice_alert(const char*fmt,const char *b0,const char *b1,const char *b2,...){
  if (fltk::beep_on_dlg()) (fltk::beep(fltk::BEEP_QUESTION));
  va_list ap;
  va_start(ap, b2);
  int r = innards("!", 0, 0, fmt, ap, b2, b1, b0);
  va_end(ap);
  return 2-r;
}

static const char* input_innards(const char* fmt, va_list ap,
				 const char* defstr, uchar type) {
  int r = innards("?", defstr ? defstr : "", type,
		  fmt, ap, cancel, ok, 0);
  return r ? textfield->text() : 0;
}

/*!
  \image html fl_input.gif
  Pops up a window displaying a string, lets the user edit it, and
  return the new value. The cancel button returns NULL. The returned
  pointer is only valid until the next time fltk::input() is
  called. Due to back-compatability, the arguments to any printf
  commands in the label are after the default value.
*/
const char* fltk::input(const char *fmt, const char *defstr, ...) {
  if (fltk::beep_on_dlg()) (fltk::beep(fltk::BEEP_QUESTION));
  va_list ap;
  va_start(ap, defstr);
  const char* r = input_innards(fmt, ap, defstr, Input::NORMAL);
  va_end(ap);
  return r;
}

/*!
  \image html fl_password.gif
  Same as fltk::input() except an fltk::SecretInput field is used. 
*/
const char *fltk::password(const char *fmt, const char *defstr, ...) {
  if (fltk::beep_on_dlg()) (fltk::beep(fltk::BEEP_PASSWORD));
  va_list ap;
  va_start(ap, defstr);
  const char* r = input_innards(fmt, ap, defstr, Input::SECRET);
  va_end(ap);
  return r;
}

/*!
  Generates a simple beep message 
*/
void fltk::beep(int type) {
#ifdef WIN32
  switch (type) {
    case BEEP_QUESTION :
    case BEEP_PASSWORD :
      MessageBeep(MB_ICONQUESTION);
      break;
    case BEEP_MESSAGE :
      MessageBeep(MB_ICONASTERISK);
      break;
    case BEEP_NOTIFICATION :
      MessageBeep(MB_ICONASTERISK);
      break;
    case BEEP_ERROR :
      MessageBeep(MB_ICONERROR);
      break;
    default :
      MessageBeep(0xFFFFFFFF);
      break;
  }
#elif defined(__APPLE__)
  switch (type) {
    case BEEP_DEFAULT :
    case BEEP_ERROR :
      SysBeep(30);
      break;
    default :
      break;
  }
#else
  switch (type) {
    case BEEP_DEFAULT :
    case BEEP_ERROR :
      if (!fltk::xdisplay) fltk::open_display();

      XBell(fltk::xdisplay, 100);
      break;
    default :
      if (!fltk::xdisplay) fltk::open_display();

      XBell(fltk::xdisplay, 50);
      break;
  }
#endif // WIN32
}

//
// End of "$Id$".
//
