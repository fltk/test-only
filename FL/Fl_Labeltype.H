//
// "$Id: Fl_Labeltype.H,v 1.5 1999/11/01 02:21:14 carl Exp $"
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

#include "Fl_Color.H"
#include "Fl_Flags.H"

// struct rather than class to allow static constants:
struct FL_API Fl_Labeltype_ {
  void (*draw_)(const Fl_Labeltype_*, const char*,
	       int,int,int,int, Fl_Color fill, Fl_Flags);
  void* data;
// public:
  void draw(const char* m, int x,int y,int w,int h, Fl_Color c, Fl_Flags f=0)
    const {draw_(this, m, x, y, w, h, c, f);}

  static const Fl_Labeltype_* find(const char* name);
};

typedef const Fl_Labeltype_* Fl_Labeltype;

// Create a global object of this type with correct parameter to define
// a new labeltype class.  Plugins can search this to find the labeltypes
// used by a program:
class FL_API Fl_Labeltype_Definer {
public:
  const char* name;
  const Fl_Labeltype_* labeltype;
  Fl_Labeltype_Definer* next;
  static Fl_Labeltype_Definer* first;
  Fl_Labeltype_Definer(char* n, const Fl_Labeltype_& s) : name(n), labeltype(&s), next(first) {
    first = this;}
};

extern FL_API Fl_Labeltype_ fl_no_label;
#define FL_NO_LABEL &fl_no_label
extern FL_API Fl_Labeltype_ fl_normal_label;
#define FL_NORMAL_LABEL &fl_normal_label
extern FL_API const Fl_Labeltype_ fl_symbol_label;
#define FL_SYMBOL_LABEL &fl_symbol_label
extern FL_API const Fl_Labeltype_ fl_shadow_label;
#define FL_SHADOW_LABEL &fl_shadow_label
extern FL_API const Fl_Labeltype_ fl_engraved_label;
#define FL_ENGRAVED_LABEL &fl_engraved_label
extern FL_API const Fl_Labeltype_ fl_embossed_label;
#define FL_EMBOSSED_LABEL &fl_embossed_label

#endif
