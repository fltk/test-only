//
// "$Id: events.h,v 1.2 2002/12/10 02:00:29 easysw Exp $"
//
// Event types and data. A Widget::handle() method needs this.
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

#ifndef fltk_events_h
#define fltk_events_h

#include "FL_API.h"

namespace fltk {

enum {
  NO_EVENT	= 0,
  PUSH		= 1,
  RELEASE	= 2,
  ENTER		= 3,
  LEAVE		= 4,
  DRAG		= 5,
  FOCUS		= 6,
  UNFOCUS	= 7,
  KEY		= 8,
  KEYUP		= 9,
  MOVE		= 10,
  SHORTCUT	= 11,
  ACTIVATE	= 12,
  DEACTIVATE	= 13,
  SHOW		= 14,
  HIDE		= 15,
  MOUSEWHEEL	= 16,
  PASTE		= 17,
  DND_ENTER	= 18,
  DND_DRAG	= 19,
  DND_LEAVE	= 20,
  DND_RELEASE	= 21,
  TIMEOUT	= 22,
  TOOLTIP	= 23
};

// event_key() and get_key(n) return these values.
// The values match X11 keysym assignments
enum {
  LeftButton	= 1,
  MiddleButton	= 2,
  RightButton	= 3,
  SpaceKey	= 32,	  // same as ' '
  // 'a'-'z', '`', '-', '=', '[', ']', '\\', ',', '.', '/', ';', '\''
  BackSpaceKey	= 0xff08,
  TabKey	= 0xff09,
  ClearKey	= 0xff0b, // on some systems with NumLock off '5' produces this
  ReturnKey	= 0xff0d,
  PauseKey	= 0xff13,
  ScrollLockKey	= 0xff14,
  EscapeKey	= 0xff1b,
  HomeKey	= 0xff50,
  LeftKey	= 0xff51,
  UpKey		= 0xff52,
  RightKey	= 0xff53,
  DownKey	= 0xff54,
  PageUpKey	= 0xff55,
  PageDownKey	= 0xff56,
  EndKey	= 0xff57,
  PrintKey	= 0xff61,
  InsertKey	= 0xff63,
  MenuKey	= 0xff67,
  HelpKey	= 0xff68, // the 'help' key on Mac keyboards
  NumLockKey	= 0xff7f,
  Keypad	= 0xff80,
  KeypadEnter	= Keypad+'\r',
  MultiplyKey	= Keypad+'*',
  AddKey	= Keypad+'+',
  SubtractKey	= Keypad+'-',
  DecimalKey	= Keypad+'.',
  DivideKey	= Keypad+'/',
  Keypad0	= Keypad+'0',
  Keypad1	= Keypad+'1',
  Keypad2	= Keypad+'2',
  Keypad3	= Keypad+'3',
  Keypad4	= Keypad+'4',
  Keypad5	= Keypad+'5',
  Keypad6	= Keypad+'6',
  Keypad7	= Keypad+'7',
  Keypad8	= Keypad+'8',
  Keypad9	= Keypad+'9',
  KeypadLast	= 0xffbd, // Keypad+'=', use to range-check keypad
  F0Key		= 0xffbd,
  F1Key		= F0Key+1,
  F2Key		= F0Key+2,
  F3Key		= F0Key+3,
  F4Key		= F0Key+4,
  F5Key		= F0Key+5,
  F6Key		= F0Key+6,
  F7Key		= F0Key+7,
  F8Key		= F0Key+8,
  F9Key		= F0Key+9,
  F10Key	= F0Key+10,
  F11Key	= F0Key+11,
  F12Key	= F0Key+12,
  // use F0Key+n to get function key n
  LastFunctionKey = F0Key+35, // highest number allowed
  LeftShiftKey	= 0xffe1,
  RightShiftKey	= 0xffe2,
  LeftControlKey= 0xffe3,
  RightControlKey=0xffe4,
  CapsLockKey	= 0xffe5,
  LeftAltKey	= 0xffe9,
  RightAltKey	= 0xffea,
  LeftCommandKey= 0xffeb, // left "windows" key, XK_Super_L
  RightCommandKey=0xffec, // right "windows" key, XK_Super_L
  DeleteKey	= 0xffff
};

// event_state():
enum {
  SHIFT		= 0x00010000,
  CAPSLOCK	= 0x00020000,
  CTRL		= 0x00040000,
  ALT		= 0x00080000,
  NUMLOCK	= 0x00100000,
  COMMAND	= 0x00400000,
  SCROLLLOCK	= 0x00800000,
  BUTTON1	= 0x01000000,
  BUTTON2	= 0x02000000,
  BUTTON3	= 0x04000000,
  ANY_BUTTON	= 0x7f000000
};
inline unsigned BUTTON(int n) {return 0x00800000 << n;}

class Widget;
class Window;

////////////////////////////////////////////////////////////////
// Do not use these variables, they may not exist in future versions:

extern FL_API int e_type;
extern FL_API int e_x;
extern FL_API int e_y;
extern FL_API int e_dx;
extern FL_API int e_dy;
extern FL_API int e_x_root;
extern FL_API int e_y_root;
extern FL_API int e_state;
extern FL_API int e_clicks;
extern FL_API int e_is_click;
extern FL_API int e_keysym;
extern FL_API int e_length;
extern FL_API char* e_text;
extern FL_API int compose_state;
extern FL_API Widget* belowmouse_;
extern FL_API Widget* pushed_;
extern FL_API Widget* focus_;
extern FL_API Widget* modal_;
extern FL_API bool grab_;
extern FL_API bool exit_modal_;

////////////////////////////////////////////////////////////////

// event information:
inline int  event()			{return e_type;}
inline int  event_x()			{return e_x;}
inline int  event_y()			{return e_y;}
inline int  event_dx()			{return e_dx;}
inline int  event_dy()			{return e_dy;}
inline int  event_x_root()		{return e_x_root;}
inline int  event_y_root()		{return e_y_root;}
inline int  event_clicks()		{return e_clicks;}
inline void event_clicks(int i)		{e_clicks = i;}
inline bool event_is_click()		{return e_is_click != 0;}
inline void event_is_click(bool)	{e_is_click = 0;} // only false works!
inline int  event_state()		{return e_state;}
inline bool event_state(int i)		{return (e_state&i) != 0;}
inline int  event_key()			{return e_keysym;}
inline int  event_button()		{return e_keysym;}
FL_API bool event_key_state(int);
inline const char* event_text() 	{return e_text;}
inline int  event_length() 		{return e_length;}

// tests on current event:
FL_API bool event_inside(int,int,int,int);
FL_API bool test_shortcut(int shortcut);
FL_API const char* key_name(int key);
FL_API bool compose(int &del);
inline void compose_reset()		{compose_state = 0;}

// get current information, not info from last event:
FL_API bool get_key_state(int);
FL_API void get_mouse(int &,int &);

// event destinations:
FL_API bool handle(int, Window*);
FL_API void add_event_handler(int (*h)(int, Window*));
inline Widget* belowmouse() 		{return belowmouse_;}
FL_API void belowmouse(Widget*);
inline void belowmouse(Widget& w)	{belowmouse(&w);}
inline Widget* pushed()			{return pushed_;}
FL_API void pushed(Widget*);
inline void pushed(Widget& w)		{pushed(&w);}
inline Widget* focus()			{return focus_;}
FL_API void focus(Widget*);
inline void focus(Widget& w)		{focus(&w);}

// Modal widgets (block events going to any other widgets):
FL_API void modal(Widget*, bool grab = false);
inline Widget* modal()			{return modal_;}
inline bool grab()			{return grab_;}
inline void exit_modal()		{exit_modal_ = true;}
inline bool exit_modal_flag()		{return exit_modal_;}

// cut/paste/dnd:
FL_API void copy(const char* stuff, int len, bool clipboard = false);
FL_API void paste(Widget &receiver, bool clipboard = false);
FL_API bool dnd();

}

#endif


