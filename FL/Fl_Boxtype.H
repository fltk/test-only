//
// "$Id: Fl_Boxtype.H,v 1.20 2000/01/07 22:58:47 mike Exp $"
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

#include "Fl_Color.H"
#include "Fl_Flags.H"

#ifdef _MSC_VER
//#  pragma disable warning 4355
#endif // _MSC_VER

// the abstract base class:
class FL_API Fl_Boxtype_ {
public:
  virtual void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const = 0;
  virtual void inset(int&,int&,int&,int&) const = 0;
  virtual int fills_rectangle() const = 0;
  int dx() const {int x = 0, y = 0, w = 0, h = 0; inset(x,y,w,h); return x;}
  int dy() const {int x = 0, y = 0, w = 0, h = 0; inset(x,y,w,h); return y;}
  int dw() const {int x = 0, y = 0, w = 0, h = 0; inset(x,y,w,h); return -w;}
  int dh() const {int x = 0, y = 0, w = 0, h = 0; inset(x,y,w,h); return -h;}
  const char* name;
  const Fl_Boxtype_* next;
  static const Fl_Boxtype_* first;
  Fl_Boxtype_(const char* n) : name(n), next(first) {first = this;}
  static const Fl_Boxtype_* find(const char* name);
};

typedef const Fl_Boxtype_* Fl_Boxtype;

// draw a pattern of lines around a box, pattern is from the data string
// in which 'A'=black, 'X'=white, other characters are gray levels
class FL_API Fl_Frame_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  const char* data; // public so themes can mess with it...
  const Fl_Frame_Box* down;
  Fl_Frame_Box(const char* n, const char* c, const Fl_Frame_Box* d) :
    Fl_Boxtype_(n), data(c), down(d) {}
  Fl_Frame_Box(const char* n, const char* c) :
    Fl_Boxtype_(n), data(c), down(this) {}
};
extern FL_API Fl_Frame_Box fl_up_box;
#define FL_UP_BOX (&fl_up_box)
#define FL_NORMAL_BOX (&fl_up_box)
extern FL_API Fl_Frame_Box fl_down_box;
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

class FL_API Fl_Flat_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_Flat_Box(const char* n) : Fl_Boxtype_(n) {}
};
extern FL_API const Fl_Flat_Box fl_flat_box;
#define FL_FLAT_BOX (&fl_flat_box)

// Combination of flat & any other boxtype when highlighted or pressed:
class FL_API Fl_Highlight_Box : public Fl_Flat_Box {
  const Fl_Boxtype_* down;
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_Highlight_Box(const char* n, const Fl_Boxtype_* d) :
    Fl_Flat_Box(n), down(d) {}
};
extern FL_API const Fl_Highlight_Box fl_highlight_up_box;
#define FL_HIGHLIGHT_UP_BOX (&fl_highlight_up_box)
#define FL_FLAT_UP_BOX (&fl_highlight_up_box)
#define FL_HIGHLIGHT_BOX (&fl_highlight_up_box)
extern FL_API const Fl_Highlight_Box fl_highlight_down_box;
#define FL_HIGHLIGHT_DOWN_BOX (&fl_highlight_down_box)
#define FL_FLAT_DOWN_BOX (&fl_highlight_down_box)

// Circle, pattern of edges is stolen from another Fl_Frame_Box
class FL_API Fl_Round_Box : public Fl_Boxtype_ {
  const Fl_Frame_Box* data_from;
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_Round_Box(const char* n, const Fl_Frame_Box* d) :
    Fl_Boxtype_(n), data_from(d) {}
};
extern FL_API const Fl_Round_Box fl_round_up_box;
#define FL_ROUND_UP_BOX (&fl_round_up_box)
#define FL_ROUND_BOX (&fl_round_up_box)
extern FL_API const Fl_Round_Box fl_round_down_box;
#define FL_ROUND_DOWN_BOX (&fl_round_down_box)

// Diamond, pattern of edges is stolen from another Fl_Frame_Box
class FL_API Fl_Diamond_Box : public Fl_Boxtype_ {
  const Fl_Frame_Box* data_from;
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_Diamond_Box(const char* n, const Fl_Frame_Box* d) :
    Fl_Boxtype_(n), data_from(d) {}
};
extern FL_API const Fl_Diamond_Box fl_diamond_up_box;
#define FL_DIAMOND_UP_BOX (&fl_diamond_up_box)
#define FL_DIAMOND_BOX (&fl_diamond_up_box)
extern FL_API const Fl_Diamond_Box fl_diamond_down_box;
#define FL_DIAMOND_DOWN_BOX (&fl_diamond_down_box)

// a bunch of "one-of" types:

class FL_API Fl_No_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_No_Box(const char* n) : Fl_Boxtype_(n) {}
};
extern FL_API const Fl_No_Box fl_no_box;
#define FL_NO_BOX (&fl_no_box)

class FL_API Fl_Shadow_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_Shadow_Box(const char* n) : Fl_Boxtype_(n) {}
};
extern FL_API const Fl_Shadow_Box fl_shadow_box;
#define FL_SHADOW_BOX (&fl_shadow_box)

class FL_API Fl_Rounded_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_Rounded_Box(const char* n) : Fl_Boxtype_(n) {}
};
extern FL_API const Fl_Rounded_Box fl_rounded_box;
#define FL_ROUNDED_BOX (&fl_rounded_box)

class FL_API Fl_RShadow_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_RShadow_Box(const char* n) : Fl_Boxtype_(n) {}
};
extern FL_API const Fl_RShadow_Box fl_rshadow_box;
#define FL_RSHADOW_BOX (&fl_rshadow_box)

class FL_API Fl_RFlat_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_RFlat_Box(const char* n) : Fl_Boxtype_(n) {}
};
extern FL_API const Fl_RFlat_Box fl_rflat_box;
#define FL_RFLAT_BOX (&fl_rflat_box)

class FL_API Fl_Oval_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_Oval_Box(const char* n) : Fl_Boxtype_(n) {}
};
extern FL_API const Fl_Oval_Box fl_oval_box;
#define FL_OVAL_BOX (&fl_oval_box)

class FL_API Fl_Oval_Shadow_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_Oval_Shadow_Box(const char* n) : Fl_Boxtype_(n) {}
};
extern FL_API const Fl_Oval_Shadow_Box fl_oval_shadow_box;
#define FL_OSHADOW_BOX (&fl_oval_shadow_box)

class FL_API Fl_Oval_Flat_Box : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_Oval_Flat_Box(const char* n) : Fl_Boxtype_(n) {}
};
extern FL_API const Fl_Oval_Flat_Box fl_oval_flat_box;
#define FL_OFLAT_BOX (&fl_oval_flat_box)

class FL_API Fl_Border_Frame : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color fill, Fl_Flags=0) const;
  void inset(int&,int&,int&,int&) const;
  int fills_rectangle() const;
  Fl_Border_Frame(const char* n) : Fl_Boxtype_(n) {}
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
// End of "$Id: Fl_Boxtype.H,v 1.20 2000/01/07 22:58:47 mike Exp $".
//
