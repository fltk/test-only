//
// "$Id$"
//
// Style header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

/** \file
 fltk3::Style class . */

#ifndef FLTK3_STYLE_H
#define FLTK3_STYLE_H


#include "enumerations.h"
#include "Export.h"


namespace fltk3 {
  
  class FLTK3_EXPORT Style {
    
  public:
    
    static unsigned int current_;
    
    Style *parent_;
    
    unsigned int version_;
    Box* box_;
    Align align_;
    Font labelfont_;
    Fontsize labelsize_;
    Color labelcolor_;
    Font textfont_;
    Fontsize textsize_;
    Color textcolor_;      
    Labeltype labeltype_; // FIXME: does this belong here?
    Color color_;
    Color color2_;
    
    unsigned int private_:1;
    unsigned int box_set_:1;
    unsigned int align_set_:1;
    unsigned int labelfont_set_:1;
    unsigned int labelsize_set_:1;
    unsigned int labelcolor_set_:1;
    unsigned int textfont_set_:1;
    unsigned int textsize_set_:1;
    unsigned int textcolor_set_:1;
    unsigned int labeltype_set_:1;
    unsigned int color_set_:1;
    unsigned int color2_set_:1;
    
    unsigned int update();
    void refresh() const { if (version_ != current_) ((Style*)this)->update(); }
    void invalidate() { if (!private_) current_++; }
    
  public:
    
    Style();
    Style(Style *parent);
    ~Style();
    
    Font labelfont() const { refresh(); return labelfont_;}
    void labelfont(fltk3::Font f) { labelfont_=f; labelfont_set_ = 1; invalidate(); }
    void clear_labelfont() { labelfont_set_=0; invalidate(); }

    Fontsize labelsize() const { refresh(); return labelsize_;}
    void labelsize(fltk3::Fontsize s) { labelsize_=s; labelsize_set_ = 1; invalidate(); }
    void clear_labelsize() { labelsize_set_=0; invalidate(); }
    
    Labeltype labeltype() const { refresh(); return labeltype_;}
    void labeltype(fltk3::Labeltype s) { labeltype_=s; labeltype_set_ = 1; invalidate(); }
    void clear_labeltype() { labeltype_set_=0; invalidate(); }
    
    Color labelcolor() const { refresh(); return labelcolor_;}
    void labelcolor(Color s) { labelcolor_=s; labelcolor_set_ = 1; invalidate(); }
    void clear_labelcolor() { labelcolor_set_=0; invalidate(); }
    
    Align align() const { refresh(); return align_;}
    void align(Align s) { align_=s; align_set_ = 1; invalidate(); }
    void clear_align() { align_set_=0; invalidate(); }

    Font textfont() const { refresh(); return textfont_;}
    void textfont(fltk3::Font f) { textfont_=f; textfont_set_ = 1; invalidate(); }
    void clear_textfont() { textfont_set_=0; invalidate(); }
    
    Fontsize textsize() const { refresh(); return textsize_;}
    void textsize(fltk3::Fontsize s) { textsize_=s; textsize_set_ = 1; invalidate(); }
    void clear_textsize() { textsize_set_=0; invalidate(); }
    
    Color textcolor() const { refresh(); return textcolor_;}
    void textcolor(Color s) { textcolor_=s; textcolor_set_ = 1; invalidate(); }
    void clear_textcolor() { textcolor_set_=0; invalidate(); }
    
    Color color() const { refresh(); return color_;}
    void color(Color s) { color_=s; color_set_ = 1; invalidate(); }
    void clear_color() { color_set_=0; invalidate(); }
    
    Color color2() const { refresh(); return color2_;}
    void color2(Color s) { color2_=s; color2_set_ = 1; invalidate(); }
    void clear_color2() { color2_set_=0; invalidate(); }
    
    Box* box() const { refresh(); return box_;}
    void box(Box* b) { box_=b; box_set_ = 1; invalidate(); }
    void clear_box() { box_set_=0; invalidate(); }
    
    
    /**
     Use an existing style for a new label.
     If the style is public, the same style is returned. However, if the style 
     is private, it will be duplicated, referencing the first available publi 
     style.
     \return a style that is guaranteed to be public.
     */
    Style *as_public();
    
    Style *make_private();
    bool is_private() { return private_; }
  };

  extern Style default_style;
}

#endif

//
// End of "$Id$".
//
