/
// "$Id$
/
// WIN32 GDI printing device for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
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
// Please report all bugs and problems to http://www.fltk.org/str.ph
/

//#include <stdlib.h
//#include <math.h
#include <FL/Fl.H
//#include <windows.h
//#include <wingdi.h
//#include <FL/math.h

#include <FL/Fl_GDI_Printer.H

#include <FL/x.H
#include "../win/Fl_Win_Display.H
#include <FL/Enumerations.H
//#include <FL/fl_draw.H

//#include <FL/Fl_Group.H



//extern FL_EXPORT void set_xmaps(HPEN tmppen_, HBRUSH tmpbrush_, HPEN savepen_, Fl_Brush *brushes_, Fl_XMap * fl_xmap_, Fl_XMap xmap_)
//extern FL_EXPORT void pop_xmaps()
extern FL_EXPORT void push_xmaps()



Fl_GDI_Printer::~Fl_GDI_Printer()

 
  if(nPages
      EndPage(gc_)
  EndDoc(gc_)

  // cleaning pens&brushes..
  /
  if(xmap.pen
    DeleteObject((HGDIOBJ)(xmap.pen))
  int i
  for(i=0; i<256; i++
    if(fl_xmap[i].pen
      DeleteObject((HGDIOBJ)(fl_xmap[i].pen))

  for(i=0; i<FL_N_BRUSH; i++
  if(brushes[i].brush
    DeleteObject(brushes[i].brush)
  *
  //end clea
 
  DeleteDC(gc_)
  if(!GlobalUnlock(mode_)
    GlobalFree(mode_)
}


extern Fl_Win_Display fl_disp

Fl_Device * Fl_GDI_Printer::set_current()
  Fl_Device * c;
  if((c = current()) == this) return this
  if(c == &fl_disp){  //must do some stuf
    Fl::flush()
    push_xmaps()
  
  //set_xmaps(tmppen, tmpbrush, savepen, brushes, fl_xmap, xmap)
  fl_device = this
  fl_gc = gc_
  fl_clip_region(0)
  return c






////////////// gets the paper size from DEVMODE ////////////////////

static int gdipaper(DEVMODE  *mod)
  if(!(mod->dmFields & DM_PAPERSIZE)) return 0

  int paper
  switch (mod->dmPaperSize)
    case DMPAPER_A3
      paper = Fl_Printer::A3; break
    case DMPAPER_A4
      paper = Fl_Printer::A4; break
    case DMPAPER_A5
      paper = Fl_Printer::A5; break
    //case DMPAPER_A6: paper = FL_A6; break
    case DMPAPER_B4
      paper = Fl_Printer::B4; break
    case DMPAPER_B5: paper = Fl_Printer::B5; break
    //case DMPAPER_B6: paper = FL_B6; break
    case DMPAPER_EXECUTIVE
      paper = Fl_Printer::EXECUTIVE; break
    case DMPAPER_FOLIO
      paper = Fl_Printer::FOLIO; break
    case DMPAPER_LEDGER
      paper = Fl_Printer::LEDGER; break
    case DMPAPER_LEGAL
      paper = Fl_Printer::LEGAL; break
    case DMPAPER_LETTER
      paper = Fl_Printer::LETTER; break
    case DMPAPER_TABLOID
      paper = Fl_Printer::TABLOID; break
    case DMPAPER_ENV_10
      paper = Fl_Printer::ENVELOPE; break
    default
      paper = -1
  
  return paper


////////////// sets the paper size in DEVMODE ////////////////////

static int gdipaper(DEVMODE  *mode, int format)
  switch(format)
    case Fl_Printer::A3: mode->dmPaperSize = DMPAPER_A3; break
    case Fl_Printer::A4: mode->dmPaperSize = DMPAPER_A4; break
    case Fl_Printer::A5: mode->dmPaperSize = DMPAPER_A5; break

    case Fl_Printer::B4: mode->dmPaperSize = DMPAPER_B4; break
    case Fl_Printer::B5: mode->dmPaperSize = DMPAPER_B5; break

    case Fl_Printer::EXECUTIVE: mode->dmPaperSize = DMPAPER_EXECUTIVE; break
    case Fl_Printer::FOLIO: mode->dmPaperSize = DMPAPER_FOLIO; break
    case Fl_Printer::LEDGER: mode->dmPaperSize = DMPAPER_LEDGER; break
    case Fl_Printer::LEGAL: mode->dmPaperSize = DMPAPER_LEGAL; break
    case Fl_Printer::LETTER: mode->dmPaperSize = DMPAPER_LETTER; break
    case Fl_Printer::TABLOID: mode->dmPaperSize = DMPAPER_TABLOID; break
    case Fl_Printer::ENVELOPE: mode->dmPaperSize = DMPAPER_ENV_9; break
    default: return -1
  
  return 0



/////////////////////////////////////////////////////////////////////////////////////////////

Fl_GDI_Printer::Fl_GDI_Printer(HDC gc, DEVMODE * mode ):Fl_Printer(),/*delete_mode_(0),*/ style_(0),width_(0),dashes_(0),mask(0)
  type_ = FL_GDI_PRINTER
 
  gc_=gc

  static DOCINFO DocInfo = { sizeof(DOCINFO), doc_info(), NULL,0 };// declare DocInfo for use and set the name of the print job as 'Name Of Document
  mode_  = (DEVMODE *)GlobalLock(mode)
  int orientation_ = 0
  if(mode_->dmOrientation==DMORIENT_LANDSCAPE
    orientation_ = 1
  int paper
  if(mode_->dmFields & DM_PAPERSIZE)
    paper = gdipaper(mode_)
    if(mode_->dmOrientation == DMORIENT_PORTRAIT)
      pw_ = Fl_Printer::page_formats[paper][0]
      ph_ = Fl_Printer::page_formats[paper][1]
    }else
      pw_ = Fl_Printer::page_formats[paper][1]
      ph_ = Fl_Printer::page_formats[paper][0]
    
  

  ResetDC(gc_,mode_)
  SetMapMode(gc_, MM_ANISOTROPIC)
  SetTextAlign(gc_, TA_BASELINE|TA_LEFT)
  SetBkMode(gc_, TRANSPARENT)
  StartDoc(gc_, &DocInfo)
  ix = GetDeviceCaps(gc_, LOGPIXELSX)
  iy = GetDeviceCaps(gc_, LOGPIXELSY)
  ox = GetDeviceCaps(gc_, PHYSICALOFFSETX)
  oy = GetDeviceCaps(gc_, PHYSICALOFFSETY)
  if(ix<iy
	  max_res_=ix
  els
	  max_res_=iy

  nPages = 0

  /

  int i
  // Unlike for static vars, here I have to zero the memor
  for(i=0;i<FL_N_BRUSH;i++)
    brushes[i].brush = 0
    brushes[i].usage = 0
    brushes[i].backref = 0
  
  tmppen = 0
  tmpbrush = 0
  savepen = 0
  for(i=0;i<256;i++)
    fl_xmap[i].rgb = 0
    fl_xmap[i].pen = 0;	// pen, 0 if none created ye
    fl_xmap[i].brush = 0
  
  xmap.rgb=0
  xmap.brush =0
  xmap.pen = 0
  *
  type_ = 0x200



/////////////////////////////////   paging   /////////////////////////////////////


void Fl_GDI_Printer::set_page(int page)
  if(page)
    ResetDC(gc_,mode_)
    SetMapMode(gc_, MM_ANISOTROPIC)
    SetTextAlign(gc_, TA_BASELINE|TA_LEFT)
    SetBkMode(gc_, TRANSPARENT)
    StartPage(gc_)
    ix = GetDeviceCaps(gc_, LOGPIXELSX)
    iy = GetDeviceCaps(gc_, LOGPIXELSY)
    ox = GetDeviceCaps(gc_, PHYSICALOFFSETX)
    oy = GetDeviceCaps(gc_, PHYSICALOFFSETY)
  
  SetViewportOrgEx(gc_,VOx =  - ox, VOy = - oy,0); //setting origin to the upper left corne
  SetViewportExtEx(gc_, VEx = (long)(ix * iy),  VEy = (long)(iy *ix ),0)
  SetWindowExtEx(gc_, WEx = iy * 72,  WEy = ix * 72, 0); //72 pixels per inch mappin
  SetWindowOrgEx(gc_, WOx = 0, WOy = 0,0)




void Fl_GDI_Printer::page(double pw, double ph, int media)
  if(nPages
    EndPage(gc_)
  nPages++
  pw_=pw
  ph_=ph
  if(ph>pw
    mode_->dmOrientation = DMORIENT_PORTRAIT
  els
    mode_->dmOrientation = DMORIENT_LANDSCAPE
  if(media)
    mode_->dmPaperWidth = (int) (pw*254/72)
    mode_->dmPaperLength = (int) (ph*254/72)
    mode_->dmFields |= DM_PAPERLENGTH | DM_PAPERWIDTH
    mode_->dmFields &= ~DM_PAPERSIZE
  
  set_page(1)
}


void Fl_GDI_Printer::page(int format)
  if(nPages
    EndPage(gc_)
  if(format & LANDSCAPE)
    mode_->dmOrientation = DMORIENT_LANDSCAPE
    pw_ = page_formats[format & 0xFF][1]
    ph_ = page_formats[format & 0xFF][0]
  }else
    mode_->dmOrientation = DMORIENT_PORTRAIT
    pw_ = page_formats[format & 0xFF][0]
    ph_ = page_formats[format & 0xFF][1]
  
  if(format & MEDIA)
    if(!gdipaper(mode_, format& 0xFF))
        mode_->dmFields &= ~DM_PAPERLENGTH & ~DM_PAPERWIDTH
        mode_->dmFields |= DM_PAPERSIZE
    }else
       mode_->dmPaperWidth = (int) (pw_*254/72)
       mode_->dmPaperLength = (int) (ph_*254/72)
       mode_->dmFields |= DM_PAPERLENGTH | DM_PAPERWIDTH
       mode_->dmFields &= ~DM_PAPERSIZE
    
  
  if(format & LANDSCAPE)
    mode_->dmOrientation = DMORIENT_LANDSCAPE
  }else
    mode_->dmOrientation = DMORIENT_PORTRAIT

  
  set_page(1)
}


void Fl_GDI_Printer::place(double x, double y, double tx, double ty, double s)
//  SetViewportOrgEx(gc_, VOx = (long)(ix * (lm_+tx) /72 - ox + dx), VOy = (long)( iy *(tm_+th) /72 - oy + dy),0); //setting origin to the upper left corner inside margin

  SetViewportOrgEx(gc_, VOx = (long)(ix * tx /72 - ox ), VOy = (long)( iy *ty /72 - oy),0); //setting origin to the upper left corner inside margin
  SetWindowOrgEx(gc_, WOx = (long)x,  WOy = (long)y,0)
  SetViewportExtEx(gc_,  VEx =(long)(ix * iy *s),  VEy =(long)(iy * ix * s), 0)
  SetWindowExtEx(gc_, WEx = iy *72, WEy = ix * 72,0)
}



void Fl_GDI_Printer::set_subpixel()
  SetWindowExtEx(gc_, WEx*(max_res_), WEy*(max_res_), 0)
  SetWindowOrgEx(gc_, WOx*(max_res_),  WOy*(max_res_) ,0);
  sty(style_, width_, dashes_, max_res_)
}

void Fl_GDI_Printer::set_normal()
  SetWindowExtEx(gc_, WEx, WEy, 0)
  SetWindowOrgEx(gc_, WOx, WOy,0)
  sty(style_, width_, dashes_, 1)
}

/
// End of "$Id$
/



