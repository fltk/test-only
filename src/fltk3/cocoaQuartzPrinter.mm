//
// "$Id$"
//
// Mac OS X-specific printing support (objective-c++) for the Fast Light Tool Kit (FLTK).
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

#ifdef __APPLE__
#include <fltk3/Printer.h>

#include <fltk3/run.h>
#include <fltk3/ask.h>
#include <fltk3/draw.h>
#import <Cocoa/Cocoa.h>

extern void fl_quartz_restore_line_style_();

fltk3::SystemPrinter::SystemPrinter(void)
{
  x_offset = 0;
  y_offset = 0;
  scale_x = scale_y = 1.;
  gc = 0;
  driver(fltk3::DisplayDevice::display_device()->driver());
}

fltk3::SystemPrinter::~SystemPrinter(void) {}

int fltk3::SystemPrinter::start_job (int pagecount, int *frompage, int *topage)
//printing using a Quartz graphics context
//returns 0 iff OK
{
  OSStatus status = 0;
  fl_open_display();
  Fl_X::q_release_context();
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
  if (fl_mac_os_version >= 100500) {
    NSPrintInfo *info = [NSPrintInfo sharedPrintInfo];
    NSPrintPanel *panel = [NSPrintPanel printPanel];
    //from 10.5
    [panel setOptions:NSPrintPanelShowsCopies | NSPrintPanelShowsPageRange | NSPrintPanelShowsPageSetupAccessory];
    NSInteger retval = [panel runModalWithPrintInfo:info];//from 10.5
    if(retval != NSOKButton) {
      fltk3::Window *w = fltk3::first_window();
      if (w) w->show();
      return 1;
    }
    printSession = (PMPrintSession)[info PMPrintSession];//from 10.5
    pageFormat = (PMPageFormat)[info PMPageFormat];//from 10.5
    printSettings = (PMPrintSettings)[info PMPrintSettings];//from 10.5
    UInt32 from32, to32;
    PMGetFirstPage(printSettings, &from32); 
    if (frompage) *frompage = (int)from32;
    PMGetLastPage(printSettings, &to32); 
    if (topage) *topage = (int)to32;
    if(topage && *topage > pagecount) *topage = pagecount;
    status = PMSessionBeginCGDocumentNoDialog(printSession, printSettings, pageFormat);//from 10.4
  }
  else {
#endif
    
#if !__LP64__
    Boolean accepted;
    status = PMCreateSession(&printSession);
    if (status != noErr) return 1;
    status = PMCreatePageFormat(&pageFormat);
    status = PMSessionDefaultPageFormat(printSession, pageFormat);
    if (status != noErr) return 1;
    // get pointer to the PMSessionPageSetupDialog Carbon function
    typedef OSStatus (*dialog_f)(PMPrintSession, PMPageFormat, Boolean *);
    static dialog_f f = NULL;
    if (!f) f = (dialog_f)Fl_X::get_carbon_function("PMSessionPageSetupDialog");
    status = (*f)(printSession, pageFormat, &accepted);
    if (status != noErr || !accepted) {
      fltk3::first_window()->show();
      return 1;
    }
    status = PMCreatePrintSettings(&printSettings);
    if (status != noErr || printSettings == kPMNoPrintSettings) return 1;
    status = PMSessionDefaultPrintSettings (printSession, printSettings);
    if (status != noErr) return 1;
    PMSetPageRange(printSettings, 1, (UInt32)kPMPrintAllPages);
    // get pointer to the PMSessionPrintDialog Carbon function
    typedef OSStatus (*dialog_f2)(PMPrintSession, PMPrintSettings, PMPageFormat, Boolean *);
    static dialog_f2 f2 = NULL;
    if (!f2) f2 = (dialog_f2)Fl_X::get_carbon_function("PMSessionPrintDialog");
    status = (*f2)(printSession, printSettings, pageFormat, &accepted);
    if (!accepted) status = kPMCancel;
    if (status != noErr) {
      fltk3::first_window()->show();
      return 1;
    }
    UInt32 from32, to32;
    PMGetFirstPage(printSettings, &from32); 
    if (frompage) *frompage = (int)from32;
    PMGetLastPage(printSettings, &to32); 
    if (topage) *topage = (int)to32;
    if(topage && *topage > pagecount) *topage = pagecount;
    CFStringRef mystring[1];
    mystring[0] = kPMGraphicsContextCoreGraphics;
    CFArrayRef array = CFArrayCreate(NULL, (const void **)mystring, 1, &kCFTypeArrayCallBacks);
    status = PMSessionSetDocumentFormatGeneration(printSession, kPMDocumentFormatDefault, array, NULL);
    CFRelease(array);
    status = PMSessionBeginDocumentNoDialog(printSession, printSettings, pageFormat);
#endif //__LP64__
    
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
  }
#endif
  if (status != noErr) return 1;
  y_offset = x_offset = 0;
  this->set_current();
  return 0;
}

void fltk3::SystemPrinter::margins(int *left, int *top, int *right, int *bottom)
{
  PMPaper paper;
  PMGetPageFormatPaper(pageFormat, &paper);
  PMOrientation orientation;
  PMGetOrientation(pageFormat, &orientation);
  PMPaperMargins margins;
  PMPaperGetMargins(paper, &margins);
  if(orientation == kPMPortrait) {
    if (left) *left = (int)(margins.left / scale_x + 0.5);
    if (top) *top = (int)(margins.top / scale_y + 0.5);
    if (right) *right = (int)(margins.right / scale_x + 0.5);
    if (bottom) *bottom = (int)(margins.bottom / scale_y + 0.5);
    }
  else {
    if (left) *left = (int)(margins.top / scale_x + 0.5);
    if (top) *top = (int)(margins.left / scale_y + 0.5);
    if (right) *right = (int)(margins.bottom / scale_x + 0.5);
    if (bottom) *bottom = (int)(margins.right / scale_y + 0.5);
  }
}

