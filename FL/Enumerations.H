/
// "$Id$
/
// Enumerations for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#ifndef Fl_Enumerations_
#define Fl_Enumerations_

#  include "Fl_Export.H


/
// The FLTK version number; this is changed slightly from the beta version
// because the old "const double" definition would not allow for conditiona
// compilation..
/
// FL_VERSION is a double that describes the major and minor version numbers
// Version 1.1 is actually stored as 1.01 to allow for more than 9 mino
// releases
/
// The FL_MAJOR_VERSION, FL_MINOR_VERSION, and FL_PATCH_VERSION constant
// give the integral values for the major, minor, and patch release
// respectively
/

#define FL_MAJOR_VERSION	
#define FL_MINOR_VERSION	
#define FL_PATCH_VERSION	
#define FL_VERSION		((double)FL_MAJOR_VERSION + 
				 (double)FL_MINOR_VERSION * 0.01 + 
				 (double)FL_PATCH_VERSION * 0.0001

typedef unsigned char uchar
typedef unsigned long ulong

enum Fl_Event {	// event
  FL_NO_EVENT		= 0
  FL_PUSH		= 1
  FL_RELEASE		= 2
  FL_ENTER		= 3
  FL_LEAVE		= 4
  FL_DRAG		= 5
  FL_FOCUS		= 6
  FL_UNFOCUS		= 7
  FL_KEYDOWN		= 8
  FL_KEYUP		= 9
  FL_CLOSE		= 10
  FL_MOVE		= 11
  FL_SHORTCUT		= 12
  FL_DEACTIVATE		= 13
  FL_ACTIVATE		= 14
  FL_HIDE		= 15
  FL_SHOW		= 16
  FL_PASTE		= 17
  FL_SELECTIONCLEAR	= 18
  FL_MOUSEWHEEL		= 19
  FL_DND_ENTER		= 20
  FL_DND_DRAG		= 21
  FL_DND_LEAVE		= 22
  FL_DND_RELEASE	= 2
}
#define FL_KEYBOARD FL_KEYDOW

enum Fl_When { // Fl_Widget::when()
  FL_WHEN_NEVER		= 0
  FL_WHEN_CHANGED	= 1
  FL_WHEN_RELEASE	= 4
  FL_WHEN_RELEASE_ALWAYS= 6
  FL_WHEN_ENTER_KEY	= 8
  FL_WHEN_ENTER_KEY_ALWAYS=10
  FL_WHEN_ENTER_KEY_CHANGED=11
  FL_WHEN_NOT_CHANGED	= 2 // modifier bit to disable changed() tes
}

// Fl::event_key() and Fl::get_key(n) (use ascii letters for all other keys)
#define FL_Button	0xfee8 // use Fl_Button+FL_*_MOUS
#define FL_BackSpace	0xff0
#define FL_Tab		0xff0
#define FL_Enter	0xff0
#define FL_Pause	0xff1
#define FL_Scroll_Lock	0xff1
#define FL_Escape	0xff1
#define FL_Home		0xff5
#define FL_Left		0xff5
#define FL_Up		0xff5
#define FL_Right	0xff5
#define FL_Down		0xff5
#define FL_Page_Up	0xff5
#define FL_Page_Down	0xff5
#define FL_End		0xff5
#define FL_Print	0xff6
#define FL_Insert	0xff6
#define FL_Menu		0xff67 // the "menu/apps" key on XFree8
#define FL_Help		0xff68 // the 'help' key on Mac keyboard
#define FL_Num_Lock	0xff7
#define FL_KP		0xff80 // use FL_KP+'x' for 'x' on numeric keypa
#define FL_KP_Enter	0xff8d // same as Fl_KP+'\r
#define FL_KP_Last	0xffbd // use to range-check keypa
#define FL_F		0xffbd // use FL_F+n for function key 
#define FL_F_Last	0xffe0 // use to range-check function key
#define FL_Shift_L	0xffe
#define FL_Shift_R	0xffe
#define FL_Control_L	0xffe
#define FL_Control_R	0xffe
#define FL_Caps_Lock	0xffe
#define FL_Meta_L	0xffe7 // the left MSWindows key on XFree8
#define FL_Meta_R	0xffe8 // the right MSWindows key on XFree8
#define FL_Alt_L	0xffe
#define FL_Alt_R	0xffe
#define FL_Delete	0xfff

