//
// "$Id: Fl_Labeltype.h,v 1.2 2002/10/29 00:37:23 easysw Exp $"
//
// Labeltypes used by FLTK styles.
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

#ifndef Fl_Labeltype_h
#define Fl_Labeltype_h

#include "Fl_Color.h"
#include "Fl_Flags.h"

// the abstract base class:
class FL_API Fl_Labeltype_ {
public:
   /**
      The first argument is the Fl_Labeltype_ itself, this allows access to 
      the data value. Next is the text string. The next four integers are 
      the x,y,w,h to fit the label into. The next is the color to fill the 
      interior of the letters with. The last argument is a set of bitflags.
      The following values are of use to the labeltypes, ignore any other 
      bits that are on: 

      - FL_ALIGN_TOP - align the label at the top of the widget. 
      - FL_ALIGN_BOTTOM - align the label at the bottom of the widget. 
      - FL_ALIGN_LEFT - align the label to the left of the widget. 
      - FL_ALIGN_RIGHT - align the label to the right of the widget. 
      - FL_ALIGN_CLIP - clip the label to the widget's bounding box. 
      - FL_ALIGN_WRAP - wrap the label text as needed. 
      - FL_SHORTCUT_LABEL - interpret &x to underscore under the next letter. 
      - FL_INACTIVE - gray out the widget. 
      - FL_VALUE - 
         draw the label atop a pushed-in box (usually you can ignore this). 
      - FL_SELECTED - 
         draw the label atop a box with keyboard focus 
         (usually you can ignore this). 
      - FL_HIGHLIGHT - 
         draw the label atop a highlighted box (usually you can ignore this).
      
      The label should be drawn inside this bounding box. Ignore the setting 
      of FL_ALIGN_INSIDE. Outside labels are handled by the Fl_Group code by
      modifying the bounding box before this is called.
      
      If none of the align flags are on you should center the label 
      in the widget. 

      The function is not called if the label value is NULL. 
   */
  virtual void draw(const char*, int,int,int,int, Fl_Color, Fl_Flags) const;
  const char* name;
  const Fl_Labeltype_* next;
  static const Fl_Labeltype_* first;
  Fl_Labeltype_(const char* n) : name(n), next(first) {first = this;}
  static const Fl_Labeltype_* find(const char* name);
};

/**
   Fl_Labeltype is a pointer to the structure Fl_Labeltype_, 
   which contains the information needed to draw text labels. 
   This is used by Fl_Widget::label_type(). 
*/
typedef const Fl_Labeltype_* Fl_Labeltype;

extern FL_API Fl_Labeltype_ fl_normal_label;
#define FL_NORMAL_LABEL (&fl_normal_label)

class FL_API Fl_No_Label : public Fl_Labeltype_ {
public:
  void draw(const char*, int,int,int,int, Fl_Color, Fl_Flags) const;
  Fl_No_Label(const char * n) : Fl_Labeltype_(n) {}
};
extern FL_API Fl_No_Label fl_no_label;
#define FL_NO_LABEL (&fl_no_label)

class FL_API Fl_Symbol_Label : public Fl_Labeltype_ {
public:
  void draw(const char*, int,int,int,int, Fl_Color, Fl_Flags) const;
  Fl_Symbol_Label(const char * n) : Fl_Labeltype_(n) {}
};
extern FL_API const Fl_Symbol_Label fl_symbol_label;
#define FL_SYMBOL_LABEL (&fl_symbol_label)

class FL_API Fl_Engraved_Label : public Fl_Labeltype_ {
  const int* data;
public:
  void draw(const char*, int,int,int,int, Fl_Color, Fl_Flags) const;
  Fl_Engraved_Label(const char * n, const int p[][3])
    : Fl_Labeltype_(n), data((const int*)p) {}
};
extern FL_API const Fl_Engraved_Label fl_shadow_label;
#define FL_SHADOW_LABEL (&fl_shadow_label)
extern FL_API const Fl_Engraved_Label fl_engraved_label;
#define FL_ENGRAVED_LABEL (&fl_engraved_label)
extern FL_API const Fl_Engraved_Label fl_embossed_label;
#define FL_EMBOSSED_LABEL (&fl_embossed_label)

#endif
