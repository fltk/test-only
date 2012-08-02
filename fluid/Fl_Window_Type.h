//
// "$Id$"
//
// Window type header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#ifndef FLUID3_WINDOW_TYPE_H
#define FLUID3_WINDOW_TYPE_H

#include "Fl_Widget_Type.h"
#include <fltk3/DoubleWindow.h>


extern fltk3::MenuItem window_type_menu[];
extern int use_FL_COMMAND;
extern class Fl_Widget_Class_Type Fl_Widget_Class_type;
extern class Fl_Widget_Class_Type *current_widget_class;


class Fl_Window_Type : public Fl_Widget_Type {
  
  friend class Overlay_Window;
  
protected:
  
  enum {LEFT=1,RIGHT=2,BOTTOM=4,TOP=8,DRAG=16,BOX=32};
  
  int pCurrentMouseX, pCurrentMouseY;		// mouse position during dragging
  int pInitialMouseX, pInitialMouseY; // initial position of selection box
  fltk3::Rectangle pSelectionBox; // bounding box of selection before snapping
  int pDeltaX, pDeltaY;
  int pDragMode;		// which parts of bbox are being moved
  int pNumSelected;	// number of children selected
  int pRecalculateSelectionBox;		// set by fix_overlay()
  
public:
  
  uchar pIsModal, pIsNonModal;
  int pMinW, pMinH, pMaxW, pMaxH;
  
protected:
  
  fltk3::MenuItem* subtypes() {return window_type_menu;}
  void draw_overlay();
  void handle_drag_event();
  void newposition(Fl_Widget_Type *,int &x,int &y,int &w,int &h);
  Fl_Widget_Type *find_innermost_for_event();
  int handle(int);
  virtual void setlabel(const char *);
  void write_code1();
  void write_code2();
  Fl_Widget_Type *_make() {return 0;} // we don't call this
  fltk3::Widget *widget(int,int,int,int) {return 0;}
  void move_all_children();
  int pixmapID() { return 1; }
  
public:
  
  Fl_Window_Type() 
  : pCurrentMouseX(0), pCurrentMouseY(0),
  pInitialMouseX(0), pInitialMouseY(0),
  pSelectionBox(0, 0, 0, 0),
  pDeltaX(0), pDeltaY(0),
  pDragMode(0),
  pNumSelected(0),
  pRecalculateSelectionBox(0),
  pIsModal(0), pIsNonModal(0),
  pMinW(0), pMinH(0), pMaxW(0), pMaxH(0)
  { }
  
  Fl_Type *make();
  virtual const char *type_name() {return "fltk3::Window";}
  virtual const char *alt_type_name() {return "Fl_Window";}
  
  void open();
  
  void fix_overlay();			// Update the bounding box, etc
  uchar *read_image(int &ww, int &hh);	// Read an image of the window
  
  virtual void write_properties();
  virtual char read_property(const char *);
  
  void add_child(Fl_Type*, Fl_Type*);
  void move_child(Fl_Type*, Fl_Type*);
  void remove_child(Fl_Type*);
  
  int is_parent() const {return 1;}
  int is_group() const {return 1;}
  int is_window() const {return 1;}
  
  fltk3::Widget *enter_live_mode(int top=0);
  void leave_live_mode();
  void copy_properties();
};


class Fl_Widget_Class_Type : private Fl_Window_Type {
public:
  Fl_Widget_Class_Type() {
    write_public_state = 0;
    wc_relative = 0;
  }
  // state variables for output:
  char write_public_state; // true when public: has been printed
  char wc_relative; // if true, reposition all child widgets in an fltk3::Group
  
  virtual void write_properties();
  virtual char read_property(const char *);
  
  void write_code1();
  void write_code2();
  Fl_Type *make();
  virtual const char *type_name() {return "widget_class";}
  virtual const char *alt_type_name() {return "widget_class";}
  int pixmapID() { return 48; }
  int is_parent() const {return 1;}
  int is_code_block() const {return 1;}
  int is_decl_block() const {return 1;}
  int is_class() const {return 1;}
};


#endif

//
// End of "$Id$".
//
