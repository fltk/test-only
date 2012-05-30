//
// "$Id$"
//
// Box header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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
 fltk3::Widget widget . */

#ifndef FLTK3_BOX_H
#define FLTK3_BOX_H

#include "Symbol.h"

/* Comment from matt , r9557
 * Removed fltk3::Box as a wiget class. In FLTK2, boxes are drawn simply by 
 * using fltk::Widget. I assume we could add fltk3::BoxWidget. Why all this? 
 * 'Box' will be used later as a base class for a much more flexible Boxtype.
 */

namespace fltk3 {
  
  class FLTK3_EXPORT Box : public Symbol {
  public:
    Box(const char *name=0L) 
    : Symbol(name) { }
    
    /**
     For shaded boxes, return the box with "raised" graphics.
     */
    virtual Box* up() { return this; }
    
    /**
     For shaded boxes, return the box with "lowered" graphics.
     */
    virtual Box* down() { return this; }
    
    /**
     Return the corresponding frame type (box without background).
     */
    virtual Box* frame() { return this; }

    /**
     Return the corresponding filled box type (box with background).
     */
    virtual Box* box() { return this; }
  };
  
  class FLTK3_EXPORT NoBox : public Box {
  public:
    void _draw(const Rectangle&) const;
    NoBox(const char* name) : fltk3::Box(name) { set_inset(0, 0, 0, 0); }
  };

  
  class FLTK3_EXPORT FlatBox : public Box {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return true; }
    bool is_frame() const { return true; }
    FlatBox(const char* name) : fltk3::Box(name) { set_inset(0, 0, 0, 0); }
  };
  
  class FLTK3_EXPORT UpBox : public FlatBox {
  public:
    Box* down() { return DOWN_BOX; }
    void _draw(const Rectangle&) const;
    UpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT DownBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    DownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT ThinUpBox : public FlatBox {
  public:
    Box* down() { return THIN_DOWN_BOX; }
    void _draw(const Rectangle&) const;
    ThinUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT ThinDownBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    ThinDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT EngravedBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    EngravedBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT EmbossedBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    EmbossedBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT BorderBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    BorderBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT ShadowBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    ShadowBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -5, -5); }
  };
  
  class FLTK3_EXPORT RoundedBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    RoundedBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, 2, 2); }
  };
  
  class FLTK3_EXPORT RShadowBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    RShadowBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT RFlatBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    RFlatBox(const char* name) : fltk3::FlatBox(name) { set_inset(0, 0, 0, 0); }
  };
  
  class FLTK3_EXPORT RoundUpBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    RoundUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT RoundDownBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    RoundDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT DiamondUpBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    DiamondUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(0, 0, 0, 0); }
  };
  
  class FLTK3_EXPORT DiamondDownBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    DiamondDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(0, 0, 0, 0); }
  };
  
  class FLTK3_EXPORT OvalBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    OvalBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT OShadowBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    OShadowBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT OFlatBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    OFlatBox(const char* name) : fltk3::FlatBox(name) { set_inset(0, 0, 0, 0); }
  };
  
  class FLTK3_EXPORT PlasticUpBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    PlasticUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(4, 4, -8, -8); }
  };
  
  class FLTK3_EXPORT PlasticDownBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    PlasticDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT PlasticThinUpBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    PlasticThinUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT PlasticThinDownBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    PlasticThinDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT PlasticRoundUpBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    PlasticRoundUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT PlasticRoundDownBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    PlasticRoundDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT ClassicUpBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    ClassicUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT ClassicDownBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    ClassicDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT ClassicThinUpBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    ClassicThinUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT ClassicThinDownBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    ClassicThinDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT ClassicRoundUpBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    ClassicRoundUpBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT ClassicRoundDownBox : public FlatBox {
  public:
    void _draw(const Rectangle&) const;
    ClassicRoundDownBox(const char* name) : fltk3::FlatBox(name) { set_inset(2, 2, -4, -4); }
  };
  
  
  class FLTK3_EXPORT BorderFrame : public Box {
  public:
    void _draw(const Rectangle&) const;
    bool fills_rectangle() const { return false; }
    bool is_frame() const { return true; }
    BorderFrame(const char* name) : fltk3::Box(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT UpFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    UpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT DownFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    DownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT ThinUpFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    ThinUpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT ThinDownFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    ThinDownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT EngravedFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    EngravedFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT EmbossedFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    EmbossedFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT ShadowFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    ShadowFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -5, -5); }
  };
  
  class FLTK3_EXPORT RoundedFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    RoundedFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT OvalFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    OvalFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT PlasticUpFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    PlasticUpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT PlasticDownFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    PlasticDownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT ClassicUpFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    ClassicUpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT ClassicDownFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    ClassicDownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(2, 2, -4, -4); }
  };
  
  class FLTK3_EXPORT ClassicThinUpFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    ClassicThinUpFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
  };
  
  class FLTK3_EXPORT ClassicThinDownFrame : public BorderFrame {
  public:
    void _draw(const Rectangle&) const;
    ClassicThinDownFrame(const char* name) : fltk3::BorderFrame(name) { set_inset(1, 1, -2, -2); }
  };

}


#endif

//
// End of "$Id$".
//
