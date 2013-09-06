//
// "$Id$"
//
// Character compose processing for the Fast Light Tool Kit (FLTK).
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

#include <fltk3/run.h>
#include <fltk3/x.h>

#ifndef FLTK3_DOXYGEN
int fltk3::compose_state = 0;
#ifdef __APPLE__
int Fl_X::next_marked_length = 0;
#endif
#endif

#if !defined(WIN32) && !defined(__APPLE__)
extern XIC fl_xim_ic;
#endif

/** Any text editing widget should call this for each fltk3::KEYBOARD event.
 Use of this function is very simple.
 
 <p>If <i>true</i> is returned, then it has modified the
 fltk3::event_text() and fltk3::event_length() to a set of <i>bytes</i> to
 insert (it may be of zero length!).  It will also set the "del"
 parameter to the number of <i>bytes</i> to the left of the cursor to
 delete, this is used to delete the results of the previous call to
 fltk3::compose().
 
 <p>If <i>false</i> is returned, the keys should be treated as function
 keys, and del is set to zero. You could insert the text anyways, if
 you don't know what else to do.
 
 <p>On the Mac OS platform, text input can involve marked text, that is, 
 temporary text replaced by other text during the input process. This occurs,
 e.g., when using dead keys or when entering CJK characters.
 Text editing widgets should preferentially signal
 marked text, usually underlining it. Widgets can use
 <tt>int fltk3::compose_state</tt> <i>after</i> having called fltk3::compose(int&)
 to obtain the length in bytes of marked text that always finishes at the
 current insertion point. It's the widget's task to underline marked text.
 Widgets should also call <tt>void fltk3::reset_marked_text()</tt> when processing fltk3::UNFOCUS
 events. Optionally, widgets can also call
 <tt>void fltk3::insertion_point_location(int x, int y, int height)</tt> to indicate the window 
 coordinates of the bottom of the current insertion point and the line height. 
 This way, auxiliary windows that help choosing among alternative characters 
 appear just below the insertion point. If widgets don't do that, 
 auxiliary windows appear at the widget's bottom. The
 fltk3::Input and fltk3::TextEditor widgets underline marked text.
 If none of this is done by a user-defined text editing widget,
 text input will work, but will not signal to the user what text is marked.
 Finally, text editing widgets should call <tt>set_flag(MAC_USE_ACCENTS_MENU);</tt>
 in their constructor if they want to use the feature introduced with Mac OS 10.7 "Lion"
 where pressing and holding a key on the keyboard opens an accented-character menu window.
 
 <p>Though the current implementation returns immediately, future
 versions may take quite awhile, as they may pop up a window or do
 other user-interface things to allow characters to be selected.
 */
int fltk3::compose(int& del) {
  int condition;
#if defined(__APPLE__)
  int has_text_key = fltk3::compose_state || fltk3::e_keysym <= '~' || fltk3::e_keysym == fltk3::IsoKey ||
  (fltk3::e_keysym >= fltk3::KPKey && fltk3::e_keysym <= fltk3::KPLastKey && fltk3::e_keysym != fltk3::KPEnterKey);
  condition = fltk3::e_state&(fltk3::META | fltk3::CTRL) || 
  (fltk3::e_keysym >= fltk3::ShiftLKey && fltk3::e_keysym <= fltk3::AltRKey) || // called from flagsChanged
  !has_text_key ;
#else
  unsigned char ascii = (unsigned char)e_text[0];
#if defined(WIN32)
  condition = (e_state & (fltk3::ALT | fltk3::META)) && !(ascii & 128) ;
#else
  condition = (e_state & (fltk3::ALT | fltk3::META | fltk3::CTRL)) && !(ascii & 128) ;
#endif // WIN32
#endif // __APPLE__
  if (condition) { del = 0; return 0;} // this stuff is to be treated as a function key
  del = fltk3::compose_state;
#ifdef __APPLE__
  fltk3::compose_state = Fl_X::next_marked_length;
#else
  fltk3::compose_state = 0;
  // Only insert non-control characters:
  if ( (!fltk3::compose_state) && ! (ascii & ~31 && ascii!=127)) { return 0; }
#endif
  return 1;
}

#ifdef __APPLE__
static int insertion_point_x = 0;
static int insertion_point_y = 0;
static int insertion_point_height = 0;
static bool insertion_point_location_is_valid = false;

void fltk3::reset_marked_text() {
  fltk3::compose_state = 0;
  Fl_X::next_marked_length = 0;
  insertion_point_location_is_valid = false;
}
int Fl_X::insertion_point_location(int *px, int *py, int *pheight) 
// return true if the current coordinates of the insertion point are available
{
  if ( ! insertion_point_location_is_valid ) return false;
  *px = insertion_point_x;
  *py = insertion_point_y;
  *pheight = insertion_point_height;
  return true;
}
void fltk3::insertion_point_location(int x, int y, int height) {
  insertion_point_location_is_valid = true;
  insertion_point_x = x;
  insertion_point_y = y;
  insertion_point_height = height;
}
#endif // __APPLE__

/**
 If the user moves the cursor, be sure to call fltk3::compose_reset().
 The next call to fltk3::compose() will start out in an initial state. In
 particular it will not set "del" to non-zero. This call is very fast
 so it is ok to call it many times and in many places.
 */
void fltk3::compose_reset()
{
  fltk3::compose_state = 0;
#if !defined(WIN32) && !defined(__APPLE__)
  if (fl_xim_ic) XmbResetIC(fl_xim_ic);
#endif
}

//
// End of "$Id$"
//

