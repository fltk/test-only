/
// "$Id$
/
// Fl_Printer header file for the Fast Light Tool Kit (FLTK)
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
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/


#ifndef Fl_Printer_
#define Fl_Printer_
#include <FL/Fl_Device.H
#include <FL/fl_draw.H
#include <FL/Enumerations.H

const double FL_POINT = 1
const double FL_INCH = 72
const double FL_MM = 2.835
const double FL_CM = 28.35
const double FL_PICA = 12



const int NO_PAGE_FORMATS=30

/** This is a base class for all printing devices. It also defines some usefull methods for paging and placement of drawings on the page. */

class FL_EXPORT Fl_Printer:public Fl_Device
  static const char * doc_info_

public

  enum Page_Format
    A0 = 0,
    A1
    A2
    A3
    A4
    A5
    A6
    A7
    A8
    A9
    B0
    B1
    B2
    B3
    B4
    B5
    B6
    B7
    B8
    B9
    B10
    C5E
    DLE
    EXECUTIVE
    FOLIO
    LEDGER
    LEGAL
    LETTER
    TABLOID
    ENVELOPE
    MEDIA = 0x100
  }
  enum Page_Layout {PORTRAIT = 0, LANDSCAPE = 0x100, REVERSED = 0x200, ORIENTATION = 0x300}


protected
  double pw_, ph_
  static const int page_formats[NO_PAGE_FORMATS][2]
  uchar bg_r, bg_g, bg_b
public
 
  /** This method sets the information about the document when an instance of s Fl_Printer class is created.
  
  * The value is used for all Fl_Printer classes and should be set/changed before calling of the constructo
  * of a Fl_Printer subclass. If not the default value is "FLTK Application" is used. For postscript printer
  * %%Creator: DSC comment is generated. *
  static void doc_info(const char * info){ doc_info_ = info;}
 
  /** The method returns pointer to current docinfo string. *
  static const char * doc_info(){return doc_info_;}
  virtual void page(double pw, double ph, int media = 0)=0
  virtual void page(int format)=0
  virtual void orientation (int o){}
  double page_width(){return pw_;}
  double page_height(){return ph_;}
  void alpha_background(uchar r, uchar g, uchar b){bg_r = r, bg_g = g; bg_b = b;
  void alpha_background(uchar *r, uchar *g, uchar *b){ *r = bg_r, *g = bg_g; *b = bg_b;
  void alpha_background(Fl_Color c)
  virtual void place(double x, double y,  double tx, double ty,  double scale = 1)=0
  void place(double x, double y, double w, double h, double tx, double ty, double tw, double th, int align = FL_ALIGN_CENTER)
  void place(Fl_Widget * w, double tx, double ty, double tw, double th, int align = FL_ALIGN_CENTER )
    place(w->x(), w->y(), w->w(), w->h(), tx, ty, tw, th, align)
  }
  Fl_Printer()
}





#endi

/
// "$Id$
/
