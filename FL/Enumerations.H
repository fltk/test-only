//
// "$Id$"
//
// Enumerations for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Enumerations_H
#define Fl_Enumerations_H

#  include "Fl_Export.H"

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

/** @defgroup enumerations \<FL/Enumerations.H\>
* This module lists the enumerations provided in the <FL/Enumerations.H> header file, organized by section. Constants whose value is zero are marked with "(0)", this is often useful to know when programming.
* @{
*/


/** @name Version Numbers
* 
* The FLTK version number is stored in a number of compile-time constants:
*
*/
//@{

#define FL_MAJOR_VERSION	1  ///<  The major release number, currently 1.
#define FL_MINOR_VERSION	2  ///<  The minor release number, currently 2.
#define FL_PATCH_VERSION	0  ///<  The patch release number, currently 0.
#define FL_VERSION		((double)FL_MAJOR_VERSION + (double)FL_MINOR_VERSION * 0.01 + (double)FL_PATCH_VERSION * 0.0001) 
  ///<  A combined floating-point version number for the major, minor, and patch release numbers, currently 1.0200.

//@}

typedef unsigned char uchar;
typedef unsigned long ulong;

/** @name Events
*
* \copydoc Fl_Event
* The following events are currently defined:
*
*/

//@{

/** 
*
* Events are identified by an Fl_Event enumeration value. 
*
*/


enum Fl_Event {	// events
  FL_NO_EVENT		= 0,  ///< No event (or an event fltk does not understand) occurred (0).
  FL_PUSH		= 1,      ///< A mouse button was pushed. 
  FL_RELEASE		= 2,  ///< A mouse button was released.
  FL_ENTER		= 3,    ///< The mouse pointer entered a widget. 
  FL_LEAVE		= 4,    ///< The mouse pointer left a widget. 
  FL_DRAG		= 5,      ///< The mouse pointer was moved with a button pressed.
  FL_FOCUS		= 6,    ///< A widget should receive keyboard focus. 
  FL_UNFOCUS		= 7,  ///< A widget loses keyboard focus. 
  FL_KEYDOWN		= 8,  ///< A key was pressed.
  FL_KEYUP		= 9,    ///< A key was released. 
  FL_CLOSE		= 10,   ///< A window was closed.
  FL_MOVE		= 11,     ///< The mouse pointer was moved with no buttons pressed.
  FL_SHORTCUT		= 12, ///< The user pressed a shortcut key.
  FL_DEACTIVATE		= 13,   ///< The widget has been deactivated.
  FL_ACTIVATE		= 14,     ///< The widget has been activated. 
  FL_HIDE		= 15,         ///< The widget has been hidden. 
  FL_SHOW		= 16,         ///< The widget has been shown. 
  FL_PASTE		= 17,       ///< The widget should paste the contents of the clipboard.
  FL_SELECTIONCLEAR	= 18, ///< The widget should clear any selections made for the clipboard.
  FL_MOUSEWHEEL		= 19,   ///< The horizontal or vertical mousewheel was turned.
  FL_DND_ENTER		= 20,   ///< The mouse pointer entered a widget dragging data.
  FL_DND_DRAG		= 21,     ///< The mouse pointer was moved dragging data. 
  FL_DND_LEAVE		= 22,   ///< The mouse pointer left a widget still dragging data.
  FL_DND_RELEASE	= 23    ///< Dragged data is about to be dropped. 
};

#define FL_KEYBOARD FL_KEYDOWN
//@}

/** @name Callback "When" Conditions
*
* The conditions when callback is invoked is determined by an unsigned char value and corresponding ::Fl_When enumeration. 
* See also Fl_Widget::when().
*/
//@{

/**
* Fl_When constants determine when a callback is performed:
*/

enum Fl_When { // Fl_Widget::when():
  FL_WHEN_NEVER		= 0,          ///<  Never call the callback (0). 
  FL_WHEN_CHANGED	= 1,          ///< Do the callback only when the widget value changes. 
  FL_WHEN_NOT_CHANGED	= 2,      ///< Do the callback whenever the user interacts with the widget. 
  FL_WHEN_RELEASE	= 4,          ///< Do the callback when the button or key is released and the value changes.
  FL_WHEN_RELEASE_ALWAYS= 6,    ///< Do the callback when the user presses the ENTER key and the value changes. 
  FL_WHEN_ENTER_KEY	= 8,        ///< Do the callback when the button or key is released, even if the value doesn't change.
  FL_WHEN_ENTER_KEY_ALWAYS=10,  ///< Do the callback when the user presses the ENTER key, even if the value doesn't change. 
  FL_WHEN_ENTER_KEY_CHANGED=11, ///< Do the callback when the user presses the ENTER key, and the value changes. 
 };

