//
// "$Id$"
//
// MacOS keyboard state routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

// Return the current state of a key.  Keys are named by fltk symbols,
// which are actually X keysyms.  So this has to translate to macOS
// symbols.

#include <fltk3/run.h>
#include <fltk3/x.h>
#include <config.h>

// The list of Mac OS virtual keycodes appears with OS 10.5 in
// ...../Carbon.framework/Frameworks/HIToolbox.framework/Headers/Events.h
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
#include <Carbon/Carbon.h>
#else
/*
 *    These constants are the virtual keycodes defined originally in
 *    Inside Mac Volume V, pg. V-191. They identify physical keys on a
 *    keyboard. Those constants with "ANSI" in the name are labeled
 *    according to the key position on an ANSI-standard US keyboard.
 *    For example, kVK_ANSI_A indicates the virtual keycode for the key
 *    with the letter 'A' in the US keyboard layout. Other keyboard
 *    layouts may have the 'A' key label on a different physical key;
 *    in this case, pressing 'A' will generate a different virtual
 *    keycode.
 */
enum {
  kVK_ANSI_A                    = 0x00,
  kVK_ANSI_S                    = 0x01,
  kVK_ANSI_D                    = 0x02,
  kVK_ANSI_F                    = 0x03,
  kVK_ANSI_H                    = 0x04,
  kVK_ANSI_G                    = 0x05,
  kVK_ANSI_Z                    = 0x06,
  kVK_ANSI_X                    = 0x07,
  kVK_ANSI_C                    = 0x08,
  kVK_ANSI_V                    = 0x09,
  kVK_ANSI_B                    = 0x0B,
  kVK_ANSI_Q                    = 0x0C,
  kVK_ANSI_W                    = 0x0D,
  kVK_ANSI_E                    = 0x0E,
  kVK_ANSI_R                    = 0x0F,
  kVK_ANSI_Y                    = 0x10,
  kVK_ANSI_T                    = 0x11,
  kVK_ANSI_1                    = 0x12,
  kVK_ANSI_2                    = 0x13,
  kVK_ANSI_3                    = 0x14,
  kVK_ANSI_4                    = 0x15,
  kVK_ANSI_6                    = 0x16,
  kVK_ANSI_5                    = 0x17,
  kVK_ANSI_Equal                = 0x18,
  kVK_ANSI_9                    = 0x19,
  kVK_ANSI_7                    = 0x1A,
  kVK_ANSI_Minus                = 0x1B,
  kVK_ANSI_8                    = 0x1C,
  kVK_ANSI_0                    = 0x1D,
  kVK_ANSI_RightBracket         = 0x1E,
  kVK_ANSI_O                    = 0x1F,
  kVK_ANSI_U                    = 0x20,
  kVK_ANSI_LeftBracket          = 0x21,
  kVK_ANSI_I                    = 0x22,
  kVK_ANSI_P                    = 0x23,
  kVK_ANSI_L                    = 0x25,
  kVK_ANSI_J                    = 0x26,
  kVK_ANSI_Quote                = 0x27,
  kVK_ANSI_K                    = 0x28,
  kVK_ANSI_Semicolon            = 0x29,
  kVK_ANSI_Backslash            = 0x2A,
  kVK_ANSI_Comma                = 0x2B,
  kVK_ANSI_Slash                = 0x2C,
  kVK_ANSI_N                    = 0x2D,
  kVK_ANSI_M                    = 0x2E,
  kVK_ANSI_Period               = 0x2F,
  kVK_ANSI_Grave                = 0x32,
  kVK_ANSI_KeypadDecimal        = 0x41,
  kVK_ANSI_KeypadMultiply       = 0x43,
  kVK_ANSI_KeypadPlus           = 0x45,
  kVK_ANSI_KeypadClear          = 0x47,
  kVK_ANSI_KeypadDivide         = 0x4B,
  kVK_ANSI_KeypadEnter          = 0x4C,
  kVK_ANSI_KeypadMinus          = 0x4E,
  kVK_ANSI_KeypadEquals         = 0x51,
  kVK_ANSI_Keypad0              = 0x52,
  kVK_ANSI_Keypad1              = 0x53,
  kVK_ANSI_Keypad2              = 0x54,
  kVK_ANSI_Keypad3              = 0x55,
  kVK_ANSI_Keypad4              = 0x56,
  kVK_ANSI_Keypad5              = 0x57,
  kVK_ANSI_Keypad6              = 0x58,
  kVK_ANSI_Keypad7              = 0x59,
  kVK_ANSI_Keypad8              = 0x5B,
  kVK_ANSI_Keypad9              = 0x5C
};

