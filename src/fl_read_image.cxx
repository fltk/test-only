//
// "$Id$"
//
// X11 image reading routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#include <FL/x.H>
#include <FL/fl_draw.H>
#include "flstring.h"

#ifdef DEBUG
#  include <stdio.h>
#endif // DEBUG

#ifdef WIN32
#  include "win/read_image.cxx"
#elif defined(__APPLE__)
#  include "carbon/read_image.cxx"
#else
#  include <X11/Xutil.h>
#  ifdef __sgi
#    include <X11/extensions/readdisplay.h>
#  endif // __sgi
#  include "xlib/read_image.cxx"
#endif

//
// End of "$Id$".
//
