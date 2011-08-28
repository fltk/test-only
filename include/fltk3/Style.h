//
// "$Id: Style.h 8623 2011-04-24 17:09:41Z AlbrechtS $"
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


namespace fltk3 {
  
  class Style {
    
  protected:
    
    static unsigned int current_;
    
    unsigned int version_;
    Style *parent_;
    
    Font labelfont_;
    Fontsize labelsize_;

    unsigned int private_:1;
    unsigned int labelfont_set_:1;
    unsigned int labelsize_set_:1;
    
    void update();
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

    Style *make_private();
  };

  extern Style default_style;
};

#endif

//
// End of "$Id: Style.h 8623 2011-04-24 17:09:41Z AlbrechtS $".
//
