//
// "$Id: fl_load_browser.cxx,v 1.1 2000/05/15 05:52:23 bill Exp $"
//
// File loading routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

#include <FL/Fl.H>
#include <FL/Fl_Browser.H>
#include <stdio.h>

int fl_load_browser(Fl_Widget* o, const char* filename) {
  Fl_Browser* b = (Fl_Browser*)o;
#define MAXFL_BLINE 1024
  char newtext[MAXFL_BLINE];
  int c;
  int i;
  b->clear();
  if (!filename || !(filename[0])) return 1;
  FILE *fl = fopen(filename,"r");
  if (!fl) return 0;
  i = 0;
  do {
    c = getc(fl);
    if (c == '\n' || c <= 0 || i>=(MAXFL_BLINE-1)) {
      newtext[i] = 0;
      b->add(newtext);
      i = 0;
    } else
      newtext[i++] = c;
  } while (c >= 0);
  fclose(fl);
  return 1;
}

//
// End of "$Id: fl_load_browser.cxx,v 1.1 2000/05/15 05:52:23 bill Exp $"
//