//@}

/** @name Mouse button event values
*
*  The following constants define the button numbers for ::FL_PUSH  and ::FL_RELEASE events:
* See also Fl::event_button()
*/

//@{
#define FL_LEFT_MOUSE	1   ///< The left mouse button
#define FL_MIDDLE_MOUSE	2 ///< The middle mouse button
#define FL_RIGHT_MOUSE	3 ///< The right mouse button
//@}



/** @name Key event values.
*
*  The following constants define the non-ASCII keys on the keyboard for FL_KEYBOARD and FL_SHORTCUT events
*  See also Fl::event_key().
*
*/

//@{

// Fl::event_key() and Fl::get_key(n) (use ascii letters for all other keys):
#define FL_Button	0xfee8        ///< A mouse button; use Fl_Button + n for mouse button n.
#define FL_BackSpace	0xff08    ///< The backspace key. 
#define FL_Tab		0xff09        ///< The tab key.
#define FL_Enter	0xff0d        ///< The enter key.
#define FL_Pause	0xff13        ///< The pause key. 
#define FL_Scroll_Lock	0xff14  ///< The scroll lock key.
#define FL_Escape	0xff1b        ///< The escape key.
#define FL_Home		0xff50        ///< The home key.
#define FL_Left		0xff51        ///< The left arrow key.
#define FL_Up		0xff52          ///< The up arrow key.
#define FL_Right	0xff53        ///< The right arrow key.
#define FL_Down		0xff54        ///< The down arrow key.
#define FL_Page_Up	0xff55      ///< The page-up key.
#define FL_Page_Down	0xff56    ///< The page-down key.
#define FL_End		0xff57        ///< The end key.
#define FL_Print	0xff61        ///< The print (or print-screen) key.
#define FL_Insert	0xff63        ///< The insert key.
#define FL_Menu		0xff67        ///< The menu key.
 // the "menu/apps" key on XFree86
#define FL_Help		0xff68        ///< The 'help' key on Mac keyboards
#define FL_Num_Lock	0xff7f      ///< The num lock key.
#define FL_KP		0xff80          ///< One of the keypad numbers; use FL_KP + n  for number n.
#define FL_KP_Enter	0xff8d      ///< The enter key on the keypad. The same as Fl_KP+'\\r'
#define FL_KP_Last	0xffbd
 // use to range-check keypad
#define FL_F		0xffbd          ///<  One of the function keys; use FL_F + n for function key n.
 // use FL_F+n for function key n
#define FL_F_Last	0xffe0
 // use to range-check function keys
#define FL_Shift_L	0xffe1      ///< The lefthand shift key.
#define FL_Shift_R	0xffe2      ///< The righthand shift key.
#define FL_Control_L	0xffe3    ///< The lefthand control key.
#define FL_Control_R	0xffe4    ///< The righthand control key.
#define FL_Caps_Lock	0xffe5    ///< The caps lock key.
#define FL_Meta_L	0xffe7        ///< The left meta/Windows key.
#define FL_Meta_R	0xffe8        ///< The right meta/Windows key.
#define FL_Alt_L	0xffe9        ///< The left alt key.
#define FL_Alt_R	0xffea        ///< The right alt key.
#define FL_Delete	0xffff        ///< The delete key.


//@}

/** @name Fl::event_state() values
*
* The following constants define bits in the Fl::event_state()  value. 
*
*/

//@{

#define FL_SHIFT	0x00010000    ///<  One of the shift keys is down.
#define FL_CAPS_LOCK	0x00020000  ///< The caps lock is on.
#define FL_CTRL		0x00040000 ///< One of the ctrl keys is down.
#define FL_ALT		0x00080000 ///< One of the alt keys is down.
#define FL_NUM_LOCK	0x00100000 ///< The num lock is on.
// most X servers do this?
#define FL_META		0x00400000 ///< One of the meta/Windows keys is down.
// correct for XFree86
#define FL_SCROLL_LOCK	0x00800000 ///< The scroll lock is on.
// correct for XFree86
#define FL_BUTTON1	0x01000000 ///< Mouse button 1 is pushed.
#define FL_BUTTON2	0x02000000 ///< Mouse button 2 is pushed.
#define FL_BUTTON3	0x04000000 ///< Mouse button 3 is pushed.
#define FL_BUTTONS	0x7f000000 ///< Any mouse button is pushed.
#define FL_BUTTON(n)	(0x00800000<<(n)) ///<  Mouse button N (N > 0) is pushed.

