//
// "$Id: Fl_Group.h,v 1.6 2002/10/29 00:37:23 easysw Exp $"
//
// Group header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Group_H
#define Fl_Group_H

#ifndef Fl_Widget_H
#include "Fl_Widget.h"
#endif

/**

   The Fl_Group class is the FLTK container widget. It maintains an array of 
   child widgets. These children can themselves be any widget including 
   Fl_Group, nesting groups allows much more control over layout and resize
   behavior. Nested groups are also necessary to group radio buttons together.
   
   By default Fl_Group preserves the positions and sizes of all it's 
   children, they do not move no matter what sizes or other children 
   are added or removed. 

   Setting resizable() will change the layout behavior so that it responds 
   to resizing by moving or resizing the children to fit. 
   See below for details. 

   You may want to use an Fl_Pack or a Fl_Scroll to get other common 
   layout behavior that can respond to changes in the sizes of child 
   widgets. 

   The most-used subclass of Fl_Group is Fl_Window, all the rules about
   resizing apply to windows. Setting resizable() on a window will allow 
   the user to resize it. If you want different behavior 
   (such as from Fl_Pack) for your window you should make the window have 
   that as a single resizable child that fills it. 

   Fl_Menu_ is a subclass and thus all menus and browsers are groups and 
   the items in them are widgets. 

*/
class FL_API Fl_Group : public Fl_Widget {
public:

  /** Returns how many child widgets the group has. */
  int children() const {return children_;}
  /** Returns a child, n >= 0 && n < children(). No range checking is done! */
  Fl_Widget* child(int n) const {return array_[n];}

  void draw();
  void draw_group_box() const;
  void layout();
  int handle(int);

  /**
   begin() sets the current group so you can build the widget tree 
   by just constructing the widgets. begin() is automatically called
   by the constructor for Fl_Group (and thus for Fl_Window as well). 
   begin() is exactly the same as current(this). 

   Don't forget to end() the group or window! 
  */
  void begin() {current_ = this;}
  /**
   end() is exactly the same as current(this->parent()). 
   Any new widgets added to the widget tree will be added 
   to the parent of the group. 
  */
  void end() {current_ = (Fl_Group*)parent();}

  /*@{*/
  /**
   current() returns the currently active group. The Fl_Widget 
   constructor automatically does current()->add(widget) if 
   this is not null. To prevent new widgets from being added to a 
   group, call Fl_Group::current(0). 
  */
  static Fl_Group *current() {return current_;}
  static void current(Fl_Group *g) {current_ = g;}
  /*@}*/

  /*@{*/
  /**
   Searches the children for w, returns the index of w or of a parent
   of w that is a child of this. 
   Returns children() if the widget is NULL or not found. 
  */
  int find(const Fl_Widget* w) const;
  int find(const Fl_Widget& w) const {return find(&w);}
  /*@}*/

  /**
   Creates a new Fl_Group widget using the given position, size, 
   and label string. The default boxtype is FL_NO_BOX.
  */
  Fl_Group(int x,int y,int w,int h, const char * label= 0);
  /**
   The destructor also deletes all the children. This allows a whole 
   tree to be deleted at once, without having to keep a pointer to all
   the children in the user code. A kludge has been done so the Fl_Group 
   and all of it's children can be automatic (local) variables, but you
   must declare the Fl_Group first, so that it is destroyed last. 
  */
  virtual ~Fl_Group();
  /*@{*/
  /**
   The widget is removed from it's current group (if any) and then added 
   to the end of this group. 
  */
  void add(Fl_Widget&);
  void add(Fl_Widget* o) {add(*o);}
  /*@}*/
  /**
   The widget is removed from it's current group (if any) and then inserted
   into this group. It is put at index n (or at the end if n >= children().
   This can also be used to rearrange the windgets inside a group. 
  */
  void insert(Fl_Widget& w, int index);
  /**
   This does insert(w, find(beforethis)). This will append the widget if 
   beforethis is not in the group. 
  */
  void insert(Fl_Widget& o, Fl_Widget* beforethis) {insert(o,find(beforethis));}
  /** Remove the indexed widget from the group. */
  void remove(int index);
  /*@{*/
  /**
   Removes a widget from the group. This does nothing if the widget is not 
   currently a child of this group. 
  */
  void remove(Fl_Widget& o) {remove(find(o));}
  void remove(Fl_Widget* o) {remove(find(*o));}
  /*@}*/
  /** Remove the indexed widget and insert the passed widget in it's place. */
  void replace(int index, Fl_Widget&);
  /**
   Find the old widget and remove it and insert the new one. If the widget 
   is not a child, the new widget is appended to the end of the list.
  */
  void replace(Fl_Widget& old, Fl_Widget& o) {replace(find(old),o);}
  /** Deletes all children from the group and makes it empty. */
  void clear();

