//
// "$Id: ask.h 7365 2010-03-30 15:18:29Z matt $"
//
// Standard dialog header file for the Fast Light Tool Kit (FLTK).
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

#ifndef fltk3_ask_H
#  define fltk3_ask_H

#  include "Enumerations.H"

#  ifdef __GNUC__
#    define __fl_attr(x) __attribute__ (x)
#    if __GNUC__ < 3
#      define __deprecated__
#    endif // __GNUC__ < 3
#  else
#    define __fl_attr(x)
#  endif // __GNUC__

namespace fltk3 {

  class Widget;
  
  /** Different system beeps available. \relatesalso fltk3::beep(int) */
  enum {
    BEEP_DEFAULT = 0,
    BEEP_MESSAGE,
    BEEP_ERROR,
    BEEP_QUESTION,
    BEEP_PASSWORD,
    BEEP_NOTIFICATION
  };

  FL_EXPORT void beep(int type=BEEP_DEFAULT);
  FL_EXPORT void message(const char *,...) __fl_attr((__format__ (__printf__, 1, 2)));
  FL_EXPORT void alert(const char *,...) __fl_attr((__format__ (__printf__, 1, 2)));
  // fltk3::ask() is deprecated since it uses "Yes" and "No" for the buttons,
  // which does not conform to the current FLTK Human Interface Guidelines.
  // Use fltk3::choice() instead with the appropriate verbs instead.
  FL_EXPORT int ask(const char *,...) __fl_attr((__format__ (__printf__, 1, 2), __deprecated__));
  FL_EXPORT int choice(const char *q,const char *b0,const char *b1,const char *b2,...) __fl_attr((__format__ (__printf__, 1, 5)));
  FL_EXPORT const char *input(const char *label, const char *deflt = 0, ...) __fl_attr((__format__ (__printf__, 1, 3)));
  FL_EXPORT const char *password(const char *label, const char *deflt = 0, ...) __fl_attr((__format__ (__printf__, 1, 3)));

  FL_EXPORT fltk3::Widget *message_icon();
  extern FL_EXPORT Fl_Font message_font_;
  extern FL_EXPORT Fl_Fontsize message_size_;
  
  inline void message_font(Fl_Font f, Fl_Fontsize s) {
    message_font_ = f; message_size_ = s;
  }

  // pointers you can use to change FLTK to a foreign language:
  extern FL_EXPORT const char* text_no;
  extern FL_EXPORT const char* text_yes;
  extern FL_EXPORT const char* text_ok;
  extern FL_EXPORT const char* text_cancel;
  extern FL_EXPORT const char* text_close;

} // namespace fltk3

#endif // !fl_ask_H

//
// End of "$Id: ask.h 7365 2010-03-30 15:18:29Z matt $".
//
