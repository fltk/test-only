// "$Id: visual.h,v 1.3 2003/04/20 03:17:49 easysw Exp $"
//
// Mostly X-specific crap to allow you to force the "visual" used by
// fltk to one you like. Does nothing on other systems.
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

#ifndef fltk_visual_h
#define fltk_visual_h

#include "FL_API.h"

namespace fltk {

enum { // Values for visual(), glVisual(), GlWindow::mode()
  RGB_COLOR	= 0,
  INDEXED_COLOR	= 1,
  SINGLE_BUFFER	= 0,
  DOUBLE_BUFFER	= 2,
  ACCUM_BUFFER	= 4,
  ALPHA_BUFFER	= 8,
  DEPTH_BUFFER	= 16,
  STENCIL_BUFFER= 32,
  RGB24_COLOR	= 64,
  MULTISAMPLE	= 128,
  STEREO	= 256
};

// Make it capable of displaying the given attributes:
extern FL_API bool visual(int);

// Use OpenGL to choose the visual:
extern FL_API bool glVisual(int);

// Make it use it's own colormap:
extern FL_API void own_colormap();

}

#endif
