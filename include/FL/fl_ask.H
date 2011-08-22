//
// "$Id$"
//
// Standard dialog header file for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper started
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

#ifndef fl_ask_H
#define fl_ask_H

#include <fltk3/ask.h>
#include <stdarg.h>
#include "Enumerations.H"

class Fl_Widget;

enum {
  FL_BEEP_DEFAULT = 0,
  FL_BEEP_MESSAGE,
  FL_BEEP_ERROR,
  FL_BEEP_QUESTION,
  FL_BEEP_PASSWORD,
  FL_BEEP_NOTIFICATION
};

inline void fl_beep(int type = FL_BEEP_DEFAULT) {
  fltk3::beep(type);
}

inline void fl_message(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  fltk3::vmessage(fmt, ap);
  va_end(ap);
}

inline void fl_alert(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  fltk3::alert(fmt, ap);
  va_end(ap);
}

inline int fl_ask(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int ret = fltk3::vask(fmt, ap);
  va_end(ap);
  return ret;
}

inline int fl_choice(const char *q,const char *b0,const char *b1,const char *b2, ...) {
  va_list ap;
  va_start(ap, b2);
  int ret = fltk3::vchoice(q, b0, b1, b2, ap);
  va_end(ap);
  return ret;
}

inline const char *fl_input(const char *label, const char *deflt = 0, ...) {
  va_list ap;
  va_start(ap, deflt);
  const char *ret = fltk3::vinput(label, deflt, ap);
  va_end(ap);
  return ret;
}

inline const char *fl_password(const char *label, const char *deflt = 0, ...) {
  va_list ap;
  va_start(ap, deflt);
  const char *ret = fltk3::vpassword(label, deflt, ap);
  va_end(ap);
  return ret;
}

#if 0 // FIXME: 123
inline Fl_Widget *fl_message_icon();
extern inline Fl_Font fl_message_font_;
extern inline Fl_Fontsize fl_message_size_;
inline void fl_message_font(Fl_Font f, Fl_Fontsize s) {
  fl_message_font_ = f; fl_message_size_ = s;}
#endif

inline void fl_message_hotspot(int enable) {
  fltk3::message_hotspot(enable);
}

inline int fl_message_hotspot(void) {
  return fltk3::message_hotspot();
}

inline void fl_message_title(const char *title) {
  fltk3::message_title(title);
}

inline void fl_message_title_default(const char *title) {
  fltk3::message_title_default(title);
}

#if 0 // FIXME: 123
// pointers you can use to change FLTK to a foreign language:
extern inline const char* fl_no;
extern inline const char* fl_yes;
extern inline const char* fl_ok;
extern inline const char* fl_cancel;
extern inline const char* fl_close;
#endif

#endif // !fl_ask_H

//
// End of "$Id$".
//
