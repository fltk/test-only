//
// "$Id$"
//
// Standard dialog functions for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2009 by Bill Spitzak and others.
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

// Implementation of fltk3::message, fltk3::ask, fltk3::choice, fltk3::input
// The three-message fl_show_x functions are for forms compatibility
// mostly.  In most cases it is easier to get a multi-line message
// by putting newlines in the message.

#include <stdio.h>
#include <stdarg.h>
#include "flstring.h"

#include <fltk3/run.h>

#include <fltk3/ask.h>

#include <fltk3/Fl_Box.H>
#include <fltk3/Button.h>
#include <fltk3/ReturnButton.h>
#include <fltk3/Window.h>
#include <fltk3/Input.h>
#include <fltk3/Fl_Secret_Input.H>
#include <fltk3/x.H>
#include <fltk3/draw.h>

#ifdef __APPLE__
# if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
#  include <AudioToolbox/AudioServices.h>
# endif
#endif

static fltk3::Window *message_form;
static Fl_Box *message;
static Fl_Box *icon;
static fltk3::Button *button[3];
static fltk3::Input *input;
static int ret_val;
static const char *iconlabel = "?";
Fl_Font fltk3::message_font_ = fltk3::HELVETICA;
Fl_Fontsize fltk3::message_size_ = 14;

static char avoidRecursion = 0;

// sets the global return value (ret_val) and closes the window
static void button_cb(fltk3::Widget *bt, void *val) {
  ret_val = (int)(long)val;
  message_form->hide();
}

static fltk3::Window *makeform() {
 if (message_form) {
   message_form->size(410,103);
   return message_form;
 }
 // make sure that the dialog does not become the child of some 
 // current group
  fltk3::Group *previously_current_group = fltk3::Group::current();
  fltk3::Group::current(0);
 // create a new top level window
  fltk3::Window *w = message_form = new fltk3::Window(410,103,"");
 // w->clear_border();
 // w->box(fltk3::UP_BOX);
 (message = new Fl_Box(60, 25, 340, 20))
   ->align(fltk3::ALIGN_LEFT|fltk3::ALIGN_INSIDE|fltk3::ALIGN_WRAP);
 (input = new fltk3::Input(60, 37, 340, 23))->hide();
 {Fl_Box* o = icon = new Fl_Box(10, 10, 50, 50);
  o->box(fltk3::THIN_UP_BOX);
  o->labelfont(fltk3::TIMES_BOLD);
  o->labelsize(34);
  o->color(FL_WHITE);
  o->labelcolor(FL_BLUE);
 }
 // create the buttons (right to left)
 for (int b=0, x=310; b<3; b++, x -= 100) {
   button[b] = new fltk3::Button(x, 70, 90, 23);
   button[b]->align(fltk3::ALIGN_INSIDE|fltk3::ALIGN_WRAP);
   button[b]->callback(button_cb,(void *)b);
 }
 button[0]->shortcut(FL_Escape);
 w->resizable(new Fl_Box(60,10,110-60,27));
 w->end();
 w->set_modal();
  fltk3::Group::current(previously_current_group);
 return w;
}

/*
 * 'resizeform()' - Resize the form and widgets so that they hold everything
 *                  that is asked of them...
 */

void resizeform() {
  int	i;
  int	message_w, message_h;
  int	text_height;
  int	button_w[3], button_h[3];
  int	x, w, h, max_w, max_h;
	const int icon_size = 50;

  fl_font(fltk3::message_font_, fltk3::message_size_);
  message_w = message_h = 0;
  fl_measure(message->label(), message_w, message_h);

  message_w += 10;
  message_h += 10;
  if (message_w < 340)
    message_w = 340;
  if (message_h < 30)
    message_h = 30;

  fl_font(button[0]->labelfont(), button[0]->labelsize());

  memset(button_w, 0, sizeof(button_w));
  memset(button_h, 0, sizeof(button_h));

  for (max_h = 25, i = 0; i < 3; i ++)
    if (button[i]->visible())
    {
      fl_measure(button[i]->label(), button_w[i], button_h[i]);

      if (i == 1)
        button_w[1] += 20;

      button_w[i] += 30;
      button_h[i] += 10;

      if (button_h[i] > max_h)
        max_h = button_h[i];
    }

  if (input->visible()) text_height = message_h + 25;
  else text_height = message_h;

  max_w = message_w + 10 + icon_size;
  w     = button_w[0] + button_w[1] + button_w[2] - 10;

  if (w > max_w)
    max_w = w;

  message_w = max_w - 10 - icon_size;

  w = max_w + 20;
  h = max_h + 30 + text_height;

  message_form->size(w, h);
  message_form->size_range(w, h, w, h);

  message->resize(20 + icon_size, 10, message_w, message_h);
  icon->resize(10, 10, icon_size, icon_size);
  icon->labelsize(icon_size - 10);
  input->resize(20 + icon_size, 10 + message_h, message_w, 25);

  for (x = w, i = 0; i < 3; i ++)
    if (button_w[i])
    {
      x -= button_w[i];
      button[i]->resize(x, h - 10 - max_h, button_w[i] - 10, max_h);

//      printf("button %d (%s) is %dx%d+%d,%d\n", i, button[i]->label(),
//             button[i]->w(), button[i]->h(),
//	     button[i]->x(), button[i]->y());
    }
}

