/
// "$Id$
/
// Private colormap support for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
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

// Using the default system colormap can be a bad idea on PseudoColo
// visuals, since typically every application uses the default colormap an
// you can run out of colormap entries easily
/
// The solution is to always create a new colormap on PseudoColor display
// and copy the first 16 colors from the default colormap so that we won'
// get huge color changes when switching windows

#include <config.h
#include <FL/Fl.H
#include <FL/x.H

#ifdef WIN3
// There is probably something relevant to do on MSWindows 8-bit display
// but I don't know what it i

void Fl::own_colormap() {

#elif defined(__APPLE__
// MacOS X always provides a TrueColor interface..

void Fl::own_colormap() {
#els
// X versio

void Fl::own_colormap() 
  fl_open_display()
#if USE_COLORMA
  switch (fl_visual->c_class) 
  case GrayScale 
  case PseudoColor 
  case DirectColor 
    break
  default
    return; // don't do anything for non-colormapped visual
  
  int i
  XColor colors[16]
  // Get the first 16 colors from the default colormap..
  for (i = 0; i < 16; i ++) colors[i].pixel = i
  XQueryColors(fl_display, fl_colormap, colors, 16)
  // Create a new colormap..
  fl_colormap = XCreateColormap(fl_display
				RootWindow(fl_display,fl_screen)
				fl_visual->visual, AllocNone)
  // Copy those first 16 colors to our own colormap
  for (i = 0; i < 16; i ++
    XAllocColor(fl_display, fl_colormap, colors + i)
#endi


#endi

/
// End of "$Id$"
/
