/
// "$Id$
/
// Xlib visual support for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2003 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

// Set the default visual according to passed switches

#include <config.h
#include <FL/Fl.H
#include <FL/x.H


#if USE_XDB
#include <X11/extensions/Xdbe.h
#endi

static int test_visual(XVisualInfo& v, int flags) 
  if (v.screen != fl_screen) return 0
#if USE_COLORMA
  if (!(flags & FL_INDEX)) 
    if (v.c_class != StaticColor && v.c_class != TrueColor) return 0
    if (v.depth <= 8) return 0; // fltk will work better in colormap mod
  
  if (flags & FL_RGB8) 
    if (v.depth < 24) return 0
  
  // for now, fltk does not like colormaps of more than 8 bits
  if ((v.c_class&1) && v.depth > 8) return 0
#els
  // simpler if we can't use colormapped visuals at all
  if (v.c_class != StaticColor && v.c_class != TrueColor) return 0
#endi
#if USE_XDB
  if (flags & FL_DOUBLE) 
    static XdbeScreenVisualInfo *xdbejunk
    if (!xdbejunk) 
      int event_base, error_base
      if (!XdbeQueryExtension(fl_display, &event_base, &error_base)) return 0
      Drawable root = RootWindow(fl_display,fl_screen)
      int numscreens = 1
      xdbejunk = XdbeGetVisualInfo(fl_display,&root,&numscreens)
      if (!xdbejunk) return 0
    
    for (int j = 0; ; j++) 
      if (j >= xdbejunk->count) return 0
      if (xdbejunk->visinfo[j].visual == v.visualid) break
    
  
#endi
  return 1


int Fl::visual(int flags) 
#if USE_XDBE == 
  if (flags & FL_DOUBLE) return 0
#endi
  fl_open_display()
  // always use default if possible
  if (test_visual(*fl_visual, flags)) return 1
  // get all the visuals
  XVisualInfo vTemplate
  int num
  XVisualInfo *visualList = XGetVisualInfo(fl_display, 0, &vTemplate, &num)
  // find all matches, use the one with greatest depth
  XVisualInfo *found = 0
  for (int i=0; i<num; i++) if (test_visual(visualList[i], flags)) 
    if (!found || found->depth < visualList[i].depth
      found = &visualList[i]
  
  if (!found) {XFree((void*)visualList); return 0;
  fl_visual = found
  fl_colormap = XCreateColormap(fl_display, RootWindow(fl_display,fl_screen)
				fl_visual->visual, AllocNone)
  return 1



/
// End of "$Id$"
/