static int innards(const char* fmt, va_list ap,
  const char *b0,
  const char *b1,
  const char *b2)
{
  fltk3::pushed(0); // stop dragging (STR #2159)

  avoidRecursion = 1;

  makeform();
  char buffer[1024];
  if (!strcmp(fmt,"%s")) {
    message->label(va_arg(ap, const char*));
  } else {
    ::vsnprintf(buffer, 1024, fmt, ap);
    message->label(buffer);
  }

  message->labelfont(fltk3::message_font_);
  message->labelsize(fltk3::message_size_);
  if (b0) {button[0]->show(); button[0]->label(b0); button[1]->position(210,70);}
  else {button[0]->hide(); button[1]->position(310,70);}
  if (b1) {button[1]->show(); button[1]->label(b1);}
  else button[1]->hide();
  if (b2) {button[2]->show(); button[2]->label(b2);}
  else button[2]->hide();
  const char* prev_icon_label = icon->label();
  if (!prev_icon_label) icon->label(iconlabel);

  resizeform();

  if (button[1]->visible() && !input->visible()) 
    button[1]->take_focus();
  message_form->hotspot(button[0]);
  if (b0 && fltk3::Widget::label_shortcut(b0))
    button[0]->shortcut(0);
  else
    button[0]->shortcut(FL_Escape);

  // deactivate fltk3::grab(), because it is incompatible with modal windows
  fltk3::Window* g = fltk3::grab();
  if (g) fltk3::grab(0);
  message_form->show();
  while (message_form->shown()) fltk3::wait();
  if (g) // regrab the previous popup menu, if there was one
    fltk3::grab(g);
  icon->label(prev_icon_label);

  avoidRecursion = 0;
  return ret_val;
}

 /** \addtogroup group_comdlg
    @{ */

// pointers you can use to change FLTK to a foreign language:
const char* fltk3::text_no = "No";        ///< string pointer used in common dialogs, you can change it to a foreign language
const char* fltk3::text_yes= "Yes";       ///< string pointer used in common dialogs, you can change it to a foreign language
const char* fltk3::text_ok = "OK";        ///< string pointer used in common dialogs, you can change it to a foreign language
const char* fltk3::text_cancel= "Cancel"; ///< string pointer used in common dialogs, you can change it to a foreign language
const char* fltk3::text_close= "Close";   ///< string pointer used in common dialogs, you can change it to a foreign language

// fltk functions:

/**
   Emits a system beep message.
 */
void fltk3::beep(int type) {
#ifdef WIN32
  switch (type) {
    case fltk3::BEEP_QUESTION :
    case fltk3::BEEP_PASSWORD :
      MessageBeep(MB_ICONQUESTION);
      break;
    case fltk3::BEEP_MESSAGE :
      MessageBeep(MB_ICONASTERISK);
      break;
    case fltk3::BEEP_NOTIFICATION :
      MessageBeep(MB_ICONASTERISK);
      break;
    case fltk3::BEEP_ERROR :
      MessageBeep(MB_ICONERROR);
      break;
    default :
      MessageBeep(0xFFFFFFFF);
      break;
  }
#elif defined(__APPLE__)
  switch (type) {
    case fltk3::BEEP_DEFAULT :
    case fltk3::BEEP_ERROR :
      // How Apple is not any better than Microsoft:
      /* MacOS 8 */   // SysBeep(30);
      /* OS X 10.1 */ // AlertSoundPlay();
      /* OS X 10.5 */ // AudioServicesPlayAlertSound(kUserPreferredAlert);
      /* OS X 10.6 */ // AudioServicesPlayAlertSound(kSystemSoundID_UserPreferredAlert);
# if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
      if (AudioServicesPlayAlertSound!=0L)
#   if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
        AudioServicesPlayAlertSound(kSystemSoundID_UserPreferredAlert);
#   else
        AudioServicesPlayAlertSound(kUserPreferredAlert);
#   endif
      else
# endif
#if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
        AlertSoundPlay();
#else
    {
    }
#endif
      break;
    default :
      break;
  }
#else
  switch (type) {
    case fltk3::BEEP_DEFAULT :
    case fltk3::BEEP_ERROR :
      if (!fl_display) fl_open_display();

      XBell(fl_display, 100);
      break;
    default :
      if (!fl_display) fl_open_display();

      XBell(fl_display, 50);
      break;
  }
#endif // WIN32
}
/** Shows an information message dialog box.

   \note Common dialog boxes are application modal. No more than one common dialog box
   can be open at any time. Requests for additional dialog boxes are ignored.

   \param[in] fmt can be used as an sprintf-like format and variables for the message text
 */
