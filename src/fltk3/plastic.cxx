//
// "$Id$"
//
// "Plastic" drawing routines for the Fast Light Tool Kit (FLTK).
//
// These box types provide a cross between Aqua and KDE buttons; kindof
// like translucent plastic buttons...
//
// Copyright 2001-2010 by Michael Sweet.
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

// Box drawing code for an obscure box type.
// These box types are in separate files so they are not linked
// in if not used.

#include <fltk3/run.h>
#include <fltk3/draw.h>
#include <fltk3/Box.h>
#include "flstring.h"

//
// Uncomment the following line to restore the old plastic box type
// appearance.
//

//#define USE_OLD_PLASTIC_BOX
#define USE_OLD_PLASTIC_COLOR

namespace fltk3 {
  extern uchar* gray_ramp();
}


inline fltk3::Color shade_color(uchar gc, fltk3::Color bc) {
#ifdef USE_OLD_PLASTIC_COLOR
  return fltk3::color_average((fltk3::Color)gc, bc, 0.75f);
#else
  unsigned	grgb = fltk3::get_color((fltk3::Color)gc),
		brgb = fltk3::get_color(bc);
  int		red, green, blue, gray;


  gray  = ((grgb >> 24) & 255);
  red   = gray * ((brgb >> 24) & 255) / 255 + gray * gray / 510;
  gray  = ((grgb >> 16) & 255);
  green = gray * ((brgb >> 16) & 255) / 255 + gray * gray / 510;
  gray  = ((grgb >> 8) & 255);
  blue  = gray * ((brgb >> 8) & 255) / 255 + gray * gray / 510;

  if (red > 255)
    red = 255;

  if (green > 255)
    green = 255;

  if (blue > 255)
    blue = 255;

  if (fltk3::draw_box_active())
    return fltk3::rgb_color(red, green, blue);
  else
    return fltk3::color_average(fltk3::GRAY, fltk3::rgb_color(red, green, blue), 0.75f);
#endif // USE_OLD_PLASTIC_COLOR
}


static void frame_rect(int x, int y, int w, int h, const char *c, fltk3::Color bc) {
  uchar *g = fltk3::gray_ramp();
  int b = ((int)strlen(c)) / 4 + 1;

  for (x += b, y += b, w -= 2 * b, h -= 2 * b; b > 1; b --)
  {
    // Draw lines around the perimeter of the button, 4 colors per
    // circuit.
    fltk3::color(shade_color(g[(int)*c++], bc));
    fltk3::line(x, y + h + b, x + w - 1, y + h + b, x + w + b - 1, y + h);
    fltk3::color(shade_color(g[(int)*c++], bc));
    fltk3::line(x + w + b - 1, y + h, x + w + b - 1, y, x + w - 1, y - b);
    fltk3::color(shade_color(g[(int)*c++], bc));
    fltk3::line(x + w - 1, y - b, x, y - b, x - b, y);
    fltk3::color(shade_color(g[(int)*c++], bc));
    fltk3::line(x - b, y, x - b, y + h, x, y + h + b);
  }
}


static void frame_round(int x, int y, int w, int h, const char *c, fltk3::Color bc) {
  uchar *g = fltk3::gray_ramp();
  int b = ((int)strlen(c)) / 4 + 1;

  if (w==h) {
    for (; b > 1; b --, x ++, y ++, w -= 2, h -= 2)
    {
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x, y, w, h, 45.0, 135.0);
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x, y, w, h, 315.0, 405.0);
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x, y, w, h, 225.0, 315.0);
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x, y, w, h, 135.0, 225.0);
    }
  } else if (w>h) {
    int d = h/2;
    for (; b > 1; d--, b --, x ++, y ++, w -= 2, h -= 2)
    {
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x, y, h, h, 90.0, 135.0);
      fltk3::xyline(x+d, y, x+w-d);
      fltk3::arc(x+w-h, y, h, h, 45.0, 90.0);
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x+w-h, y, h, h, 315.0, 405.0);
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x+w-h, y, h, h, 270.0, 315.0);
      fltk3::xyline(x+d, y+h-1, x+w-d);
      fltk3::arc(x, y, h, h, 225.0, 270.0);
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x, y, h, h, 135.0, 225.0);
    }
  } else if (w<h) {
    int d = w/2;
    for (; b > 1; d--, b --, x ++, y ++, w -= 2, h -= 2)
    {
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x, y, w, w, 45.0, 135.0);
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x, y, w, w, 0.0, 45.0);
      fltk3::yxline(x+w-1, y+d, y+h-d);
      fltk3::arc(x, y+h-w, w, w, 315.0, 360.0);
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x, y+h-w, w, w, 225.0, 315.0);
      fltk3::color(shade_color(g[(int)*c++], bc));
      fltk3::arc(x, y+h-w, w, w, 180.0, 225.0);
      fltk3::yxline(x, y+d, y+h-d);
      fltk3::arc(x, y, w, w, 135.0, 180.0);
    }
  }
}


