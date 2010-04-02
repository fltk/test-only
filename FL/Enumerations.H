//
// "$Id$"
//
// Enumerations compatibility for the Fast Light Tool Kit (FLTK).
//

#ifndef Fltk1_Enumerations_H
#define Fltk1_Enumerations_H

#include <fltk3/Enumerations.H>

#  include "Fl_Export.H"
#  include "fl_types.h"

#if 0

#define FL_MAJOR_VERSION	3
#define FL_MINOR_VERSION	0
#define FL_PATCH_VERSION	0
#define FL_VERSION		((double)FL_MAJOR_VERSION + \
				 (double)FL_MINOR_VERSION * 0.01 + \
				 (double)FL_PATCH_VERSION * 0.0001)
#endif

enum Fl_Event {	// events
  FL_NO_EVENT		= fltk3::NO_EVENT,
  FL_PUSH		= fltk3::PUSH,
  FL_RELEASE		= fltk3::RELEASE,
  FL_ENTER		= fltk3::ENTER,
  FL_LEAVE		= fltk3::LEAVE,
  FL_DRAG		= fltk3::DRAG,
  FL_FOCUS		= fltk3::FOCUS,
  FL_UNFOCUS		= fltk3::UNFOCUS,
  FL_KEYDOWN		= fltk3::KEY,
  FL_KEYBOARD		= fltk3::KEY,
  FL_KEYUP		= fltk3::KEYUP,
  FL_CLOSE		= fltk3::CLOSE,
  FL_MOVE		= fltk3::MOVE,
  FL_SHORTCUT		= fltk3::SHORTCUT,
  FL_DEACTIVATE		= fltk3::DEACTIVATE,
  FL_ACTIVATE		= fltk3::ACTIVATE,
  FL_HIDE		= fltk3::HIDE,
  FL_SHOW		= fltk3::SHOW,
  FL_PASTE		= fltk3::PASTE,
  FL_SELECTIONCLEAR	= fltk3::SELECTIONCLEAR,
  FL_MOUSEWHEEL		= fltk3::MOUSEWHEEL,
  FL_DND_ENTER		= fltk3::DND_ENTER,
  FL_DND_DRAG		= fltk3::DND_DRAG,
  FL_DND_LEAVE		= fltk3::DND_LEAVE,
  FL_DND_RELEASE	= fltk3::DND_RELEASE
};

enum Fl_When {
  FL_WHEN_NEVER             = fltk3::WHEN_NEVER,
  FL_WHEN_CHANGED           = fltk3::WHEN_CHANGED,
  FL_WHEN_NOT_CHANGED       = fltk3::WHEN_NOT_CHANGED,
  FL_WHEN_RELEASE           = fltk3::WHEN_RELEASE,
  FL_WHEN_RELEASE_ALWAYS    = fltk3::WHEN_RELEASE_ALWAYS,
  FL_WHEN_ENTER_KEY         = fltk3::WHEN_ENTER_KEY,
  FL_WHEN_ENTER_KEY_ALWAYS  = fltk3::WHEN_ENTER_KEY_ALWAYS,
  FL_WHEN_ENTER_KEY_CHANGED = fltk3::WHEN_ENTER_KEY_CHANGED
};

#if 0

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

#define FL_LEFT_MOUSE	1
#define FL_MIDDLE_MOUSE	2
#define FL_RIGHT_MOUSE	3

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
#define FL_BUTTON(n)	(0x00800000<<(n))

#define FL_KEY_MASK 0x0000ffff

#ifdef __APPLE__
#  define FL_COMMAND  FL_META
#  define FL_CONTROL  FL_CTRL
#else
#  define FL_COMMAND  FL_CTRL
#  define FL_CONTROL  FL_META
#endif

#endif

