//
// "$Id: Fl_Clock.cxx,v 1.32 2002/09/16 00:29:05 spitzak Exp $"
//
// Clock widget for the Fast Light Tool Kit (FLTK).
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
#include <fltk/Fl_Clock.h>
#include <fltk/fl_draw.h>
#include <math.h>
#include <time.h>
#ifndef _WIN32
#  include <sys/time.h>
#endif /* !_WIN32 */

// Original clock display written by Paul Haeberli at SGI.
// Modifications by Mark Overmars for Forms
// Further changes by Bill Spitzak for fltk

const float hourhand[4][2] = {{-0.5f, 0}, {0, 1.5f}, {0.5f, 0}, {0, -7.0f}};
const float minhand[4][2] = {{-0.5f, 0}, {0, 1.5f}, {0.5f, 0}, {0, -11.5f}};
const float sechand[4][2] = {{-0.1f, 0}, {0, 2.0f}, {0.1f, 0}, {0, -11.5f}};

static void drawhand(float ang,const float v[][2],Fl_Color fill,Fl_Color line)
{
  fl_push_matrix();
  fl_rotate(ang);
  fl_vertices(4, v);
  fl_color(fill); fl_fill_stroke(line);
  fl_pop_matrix();
}

void Fl_Clock_Output::drawhands(Fl_Color fill, Fl_Color line) {
  drawhand(-360*(hour()+minute()/60.0f)/12, hourhand, fill, line);
  drawhand(-360*(minute()+second()/60.0f)/60, minhand, fill, line);
  drawhand(-360*(second()/60.0f), sechand, fill, line);
}

static void rect(float x, float y, float w, float h) {
  float v[4][2];
  v[0][0] = v[3][0] = float(x);
  v[1][0] = v[2][0] = float(x+w);
  v[0][1] = v[1][1] = float(y);
  v[2][1] = v[3][1] = float(y+h);
  fl_vertices(4,v);
  fl_fill();
}

void Fl_Clock_Output::draw(int x, int y, int w, int h) {
  fl_push_matrix();
  fl_translate(x+w/2.0f-.5f, y+h/2.0f-.5f);
  fl_scale((w-1)/28.0f, (h-1)/28.0f);
  if (type() == FL_ROUND_CLOCK) {
    fl_circle(0,0,14);
    fl_color(color()); fl_fill_stroke(FL_BLACK);
  }
  // draw the shadows:
  fl_push_matrix();
  fl_translate(0.60f, 0.60f);
  Fl_Color c = fl_color_average(color(), FL_BLACK, .3);
  drawhands(c, c);
  fl_pop_matrix();
  // draw the tick marks:
  fl_push_matrix();
  fl_color(text_color()); // color was 52
  for (int i=0; i<12; i++) {
    if (i==6) rect(-0.5, 9, 1, 2);
    else if (i==3 || i==0 || i== 9) rect(-0.5, 9.5, 1, 1);
    else rect(-0.25, 9.5, .5, 1);
    fl_rotate(-30);
  }
  fl_pop_matrix();
  // draw the hands:
  drawhands(selection_color(), text_color()); // color was 54
  fl_pop_matrix();
}

void Fl_Clock_Output::draw() {
  draw_box();
  draw(0, 0, w(), h());
}

void Fl_Clock_Output::value(int h, int m, int s) {
  if (h!=hour_ || m!=minute_ || s!=second_) {
    hour_ = h; minute_ = m; second_ = s;
    redraw(FL_DAMAGE_CHILD);
  }
}

void Fl_Clock_Output::value(ulong v) {
  struct tm *timeofday;
  timeofday = localtime((const time_t *)&v);
  value(timeofday->tm_hour, timeofday->tm_min, timeofday->tm_sec);
}

////////////////////////////////////////////////////////////////

Fl_Clock::Fl_Clock(int x, int y, int w, int h, const char *l)
  : Fl_Clock_Output(x, y, w, h, l) {}

static void tick(void *v) {
#ifdef _WIN32
  ((Fl_Clock*)v)->value(time(0));
  Fl::add_timeout(1.0f, (Fl_Timeout_Handler)tick, v);
#else
  struct timeval t;
  gettimeofday(&t, 0);
  ((Fl_Clock*)v)->value(t.tv_sec);
  float delay = 1.0f-t.tv_usec*.000001f;
  if (delay < .1f || delay > .9f) delay = 1.0f;
  Fl::add_timeout(delay, (Fl_Timeout_Handler)tick, v);
#endif
}

int Fl_Clock::handle(int event) {
  switch (event) {
  case FL_SHOW:
    tick(this);
    break;
  case FL_HIDE:
    Fl::remove_timeout((Fl_Timeout_Handler)tick, this);
    break;
  }
  return Fl_Clock_Output::handle(event);
}

Fl_Clock::~Fl_Clock() {
  Fl::remove_timeout((Fl_Timeout_Handler)tick, this);
}

static void revert(Fl_Style* s) {
  s->selection_color = FL_LIGHT1;
  //s->button_color = FL_DARK3;
  //s->text_color = FL_BLACK;
}
static Fl_Named_Style style("Clock", revert, &Fl_Clock::default_style);
Fl_Named_Style* Fl_Clock::default_style = &::style;

Fl_Clock_Output::Fl_Clock_Output(int x, int y, int w, int h, const char *l)
: Fl_Widget(x, y, w, h, l) {
  style(Fl_Clock::default_style);
  clear_flag(FL_ALIGN_MASK);
  set_flag(FL_ALIGN_BOTTOM);
  hour_ = 0;
  minute_ = 0;
  second_ = 0;
  value_ = 0;
}

//
// End of "$Id: Fl_Clock.cxx,v 1.32 2002/09/16 00:29:05 spitzak Exp $".
//
