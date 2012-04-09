//
// "$Id$"
//
// Header file for fltk3::TextEditor class.
//
// Copyright 2001-2010 by Bill Spitzak and others.
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

/* \file
 fltk3::TextEditor widget . */


#ifndef FLtk3_TEXT_EDITOR_H
#define FLtk3_TEXT_EDITOR_H

#include "TextDisplay.h"


namespace fltk3 {

  // key will match in any state
  const unsigned int TEXT_EDITOR_ANY_STATE = (unsigned)(-1);
  
  /**
   This is the FLTK text editor widget. It allows the user to
   edit multiple lines of text and supports highlighting and
   scrolling. The buffer that is displayed in the widget is managed
   by the fltk3::TextBuffer
   class.
   */
  class FLTK3_EXPORT TextEditor : public fltk3::TextDisplay {

  public:
    /** Key function binding callback type */
    typedef int (*KeyFunc)(unsigned int key, fltk3::TextEditor* editor);
    
    /** Simple linked list associating a key/state to a function */
    struct KeyBinding {
      unsigned int key;		///< the key pressed
      unsigned int state;	///< the state of key modifiers
      KeyFunc     function;	///< associated function
      KeyBinding* next;	///< next key binding in the list
    };
    
    TextEditor(int X, int Y, int W, int H, const char* l = 0);
    ~TextEditor() { remove_all_key_bindings(); }
    virtual int readonly() { return 0; }
    virtual int handle(int e);
    /**
     Sets the current insert mode; if non-zero, new text
     is inserted before the current cursor position. Otherwise, new
     text replaces text at the current cursor position.
     */
    void insert_mode(int b) { insert_mode_ = b; }
    /**
     Gets the current insert mode; if non-zero, new text
     is inserted before the current cursor position. Otherwise, new
     text replaces text at the current cursor position.
     */
    int insert_mode() { return insert_mode_; }
    
    void add_key_binding(unsigned key, unsigned state, KeyFunc f, KeyBinding** list);
    /** Adds a key of state "state" with the function "function" */
    void add_key_binding(unsigned key, unsigned state, KeyFunc f)
    { add_key_binding(key, state, f, &key_bindings); }
    void remove_key_binding(unsigned key, unsigned state, KeyBinding** list);
    /** Removes the key binding associated with the key "key" of state "state". */
    void remove_key_binding(unsigned key, unsigned state)
    { remove_key_binding(key, state, &key_bindings); }
    void remove_all_key_bindings(KeyBinding** list);
    /** Removes all of the key bindings associated with the text editor or list. */
    void remove_all_key_bindings() { remove_all_key_bindings(&key_bindings); }
    void add_default_key_bindings(KeyBinding** list);
    KeyFunc bound_key_function(unsigned int key, unsigned int state, KeyBinding* list);
    /**  Returns the function associated with a key binding. */
    KeyFunc bound_key_function(unsigned int key, unsigned int state)
    { return bound_key_function(key, state, key_bindings); }
    /**  Sets the default key function for unassigned keys. */
    void default_key_function(KeyFunc f) { default_key_function_ = f; }
    
    // functions for the built in default bindings
    static int kf_default(unsigned int c, fltk3::TextEditor* e);
    static int kf_ignore(unsigned int c, fltk3::TextEditor* e);
    static int kf_backspace(unsigned int c, fltk3::TextEditor* e);
    static int kf_enter(unsigned int c, fltk3::TextEditor* e);
    static int kf_move(unsigned int c, fltk3::TextEditor* e);
    static int kf_shift_move(unsigned int c, fltk3::TextEditor* e);
    static int kf_ctrl_move(unsigned int c, fltk3::TextEditor* e);
    static int kf_c_s_move(unsigned int c, fltk3::TextEditor* e);
    static int kf_meta_move(unsigned int c, fltk3::TextEditor* e);
    static int kf_m_s_move(unsigned int c, fltk3::TextEditor* e);
    static int kf_home(unsigned int, fltk3::TextEditor* e);
    static int kf_end(unsigned int c, fltk3::TextEditor* e);
    static int kf_left(unsigned int c, fltk3::TextEditor* e);
    static int kf_up(unsigned int c, fltk3::TextEditor* e);
    static int kf_right(unsigned int c, fltk3::TextEditor* e);
    static int kf_down(unsigned int c, fltk3::TextEditor* e);
    static int kf_page_up(unsigned int c, fltk3::TextEditor* e);
    static int kf_page_down(unsigned int c, fltk3::TextEditor* e);
    static int kf_insert(unsigned int c, fltk3::TextEditor* e);
    static int kf_delete(unsigned int c, fltk3::TextEditor* e);
    static int kf_copy(unsigned int c, fltk3::TextEditor* e);
    static int kf_cut(unsigned int c, fltk3::TextEditor* e);
    static int kf_paste(unsigned int c, fltk3::TextEditor* e);
    static int kf_select_all(unsigned int c, fltk3::TextEditor* e);
    static int kf_undo(unsigned int c, fltk3::TextEditor* e);
    
  protected:
    int handle_key();
    void maybe_do_callback();
    
#ifndef FLTK3_DOXYGEN
    int insert_mode_;
    KeyBinding* key_bindings;
    static KeyBinding* global_key_bindings;
    KeyFunc default_key_function_;
#endif
  };
  
} // namespace

#endif

//
// End of "$Id$".
//

