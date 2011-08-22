//
// "$Id$"
//
// Header file for Fl_Text_Buffer class.
// FLTK 123 alias finished
//
// Copyright 2001-2011 by Bill Spitzak and others.
// Original code Copyright Mark Edel.  Permission to distribute under
// the LGPL for the FLTK library granted by Mark Edel.
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

#ifndef FL_TEXT_BUFFER_H
#define FL_TEXT_BUFFER_H

#include <fltk3/TextBuffer.h>
#include "Fl_Export.H"


class FL_EXPORT Fl_Text_Selection : public fltk3::TextSelection {
  
public:
  
  Fl_Text_Selection() : fltk3::TextSelection() { }

};


typedef fltk3::TextModifyCb Fl_Text_Modify_Cb;

typedef fltk3::TextPredeleteCb Fl_Text_Predelete_Cb;


class FL_EXPORT Fl_Text_Buffer : public fltk3::TextBuffer {
  
public:

  Fl_Text_Buffer(int requestedSize = 0, int preferredGapSize = 1024)
  : fltk3::TextBuffer(requestedSize, preferredGapSize) { }

  void add_modify_callback(Fl_Text_Modify_Cb cb, void* v) {
    fltk3::TextBuffer::add_modify_callback( (fltk3::TextModifyCb)cb, v);
  }
  
  void remove_modify_callback(Fl_Text_Modify_Cb cb, void* v) {
    fltk3::TextBuffer::remove_modify_callback( (fltk3::TextModifyCb)cb, v);
  }
  
  void add_predelete_callback(Fl_Text_Predelete_Cb cb, void* v) {
    fltk3::TextBuffer::add_predelete_callback( (fltk3::TextPredeleteCb)cb, v);
  }    

  void remove_predelete_callback(Fl_Text_Predelete_Cb cb, void* v) {
    fltk3::TextBuffer::remove_predelete_callback( (fltk3::TextPredeleteCb)cb, v);
  }

  const Fl_Text_Selection* primary_selection() const { 
    return (Fl_Text_Selection*)fltk3::TextBuffer::primary_selection(); 
  }
  
  Fl_Text_Selection* primary_selection() {
    return (Fl_Text_Selection*)fltk3::TextBuffer::primary_selection(); 
  }
  
  const Fl_Text_Selection* secondary_selection() const { 
    return (Fl_Text_Selection*)fltk3::TextBuffer::secondary_selection(); 
  }
  
  const Fl_Text_Selection* highlight_selection() const { 
    return (Fl_Text_Selection*)fltk3::TextBuffer::highlight_selection(); 
  }
    
};

#endif

//
// End of "$Id$".
//
