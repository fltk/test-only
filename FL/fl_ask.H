//
// "$Id: fl_ask.H,v 1.14 2001/02/23 05:14:18 clip Exp $"
//
// Standard dialog header file for the Fast Light Tool Kit (FLTK).
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

#ifndef fl_ask_H
#define fl_ask_H

#include "Enumerations.H"
#include "Fl_Font.H"
#include "Fl_Style.H"

class FL_API Fl_Widget;

FL_API void fl_message(const char *,...);
FL_API void fl_alert(const char *,...);
FL_API int fl_ask(const char *,...);
FL_API int fl_choice(const char *q,const char *b0,const char *b1,const char *b2,...);
FL_API const char *fl_input(const char *label, const char *deflt = 0, ...);
FL_API const char *fl_password(const char *label, const char *deflt = 0, ...);

// You can modify this widget to change the icon in the popup panel:
FL_API void fl_message_icon(Fl_Widget *);

#ifndef FLTK_2
extern FL_API Fl_Named_Style* fl_message_style;
inline void fl_message_font(Fl_Font f, unsigned s) {
  fl_message_style->label_font = f;
  fl_message_style->label_size = s;
}
#endif

// pointers you can use to change FLTK to a foreign language:
extern FL_API const char* fl_no;
extern FL_API const char* fl_yes;
extern FL_API const char* fl_ok;
extern FL_API const char* fl_cancel;

#endif

//
// End of "$Id: fl_ask.H,v 1.14 2001/02/23 05:14:18 clip Exp $".
//
