//
// "$Id: Fl_Flags.h,v 1.4 2002/01/23 08:46:00 spitzak Exp $"
//
// Unified flags set for fltk
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

#ifndef Fl_Flags_h
#define Fl_Flags_h

// This same value is used to:

// 1. store state and other information in a widget or menu item
// 2. provide hints to boxes, labeltypes, and images for how to draw
// 3. provide structural information to an array of menu items

// These are shared because various parts of the code, especially
// drawing, want all of this information at once, and providing it
// as a single word is the easiest way.

typedef int Fl_Flags;
typedef Fl_Flags Fl_Align; // for back compatability

enum {
  FL_NO_FLAGS		= 0,

// from Fl_Align, values are comptable with fltk 1.0:
// These control the location and appearance of labels:
  FL_ALIGN_CENTER	= 0,
  FL_ALIGN_TOP          = 0x00000001,
  FL_ALIGN_BOTTOM       = 0x00000002,
  FL_ALIGN_LEFT         = 0x00000004,
  FL_ALIGN_RIGHT        = 0x00000008,
  FL_ALIGN_INSIDE       = 0x00000010,
  FL_ALIGN_CLIP         = 0x00000040,
  FL_ALIGN_WRAP         = 0x00000080,
  FL_ALIGN_MASK		= 0x000000FF,
#ifndef FLTK_2
  FL_ALIGN_TOP_LEFT	= FL_ALIGN_TOP | FL_ALIGN_LEFT,
  FL_ALIGN_TOP_RIGHT	= FL_ALIGN_TOP | FL_ALIGN_RIGHT,
  FL_ALIGN_BOTTOM_LEFT	= FL_ALIGN_BOTTOM | FL_ALIGN_LEFT,
  FL_ALIGN_BOTTOM_RIGHT	= FL_ALIGN_BOTTOM | FL_ALIGN_RIGHT,
  FL_ALIGN_LEFT_TOP	= FL_ALIGN_TOP_LEFT,
  FL_ALIGN_RIGHT_TOP	= FL_ALIGN_TOP_RIGHT,
  FL_ALIGN_LEFT_BOTTOM	= FL_ALIGN_BOTTOM_LEFT,
  FL_ALIGN_RIGHT_BOTTOM	= FL_ALIGN_BOTTOM_RIGHT,
  FL_ALIGN_NOWRAP	= 0,
#endif

  FL_INACTIVE		= 0x00000100, // does not get events, gray it out
  FL_OUTPUT		= 0x00000200, // does not get events
  FL_VALUE		= 0x00000400, // indicates true/down/on state
  FL_SELECTED		= 0x00000800, // draw using selection colors
  FL_INVISIBLE		= 0x00001000, // does not get events, does not draw
  FL_HIGHLIGHT		= 0x00002000, // draw highlighted
  FL_CHANGED		= 0x00004000, // value changed since last callback
  FL_COPIED_LABEL	= 0x00008000, // copy_label() was called
  FL_RAW_LABEL		= 0x00010000, // don't interpret & or @ in label
  FL_PACK_VERTICAL	= 0x00020000  // Fl_Pack puts this widget vertical

};

#endif
