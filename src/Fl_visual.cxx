//
// "$Id: Fl_visual.cxx,v 1.14 2002/12/09 04:52:27 spitzak Exp $"
//
// Visual support for the Fast Light Tool Kit (FLTK).
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

// Set the default visual according to passed switches:

#include <fltk/visual.h>
#include <fltk/x.h>
#include <config.h>

#ifdef _WIN32
bool fltk::visual(int flags) {
  if (flags & DOUBLE_BUFFER) return false;
  bool ret = true;
  HDC screen = GetDC(0);
  if ((!(flags & INDEXED_COLOR) && GetDeviceCaps(screen, BITSPIXEL) <= 8) ||
     ((flags & RGB24_COLOR) && GetDeviceCaps(screen, BITSPIXEL)<24)) ret = false;
  ReleaseDC(0, screen);
  return ret;
}
#else

#if USE_XDBE
#define Window XWindow
#include <X11/extensions/Xdbe.h>
#undef Window
#endif

using namespace fltk;

static int test_visual(XVisualInfo& v, int flags) {
  if (v.screen != xscreen) return 0;
#if USE_COLORMAP
  if (!(flags & INDEXED_COLOR)) {
    if (v.c_class != StaticColor && v.c_class != TrueColor) return 0;
    if (v.depth <= 8) return 0; // fltk will work better in colormap mode
  }
  if (flags & RGB24_COLOR) {
    if (v.depth < 24) return 0;
  }
  // for now, fltk does not like colormaps of more than 8 bits:
  if ((v.c_class&1) && v.depth > 8) return 0;
#else
  // simpler if we can't use colormapped visuals at all:
  if (v.c_class != StaticColor && v.c_class != TrueColor) return 0;
#endif
#if USE_XDBE
  if (flags & DOUBLE_BUFFER) {
    static XdbeScreenVisualInfo *xdbejunk;
    if (!xdbejunk) {
      int event_base, error_base;
      if (!XdbeQueryExtension(xdisplay, &event_base, &error_base)) return 0;
      XWindow root = RootWindow(xdisplay, xscreen);
      int numscreens = 1;
      xdbejunk = XdbeGetVisualInfo(xdisplay,&root,&numscreens);
      if (!xdbejunk) return 0;
    }
    for (int j = 0; ; j++) {
      if (j >= xdbejunk->count) return 0;
      if (xdbejunk->visinfo[j].visual == v.visualid) break;
    }
  }
#endif
  return 1;
}

bool fltk::visual(int flags) {
#if USE_XDBE == 0
  if (flags & DOUBLE_BUFFER) return false;
#endif
  open_display();
  // always use default if possible:
  if (test_visual(*xvisual, flags)) return true;
  // get all the visuals:
  XVisualInfo vTemplate;
  int num;
  XVisualInfo *visualList = XGetVisualInfo(xdisplay, 0, &vTemplate, &num);
  // find all matches, use the one with greatest depth:
  XVisualInfo *found = 0;
  for (int i=0; i<num; i++) if (test_visual(visualList[i], flags)) {
    if (!found || found->depth < visualList[i].depth)
      found = &visualList[i];
  }
  if (!found) {XFree((void*)visualList); return false;}
  xvisual = found;
  xcolormap = XCreateColormap(xdisplay, RootWindow(xdisplay,xscreen),
			      xvisual->visual, AllocNone);
  return true;
}

#endif

//
// End of "$Id: Fl_visual.cxx,v 1.14 2002/12/09 04:52:27 spitzak Exp $".
//