void fltk3::message(const char *fmt, ...) {

  if (avoidRecursion) return;

  va_list ap;

  fltk3::beep(fltk3::BEEP_MESSAGE);

  va_start(ap, fmt);
  iconlabel = "i";
  innards(fmt, ap, 0, fltk3::text_close, 0);
  va_end(ap);
  iconlabel = "?";
}

/** Shows an alert message dialog box

   \note Common dialog boxes are application modal. No more than one common dialog box
   can be open at any time. Requests for additional dialog boxes are ignored.

   \param[in] fmt can be used as an sprintf-like format and variables for the message text
 */
void fltk3::alert(const char *fmt, ...) {

  if (avoidRecursion) return;

  va_list ap;

  fltk3::beep(fltk3::BEEP_ERROR);

  va_start(ap, fmt);
  iconlabel = "!";
  innards(fmt, ap, 0, fltk3::text_close, 0);
  va_end(ap);
  iconlabel = "?";
}
/** Shows a dialog displaying the \p fmt message,
    this dialog features 2 yes/no buttons

   \note Common dialog boxes are application modal. No more than one common dialog box
   can be open at any time. Requests for additional dialog boxes are ignored.

   \param[in] fmt can be used as an sprintf-like format and variables for the message text
   \retval 0 if the no button is selected or another dialog box is still open
   \retval 1 if yes is selected
 */
int fltk3::ask(const char *fmt, ...) {

  if (avoidRecursion) return 0;

  va_list ap;

  fltk3::beep(fltk3::BEEP_QUESTION);

  va_start(ap, fmt);
  int r = innards(fmt, ap, fltk3::text_no, fltk3::text_yes, 0);
  va_end(ap);

  return r;
}

/** Shows a dialog displaying the \p fmt message,
    this dialog features up to 3 customizable choice buttons

   \note Common dialog boxes are application modal. No more than one common dialog box
   can be open at any time. Requests for additional dialog boxes are ignored.

   \param[in] fmt can be used as an sprintf-like format and variables for the message text
   \param[in] b0 text label of button 0
   \param[in] b1 text label of button 1
   \param[in] b2 text label of button 2
   \retval 0 if the first button with \p b0 text is selected or another dialog box is still open
   \retval 1 if the second button with \p b1 text is selected
   \retval 2 if the third button with \p b2 text is selected
 */
int fltk3::choice(const char*fmt,const char *b0,const char *b1,const char *b2,...){

  if (avoidRecursion) return 0;

  va_list ap;

  fltk3::beep(fltk3::BEEP_QUESTION);

  va_start(ap, b2);
  int r = innards(fmt, ap, b0, b1, b2);
  va_end(ap);
  return r;
}
/** Gets the Fl_Box icon container of the current default dialog used in 
    many common dialogs like fltk3::message(), fltk3::alert(), 
    fltk3::ask(), fltk3::choice(), fltk3::input(), fltk3::password() 
*/ 
fltk3::Widget *fltk3::message_icon() {makeform(); return icon;}

static const char* input_innards(const char* fmt, va_list ap,
				 const char* defstr, uchar type) {
  makeform();
  message->position(60,10);
  input->type(type);
  input->show();
  input->value(defstr);
  input->take_focus();

  int r = innards(fmt, ap, fltk3::text_cancel, fltk3::text_ok, 0);
  input->hide();
  message->position(60,25);
  return r ? input->value() : 0;
}

/** Shows an input dialog displaying the \p fmt message

   \note Common dialog boxes are application modal. No more than one common dialog box
   can be open at any time. Requests for additional dialog boxes are ignored.

   \param[in] fmt can be used as an sprintf-like format and variables for the message text
   \param[in] defstr defines the default returned string if no text is entered
   \return the user string input if OK was pushed, NULL if Cancel was pushed or another dialog box was still open
 */
const char* fltk3::input(const char *fmt, const char *defstr, ...) {

  if (avoidRecursion) return 0;

  fltk3::beep(fltk3::BEEP_QUESTION);

  va_list ap;
  va_start(ap, defstr);
  const char* r = input_innards(fmt, ap, defstr, FL_NORMAL_INPUT);
  va_end(ap);
  return r;
}

/** Shows an input dialog displaying the \p fmt message.

    Like fltk3::input() except the input text is not shown,
    '*' characters are displayed instead.

   \note Common dialog boxes are application modal. No more than one common dialog box
   can be open at any time. Requests for additional dialog boxes are ignored.

   \param[in] fmt can be used as an sprintf-like format and variables for the message text
   \param[in] defstr defines the default returned string if no text is entered
   \return the user string input if OK was pushed, NULL if Cancel was pushed or aother dialog box was still open
 */
const char *fltk3::password(const char *fmt, const char *defstr, ...) {

  if (avoidRecursion) return 0;

  fltk3::beep(fltk3::BEEP_PASSWORD);

  va_list ap;
  va_start(ap, defstr);
  const char* r = input_innards(fmt, ap, defstr, FL_SECRET_INPUT);
  va_end(ap);
  return r;
}

/** @} */

//
// End of "$Id$".
//
