//
// "$Id: ask.h 8441 2011-02-18 08:52:48Z AlbrechtS $"
//
// Standard dialog header file for the Fast Light Tool Kit (FLTK).
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

#ifndef fltk3_ask_H
#  define fltk3_ask_H

#  include "enumerations.h"

#  ifdef __GNUC__
#    define __fl_attr(x) __attribute__ (x)
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
  
  FLTK3_EXPORT void beep(int type = fltk3::BEEP_DEFAULT);
  FLTK3_EXPORT void message(const char *,...) __fl_attr((__format__ (__printf__, 1, 2)));
  FLTK3_EXPORT void alert(const char *,...) __fl_attr((__format__ (__printf__, 1, 2)));
  // fltk3::ask() is deprecated since it uses "Yes" and "No" for the buttons,
  // which does not conform to the current FLTK Human Interface Guidelines.
  // Use fltk3::choice() instead with the appropriate verbs instead.
  FLTK3_EXPORT int ask(const char *,...) __fl_attr((__format__ (__printf__, 1, 2), __deprecated__));
  FLTK3_EXPORT int choice(const char *q,const char *b0,const char *b1,const char *b2,...) __fl_attr((__format__ (__printf__, 1, 5)));
  FLTK3_EXPORT const char *input(const char *label, const char *deflt = 0, ...) __fl_attr((__format__ (__printf__, 1, 3)));
  FLTK3_EXPORT const char *password(const char *label, const char *deflt = 0, ...) __fl_attr((__format__ (__printf__, 1, 3)));
  
  FLTK3_EXPORT fltk3::Widget *message_icon();
  extern FLTK3_EXPORT fltk3::Font message_font_;
  extern FLTK3_EXPORT fltk3::Fontsize message_size_;
  inline void message_font(fltk3::Font f, fltk3::Fontsize s) {
    fltk3::message_font_ = f; fltk3::message_size_ = s;
  }
  
  FLTK3_EXPORT void message_hotspot(int enable);
  FLTK3_EXPORT int message_hotspot(void);
  
  FLTK3_EXPORT void message_title(const char *title);
  FLTK3_EXPORT void message_title_default(const char *title);
  
  // pointers you can use to change FLTK to a foreign language:
  extern FLTK3_EXPORT const char* no;
  extern FLTK3_EXPORT const char* yes;
  extern FLTK3_EXPORT const char* ok;
  extern FLTK3_EXPORT const char* cancel;
  extern FLTK3_EXPORT const char* close;
  
}

#endif // !fltk3::ask_H

//
// End of "$Id: ask.h 8441 2011-02-18 08:52:48Z AlbrechtS $".
//
