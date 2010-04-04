//
// "$Id$"
//
// WIN32 keyboard state routines for the Fast Light Tool Kit (FLTK).
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
// which are actually X keysyms.  So this has to translate to MSWindows
// VK_x symbols.

#include <fltk3/run.h>
#include <fltk3/x.H>
#include <config.h>

// convert an Fltk (X) keysym to a MSWindows VK symbol:
// See also the inverse converter in Fl_win32.cxx
// This table is in numeric order by Fltk symbol order for binary search:

static const struct {unsigned short vk, fltk;} vktab[] = {
  {VK_SPACE,	' '},
  {'1',		'!'},
  {0xde,	'\"'},
  {'3',		'#'},
  {'4',		'$'},
  {'5',		'%'},
  {'7',		'&'},
  {0xde,	'\''},
  {'9',		'('},
  {'0',		')'},
  {'8',		'*'},
  {0xbb,	'+'},
  {0xbc,	','},
  {0xbd,	'-'},
  {0xbe,	'.'},
  {0xbf,	'/'},
  {0xba,	':'},
  {0xba,	';'},
  {0xbc,	'<'},
  {0xbb,	'='},
  {0xbe,	'>'},
  {0xbf,	'?'},
  {'2',		'@'},
  {0xdb,	'['},
  {0xdc,	'\\'},
  {0xdd,	']'},
  {'6',		'^'},
  {0xbd,	'_'},
  {0xc0,	'`'},
  {0xdb,	'{'},
  {0xdc,	'|'},
  {0xdd,	'}'},
  {0xc0,	'~'},
  {VK_BACK,	fltk3::BackSpaceKey},
  {VK_TAB,	fltk3::TabKey},
  {VK_CLEAR,	0xff0b/*XK_Clear*/},
  {VK_RETURN,	fltk3::EnterKey},
  {VK_PAUSE,	fltk3::PauseKey},
  {VK_SCROLL,	fltk3::ScrollLockKey},
  {VK_ESCAPE,	fltk3::EscapeKey},
  {VK_HOME,	fltk3::HomeKey},
  {VK_LEFT,	fltk3::LeftKey},
  {VK_UP,	fltk3::UpKey},
  {VK_RIGHT,	fltk3::RightKey},
  {VK_DOWN,	fltk3::DownKey},
  {VK_PRIOR,	fltk3::PageUpKey},
  {VK_NEXT,	fltk3::PageDownKey},
  {VK_END,	fltk3::EndKey},
  {VK_SNAPSHOT,	fltk3::PrintKey},
  {VK_INSERT,	FL_Insert},
  {VK_APPS,	fltk3::MenuKey},
  {VK_NUMLOCK,	fltk3::NumLockKey},
//{VK_???,	fltk3::Keypad_Enter},
  {VK_MULTIPLY,	fltk3::Keypad+'*'},
  {VK_ADD,	fltk3::Keypad+'+'},
  {VK_SUBTRACT,	fltk3::Keypad+'-'},
  {VK_DECIMAL,	fltk3::Keypad+'.'},
  {VK_DIVIDE,	fltk3::Keypad+'/'},
  {VK_LSHIFT,	fltk3::LeftShiftKey},
  {VK_RSHIFT,	fltk3::RightShiftKey},
  {VK_LCONTROL,	fltk3::LeftControlKey},
  {VK_RCONTROL,	fltk3::RightControlKey},
  {VK_CAPITAL,	fltk3::CapsLockKey},
  {VK_LWIN,	fltk3::LeftMetaKey},
  {VK_RWIN,	fltk3::RightMetaKey},
  {VK_LMENU,	fltk3::LeftAltKey},
  {VK_RMENU,	fltk3::RightAltKey},
  {VK_DELETE,	fltk3::DeleteKey}
};

static int fltk2ms(int fltk) {
  if (fltk >= '0' && fltk <= '9') return fltk;
  if (fltk >= 'A' && fltk <= 'Z') return fltk;
  if (fltk >= 'a' && fltk <= 'z') return fltk-('a'-'A');
  if (fltk > fltk3::FKey && fltk <= fltk3::FKey+16) return fltk-(fltk3::FKey-(VK_F1-1));
  if (fltk >= fltk3::Keypad+'0' && fltk<=fltk3::Keypad+'9') return fltk-(fltk3::Keypad+'0'-VK_NUMPAD0);
  int a = 0;
  int b = sizeof(vktab)/sizeof(*vktab);
  while (a < b) {
    int c = (a+b)/2;
    if (vktab[c].fltk == fltk) return vktab[c].vk;
    if (vktab[c].fltk < fltk) a = c+1; else b = c;
  }
  return 0;
}

int fltk3::event_key(int k) {
  return GetKeyState(fltk2ms(k))&~1;
}

int fltk3::get_key(int k) {
  uchar foo[256];
  GetKeyboardState(foo);
  return foo[fltk2ms(k)]&~1;
}

//
// End of "$Id$".
//