enum Fl_Boxtype {
  FL_NO_BOX                 = fltk3::NO_BOX,
  FL_FLAT_BOX               = fltk3::FLAT_BOX,
  FL_UP_BOX                 = fltk3::UP_BOX,
  FL_DOWN_BOX               = fltk3::DOWN_BOX,
  FL_UP_FRAME               = fltk3::UP_FRAME,
  FL_DOWN_FRAME             = fltk3::DOWN_FRAME,
  FL_THIN_UP_BOX            = fltk3::THIN_UP_BOX,
  FL_THIN_DOWN_BOX          = fltk3::THIN_DOWN_BOX,
  FL_THIN_UP_FRAME          = fltk3::THIN_UP_FRAME,
  FL_THIN_DOWN_FRAME        = fltk3::THIN_DOWN_FRAME,
  FL_ENGRAVED_BOX           = fltk3::ENGRAVED_BOX,
  FL_EMBOSSED_BOX           = fltk3::EMBOSSED_BOX,
  FL_ENGRAVED_FRAME         = fltk3::ENGRAVED_FRAME,
  FL_EMBOSSED_FRAME         = fltk3::EMBOSSED_FRAME,
  FL_BORDER_BOX             = fltk3::BORDER_BOX,
  FL_SHADOW_BOX             = fltk3::SHADOW_BOX,
  FL_BORDER_FRAME           = fltk3::BORDER_FRAME,
  FL_SHADOW_FRAME           = fltk3::SHADOW_FRAME,
  FL_ROUNDED_BOX            = fltk3::ROUNDED_BOX,
  FL_RSHADOW_BOX            = fltk3::RSHADOW_BOX,
  FL_ROUNDED_FRAME          = fltk3::ROUNDED_FRAME,
  FL_RFLAT_BOX              = fltk3::RFLAT_BOX,
  FL_ROUND_UP_BOX           = fltk3::ROUND_UP_BOX,
  FL_ROUND_DOWN_BOX         = fltk3::ROUND_DOWN_BOX,
  FL_DIAMOND_UP_BOX         = fltk3::DIAMOND_UP_BOX,
  FL_DIAMOND_DOWN_BOX       = fltk3::DIAMOND_DOWN_BOX,
  FL_OVAL_BOX               = fltk3::OVAL_BOX,
  FL_OSHADOW_BOX            = fltk3::OSHADOW_BOX,
  FL_OVAL_FRAME             = fltk3::OVAL_FRAME,
  FL_OFLAT_BOX              = fltk3::OFLAT_BOX,
  FL_PLASTIC_UP_BOX         = fltk3::PLASTIC_UP_BOX,
  FL_PLASTIC_DOWN_BOX       = fltk3::PLASTIC_DOWN_BOX,
  FL_PLASTIC_UP_FRAME       = fltk3::PLASTIC_UP_FRAME,
  FL_PLASTIC_DOWN_FRAME     = fltk3::PLASTIC_DOWN_FRAME,
  FL_PLASTIC_THIN_UP_BOX    = fltk3::PLASTIC_THIN_UP_BOX,
  FL_PLASTIC_THIN_DOWN_BOX  = fltk3::PLASTIC_THIN_DOWN_BOX,
  FL_PLASTIC_ROUND_UP_BOX   = fltk3::PLASTIC_ROUND_UP_BOX,
  FL_PLASTIC_ROUND_DOWN_BOX = fltk3::PLASTIC_ROUND_DOWN_BOX,
  FL_GTK_UP_BOX             = fltk3::GTK_UP_BOX,
  FL_GTK_DOWN_BOX           = fltk3::GTK_DOWN_BOX,
  FL_GTK_UP_FRAME           = fltk3::GTK_UP_FRAME,
  FL_GTK_DOWN_FRAME         = fltk3::GTK_DOWN_FRAME,
  FL_GTK_THIN_UP_BOX        = fltk3::GTK_THIN_UP_BOX,
  FL_GTK_THIN_DOWN_BOX      = fltk3::GTK_THIN_DOWN_BOX,
  FL_GTK_THIN_UP_FRAME      = fltk3::GTK_THIN_UP_FRAME,
  FL_GTK_THIN_DOWN_FRAME    = fltk3::GTK_THIN_DOWN_FRAME,
  FL_GTK_ROUND_UP_BOX       = fltk3::GTK_ROUND_UP_BOX,
  FL_GTK_ROUND_DOWN_BOX     = fltk3::GTK_ROUND_DOWN_BOX,
  FL_FREE_BOXTYPE           = fltk3::FREE_BOXTYPE
};