// Fl::event_button()
#define FL_LEFT_MOUSE	
#define FL_MIDDLE_MOUSE	
#define FL_RIGHT_MOUSE	

// Fl::event_state()
#define FL_SHIFT	0x0001000
#define FL_CAPS_LOCK	0x0002000
#define FL_CTRL		0x0004000
#define FL_ALT		0x0008000
#define FL_NUM_LOCK	0x00100000 // most X servers do this
#define FL_META		0x00400000 // correct for XFree8
#define FL_SCROLL_LOCK	0x00800000 // correct for XFree8
#define FL_BUTTON1	0x0100000
#define FL_BUTTON2	0x0200000
#define FL_BUTTON3	0x0400000
#define FL_BUTTONS	0x7f000000 // All possible button
#define FL_BUTTON(n)	(0x00800000<<(n)

#ifdef __APPLE_
#  define FL_COMMAND	FL_MET
#els
#  define FL_COMMAND	FL_CTR
#endif // __APPLE_

enum Fl_Boxtype { // boxtypes (if you change these you must fix fl_boxtype.C)
  FL_NO_BOX = 0,	FL_FLAT_BOX

  FL_UP_BOX,		FL_DOWN_BOX
  FL_UP_FRAME,		FL_DOWN_FRAME
  FL_THIN_UP_BOX,	FL_THIN_DOWN_BOX
  FL_THIN_UP_FRAME,	FL_THIN_DOWN_FRAME
  FL_ENGRAVED_BOX,	FL_EMBOSSED_BOX
  FL_ENGRAVED_FRAME,	FL_EMBOSSED_FRAME
  FL_BORDER_BOX,	_FL_SHADOW_BOX
  FL_BORDER_FRAME,	_FL_SHADOW_FRAME
  _FL_ROUNDED_BOX,	_FL_RSHADOW_BOX
  _FL_ROUNDED_FRAME,	_FL_RFLAT_BOX
  _FL_ROUND_UP_BOX,	_FL_ROUND_DOWN_BOX
  _FL_DIAMOND_UP_BOX,	_FL_DIAMOND_DOWN_BOX
  _FL_OVAL_BOX,		_FL_OSHADOW_BOX
  _FL_OVAL_FRAME,	_FL_OFLAT_BOX
  _FL_PLASTIC_UP_BOX,	_FL_PLASTIC_DOWN_BOX
  _FL_PLASTIC_UP_FRAME,	_FL_PLASTIC_DOWN_FRAME
  _FL_PLASTIC_THIN_UP_BOX,	_FL_PLASTIC_THIN_DOWN_BOX
  FL_FREE_BOXTYP
}
extern FL_EXPORT Fl_Boxtype fl_define_FL_ROUND_UP_BOX()
#define FL_ROUND_UP_BOX fl_define_FL_ROUND_UP_BOX(
#define FL_ROUND_DOWN_BOX (Fl_Boxtype)(fl_define_FL_ROUND_UP_BOX()+1
extern FL_EXPORT Fl_Boxtype fl_define_FL_SHADOW_BOX()
#define FL_SHADOW_BOX fl_define_FL_SHADOW_BOX(
#define FL_SHADOW_FRAME (Fl_Boxtype)(fl_define_FL_SHADOW_BOX()+2
extern FL_EXPORT Fl_Boxtype fl_define_FL_ROUNDED_BOX()
#define FL_ROUNDED_BOX fl_define_FL_ROUNDED_BOX(
#define FL_ROUNDED_FRAME (Fl_Boxtype)(fl_define_FL_ROUNDED_BOX()+2
extern FL_EXPORT Fl_Boxtype fl_define_FL_RFLAT_BOX()
#define FL_RFLAT_BOX fl_define_FL_RFLAT_BOX(
extern FL_EXPORT Fl_Boxtype fl_define_FL_RSHADOW_BOX()
#define FL_RSHADOW_BOX fl_define_FL_RSHADOW_BOX(
extern FL_EXPORT Fl_Boxtype fl_define_FL_DIAMOND_BOX()
#define FL_DIAMOND_UP_BOX fl_define_FL_DIAMOND_BOX(
#define FL_DIAMOND_DOWN_BOX (Fl_Boxtype)(fl_define_FL_DIAMOND_BOX()+1
extern FL_EXPORT Fl_Boxtype fl_define_FL_OVAL_BOX()
#define FL_OVAL_BOX fl_define_FL_OVAL_BOX(
#define FL_OSHADOW_BOX (Fl_Boxtype)(fl_define_FL_OVAL_BOX()+1
#define FL_OVAL_FRAME (Fl_Boxtype)(fl_define_FL_OVAL_BOX()+2
#define FL_OFLAT_BOX (Fl_Boxtype)(fl_define_FL_OVAL_BOX()+3

