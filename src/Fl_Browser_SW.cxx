//
// "$Id: Fl_Browser_SW.cxx,v 1.2 1999/03/18 22:59:02 carl Exp $"
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

#include <FL/Fl_Browser_.H>

void Fl_Browser_::textfont(uchar s) {
  mstyle(&_style);
  BROWSER_STYLE->sbf |= bf(TEXTFONT);
  BROWSER_STYLE->browser(TEXTFONT)=s;
}

void Fl_Browser_::textsize(uchar s) {
  mstyle(&_style);
  BROWSER_STYLE->sbf |= bf(TEXTSIZE);
  BROWSER_STYLE->browser(TEXTSIZE)=s;
}

void Fl_Browser_::textcolor(uchar n) {
  mstyle(&_style);
  BROWSER_STYLE->sbf |= bf(TEXTCOLOR);
  BROWSER_STYLE->browser(TEXTCOLOR)=n;
}

void Fl_Browser_::selected_textcolor(uchar n) {
  mstyle(&_style);
  BROWSER_STYLE->sbf |= bf(SELECTED_TEXTCOLOR);
  BROWSER_STYLE->browser(SELECTED_TEXTCOLOR)=n;
}