static void shade_rect(int x, int y, int w, int h, const char *c, fltk3::Color bc) {
  uchar		*g = fltk3::gray_ramp();
  int		i, j;
  int		clen = (int)strlen(c) - 1;
  int		chalf = clen / 2;
  int		cstep = 1;

  if (h < (w * 2)) {
    // Horizontal shading...
    if (clen >= h) cstep = 2;

    for (i = 0, j = 0; j < chalf; i ++, j += cstep) {
      // Draw the top line and points...
      fltk3::color(shade_color(g[(int)c[i]], bc));
      fltk3::xyline(x + 1, y + i, x + w - 2);

      fltk3::color(shade_color(g[c[i] - 2], bc));
      fltk3::point(x, y + i + 1);
      fltk3::point(x + w - 1, y + i + 1);

      // Draw the bottom line and points...
      fltk3::color(shade_color(g[(int)c[clen - i]], bc));
      fltk3::xyline(x + 1, y + h - i, x + w - 2);

      fltk3::color(shade_color(g[c[clen - i] - 2], bc));
      fltk3::point(x, y + h - i);
      fltk3::point(x + w - 1, y + h - i);
    }

    // Draw the interior and sides...
    i = chalf / cstep;

    fltk3::color(shade_color(g[(int)c[chalf]], bc));
    fltk3::rectf(x + 1, y + i, w - 2, h - 2 * i + 1);

    fltk3::color(shade_color(g[c[chalf] - 2], bc));
    fltk3::yxline(x, y + i, y + h - i);
    fltk3::yxline(x + w - 1, y + i, y + h - i);
  } else {
    // Vertical shading...
    if (clen >= w) cstep = 2;

    for (i = 0, j = 0; j < chalf; i ++, j += cstep) {
      // Draw the left line and points...
      fltk3::color(shade_color(g[(int)c[i]], bc));
      fltk3::yxline(x + i, y + 1, y + h - 1);

      fltk3::color(shade_color(g[c[i] - 2], bc));
      fltk3::point(x + i + 1, y);
      fltk3::point(x + i + 1, y + h);

      // Draw the right line and points...
      fltk3::color(shade_color(g[(int)c[clen - i]], bc));
      fltk3::yxline(x + w - 1 - i, y + 1, y + h - 1);

      fltk3::color(shade_color(g[c[clen - i] - 2], bc));
      fltk3::point(x + w - 2 - i, y);
      fltk3::point(x + w - 2 - i, y + h);
    }

    // Draw the interior, top, and bottom...
    i = chalf / cstep;

    fltk3::color(shade_color(g[(int)c[chalf]], bc));
    fltk3::rectf(x + i, y + 1, w - 2 * i, h - 1);

    fltk3::color(shade_color(g[c[chalf] - 2], bc));
    fltk3::xyline(x + i, y, x + w - i);
    fltk3::xyline(x + i, y + h, x + w - i);
  }
}

