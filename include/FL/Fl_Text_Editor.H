//
// "$Id$"
//
// Header file for Fl_Text_Editor class.
//
// Copyright 2001-2012 by Bill Spitzak and others.
// Original code Copyright Mark Edel.  Permission to distribute under
// the LGPL for the FLTK library granted by Mark Edel.
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

#ifndef FL_TEXT_EDITOR_H
#define FL_TEXT_EDITOR_H

#include <fltk3/TextEditor.h>
#include "Fl_Text_Display.H"

#define FL_TEXT_EDITOR_ANY_STATE  (-1L)


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Text_Editor, TextEditor)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Text_Editor, TextEditor)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Text_Editor : public Fl_Text_Display {
  
public:

  typedef int (*Key_Func)(int key, Fl_Text_Editor* editor);
 
  struct Key_Binding {
    int          key;		///< the key pressed
    int          state;	///< the state of key modifiers
    Key_Func     function;	///< associated function
    Key_Binding* next;	///< next key binding in the list
  };
  
  FLTK3_WIDGET_VCALLS(Fl_Text_Editor, TextEditor)

  Fl_Text_Editor() { /* empty */ }
  
  Fl_Text_Editor(int X, int Y, int W, int H, const char* l = 0) {
    _p = new fltk3::TextEditor_I(_ctor_dx(X), _ctor_dy(Y), W, H, l);
    _p->wrapper(this);
  }

  void insert_mode(int b) {
    ((fltk3::TextEditor*)_p)->insert_mode(b);
  }

  int insert_mode() { 
    return ((fltk3::TextEditor*)_p)->insert_mode();
  }
  
  void add_key_binding(int key, int state, Key_Func f, Key_Binding** list) {
    ((fltk3::TextEditor*)_p)->add_key_binding(key, state, (fltk3::TextEditor::KeyFunc)f, (fltk3::TextEditor::KeyBinding**)list);
  }

  void add_key_binding(int key, int state, Key_Func f) {
    ((fltk3::TextEditor*)_p)->add_key_binding(key, state, (fltk3::TextEditor::KeyFunc)f);
  }

  void remove_key_binding(int key, int state, Key_Binding** list) {
    ((fltk3::TextEditor*)_p)->remove_key_binding(key, state, (fltk3::TextEditor::KeyBinding**)list);
  }

  void remove_key_binding(int key, int state) {
    ((fltk3::TextEditor*)_p)->remove_key_binding(key, state);
  }

  void remove_all_key_bindings(Key_Binding** list) {
    ((fltk3::TextEditor*)_p)->remove_all_key_bindings((fltk3::TextEditor::KeyBinding**)list);
  }

  void remove_all_key_bindings() {
    ((fltk3::TextEditor*)_p)->remove_all_key_bindings();
  }

  void add_default_key_bindings(Key_Binding** list) {
    ((fltk3::TextEditor*)_p)->add_default_key_bindings((fltk3::TextEditor::KeyBinding**)list);
  }
  
  Key_Func bound_key_function(int key, int state, Key_Binding* list) {
    return (Key_Func)((fltk3::TextEditor*)_p)->bound_key_function(key, state, (fltk3::TextEditor::KeyBinding*)list);
  }
  
  Key_Func bound_key_function(int key, int state) {
    return (Key_Func)((fltk3::TextEditor*)_p)->bound_key_function(key, state);
  }

  void default_key_function(Key_Func f) {
    ((fltk3::TextEditor*)_p)->default_key_function((fltk3::TextEditor::KeyFunc)f);
  }
  
  static int kf_default(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_default(c, (fltk3::TextEditor*)fltk3::_1to3_widget(e));
    // FIXME: all this key function emulation is not thought through!
  }
  
  static int kf_ignore(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_ignore(c, (fltk3::TextEditor*)e);
  }
  
  static int kf_backspace(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_backspace(c, (fltk3::TextEditor*)e);
  }

  static int kf_enter(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_enter(c, (fltk3::TextEditor*)e);
  }

  static int kf_move(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_move(c, (fltk3::TextEditor*)e);
  }

  static int kf_shift_move(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_shift_move(c, (fltk3::TextEditor*)e);
  }

  static int kf_ctrl_move(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_ctrl_move(c, (fltk3::TextEditor*)e);
  }

  static int kf_c_s_move(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_c_s_move(c, (fltk3::TextEditor*)e);
  }

  static int kf_meta_move(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_meta_move(c, (fltk3::TextEditor*)e);
  }

  static int kf_m_s_move(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_m_s_move(c, (fltk3::TextEditor*)e);
  }

  static int kf_home(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_home(c, (fltk3::TextEditor*)e);
  }

  static int kf_end(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_end(c, (fltk3::TextEditor*)e);
  }

  static int kf_left(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_left(c, (fltk3::TextEditor*)e);
  }

  static int kf_up(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_up(c, (fltk3::TextEditor*)e);
  }

  static int kf_right(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_right(c, (fltk3::TextEditor*)e);
  }

  static int kf_down(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_down(c, (fltk3::TextEditor*)e);
  }

  static int kf_page_up(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_page_up(c, (fltk3::TextEditor*)e);
  }

  static int kf_page_down(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_page_down(c, (fltk3::TextEditor*)e);
  }

  static int kf_insert(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_insert(c, (fltk3::TextEditor*)e);
  }

  static int kf_delete(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_delete(c, (fltk3::TextEditor*)e);
  }

  static int kf_copy(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_copy(c, (fltk3::TextEditor*)e);
  }

  static int kf_cut(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_cut(c, (fltk3::TextEditor*)e);
  }

  static int kf_paste(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_paste(c, (fltk3::TextEditor*)e);
  }

  static int kf_select_all(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_select_all(c, (fltk3::TextEditor*)e);
  }

  static int kf_undo(int c, Fl_Text_Editor* e) {
    return fltk3::TextEditor::kf_undo(c, (fltk3::TextEditor*)e);
  }
  
protected:
  
  int handle_key() {
    return ((fltk3::TextEditor_I*)_p)->handle_key();
  }
  
  void maybe_do_callback() {
    ((fltk3::TextEditor_I*)_p)->maybe_do_callback();
  }
  
};


#endif

//
// End of "$Id$".
//