#ifdef __APPLE__
#  define FL_COMMAND	FL_META ///< An alias for FL_CTRL on WIN32 and X11, or FL_META on MacOS X.
#else
#  define FL_COMMAND	FL_CTRL ///< An alias for FL_CTRL on WIN32 and X11, or FL_META on MacOS X.
#endif // __APPLE__

//@}


enum Fl_Labeltype {	// labeltypes:
  FL_NORMAL_LABEL	= 0,
  FL_NO_LABEL,
  _FL_SHADOW_LABEL,
  _FL_ENGRAVED_LABEL,
  _FL_EMBOSSED_LABEL,
  _FL_MULTI_LABEL,
  _FL_ICON_LABEL,
  _FL_IMAGE_LABEL,

  FL_FREE_LABELTYPE
};
#define FL_SYMBOL_LABEL FL_NORMAL_LABEL
extern Fl_Labeltype FL_EXPORT fl_define_FL_SHADOW_LABEL();
#define FL_SHADOW_LABEL fl_define_FL_SHADOW_LABEL()
extern Fl_Labeltype FL_EXPORT fl_define_FL_ENGRAVED_LABEL();
#define FL_ENGRAVED_LABEL fl_define_FL_ENGRAVED_LABEL()
extern Fl_Labeltype FL_EXPORT fl_define_FL_EMBOSSED_LABEL();
#define FL_EMBOSSED_LABEL fl_define_FL_EMBOSSED_LABEL()

enum Fl_Align {	// align() values
  FL_ALIGN_CENTER		= 0,
  FL_ALIGN_TOP			= 1,
  FL_ALIGN_BOTTOM		= 2,
  FL_ALIGN_LEFT			= 4,
  FL_ALIGN_RIGHT		= 8,
  FL_ALIGN_INSIDE		= 16,
  FL_ALIGN_TEXT_OVER_IMAGE	= 32,
  FL_ALIGN_IMAGE_OVER_TEXT	= 0,
  FL_ALIGN_CLIP			= 64,
  FL_ALIGN_WRAP			= 128,
  FL_ALIGN_TOP_LEFT		= FL_ALIGN_TOP | FL_ALIGN_LEFT,
  FL_ALIGN_TOP_RIGHT		= FL_ALIGN_TOP | FL_ALIGN_RIGHT,
  FL_ALIGN_BOTTOM_LEFT		= FL_ALIGN_BOTTOM | FL_ALIGN_LEFT,
  FL_ALIGN_BOTTOM_RIGHT		= FL_ALIGN_BOTTOM | FL_ALIGN_RIGHT,
  FL_ALIGN_LEFT_TOP		= FL_ALIGN_TOP_LEFT,
  FL_ALIGN_RIGHT_TOP		= FL_ALIGN_TOP_RIGHT,
  FL_ALIGN_LEFT_BOTTOM		= FL_ALIGN_BOTTOM_LEFT,
  FL_ALIGN_RIGHT_BOTTOM		= FL_ALIGN_BOTTOM_RIGHT,
  FL_ALIGN_NOWRAP		= 0 // for back compatability
};

enum Fl_Font {	// standard fonts
  FL_HELVETICA		= 0,
  FL_HELVETICA_BOLD,
  FL_HELVETICA_ITALIC,
  FL_HELVETICA_BOLD_ITALIC,
  FL_COURIER,
  FL_COURIER_BOLD,
  FL_COURIER_ITALIC,
  FL_COURIER_BOLD_ITALIC,
  FL_TIMES,
  FL_TIMES_BOLD,
  FL_TIMES_ITALIC,
  FL_TIMES_BOLD_ITALIC,
  FL_SYMBOL,
  FL_SCREEN,
  FL_SCREEN_BOLD,
  FL_ZAPF_DINGBATS,

  FL_FREE_FONT		= 16,	// first one to allocate
  FL_BOLD		= 1,	// add this to helvetica, courier, or times
  FL_ITALIC		= 2	// add this to helvetica, courier, or times
};

extern FL_EXPORT int FL_NORMAL_SIZE;


