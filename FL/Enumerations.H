//
// "$Id: Enumerations.H,v 1.27 1999/08/16 07:30:47 bill Exp $"
//
// Enumerations for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Enumerations_H
#define Fl_Enumerations_H

//
// The following is only used when building DLLs under WIN32 with Visual C++...
//

#if defined(FL_DLL) && defined(_MSC_VER)
#  ifdef FL_LIBRARY
#    define FL_EXPORT	__declspec(dllexport)
#  else
#    define FL_EXPORT	__declspec(dllimport)
#  endif /* FL_LIBRARY */
#else
#  define FL_EXPORT
#endif /* FL_DLL && _MSC_VER */

//
// The FLTK version number; this is changed slightly from the beta versions
// because the old "const double" definition would not allow for conditional
// compilation...
//
// FL_VERSION is a double that describes the major and minor version numbers.
// Version 1.1 is actually stored as 1.01 to allow for more than 9 minor
// releases.
//
// The FL_MAJOR_VERSION, FL_MINOR_VERSION, and FL_PATCH_VERSION constants
// give the integral values for the major, minor, and patch releases
// respectively.
//

#define FL_MAJOR_VERSION	2
#define FL_MINOR_VERSION	0
#define FL_PATCH_VERSION	0
#define FL_VERSION		((double)FL_MAJOR_VERSION + \
				 (double)FL_MINOR_VERSION * 0.01)

typedef unsigned char uchar;
typedef unsigned long ulong;

enum Fl_Event {	// events
  FL_NO_EVENT		= 0,
  FL_PUSH		= 1,
  FL_RELEASE		= 2,
  FL_ENTER		= 3,
  FL_LEAVE		= 4,
  FL_DRAG		= 5,
  FL_FOCUS		= 6,
  FL_UNFOCUS		= 7,
  FL_KEYBOARD		= 8,
  FL_CLOSE		= 9,
  FL_MOVE		= 10,
  FL_SHORTCUT		= 11,
  FL_DEACTIVATE		= 13,
  FL_ACTIVATE		= 14,
  FL_HIDE		= 15,
  FL_SHOW		= 16,
  FL_PASTE		= 17,
  FL_SELECTIONCLEAR	= 18
};

enum Fl_When { // Fl_Widget::when():
  FL_WHEN_NEVER		= 0,
  FL_WHEN_CHANGED	= 1,
  FL_WHEN_RELEASE	= 4,
  FL_WHEN_RELEASE_ALWAYS= 6,
  FL_WHEN_ENTER_KEY	= 8,
  FL_WHEN_ENTER_KEY_ALWAYS=10,
  FL_WHEN_ENTER_KEY_CHANGED=11,
  FL_WHEN_NOT_CHANGED	= 2 // modifier bit to disable changed() test
};

// Fl::event_key() and Fl::get_key(n) (use ascii letters for all other keys):
#define FL_Button	0xfee8 // use Fl_Button+n for mouse button n
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
#define FL_Menu		0xff67 // the "menu/apps" key on XFree86
#define FL_Num_Lock	0xff7f
#define FL_KP		0xff80 // use FL_KP+'x' for 'x' on numeric keypad
#define FL_KP_Enter	0xff8d // same as Fl_KP+'\r'
#define FL_KP_Last	0xffbd // use to range-check keypad
#define FL_F		0xffbd // use FL_F+n for function key n
#define FL_F_Last	0xffe0 // use to range-check function keys
#define FL_Shift_L	0xffe1
#define FL_Shift_R	0xffe2
#define FL_Control_L	0xffe3
#define FL_Control_R	0xffe4
#define FL_Caps_Lock	0xffe5
#define FL_Meta_L	0xffe7 // the left MSWindows key on XFree86
#define FL_Meta_R	0xffe8 // the right MSWindows key on XFree86
#define FL_Alt_L	0xffe9
#define FL_Alt_R	0xffea
#define FL_Delete	0xffff

// Fl::event_state():
#define FL_SHIFT	0x00010000
#define FL_CAPS_LOCK	0x00020000
#define FL_CTRL		0x00040000
#define FL_ALT		0x00080000
#define FL_NUM_LOCK	0x00100000 // most X servers do this?
#define FL_META		0x00400000 // correct for XFree86
#define FL_SCROLL_LOCK	0x00800000 // correct for XFree86
#define FL_BUTTON1	0x01000000
#define FL_BUTTON2	0x02000000
#define FL_BUTTON3	0x04000000

enum Fl_Cursor {	// standard cursors
  FL_CURSOR_DEFAULT	= 0,
  FL_CURSOR_ARROW	= 35,
  FL_CURSOR_CROSS	= 66,
  FL_CURSOR_WAIT	= 76,
  FL_CURSOR_INSERT	= 77,
  FL_CURSOR_HAND	= 31,
  FL_CURSOR_HELP	= 47,
  FL_CURSOR_MOVE	= 27,
  // fltk provides bitmaps for these:
  FL_CURSOR_NS		= 78,
  FL_CURSOR_WE		= 79,
  FL_CURSOR_NWSE	= 80,
  FL_CURSOR_NESW	= 81,
  FL_CURSOR_NONE	= 255,
  // for back compatability (non MSWindows ones):
  FL_CURSOR_N		= 70,
  FL_CURSOR_NE		= 69,
  FL_CURSOR_E		= 49,
  FL_CURSOR_SE		= 8,
  FL_CURSOR_S		= 9,
  FL_CURSOR_SW		= 7,
  FL_CURSOR_W		= 36,
  FL_CURSOR_NW		= 68
  //FL_CURSOR_NS	= 22,
  //FL_CURSOR_WE	= 55,
};

enum { // values for "when" passed to Fl::add_fd()
  FL_READ = 1,
  FL_WRITE = 4,
  FL_EXCEPT = 8
};

enum Fl_Mode { // visual types and Fl_Gl_Window::mode() (values match Glut)
  FL_RGB	= 0,
  FL_INDEX	= 1,
  FL_SINGLE	= 0,
  FL_DOUBLE	= 2,
  FL_ACCUM	= 4,
  FL_ALPHA	= 8,
  FL_DEPTH	= 16,
  FL_STENCIL	= 32,
  FL_RGB8	= 64,
  FL_MULTISAMPLE= 128
};

// damage masks

enum Fl_Damage {
  FL_DAMAGE_CHILD       = 0x01,
  FL_DAMAGE_EXPOSE      = 0x02,
  FL_DAMAGE_SCROLL      = 0x04,
  FL_DAMAGE_OVERLAY     = 0x08,
  FL_DAMAGE_CHILD_LABEL = 0x10,
  FL_DAMAGE_LAYOUT	= 0x20,
  FL_DAMAGE_ALL         = 0x80
};

#endif

//
// End of "$Id: Enumerations.H,v 1.27 1999/08/16 07:30:47 bill Exp $".
//