extern FL_EXPORT Fl_Boxtype fl_define_FL_PLASTIC_UP_BOX()
#define FL_PLASTIC_UP_BOX fl_define_FL_PLASTIC_UP_BOX(
#define FL_PLASTIC_DOWN_BOX (Fl_Boxtype)(fl_define_FL_PLASTIC_UP_BOX()+1
#define FL_PLASTIC_UP_FRAME (Fl_Boxtype)(fl_define_FL_PLASTIC_UP_BOX()+2
#define FL_PLASTIC_DOWN_FRAME (Fl_Boxtype)(fl_define_FL_PLASTIC_UP_BOX()+3
#define FL_PLASTIC_THIN_UP_BOX (Fl_Boxtype)(fl_define_FL_PLASTIC_UP_BOX()+4
#define FL_PLASTIC_THIN_DOWN_BOX (Fl_Boxtype)(fl_define_FL_PLASTIC_UP_BOX()+5

// conversions of box types to other boxtypes
extern FL_EXPORT Fl_Boxtype fl_down(Fl_Boxtype b)
inline Fl_Boxtype fl_frame(Fl_Boxtype b) {return (Fl_Boxtype)(b|2);

// back-compatability box types
#define FL_FRAME FL_ENGRAVED_FRAM
#define FL_FRAME_BOX FL_ENGRAVED_BO
#define FL_CIRCLE_BOX FL_ROUND_DOWN_BO
#define FL_DIAMOND_BOX FL_DIAMOND_DOWN_BO

enum Fl_Labeltype {	// labeltypes
  FL_NORMAL_LABEL	= 0
  FL_NO_LABEL
  _FL_SHADOW_LABEL
  _FL_ENGRAVED_LABEL
  _FL_EMBOSSED_LABEL
  _FL_MULTI_LABEL
  _FL_ICON_LABEL
  _FL_IMAGE_LABEL

  FL_FREE_LABELTYP
}
#define FL_SYMBOL_LABEL FL_NORMAL_LABE
extern Fl_Labeltype FL_EXPORT fl_define_FL_SHADOW_LABEL()
#define FL_SHADOW_LABEL fl_define_FL_SHADOW_LABEL(
extern Fl_Labeltype FL_EXPORT fl_define_FL_ENGRAVED_LABEL()
#define FL_ENGRAVED_LABEL fl_define_FL_ENGRAVED_LABEL(
extern Fl_Labeltype FL_EXPORT fl_define_FL_EMBOSSED_LABEL()
#define FL_EMBOSSED_LABEL fl_define_FL_EMBOSSED_LABEL(

enum Fl_Align {	// align() value
  FL_ALIGN_CENTER		= 0
  FL_ALIGN_TOP			= 1
  FL_ALIGN_BOTTOM		= 2
  FL_ALIGN_LEFT			= 4
  FL_ALIGN_RIGHT		= 8
  FL_ALIGN_INSIDE		= 16
  FL_ALIGN_TEXT_OVER_IMAGE	= 32
  FL_ALIGN_IMAGE_OVER_TEXT	= 0
  FL_ALIGN_CLIP			= 64
  FL_ALIGN_WRAP			= 128
  FL_ALIGN_TOP_LEFT		= FL_ALIGN_TOP | FL_ALIGN_LEFT
  FL_ALIGN_TOP_RIGHT		= FL_ALIGN_TOP | FL_ALIGN_RIGHT
  FL_ALIGN_BOTTOM_LEFT		= FL_ALIGN_BOTTOM | FL_ALIGN_LEFT
  FL_ALIGN_BOTTOM_RIGHT		= FL_ALIGN_BOTTOM | FL_ALIGN_RIGHT
  FL_ALIGN_LEFT_TOP		= FL_ALIGN_TOP_LEFT
  FL_ALIGN_RIGHT_TOP		= FL_ALIGN_TOP_RIGHT
  FL_ALIGN_LEFT_BOTTOM		= FL_ALIGN_BOTTOM_LEFT
  FL_ALIGN_RIGHT_BOTTOM		= FL_ALIGN_BOTTOM_RIGHT
  FL_ALIGN_NOWRAP		= 0 // for back compatabilit
}

enum Fl_Font {	// standard font
  FL_HELVETICA		= 0
  FL_HELVETICA_BOLD
  FL_HELVETICA_ITALIC
  FL_HELVETICA_BOLD_ITALIC
  FL_COURIER
  FL_COURIER_BOLD
  FL_COURIER_ITALIC
  FL_COURIER_BOLD_ITALIC
  FL_TIMES
  FL_TIMES_BOLD
  FL_TIMES_ITALIC
  FL_TIMES_BOLD_ITALIC
  FL_SYMBOL
  FL_SCREEN
  FL_SCREEN_BOLD
  FL_ZAPF_DINGBATS

  FL_FREE_FONT		= 16,	// first one to allocat
  FL_BOLD		= 1,	// add this to helvetica, courier, or time
  FL_ITALIC		= 2	// add this to helvetica, courier, or time
}

extern FL_EXPORT int FL_NORMAL_SIZE

enum Fl_Color {	// standard color
 
  // Features of transparent color should not be normaly plotte
  FL_TRANSPARENT_COLOR = 0

  // These are used as default colors in widgets and altered as necessar
  FL_BACKGROUND2_COLOR  = 7
  FL_INACTIVE_COLOR	= 8
  FL_FOREGROUND_COLOR   = 14
  FL_SELECTION_COLOR	= 15

  // boxtypes generally limit themselves to these colors s
  // the whole ramp is not allocated
  FL_GRAY0		= 32,	// 'A
  FL_DARK3		= 39,	// 'H
  FL_DARK2		= 45,   // 'N
  FL_DARK1		= 47,	// 'P
  FL_BACKGROUND_COLOR	= 49,	// 'R' default background colo
  FL_LIGHT1		= 50,	// 'S
  FL_LIGHT2		= 52,	// 'U
  FL_LIGHT3		= 54,	// 'W

  // FLTK provides a 5x8x5 color cube that is used with colormap visual
  FL_BLACK		= 56
  FL_RED		= 88
  FL_GREEN		= 63
  FL_YELLOW		= 95
  FL_BLUE		= 216
  FL_MAGENTA		= 248
  FL_CYAN		= 223
  FL_DARK_RED		= 72

  FL_DARK_GREEN		= 60
  FL_DARK_YELLOW	= 76
  FL_DARK_BLUE		= 136
  FL_DARK_MAGENTA	= 152
  FL_DARK_CYAN		= 140

  FL_WHITE		= 25
}

#define FL_FREE_COLOR		(Fl_Color)1
#define FL_NUM_FREE_COLOR	1
#define FL_GRAY_RAMP		(Fl_Color)3
#define FL_NUM_GRAY		2
#define FL_GRAY			FL_BACKGROUND_COLO
#define FL_COLOR_CUBE		(Fl_Color)5
#define FL_NUM_RED		
#define FL_NUM_GREEN		
#define FL_NUM_BLUE		

FL_EXPORT Fl_Color fl_inactive(Fl_Color c)
FL_EXPORT Fl_Color fl_contrast(Fl_Color fg, Fl_Color bg)
FL_EXPORT Fl_Color fl_color_average(Fl_Color c1, Fl_Color c2, float weight)
inline Fl_Color fl_lighter(Fl_Color c) { return fl_color_average(c, FL_WHITE, .67f); 
inline Fl_Color fl_darker(Fl_Color c) { return fl_color_average(c, FL_BLACK, .67f); 
inline Fl_Color fl_rgb_color(uchar r, uchar g, uchar b) 
  if (!r && !g && !b) return FL_BLACK
  else return (Fl_Color)(((((r << 8) | g) << 8) | b) << 8)

inline Fl_Color fl_rgb_color(uchar g) 
  if (!g) return FL_BLACK
  else return (Fl_Color)(((((g << 8) | g) << 8) | g) << 8)

inline Fl_Color fl_gray_ramp(int i) {return (Fl_Color)(i+FL_GRAY_RAMP);
inline Fl_Color fl_color_cube(int r, int g, int b) 
  return (Fl_Color)((b*FL_NUM_RED + r) * FL_NUM_GREEN + g + FL_COLOR_CUBE);

enum Fl_Cursor {	// standard cursor
  FL_CURSOR_DEFAULT	= 0
  FL_CURSOR_ARROW	= 35
  FL_CURSOR_CROSS	= 66
  FL_CURSOR_WAIT	= 76
  FL_CURSOR_INSERT	= 77
  FL_CURSOR_HAND	= 31
  FL_CURSOR_HELP	= 47
  FL_CURSOR_MOVE	= 27
  // fltk provides bitmaps for these
  FL_CURSOR_NS		= 78
  FL_CURSOR_WE		= 79
  FL_CURSOR_NWSE	= 80
  FL_CURSOR_NESW	= 81
  FL_CURSOR_NONE	= 255
  // for back compatability (non MSWindows ones)
  FL_CURSOR_N		= 70
  FL_CURSOR_NE		= 69
  FL_CURSOR_E		= 49
  FL_CURSOR_SE		= 8
  FL_CURSOR_S		= 9
  FL_CURSOR_SW		= 7
  FL_CURSOR_W		= 36
  FL_CURSOR_NW		= 6
  //FL_CURSOR_NS	= 22
  //FL_CURSOR_WE	= 55
}


enum { // values for "when" passed to Fl::add_fd(
  FL_READ = 1
  FL_WRITE = 4
  FL_EXCEPT = 
}

enum Fl_Mode { // visual types and Fl_Gl_Window::mode() (values match Glut
  FL_RGB	= 0
  FL_INDEX	= 1
  FL_SINGLE	= 0
  FL_DOUBLE	= 2
  FL_ACCUM	= 4
  FL_ALPHA	= 8
  FL_DEPTH	= 16
  FL_STENCIL	= 32
  FL_RGB8	= 64
  FL_MULTISAMPLE= 128
  FL_STEREO     = 256
  FL_FAKE_SINGLE = 512	// Fake single buffered windows using double-buffe
}

// damage mask

enum Fl_Damage 
  FL_DAMAGE_CHILD    = 0x01
  FL_DAMAGE_EXPOSE   = 0x02
  FL_DAMAGE_SCROLL   = 0x04
  FL_DAMAGE_OVERLAY  = 0x08
  FL_DAMAGE_USER1    = 0x10
  FL_DAMAGE_USER2    = 0x20
  FL_DAMAGE_STYLE    = 0x40
  FL_DAMAGE_ALL      = 0x8
}

enum Fl_Device_Type
  FL_XLIB_DISPLAY = 1
  FL_CARBON_DISPLAY = 2
  FL_WIN_DISPLAY = 3
  FL_PS_PRINTER = 256
  FL_GDI_PRINTER = 25
}
 

// FLTK 1.0.x compatibility definitions..
#  ifdef FLTK_1_0_COMPA
#    define contrast	fl_contras
#    define down	fl_dow
#    define frame	fl_fram
#    define inactive	fl_inactiv
#  endif // FLTK_1_0_COMPA

#endi

/
// End of "$Id$"
/