static void shade_round(int x, int y, int w, int h, const char *c, fltk3::Color bc) {
  uchar		*g = fltk3::gray_ramp();
  int		i;
  int		clen = (int)strlen(c) - 1;
  int		chalf = clen / 2;

  if (w>h) {
    int d = h/2;
    const int na = 8;
    for (i=0; i<chalf; i++, d--, x++, y++, w-=2, h-=2)
    {
      fltk3::color(shade_color(g[(int)c[i]], bc));
      fltk3::pie(x, y, h, h, 90.0, 135.0+i*na);
      fltk3::xyline(x+d, y, x+w-d);
      fltk3::pie(x+w-h, y, h, h, 45.0+i*na, 90.0);
      fltk3::color(shade_color(g[(int)c[i] - 2], bc));
      fltk3::pie(x+w-h, y, h, h, 315.0+i*na, 405.0+i*na);
      fltk3::color(shade_color(g[(int)c[clen - i]], bc));
      fltk3::pie(x+w-h, y, h, h, 270.0, 315.0+i*na);
      fltk3::xyline(x+d, y+h-1, x+w-d);
      fltk3::pie(x, y, h, h, 225.0+i*na, 270.0);
      fltk3::color(shade_color(g[c[(int)clen - i] - 2], bc));
      fltk3::pie(x, y, h, h, 135.0+i*na, 225.0+i*na);
    }
    fltk3::color(shade_color(g[(int)c[chalf]], bc));
    fltk3::rectf(x+d, y, w-h+1, h+1);
    fltk3::pie(x, y, h, h, 90.0, 270.0);
    fltk3::pie(x+w-h, y, h, h, 270.0, 90.0);
  } else {
    int d = w/2;
    const int na = 8;
    for (i=0; i<chalf; i++, d--, x++, y++, w-=2, h-=2)
    {
      fltk3::color(shade_color(g[(int)c[i]], bc));
      fltk3::pie(x, y, w, w, 45.0+i*na, 135.0+i*na);
      fltk3::color(shade_color(g[c[i] - 2], bc));
      fltk3::pie(x, y, w, w, 0.0, 45.0+i*na);
      fltk3::yxline(x+w-1, y+d, y+h-d);
      fltk3::pie(x, y+h-w, w, w, 315.0+i*na, 360.0);
      fltk3::color(shade_color(g[(int)c[clen - i]], bc));
      fltk3::pie(x, y+h-w, w, w, 225.0+i*na, 315.0+i*na);
      fltk3::color(shade_color(g[c[clen - i] - 2], bc));
      fltk3::pie(x, y+h-w, w, w, 180.0, 225.0+i*na);
      fltk3::yxline(x, y+d, y+h-d);
      fltk3::pie(x, y, w, w, 135.0+i*na, 180.0);
    }
    fltk3::color(shade_color(g[(int)c[chalf]], bc));
    fltk3::rectf(x, y+d, w+1, h-w+1);
    fltk3::pie(x, y, w, w, 0.0, 180.0);
    fltk3::pie(x, y+h-w, w, w, 180.0, 360.0);
  }
}


void fl_plastic_up_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*) {
  frame_rect(x, y, w, h - 1, "KLDIIJLM", c);
}


static void narrow_thin_box(int x, int y, int w, int h, fltk3::Color c) {
  if (h<=0 || w<=0) return;
  uchar *g = fltk3::gray_ramp();
  fltk3::color(shade_color(g[(uchar)'R'], c));
  fltk3::rectf(x+1, y+1, w-2, h-2);
  fltk3::color(shade_color(g[(uchar)'I'], c));
  if (w > 1) {
    fltk3::xyline(x+1, y, x+w-2);
    fltk3::xyline(x+1, y+h-1, x+w-2);
  }
  if (h > 1) {
    fltk3::yxline(x, y+1, y+h-2);
    fltk3::yxline(x+w-1, y+1, y+h-2);
  }
}


void fl_plastic_thin_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*) {
#ifdef USE_OLD_PLASTIC_BOX
  shade_rect(x + 2, y + 2, w - 4, h - 5, "RVQNOPQRSTUVWVQ", c);
  up_frame(x, y, w, h, c);
#else
  if (w > 4 && h > 4) {
    shade_rect(x + 1, y + 1, w - 2, h - 3, "RQOQSUWQ", c);
    frame_rect(x, y, w, h - 1, "IJLM", c);
  } else {
    narrow_thin_box(x, y, w, h, c);
  }
#endif // USE_OLD_PLASTIC_BOX
}


