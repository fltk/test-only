//
// "$Id: Fl_Tooltip.cxx,v 1.3 1999/04/10 14:13:49 carl Exp $"
//
// Tooltip code for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Tooltip.H>

#include <stdio.h>

Fl_TooltipBox *Fl_Tooltip::box = 0;
Fl_Menu_Window *Fl_Tooltip::window = 0;
Fl_Widget *Fl_Tooltip::widget = 0;
int Fl_Tooltip::shown = 0;

Fl_Tooltip::Style Fl_Tooltip::default_style = {
  0.5f,
  215,
  FL_BORDER_BOX,
  FL_HELVETICA,
  FL_NORMAL_SIZE,
  FL_BLACK
};

#define MAX_WIDTH 400

class Fl_TooltipBox : public Fl_Menu_Window {
public:
  
  Fl_TooltipBox() : Fl_Menu_Window(0,0,10,10) {}

  ~Fl_TooltipBox() { }

  void draw() {
            color(Fl_Tooltip::default_style.color_);
    labeltype(FL_NORMAL_LABEL);
    labelfont(Fl_Tooltip::default_style.textfont_);
    labelsize(Fl_Tooltip::default_style.textsize_);
    labelcolor(Fl_Tooltip::default_style.textcolor_);
    align(FL_ALIGN_LEFT|FL_ALIGN_WRAP|FL_ALIGN_INSIDE);
    box((Fl_Boxtype)Fl_Tooltip::default_style.box_);
    Fl_Tooltip::loadstyle();
    draw_box(box(),0,0,w(),h(),color());
    draw_label(3,3,w()-6,h()-6, labelcolor());
  }

  int handle(int event) {
    if (event == FL_LEAVE) {
      Fl_Tooltip::exit(Fl_Tooltip::widget);
      return 1;
    }
    return Fl_Menu_Window::handle(event);
  }

};

void 
Fl_Tooltip::tooltip_timeout(Fl_Widget *v) {
  if (!v || !v->tooltip()) return;
  Fl_Window *widgetWindow = v->window();
  if (!widgetWindow) return; // this should not happen?
    
  if (!window) {
    Fl_Group* saveCurrent = Fl_Group::current();
    Fl_Group::current(0);
    window = new Fl_TooltipBox();
    window->clear_border();
    window->end();
    Fl_Group::current(saveCurrent);
  }

  // this cast bypasses the normal Fl_Window label() code:
  ((Fl_Widget*)window)->label(v->tooltip());

  fl_font(default_style.textfont_, default_style.textsize_);
  int ww, hh;
  ww = MAX_WIDTH;
  fl_measure(v->tooltip(), ww, hh);
  ww += 6; hh += 6;

  int ox = widgetWindow->x_root() + v->x() + v->w()/2;
  if (ox+ww > Fl::w()) ox = Fl::w() - ww;
  if (ox < 0) ox = 0;

  int oy = widgetWindow->y_root() + v->y() + v->h() + 4;
  if (oy+hh > Fl::h()) oy = widgetWindow->y_root() + v->y() - hh - 4;
  if (oy < 0) oy = 0;

  window->resize(ox, oy, ww, hh);
  Fl::grab(*widgetWindow); // necessary to get override_redirect turned on
  window->show();
  Fl::release();
}

// Load styles stuff
void
Fl_Tooltip::loadstyle() {
  if (!Fl::s_tooltip) {
    Fl::s_tooltip = 1;

    Fl::find("tooltip/box", default_style.box_, 1);
    Fl::find("tooltip/color", default_style.color_, 1);
    Fl::find("tooltip/text color", default_style.textcolor_, 1);
    Fl::find("tooltip/text font", default_style.textfont_, 1);
    Fl::find("tooltip/text size", default_style.textsize_, 1);

    int i;
    if (!Fl::find("tooltip/delay", i, 1))
      delay(((float)i)/1000.0f);
  }
}

void
Fl_Tooltip::tooltip_enter(Fl_Widget* w) {
  loadstyle();
  if (widget && w != widget) tooltip_exit(widget);
  if (!w || !w->tooltip()) return;
  widget = w;

  Fl::add_timeout(default_style.delay_, (void (*)(void *))Fl_Tooltip::tooltip_timeout, w);
}

extern Fl_Window *fl_xmousewin; // which window X thinks has FL_ENTER

void
Fl_Tooltip::tooltip_exit(Fl_Widget *w) {
  if (!w || w != widget) return;
  widget = 0;
  Fl::remove_timeout((void (*)(void *))Fl_Tooltip::tooltip_timeout, w);
  if (window) {
    // try to not make window disappear if user moves mouse into it.
    // Unfortunately fl_xmousewin is set to zero first, then to the
    // window, so I have to guess that this is happening:

    // This is done in Fl::belowmouse() now - CET
    //if (!fl_xmousewin || fl_xmousewin == window) return;
    window->hide();
  }
}

// setting the tooltip on a widget also sets the function pointers:

void Fl_Widget::tooltip(const char *t) {
  static char beenhere = 0;
  if (!beenhere) {
    beenhere = 1;
    fl_tooltip_enter = Fl_Tooltip::tooltip_enter;
    fl_tooltip_exit = Fl_Tooltip::tooltip_exit;
  }
  tooltip_ = t;
}

//
// End of "$Id: Fl_Tooltip.cxx,v 1.3 1999/04/10 14:13:49 carl Exp $".
//
