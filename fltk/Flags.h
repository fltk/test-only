//
// "$Id: Flags.h,v 1.2 2002/12/10 02:00:29 easysw Exp $"
//
// Unified flags set for fltk. These flags control the appearance of
// boxes and widgets. This same value is used to:
//
// 1. store state and other information in a widget or menu item
// 2. provide hints to boxes, labeltypes, and images for how to draw
// 3. provide structural information to an array of menu items
//
// These are shared because various parts of the code, especially
// drawing, want all of this information at once, and providing it
// as a single word is the easiest way.
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

#ifndef fltk_Flags_h
#define fltk_Flags_h

namespace fltk {

typedef int Flags;
typedef Flags Align; // for back compatability

enum {
  NO_FLAGS		= 0,

  // from Align, values are comptable with fltk 1.0:
  // These control the location and appearance of labels:
  ALIGN_CENTER		= 0,
  ALIGN_TOP		= 0x00000001,
  ALIGN_BOTTOM		= 0x00000002,
  ALIGN_LEFT		= 0x00000004,
  ALIGN_RIGHT		= 0x00000008,
  ALIGN_INSIDE		= 0x00000010,
  ALIGN_CLIP		= 0x00000040,
  ALIGN_WRAP		= 0x00000080,
  ALIGN_MASK		= 0x000000FF,

  INACTIVE		= 0x00000100, // does not get events, gray it out
  OUTPUT		= 0x00000200, // does not get events
  VALUE			= 0x00000400, // indicates true/down/on state
  SELECTED		= 0x00000800, // draw using selection colors
  INVISIBLE		= 0x00001000, // does not get events, does not draw
  HIGHLIGHT		= 0x00002000, // draw highlighted
  CHANGED		= 0x00004000, // value changed since last callback
  COPIED_LABEL		= 0x00008000, // copy_label() was called
  RAW_LABEL		= 0x00010000, // don't interpret & or @ in label
  PACK_VERTICAL		= 0x00020000, // Pack puts this widget vertical
  CLICK_TO_FOCUS	= 0x00040000  // Set true if clicking this widget causes it to take focus

};

}

#endif
