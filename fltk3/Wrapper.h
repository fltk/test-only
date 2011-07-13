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

#define FLTK3_WRAPPER_VCALLS_OBJECT_RET(rtype, klass, proto, call, flag) \
virtual rtype proto { \
  rtype ret = 0; \
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

#define FLTK3_OBJECT_VCALLS_WRAPPER_RET(rtype, call, flag) \
  if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallWidget##flag) ) { \
    pWrapper->pVCalls |= Wrapper::pVCallWidget##flag; \
    rtype ret = ((WidgetWrapper*)pWrapper)->call; \
    pWrapper->pVCalls &= ~Wrapper::pVCallWidget##flag; \
    return ret; \
  }


#define FLTK3_WRAPPER_INTERFACE_BEGIN(type1, type3) \
  class type1; \
  namespace fltk3 { \
    class type3##_I : public type3 { \
      friend class ::type1; \
    public: \
      type3##_I(int x, int y, int w, int h, const char *l) \
      : type3(x, y, w, h, l) { }

#define FLTK3_WRAPPER_INTERFACE_WIDGET(type1, type3) \
      void show() { \
        FLTK3_OBJECT_VCALLS_WRAPPER(show(), Show) \
        type3::show(); \
      } \
      void hide() { \
        FLTK3_OBJECT_VCALLS_WRAPPER(hide(), Hide) \
        type3::hide(); \
      } \
      void resize(int X, int Y, int W, int H) { \
        FLTK3_OBJECT_VCALLS_WRAPPER(resize(X, Y, W, H), Resize) \
        type3::resize(X, Y, W, H); \
      } \
      void draw() { \
        FLTK3_OBJECT_VCALLS_WRAPPER(draw(), Draw) \
        type3::draw(); \
      } \
      int handle(int event) { \
        FLTK3_OBJECT_VCALLS_WRAPPER_RET(int, handle(event), Handle) \
        return type3::handle(event); \
      }

#define FLTK3_WRAPPER_INTERFACE_END() \
    }; \
  }


#define FLTK3_WIDGET_VCALLS(type3) \
  FLTK3_WRAPPER_VCALLS_OBJECT(type3##_I, show(), hide(), Show) \
  FLTK3_WRAPPER_VCALLS_OBJECT(type3##_I, hide(), show(), Hide) \
  FLTK3_WRAPPER_VCALLS_OBJECT(type3##_I, draw(), draw(), Draw) \
  FLTK3_WRAPPER_VCALLS_OBJECT(type3##_I, resize(int x, int y, int w, int h), resize(x, y, w, h), Resize) \
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, type3##_I, handle(int event), handle(event), Handle)


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
    
    mutable unsigned int pVCalls;
    static const unsigned int pVCallDtor          = 1<<0;
    
    static const unsigned int pVCallWidgetDraw    = 1<<1;
    static const unsigned int pVCallWidgetHandle  = 1<<2;
    static const unsigned int pVCallWidgetResize  = 1<<3;
    static const unsigned int pVCallWidgetShow    = 1<<4;
    static const unsigned int pVCallWidgetHide    = 1<<5;
    static const unsigned int pVCallWidgetDrawOverlay         = 1<<6;
    static const unsigned int pVCallWidgetBrowserItemFirst    = 1<<7;
    static const unsigned int pVCallWidgetBrowserItemNext     = 1<<8;
    static const unsigned int pVCallWidgetBrowserItemPrev     = 1<<9;
    static const unsigned int pVCallWidgetBrowserItemLast     = 1<<10;
    static const unsigned int pVCallWidgetBrowserItemHeight   = 1<<11;
    static const unsigned int pVCallWidgetBrowserItemWidth    = 1<<12;
    static const unsigned int pVCallWidgetBrowserItemQuickHeight = 1<<13;
    static const unsigned int pVCallWidgetBrowserItemDraw     = 1<<14;
    static const unsigned int pVCallWidgetBrowserItemText     = 1<<15;
    static const unsigned int pVCallWidgetBrowserItemSwap     = 1<<16;
    static const unsigned int pVCallWidgetBrowserItemAt       = 1<<17;
    static const unsigned int pVCallWidgetBrowserFullWidth    = 1<<18;
    static const unsigned int pVCallWidgetBrowserFullHeight   = 1<<19;
    static const unsigned int pVCallWidgetBrowserIncrHeight   = 1<<20;
    static const unsigned int pVCallWidgetBrowserItemSelect   = 1<<21;
    static const unsigned int pVCallWidgetBrowserItemSelected = 1<<22;
    
    static const unsigned int pVCallImageCopyWH       = 1<<1;
    static const unsigned int pVCallImageColorAverage = 1<<2;
    static const unsigned int pVCallImageDesaturate   = 1<<3;
    static const unsigned int pVCallImageLabelW       = 1<<4;
    static const unsigned int pVCallImageLabelM       = 1<<5;
    static const unsigned int pVCallImageDraw         = 1<<6;
    static const unsigned int pVCallImageUncache      = 1<<7;
  };

  
  class WidgetWrapper : public Wrapper {
  public:
    virtual ~WidgetWrapper() {}
    FLTK3_WRAPPER_VCALLS_OBJECT(Widget, draw(), draw(), Draw)
    FLTK3_WRAPPER_VCALLS_OBJECT_RET(int, Widget, handle(int event), handle(event), Handle)
    FLTK3_WRAPPER_VCALLS_OBJECT(Widget, resize(int x, int y, int w, int h), resize(x, y, w, h), Resize)
    FLTK3_WRAPPER_VCALLS_OBJECT(Widget, show(), show(), Show)
    FLTK3_WRAPPER_VCALLS_OBJECT(Widget, hide(), hide(), Hide)
    virtual void draw_overlay() {} // neede by overlay windows
    virtual void *item_first() const { return 0; }
    virtual void *item_next(void *item) const { return 0; }
    virtual void *item_prev(void *item) const { return 0; }    
    virtual void *item_last() const { return 0; }
    virtual int item_height(void *item) const { return 0; }
    virtual int item_width(void *item) const { return 0; }
    virtual int item_quick_height(void *item) const { return 0; }
    virtual void item_draw(void *item,int X,int Y,int W,int H) const {}
    virtual const char *item_text(void *item) const { (void)item; return 0L; }
    virtual void item_swap(void *a,void *b) { (void)a; (void)b; }
    virtual void *item_at(int index) const { (void)index; return 0L; }
    virtual int full_width() const { return 0; }
    virtual int full_height() const { return 0; }
    virtual int incr_height() const { return 0; }
    virtual void item_select(void *item,int val=1) { }
    virtual int item_selected(void *item) const { return 0; }
  };
  
  
  class ImageWrapper : public Wrapper {
  public:
    /*
    virtual ~Image();
    virtual fltk3::Image *copy(int W, int H);
    virtual void color_average(fltk3::Color c, float i);
    virtual void desaturate();
    virtual void label(fltk3::Widget*w);
    virtual void label(fltk3::MenuItem*m);
    virtual void draw(int X, int Y, int W, int H, int cx=0, int cy=0); // platform dependent
    virtual void uncache();
     */
  };
  
}; // namespace fltk3


#endif

//
// End of "$Id: Wrapper.h 7365 2010-03-30 15:18:29Z matt $".
//
