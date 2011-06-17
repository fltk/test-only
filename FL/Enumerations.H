//
// "$Id$"
//
// Enumerations for the Fast Light Tool Kit (FLTK).
// FLTK 123 wrapper complete
//
// Copyright 1998-2011 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#ifndef Fl_Enumerations_H
#define Fl_Enumerations_H

#include <fltk3/enumerations.h>

#include "Fl_Export.H"
#include "fl_types.h"

#undef FL_MAJOR_VERSION
#define FL_MAJOR_VERSION	1
#undef FL_MINOR_VERSION
#define FL_MINOR_VERSION	3
#undef FL_PATCH_VERSION
#define FL_PATCH_VERSION	0
#undef FL_VERSION
#define FL_VERSION \
  ((double)FL_MAJOR_VERSION + \
  (double)FL_MINOR_VERSION * 0.01 + \
  (double)FL_PATCH_VERSION * 0.0001)


class Fl_Widget;
class Fl_Window;
class Fl_Group;

namespace fltk3 {
  class Widget;
  class Window;
  class Group;
  void *_wrapper(Widget*);
  void *_wrapper(Window*);
  void *_wrapper(Group*);
  inline Fl_Widget *_3to1_widget(Widget *w) { return w?(Fl_Widget*)_wrapper(w):0; }
  inline Fl_Window *_3to1_window(Window *w) { return w?(Fl_Window*)_wrapper(w):0; }
  inline Fl_Group  *_3to1_group(Group *w) { return w?(Fl_Group*)_wrapper(w):0; }
  void *_object(Fl_Widget *w);
  void *_object(Fl_Window *w);
  void *_object(Fl_Group *w);
  inline Widget *_1to3_widget(Fl_Widget *w) { return w?(fltk3::Widget*)_object(w):0; }
  inline Window *_1to3_window(Fl_Window *w) { return w?(fltk3::Window*)_object(w):0; }
  inline Group  *_1to3_group(Fl_Group *w) { return w?(fltk3::Group*)_object(w):0; }
}


typedef enum {
  FL_NO_EVENT		= 0,
  FL_PUSH		= 1,
  FL_RELEASE		= 2,
  FL_ENTER		= 3,
  FL_LEAVE		= 4,
  FL_DRAG		= 5,
  FL_FOCUS		= 6,
  FL_UNFOCUS		= 7,
  FL_KEYDOWN		= 8,
  FL_KEYBOARD		= 8,
  FL_KEYUP		= 9,
  FL_CLOSE		= 10,
  FL_MOVE		= 11,
  FL_SHORTCUT		= 12,
  FL_DEACTIVATE		= 13,
  FL_ACTIVATE		= 14,
  FL_HIDE		= 15,
  FL_SHOW		= 16,
  FL_PASTE		= 17,
  FL_SELECTIONCLEAR	= 18,
  FL_MOUSEWHEEL		= 19,
  FL_DND_ENTER		= 20,
  FL_DND_DRAG		= 21,
  FL_DND_LEAVE		= 22,
  FL_DND_RELEASE	= 23
} Fl_Event;

namespace fltk3 {
  inline Fl_Event _3to1_event(Event e) { return (Fl_Event)e; }
  inline Event _1to3_event(Fl_Event e) { return (Event)e; }
}

typedef enum {
  FL_WHEN_NEVER		= 0,
  FL_WHEN_CHANGED	= 1,
  FL_WHEN_NOT_CHANGED	= 2,
  FL_WHEN_RELEASE	= 4,
  FL_WHEN_RELEASE_ALWAYS= 6,
  FL_WHEN_ENTER_KEY	= 8,
  FL_WHEN_ENTER_KEY_ALWAYS=10,
  FL_WHEN_ENTER_KEY_CHANGED=11
} Fl_When;

namespace fltk3 {
  inline Fl_When _3to1_when(When w) { return (Fl_When)w; }
  inline When _1to3_when(Fl_When w) { return (When)w; }
}