/* keycodes for keys that are independent of keyboard layout*/
enum {
  kVK_Return                    = 0x24,
  kVK_Tab                       = 0x30,
  kVK_Space                     = 0x31,
  kVK_Delete                    = 0x33,
  kVK_Escape                    = 0x35,
  kVK_Command                   = 0x37,
  kVK_Shift                     = 0x38,
  kVK_CapsLock                  = 0x39,
  kVK_Option                    = 0x3A,
  kVK_Control                   = 0x3B,
  kVK_RightShift                = 0x3C,
  kVK_RightOption               = 0x3D,
  kVK_RightControl              = 0x3E,
  kVK_Function                  = 0x3F,
  kVK_F17                       = 0x40,
  kVK_VolumeUp                  = 0x48,
  kVK_VolumeDown                = 0x49,
  kVK_Mute                      = 0x4A,
  kVK_F18                       = 0x4F,
  kVK_F19                       = 0x50,
  kVK_F20                       = 0x5A,
  kVK_F5                        = 0x60,
  kVK_F6                        = 0x61,
  kVK_F7                        = 0x62,
  kVK_F3                        = 0x63,
  kVK_F8                        = 0x64,
  kVK_F9                        = 0x65,
  kVK_F11                       = 0x67,
  kVK_F13                       = 0x69,
  kVK_F16                       = 0x6A,
  kVK_F14                       = 0x6B,
  kVK_F10                       = 0x6D,
  kVK_F12                       = 0x6F,
  kVK_F15                       = 0x71,
  kVK_Help                      = 0x72,
  kVK_Home                      = 0x73,
  kVK_PageUp                    = 0x74,
  kVK_ForwardDelete             = 0x75,
  kVK_F4                        = 0x76,
  kVK_End                       = 0x77,
  kVK_F2                        = 0x78,
  kVK_PageDown                  = 0x79,
  kVK_F1                        = 0x7A,
  kVK_LeftArrow                 = 0x7B,
  kVK_RightArrow                = 0x7C,
  kVK_DownArrow                 = 0x7D,
  kVK_UpArrow                   = 0x7E
};

/* ISO keyboards only*/
enum {
  kVK_ISO_Section               = 0x0A
};

/* JIS keyboards only*/
enum {
  kVK_JIS_Yen                   = 0x5D,
  kVK_JIS_Underscore            = 0x5E,
  kVK_JIS_KeypadComma           = 0x5F,
  kVK_JIS_Eisu                  = 0x66,
  kVK_JIS_Kana                  = 0x68
};

#endif