void fl_plastic_up_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) {
#ifdef USE_OLD_PLASTIC_BOX
  shade_rect(x + 2, y + 2, w - 4, h - 5, "RVQNOPQRSTUVWVQ", c);
  up_frame(x, y, w, h, c);
#else
  if (w > 8 && h > 8) {
    shade_rect(x + 1, y + 1, w - 2, h - 3, "RVQNOPQRSTUVWVQ", c);
    frame_rect(x, y, w, h - 1, "IJLM", c);
  } else {
    fl_plastic_thin_up_box(x, y, w, h, c, t);
  }
#endif // USE_OLD_PLASTIC_BOX
}


void fl_plastic_up_round(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*) {
  shade_round(x, y, w, h, "RVQNOPQRSTUVWVQ", c);
  frame_round(x, y, w, h, "IJLM", c);
}


void fl_plastic_down_frame(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*) {
  frame_rect(x, y, w, h - 1, "LLLLTTRR", c);
}


void fl_plastic_down_box(int x, int y, int w, int h, fltk3::Color c, fltk3::Box* t) {
  if (w > 6 && h > 6) {
    shade_rect(x + 2, y + 2, w - 4, h - 5, "STUVWWWVT", c);
    fl_plastic_down_frame(x, y, w, h, c, t);
  }
  else {
    narrow_thin_box(x, y, w, h, c);
  }
}


void fl_plastic_down_round(int x, int y, int w, int h, fltk3::Color c, fltk3::Box*) {
  shade_round(x, y, w, h, "STUVWWWVT", c);
  frame_round(x, y, w, h, "IJLM", c);
}

//------------------------------------------------------------------------------

void fltk3::PlasticUpBox::_draw(const Rectangle &r) const
{
  fl_plastic_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::PlasticUpBox plasticUpBox("plasticUpBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::PLASTIC_UP_BOX = &plasticUpBox;

//------------------------------------------------------------------------------

void fltk3::PlasticDownBox::_draw(const Rectangle &r) const
{
  fl_plastic_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::PlasticDownBox plasticDownBox("plasticDownBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::PLASTIC_DOWN_BOX = &plasticDownBox;

//------------------------------------------------------------------------------

void fltk3::PlasticThinUpBox::_draw(const Rectangle &r) const
{
  fl_plastic_thin_up_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::PlasticThinUpBox plasticThinUpBox("plasticThinUpBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::PLASTIC_THIN_UP_BOX = &plasticThinUpBox;

//------------------------------------------------------------------------------

void fltk3::PlasticThinDownBox::_draw(const Rectangle &r) const
{
  fl_plastic_down_box(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::PlasticThinDownBox plasticThinDownBox("plasticThinDownBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::PLASTIC_THIN_DOWN_BOX = &plasticThinDownBox;

//------------------------------------------------------------------------------

void fltk3::PlasticRoundUpBox::_draw(const Rectangle &r) const
{
  fl_plastic_up_round(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::PlasticRoundUpBox plasticRoundUpBox("plasticRoundUpBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::PLASTIC_ROUND_UP_BOX = &plasticRoundUpBox;

//------------------------------------------------------------------------------

void fltk3::PlasticRoundDownBox::_draw(const Rectangle &r) const
{
  fl_plastic_down_round(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::PlasticRoundDownBox plasticRoundDownBox("plasticRoundDownBox");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::PLASTIC_ROUND_DOWN_BOX = &plasticRoundDownBox;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

void fltk3::PlasticUpFrame::_draw(const Rectangle &r) const
{
  fl_plastic_up_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::PlasticUpFrame plasticUpFrame("plasticUpFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::PLASTIC_UP_FRAME = &plasticUpFrame;

//------------------------------------------------------------------------------

void fltk3::PlasticDownFrame::_draw(const Rectangle &r) const
{
  fl_plastic_down_frame(r.x(), r.y(), r.w(), r.h(), fltk3::color(), 0);
}

static fltk3::PlasticDownFrame plasticDownFrame("plasticDownFrame");

/*!
 Draws a raised rectangle.
 */
fltk3::Box* const fltk3::PLASTIC_DOWN_FRAME = &plasticDownFrame;

//------------------------------------------------------------------------------





//
// End of "$Id$".
//
