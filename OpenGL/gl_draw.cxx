//
// "$Id: gl_draw.cxx,v 1.22 2002/07/01 15:28:18 spitzak Exp $"
//
// OpenGL drawing support routines for the Fast Light Tool Kit (FLTK).
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

// Functions from <fltk/gl.h>
// See also Fl_Gl_Window and gl_start.C

#include <config.h>
#if HAVE_GL

#include <fltk/fl_draw.h>
#include <fltk/gl.h>
#include "Fl_Gl_Choice.h"
#include <string.h>

// binary tree of all the fonts+sizes we have made so far:
struct FontSize {
  Fl_Font font;
  double size;
  FontSize* left, *right;
  int listbase;
#if USE_XFT
  XFontStruct* xfont;
#endif
};
static FontSize* root, *current;

void gl_font(Fl_Font font, double size) {
  fl_font(font, size); // necessary so fl_measure() works
  size = fl_size(); // get the rounded value
  if (!current || current->font != font || current->size != size) {
    FontSize** p = &root;
    while (*p) {
      if (font < (*p)->font) p = &((*p)->left);
      else if (font > (*p)->font) p = &((*p)->right);
      else if (size < (*p)->size) p = &((*p)->left);
      else if (size > (*p)->size) p = &((*p)->right);
      else {current = *p; goto GOTIT;}
    }
    *p = current = new FontSize;
    current->font = font;
    current->size = size;
    current->left = current->right = 0;
    current->listbase = glGenLists(256);
#ifdef _WIN32
    int base = fl_textmetric()->tmFirstChar;
    int size = fl_textmetric()->tmLastChar - base + 1;
    HDC hdc = GetDC(0);
    HFONT oldFid = (HFONT)SelectObject(hdc, fl_xfont());
    wglUseFontBitmaps(hdc, base, size, current->listbase+base); 
    SelectObject(hdc, oldFid);
#else
    XFontStruct* xfont = fl_xfont();
#if USE_XFT
    current->xfont = xfont;
#endif
    int base = xfont->min_char_or_byte2;
    int size = xfont->max_char_or_byte2-base+1;
    glXUseXFont(xfont->fid, base, size, current->listbase+base);
#endif
  }
 GOTIT:
  glListBase(current->listbase);
}

void gl_font(int fontid, double size) {
  gl_font(fl_fonts + (fontid % 16), size);
}

void gl_draw(const char* str, int n) {
  glCallLists(n, GL_UNSIGNED_BYTE, str);
}

void gl_draw(const char* str, int n, double x, double y, double z) {
  glRasterPos3d(x, y, z);
  gl_draw(str, n);
}

void gl_draw(const char* str) {
  gl_draw(str, strlen(str));
}

void gl_draw(const char* str, double x, double y, double z) {
  gl_draw(str, strlen(str), x, y, z);
}

#if USE_XFT
// We must use the X font, the normal functions will use the Xft font:

double gl_height() {
  return current->xfont->ascent+current->xfont->descent;
}

double gl_descent() { return current->xfont->descent; }

double gl_width(const char* s, int n) {
  return XTextWidth(current->xfont, s, n);
}

double gl_width(const char* s) {return gl_width(s, strlen(s));}

#else
// The old X and Windows versions use exactly the same fonts for OpenGL
// and for normal drawing, so we can share the functions:

double gl_height() {return fl_height();}
double gl_descent() {return fl_descent();}
double gl_width(const char* s) {return fl_width(s);}
double gl_width(const char* s, int n) {return fl_width(s,n);}

#endif

void gl_rect(int x, int y, int w, int h) {
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

void gl_color(Fl_Color i) {
#if USE_GL_OVERLAY
#ifndef _WIN32
  if (fl_overlay) {glIndexi(int(fl_xpixel(i))); return;}
#else
  if (fl_overlay && fl_overlay_depth) {
    i = fl_nearest_color(i); // convert to 8-bit color
    if (fl_overlay_depth < 8) {
      // only black & white produce the expected colors.  This could
      // be improved by fixing the colormap set in Fl_Gl_Overlay.cxx
      unsigned size = 1<<fl_overlay_depth;
      if (!i) glIndexi(size-2);
      else if (i >= size-2) glIndexi(size-1);
      else glIndexi(i);
    } else {
      glIndexi(i ? i : FL_GRAY_RAMP);
    }
    return;
  }   
#endif
#endif
  Fl_Color c = fl_get_color(i);
  glColor3ub((uchar)(c>>24), (uchar)(c>>16), (uchar)(c>>8));
}
  
void gl_draw_image(const uchar* b, int x, int y, int w, int h, int d, int ld) {
  if (!ld) ld = w*d;
  glPixelStorei(GL_UNPACK_ROW_LENGTH, ld/d);
  glRasterPos2i(x, y);
  glDrawPixels(w, h, d<4?GL_RGB:GL_RGBA, GL_UNSIGNED_BYTE, (const ulong*)b);
}

#endif

//
// End of "$Id: gl_draw.cxx,v 1.22 2002/07/01 15:28:18 spitzak Exp $".
//
