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

#include <fltk3/Widget.h>
#include <fltk3/Wrapper.h>

/*
 
 Wrapping a virtual function is somewhat complicated. There are few virtual
 functions in FLTK, but wrapping those needs to be perfect. 

 Here are the use cases we need to cover. The draw() method is usually called
 from the core and can (or must) be overriden by the user. show() is usually
 called by the user and must call the correct show() in FLTK 3.
 
 To add complexity, an overridden method may call the originl method or a 
 method of the superclass at any time.
 
 
 A: draw() called by the core, not overridden:
 
 user over FLTK1 FLTK3 core flag
 
                       core  0     core calls FLTK3 draw()
                      /              virtual call
                 FLTK3       0->1  checks if a wrapper exists and calls it
                /                    direct call
           FLTK1             1     method not overridden, call FLTK3
                \                    direct call
                 FLTK3       1     call from FLTK1, run FLTK3 draw() and return
                /                    return
           FLTK1             1->0  return to FLTK3
                \                    return
                 FLTK3       0     we are done here, return to core
                      \              return
                       core  0
 
 
 B: draw() called by the core, wrapper overridden by the user:
 
 user over FLTK1 FLTK3 core flag
 
                       core  0     core calls FLTK3 draw()
                      /              virtual call
                 FLTK3       0->1  checks if a wrapper exists and calls it
          ,-----'                    direct call
      over                   1     method overridden, eventually returns to FLTK3
          `-----.                    return
                 FLTK3       1->0  method overriden, don't run FLTK3 draw(), return
                      \              return
                       core  0

 
 C: draw() called by the core, wrapper overridden by the user, overriden method calls original draw
 
 user over FLTK1 FLTK3 core flag
 
                       core  0     core calls FLTK3 draw()
                      /              virtual call
                 FLTK3       0->1  checks if a wrapper exists and calls it
          ,-----'                    direct call
      over                   1     method overridden, calls original draw()
          \                          virtual or direct call
           FLTK1             1     call FLTK3
                \                    direct call
                 FLTK3       1     call from FLTK1, run FLTK3 draw() and return
                /                    return
           FLTK1             1     return to wherever we came from
          /                          return
      over                   1     eventually returns to FLTK3
          `-----.                    return
                 FLTK3       1->0  method overriden, don't run FLTK3 draw(), return
                      \              return
                       core  0
 

 D: show() called by the user, nothing overridden

 user over FLTK1 FLTK3 core flag
 
 user                        0     user calls virtual method
     `----.                          virtual or direct call
           FLTK1             0->1  call FLTK3
                \                    direct call
                 FLTK3       1     call from FLTK1, run FLTK3 draw() and return
                /                    return
           FLTK1             1->0  return to wherever we came from
     ,----'                          return
 user

 
 E: show() called by the user, overridden by the user, calling original show()
 
 user over FLTK1 FLTK3 core flag
 
 user                        0     user calls virtual method
     \                               virtual call
      over                   0     call original function
          \                          direct call
           FLTK1             0->1  call FLTK3
                \                    direct call
                 FLTK3       1     call from FLTK1, run FLTK3 draw() and return
                /                    return
           FLTK1             1->0  return to wherever we came from
          /                          return
      over                   0     eventually return to use
     /                               return
 user
 
 
 */

#define FLTK3_WRAPPER_VCALLS_OBJECT(klass, proto, call, flag) \
virtual void proto { \
  if ( pVCalls & pVCallWidget##flag ) { \
    ((fltk3::klass*)_p)->call; \
  } else { \
    pVCalls |= pVCallWidget##flag; \
    ((fltk3::klass*)_p)->call; \
    pVCalls &= ~pVCallWidget##flag; \
  } \
}

#define FLTK3_WRAPPER_VCALLS_OBJECT_INT(klass, proto, call, flag) \
virtual int proto { \
  int ret = 0; \
  if ( pVCalls & pVCallWidget##flag ) { \
    ret = ((fltk3::klass*)_p)->call; \
  } else { \
    pVCalls |= pVCallWidget##flag; \
    ret = ((fltk3::klass*)_p)->call; \
    pVCalls &= ~pVCallWidget##flag; \
  } \
  return ret; \
}

#define FLTK3_OBJECT_VCALLS_WRAPPER(call, flag) \
  if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallWidget##flag) ) { \
    pWrapper->pVCalls |= Wrapper::pVCallWidget##flag; \
    ((WidgetWrapper*)pWrapper)->call; \
    pWrapper->pVCalls &= ~Wrapper::pVCallWidget##flag; \
    return; \
  }

#define FLTK3_OBJECT_VCALLS_WRAPPER_INT(call, flag) \
  if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallWidget##flag) ) { \
    pWrapper->pVCalls |= Wrapper::pVCallWidget##flag; \
    int ret = ((WidgetWrapper*)pWrapper)->call; \
    pWrapper->pVCalls &= ~Wrapper::pVCallWidget##flag; \
    return ret; \
  }


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
    static const unsigned int pVCallWidgetDrawOverlay = 1<<6;
  };
  
  
  class WidgetWrapper : public Wrapper {
  public:
    virtual ~WidgetWrapper() {}
    FLTK3_WRAPPER_VCALLS_OBJECT(Widget, 
                                draw(),
                                draw(),
                                Draw)
    FLTK3_WRAPPER_VCALLS_OBJECT_INT(Widget,
                                    handle(int event),
                                    handle(event),
                                    Handle)
    FLTK3_WRAPPER_VCALLS_OBJECT(Widget,
                                resize(int x, int y, int w, int h),
                                resize(x, y, w, h),
                                Resize)
    FLTK3_WRAPPER_VCALLS_OBJECT(Widget,
                                show(),
                                show(),
                                Show)
    FLTK3_WRAPPER_VCALLS_OBJECT(Widget,
                                hide(),
                                hide(),
                                Hide)
    virtual void draw_overlay() {} // neede by overlay windows
//    FLTK3_WRAPPER_VCALLS_OBJECT(Window,
//                                draw_overlay(),
//                                draw_overlay(),
//                                DrawOverlay)
  };
  
  
}; // namespace fltk3


#endif

//
// End of "$Id: Wrapper.h 7365 2010-03-30 15:18:29Z matt $".
//
