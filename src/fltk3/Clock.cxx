//
// "$Id$"
//
// Clock widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <fltk3/run.h>
#include <fltk3/Clock.h>
#include <fltk3/draw.h>
#include <math.h>
#include <time.h>
#ifndef WIN32
#  include <sys/time.h>
#endif /* !WIN32 */

// Original clock display written by Paul Haeberli at SGI.
// Modifications by Mark Overmars for Forms
// Further changes by Bill Spitzak for fltk

const float hourhand[4][2] = {{-0.5f, 0}, {0, 1.5f}, {0.5f, 0}, {0, -7.0f}};
const float  minhand[4][2] = {{-0.5f, 0}, {0, 1.5f}, {0.5f, 0}, {0, -11.5f}};
const float  sechand[4][2] = {{-0.1f, 0}, {0, 2.0f}, {0.1f, 0}, {0, -11.5f}};

static void drawhand(double ang,const float v[][2],fltk3::Color fill,fltk3::Color line)
{
  fltk3::push_matrix();
  fltk3::rotate(ang);
  fltk3::color(fill); fltk3::begin_polygon();
  int i; for (i=0; i<4; i++) fltk3::vertex(v[i][0],v[i][1]); fltk3::end_polygon();
  fltk3::color(line); fltk3::begin_loop();
  for (i=0; i<4; i++) fltk3::vertex(v[i][0],v[i][1]); fltk3::end_loop();
  fltk3::pop_matrix();
}

void fltk3::ClockOutput::drawhands(fltk3::Color fill, fltk3::Color line) {
  if (!active_r()) {
    fill = fltk3::inactive(fill);
    line = fltk3::inactive(line);
  }
  drawhand(-360*(hour()+minute()/60.0)/12, hourhand, fill, line);
  drawhand(-360*(minute()+second()/60.0)/60, minhand, fill, line);
  drawhand(-360*(second()/60.0), sechand, fill, line);
}

static void rect(double x, double y, double w, double h) {
  double r = x+w;
  double t = y+h;
  fltk3::begin_polygon();
  fltk3::vertex(x, y);
  fltk3::vertex(r, y);
  fltk3::vertex(r, t);
  fltk3::vertex(x, t);
  fltk3::end_polygon();
}

/**
  Draw clock with the given position and size.
  \param[in] X, Y, W, H position and size
*/
void fltk3::ClockOutput::draw(int X, int Y, int W, int H) {
  fltk3::Color box_color = type()==fltk3::ROUND_CLOCK ? fltk3::GRAY : color();
  fltk3::Color shadow_color = fltk3::color_average(box_color, fltk3::BLACK, 0.5);
  draw_box(box(), X, Y, W, H, box_color);
  fltk3::push_matrix();
  fltk3::translate(X+W/2.0-.5, Y+H/2.0-.5);
  fltk3::scale((W-1)/28.0, (H-1)/28.0);
  if (type() == fltk3::ROUND_CLOCK) {
    fltk3::color(active_r() ? color() : fltk3::inactive(color()));
    fltk3::begin_polygon(); fltk3::circle(0,0,14); fltk3::end_polygon();
    fltk3::color(active_r() ? fltk3::FOREGROUND_COLOR : fltk3::inactive(fltk3::FOREGROUND_COLOR));
    fltk3::begin_loop(); fltk3::circle(0,0,14); fltk3::end_loop();
  }
  // draw the shadows:
  fltk3::push_matrix();
  fltk3::translate(0.60, 0.60);
  drawhands(shadow_color, shadow_color);
  fltk3::pop_matrix();
  // draw the tick marks:
  fltk3::push_matrix();
  fltk3::color(active_r() ? fltk3::FOREGROUND_COLOR : fltk3::inactive(fltk3::FOREGROUND_COLOR));
  for (int i=0; i<12; i++) {
    if (i==6) ::rect(-0.5, 9, 1, 2);
    else if (i==3 || i==0 || i== 9) ::rect(-0.5, 9.5, 1, 1);
    else ::rect(-0.25, 9.5, .5, 1);
    fltk3::rotate(-30);
  }
  fltk3::pop_matrix();
  // draw the hands:
  drawhands(selection_color(), fltk3::FOREGROUND_COLOR); // color was 54
  fltk3::pop_matrix();
}

/**
  Draw clock with current position and size.
*/
void fltk3::ClockOutput::draw() {
  draw(x(), y(), w(), h());
  draw_label();
}

/**
  Set the displayed time.
  Set the time in hours, minutes, and seconds.
  \param[in] H, m, s displayed time
  \see hour(), minute(), second()
 */
void fltk3::ClockOutput::value(int H, int m, int s) {
  if (H!=hour_ || m!=minute_ || s!=second_) {
    hour_ = H; minute_ = m; second_ = s;
    value_ = (H * 60 + m) * 60 + s;
    damage(fltk3::DAMAGE_CHILD);
  }
}

/**
  Set the displayed time.
  Set the time in seconds since the UNIX epoch (January 1, 1970).
  \param[in] v seconds since epoch
  \see value()
 */
void fltk3::ClockOutput::value(ulong v) {
  value_ = v;
  struct tm *timeofday;
  // Some platforms, notably Windows, now use a 64-bit time_t value...
  time_t vv = (time_t)v;
  timeofday = localtime(&vv);
  value(timeofday->tm_hour, timeofday->tm_min, timeofday->tm_sec);
}

/**
  Create a new fltk3::ClockOutput widget with the given position, size and label.
  The default boxtype is \c fltk3::NO_BOX.
  \param[in] X, Y, W, H position and size of the widget
  \param[in] L widget label, default is no label
 */
fltk3::ClockOutput::ClockOutput(int X, int Y, int W, int H, const char *L)
: fltk3::Widget(X, Y, W, H, L) {
  box(fltk3::UP_BOX);
  selection_color(fltk3::gray_ramp(5));
  align(fltk3::ALIGN_BOTTOM);
  hour_ = 0;
  minute_ = 0;
  second_ = 0;
  value_ = 0;
}

////////////////////////////////////////////////////////////////

/**
  Create an fltk3::Clock widget using the given position, size, and label string.
  The default boxtype is \c fltk3::NO_BOX.
  \param[in] X, Y, W, H position and size of the widget
  \param[in] L widget label, default is no label
 */
fltk3::Clock::Clock(int X, int Y, int W, int H, const char *L)
  : fltk3::ClockOutput(X, Y, W, H, L) {}

/**
  Create an fltk3::Clock widget using the given boxtype, position, size, and
  label string.
  \param[in] t boxtype
  \param[in] X, Y, W, H position and size of the widget
  \param[in] L widget label, default is no label
 */
fltk3::Clock::Clock(uchar t, int X, int Y, int W, int H, const char *L)
  : fltk3::ClockOutput(X, Y, W, H, L) {
  type(t);
  box(t==fltk3::ROUND_CLOCK ? fltk3::NO_BOX : fltk3::UP_BOX);
}

static void tick(void *v) {
  ((fltk3::Clock*)v)->value(time(0));
  fltk3::add_timeout(1.0, tick, v);
}

int fltk3::Clock::handle(int event) {
  switch (event) {
  case fltk3::SHOW:
    tick(this);
    break;
  case fltk3::HIDE:
    fltk3::remove_timeout(tick, this);
    break;
  }
  return ClockOutput::handle(event);
}
  
/**
  The destructor removes the clock.
 */
fltk3::Clock::~Clock() {
  fltk3::remove_timeout(tick, this);
}

//
// End of "$Id$".
//
