//
// "$Id: Fl_Float_Input.cxx,v 1.5 2001/09/10 07:38:06 spitzak Exp $"
//
// Input widget for the Fast Light Tool Kit (FLTK).
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

#include <fltk/Fl.h>
#include <fltk/Fl_Widget.h>
#include <fltk/Fl_Float_Input.h>
#include <string.h>

int Fl_Float_Input::replace(int b, int e, const char* text, int ilen) {
  for (int n = 0; n < ilen; n++) {
    char ascii = text[n];
    Fl::compose_reset(); // ignore any foreign letters...
    // This is complex to allow "0xff12" hex to be typed:
    if (b+n==0 && (ascii == '+' || ascii == '-') ||
	(ascii >= '0' && ascii <= '9') ||
	(b+n==1 && index(0)=='0' && (ascii=='x' || ascii == 'X')) ||
	(b+n>1 && index(0)=='0' && (index(1)=='x'||index(1)=='X')
	 && (ascii>='A'&& ascii<='F' || ascii>='a'&& ascii<='f')) ||
	type()==FL_FLOAT_INPUT && ascii && strchr(".eE+-", ascii))
      continue; // it's ok;
    return 0;
  }
  return Fl_Input::replace(b,e,text,ilen);
}

//
// End of "$Id: Fl_Float_Input.cxx,v 1.5 2001/09/10 07:38:06 spitzak Exp $"
//
