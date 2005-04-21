//
// "$Id$"
//
// Style Implementation header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//


//  DO NOT include this header unmess you derive a new Style class
// (a subclass of Fl_Widget_Style ...)Endusers (not FLTK developers) 
// should disregard this header.

#ifndef Fl_Style_List_H
#define Fl_Style_List_H

class Fl_Widget_Style;

class Fl_Style_List{
public:
  Fl_Style_List * parent_;
  Fl_Style_List * children_;
  Fl_Widget_Style * normal_;
  Fl_Widget_Style * highlight_;
  Fl_Style_List * prev_; // for easy deleting;
  Fl_Style_List * next_;
  unsigned mask_;
  unsigned parent_mask_;
public:
  Fl_Style_List(Fl_Widget_Style * parent, Fl_Widget_Style * style);
  ~Fl_Style_List();
};


#define fl_update_styles(FL_STYLE_CLASS, FL_STYLE_FUNCTION, FL_STYLE_FLAG, FL_STYLE_VALUE) \
{ \
  if (!(flags() & STATIC)) return; \
  if(list()->highlight_ && !(flags() & HIGHLIGHT)){ \
    FL_STYLE_CLASS * h = (FL_STYLE_CLASS *)(list()->highlight_); \
    if(FL_STYLE_FLAG & ~(h->flags()) & ~(list()->parent_mask_)){ \
      h->FL_STYLE_FUNCTION(FL_STYLE_VALUE); \
      h->clear_flag(FL_STYLE_FLAG); \
    } \
  } \
  Fl_Style_List * child_list = list()->children_; \
  while(child_list){ \
    FL_STYLE_CLASS * child = 0; \
    if(flags() & HIGHLIGHT){ \
      child = (FL_STYLE_CLASS *)(child_list->highlight_); \
      if(child && !(FL_STYLE_FLAG & child_list->parent_mask_)) \
       child = 0; \
    }else \
      child = (FL_STYLE_CLASS *)(child_list->normal_); \
    if(child && !(FL_STYLE_FLAG & child->flags())){ \
      child->FL_STYLE_FUNCTION(FL_STYLE_VALUE); \
      child->clear_flag(FL_STYLE_FLAG); \
    } \
    child_list = child_list->next_; \
  } \
}





#endif

//
// End of "$Id: Fl_Style.H 3975 2005-01-27 21:25:00Z rokan $".
//


