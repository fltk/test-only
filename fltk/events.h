//
// "$Id: events.h 6514 2008-11-10 21:10:13Z spitzak $"
//

#ifndef fltk2_events_h
#define fltk2_events_h

#include "FL_API.h"

#include <fltk3/Enumerations.H>

namespace fltk {

enum {
  NO_EVENT	= fltk3::NO_EVENT,
  PUSH		= fltk3::PUSH,
  RELEASE	= fltk3::RELEASE,
  ENTER		= fltk3::ENTER,
  LEAVE		= fltk3::LEAVE,
  DRAG		= fltk3::DRAG,
  FOCUS		= fltk3::FOCUS,
  UNFOCUS	= fltk3::UNFOCUS,
  KEY		= fltk3::KEY,
  KEYUP		= fltk3::KEYUP,
  FOCUS_CHANGE	= fltk3::FOCUS_CHANGE,
  MOVE		= fltk3::MOVE,
  SHORTCUT	= fltk3::SHORTCUT,
  DEACTIVATE	= fltk3::DEACTIVATE,
  ACTIVATE	= fltk3::ACTIVATE,
  HIDE		= fltk3::HIDE,
  SHOW		= fltk3::SHOW,
  PASTE		= fltk3::PASTE,
  TIMEOUT	= fltk3::TIMEOUT,
  MOUSEWHEEL	= fltk3::MOUSEWHEEL,
  DND_ENTER	= fltk3::DND_ENTER,
  DND_DRAG	= fltk3::DND_DRAG,
  DND_LEAVE	= fltk3::DND_LEAVE,
  DND_RELEASE	= fltk3::DND_RELEASE,
  TOOLTIP	= fltk3::TOOLTIP
};

enum {
  LeftButton	= 1,
  MiddleButton	= 2,
  RightButton	= 3,
  SpaceKey	= 32,
  BackSpaceKey	= 0xff08,
  TabKey	= 0xff09,
  ClearKey	= 0xff0b,
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
  HelpKey	= 0xff68,
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
  KeypadLast	= 0xffbd,
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
  F10Key	= F0Key+10
  F11Key	= F0Key+11
  F12Key	= F0Key+12
  LastFunctionKey = F0Key+35,
  LeftShiftKey	= 0xffe1,
  RightShiftKey	= 0xffe2,
  LeftCtrlKey	= 0xffe3,
  RightCtrlKey	= 0xffe4,
  CapsLockKey	= 0xffe5,
  LeftMetaKey	= 0xffe7,
  RightMetaKey	= 0xffe8,
  LeftAltKey	= 0xffe9,
  RightAltKey	= 0xffea,
  DeleteKey	= 0xffff,
#if  defined(__APPLE__)
  LeftAccKey	= LeftCtrlKey,
  RightAccKey	= RightCtrlKey,
  LeftCmdKey	= LeftMetaKey,
  RightCmdKey	= RightMetaKey
#else
  LeftAccKey	= LeftAltKey,
  RightAccKey	= RightAltKey,
  LeftCmdKey	= LeftCtrlKey,
  RightCmdKey	= RightCtrlKey
#endif
};

enum {
  SHIFT		= 0x00010000,
  CAPSLOCK	= 0x00020000,
  CTRL		= 0x00040000,
  ALT		= 0x00080000,
  NUMLOCK	= 0x00100000,
  META		= 0x00400000,
  SCROLLLOCK	= 0x00800000,
  BUTTON1	= 0x01000000,
  BUTTON2	= 0x02000000,
  BUTTON3	= 0x04000000,
  ANY_BUTTON	= 0x7f000000,
#if defined(__APPLE__)
  ACCELERATOR	= CTRL,
  OPTION	= ALT,
  COMMAND	= META
#else
  ACCELERATOR	= ALT,
  COMMAND	= CTRL,
  OPTION	= ALT|META
#endif
};

inline unsigned BUTTON(int n) {return 0x00800000 << n;}

enum {
  DEVICE_MOUSE    = 0,
  DEVICE_STYLUS   = 1,
  DEVICE_ERASER   = 2,
  DEVICE_CURSOR   = 3,
  DEVICE_AIRBRUSH = 4,
  DEVICE_TOUCH    = 5
};

class Rectangle;
class Widget;
class Window;

extern FL_API int e_type;
extern FL_API int e_x;
extern FL_API int e_y;
extern FL_API int e_dx;
extern FL_API int e_dy;
extern FL_API int e_x_root;
extern FL_API int e_y_root;
extern FL_API unsigned e_state;
extern FL_API int e_clicks;
extern FL_API unsigned e_is_click;
extern FL_API unsigned e_keysym;
extern FL_API unsigned e_length;
extern FL_API const char* e_text;
extern FL_API unsigned e_key_repeated;
extern FL_API float e_pressure;
extern FL_API float e_x_tilt;
extern FL_API float e_y_tilt;
extern FL_API int e_device;
extern FL_API int compose_state;
extern FL_API Widget* belowmouse_;
extern FL_API Widget* pushed_;
extern FL_API Widget* focus_;
extern FL_API Widget* modal_;
extern FL_API bool grab_;
extern FL_API bool exit_modal_;

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
inline unsigned event_state()		{return e_state;}
inline bool event_state(unsigned i)	{return (e_state&i) != 0;}
inline unsigned event_key()		{return e_keysym;}
inline unsigned event_button()		{return e_keysym;}
FL_API bool event_key_state(unsigned);
inline const char* event_text() 	{return e_text;}
inline unsigned event_length() 		{return e_length;}
inline unsigned event_key_repeated()	{return e_key_repeated;}
inline float event_pressure()	 	{return e_pressure;}
inline float event_x_tilt()     	{return e_x_tilt;}
inline float event_y_tilt()     	{return e_y_tilt;}
inline int  event_device()      	{return e_device;}

FL_API bool event_inside(const Rectangle&);
FL_API bool compose(int &del);
inline void compose_reset()		{compose_state = 0;}

FL_API bool try_shortcut();
FL_API const char* key_name(unsigned key);
FL_API unsigned key(const char* name);

class FL_API ShortcutFunctor {
 public:
  virtual bool handle(const Widget*, unsigned key) = 0;
};
FL_API unsigned foreachShortcut(const Widget*, ShortcutFunctor&);
inline unsigned foreachShortcut(ShortcutFunctor& f) { return foreachShortcut(0,f); }

FL_API bool get_key_state(unsigned);
FL_API void get_mouse(int &,int &);
FL_API bool warp_mouse(int, int);

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

FL_API void copy(const char* stuff, int len, bool clipboard = false);
FL_API void paste(Widget &receiver, bool clipboard = false);
FL_API bool dnd();

FL_API void modal(Widget*, bool grab = false);
inline Widget* modal()			{return modal_;}
inline bool grab()			{return grab_;}
inline void exit_modal()		{exit_modal_ = true;}
inline bool exit_modal_flag()		{return exit_modal_;}

const char  *event_name(int event); /// see STR #508

}

#endif

//
// $Id: events.h 6514 2008-11-10 21:10:13Z spitzak $
//