#define FL_Button	0xfee8
#define FL_BackSpace	0xff08
#define FL_Tab		0xff09
#define FL_Enter	0xff0d
#define FL_Pause	0xff13
#define FL_Scroll_Lock	0xff14
#define FL_Escape	0xff1b
#define FL_Home		0xff50
#define FL_Left		0xff51
#define FL_Up		0xff52
#define FL_Right	0xff53
#define FL_Down		0xff54
#define FL_Page_Up	0xff55
#define FL_Page_Down	0xff56
#define FL_End		0xff57
#define FL_Print	0xff61
#define FL_Insert	0xff63
#define FL_Menu		0xff67
#define FL_Help		0xff68
#define FL_Num_Lock	0xff7f
#define FL_KP		0xff80
#define FL_KP_Enter	0xff8d
#define FL_KP_Last	0xffbd
#define FL_F		0xffbd
#define FL_F_Last	0xffe0
#define FL_Shift_L	0xffe1
#define FL_Shift_R	0xffe2
#define FL_Control_L	0xffe3
#define FL_Control_R	0xffe4
#define FL_Caps_Lock	0xffe5
#define FL_Meta_L	0xffe7
#define FL_Meta_R	0xffe8
#define FL_Alt_L	0xffe9
#define FL_Alt_R	0xffea
#define FL_Delete	0xffff

namespace fltk3 {
  inline unsigned int _3to1_key(unsigned int k) { return k; }
  inline unsigned int _1to3_key(unsigned int k) { return k; }
}

#define FL_LEFT_MOUSE	1
#define FL_MIDDLE_MOUSE	2
#define FL_RIGHT_MOUSE	3

namespace fltk3 {
  inline unsigned int _3to1_button(unsigned int k) { return k; }
  inline unsigned int _1to3_button(unsigned int k) { return k; }
}

#define FL_SHIFT	0x00010000
#define FL_CAPS_LOCK	0x00020000
#define FL_CTRL		0x00040000
#define FL_ALT		0x00080000
#define FL_NUM_LOCK	0x00100000				
#define FL_META		0x00400000
#define FL_SCROLL_LOCK	0x00800000
#define FL_BUTTON1	0x01000000
#define FL_BUTTON2	0x02000000
#define FL_BUTTON3	0x04000000
#define FL_BUTTONS	0x7f000000

namespace fltk3 {
  inline unsigned int _3to1_modifier(unsigned int k) { return k; }
  inline unsigned int _1to3_modifier(unsigned int k) { return k; }
}

#define FL_BUTTON(n)	(0x00800000<<(n))
#define FL_KEY_MASK 0x0000ffff

#ifdef __APPLE__
#  define FL_COMMAND	FL_META
#  define FL_CONTROL 	FL_CTRL
#else
#  define FL_COMMAND	FL_CTRL
#  define FL_CONTROL	FL_META
#endif // __APPLE__

typedef enum {
  FL_NO_BOX = 0,
  FL_FLAT_BOX,
  FL_UP_BOX,
  FL_DOWN_BOX,
  FL_UP_FRAME,
  FL_DOWN_FRAME,
  FL_THIN_UP_BOX,
  FL_THIN_DOWN_BOX,
  FL_THIN_UP_FRAME,
  FL_THIN_DOWN_FRAME,
  FL_ENGRAVED_BOX,
  FL_EMBOSSED_BOX,
  FL_ENGRAVED_FRAME,
  FL_EMBOSSED_FRAME,
  FL_BORDER_BOX,
  FL_SHADOW_BOX,
  FL_BORDER_FRAME,
  FL_SHADOW_FRAME,
  FL_ROUNDED_BOX,
  FL_RSHADOW_BOX,
  FL_ROUNDED_FRAME,
  FL_RFLAT_BOX,
  FL_ROUND_UP_BOX,
  FL_ROUND_DOWN_BOX,
  FL_DIAMOND_UP_BOX,
  FL_DIAMOND_DOWN_BOX,
  FL_OVAL_BOX,
  FL_OSHADOW_BOX,
  FL_OVAL_FRAME,
  FL_OFLAT_BOX,
  FL_PLASTIC_UP_BOX,
  FL_PLASTIC_DOWN_BOX,
  FL_PLASTIC_UP_FRAME,
  FL_PLASTIC_DOWN_FRAME,
  FL_PLASTIC_THIN_UP_BOX,
  FL_PLASTIC_THIN_DOWN_BOX,
  FL_PLASTIC_ROUND_UP_BOX,
  FL_PLASTIC_ROUND_DOWN_BOX,
  FL_GTK_UP_BOX,
  FL_GTK_DOWN_BOX,
  FL_GTK_UP_FRAME,
  FL_GTK_DOWN_FRAME,
  FL_GTK_THIN_UP_BOX,
  FL_GTK_THIN_DOWN_BOX,
  FL_GTK_THIN_UP_FRAME,
  FL_GTK_THIN_DOWN_FRAME,
  FL_GTK_ROUND_UP_BOX,
  FL_GTK_ROUND_DOWN_BOX,
  FL_FREE_BOXTYPE
} Fl_Boxtype;

