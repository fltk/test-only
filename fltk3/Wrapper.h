//
// "$Id: Wrapper.h 7365 2010-03-30 15:18:29Z matt $"
//
// Widget header file for the Fast Light Tool Kit (FLTK).
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

/* \file
   fltk3::Wrapper */

#ifndef FLTK3_Wrapper_H
#define FLTK3_Wrapper_H

/*
 
#define FLTK3_WRAPPER_VCALLS_OBJECT(proto, call, flag) \
  virtual void proto { \
    if ( pVCalls & pVCallWidget##flag ) { \
    } else { \
      pVCalls |= pVCallWidget##flag; \
      ((fltk3::Widget*)_p)->call; \
      pVCalls &= ~pVCallWidget##flag; \
    } \
  }

#define FLTK3_OBJECT_VCALLS_WRAPPER(call, flag) \
  if (pWrapper) { \
    if ( pWrapper->pVCalls & Wrapper::pVCallWidget##flag ) { \
    } else { \
      pWrapper->pVCalls |= Wrapper::pVCallWidget##flag; \
      ((WidgetWrapper*)pWrapper)->call; \
      if ( !(pWrapper->pVCalls & Wrapper::pVCallWidget##flag) ) \
        return; \
    } \
    pWrapper->pVCalls &= ~Wrapper::pVCallWidget##flag; \
  }

#define FLTK3_WRAPPER_VCALLS_OBJECT_INT(proto, call, flag) \
  virtual int proto { \
    int ret = 0; \
    if ( pVCalls & pVCallWidget##flag ) { \
    } else { \
      pVCalls |= pVCallWidget##flag; \
      ret = ((fltk3::Widget*)_p)->call; \
      pVCalls &= ~pVCallWidget##flag; \
    } \
    return ret; \
  }

#define FLTK3_OBJECT_VCALLS_WRAPPER_INT(call, flag) \
  if (pWrapper) { \
    if ( pWrapper->pVCalls & Wrapper::pVCallWidget##flag ) { \
    } else { \
      pWrapper->pVCalls |= Wrapper::pVCallWidget##flag; \
      int ret = ((WidgetWrapper*)pWrapper)->call; \
      if ( !(pWrapper->pVCalls & Wrapper::pVCallWidget##flag) ) \
        return ret; \
    } \
    pWrapper->pVCalls &= ~Wrapper::pVCallWidget##flag; \
  }

 */

namespace fltk3 {
  
  class Object;
  
  /**
   * \brief a base class for any kind of langage wrapper around FLTK3.
   */
  class Wrapper {
  protected:
  public:
    Object *_p;
    Wrapper() 
    : _p(0L), pVCalls(0) { }
    virtual ~Wrapper();
    
    unsigned int pVCalls;
    static const unsigned int pVCallDtor          = 1<<0;
    static const unsigned int pVCallWidgetDraw    = 1<<1;
    static const unsigned int pVCallWidgetHandle  = 1<<2;
    static const unsigned int pVCallWidgetResize  = 1<<3;
    static const unsigned int pVCallWidgetShow    = 1<<4;
    static const unsigned int pVCallWidgetHide    = 1<<5;
    
    virtual void draw() { /* call _p->draw() with a flag set */ }
  };
  
  
  class WidgetWrapper : public Wrapper {
  public:
    virtual ~WidgetWrapper() {}
    /*
    FLTK3_WRAPPER_VCALLS_OBJECT(draw(),
                                draw(),
                                Draw)
    FLTK3_WRAPPER_VCALLS_OBJECT_INT(handle(int event),
                                    handle(event),
                                    Handle)
    FLTK3_WRAPPER_VCALLS_OBJECT(resize(int x, int y, int w, int h),
                                resize(x, y, w, h),
                                Resize)
    FLTK3_WRAPPER_VCALLS_OBJECT(show(),
                                show(),
                                Show)
    FLTK3_WRAPPER_VCALLS_OBJECT(hide(),
                                hide(),
                                Hide)
     */
  };
  
  
}; // namespace fltk3


#endif

//
// End of "$Id: Wrapper.h 7365 2010-03-30 15:18:29Z matt $".
//
