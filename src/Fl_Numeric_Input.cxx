//
// "$Id: Fl_Numeric_Input.cxx,v 1.3 2002/02/10 22:57:48 spitzak Exp $"
//
// Copyright 2002 by Bill Spitzak, Digital Domain, and others.
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

// This user interface design is Copyright (C) 2000-2002 Digital Domain.
// Patent Pending!
//
// License is ONLY granted to implement this algorithim as GPL or LGPL code.
//
// License to use this technology in closed source code may be available:
//
// Digital Domain
// Intellectual Property
// 300 Rose Avenue, Venice, CA 90291, USA
// (310)314-2800 FAX(310)314-2888

#include <fltk/Fl.h>
#include <fltk/Fl_Numeric_Input.h>
#include <stdio.h>

// Sets the string value to the %g formatted version of v
void Fl_Numeric_Input::value(double v) {
  char buf[100];
  sprintf(buf, "%g", v);
#if 0
  if (v > 1 || v < -1)
    sprintf(buf, "%.4g", v);
  else
    sprintf(buf, "%.3g", v);
#endif
  Fl_Input::value(buf);
}

// Sets the string value to the %d formatted version of v
void Fl_Numeric_Input::value(int v) {
  char buf[100];
  sprintf(buf, "%d", v);
  Fl_Input::value(buf);
}

int Fl_Numeric_Input::handle(int event) {
  if (event == FL_KEY) switch (Fl::event_key()) {
  case FL_Up:
    if (Fl::event_state(FL_CTRL)) return 0;
    return handle_arrow(1);
  case FL_Down:
    if (Fl::event_state(FL_CTRL)) return 0;
    return handle_arrow(-1);
  case FL_Left:
  case FL_Right:
    if (Fl::event_state(FL_CTRL)) return 0;
    break;
  }
  return Fl_Input::handle(event);
}

// Handle and up or down arrow key:
int Fl_Numeric_Input::handle_arrow(int dir)
{
  // locate the character to change:
  int p; char c;
  int q = position();
  if (mark() > q) q = mark() - 1;
  else if (mark() < q) q = q - 1;
  const char* v = value();

  // make insertion after decimal point work:
  if (v[q] == '.') q++;

  int save_when = when(); when(0);

  // add trailing ".0" if cursor is on end of number:
  if (v[q] < '0' || v[q] > '9') {
    for (int g = q-1;;g--) {
      // search to see if decimal point is already there:
      if (g >= 0 && v[g] == '.') break;
      if (g < 0 || v[g] < '0' || v[g] > '9') {
	// if no digits before cursor, assumme we are not pointing at a number:
	if (g >= q-1) goto DONE;
	// if it does not like period edit the last digit instead:
	if (!replace(q,q,'.')) {q--; goto INT;}
	q++;
	break;
      }
    }
    replace(q,q,'0');
  }
 INT:

  // if it's a negative number we reverse direction:
  for (p = q-1; p >= 0; p--) {
    c = v[p];
    if (c == '-') {dir = -dir; break;}
    if (c != '.' && (c < '0' || c > '9')) break;
  }

  if (dir > 0) {
  UP_CASE:
    // up to a larger absolute value, which is much simpler:
    for (p = q; p >= 0; p--) {
      c = v[p];
      if (c == '.') continue;
      if (c < '0' || c > '9') break;
      if (c < '9') {
	replace(p, p+1, c+1);
	goto DONE;
      }
      replace(p, p+1, '0');
    }
    replace(p+1, p+1, '1');
    q++;

  } else {
    // down to a smaller absolute value:
    // first check if all the digits are zero, if so we reverse the sign:
    for (p = q; ; p--) {
      if (p < 0 || (v[p] < '0' || v[p] > '9') && v[p] != '.') {
	if (p >= 0 && v[p] == '-') {
	  Fl_Input::replace(p, p+1, 0,0);
	  q--;
	} else {
	  replace(p+1, p+1, '-');
	  q++;
	}
	goto UP_CASE;
      }
      if (v[p] != '.' && v[p] != '0') break;
    }

    for (p = q; p >= 0; p--) {
      c = v[p];
      if (c == '.') continue;
      if (c < '0' || c > '9') break;
      if (c == '1') {
	// delete leading zeros:
	int g = p;
	while (g > 0 && v[g-1]=='0') g--;
	if (!(g > 0 && (v[g-1]>='0' && v[g-1]<='9' || v[g-1]=='.'))) {
	  if (p < q) {
	    Fl_Input::replace(g, p+1, 0, 0);
	    q -= p-g+1;
	    goto DONE;
	  }
	  Fl_Input::replace(g, p, 0, 0);
	  q -= p-g;
	  p = g;
	}
      }
      if (c > '0') {
	replace(p, p+1, c-1);
	goto DONE;
      }
      replace(p, p+1, '9');
    }
  }
 DONE:
  position(q, q+1);
  when(save_when);
  if (save_when&(FL_WHEN_CHANGED|FL_WHEN_ENTER_KEY) && changed()) {
    clear_changed(); do_callback();
  }
  return 1;
}

