//
// "$Id$"
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
#include <fltk3/Image.h>

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


#define FLTK3_WRAPPER_VCALLS_OBJECT_DTOR(type1, type3) \
virtual ~type1() { \
  if ( !(pVCalls & pVCallDtor) ) { \
    pVCalls |= pVCallDtor; \
  } \
}

#define FLTK3_WRAPPER_VCALLS_OBJECT(type, klass, proto, call, flag) \
virtual void proto { \
  if ( pVCalls & pVCall##type##flag ) { \
    ((fltk3::klass*)_p)->call; \
  } else { \
    pVCalls |= pVCall##type##flag; \
    ((fltk3::klass*)_p)->call; \
    pVCalls &= ~pVCall##type##flag; \
  } \
}
#define FLTK3_WRAPPER_VCALLS_WIDGET(klass, proto, call, flag) \
  FLTK3_WRAPPER_VCALLS_OBJECT(Widget, klass, proto, call, flag)
#define FLTK3_WRAPPER_VCALLS_IMAGE(klass, proto, call, flag) \
  FLTK3_WRAPPER_VCALLS_OBJECT(Image, klass, proto, call, flag)

#define FLTK3_WRAPPER_VCALLS_OBJECT_RET(type, rtype, klass, proto, call, flag) \
virtual rtype proto { \
  rtype ret = 0; \
  if ( pVCalls & pVCall##type##flag ) { \
    ret = ((fltk3::klass*)_p)->call; \
  } else { \
    pVCalls |= pVCall##type##flag; \
    ret = ((fltk3::klass*)_p)->call; \
    pVCalls &= ~pVCall##type##flag; \
  } \
  return ret; \
}
#define FLTK3_WRAPPER_VCALLS_WIDGET_RET(rtype, klass, proto, call, flag) \
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(Widget, rtype, klass, proto, call, flag)
#define FLTK3_WRAPPER_VCALLS_IMAGE_RET(rtype, klass, proto, call, flag) \
  FLTK3_WRAPPER_VCALLS_OBJECT_RET(Image, rtype, klass, proto, call, flag)

#define FLTK3_OBJECT_VCALLS_WRAPPER_DTOR() \
  if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCallDtor) ) { \
    pWrapper->pVCalls |= Wrapper::pVCallDtor; \
    delete ((Wrapper*)pWrapper); \
    return; \
  }