namespace fltk3 {
  inline Fl_Boxtype _3to1_boxtype(Boxtype k) { return (Fl_Boxtype)k; }
  inline Boxtype _1to3_boxtype(Fl_Boxtype k) { return (Boxtype)k; }
}

inline Fl_Boxtype fl_box(Fl_Boxtype b) {
  return fltk3::_3to1_boxtype(fltk3::box(fltk3::_1to3_boxtype(b)));
}

inline Fl_Boxtype fl_down(Fl_Boxtype b) {
  return fltk3::_3to1_boxtype(fltk3::down(fltk3::_1to3_boxtype(b)));
}

inline Fl_Boxtype fl_frame(Fl_Boxtype b) {
  return fltk3::_3to1_boxtype(fltk3::frame(fltk3::_1to3_boxtype(b)));
}

#define FL_FRAME FL_ENGRAVED_FRAME
#define FL_FRAME_BOX FL_ENGRAVED_BOX
#define FL_CIRCLE_BOX FL_ROUND_DOWN_BOX
#define FL_DIAMOND_BOX FL_DIAMOND_DOWN_BOX

typedef enum {
  FL_NORMAL_LABEL = 0,
  FL_SYMBOL_LABEL = 0,
  FL_NO_LABEL,
  FL_SHADOW_LABEL,
  FL_ENGRAVED_LABEL,
  FL_EMBOSSED_LABEL,
  FL_MULTI_LABEL,
  FL_ICON_LABEL,
  FL_IMAGE_LABEL,
  FL_FREE_LABELTYPE
} Fl_Labeltype;

namespace fltk3 {
  inline Fl_Labeltype _3to1_labeltype(Labeltype k) { return (Fl_Labeltype)k; }
  inline Labeltype _1to3_labeltype(Fl_Labeltype k) { return (Labeltype)k; }
}


typedef unsigned Fl_Align;
const Fl_Align FL_ALIGN_CENTER		= (Fl_Align)0;
const Fl_Align FL_ALIGN_TOP		= (Fl_Align)1;
const Fl_Align FL_ALIGN_BOTTOM		= (Fl_Align)2;
const Fl_Align FL_ALIGN_LEFT		= (Fl_Align)4;
const Fl_Align FL_ALIGN_RIGHT		= (Fl_Align)8;
const Fl_Align FL_ALIGN_INSIDE		= (Fl_Align)16;
const Fl_Align FL_ALIGN_TEXT_OVER_IMAGE	= (Fl_Align)0x0020;
const Fl_Align FL_ALIGN_IMAGE_OVER_TEXT	= (Fl_Align)0x0000;
const Fl_Align FL_ALIGN_CLIP		= (Fl_Align)64;
const Fl_Align FL_ALIGN_WRAP		= (Fl_Align)128;
const Fl_Align FL_ALIGN_IMAGE_NEXT_TO_TEXT = (Fl_Align)0x0100;
const Fl_Align FL_ALIGN_TEXT_NEXT_TO_IMAGE = (Fl_Align)0x0120;
const Fl_Align FL_ALIGN_IMAGE_BACKDROP  = (Fl_Align)0x0200;
const Fl_Align FL_ALIGN_TOP_LEFT	= FL_ALIGN_TOP | FL_ALIGN_LEFT;
const Fl_Align FL_ALIGN_TOP_RIGHT	= FL_ALIGN_TOP | FL_ALIGN_RIGHT;
const Fl_Align FL_ALIGN_BOTTOM_LEFT	= FL_ALIGN_BOTTOM | FL_ALIGN_LEFT;
const Fl_Align FL_ALIGN_BOTTOM_RIGHT	= FL_ALIGN_BOTTOM | FL_ALIGN_RIGHT;
const Fl_Align FL_ALIGN_LEFT_TOP	= 0x0007;
const Fl_Align FL_ALIGN_RIGHT_TOP	= 0x000b;
const Fl_Align FL_ALIGN_LEFT_BOTTOM	= 0x000d;
const Fl_Align FL_ALIGN_RIGHT_BOTTOM	= 0x000e;
const Fl_Align FL_ALIGN_NOWRAP		= (Fl_Align)0;
const Fl_Align FL_ALIGN_POSITION_MASK   = 0x000f;
const Fl_Align FL_ALIGN_IMAGE_MASK      = 0x0320;

