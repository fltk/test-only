/
// "$Id$
/
// Xlib line style code for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2003 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#include <FL/Fl.H
#include <FL/fl_draw.H
#include <FL/x.H
#include "../flstring.h
#include <stdio.h
#include "Fl_Xlib_Display.H

void Fl_Xlib_Display::line_style(int style, int width, char* dashes) 

  int ndashes = dashes ? strlen(dashes) : 0
  // emulate the WIN32 dash patterns on 
  char buf[7]
  if (!ndashes && (style&0xff)) 
    int w = width ? width : 1
    char dash, dot, gap
    // adjust lengths to account for cap
    if (style & 0x200) 
      dash = char(2*w)
      dot = 1; // unfortunately 0 does not wor
      gap = char(2*w-1)
    } else 
      dash = char(3*w)
      dot = gap = char(w)
    
    char* p = dashes = buf
    switch (style & 0xff) 
    case FL_DASH:	*p++ = dash; *p++ = gap; break
    case FL_DOT:	*p++ = dot; *p++ = gap; break
    case FL_DASHDOT:	*p++ = dash; *p++ = gap; *p++ = dot; *p++ = gap; break
    case FL_DASHDOTDOT: *p++ = dash; *p++ = gap; *p++ = dot; *p++ = gap; *p++ = dot; *p++ = gap; break
    
    ndashes = p-buf
  
  static int Cap[4] = {CapButt, CapButt, CapRound, CapProjecting}
  static int Join[4] = {JoinMiter, JoinMiter, JoinRound, JoinBevel}
  XSetLineAttributes(fl_display, fl_gc, width,
		     ndashes ? LineOnOffDash : LineSolid
		     Cap[(style>>8)&3], Join[(style>>12)&3])
  if (ndashes) XSetDashes(fl_display, fl_gc, 0, dashes, ndashes)




/
// End of "$Id$"
/
