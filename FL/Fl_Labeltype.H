//
// "$Id: Fl_Labeltype.H,v 1.2 1999/09/19 04:58:54 vincent Exp $"
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

// struct rather than class to allow FL_IMPORT static constants:
struct Fl_Labeltype_ {
  void (*draw_)(const Fl_Labeltype_*, const char*,
	       int,int,int,int, Fl_Color fill, Fl_Flags);
  void* data;
// public:
  void draw(const char* m, int x,int y,int w,int h, Fl_Color c, Fl_Flags f=0)
    const {draw_(this, m, x, y, w, h, c, f);}
};

typedef const Fl_Labeltype_* Fl_Labeltype;

FL_EXPORT FL_IMPORT extern Fl_Labeltype_ fl_no_label;
#define FL_NO_LABEL &fl_normal_label
FL_EXPORT FL_IMPORT extern Fl_Labeltype_ fl_normal_label;
#define FL_NORMAL_LABEL &fl_normal_label
FL_EXPORT FL_IMPORT extern const Fl_Labeltype_ fl_symbol_label;
#define FL_SYMBOL_LABEL &fl_symbol_label
FL_EXPORT FL_IMPORT extern const Fl_Labeltype_ fl_shadow_label;
#define FL_SHADOW_LABEL &fl_shadow_label
FL_EXPORT FL_IMPORT extern const Fl_Labeltype_ fl_engraved_label;
#define FL_ENGRAVED_LABEL &fl_engraved_label
FL_EXPORT FL_IMPORT extern const Fl_Labeltype_ fl_embossed_label;
#define FL_EMBOSSED_LABEL &fl_embossed_label

#endif
