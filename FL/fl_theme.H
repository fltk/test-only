//
// "$Id: fl_theme.H,v 1.6 2001/02/21 06:15:44 clip Exp $"
//
// Header for themes that also go into fltk_themes static library.
// These are the important common ones such as Windows, Motif, KDE1 & GNOME.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef fl_theme_H
#define fl_theme_H

#include <FL/Enumerations.H>

typedef int (*Fl_Theme_Handler)(int);
typedef int (*Fl_Theme_Function)();

// We don't want FL_API for these
int fl_windows();
int fl_kde(int = 0);
int fl_gnome();
int fl_motif();
int fl_xft();

FL_API void fl_theme_handler(Fl_Theme_Handler);

#endif

//
// End of "$Id: fl_theme.H,v 1.6 2001/02/21 06:15:44 clip Exp $".
//