/** @name Colors
*
* Colors are either ::Fl_Color enumerations describing 8-bit indexes into a virtual colormap or 24-bit RGB color values. 
* Color indices occupy the lower 8 bits of the value, while RGB colors occupy the upper 24 bits, for a byte organization of RGBI.
*/

//@{

/**
*
* This enumeration type holds a FLTK color constants.
* They define user-specified foreground and background colors, 
* as well as specific colors and the start of the grayscale ramp and color cube in the virtual colormap. 
* Inline functions are provided to retrieve specific grayscale, color cube, or RGB color values.
*/



enum Fl_Color {	// standard colors


 
  FL_NO_COLOR = 0 /**< The transparent color (0): if color is been set to this value, things are not dwawn at all */,
  
   // These are used as default colors in widgets and altered as necessary
  FL_BACKGROUND2_COLOR  = 7, /**<  The default background color for text, list, and valuator widgets (usualy white) */
  FL_INACTIVE_COLOR	= 8, /**<  The inactive foreground color */
  FL_FOREGROUND_COLOR   = 14, /**< The default foreground color is used for labels and text. */
  FL_SELECTION_COLOR	= 15, /**< The default selection/highlight color */

  FL_GRAY0		= 32,	// 'A' 
  FL_DARK3		= 39,	// 'H'
  FL_DARK2		= 45,   // 'N'
  FL_DARK1		= 47,	// 'P'
  FL_BACKGROUND_COLOR	= 49,	// 'R' default background color
  FL_LIGHT1		= 50,	// 'S'
  FL_LIGHT2		= 52,	// 'U'
  FL_LIGHT3		= 54,	// 'W'

  // FLTK provides a 5x8x5 color cube that is used with colormap visuals

  

  FL_BLACK		= 56, /**< This and the following color constants can be used to access the colors from the FLTK standard color cube */
  FL_RED		= 88,   /**< red */
  FL_GREEN		= 63, /**< green */
  FL_YELLOW		= 95, /**< yellow */
  FL_BLUE		= 216,  /**< blue */
  FL_MAGENTA		= 248, /**< magenta */
  FL_CYAN		= 223,  /**< cyan */
  FL_DARK_RED		= 72, /**< dark red */

  FL_DARK_GREEN		= 60, /**< dark green */
  FL_DARK_YELLOW	= 76, /**< dark yellow */
  FL_DARK_BLUE		= 136, /**< dark blue */
  FL_DARK_MAGENTA	= 152,  /**< dark magenta */
  FL_DARK_CYAN		= 140,  /**< dark cyan */

  FL_WHITE		= 255 /**< white */

};



#define FL_FREE_COLOR		(Fl_Color)16
#define FL_NUM_FREE_COLOR	16
#define FL_GRAY_RAMP		(Fl_Color)32
#define FL_NUM_GRAY		24
#define FL_GRAY			FL_BACKGROUND_COLOR
#define FL_COLOR_CUBE		(Fl_Color)56
#define FL_NUM_RED		5
#define FL_NUM_GREEN		8
#define FL_NUM_BLUE		5


//@}

/** @name Color functions
* Inline functions are provided to retrieve specific grayscale, color cube, or RGB color values.
* @{
*/
FL_EXPORT Fl_Color fl_inactive(Fl_Color c);
FL_EXPORT Fl_Color fl_contrast(Fl_Color fg, Fl_Color bg);
FL_EXPORT Fl_Color fl_color_average(Fl_Color c1, Fl_Color c2, float weight);
inline Fl_Color fl_lighter(Fl_Color c) { return fl_color_average(c, FL_WHITE, .67f); }
inline Fl_Color fl_darker(Fl_Color c) { return fl_color_average(c, FL_BLACK, .67f); }
inline Fl_Color fl_rgb_color(uchar r, uchar g, uchar b) {
  if (!r && !g && !b) return FL_BLACK;
  else return (Fl_Color)(((((r << 8) | g) << 8) | b) << 8);
}
inline Fl_Color fl_rgb_color(uchar g) {
  if (!g) return FL_BLACK;
  else return (Fl_Color)(((((g << 8) | g) << 8) | g) << 8);
}
inline Fl_Color fl_gray_ramp(int i) {return (Fl_Color)(i+FL_GRAY_RAMP);}
inline Fl_Color fl_color_cube(int r, int g, int b) {
  return (Fl_Color)((b*FL_NUM_RED + r) * FL_NUM_GREEN + g + FL_COLOR_CUBE);}

//@}



/** \name Box types
* Box types, which defone appereance of widgets, ade specified either by Fl_Boxtype enumeration or an primitive function
* which returns a boxtype value.
* @{
*/

