//
// "$Id: Fl_Device.cxx,v 1.1.2.1 2004/03/29 21:22:35 rokan Exp $"
//
// Device class for the Fast Light Tool Kit (FLTK).
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


#include <FL/Fl_Device.H>
#include <FL/Fl_Image.H>

Fl_Device::~Fl_Device(){
  while(image_caches)  delete image_caches;
};



Fl_Image_Cache * Fl_Device::check_image_cache(Fl_Image * im){
   if(!im->cache_) return 0;
   if(this != im->cache_->device) im->uncache();
   return im->cache_;
};


// Following should go later into the subdirectories

#ifdef __APPLE__ 
#  define FL_DISPLAY Fl_Carbon_Display
#  include "carbon/Fl_Carbon_Display.H"
#elif defined(WIN32)
#  define FL_DISPLAY Fl_Win_Display
#  include "win/Fl_Win_Display.H"
#else
#  define FL_DISPLAY Fl_Xlib_Display
#  include "xlib/Fl_Xlib_Display.H"
#endif  // __APPLE__

FL_EXPORT FL_DISPLAY fl_disp;
Fl_Device * fl_device=&fl_disp;

#ifdef WIN32
  extern FL_EXPORT void pop_xmaps();
  extern FL_EXPORT void push_xmaps();
#endif

Fl_Device * Fl_Device::current(){ return fl_device;};

Fl_Device * Fl_Device::set_current(){
  if(fl_device == this) return this;
  Fl_Device * c = fl_device;
  fl_device = this;
  return c;
};

#ifdef WIN32
#include <FL/Fl.H>
Fl_Device * FL_DISPLAY::set_current(){
  if(fl_device == this) return this;
  if(fl_device == &fl_disp) Fl::flush();
  Fl_Device * c = fl_device;
  fl_device = this;

  if(fl_device == &fl_disp)
    pop_xmaps(); // also sets fl_gc back
  return c;
};
#endif


//
// "$Id: Fl_Device.cxx,v 1.1.2.1 2004/03/29 21:22:35 rokan Exp $"
//

