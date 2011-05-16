//
// "$Id: Group.h 8157 2011-01-01 14:01:53Z AlbrechtS $"
//
// Group header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::Group, Fl_End classes . */

#ifndef FLTK3_GROUP_H
#define FLTK3_GROUP_H

#include "Widget.h"

namespace fltk3 {
  
  /**
   The fltk3::Group class is the FLTK container widget. It maintains
   an array of child widgets. These children can themselves be any widget
   including fltk3::Group. The most important subclass of fltk3::Group
   is Fl_Window, however groups can also be used to control radio buttons 
   or to enforce resize behavior.
   */
  class FL_EXPORT Group : public Widget {
    
    fltk3::Widget** array_;
    fltk3::Widget* savedfocus_;
    fltk3::Widget* resizable_;
    int children_;
    int *sizes_; // remembered initial sizes of children
    
    int navigation(int);
    static Group *current_;
    
    // unimplemented copy ctor and assignment operator
    Group(const Group&);
    Group& operator=(const Group&);
    
  protected:
    void draw();
    void draw_child(fltk3::Widget& widget) const;
    void draw_children();
    void draw_outside_label(const fltk3::Widget& widget) const ;
    void update_child(fltk3::Widget& widget) const;
    int  *sizes();
    
  public:
    
    int handle(int);
    void begin();
    void end();
    static Group *current();
    static void current(Group *g);
    
    /**
     Returns how many child widgets the group has.
     */
    int children() const {return children_;}
    /**
     Returns array()[n].  <i>No range checking is done!</i>
     */
    fltk3::Widget* child(int n) const {return array()[n];}
    int find(const fltk3::Widget*) const;
    /**
     See int fltk3::Group::find(const fltk3::Widget *w) const 
     */
    int find(const fltk3::Widget& o) const {return find(&o);}
    fltk3::Widget* const* array() const;
    
    void resize(int,int,int,int);
    /**
     Creates a new fltk3::Group widget using the given position, size,
     and label string. The default boxtype is fltk3::NO_BOX.
     */
    Group(int,int,int,int, const char * = 0);
    virtual ~Group();
    void add(fltk3::Widget&);
    /**
     See void fltk3::Group::add(fltk3::Widget &w) 
     */
    void add(fltk3::Widget* o) {add(*o);}
    void insert(fltk3::Widget&, int i);
    /**
     This does insert(w, find(before)).  This will append the
     widget if \p before is not in the group.
     */
    void insert(fltk3::Widget& o, fltk3::Widget* before) {insert(o,find(before));}
    void remove(int index);
    void remove(fltk3::Widget&);
    /**
     Removes the widget \p o from the group.
     \sa void remove(fltk3::Widget&)
     */
    void remove(fltk3::Widget* o) {remove(*o);}
    void clear();
    
    /**
     See void fltk3::Group::resizable(fltk3::Widget *box) 
     */
    void resizable(fltk3::Widget& o) {resizable_ = &o;}
    /**
     The resizable widget defines the resizing box for the group. When the
     group is resized it calculates a new size and position for all of its
     children. Widgets that are horizontally or vertically inside the
     dimensions of the box are scaled to the new size. Widgets outside the
     box are moved.
     
     In these examples the gray area is the resizable:
     
     \image html resizebox1.png
     
     \image html resizebox2.png
     
     \image latex resizebox1.png "before resize"  width=4cm
     
     \image latex resizebox2.png "after resize"   width=4cm
     
     The resizable may be set to the group itself, in which case all the
     contents are resized. This is the default value for fltk3::Group,
     although NULL is the default for Fl_Window and Fl_Pack.
     
     If the resizable is NULL then all widgets remain a fixed size
     and distance from the top-left corner.
     
     It is possible to achieve any type of resize behavior by using an
     invisible fltk3::Box as the resizable and/or by using a hierarchy
     of child fltk3::Group's.
     */
    void resizable(fltk3::Widget* o) {resizable_ = o;}
    /**
     See void fltk3::Group::resizable(fltk3::Widget *box) 
     */
    fltk3::Widget* resizable() const {return resizable_;}
    /**
     Adds a widget to the group and makes it the resizable widget.
     */
    void add_resizable(fltk3::Widget& o) {resizable_ = &o; add(o);}
    void init_sizes();
    
    /**
     Controls whether the group widget clips the drawing of
     child widgets to its bounding box.
     
     Set \p c to 1 if you want to clip the child widgets to the
     bounding box.
     
     The default is to not clip (0) the drawing of child widgets.
     */
    void clip_children(int c) { if (c) set_flag(CLIP_CHILDREN); else clear_flag(CLIP_CHILDREN); }
    /**
     Returns the current clipping mode.
     
     \return true, if clipping is enabled, false otherwise.
     
     \see void fltk3::Group::clip_children(int c)
     */
    unsigned int clip_children() { return (flags() & CLIP_CHILDREN) != 0; }
    
    // Note: Doxygen docs in Widget.h to avoid redundancy.
    virtual Group* as_group() { return this; }
    
    // back compatibility functions:
    
    /**
     \deprecated This is for backwards compatibility only. You should use
     \e W->%take_focus() instead.
     \sa fltk3::Widget::take_focus();
     */
    void focus(fltk3::Widget* W) {W->take_focus();}
    
    /** This is for forms compatibility only */
    fltk3::Widget* & _ddfdesign_kludge() {return resizable_;}
    
    /** This is for forms compatibility only */
    void forms_end();
  };
  
  // dummy class used to end child groups in constructors for complex
  // subclasses of fltk3::Group:
  /**
   This is a dummy class that allows you to end a fltk3::Group in a constructor list of a
   class:
   \code 
   class MyClass {
   fltk3::Group group;
   Fl_Button button_in_group;
   Fl_End end;
   Fl_Button button_outside_group;
   MyClass();
   };
   MyClass::MyClass() :
   group(10,10,100,100),
   button_in_group(20,20,60,30),
   end(),
   button_outside_group(10,120,60,30)
   {} 
   \endcode
   */
  class FL_EXPORT Fl_End {
  public:
    /** All it does is calling fltk3::Group::current()->end() */
    Fl_End() {Group::current()->end();}
  };
  
}

#endif

//
// End of "$Id: Group.h 8157 2011-01-01 14:01:53Z AlbrechtS $".
//
