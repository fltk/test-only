//
// "$Id: Fl_Boxtype.h,v 1.4 2002/10/29 00:37:22 easysw Exp $"
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

// the abstract base class:

class FL_API Fl_Boxtype_ {
protected:
  int dx_, dy_, dw_, dh_;
  int fills_rectangle_;
public:
   /**
   This is the function that draws the box. 
   The four integers are the x,y,w,h of the rectangle to 
   fill with the box. The color is used to fill the interior of 
   the box with (except for FRAME types which use it to draw the edge). 
   The last argument is a set of bitflags, the following ones are 
   useful (ignore any other bits that are on): 

   - FL_INACTIVE  - gray out the widget. 
   - FL_VALUE     - draw the box pushed in. Typically this is implemented 
                    by calling another boxtype to do the drawing. 
   - FL_HIGHLIGHT - draw the box highlighted. Usually you can ignore 
                    this because the fill color will also be changed. 
   - FL_SELECTED  - the widget has the focus. 
   - FL_INVISIBLE - you should only draw the outer edge of the box and 
                    not fill it in. This is used to draw the boxes around 
                    browsers and text widgets, and to draw the focus box. 
                    You can ignore this and things will still draw ok, 
                    but they may flicker more than necessary. 
   
   A simple drawing function might fill a rectangle with the 
   given color and then draw a black outline: 

   \code
   void MyBoxtype::draw(int x, int y, int w, int h, Fl_Color c, Fl_Flags f) const
   {
     // draw the interior:
     if (!(f&FL_INVISIBLE)) {
       fl_color(c);
       fl_rectf(x+1, y+1, w-2, h-2);
     }
     // draw the edge:
     fl_color(FL_BLACK);
     fl_rect(x, y, w, h);
   }
   \endcode
   */
  virtual void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const = 0;
  /**
   Returns true if the draw function completely fills all the pixels 
   in the rectangle passed to it (ignoring any FL_INVISIBLE flag). 
   Many parts of fltk will take advantage of this to speed up drawing 
   and eliminate blinking. 
   This is an inline funcion, if you are making a subclass your 
   constructor should set the protected member variable fills_rectangle_ 
   with the value you want returned. 
  */
  int fills_rectangle() const {return fills_rectangle_;}
  /*@{*/
  /**
   Return the offsets for the bounding box that should be subtracted when 
   drawing the label inside the box. These are all positive numbers, 
   so dx() and dy() are added to the x and y, while dw() and dh() 
   are subtracted from the width and height. Usually dw() is two 
   times dx(), and dh() is two times dy(), and usually dx() 
   and dy() are equal. 
   These are inline functions, if you are making a subclass your 
   constructor should set the protected members dx_, dy_, dw_, 
   and dh_ to initialize the return value. 
  */
  int dx() const {return dx_;}
  int dy() const {return dy_;}
  int dw() const {return dw_;}
  int dh() const {return dh_;}
  /*@}*/
  /**
   Changes the passed rectangle into a rectangle that matches 
   the "interior" of the box. This is an inline function that 
   just adds or subtracts dx(), dy(), dw(), and dh() from the passed values.
  */
  void inset(int&X,int&Y,int&W,int&H) const {X+=dx_; Y+=dy_; W-=dw_; H-=dh_;}
  const char* name;
  const Fl_Boxtype_* next;
  static const Fl_Boxtype_* first;
  /** Default constructor. */
  Fl_Boxtype_() : name(0) {}
  /**
   This constructs a "named" boxtype. It is added to a linked 
   list of all named boxtypes. This list may be searched by 
   the find() function to find a boxtype by name. This is 
   useful for themes that want to parse a text file describing the theme. 
  */
  Fl_Boxtype_(const char* n) : name(n), next(first) {first = this;}
  /**
   Locate the first boxtype with the given name. 
   Case is ignored when doing the comparison. 
   Returns NULL if no matching name is found.
  */
  static const Fl_Boxtype_* find(const char* name);
};

/**

   Fl_Boxtype is a pointer to the structure Fl_Boxtype_, which contains the 
   information needed to draw the rectangles around and inside widgets. 
   
   \image boxtypes.gif

   Boxtypes are stored in the box, button_box, and focus_box fields 
   of the Fl_Style structure, and typically used by the draw() methods of 
   Fl_Widgets. 

*/
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

// Plastic/Aqua shading:
class FL_API Fl_Plastic_Box : public Fl_Frame_Box {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Plastic_Box(const char* n, const char* c, const Fl_Frame_Box* d=0);
};
extern FL_API const Fl_Plastic_Box fl_plastic_up_box;
#define FL_PLASTIC_UP_BOX (&fl_plastic_up_box)
#define FL_PLASTIC_BOX (&fl_plastic_up_box)
extern FL_API const Fl_Plastic_Box fl_plastic_down_box;
#define FL_PLASTIC_DOWN_BOX (&fl_plastic_down_box)

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

class FL_API Fl_Dotted_Frame : public Fl_Boxtype_ {
public:
  void draw(int,int,int,int, Fl_Color, Fl_Flags=0) const;
  Fl_Dotted_Frame(const char* n);
};
extern FL_API const Fl_Dotted_Frame fl_dotted_frame;
#define FL_DOTTED_FRAME (&fl_dotted_frame)

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
// End of "$Id: Fl_Boxtype.h,v 1.4 2002/10/29 00:37:22 easysw Exp $".
//