// convert an FLTK (X) keysym to a MacOS symbol:
// This table is in numeric order by FLTK symbol order for binary search.
static const struct {unsigned short vk, fltk;} vktab[] = {
  { kVK_Space, ' ' }, { kVK_ANSI_Quote, '\'' }, { kVK_ANSI_Comma, ',' }, { kVK_ANSI_Minus, '-' }, { kVK_ANSI_Period, '.' }, { kVK_ANSI_Slash, '/' }, 
  { kVK_ANSI_0, '0' }, { kVK_ANSI_1, '1'  }, { kVK_ANSI_2, '2'  }, { kVK_ANSI_3, '3'  }, 
  { kVK_ANSI_4, '4' }, { kVK_ANSI_5, '5'  }, { kVK_ANSI_6, '6'  }, { kVK_ANSI_7, '7'  }, 
  { kVK_ANSI_8, '8' }, { kVK_ANSI_9, '9'  }, { kVK_ANSI_Semicolon, ';'  }, { kVK_ANSI_Equal, '='  },
  { kVK_ANSI_A, 'A' }, { kVK_ANSI_B, 'B'  }, {  kVK_ANSI_C, 'C'  }, {  kVK_ANSI_D, 'D'  }, 
  { kVK_ANSI_E, 'E' }, {  kVK_ANSI_F, 'F'  }, {  kVK_ANSI_G, 'G'  }, {  kVK_ANSI_H, 'H'  }, 
  { kVK_ANSI_I, 'I' }, { kVK_ANSI_J, 'J'  }, { kVK_ANSI_K, 'K'  }, { kVK_ANSI_L, 'L'  }, 
  { kVK_ANSI_M, 'M' }, { kVK_ANSI_N, 'N'  }, { kVK_ANSI_O, 'O'  }, { kVK_ANSI_P, 'P'  }, 
  { kVK_ANSI_Q, 'Q' }, { kVK_ANSI_R, 'R'  }, {  kVK_ANSI_S, 'S'  }, { kVK_ANSI_T, 'T'  }, 
  { kVK_ANSI_U, 'U' }, {  kVK_ANSI_V, 'V'  }, { kVK_ANSI_W, 'W'  }, {  kVK_ANSI_X, 'X'  }, 
  { kVK_ANSI_Y, 'Y' }, {  kVK_ANSI_Z, 'Z'  }, 
  { kVK_ANSI_LeftBracket, '[' }, { kVK_ANSI_Backslash, '\\' },{ kVK_ANSI_RightBracket, ']' }, { kVK_ANSI_Grave, '`' }, 
  { kVK_VolumeDown, fltk3::VolumeDownKey}, { kVK_Mute, fltk3::VolumeMuteKey}, { kVK_VolumeUp, fltk3::VolumeUpKey},
  { kVK_Delete, fltk3::BackSpaceKey }, { kVK_Tab, fltk3::TabKey }, { kVK_ISO_Section, fltk3::IsoKey }, { kVK_Return, fltk3::EnterKey }, /*{ 0x7F, fltk3::PauseKey },
  { 0x7F, fltk3::ScrollLockKey },*/ { kVK_Escape, fltk3::EscapeKey },
  { kVK_JIS_Kana, fltk3::KanaKey}, { kVK_JIS_Eisu, fltk3::EisuKey}, { kVK_JIS_Yen, fltk3::YenKey}, { kVK_JIS_Underscore, fltk3::JISUnderscoreKey},
  { kVK_Home, fltk3::HomeKey }, { kVK_LeftArrow, fltk3::LeftKey },
  { kVK_UpArrow, fltk3::UpKey }, { kVK_RightArrow, fltk3::RightKey }, { kVK_DownArrow, fltk3::DownKey }, { kVK_PageUp, fltk3::PageUpKey },
  { kVK_PageDown, fltk3::PageDownKey },  { kVK_End, fltk3::EndKey }, /*{ 0x7F, fltk3::PrintKey }, { 0x7F, fltk3::InsertKey },*/
  { 0x6e, fltk3::MenuKey }, { kVK_Help, fltk3::HelpKey }, { kVK_ANSI_KeypadClear, fltk3::NumLockKey },
  { kVK_ANSI_KeypadEnter, fltk3::KPEnterKey }, { kVK_ANSI_KeypadMultiply, fltk3::KPKey+'*' }, { kVK_ANSI_KeypadPlus, fltk3::KPKey+'+'}, 
  { kVK_JIS_KeypadComma, fltk3::KPKey+',' },
  { kVK_ANSI_KeypadMinus, fltk3::KPKey+'-' }, { kVK_ANSI_KeypadDecimal, fltk3::KPKey+'.' }, { kVK_ANSI_KeypadDivide, fltk3::KPKey+'/' }, 
  { kVK_ANSI_Keypad0, fltk3::KPKey+'0' }, { kVK_ANSI_Keypad1, fltk3::KPKey+'1' }, { kVK_ANSI_Keypad2, fltk3::KPKey+'2' }, { kVK_ANSI_Keypad3, fltk3::KPKey+'3' }, 
  { kVK_ANSI_Keypad4, fltk3::KPKey+'4' }, { kVK_ANSI_Keypad5, fltk3::KPKey+'5' }, { kVK_ANSI_Keypad6, fltk3::KPKey+'6' }, { kVK_ANSI_Keypad7, fltk3::KPKey+'7' }, 
  { kVK_ANSI_Keypad8, fltk3::KPKey+'8' }, { kVK_ANSI_Keypad9, fltk3::KPKey+'9' }, { kVK_ANSI_KeypadEquals, fltk3::KPKey+'=' }, 
  { kVK_F1, fltk3::FKey+1 }, { kVK_F2, fltk3::FKey+2  }, { kVK_F3, fltk3::FKey+3  }, { kVK_F4, fltk3::FKey+4  }, 
  { kVK_F5, fltk3::FKey+5 }, { kVK_F6, fltk3::FKey+6  }, { kVK_F7, fltk3::FKey+7  }, { kVK_F8, fltk3::FKey+8  }, 
  { kVK_F9, fltk3::FKey+9 }, { kVK_F10, fltk3::FKey+10 }, { kVK_F11, fltk3::FKey+11 }, { kVK_F12, fltk3::FKey+12 }, 
  { kVK_F13, fltk3::FKey+13 }, { kVK_F14, fltk3::FKey+14 }, { kVK_F15, fltk3::FKey+15 }, { kVK_F16, fltk3::FKey+16 }, 
  { kVK_F17, fltk3::FKey+17 }, { kVK_F18, fltk3::FKey+18 }, { kVK_F19, fltk3::FKey+19 }, { kVK_F20, fltk3::FKey+20 }, 
  { kVK_Shift, fltk3::ShiftLKey }, { kVK_RightShift, fltk3::ShiftRKey }, { kVK_Control, fltk3::ControlLKey }, { kVK_RightControl, fltk3::ControlRKey }, 
  { kVK_CapsLock, fltk3::CapsLockKey }, { kVK_Command, fltk3::MetaLKey }, { 0x36, fltk3::MetaRKey },
  { kVK_Option, fltk3::AltLKey }, { kVK_RightOption, fltk3::AltRKey }, { kVK_ForwardDelete, fltk3::DeleteKey },
};

