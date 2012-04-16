//
// "$Id$"
//
// Support for graphics output to PostScript file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2012 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#ifndef Fl_PostScript_H
#define Fl_PostScript_H
#include <fltk3/PostScript.h>

class FL_EXPORT Fl_PostScript_File_Device : public Fl_Paged_Device {
public:
  Fl_PostScript_File_Device() : Fl_Paged_Device() {
    _p->wrapper(NULL);
    delete _p;
    _p = new fltk3::PostScriptFileDevice();
    _p->wrapper(this);
  }
  ~Fl_PostScript_File_Device() { }
  int start_job(int pagecount, enum Fl_Paged_Device::Page_Format format = Fl_Paged_Device::A4, 
		enum Fl_Paged_Device::Page_Layout layout = Fl_Paged_Device::PORTRAIT) {
    return ((fltk3::PostScriptFileDevice*)_p)->start_job(pagecount, (fltk3::PagedDevice::Page_Format)format, 
							 (fltk3::PagedDevice::Page_Layout)layout);
    }
  int start_job(FILE *ps_output, int pagecount, enum Fl_Paged_Device::Page_Format format = Fl_Paged_Device::A4, 
		enum Fl_Paged_Device::Page_Layout layout = Fl_Paged_Device::PORTRAIT) {
    return ((fltk3::PostScriptFileDevice*)_p)->start_job(ps_output, pagecount, (fltk3::PagedDevice::Page_Format)format, 
							 (fltk3::PagedDevice::Page_Layout)layout);
    }
};

class Fl_PostScript_Graphics_Driver : public Fl_Graphics_Driver {
public:
  Fl_PostScript_Graphics_Driver() : Fl_Graphics_Driver() {
    _p->wrapper(NULL);
    delete _p;
    _p = new fltk3::PostScriptGraphicsDriver();
    _p->wrapper(this);
  }
};

#endif // Fl_PostScript_H

//
// End of "$Id$"
//
