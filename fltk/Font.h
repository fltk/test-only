//
// "$Id: Font.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $"
//
// When you get a font by name, it returns one of these.
// Do not attempt to create your own or modify it.
//
// Copyright 2002 by Bill Spitzak and others.
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

#ifndef fltk_Font_h
#define fltk_Font_h

#include "Style.h"

namespace fltk {

// This is a struct so I can init a table with constants:
struct FL_API Font {
  const char* name_;
  int attributes_;
  // other fields are added here!

  // return a name with " bold italic" text added to it:
  const char* name() const;

  // return the name and attributes split up:
  const char* name(int* p) {*p = attributes_; return name_;}

  Font* plus(int attributes);
  Font* bold() {return plus(BOLD);}
  Font* italic() {return plus(ITALIC);}

  // return array of sizes:
  int sizes(int*&);

  // return array of encodings:
  int encodings(const char**&);

  // Return the system-specific name
  const char* system_name();

  // Return the full X11 name for the currently selected font+size+encoding
  // Return current_font()->system_name() on other systems:
  static const char* current_name();

  // "measurement" is considered a drawing function, see draw.h
};

// Find and return every font on the system.
FL_API int list_fonts(Font**& arrayp);

}

#endif
