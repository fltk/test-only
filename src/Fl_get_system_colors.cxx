//
// "$Id: Fl_get_system_colors.cxx,v 1.10 1999/11/20 04:42:46 vincent Exp $"
//
// System color support for the Fast Light Tool Kit (FLTK).
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

#include <FL/x.H>
#include <FL/Fl_Color.H>
#include <FL/math.h>
#include <config.h>

void fl_background(Fl_Color c) {
  // replace the gray ramp so that FL_GRAY is this color
  int r = (c>>24)&255;
  if (!r) r = 1; else if (r==255) r = 254;
  double powr = log(r/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
  int g = (c>>16)&255;
  if (!g) g = 1; else if (g==255) g = 254;
  double powg = log(g/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
  int b = (c>>8)&255;
  if (!b) b = 1; else if (b==255) b = 254;
  double powb = log(b/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
  for (int i = 0; i < FL_NUM_GRAY; i++) {
    double gray = i/(FL_NUM_GRAY-1.0);
    fl_set_color(fl_gray_ramp(i),
		 fl_rgb(uchar(pow(gray,powr)*255+.5),
			uchar(pow(gray,powg)*255+.5),
			uchar(pow(gray,powb)*255+.5)));
  }
}

void fl_foreground(Fl_Color c) {
  fl_set_color(FL_NO_COLOR, c);
}

#include <FL/Fl_Input.H>
void fl_text_background(Fl_Color c) {
  Fl_Style& s = *Fl_Input::default_style;
  s.set_color(c);
  s.set_text_color(fl_contrast(s.text_color, c));
  s.set_selection_color(fl_contrast(s.selection_color, c));
  s.set_selection_text_color(fl_contrast(s.selection_text_color,
					 s.selection_color));
  s.set_off_color(fl_contrast(s.off_color, c)); // cursor color
}

// Named color parser:
#ifdef WIN32
#include <stdio.h>
#endif

Fl_Color fl_rgb(const char* name) {
  if (!name || !*name) return FL_NO_COLOR;
#ifdef WIN32
  // simulation of XParseColor:
  if (*name == '#') name++;
  if (name[0]=='0' && name[1]=='x') name += 2;
  int n = strlen(name);
  int m = n/3;
  const char *pattern = 0;
  switch(m) {
  case 1: pattern = "%1x%1x%1x"; break;
  case 2: pattern = "%2x%2x%2x"; break;
  case 3: pattern = "%3x%3x%3x"; break;
  case 4: pattern = "%4x%4x%4x"; break;
  default: return FL_NO_COLOR;
  }
  int R,G,B; if (sscanf(name, pattern, &R,&G,&B) != 3) return FL_NO_COLOR;
  switch(m) {
  case 1: R *= 0x11; G *= 0x11; B *= 0x11; break;
  case 3: R >>= 4; G >>= 4; B >>= 4; break;
  case 4: R >>= 8; G >>= 8; B >>= 8; break;
  }
  Fl_Color c = fl_rgb(R,G,B);
#else
  XColor x;
  fl_open_display();
  if (!XParseColor(fl_display, fl_colormap, name, &x)) return FL_NO_COLOR;
  Fl_Color c = fl_rgb(x.red>>8, x.green>>8, x.blue>>8);
#endif
  if (!c) c = FL_BLACK;
  return c;
}

//
// End of "$Id: Fl_get_system_colors.cxx,v 1.10 1999/11/20 04:42:46 vincent Exp $".
//
