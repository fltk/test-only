//
// "$Id: names.h 7903 2010-11-28 21:06:39Z matt $"
//
// Event names header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

// Thanks to Greg Ercolano for this addition.

#ifndef FLtk3_NAMES_H
#define FLtk3_NAMES_H

/** \defgroup fl_events Events handling functions
    @{
 */

/**
  This is an array of event names you can use to convert event numbers into names.

  The array gets defined inline wherever your '\#include <fltk3/names.h>' appears.

  \b Example:
  \code
  #include <fltk3/names.h>		// array will be defined here
  int MyClass::handle(int e) {
      printf("Event was %s (%d)\n", fl_eventnames[e], e);
      // ..resulting output might be e.g. "Event was fltk3::PUSH (1)"..
      [..]
  }
  \endcode
 */
const char * const fl_eventnames[] =
{
  "fltk3::NO_EVENT",
  "fltk3::PUSH",
  "fltk3::RELEASE",
  "fltk3::ENTER",
  "fltk3::LEAVE",
  "fltk3::DRAG",
  "fltk3::FOCUS",
  "fltk3::UNFOCUS",
  "FL_KEYDOWN",
  "fltk3::KEYUP",
  "fltk3::CLOSE",
  "fltk3::MOVE",
  "fltk3::SHORTCUT",
  "fltk3::DEACTIVATE",
  "fltk3::ACTIVATE",
  "fltk3::HIDE",
  "fltk3::SHOW",
  "fltk3::PASTE",
  "fltk3::SELECTIONCLEAR",
  "fltk3::MOUSEWHEEL",
  "fltk3::DND_ENTER",
  "fltk3::DND_DRAG",
  "fltk3::DND_LEAVE",
  "fltk3::DND_RELEASE",
};

/**
  This is an array of font names you can use to convert font numbers into names.

  The array gets defined inline wherever your '\#include <fltk3/names.h>' appears.

  \b Example:
  \code
  #include <fltk3/names.h>		// array will be defined here
  int MyClass::my_callback(fltk3::Widget *w, void*) {
      int fnum = w->labelfont();
      // Resulting output might be e.g. "Label's font is fltk3::HELVETICA (0)"
      printf("Label's font is %s (%d)\n", fl_fontnames[fnum], fnum);
      // ..resulting output might be e.g. "Label's font is fltk3::HELVETICA (0)"..
      [..]
  }
  \endcode
 */
const char * const fl_fontnames[] =
{
  "fltk3::HELVETICA",
  "fltk3::HELVETICA_BOLD",
  "fltk3::HELVETICA_ITALIC",
  "fltk3::HELVETICA_BOLD_ITALIC",
  "fltk3::COURIER",
  "fltk3::COURIER_BOLD",
  "fltk3::COURIER_ITALIC",
  "fltk3::COURIER_BOLD_ITALIC",
  "fltk3::TIMES",
  "fltk3::TIMES_BOLD",
  "fltk3::TIMES_ITALIC",
  "fltk3::TIMES_BOLD_ITALIC",
  "fltk3::SYMBOL",
  "fltk3::SCREEN",
  "fltk3::SCREEN_BOLD",
  "fltk3::ZAPF_DINGBATS",
};

/** @} */

#endif /* FL_NAMES_H */

//
// End of "$Id: names.h 7903 2010-11-28 21:06:39Z matt $".
//
