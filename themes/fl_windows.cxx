//
// "$Id: fl_windows.cxx,v 1.11 2002/01/20 10:12:48 spitzak Exp $"
//
// Theme plugin file for FLTK
//
// Copyright 1999 Bill Spitzak and others.
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

// Theme plugin for fltk to more accurately emulate the Windows 98 GUI
// than the fltk default does.  Due to popular demand (ugh) some of this
// may be moved to the default...

#include <fltk/Fl.h>
#include <fltk/Fl_Widget.h>
#include <fltk/Fl_Check_Button.h>
#include <fltk/Fl_Scrollbar.h>
#include <fltk/Fl_Input.h>
#include <fltk/Fl_Output.h>
#include <fltk/Fl_Style.h>
#include <fltk/fl_theme.h>

////////////////////////////////////////////////////////////////
// Different box type used by win98 sometimes:

// More accurate copy of the colors on the edges of boxes, from Win98
// Fltk by default uses colors picked by Bill for aesthetic reasons:
// newer versions of Windows & KDE look closer to FLTK default
//static const Fl_Frame_Box win98_menu_window_box(0, "2AARRMMUU", FL_DOWN_BOX);
//extern const Fl_Frame_Box win98_down_box;
//static const Fl_Frame_Box win98_up_box(0, "2AAXXIIUU", &win98_down_box);
//       const Fl_Frame_Box win98_down_box(0, "2XXIIUUAA", &win98_up_box);

////////////////////////////////////////////////////////////////

int fl_windows() {
  Fl_Style::draw_boxes_inactive = 0;

// newer versions of Windows & KDE look closer to FLTK default
//  Fl_Widget::default_style->button_box = &win98_up_box;
// this may be needed if fltk's default is the thin box:
//  Fl_Widget::default_style->box = &win98_down_box;

  Fl_Style* s;

  if ((s = Fl_Style::find("menu"))) {
//    s->box = &win98_menu_window_box;
    s->leading = 6;
  }

  if ((s = Fl_Style::find("item"))) {
    s->button_box = FL_NO_BOX; // no box around checkmarks
  }

  if ((s = Fl_Style::find("menu bar"))) {
    s->highlight_color = FL_GRAY; // needed for title highlighting
  }

  // The default is white, but setting this will overwrite any
  // value read from Windows, so I leave it as the slight gray we default to:
  //Fl_Widget::default_style->window_color = FL_WHITE;

  if ((s = Fl_Style::find("scrollbar"))) {
//    s->box = &win98_menu_window_box;
    s->color = 52;
  }

  if ((s = Fl_Style::find("highlight button"))) {
    s->highlight_color = FL_GRAY;
  }

  return 0;
}

//
// End of "$Id: fl_windows.cxx,v 1.11 2002/01/20 10:12:48 spitzak Exp $"
//
