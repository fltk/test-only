// Function to turn a string into a color.  Used by themes and switches.
// (the function fl_rgb(r,g,b) is an inline function in Fl_Color.H)
//
// Copyright 1998-2000 by Bill Spitzak and others.
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

#include <FL/x.H>
#include <FL/Fl_Color.H>
#include <stdio.h>

Fl_Color fl_rgb(const char* name) {
  if (!name || !*name) return FL_NO_COLOR;
  int R,G,B;
  Fl_Color c;
  // read the KDE-style d,d,d color:
  if (sscanf(name, "%d,%d,%d", &R,&G,&B) == 3) {
    c = fl_rgb(R,G,B);
  } else {
#ifdef WIN32
    // simulation of XParseColor:
    if (*name == '#') name++;
    if (name[0]=='0' && name[1]=='x') name += 2;
    int n = strlen(name);
    int m = n/3;
    const char *pattern = 0;
    switch(m) {
      case 1: pattern = "%1x%1x%1x"; break;
      case 2: pattern = "%2x%2x%2x"; break;
      case 3: pattern = "%3x%3x%3x"; break;
      case 4: pattern = "%4x%4x%4x"; break;
      default: return FL_NO_COLOR;
    }
    if (sscanf(name, pattern, &R,&G,&B) != 3) return FL_NO_COLOR;
    switch(m) {
      case 1: R *= 0x11; G *= 0x11; B *= 0x11; break;
      case 3: R >>= 4; G >>= 4; B >>= 4; break;
      case 4: R >>= 8; G >>= 8; B >>= 8; break;
    }
    c = fl_rgb(R,G,B);
#else
    XColor x;
    fl_open_display();
    if (!XParseColor(fl_display, fl_colormap, name, &x)) return FL_NO_COLOR;
    c = fl_rgb(x.red>>8, x.green>>8, x.blue>>8);
#endif
  }
  if (!c) c = FL_BLACK;
  return c;
}

//
// End of "$Id: fl_rgb.cxx,v 1.2 2000/04/21 05:28:31 carl Exp $".
//