int fltk3::SystemPrinter::printable_rect(int *w, int *h)
//returns 0 iff OK
{
  OSStatus status;
  PMRect pmRect;
  int x, y;
  
  status = PMGetAdjustedPageRect(pageFormat, &pmRect);
  if (status != noErr) return 1;
  
  x = (int)pmRect.left;
  y = (int)pmRect.top;
  *w = int((int)(pmRect.right - x) / scale_x + 1);
  *h = int((int)(pmRect.bottom - y) / scale_y + 1);
  return 0;
}

void fltk3::SystemPrinter::origin(int x, int y)
{
  x_offset = x;
  y_offset = y;
  CGContextRestoreGState(fl_gc);
  CGContextRestoreGState(fl_gc);
  CGContextSaveGState(fl_gc);
  CGContextScaleCTM(fl_gc, scale_x, scale_y);
  CGContextTranslateCTM(fl_gc, x, y);
  CGContextRotateCTM(fl_gc, angle);
  CGContextSaveGState(fl_gc);
}

void fltk3::SystemPrinter::scale (float s_x, float s_y)
{
  if (s_y == 0.) s_y = s_x;
  scale_x = s_x;
  scale_y = s_y;
  CGContextRestoreGState(fl_gc);
  CGContextRestoreGState(fl_gc);
  CGContextSaveGState(fl_gc);
  CGContextScaleCTM(fl_gc, scale_x, scale_y);
  CGContextRotateCTM(fl_gc, angle);
  x_offset = y_offset = 0;
  CGContextSaveGState(fl_gc);
}

void fltk3::SystemPrinter::rotate (float rot_angle)
{
  angle = - rot_angle * M_PI / 180.;
  CGContextRestoreGState(fl_gc);
  CGContextRestoreGState(fl_gc);
  CGContextSaveGState(fl_gc);
  CGContextScaleCTM(fl_gc, scale_x, scale_y);
  CGContextTranslateCTM(fl_gc, x_offset, y_offset);
  CGContextRotateCTM(fl_gc, angle);
  CGContextSaveGState(fl_gc);
}

void fltk3::SystemPrinter::translate(int x, int y)
{
  CGContextSaveGState(fl_gc);
  CGContextTranslateCTM(fl_gc, x, y );
  CGContextSaveGState(fl_gc);
}

void fltk3::SystemPrinter::untranslate(void)
{
  CGContextRestoreGState(fl_gc);
  CGContextRestoreGState(fl_gc);
}

int fltk3::SystemPrinter::start_page (void)
{	
  OSStatus status = PMSessionBeginPageNoDialog(printSession, pageFormat, NULL);
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
  if ( PMSessionGetCGGraphicsContext != NULL ) {
    status = PMSessionGetCGGraphicsContext(printSession, &fl_gc);
  }
  else {
#endif
#if ! __LP64__
    status = PMSessionGetGraphicsContext(printSession,NULL,(void **)&fl_gc);
#endif
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
  }
#endif
  PMRect pmRect;
  float win_scale_x, win_scale_y;

  PMPaper paper;
  PMGetPageFormatPaper(pageFormat, &paper);
  PMPaperMargins margins;
  PMPaperGetMargins(paper, &margins);
  PMOrientation orientation;
  PMGetOrientation(pageFormat, &orientation);
  
  status = PMGetAdjustedPageRect(pageFormat, &pmRect);
  double h = pmRect.bottom - pmRect.top;
  x_offset = 0;
  y_offset = 0; 
  angle = 0;
  scale_x = scale_y = 1;
  win_scale_x = win_scale_y = 1;
  if(orientation == kPMPortrait)
    CGContextTranslateCTM(fl_gc, margins.left, margins.bottom + h);
  else
    CGContextTranslateCTM(fl_gc, margins.top, margins.right + h);
  CGContextScaleCTM(fl_gc, win_scale_x, - win_scale_y);
  fl_quartz_restore_line_style_();
  CGContextSetShouldAntialias(fl_gc, false);
  CGContextSaveGState(fl_gc);
  CGContextSaveGState(fl_gc);
  fltk3::line_style(fltk3::SOLID);
  fl_window = (void *)1; // TODO: something better
  fltk3::clip_region(0);
  if( status == noErr) gc = fl_gc;
  return status != noErr;
}

int fltk3::SystemPrinter::end_page (void)
{	
  CGContextFlush(fl_gc);
  CGContextRestoreGState(fl_gc);
  CGContextRestoreGState(fl_gc);
  OSStatus status = PMSessionEndPageNoDialog(printSession);
  gc = NULL;
  return status != noErr;
}

void fltk3::SystemPrinter::end_job (void)
{
  OSStatus status;
  
  status = PMSessionError(printSession);
  if (status != noErr) {
    fltk3::alert ("PM Session error %d", (int)status);
  }
  PMSessionEndDocumentNoDialog(printSession);
  fltk3::DisplayDevice::display_device()->set_current();
  fl_gc = 0;
  fltk3::Window *w = fltk3::first_window();
  if (w) w->show();
}

#endif // __APPLE__

//
// End of "$Id$".
//
