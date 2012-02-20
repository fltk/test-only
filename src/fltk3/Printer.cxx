//
// "$Id$"
//
// Encompasses platform-specific printing-support code and 
// PostScript output code for the Fast Light Tool Kit (FLTK).
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

#include <fltk3/Printer.h>

#ifdef __APPLE__
//#include "Fl_Quartz_Printer.mm"
#elif defined(WIN32)
#include "GDIPrinter.cxx"
#endif

#include "PostScript.cxx"

// print dialog customization strings
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_title = "Print";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_printer = "Printer:";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_range = "Print Range";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_copies = "Copies";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_all = "All";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_pages = "Pages";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_from = "From:";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_to = "To:";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_properties = "Properties...";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_copyNo = "# Copies:";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_print_button = "Print";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_cancel_button = "Cancel";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::dialog_print_to_file = "Print To File";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::property_title = "Printer Properties";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::property_pagesize = "Page Size:";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::property_mode = "Output Mode:";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::property_use = "Use";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::property_save = "Save";
/** [this text may be customized at run-time] */
const char *fltk3::Printer::property_cancel = "Cancel";

const char *fltk3::Printer::class_id = "fltk3::Printer";
#if defined(__APPLE__) || defined(WIN32) || defined(FLTK3_DOXYGEN)
const char *fltk3::SystemPrinter::class_id = fltk3::Printer::class_id;
#endif
#if !( defined(__APPLE__) || defined(WIN32) )
const char *fltk3::PostScriptPrinter::class_id = fltk3::Printer::class_id;
#endif

#if defined(__APPLE__) || defined(WIN32)
void fltk3::SystemPrinter::set_current(void)
{
#ifdef __APPLE__
  fl_gc = (CGContextRef)gc;
#elif defined(WIN32)
  fl_gc = (HDC)gc;
#endif
  this->SurfaceDevice::set_current();
}

void fltk3::SystemPrinter::origin(int *x, int *y)
{
  PagedDevice::origin(x, y);
}

#endif

fltk3::Printer::Printer(void) {
#if defined(WIN32) || defined(__APPLE__)
  printer = new fltk3::SystemPrinter();
#else
  printer = new fltk3::PostScriptPrinter();
#endif
  SurfaceDevice::driver(printer->driver());
}

int fltk3::Printer::start_job(int pagecount, int *frompage, int *topage)
{
  return printer->start_job(pagecount, frompage, topage);
}

int fltk3::Printer::start_page(void)
{
  return printer->start_page();
}

int fltk3::Printer::printable_rect(int *w, int *h)
{
  return printer->printable_rect(w, h);
}

void fltk3::Printer::margins(int *left, int *top, int *right, int *bottom)
{
  printer->margins(left, top, right, bottom);
}

void fltk3::Printer::origin(int *x, int *y)
{
  printer->origin(x, y);
}

void fltk3::Printer::origin(int x, int y)
{
  printer->origin(x, y);
}

void fltk3::Printer::scale(float scale_x, float scale_y)
{
  printer->scale(scale_x, scale_y);
}

void fltk3::Printer::rotate(float angle)
{
  printer->rotate(angle);
}

void fltk3::Printer::translate(int x, int y)
{
  printer->translate(x, y);
}

void fltk3::Printer::untranslate(void)
{
  printer->untranslate();
}

int fltk3::Printer::end_page (void)
{
  return printer->end_page();
}

void fltk3::Printer::end_job (void)
{
  printer->end_job();
}

void fltk3::Printer::set_current(void)
{
  printer->set_current();
}

fltk3::GraphicsDriver* fltk3::Printer::driver(void)
{
  return printer->driver();
}

fltk3::Printer::~Printer(void)
{
  delete printer;
}


//
// End of "$Id$".
//
