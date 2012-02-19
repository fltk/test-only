//
// "$Id$"
//
// implementation of fltk3::Device class for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2011 by Bill Spitzak and others.
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

#include <fltk3/run.h>
#include <fltk3/Device.h>
#include <fltk3/Image.h>

const char *fltk3::Device::class_id = "fltk3::Device";
const char *fltk3::SurfaceDevice::class_id = "fltk3::SurfaceDevice";
const char *fltk3::DisplayDevice::class_id = "fltk3::DisplayDevice";
const char *fltk3::GraphicsDriver::class_id = "fltk3::GraphicsDriver";
#if defined(__APPLE__) || defined(FLTK3_DOXYGEN)
const char *fltk3::QuartzGraphicsDriver::class_id = "fltk3::QuartzGraphicsDriver";
#endif
#if defined(WIN32) || defined(FLTK3_DOXYGEN)
const char *fltk3::GDIGraphicsDriver::class_id = "fltk3::GDIGraphicsDriver";
#endif
#if !(defined(__APPLE__) || defined(WIN32))
namespace fltk3 {
  const char *XlibGraphicsDriver::class_id = "fltk3::XlibGraphicsDriver";
}
#endif


/** \brief Use this drawing surface for future graphics requests. */
void fltk3::SurfaceDevice::set_current(void)
{
  fltk3::graphics_driver = _driver;
  _surface = this;
}

const fltk3::GraphicsDriver::matrix fltk3::GraphicsDriver::m0 = {1, 0, 0, 1, 0, 0};

fltk3::GraphicsDriver::GraphicsDriver() {
  font_ = 0;
  size_ = 0;
  sptr=0; rstackptr=0; 
  fl_clip_state_number=0;
  m = m0; 
  fl_matrix = &m; 
  p = (XPOINT *)0;
  font_descriptor_ = NULL;
  p_size = 0;
  n = 0;
};

void fltk3::GraphicsDriver::text_extents(const char*t, int n, int& dx, int& dy, int& w, int& h)
{
  w = (int)width(t, n);
  h = - height();
  dx = 0;
  dy = descent();
}

fltk3::DisplayDevice::DisplayDevice(fltk3::GraphicsDriver *graphics_driver) : fltk3::SurfaceDevice( graphics_driver) {
#ifdef __APPLE__
  SInt32 versionMajor = 0;
  SInt32 versionMinor = 0;
  SInt32 versionBugFix = 0;
  Gestalt( gestaltSystemVersionMajor, &versionMajor );
  Gestalt( gestaltSystemVersionMinor, &versionMinor );
  Gestalt( gestaltSystemVersionBugFix, &versionBugFix );
  fl_mac_os_version = versionMajor * 10000 + versionMinor * 100 + versionBugFix;
#endif
};


//
// End of "$Id$".
//
