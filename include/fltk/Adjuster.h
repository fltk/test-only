//
// "$Id$"
//
// Copyright 1998-2006 by Bill Spitzak and others.
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
//    http://www.fltk.org/str.php
//

// Undocumented valuator provided for back-compatability.
// This may be removed before the final version.
// 3-button "slider", made for Nuke

#ifndef fltk_Adjuster_h
#define fltk_Adjuster_h


#include <fltk3/Adjuster.h>
#include "Valuator.h"


FLTK2_WRAPPER_INTERFACE_BEGIN(Adjuster, Adjuster)
FLTK2_WRAPPER_INTERFACE_WIDGET(Adjuster, Adjuster)
FLTK2_WRAPPER_INTERFACE_END()


namespace fltk {
  
  class Adjuster : public Valuator {
    
  public:

    FLTK2_WIDGET_VCALLS(Adjuster, Adjuster)
    
    Adjuster() { /* empty */ }
    
    Adjuster(int x, int y, int w, int h, const char *label=0) {
      _p = new fltk3::Adjuster_I(x, y, w, h, label);
      _p->wrapper(this);
    }
    
    void soft(int s) {
      ((fltk3::Adjuster_I*)_p)->soft(s);
    }
    
    int soft() const {
      return ((fltk3::Adjuster_I*)_p)->soft();
    }
    
#if 0
    static NamedStyle* default_style;
    
  protected:
    void value_damage();
    
#endif
    
  };
  
}
#endif

//
// End of "$Id$".
//
