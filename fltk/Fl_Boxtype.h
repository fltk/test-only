//
// "$Id: Fl_Boxtype.h,v 1.1 2001/07/23 09:50:04 spitzak Exp $"
//
// Boxes used by FLTK styles.
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef Fl_Boxtype_h
#define Fl_Boxtype_h

#include "Fl_Color.h"
#include "Fl_Flags.h"

#ifdef _MSC_VER
//#  pragma disable warning 4355
#endif // _MSC_VER

class FL_API Fl_Widget;

// the abstract base class:
class FL_API Fl_Boxtype_ {
protected:
  int dx_, dy_, dw_, dh_;
  int fills_rectangle_;
public:
  virtual void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const = 0;
  int fills_rectangle() const {return fills_rectangle_;}
  int dx() const {return dx_;}
  int dy() const {return dy_;}
  int dw() const {return dw_;}
  int dh() const {return dh_;}
  void inset(int&X,int&Y,int&W,int&H) const {X+=dx_; Y+=dy_; W-=dw_; H-=dh_;}
  const char* name;
  const Fl_Boxtype_* next;
  static const Fl_Boxtype_* first;
  Fl_Boxtype_() : name(0) {}
  Fl_Boxtype_(const char* n) : name(n), next(first) {first = this;}
  static const Fl_Boxtype_* find(const char* name);
};

typedef const Fl_Boxtype_* Fl_Boxtype;

// draw a pattern of lines around a box, pattern is from the data string
// in which 'A'=black, 'X'=white, other characters are gray levels
class FL_API Fl_Frame_Box : public Fl_Boxtype_ {
protected:
  const char* data_;
  const Fl_Frame_Box* down;
public:
  const char* data() const {return data_;}
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Frame_Box(const char* n, const char* c, const Fl_Frame_Box* d=0);
};
extern FL_API const Fl_Frame_Box fl_up_box;
#define FL_UP_BOX (&fl_up_box)
#define FL_NORMAL_BOX (&fl_up_box)
extern FL_API const Fl_Frame_Box fl_down_box;
#define FL_DOWN_BOX (&fl_down_box)
extern FL_API const Fl_Frame_Box fl_thin_up_box;
#define FL_THIN_UP_BOX (&fl_thin_up_box)
#define FL_THIN_BOX (&fl_thin_up_box)
extern FL_API const Fl_Frame_Box fl_thin_down_box;
#define FL_THIN_DOWN_BOX (&fl_thin_down_box)
extern FL_API const Fl_Frame_Box fl_engraved_box;
#define FL_ENGRAVED_BOX (&fl_engraved_box)
extern FL_API const Fl_Frame_Box fl_embossed_box;
#define FL_EMBOSSED_BOX (&fl_embossed_box)
extern FL_API const Fl_Frame_Box fl_border_box;
#define FL_BORDER_BOX (&fl_border_box)

// no border, tiling pattern is in absolute coordinates:
class FL_API Fl_Flat_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Flat_Box(const char* n);
};
extern FL_API const Fl_Flat_Box fl_flat_box;
#define FL_FLAT_BOX (&fl_flat_box)

// Combination of flat & any other boxtype when highlighted or pressed:
class FL_API Fl_Highlight_Box : public Fl_Flat_Box {
  const Fl_Boxtype_* down;
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Highlight_Box(const char* n, const Fl_Boxtype_* d);
};
extern FL_API const Fl_Highlight_Box fl_highlight_up_box;
#define FL_HIGHLIGHT_UP_BOX (&fl_highlight_up_box)
#define FL_FLAT_UP_BOX (&fl_highlight_up_box)
#define FL_HIGHLIGHT_BOX (&fl_highlight_up_box)
extern FL_API const Fl_Highlight_Box fl_highlight_down_box;
#define FL_HIGHLIGHT_DOWN_BOX (&fl_highlight_down_box)
#define FL_FLAT_DOWN_BOX (&fl_highlight_down_box)