  /*@{*/
  /**
   The resizable widget defines the resizing box for the group. When the group 
   is resized it calculates a new size and position for all of its children. 
   Widgets that are horizontally or vertically inside the dimensions of the 
   box are scaled to the new size. Widgets outside the box are moved. 

   In these examples the gray area is the resizable: 

   \image resizebox1.gif
   \image resizebox2.gif

   The resizable may be set to the group itself, in which case all the 
   contents are resized. If the resizable is NULL (the default) then all
   widgets remain a fixed size and distance from the top-left corner. 

   It is possible to achieve any type of resize behavior by using an 
   invisible Fl_Box as the resizable and/or by using a hierarchy of 
   child Fl_Group's. 

  */
  void resizable(Fl_Widget& o) {resizable_ = &o;}
  void resizable(Fl_Widget* o) {resizable_ = o;}
  Fl_Widget* resizable() const {return resizable_;}
  /*@}*/
  /** Adds a widget to the group and makes it the resizable widget. */
  void add_resizable(Fl_Widget& o) {resizable_ = &o; add(o);}
  /**
   If you call this immediately after a resize() it disables any automatic
   repositioning or resizing of the children (it acts as though resizable()
   is null). This is useful when the program wants to resize things. 
  */
  void init_sizes();
  
  /*@{*/
  /**
   The index of the widget that contains the focus. You can initialize 
   this before the group is displayed. Changing it while it is displayed 
   does not work, use widget->take_focus() instead.
  */
  void focus(int i) {focus_ = i;}
  void focus(Fl_Widget* w) {focus(find(w));}
  int focus() const {return focus_;}
  /*@}*/

  static int navigation_key();

  // back compatability function:
  friend FL_FORMS_API void fl_end_group(); // forms emulation function
  void fix_old_positions();

protected:

  void draw_child(Fl_Widget&) const;
  void update_child(Fl_Widget&) const;
  void draw_outside_label(Fl_Widget&) const ;
  int* sizes();

private:

  int children_;
  int focus_;
  Fl_Widget** array_;

  Fl_Widget* resizable_;
  int *sizes_; // remembered initial sizes of children

  static Fl_Group *current_;

};

// This dummy class can be used in constructors to set the parent
// group to any desired value before creating child widget instances.
/**

   Since the constructor of all Fl_Widgets adds the new widget as a child 
   of the "current" Fl_Group, this dummy class allows you to stop this in 
   the constructor list of a class: 
   
   \code

   class MyClass {
     Fl_Group group;
     Fl_Button button_in_group;
     Fl_End end;
     Fl_Button button_outside_group;
     MyClass();
   };

   MyClass::MyClass() :
     group(10,10,100,100),
     button_in_group(20,20,60,30), // constructor puts this in group
     end(),
     button_outside_group(10,120,60,30) // constructor puts this in group's parent
   {}

  \endcode

*/
class FL_API Fl_End {
public:
  /**
   The constructor does Fl_Group::current()->end(). 
   The only purpose of this class is to have the constructor do this.
  */
  Fl_End() {Fl_Group::current()->end();}
  Fl_End(Fl_Group* g) {Fl_Group::current(g);}
};

#endif

//
// End of "$Id: Fl_Group.h,v 1.6 2002/10/29 00:37:23 easysw Exp $".
//
