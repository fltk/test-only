/
// "$Id$
/
// Device class for the Fast Light Tool Kit (FLTK)
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


#include <FL/Fl_Device.H
#include <FL/Fl_Image.H
#include <FL/Fl.H


Fl_Device::Fl_Device():image_caches(0)
	Fl::get_color(FL_GRAY, bg_r_,bg_g_,bg_b_)
}
Fl_Device::~Fl_Device()
  while(image_caches)  delete image_caches
}


Fl_Image_Cache * Fl_Device::check_image_cache(Fl_Image * im)
  if(!im->cache_) return 0
  if(this != im->cache_->device) im->uncache()
  return im->cache_
}

Fl_Image_Cache * * Fl_Device::image_cache(Fl_Image * im)
  return &(im->cache_)
}

Fl_Device * Fl_Device::set_current()
  if(fl_device == this) return this
  Fl_Device * c = fl_device
  fl_device = this
  return c
}

Fl_Device * Fl_Device::current(){ return fl_device;}

// Following should go later into the subdirectorie

#ifdef __APPLE__
#  define FL_DISPLAY Fl_Carbon_Displa
#  include "carbon/Fl_Carbon_Display.H
#elif defined(WIN32
#  define FL_DISPLAY Fl_Win_Displa
#  include "win/Fl_Win_Display.H
#els
#  define FL_DISPLAY Fl_Xlib_Displa
#  include "xlib/Fl_Xlib_Display.H
#endif  // __APPLE_

FL_EXPORT FL_DISPLAY fl_disp
Fl_Device * fl_device=&fl_disp

#ifdef WIN3
  extern FL_EXPORT void pop_xmaps()
  extern FL_EXPORT void push_xmaps()
#endi




#ifdef WIN3
#include <FL/Fl.H
Fl_Device * FL_DISPLAY::set_current()
  if(fl_device == this) return this
  if(fl_device == &fl_disp) Fl::flush()
  Fl_Device * c = fl_device
  fl_device = this

  if(fl_device == &fl_disp
    pop_xmaps(); // also sets fl_gc bac
  return c
}
#endi


/
// "$Id$
/