// Circle with an edge pattern like Fl_Frame_Box:
class FL_API Fl_Round_Box : public Fl_Frame_Box {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Round_Box(const char* n, const char* c, const Fl_Frame_Box* d=0);
};
extern FL_API const Fl_Round_Box fl_round_up_box;
#define FL_ROUND_UP_BOX (&fl_round_up_box)
#define FL_ROUND_BOX (&fl_round_up_box)
extern FL_API const Fl_Round_Box fl_round_down_box;
#define FL_ROUND_DOWN_BOX (&fl_round_down_box)

// Diamond with an edge pattern like Fl_Frame_Box:
class FL_API Fl_Diamond_Box : public Fl_Frame_Box {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Diamond_Box(const char* n, const char* c, const Fl_Frame_Box* d=0);
};
extern FL_API const Fl_Diamond_Box fl_diamond_up_box;
#define FL_DIAMOND_UP_BOX (&fl_diamond_up_box)
#define FL_DIAMOND_BOX (&fl_diamond_up_box)
extern FL_API const Fl_Diamond_Box fl_diamond_down_box;
#define FL_DIAMOND_DOWN_BOX (&fl_diamond_down_box)

// a bunch of "one-of" types:

class FL_API Fl_No_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_No_Box(const char* n);
};
extern FL_API const Fl_No_Box fl_no_box;
#define FL_NO_BOX (&fl_no_box)

class FL_API Fl_Shadow_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Shadow_Box(const char* n);
};
extern FL_API const Fl_Shadow_Box fl_shadow_box;
#define FL_SHADOW_BOX (&fl_shadow_box)

class FL_API Fl_Rounded_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Rounded_Box(const char* n);
};
extern FL_API const Fl_Rounded_Box fl_rounded_box;
#define FL_ROUNDED_BOX (&fl_rounded_box)

class FL_API Fl_RShadow_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_RShadow_Box(const char* n);
};
extern FL_API const Fl_RShadow_Box fl_rshadow_box;
#define FL_RSHADOW_BOX (&fl_rshadow_box)

class FL_API Fl_RFlat_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_RFlat_Box(const char* n);
};
extern FL_API const Fl_RFlat_Box fl_rflat_box;
#define FL_RFLAT_BOX (&fl_rflat_box)

class FL_API Fl_Oval_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Oval_Box(const char* n);
};
extern FL_API const Fl_Oval_Box fl_oval_box;
#define FL_OVAL_BOX (&fl_oval_box)

class FL_API Fl_Oval_Shadow_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Oval_Shadow_Box(const char* n);
};
extern FL_API const Fl_Oval_Shadow_Box fl_oval_shadow_box;
#define FL_OSHADOW_BOX (&fl_oval_shadow_box)

class FL_API Fl_Oval_Flat_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Oval_Flat_Box(const char* n);
};
extern FL_API const Fl_Oval_Flat_Box fl_oval_flat_box;
#define FL_OFLAT_BOX (&fl_oval_flat_box)

class FL_API Fl_Border_Frame : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Border_Frame(const char* n);
};
extern FL_API const Fl_Border_Frame fl_border_frame;
#define FL_BORDER_FRAME (&fl_border_frame)

#ifndef FLTK_2 // back compatability section
#define FL_UP_FRAME (&fl_up_box)
#define FL_DOWN_FRAME (&fl_down_box)
#define FL_THIN_UP_FRAME (&fl_thin_box)
#define FL_THIN_DOWN_FRAME (&fl_thin_down_box)
#define FL_ENGRAVED_FRAME (&fl_engraved_box)
#define FL_EMBOSSED_FRAME (&fl_embossed_box)
#define FL_SHADOW_FRAME (&fl_shadow_box)
#define FL_ROUNDED_FRAME (&fl_rounded_box)
#define FL_OVAL_FRAME (&fl_oval_box)
#define FL_FRAME FL_ENGRAVED_FRAME
#define FL_FRAME_BOX FL_ENGRAVED_BOX
#endif

#endif

//
// End of "$Id: Fl_Boxtype.h,v 1.1 2001/07/23 09:50:04 spitzak Exp $".
//