// Unfortynately we have to include Fl_Symbol for backward compatibility as the size of Fl_symbol 
// has to be known to perform many pointer operations (including comparison between boxtypes)

typedef void(Fl_Box_Draw_F)(int, int, int, int, Fl_Color);

#include "Fl_Symbol.H" 

typedef Fl_Symbol * Fl_Boxtype;

extern Fl_Boxtype

  FL_NO_BOX ,	FL_FLAT_BOX,

  FL_UP_BOX,		FL_DOWN_BOX,
  FL_UP_FRAME,		FL_DOWN_FRAME,
  FL_THIN_UP_BOX,	FL_THIN_DOWN_BOX,
  FL_THIN_UP_FRAME,	FL_THIN_DOWN_FRAME,
  FL_ENGRAVED_BOX,	FL_EMBOSSED_BOX,
  FL_ENGRAVED_FRAME,	FL_EMBOSSED_FRAME,
  FL_BORDER_BOX,	_FL_SHADOW_BOX,
  FL_BORDER_FRAME,	_FL_SHADOW_FRAME,
  _FL_ROUNDED_BOX,	_FL_RSHADOW_BOX,
  _FL_ROUNDED_FRAME,	_FL_RFLAT_BOX,
  _FL_ROUND_UP_BOX,	_FL_ROUND_DOWN_BOX,
  _FL_DIAMOND_UP_BOX,	_FL_DIAMOND_DOWN_BOX,
  _FL_OVAL_BOX,		_FL_OSHADOW_BOX,
  _FL_OVAL_FRAME,	_FL_OFLAT_BOX,
  _FL_PLASTIC_UP_BOX,	_FL_PLASTIC_DOWN_BOX,
  _FL_PLASTIC_UP_FRAME,	_FL_PLASTIC_DOWN_FRAME,
  _FL_PLASTIC_THIN_UP_BOX,	_FL_PLASTIC_THIN_DOWN_BOX,
  FL_FREE_BOXTYPE;


/*
enum Fl_Boxtype { // boxtypes (if you change these you must fix fl_boxtype.C):
  FL_NO_BOX = 0,	FL_FLAT_BOX,

  FL_UP_BOX,		FL_DOWN_BOX,
  FL_UP_FRAME,		FL_DOWN_FRAME,
  FL_THIN_UP_BOX,	FL_THIN_DOWN_BOX,
  FL_THIN_UP_FRAME,	FL_THIN_DOWN_FRAME,
  FL_ENGRAVED_BOX,	FL_EMBOSSED_BOX,
  FL_ENGRAVED_FRAME,	FL_EMBOSSED_FRAME,
  FL_BORDER_BOX,	_FL_SHADOW_BOX,
  FL_BORDER_FRAME,	_FL_SHADOW_FRAME,
  _FL_ROUNDED_BOX,	_FL_RSHADOW_BOX,
  _FL_ROUNDED_FRAME,	_FL_RFLAT_BOX,
  _FL_ROUND_UP_BOX,	_FL_ROUND_DOWN_BOX,
  _FL_DIAMOND_UP_BOX,	_FL_DIAMOND_DOWN_BOX,
  _FL_OVAL_BOX,		_FL_OSHADOW_BOX,
  _FL_OVAL_FRAME,	_FL_OFLAT_BOX,
  _FL_PLASTIC_UP_BOX,	_FL_PLASTIC_DOWN_BOX,
  _FL_PLASTIC_UP_FRAME,	_FL_PLASTIC_DOWN_FRAME,
  _FL_PLASTIC_THIN_UP_BOX,	_FL_PLASTIC_THIN_DOWN_BOX,
  FL_FREE_BOXTYPE
};

*/

/** \cond */

extern FL_EXPORT Fl_Boxtype fl_define_FL_ROUND_UP_BOX();
extern FL_EXPORT Fl_Boxtype fl_define_FL_SHADOW_BOX();
extern FL_EXPORT Fl_Boxtype fl_define_FL_ROUNDED_BOX();
extern FL_EXPORT Fl_Boxtype fl_define_FL_RFLAT_BOX();
extern FL_EXPORT Fl_Boxtype fl_define_FL_RSHADOW_BOX();
extern FL_EXPORT Fl_Boxtype fl_define_FL_DIAMOND_BOX();
extern FL_EXPORT Fl_Boxtype fl_define_FL_OVAL_BOX();
extern FL_EXPORT Fl_Boxtype fl_define_FL_PLASTIC_UP_BOX();
/** \endcond */

