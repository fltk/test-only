//
// "$Id: Fl_Group.H,v 1.6.2.4.2.6.2.7 2004/03/18 08:00:34 matthiaswm Exp $"
//
// Group header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Group_H
#define Fl_Group_H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif

/** The Fl_Group class is the FLTK container widget. It maintains an array of child widgets.
 * These children can themselves be any widget including Fl_Group. The most important subclass
 * of Fl_Group is Fl_Window, however groups can also be used to control radio buttons or to
 * enforce resize behavior.
 */
class FL_EXPORT Fl_Group : public Fl_Widget {

  Fl_Widget** array_;
  Fl_Widget* savedfocus_;
  Fl_Widget* resizable_;
  int children_;
  int *sizes_; // remembered initial sizes of children

  int navigation(int);
  static Fl_Group *current_;

protected:

  void draw();
  void draw_child(Fl_Widget&) const;
  void draw_children();
  void draw_outside_label(const Fl_Widget&) const ;
  void update_child(Fl_Widget&) const;
  int* sizes();

public:

  int handle(int);
    /** begin() sets the current group so you can build the widget tree by just constructing
     * the widgets. begin() is automatically called by the constructor for Fl_Group (and thus
     * for Fl_Window as well). begin() is exactly the same as current(this).
     *
     * <i>Don't forget to end() the group or window!</i> */
  void begin();
    /** end() is exactly the same as <tt>current(this->parent())</tt>. Any new widgets added 
     * to the widget tree will be added to the parent of the group. */
  void end();
    /** current() returns the currently active group. */
  static Fl_Group *current();
    /** Newly created widgets will be added automatically to the end of the current group.
     * The Fl_Widget constructor automatically does current()->add(widget) if \c this is not
     * null. To prevent new widgets from being added to a group, call Fl_Group::current(0). */
  static void current(Fl_Group *g);

    /** Returns how many child widgets the group has. */
  int children() const {return children_;}
    /** Returns array()[n]. <i>No range checking is done!</i> */
  Fl_Widget* child(int n) const {return array()[n];}
    /** Searches the child array for the widget and returns the index. Returns children() 
     * if the widget is \c NULL or not found. */
  int find(const Fl_Widget*) const;
  int find(const Fl_Widget& o) const {return find(&o);}
    /** Returns a pointer to the array of children. This pointer is only valid until the
     * next time a child is added or removed. \see child(), children() */
  Fl_Widget* const* array() const;

  void resize(int x,int y,int width,int height);
    /** Creates a new Fl_Group widget using the given position, size, and label string. 
     * The default boxtype is \c FL_NO_BOX. */
  Fl_Group(int x,int y,int width,int height, const char *label = 0);
    /** The destructor also deletes all the children. This allows a whole tree to be 
     * deleted at once, without having to keep a pointer to all the children in the 
     * user code. A kludge has been done so the Fl_Group and all of it's children can be 
     * automatic (local) variables, but you must declare the Fl_Group first, so that it is
     * destroyed last. */
  virtual ~Fl_Group();
    /** The widget is removed from it's current group (if any) and then added to the end
     * of this group. */
  void add(Fl_Widget&);
  void add(Fl_Widget* o) {add(*o);}
    /** The widget is removed from it's current group (if any) and then inserted into 
     * this group. It is put at index n (or at the end if n >= children(). This can also be
     * used to rearrange the widgets inside a group. */
  void insert(Fl_Widget&, int i);
    /** This does <tt>insert(w, find(beforethis))</tt>. This will append the widget if 
     * before thisis not in the group. */
  void insert(Fl_Widget& o, Fl_Widget* before) {insert(o,find(before));}
    /** Removes a widget from the group. This does nothing if the widget is not currently 
     * a child of this group. */
  void remove(Fl_Widget&);
  void remove(Fl_Widget* o) {remove(*o);}
    /** The clear() method deletes all child widgets recursively. */
  void clear();

    /** The resizable widget defines the resizing box for the group. When the group is resized 
     * it calculates a new size and position for all of its children. Widgets that are
     * horizontally or vertically inside the dimensions of the box are scaled to the new size. 
     * Widgets outside the box are moved.
     * 
     * In these examples the gray area is the resizable: 
     *   \image html resizebox1.gif 
     *
     *   \image html resizebox2.gif
     *
     * The resizable may be set to the group itself (this is the default value for an Fl_Group, 
     * although \c NULL is the default for an Fl_Window), in which case all the contents are 
     * resized. If the resizable is \c NULL then all widgets remain a fixed size and distance 
     * from the top-left corner.
     *
     * It is possible to achieve any type of resize behavior by using an invisible Fl_Box as
     * the resizable and/or by using a hierarchy of child Fl_Group's. */
  void resizable(Fl_Widget& o) {resizable_ = &o;}
  void resizable(Fl_Widget* o) {resizable_ = o;}
  Fl_Widget* resizable() const {return resizable_;}
    /** Adds a widget to the group and makes it the resizable widget. */
  void add_resizable(Fl_Widget& o) {resizable_ = &o; add(o);}
    /** The Fl_Group widget keeps track of the original widget sizes and positions when
     * resizing occurs so that if you resize a window back to its original size the widgets 
     * will be in the correct places. If you rearrange the widgets in your group, call this 
     * method to register the new arrangement with the Fl_Group that contains them. */
  void init_sizes();

  // back compatability function:
  void focus(Fl_Widget* o) {o->take_focus();}
  Fl_Widget* & _ddfdesign_kludge() {return resizable_;}
  void forms_end();
};

/** This is a dummy class that allows you to end a Fl_Group in a 
 * constructor list of a class:
 * \code
    class MyClass {
      Fl_Group group;
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
    { }
 * \endcode
 */
class FL_EXPORT Fl_End {
public:
    /** The constructor does <tt>Fl_Group::current()->end()</tt>. */
  Fl_End() {Fl_Group::current()->end();}
};

#endif

//
// End of "$Id: Fl_Group.H,v 1.6.2.4.2.6.2.7 2004/03/18 08:00:34 matthiaswm Exp $".
//
