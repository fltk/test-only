//
// "$Id: fl_font.cxx,v 1.43 2004/01/25 06:55:05 spitzak Exp $"
//
// Font selection code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <config.h>
#include <fltk/Font.h>
#include <fltk/draw.h>
#include <fltk/x.h>

/*! \class fltk::Font

Identifies a font. You can create these with fltk::font(name) or
with fltk::list_fonts(). Do not attempt to create your instances
or modify it.

This is a struct so that fltk can initialize a table internally
with constants. Don't use the undocumented fields.
*/

/*! \defgroup font Text Drawing
    \ingroup drawing

   See the fltk::Font class for a description of what can be passed as
   a font. For most uses one of the built-in constant fonts like
   fltk::HELVETICA can be used.
*/

// Static variables containing the currently selected font, size, encoding:
fltk::Font* fltk::current_font_;
float fltk::current_size_;
const char *fltk::encoding_ = "iso8859-1";

#if USE_X11
# if USE_XFT
#  include "fl_font_xft.cxx"
# else
#  include "fl_font_x.cxx"
# endif
#elif defined(_WIN32)
# include "fl_font_win32.cxx"
#elif defined(__APPLE__)
# include "fl_font_mac.cxx"
#else
#error
#endif

/*! \fn Font* fltk::getfont()
  Return the Font sent to the last setfont().
*/
/*! \fn float fltk::getsize()
  Return the size sent to the last setfont(). You should use this as a
  minimum line spacing (using ascent()+descent() will produce oddly
  spaced lines for many fonts).
*/

/*! Draw a nul-terminated string. */
void
fltk::drawtext(const char* text, float x, float y) {
  drawtext(text, strlen(text), x, y);
}

/*!
  Draw the first n \e bytes (not characters if utf8 is used) starting
  at the given position. */
void
fltk::drawtext(const char* text, int n, float x, float y) {
  transform(x,y);
  drawtext_transformed(text, n, x, y);
}

/*! Return the width of a nul-terminated string. */
float
fltk::getwidth(const char* text) { return getwidth(text, strlen(text)); }

/*! Return a single string that names this font. If the attributes
  are zero this is easy. If they are not it prints the string into
  a static buffer and adds " bold" and/or " italic" to the end.
  A string of this form can be sent to find() to get a font with
  attributes.
*/
const char* fltk::Font::name() const {
  if (!attributes_) return name_;
  static char *buffer; if (!buffer) buffer = new char[128];
  strcpy(buffer, name_);
  if (attributes_ & BOLD) strcat(buffer, " bold");
  if (attributes_ & ITALIC) strcat(buffer, " italic");
  return buffer;
}

/*! \fn const char* fltk::Font::name(int* attributes)
  Return a string name for this font, and put any attributes
  (BOLD, ITALIC) into the location pointed to by \a attributes.
*/

//
// End of "$Id: fl_font.cxx,v 1.43 2004/01/25 06:55:05 spitzak Exp $".
//