#define FL_ROUND_UP_BOX fl_define_FL_ROUND_UP_BOX()
#define FL_ROUND_DOWN_BOX (Fl_Boxtype)(fl_define_FL_ROUND_UP_BOX()+1)
#define FL_SHADOW_BOX fl_define_FL_SHADOW_BOX()
#define FL_SHADOW_FRAME (Fl_Boxtype)(fl_define_FL_SHADOW_BOX()+2)
#define FL_ROUNDED_BOX fl_define_FL_ROUNDED_BOX()
#define FL_ROUNDED_FRAME (Fl_Boxtype)(fl_define_FL_ROUNDED_BOX()+2)
#define FL_RFLAT_BOX fl_define_FL_RFLAT_BOX()
#define FL_RSHADOW_BOX fl_define_FL_RSHADOW_BOX()
#define FL_DIAMOND_UP_BOX fl_define_FL_DIAMOND_BOX()
#define FL_DIAMOND_DOWN_BOX (Fl_Boxtype)(fl_define_FL_DIAMOND_BOX()+1)
#define FL_OVAL_BOX fl_define_FL_OVAL_BOX()
#define FL_OSHADOW_BOX (Fl_Boxtype)(fl_define_FL_OVAL_BOX()+1)
#define FL_OVAL_FRAME (Fl_Boxtype)(fl_define_FL_OVAL_BOX()+2)
#define FL_OFLAT_BOX (Fl_Boxtype)(fl_define_FL_OVAL_BOX()+3)


#define FL_PLASTIC_UP_BOX fl_define_FL_PLASTIC_UP_BOX()
#define FL_PLASTIC_DOWN_BOX (Fl_Boxtype)(fl_define_FL_PLASTIC_UP_BOX()+1)
#define FL_PLASTIC_UP_FRAME (Fl_Boxtype)(fl_define_FL_PLASTIC_UP_BOX()+2)
#define FL_PLASTIC_DOWN_FRAME (Fl_Boxtype)(fl_define_FL_PLASTIC_UP_BOX()+3)
#define FL_PLASTIC_THIN_UP_BOX (Fl_Boxtype)(fl_define_FL_PLASTIC_UP_BOX()+4)
#define FL_PLASTIC_THIN_DOWN_BOX (Fl_Boxtype)(fl_define_FL_PLASTIC_UP_BOX()+5)


/// Converts "up" dersion of a boxtype to "down" one
extern FL_EXPORT Fl_Boxtype fl_down(Fl_Boxtype b);
/// Converts ""filled" boxtype" to "frame" version 
// inline Fl_Boxtype fl_frame(Fl_Boxtype b) {return (Fl_Boxtype)(b|2);}



// back-compatability box types:
#define FL_FRAME FL_ENGRAVED_FRAME ///< @deprecated 
#define FL_FRAME_BOX FL_ENGRAVED_BOX ///< @deprecated
#define FL_CIRCLE_BOX FL_ROUND_DOWN_BOX ///< @deprecated
#define FL_DIAMOND_BOX FL_DIAMOND_DOWN_BOX ///< @deprecated

//@}

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
  FL_MULTISAMPLE= 128,
  FL_STEREO     = 256,
  FL_FAKE_SINGLE = 512	// Fake single buffered windows using double-buffer
};

// damage masks

enum Fl_Damage {
  FL_DAMAGE_CHILD    = 0x01,
  FL_DAMAGE_EXPOSE   = 0x02,
  FL_DAMAGE_SCROLL   = 0x04,
  FL_DAMAGE_OVERLAY  = 0x08,
  FL_DAMAGE_USER1    = 0x10,
  FL_DAMAGE_USER2    = 0x20,
  FL_DAMAGE_ALL      = 0x80
};

enum Fl_Device_Type{
  FL_XLIB_DISPLAY = 1,
  FL_CARBON_DISPLAY = 2,
  FL_WIN_DISPLAY = 3,
  FL_PS_PRINTER = 256,
  FL_GDI_PRINTER = 257
};
  

// FLTK 1.0.x compatibility definitions...
#  ifdef FLTK_1_0_COMPAT
#    define contrast	fl_contrast
#    define down	fl_down
#    define frame	fl_frame
#    define inactive	fl_inactive
#  endif // FLTK_1_0_COMPAT




#endif

//@}
//
// End of "$Id$".
//
