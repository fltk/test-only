//
// "$Id$"
//
// Group compatibility header file for the Fast Light Tool Kit (FLTK).
//

#ifndef Fltk1_Group_H
#define Fltk1_Group_H

#include <fltk3/Group.h>
#include "Fl_Widget.H"

class FL_EXPORT Fl_Group : public Fl_Widget {
  
#if 0
  Fl_Widget** array_;
  Fl_Widget* savedfocus_;
  Fl_Widget* resizable_;
  int children_;
  int *sizes_; // remembered initial sizes of children

  int navigation(int);
  static Fl_Group *current_;
 
  // unimplemented copy ctor and assignment operator
  Fl_Group(const Fl_Group&);
  Fl_Group& operator=(const Fl_Group&);
#endif
  
protected:
  Fl_Group() { }
  
#if 0
  void draw();
  void draw_child(Fl_Widget& widget) const;
  void draw_children();
  void draw_outside_label(const Fl_Widget& widget) const ;
  void update_child(Fl_Widget& widget) const;
  int  *sizes();
#endif

public:

#if 0
  int handle(int);
#endif
  
  void begin() { ((fltk3::Group*)_p)->begin(); }
  void end() { ((fltk3::Group*)_p)->end(); }
  
#if 0
  static Fl_Group *current();
  static void current(Fl_Group *g);

  /**
    Returns how many child widgets the group has.
  */
  int children() const {return children_;}
  /**
    Returns array()[n].  <i>No range checking is done!</i>
  */
  Fl_Widget* child(int n) const {return array()[n];}
  int find(const Fl_Widget*) const;
  /**
    See int Fl_Group::find(const Fl_Widget *w) const 
  */
  int find(const Fl_Widget& o) const {return find(&o);}
  Fl_Widget* const* array() const;

  void resize(int,int,int,int);
  /**
    Creates a new Fl_Group widget using the given position, size,
    and label string. The default boxtype is FL_NO_BOX.
  */
#endif 
  
  Fl_Group(int x, int y, int w, int h, const char *label = 0) {
    _p = new fltk3::Group(x, y, w, h, label); 
    _p->wrapper(this);
  }
  
  virtual ~Fl_Group() {
  }
  
#if 0
  void add(Fl_Widget&);
  /**
    See void Fl_Group::add(Fl_Widget &w) 
  */
  void add(Fl_Widget* o) {add(*o);}
  void insert(Fl_Widget&, int i);
  /**
    This does insert(w, find(before)).  This will append the
    widget if \p before is not in the group.
  */
  void insert(Fl_Widget& o, Fl_Widget* before) {insert(o,find(before));}
  void remove(Fl_Widget&);
  /**
    Removes the widget \p o from the group.
    \sa void remove(Fl_Widget&)
  */
  void remove(Fl_Widget* o) {remove(*o);}
  void clear();

  /**
    See void Fl_Group::resizable(Fl_Widget *box) 
  */
  void resizable(Fl_Widget& o) {resizable_ = &o;}
  /**
    The resizable widget defines the resizing box for the group. When the
    group is resized it calculates a new size and position for all of its
    children. Widgets that are horizontally or vertically inside the
    dimensions of the box are scaled to the new size. Widgets outside the
    box are moved.

    In these examples the gray area is the resizable:

    \image html resizebox1.gif
    
    \image html resizebox2.gif
    
    \image latex resizebox1.eps "before resize"  width=4cm
    
    \image latex resizebox2.eps "after resize"   width=4cm

    The resizable may be set to the group itself, in which case all the
    contents are resized. This is the default value for Fl_Group,
    although NULL is the default for Fl_Window and Fl_Pack.

    If the resizable is NULL then all widgets remain a fixed size
    and distance from the top-left corner.

    It is possible to achieve any type of resize behavior by using an
    invisible Fl_Box as the resizable and/or by using a hierarchy
    of child Fl_Group's.
  */
  void resizable(Fl_Widget* o) {resizable_ = o;}
  /**
    See void Fl_Group::resizable(Fl_Widget *box) 
  */
  Fl_Widget* resizable() const {return resizable_;}
  /**
    Adds a widget to the group and makes it the resizable widget.
  */
  void add_resizable(Fl_Widget& o) {resizable_ = &o; add(o);}
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

    \see void Fl_Group::clip_children(int c)
  */
  unsigned int clip_children() { return (flags() & CLIP_CHILDREN) != 0; }

  /** Returns an Fl_Group pointer if this widget is an Fl_Group.
  
      \retval NULL if this widget is not derived from Fl_Group.
      \note This method is provided to avoid dynamic_cast.
      \todo More documentation ...
   */
  virtual Fl_Group* as_group() { return this; }

  // back compatibility functions:

  /**
    \deprecated This is for backwards compatibility only. You should use
    \e W->%take_focus() instead.
    \sa Fl_Widget::take_focus();
  */
  void focus(Fl_Widget* W) {W->take_focus();}

  /** This is for forms compatibility only */
  Fl_Widget* & _ddfdesign_kludge() {return resizable_;}

  /** This is for forms compatibility only */
  void forms_end();
#endif
};

#if 0
// dummy class used to end child groups in constructors for complex
// subclasses of Fl_Group:
/**
  This is a dummy class that allows you to end a Fl_Group in a constructor list of a
  class:
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
   button_in_group(20,20,60,30),
   end(),
   button_outside_group(10,120,60,30)
  {} 
  \endcode
*/
class FL_EXPORT Fl_End {
public:
  /** All it does is calling Fl_Group::current()->end() */
  Fl_End() {Fl_Group::current()->end();}
};
#endif

#endif

//
// End of "$Id$".
//