namespace fltk3 {
  inline Fl_Align _3to1_align(Align k) { return (Fl_Align)k; }
  inline Align _1to3_align(Fl_Align k) { return (Align)k; }
}

typedef int Fl_Font;
const Fl_Font FL_HELVETICA              = 0;
const Fl_Font FL_HELVETICA_BOLD         = 1;
const Fl_Font FL_HELVETICA_ITALIC       = 2;
const Fl_Font FL_HELVETICA_BOLD_ITALIC  = 3;
const Fl_Font FL_COURIER                = 4;
const Fl_Font FL_COURIER_BOLD           = 5;
const Fl_Font FL_COURIER_ITALIC         = 6;
const Fl_Font FL_COURIER_BOLD_ITALIC    = 7;
const Fl_Font FL_TIMES                  = 8;
const Fl_Font FL_TIMES_BOLD             = 9;
const Fl_Font FL_TIMES_ITALIC           = 10;
const Fl_Font FL_TIMES_BOLD_ITALIC      = 11;
const Fl_Font FL_SYMBOL                 = 12;
const Fl_Font FL_SCREEN                 = 13;
const Fl_Font FL_SCREEN_BOLD            = 14;
const Fl_Font FL_ZAPF_DINGBATS          = 15;
const Fl_Font FL_FREE_FONT              = 16;
const Fl_Font FL_BOLD                   = 1;
const Fl_Font FL_ITALIC                 = 2;
const Fl_Font FL_BOLD_ITALIC            = 3;

namespace fltk3 {
  inline Fl_Font _3to1_font(Font k) { return (Fl_Font)k; }
  inline Font _1to3_font(Fl_Font k) { return (Font)k; }
}

typedef int Fl_Fontsize;
const Fl_Fontsize FL_NORMAL_SIZE = 12; // FIXME: bad!

namespace fltk3 {
  inline Fl_Fontsize _3to1_fontsize(Fontsize k) { return (Fl_Fontsize)k; }
  inline Fontsize _1to3_fontsize(Fl_Fontsize k) { return (Fontsize)k; }
}

typedef unsigned int Fl_Color;

const Fl_Color FL_FOREGROUND_COLOR  = 0;
const Fl_Color FL_BACKGROUND2_COLOR = 7;
const Fl_Color FL_INACTIVE_COLOR    = 8;
const Fl_Color FL_SELECTION_COLOR   = 15;
const Fl_Color FL_GRAY0   = 32;
const Fl_Color FL_DARK3   = 39;
const Fl_Color FL_DARK2   = 45;
const Fl_Color FL_DARK1   = 47;
const Fl_Color FL_BACKGROUND_COLOR  = 49;
const Fl_Color FL_LIGHT1  = 50;
const Fl_Color FL_LIGHT2  = 52;
const Fl_Color FL_LIGHT3  = 54;
const Fl_Color FL_BLACK   = 56;
const Fl_Color FL_RED     = 88;
const Fl_Color FL_GREEN   = 63;
const Fl_Color FL_YELLOW  = 95;
const Fl_Color FL_BLUE    = 216;
const Fl_Color FL_MAGENTA = 248;
const Fl_Color FL_CYAN    = 223;
const Fl_Color FL_DARK_RED = 72;
const Fl_Color FL_DARK_GREEN    = 60;
const Fl_Color FL_DARK_YELLOW   = 76;
const Fl_Color FL_DARK_BLUE     = 136;
const Fl_Color FL_DARK_MAGENTA  = 152;
const Fl_Color FL_DARK_CYAN     = 140;
const Fl_Color FL_WHITE         = 255;

#define FL_FREE_COLOR     (Fl_Color)16
#define FL_NUM_FREE_COLOR 16
#define FL_GRAY_RAMP      (Fl_Color)32
#define FL_NUM_GRAY       24
#define FL_GRAY           FL_BACKGROUND_COLOR
#define FL_COLOR_CUBE     (Fl_Color)56
#define FL_NUM_RED        5
#define FL_NUM_GREEN      8
#define FL_NUM_BLUE       5

namespace fltk3 {
  inline Fl_Color _3to1_color(Color k) { return (Fl_Color)k; }
  inline Color _1to3_color(Fl_Color k) { return (Color)k; }
}

inline Fl_Color fl_inactive(Fl_Color c) {
  return fltk3::_3to1_color( fltk3::inactive(fltk3::_1to3_color(c)) );
}

