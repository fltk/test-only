//
// "$Id: gl_draw.cxx,v 1.30 2004/07/27 07:03:06 spitzak Exp $"
//
// OpenGL drawing support routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

// Functions from <fltk/gl.h>
// See also GlWindow and gl_start.C

#include <config.h>
#if HAVE_GL

#include <fltk/draw.h>
#include <fltk/gl.h>
#include <fltk/utf.h>
#include <string.h>
#include "GlChoice.h"
using namespace fltk;

extern GLContext fl_current_glcontext;

#if USE_X11
#undef HFONT
#define HFONT XFontStruct*
#endif

// Binary tree of all the glListBase assignments we have made so far:
struct FontSize {
  HFONT xfont;
  FontSize* left, *right;
  int listbase;
};
static FontSize* root, *current;
static HFONT current_xfont;

void fltk::glsetfont(fltk::Font* font, float size) {
  setfont(font, size); // necessary so measure() works
  current_xfont = fltk::xfont();
  if (!fl_current_glcontext) return;
  if (!current || current->xfont != current_xfont) {
    FontSize** p = &root;
    while (*p) {
      if (current_xfont < (*p)->xfont) p = &((*p)->left);
      else if (current_xfont > (*p)->xfont) p = &((*p)->right);
      else {current = *p; goto GOTIT;}
    }
    *p = current = new FontSize;
    current->xfont = current_xfont;
    current->left = current->right = 0;
    current->listbase = glGenLists(256);
#if USE_X11
    int base = current_xfont->min_char_or_byte2;
    int last = current_xfont->max_char_or_byte2;
    if (last > 255) last = 255;
    int size = last-base+1;
    glXUseXFont(current_xfont->fid, base, size, current->listbase+base);
#elif defined(_WIN32)
    int base = textmetric()->tmFirstChar;
    int last = textmetric()->tmLastChar;
    if (last > 255) last = 255;
    int size = last-base+1;
    HDC hdc = GetDC(0);
    HFONT oldFid = (HFONT)SelectObject(hdc, current_xfont);
    wglUseFontBitmaps(hdc, base, size, current->listbase+base); 
    SelectObject(hdc, oldFid);
#else
#error
#endif
  }
 GOTIT:
  glListBase(current->listbase);
}

void fltk::gldrawtext(const char* str, int n) {
  char buffer[1024];
  int count = utf8toa(str, n, buffer, 1024);
  glCallLists(count, GL_UNSIGNED_BYTE, buffer);
}

void fltk::gldrawtext(const char* str, int n, float x, float y, float z) {
  glRasterPos3f(x, y, z);
  gldrawtext(str, n);
}

void fltk::gldrawtext(const char* str) {
  gldrawtext(str, strlen(str));
}

void fltk::gldrawtext(const char* str, float x, float y, float z) {
  gldrawtext(str, strlen(str), x, y, z);
}

#if USE_XFT
// We must use the X font, the normal functions will use the Xft font:

float fltk::glgetascent() { return current_xfont->ascent; }

float fltk::glgetdescent() { return current_xfont->descent; }

float fltk::glgetwidth(const char* s, int n) {
  return XTextWidth(current_xfont, s, n);
}

float fltk::glgetwidth(const char* s) {return glgetwidth(s, strlen(s));}

#else
// The old X and Windows versions use exactly the same fonts for OpenGL
// and for normal drawing, so we can share the functions:

float fltk::glgetascent() {return getascent();}
float fltk::glgetdescent() {return getdescent();}
float fltk::glgetwidth(const char* s) {return getwidth(s);}
float fltk::glgetwidth(const char* s, int n) {return getwidth(s,n);}

#endif

void fltk::glstrokerect(int x, int y, int w, int h) {
  if (w < 0) {w = -w; x = x-w;}
  if (h < 0) {h = -h; y = y-h;}
  glBegin(GL_LINE_STRIP);
  glVertex2i(x+w-1, y+h-1);
  glVertex2i(x+w-1, y);
  glVertex2i(x, y);
  glVertex2i(x, y+h-1);
  glVertex2i(x+w, y+h-1);
  glEnd();
}

#if USE_GL_OVERLAY
extern bool fl_overlay;
#ifdef _WIN32
extern int fl_overlay_depth;
#endif
#endif

void fltk::glsetcolor(Color i) {
#if USE_GL_OVERLAY
#ifndef _WIN32
  if (fl_overlay) {glIndexi(int(xpixel(i))); return;}
#else
  if (fl_overlay && fl_overlay_depth) {
    i = nearest_index(i); // convert to 8-bit color
    if (fl_overlay_depth < 8) {
      // only black & white produce the expected colors.  This could
      // be improved by fixing the colormap set in GlOverlay.cxx
      unsigned size = 1<<fl_overlay_depth;
      if (!i) glIndexi(size-2);
      else if (i >= size-2) glIndexi(size-1);
      else glIndexi(i);
    } else {
      glIndexi(i ? i : BLACK);
    }
    return;
  }   
#endif
#endif
  uchar r,g,b; split_color(i,r,g,b);
  glColor3ub(r,g,b);
}
  
void fltk::gldrawimage(const uchar* b, int x, int y, int w, int h, int d, int ld) {
  if (!ld) ld = w*d;
  glPixelStorei(GL_UNPACK_ROW_LENGTH, ld/d);
  glRasterPos2i(x, y);
  glDrawPixels(w, h, d<4?GL_RGB:GL_RGBA, GL_UNSIGNED_BYTE, (const unsigned long*)b);
}

#endif

//
// End of "$Id: gl_draw.cxx,v 1.30 2004/07/27 07:03:06 spitzak Exp $".
//