#define FLTK3_OBJECT_VCALLS_WRAPPER(type, call, flag) \
  if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCall##type##flag) ) { \
    pWrapper->pVCalls |= Wrapper::pVCall##type##flag; \
    ((type##Wrapper*)pWrapper)->call; \
    pWrapper->pVCalls &= ~Wrapper::pVCall##type##flag; \
    return; \
  }
#define FLTK3_WIDGET_VCALLS_WRAPPER(call, flag) \
  FLTK3_OBJECT_VCALLS_WRAPPER(Widget, call, flag)
#define FLTK3_IMAGE_VCALLS_WRAPPER(call, flag) \
  FLTK3_OBJECT_VCALLS_WRAPPER(Image, call, flag)

#define FLTK3_OBJECT_VCALLS_WRAPPER_RET(type, rtype, call, flag) \
  if (pWrapper && !(pWrapper->pVCalls & Wrapper::pVCall##type##flag) ) { \
    pWrapper->pVCalls |= Wrapper::pVCall##type##flag; \
    rtype ret = ((type##Wrapper*)pWrapper)->call; \
    pWrapper->pVCalls &= ~Wrapper::pVCall##type##flag; \
    return ret; \
  }
#define FLTK3_WIDGET_VCALLS_WRAPPER_RET(rtype, call, flag) \
  FLTK3_OBJECT_VCALLS_WRAPPER_RET(Widget, rtype, call, flag)
#define FLTK3_IMAGE_VCALLS_WRAPPER_RET(rtype, call, flag) \
  FLTK3_OBJECT_VCALLS_WRAPPER_RET(Image, rtype, call, flag)

#define FLTK3_WRAPPER_INTERFACE_BEGIN(type1, type3) \
  class type1; \
  namespace fltk3 { \
    class type3##_I : public type3 { \
      friend class ::type1; \
    public: \
      virtual ~type3##_I() { \
        FLTK3_OBJECT_VCALLS_WRAPPER_DTOR() \
      }

#define FLTK3_WRAPPER_INTERFACE_WIDGET(type1, type3) \
      type3##_I(int x, int y, int w, int h, const char *l) \
      : type3(x, y, w, h, l) { } \
      void show() { \
        FLTK3_WIDGET_VCALLS_WRAPPER(show(), Show) \
        type3::show(); \
      } \
      void hide() { \
        FLTK3_WIDGET_VCALLS_WRAPPER(hide(), Hide) \
        type3::hide(); \
      } \
      void resize(int X, int Y, int W, int H) { \
        FLTK3_WIDGET_VCALLS_WRAPPER(resize(X, Y, W, H), Resize) \
        type3::resize(X, Y, W, H); \
      } \
      void draw() { \
        FLTK3_WIDGET_VCALLS_WRAPPER(draw(), Draw) \
        type3::draw(); \
      } \
      int handle(int event) { \
        FLTK3_WIDGET_VCALLS_WRAPPER_RET(int, handle(event), Handle) \
        return type3::handle(event); \
      }

#define FLTK3_WRAPPER_INTERFACE_WINDOW(type1, type3) \
      void draw_overlay() { \
        FLTK3_WIDGET_VCALLS_WRAPPER(draw_overlay(), DrawOverlay) \
        type3::draw_overlay(); \
      } \
      type3##_I(int w, int h, const char *l) \
      : type3(w, h, l) { }

#define FLTK3_WRAPPER_INTERFACE_IMAGE_CTOR_5ARGS(type3) \
  type3##_I(const uchar *bits, int W, int H, int D=3, int LD=0) : type3(bits, W, H, D, LD) { } 

#define FLTK3_WRAPPER_INTERFACE_IMAGE_CTOR_3ARGS(type3) \
  type3##_I(const uchar *bits, int W, int H) : type3(bits, W, H) { } 

#define FLTK3_WRAPPER_INTERFACE_IMAGE_BODY(type3) \
  Image *copy(int W, int H) { \
  FLTK3_IMAGE_VCALLS_WRAPPER_RET(Image *, copy(W, H), CopyWH) \
  return type3::copy(W, H); \
  } \
  void color_average(Color c, float i) { \
  FLTK3_IMAGE_VCALLS_WRAPPER(color_average(c, i), ColorAverage) \
  type3::color_average(c, i); \
  } \
  void desaturate() { \
  FLTK3_IMAGE_VCALLS_WRAPPER(desaturate(), Desaturate) \
  type3::desaturate(); \
  } \
  void label(Widget *w) { \
  FLTK3_IMAGE_VCALLS_WRAPPER(label(w), LabelW) \
  type3::label(w); \
  } \
  void label(MenuItem *w) { \
  FLTK3_IMAGE_VCALLS_WRAPPER(label(w), LabelM) \
  type3::label(w); \
  } \
  void draw(int X, int Y, int W, int H, int cx=0, int cy=0) { \
  FLTK3_IMAGE_VCALLS_WRAPPER(draw(X, Y, W, H, cx, cy), Draw) \
  type3::draw(X, Y, W, H, cx, cy); \
  } \
  void uncache() { \
  FLTK3_IMAGE_VCALLS_WRAPPER(uncache(), Uncache) \
  type3::uncache(); \
  }

#define FLTK3_WRAPPER_INTERFACE_IMAGE(type1, type3) \
  FLTK3_WRAPPER_INTERFACE_IMAGE_CTOR_5ARGS(type3) \
  FLTK3_WRAPPER_INTERFACE_IMAGE_BODY(type3)

#define FLTK3_WRAPPER_INTERFACE_BITMAP(type1, type3) \
  FLTK3_WRAPPER_INTERFACE_IMAGE_CTOR_3ARGS(type3) \
  FLTK3_WRAPPER_INTERFACE_IMAGE_BODY(type3)

#define FLTK3_WRAPPER_INTERFACE_END() \
    }; \
  }


#define FLTK3_WIDGET_VCALLS(type1, type3) \
  FLTK3_WRAPPER_VCALLS_OBJECT_DTOR(type1, type3##_I) \
  FLTK3_WRAPPER_VCALLS_WIDGET(type3##_I, show(), show(), Show) \
  FLTK3_WRAPPER_VCALLS_WIDGET(type3##_I, hide(), hide(), Hide) \
  FLTK3_WRAPPER_VCALLS_WIDGET(type3##_I, draw(), draw(), Draw) \
  FLTK3_WRAPPER_VCALLS_WIDGET(type3##_I, resize(int x, int y, int w, int h), resize(x, y, w, h), Resize) \
  FLTK3_WRAPPER_VCALLS_WIDGET_RET(int, type3##_I, handle(int event), handle(event), Handle)


#define FLTK3_WINDOW_VCALLS(type1, type3) \
  FLTK3_WIDGET_VCALLS(type1, type3) \
  FLTK3_WRAPPER_VCALLS_WIDGET(type3##_I, draw_overlay(), draw_overlay(), DrawOverlay)

/*
#define FLTK3_IMAGE_VCALLS(type1, type3) \
  FLTK3_WRAPPER_VCALLS_OBJECT_DTOR(type1, type3##_I) \
  FLTK3_WRAPPER_VCALLS_IMAGE_RET(fltk3::Image *, type3##_I, copy(int w, int h), copy(w, h), CopyWH) \
  FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, color_average(Fl_Color c, float i), color_average(fltk3::_1to3_color(c), i), ColorAverage) \
  FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, desaturate(), desaturate(), Desaturate) \
  FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, label(Fl_Widget *w), label(fltk3::_1to3_widget(w)), LabelW) \
  FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, label(Fl_Menu_Item *w), label((fltk3::MenuItem*)w), LabelM) \
  FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, draw(int X, int Y, int W, int H, int cx=0, int cy=0), draw(X, Y, W, H, cx, cy), Draw) \
  FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, uncache(), uncache(), Uncache)
*/
#define FLTK3_IMAGE_VCALLS(type1, type3) \
FLTK3_WRAPPER_VCALLS_OBJECT_DTOR(type1, type3##_I) \
FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, color_average(Fl_Color c, float i), color_average(fltk3::_1to3_color(c), i), ColorAverage) \
FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, desaturate(), desaturate(), Desaturate) \
FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, label(Fl_Widget *w), label(fltk3::_1to3_widget(w)), LabelW) \
FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, label(Fl_Menu_Item *w), label((fltk3::MenuItem*)w), LabelM) \
FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, draw(int X, int Y, int W, int H, int cx=0, int cy=0), draw(X, Y, W, H, cx, cy), Draw) \
FLTK3_WRAPPER_VCALLS_IMAGE(type3##_I, uncache(), uncache(), Uncache)


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
	enum {
      pVCallDtor          = 1<<0,
    
      pVCallWidgetDraw    = 1<<1,
      pVCallWidgetHandle  = 1<<2,
      pVCallWidgetResize  = 1<<3,
      pVCallWidgetShow    = 1<<4,
      pVCallWidgetHide    = 1<<5,
      pVCallWidgetDrawOverlay         = 1<<6,
      pVCallWidgetBrowserItemFirst    = 1<<7,
      pVCallWidgetBrowserItemNext     = 1<<8,
      pVCallWidgetBrowserItemPrev     = 1<<9,
      pVCallWidgetBrowserItemLast     = 1<<10,
      pVCallWidgetBrowserItemHeight   = 1<<11,
      pVCallWidgetBrowserItemWidth    = 1<<12,
      pVCallWidgetBrowserItemQuickHeight = 1<<13,
      pVCallWidgetBrowserItemDraw     = 1<<14,
      pVCallWidgetBrowserItemText     = 1<<15,
      pVCallWidgetBrowserItemSwap     = 1<<16,
      pVCallWidgetBrowserItemAt       = 1<<17,
      pVCallWidgetBrowserFullWidth    = 1<<18,
      pVCallWidgetBrowserFullHeight   = 1<<19,
      pVCallWidgetBrowserIncrHeight   = 1<<20,
      pVCallWidgetBrowserItemSelect   = 1<<21,
      pVCallWidgetBrowserItemSelected = 1<<22,
    
      pVCallImageCopyWH       = 1<<1,
      pVCallImageColorAverage = 1<<2,
      pVCallImageDesaturate   = 1<<3,
      pVCallImageLabelW       = 1<<4,
      pVCallImageLabelM       = 1<<5,
      pVCallImageDraw         = 1<<6,
      pVCallImageUncache      = 1<<7
	};
  };
  
  class WidgetWrapper : public Wrapper {
  public:
    virtual ~WidgetWrapper() {}
    FLTK3_WRAPPER_VCALLS_WIDGET(Widget, draw(), draw(), Draw)
    FLTK3_WRAPPER_VCALLS_WIDGET_RET(int, Widget, handle(int event), handle(event), Handle)
    FLTK3_WRAPPER_VCALLS_WIDGET(Widget, resize(int x, int y, int w, int h), resize(x, y, w, h), Resize)
    FLTK3_WRAPPER_VCALLS_WIDGET(Widget, show(), show(), Show)
    FLTK3_WRAPPER_VCALLS_WIDGET(Widget, hide(), hide(), Hide)
    virtual void draw_overlay() {} // needed by overlay windows
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
    virtual ~ImageWrapper();
    FLTK3_WRAPPER_VCALLS_IMAGE_RET(Image *, Image, copy(int w, int h), copy(w, h), CopyWH)
    FLTK3_WRAPPER_VCALLS_IMAGE(Image, color_average(Color c, float i), color_average(c, i), ColorAverage);
    FLTK3_WRAPPER_VCALLS_IMAGE(Image, desaturate(), desaturate(), Desaturate);
    FLTK3_WRAPPER_VCALLS_IMAGE(Image, label(Widget *w), label(w), LabelW);
    FLTK3_WRAPPER_VCALLS_IMAGE(Image, label(MenuItem *w), label(w), LabelM);
    FLTK3_WRAPPER_VCALLS_IMAGE(Image, draw(int X, int Y, int W, int H, int cx=0, int cy=0), draw(X, Y, W, H, cx, cy), Draw);
    FLTK3_WRAPPER_VCALLS_IMAGE(Image, uncache(), uncache(), Uncache);
  };
  
}; // namespace fltk3


#endif

//
// End of "$Id$".
//
