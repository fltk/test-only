//
// "$Id: ask.h,v 1.2 2002/12/10 02:00:29 easysw Exp $"
//
// Utility pop-up windows for fltk
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".

#ifndef fltk_ask_h
#define fltk_ask_h

#include "FL_API.h"
#include "Style.h"

namespace fltk {

class Widget;

FL_API void message(const char *, ...);
FL_API void alert(const char *, ...);
FL_API int ask(const char *, ...);
FL_API int choice(const char *q,
		  const char *b0, const char *b1, const char *b2, ...);
FL_API const char *input(const char *label, const char *deflt = 0, ...);
FL_API const char *password(const char *label, const char *deflt = 0, ...);

extern FL_API NamedStyle* icon_style;
extern FL_API NamedStyle* message_style;

// pointers you can use to change FLTK to a foreign language:
extern FL_API const char* no;
extern FL_API const char* yes;
extern FL_API const char* ok;
extern FL_API const char* cancel;

}

#endif

//
// End of "$Id: ask.h,v 1.2 2002/12/10 02:00:29 easysw Exp $".
//
