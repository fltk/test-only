//
// "$Id$"
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

/*! \addtogroup flags
  \{ */

/*! Type returned by fltk::Widget::flags() and passed to fltk::Box
  and many other drawing functions. */
typedef int Flags;

/*! For back compatability with fltk1.1 */
typedef Flags Align; // for back compatability

enum {
  NO_FLAGS		= 0,

  // from Align, values are comptable with fltk 1.0:
  // These control the location and appearance of labels:
  // Warning: unused numbers may change behavior!
  ALIGN_CENTER		= 0,	//!< (0) The label is centered inside widget
  ALIGN_TOP		= 1,	//!< Label is centered above widget
  ALIGN_BOTTOM		= 2,	//!< Label is centered below widget
  ALIGN_LEFTTOP		= 3,	//!< Label is left of widget at top
  ALIGN_LEFT		= 4,	//!< Label is to left of widget
  ALIGN_TOPLEFT		= 5,	//!< Label is left-justified above widget
  ALIGN_BOTTOMLEFT	= 6,	//!< Label is left-justified below widget
  ALIGN_LEFTBOTTOM	= 7,	//!< Label is left of widget at bottom
  ALIGN_RIGHT		= 8,	//!< Label is to right of widget
  ALIGN_TOPRIGHT	= 9,	//!< Label is right-justified above widget
  ALIGN_BOTTOMRIGHT	= 10,	//!< Label is right-justified below widget
  ALIGN_RIGHTTOP	= 11,	//!< Label is right of widget at top
  ALIGN_CENTERLEFT	= 12,	//!< Label is centered in space left of widget
  ALIGN_RIGHTBOTTOM	= 15,	//!< Label is right of widget at bottom
  ALIGN_INSIDE		= 16,	//!< Label is inside widget, image centered
  ALIGN_INSIDE_TOP	= 17,	//!< Label is inside widget at top
  ALIGN_INSIDE_BOTTOM	= 18,	//!< Label is inside widget at bottom
  ALIGN_INSIDE_LEFT	= 20,	//!< Label is inside widget at left
  ALIGN_INSIDE_TOPLEFT	= 21,	//!< Label is inside widget at top left
  ALIGN_INSIDE_BOTTOMLEFT=22,	//!< Label is inside widget at bottom left
  ALIGN_INSIDE_RIGHT	= 24,	//!< Label is inside widget at right
  ALIGN_INSIDE_TOPRIGHT	= 25,	//!< Label is inside widget at top right
  ALIGN_INSIDE_BOTTOMRIGHT=26,	//!< Label is inside widget bottom right
  ALIGN_CLIP		= 0x00000040, //!< The label is clipped to the widget
  ALIGN_WRAP		= 0x00000080, //!< The label is word-wrapped
  ALIGN_MASK		= 0x000000FF, //!< Used to split align() from flags()

  NOTACTIVE		= 0x00000100, //!< !active()
  OUTPUT		= 0x00000200, //!< does not get events, draw text colors
  VALUE			= 0x00000400, //!< true/on state for buttons
  SELECTED		= 0x00000800, //!< chosen in browser/menu, draw selected colors
  INVISIBLE		= 0x00001000, //!< !visible(), draw_frame()
  HIGHLIGHT		= 0x00002000, //!< draw highlighted
  CHANGED		= 0x00004000, //!< value changed since last callback
  COPIED_LABEL		= 0x00008000, //!< copy_label() was called
  RAW_LABEL		= 0x00010000, //!< don't interpret & or @ in label
  LAYOUT_VERTICAL	= 0x00020000, //!< fltk::Pack puts this widget vertical
  TAB_TO_FOCUS		= 0x00040000, //!< Widget::tab_to_focus();
  CLICK_TO_FOCUS	= 0x00080000, //!< Widget::click_to_focus()
  INACTIVE		= 0x00100000, //!< draw it grayed-out
  FOCUSED		= 0x00200000, //!< draw with keyboard focus
  PUSHED		= 0x00400000  //!< draw pushed-in
};
/*! \} */

}

#endif

//
// End of "$Id$".
//