inline Fl_Boxtype fl_box(Fl_Boxtype b) {
  return (Fl_Boxtype)((b<FL_UP_BOX||b%4>1)?b:(b-2));
}
inline Fl_Boxtype fl_down(Fl_Boxtype b) {
  return (Fl_Boxtype)((b<FL_UP_BOX)?b:(b|1));
}
inline Fl_Boxtype fl_frame(Fl_Boxtype b) {
  return (Fl_Boxtype)((b%4<2)?b:(b+2));
}

enum Fl_Labeltype {
  FL_NORMAL_LABEL     = fltk3::NORMAL_LABEL, // consider RAW label
  FL_SYMBOL_LABEL     = fltk3::NORMAL_LABEL,
  FL_NO_LABEL         = fltk3::NO_LABEL,
  FL_SHADOW_LABEL     = fltk3::SHADOW_LABEL,
  FL_ENGRAVED_LABEL   = fltk3::ENGRAVED_LABEL,
  FL_EMBOSSED_LABEL   = fltk3::EMBOSSED_LABEL,
  FL_MULTI_LABEL      = fltk3::MULTI_LABEL,
  FL_ICON_LABEL       = fltk3::ICON_LABEL,
  FL_IMAGE_LABEL      = fltk3::IMAGE_LABEL,
  FL_FREE_LABELTYPE   = fltk3::FREE_LABELTYPE
};

typedef unsigned Fl_Align;
const Fl_Align FL_ALIGN_CENTER		= (Fl_Align)fltk3::ALIGN_CENTER;
const Fl_Align FL_ALIGN_TOP		= (Fl_Align)fltk3::ALIGN_TOP;
const Fl_Align FL_ALIGN_BOTTOM		= (Fl_Align)fltk3::ALIGN_BOTTOM;
const Fl_Align FL_ALIGN_LEFT		= (Fl_Align)fltk3::ALIGN_LEFT;
const Fl_Align FL_ALIGN_RIGHT		= (Fl_Align)fltk3::ALIGN_RIGHT;
const Fl_Align FL_ALIGN_INSIDE		= (Fl_Align)fltk3::ALIGN_INSIDE;
const Fl_Align FL_ALIGN_TEXT_OVER_IMAGE	= (Fl_Align)fltk3::ALIGN_TEXT_OVER_IMAGE;
const Fl_Align FL_ALIGN_IMAGE_OVER_TEXT	= (Fl_Align)fltk3::ALIGN_IMAGE_OVER_TEXT;
const Fl_Align FL_ALIGN_CLIP		= (Fl_Align)fltk3::ALIGN_CLIP;
const Fl_Align FL_ALIGN_WRAP		= (Fl_Align)fltk3::ALIGN_WRAP;
const Fl_Align FL_ALIGN_TOP_LEFT	= (Fl_Align)fltk3::ALIGN_TOP_LEFT;
const Fl_Align FL_ALIGN_TOP_RIGHT	= (Fl_Align)fltk3::ALIGN_TOP_RIGHT;
const Fl_Align FL_ALIGN_BOTTOM_LEFT	= (Fl_Align)fltk3::ALIGN_BOTTOM_LEFT;
const Fl_Align FL_ALIGN_BOTTOM_RIGHT	= (Fl_Align)fltk3::ALIGN_BOTTOM_RIGHT;
const Fl_Align FL_ALIGN_LEFT_TOP	= (Fl_Align)fltk3::ALIGN_LEFT_TOP;
const Fl_Align FL_ALIGN_RIGHT_TOP	= (Fl_Align)fltk3::ALIGN_RIGHT_TOP;
const Fl_Align FL_ALIGN_LEFT_BOTTOM	= (Fl_Align)fltk3::ALIGN_LEFT_BOTTOM;
const Fl_Align FL_ALIGN_RIGHT_BOTTOM	= (Fl_Align)fltk3::ALIGN_RIGHT_BOTTOM;
const Fl_Align FL_ALIGN_NOWRAP		= (Fl_Align)fltk3::ALIGN_NOWRAP;

