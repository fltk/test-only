//
// "$Id: Fl_Widget_SW.cxx,v 1.3 1999/05/06 05:52:22 carl Exp $"
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

#include <FL/Fl_Widget.H>

void Fl_Widget::attr(Attribute a, uchar c) {
  mstyle(&_style);
  WIDGET_STYLE->sbf |= bf(a);
  WIDGET_STYLE->widget(a)=c;
}

void Fl_Widget::box(Fl_Boxtype a) { attr(BOX, a); }
void Fl_Widget::color(uchar a) { attr(COLOR, a); }
void Fl_Widget::selection_color(uchar a) { attr(COLOR2, a); }
void Fl_Widget::color3(uchar a) { attr(COLOR3, a); }
void Fl_Widget::color(uchar a, uchar b) { attr(COLOR, a); attr(COLOR2, b); }
void Fl_Widget::label(Fl_Labeltype a, const char* b) { attr(LABELTYPE, a); label_ = b; }
void Fl_Widget::labeltype(Fl_Labeltype a) { attr(LABELTYPE, a); }
void Fl_Widget::labelcolor(uchar a) { attr(LABELCOLOR, a); }
void Fl_Widget::labelfont(uchar a) { attr(LABELFONT, a); }
void Fl_Widget::labelsize(uchar a) { attr(LABELSIZE, a); }
void Fl_Widget::color2(uchar a) {selection_color(a);}
