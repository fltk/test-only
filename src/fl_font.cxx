//
// "$Id: fl_font.cxx,v 1.27 2001/02/20 06:59:50 spitzak Exp $"
//
// Font selection code for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#include <FL/Fl_Font.H>
#include <FL/fl_draw.H>

// Static variables containing the currently selected font & size:
Fl_Font fl_font_;
unsigned fl_size_;
FL_API Fl_FontSize* fl_fontsize;

#ifdef WIN32
#include "fl_font_win32.cxx"
#else
#include "fl_font_x.cxx"
#endif

//
// End of "$Id: fl_font.cxx,v 1.27 2001/02/20 06:59:50 spitzak Exp $".
//
