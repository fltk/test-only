//
// "$Id: fl_rgb.cxx,v 1.8 2004/01/25 06:55:06 spitzak Exp $"
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/x.h>
#include <fltk/Color.h>
#include <stdio.h>

/*! Turn a string into a color. On X this uses XParseColor to lookup
  named colors like "red".

  On other platforms it only understands colors of the form "n,n,n"
  or hex strings of 3, 6, 9, or 12 digits. Maybe we should add the
  standard web color names, there are only 16 of them.
*/
fltk::Color fltk::color(const char* name) {
  if (!name || !*name) return NO_COLOR;
  int R,G,B;
  Color c;
  // read the KDE-style d,d,d color:
  if (sscanf(name, "%d,%d,%d", &R,&G,&B) == 3) {
    c = color(R,G,B);
  } else {
#if defined(_WIN32) || (defined(__APPLE__) && !USE_X11)
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
      default: return NO_COLOR;
    }
    if (sscanf(name, pattern, &R,&G,&B) != 3) return NO_COLOR;
    switch(m) {
      case 1: R *= 0x11; G *= 0x11; B *= 0x11; break;
      case 3: R >>= 4; G >>= 4; B >>= 4; break;
      case 4: R >>= 8; G >>= 8; B >>= 8; break;
    }
    c = color(R,G,B);
#else
    XColor x;
    open_display();
    if (!XParseColor(xdisplay, xcolormap, name, &x)) return NO_COLOR;
    c = color(x.red>>8, x.green>>8, x.blue>>8);
#endif
  }
  if (!c) c = BLACK;
  return c;
}

//
// End of "$Id: fl_rgb.cxx,v 1.8 2004/01/25 06:55:06 spitzak Exp $".
//
