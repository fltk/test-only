//
// "$Id$"
//
// Printing support for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Printer_H
#define Fl_Printer_H

#include <fltk3/Printer.h>
#include <FL/Fl_Paged_Device.H>
#include <FL/Fl_PostScript.H>

class Fl_Printer : public Fl_Paged_Device {
public:
  Fl_Printer() {
    _p->wrapper(NULL);
    delete _p;
    _p = new fltk3::Printer();
    _p->wrapper(this);
  }
  
#if 0 // can't wrap!
      // FIXME: 123-1
  static const char *dialog_title; 
  static const char *dialog_printer; 
  static const char *dialog_range; 
  static const char *dialog_copies; 
  static const char *dialog_all; 
  static const char *dialog_pages;
  static const char *dialog_from;
  static const char *dialog_to;
  static const char *dialog_properties;
  static const char *dialog_copyNo;
  static const char *dialog_print_button;
  static const char *dialog_cancel_button;
  static const char *dialog_print_to_file;
  static const char *property_title;
  static const char *property_pagesize;
  static const char *property_mode;
  static const char *property_use;
  static const char *property_save;
  static const char *property_cancel;
#endif
};

#endif // Fl_Printer_H

//
// End of "$Id$"
//
