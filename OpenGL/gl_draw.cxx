//
// "$Id$"
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
#include <fltk/Font.h>
#include <fltk/gl.h>
#include <fltk/utf.h>
#include <string.h>
#include "GlChoice.h"
using namespace fltk;

extern GLContext fl_current_glcontext;

// Binary tree of all the glListBase assignments we have made so far:
struct FontSize {
  FontSize* left, *right;
  int listbase;
  HFONT xfont;
#if USE_QUARTZ
  float size;
#endif
};
static FontSize* root, *current;
static HFONT current_xfont;

void fltk::glsetfont(fltk::Font* font, float size) {
#if USE_QUARTZ
  size = int(size);
#endif
  setfont(font, size); // necessary so measure() works
  current_xfont = xfont();
  if (!fl_current_glcontext) return;
  if (!current || current->xfont != current_xfont
#if USE_QUARTZ
      || current->size != size
#endif
      ) {
    FontSize** p = &root;
    while (*p) {
      if (current_xfont < (*p)->xfont) p = &((*p)->left);
      else if (current_xfont > (*p)->xfont) p = &((*p)->right);
#if USE_QUARTZ
      else if (size < (*p)->size) p = &((*p)->left);
      else if (size > (*p)->size) p = &((*p)->right);
#endif
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
#elif defined(__APPLE__)
    current->size = size;
    int attrib; const char* name = font->name(&attrib);
    CFStringRef cfname = CFStringCreateWithCString(0L, name, kCFStringEncodingASCII);
    short cfont;
    GetFNum(CFStringGetPascalStringPtr(cfname, kCFStringEncodingMacRoman),&cfont);
    CFRelease(cfname);
    aglUseFont(aglGetCurrentContext(), cfont, attrib, (int)size, 0, 256, current->listbase);
#else
#error
#endif
  }
 GOTIT:
  glListBase(current->listbase);
}

#define WCBUFLEN 256

/*! Draw text at the current glRasterPos in the current font selected
  with fltk::glsetfont(). You can use glRasterPos2f() or similar calls
  to set the position before calling this.
*/
void fltk::gldrawtext(const char* text, int n) {
  char localbuffer[WCBUFLEN];
  char* buffer = localbuffer;
  char* mallocbuffer = 0;
  int count = utf8toa(text, n, buffer, WCBUFLEN);
  if (count >= WCBUFLEN) {
    if (count == n) {
      // all ascii or errors, no conversion needed
      buffer = (char*)text;
    } else {
      buffer = mallocbuffer = new char[count+1];
      count = utf8toa(text, n, buffer, count+1);
    }
  }
  glCallLists(count, GL_UNSIGNED_BYTE, buffer);
  delete[] mallocbuffer;
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

#ifdef _WIN32
// Emulate glWindowPos2i on Windows. Unfortunatly this is rather lame
// and only works if ortho() has already been done.
void glWindowPos2i(int x, int y) {
  if (x < 0 || y < 0) {
    glRasterPos2i(0,0);
    glBitmap(0,0,0,0,x,y,0);
  } else {
    glRasterPos2i(x,y);
  }
}
#endif

#endif

//
// End of "$Id$".
//
