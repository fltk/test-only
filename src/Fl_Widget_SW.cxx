//
// "$Id: Fl_Widget_SW.cxx,v 1.1 1999/03/14 06:46:36 carl Exp $"
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

void Fl_Widget::box(Fl_Boxtype a) {
  wstyle();
  WIDGET_STYLE->sbf |= bf(BOX);
  WIDGET_STYLE->widget(BOX)=a;
}

void Fl_Widget::color(uchar a) {
  wstyle();
  WIDGET_STYLE->sbf |= bf(COLOR);
  WIDGET_STYLE->widget(COLOR)=a;
}

void Fl_Widget::selection_color(uchar a) {
  wstyle();
  WIDGET_STYLE->sbf |= bf(COLOR2);
  WIDGET_STYLE->widget(COLOR2)=a;
}

void Fl_Widget::color3(uchar a) {
  wstyle();
  WIDGET_STYLE->sbf |= bf(COLOR3);
  WIDGET_STYLE->widget(COLOR3)=a;
}

void Fl_Widget::color(uchar a, uchar b) {
  wstyle();
  WIDGET_STYLE->sbf |= (bf(COLOR) | bf(COLOR2));
  WIDGET_STYLE->widget(COLOR)=a;
  WIDGET_STYLE->widget(COLOR2)=b;
}

void Fl_Widget::label(Fl_Labeltype a,const char* b) {
  wstyle();
  WIDGET_STYLE->sbf |= bf(LABELTYPE);
  WIDGET_STYLE->widget(LABELTYPE)=a;
  label_=b;
}

void Fl_Widget::labeltype(Fl_Labeltype a) {
  wstyle();
  WIDGET_STYLE->sbf |= bf(LABELTYPE);
  WIDGET_STYLE->widget(LABELTYPE)=a;
}

void Fl_Widget::labelcolor(uchar a) {
  wstyle();
  WIDGET_STYLE->sbf |= bf(LABELCOLOR);
  WIDGET_STYLE->widget(LABELCOLOR)=a;
}

void Fl_Widget::labelfont(uchar a) {
  wstyle();
  WIDGET_STYLE->sbf |= bf(LABELFONT);
  WIDGET_STYLE->widget(LABELFONT)=a;
}

void Fl_Widget::labelsize(uchar a) {
  wstyle();
  WIDGET_STYLE->sbf |= bf(LABELSIZE);
  WIDGET_STYLE->widget(LABELSIZE)=a;
}

void Fl_Widget::color2(uchar a) {selection_color(a);}