inline Fl_Color fl_contrast(Fl_Color fg, Fl_Color bg) {
  return fltk3::_3to1_color( fltk3::contrast(fltk3::_1to3_color(fg),
                                             fltk3::_1to3_color(bg)) );
}

inline Fl_Color fl_color_average(Fl_Color c1, Fl_Color c2, float weight) {
  return fltk3::_3to1_color( fltk3::color_average(fltk3::_1to3_color(c1),
                                                  fltk3::_1to3_color(c2),
                                                  weight) );
}

inline Fl_Color fl_lighter(Fl_Color c) { 
  return fl_color_average(c, FL_WHITE, .67f); 
}

inline Fl_Color fl_darker(Fl_Color c) { 
  return fl_color_average(c, FL_BLACK, .67f); 
}

inline Fl_Color fl_rgb_color(uchar r, uchar g, uchar b) {
  return fltk3::_3to1_color( fltk3::rgb_color(r, g, b) );
}

inline Fl_Color fl_rgb_color(uchar g) {
  return fltk3::_3to1_color( fltk3::rgb_color(g) );
}

inline Fl_Color fl_gray_ramp(int i) {
  return fltk3::_3to1_color( fltk3::gray_ramp(i) );
}

inline Fl_Color fl_color_cube(int r, int g, int b) {
  return fltk3::_3to1_color( fltk3::color_cube(r, g, b) );
}

typedef enum {
  FL_CURSOR_DEFAULT	=  0,
  FL_CURSOR_ARROW	= 35,
  FL_CURSOR_CROSS	= 66,
  FL_CURSOR_WAIT	= 76,
  FL_CURSOR_INSERT	= 77,
  FL_CURSOR_HAND	= 31,
  FL_CURSOR_HELP	= 47,
  FL_CURSOR_MOVE	= 27,
  FL_CURSOR_NS		= 78,
  FL_CURSOR_WE		= 79,
  FL_CURSOR_NWSE	= 80,
  FL_CURSOR_NESW	= 81,
  FL_CURSOR_NONE	=255,
  FL_CURSOR_N		= 70,
  FL_CURSOR_NE		= 69,
  FL_CURSOR_E		= 49,
  FL_CURSOR_SE		=  8,
  FL_CURSOR_S		=  9,
  FL_CURSOR_SW		=  7,
  FL_CURSOR_W		= 36,
  FL_CURSOR_NW		= 68
} Fl_Cursor;

namespace fltk3 {
  inline Fl_Cursor _3to1_cursor(Cursor k) { return (Fl_Cursor)k; }
  inline Cursor _1to3_cursor(Fl_Cursor k) { return (Cursor)k; }
}

enum {
  FL_READ   = 1,
  FL_WRITE  = 4,
  FL_EXCEPT = 8
};

namespace fltk3 {
  inline unsigned int _3to1_fd(unsigned int k) { return k; }
  inline unsigned int _1to3_fd(unsigned int k) { return k; }
}

typedef enum { 
  FL_RGB	= 0,
  FL_INDEX	= 1,
  FL_SINGLE	= 0,
  FL_DOUBLE	= 2,
  FL_ACCUM	= 4,
  FL_ALPHA	= 8,
  FL_DEPTH	= 16,
  FL_STENCIL	= 32,
  FL_RGB8	= 64,
  FL_MULTISAMPLE= 128,
  FL_STEREO     = 256,
  FL_FAKE_SINGLE = 512
} Fl_Mode;

namespace fltk3 {
  inline Fl_Mode _3to1_cursor(Mode k) { return (Fl_Mode)k; }
  inline Mode _1to3_cursor(Fl_Mode k) { return (Mode)k; }
}

#define FL_IMAGE_WITH_ALPHA 0x40000000

typedef enum {
  FL_DAMAGE_CHILD    = 0x01,
  FL_DAMAGE_EXPOSE   = 0x02,
  FL_DAMAGE_SCROLL   = 0x04,
  FL_DAMAGE_OVERLAY  = 0x08,
  FL_DAMAGE_USER1    = 0x10,
  FL_DAMAGE_USER2    = 0x20,
  FL_DAMAGE_ALL      = 0x80
} Fl_Damage;

namespace fltk3 {
  inline Fl_Damage _3to1_damage(Damage k) { return (Fl_Damage)k; }
  inline Damage _1to3_damage(Fl_Damage k) { return (Damage)k; }
}

#endif

//
// End of "$Id$".
//
