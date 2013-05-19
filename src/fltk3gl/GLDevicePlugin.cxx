//
// "$Id$"
//
// implementation of class Fl_Gl_Device_Plugin for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010 by Bill Spitzak and others.
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
// Please report all bugs and problems to:
//
//     http://www.fltk.org/str.php
//

#include <config.h>
#include <fltk3/Printer.h>
#include <fltk3gl/GLWindow.h>
#include "GLChoice.h"
#include <fltk3/run.h>
#ifndef __APPLE__
#include <fltk3/draw.h>
#endif

#if defined(__APPLE__)
static void imgProviderReleaseData (void *info, const void *data, size_t size)
{
  free((void *)data);
}
#endif

static void print_gl_window(fltk3::GLWindow *glw, int x, int y, int height)
{
#if defined(__APPLE__)
  const int bytesperpixel = 4;
#else
  const int bytesperpixel = 3;
#endif
  glw->make_current();
  // Read OpenGL context pixels directly.
  // For extra safety, save & restore OpenGL states that are changed
  glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
  glPixelStorei(GL_PACK_ALIGNMENT, 4); /* Force 4-byte alignment */
  glPixelStorei(GL_PACK_ROW_LENGTH, 0);
  glPixelStorei(GL_PACK_SKIP_ROWS, 0);
  glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
  // Read a block of pixels from the frame buffer
  int mByteWidth = glw->w() * bytesperpixel;                
  mByteWidth = (mByteWidth + 3) & ~3;    // Align to 4 bytes
  uchar *baseAddress = (uchar*)malloc(mByteWidth * glw->h());
  glReadPixels(0, 0, glw->w(), glw->h(), 
#if defined(__APPLE__)
	       GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
#else
	       GL_RGB, GL_UNSIGNED_BYTE,
#endif
	       baseAddress);
  glPopClientAttrib();
#if defined(__APPLE__)
// kCGBitmapByteOrder32Host and CGBitmapInfo are supposed to arrive with 10.4
// but some 10.4 don't have kCGBitmapByteOrder32Host, so we play a little #define game
#if MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_4
#define kCGBitmapByteOrder32Host 0
#define CGBitmapInfo CGImageAlphaInfo
#elif ! defined(kCGBitmapByteOrder32Host)
#ifdef __BIG_ENDIAN__
#define kCGBitmapByteOrder32Host (4 << 12)
#else    /* Little endian. */
#define kCGBitmapByteOrder32Host (2 << 12)
#endif
#endif
  CGColorSpaceRef cSpace = CGColorSpaceCreateDeviceRGB();
  CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, baseAddress, mByteWidth * glw->h(), imgProviderReleaseData);
  CGImageRef image = CGImageCreate(glw->w(), glw->h(), 8, 8*bytesperpixel, mByteWidth, cSpace, 
				   (CGBitmapInfo)(kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Host), 
				   provider, NULL, false, kCGRenderingIntentDefault);
  if(image == NULL) return;
  CGContextSaveGState(fl_gc);
  CGContextTranslateCTM(fl_gc, 0, height);
  CGContextScaleCTM(fl_gc, 1.0f, -1.0f);
  CGRect rect = { { x, height - y - glw->h() }, { glw->w(), glw->h() } };
  Fl_X::q_begin_image(rect, 0, 0, glw->w(), glw->h());
  CGContextDrawImage(fl_gc, rect, image);
  Fl_X::q_end_image();
  CGContextRestoreGState(fl_gc);
  CGImageRelease(image);
  CGColorSpaceRelease(cSpace);
  CGDataProviderRelease(provider);  
#else
  fltk3::draw_image(baseAddress + (glw->h() - 1) * mByteWidth, x, y , glw->w(), glw->h(), bytesperpixel, - mByteWidth);
  free(baseAddress);
#endif // __APPLE__
}

/**
 This class will make sure that OpenGL printing is available if fltk_gl
 was linked to the program.
 */
class Fl_Gl_Device_Plugin : public fltk3::DevicePlugin {
public:
  Fl_Gl_Device_Plugin() : fltk3::DevicePlugin(name()) { }
  virtual const char *name() { return "opengl.device.fltk.org"; }
  virtual int print(fltk3::Widget *w, int x, int y, int height) {
    fltk3::GLWindow *glw = w->as_gl_window();
    if (!glw) return 0;
    print_gl_window(glw, x, y, height);
    return 1; 
  }
};

static Fl_Gl_Device_Plugin Gl_Device_Plugin;

// The purpose of this variable, used in fltk3::GLWindow.cxx, is only to force this file to be loaded
// whenever fltk3::GLWindow.cxx is loaded, that is, whenever fltk_gl is.
FLTK3_EXPORT int fl_gl_load_plugin = 0;

//
// End of "$Id$".
//