typedef int Fl_Font;
const Fl_Font FL_HELVETICA              = fltk3::HELVETICA;
const Fl_Font FL_HELVETICA_BOLD         = fltk3::HELVETICA_BOLD;
const Fl_Font FL_HELVETICA_ITALIC       = fltk3::HELVETICA_ITALIC;
const Fl_Font FL_HELVETICA_BOLD_ITALIC  = fltk3::HELVETICA_BOLD_ITALIC;
const Fl_Font FL_COURIER                = fltk3::COURIER;
const Fl_Font FL_COURIER_BOLD           = fltk3::COURIER_BOLD;
const Fl_Font FL_COURIER_ITALIC         = fltk3::COURIER_ITALIC;
const Fl_Font FL_COURIER_BOLD_ITALIC    = fltk3::COURIER_BOLD_ITALIC;
const Fl_Font FL_TIMES                  = fltk3::TIMES;
const Fl_Font FL_TIMES_BOLD             = fltk3::TIMES_BOLD;
const Fl_Font FL_TIMES_ITALIC           = fltk3::TIMES_ITALIC;
const Fl_Font FL_TIMES_BOLD_ITALIC      = fltk3::TIMES_BOLD_ITALIC;
const Fl_Font FL_SYMBOL                 = fltk3::SYMBOL_FONT;
const Fl_Font FL_SCREEN                 = fltk3::SCREEN_FONT;
const Fl_Font FL_SCREEN_BOLD            = fltk3::SCREEN_BOLD_FONT;
const Fl_Font FL_ZAPF_DINGBATS          = fltk3::ZAPF_DINGBATS;

const Fl_Font FL_FREE_FONT              = fltk3::FREE_FONT;
const Fl_Font FL_BOLD                   = fltk3::BOLD;
const Fl_Font FL_ITALIC                 = fltk3::ITALIC;
const Fl_Font FL_BOLD_ITALIC            = fltk3::BOLD_ITALIC;

#if 0

typedef int Fl_Fontsize;
extern FL_EXPORT Fl_Fontsize FL_NORMAL_SIZE;

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
  return (Fl_Color)((b*FL_NUM_RED + r) * FL_NUM_GREEN + g + FL_COLOR_CUBE);
}

enum Fl_Cursor {	// standard cursors
  FL_CURSOR_DEFAULT	= 0,
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
  FL_CURSOR_NONE	= 255,
  FL_CURSOR_N		= 70,
  FL_CURSOR_NE		= 69,
  FL_CURSOR_E		= 49,
  FL_CURSOR_SE		= 8,
  FL_CURSOR_S		= 9,
  FL_CURSOR_SW		= 7,
  FL_CURSOR_W		= 36,
  FL_CURSOR_NW		= 68
};

enum {
  FL_READ = 1,
  FL_WRITE = 4,
  FL_EXCEPT = 8
};

enum Fl_Mode {
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
};

#define FL_IMAGE_WITH_ALPHA 0x40000000

enum Fl_Damage {
  FL_DAMAGE_CHILD    = 0x01,
  FL_DAMAGE_EXPOSE   = 0x02,
  FL_DAMAGE_SCROLL   = 0x04,
  FL_DAMAGE_OVERLAY  = 0x08,
  FL_DAMAGE_USER1    = 0x10,
  FL_DAMAGE_USER2    = 0x20,
  FL_DAMAGE_ALL      = 0x80
};

#endif

#endif

//
// End of "$Id$".
//