// Computes the macKeyLookUp table that transforms a Mac OS virtual keycode into an FLTK keysym
unsigned short *Fl_X::compute_macKeyLookUp()
{
  static unsigned short macKeyLookUp[128];
  memset(macKeyLookUp, 0, sizeof(macKeyLookUp));
  for (unsigned i = 0; i < sizeof(vktab)/sizeof(*vktab); i++) {
    macKeyLookUp[vktab[i].vk] = vktab[i].fltk;
  }
  return macKeyLookUp;
}

static int fltk2mac(int fltk) {
  int a = 0;
  int b = sizeof(vktab)/sizeof(*vktab);
  while (a < b) {
    int c = (a+b)/2;
    if (vktab[c].fltk == fltk) return vktab[c].vk;
    if (vktab[c].fltk < fltk) a = c+1; else b = c;
  }
  return vktab[a].vk;
}

//: returns true, if that key was pressed during the last event
int fltk3::event_key(unsigned int k) {
  return get_key(k);
}

//: returns true, if that key is pressed right now
int fltk3::get_key(unsigned int k) {
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
  if(CGEventSourceKeyState != NULL) {
    return (int)CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, fltk2mac(k) );
    }
  else 
#endif
  {
  typedef UInt32 fl_KeyMap[4];
  fl_KeyMap foo;
  // use the GetKeys Carbon function
  typedef void (*keymap_f)(fl_KeyMap);
  static keymap_f f = NULL;
  if (!f) f = ( keymap_f )Fl_X::get_carbon_function("GetKeys");
  (*f)(foo);
#ifdef MAC_TEST_FOR_KEYCODES
 static int cnt = 0;
 if (cnt++>1024) {
  cnt = 0;
  printf("%08x %08x %08x %08x\n", (ulong*)(foo)[3], (ulong*)(foo)[2], (ulong*)(foo)[1], (ulong*)(foo)[0]);
 }
#endif
  unsigned char *b = (unsigned char*)foo;
  // KP_Enter can be at different locations for Powerbooks vs. desktop Macs
  if (k==fltk3::KPEnterKey) {
    return (((b[0x34>>3]>>(0x34&7))&1)||((b[0x4c>>3]>>(0x4c&7))&1));
  }
  int i = fltk2mac(k);
  return (b[i>>3]>>(i&7))&1;
  }
}

//
// End of "$Id$".
//
