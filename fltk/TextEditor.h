//
// "$Id: TextEditor.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $"
//
// Text editor. Useful for composing email.
//
// Copyright Mark Edel.  Permission to distribute under the LGPL for
// the FLTK library granted by Mark Edel.
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

#ifndef fltk_TextEditor_h
#define fltk_TextEditor_h

#include "TextDisplay.h"

namespace fltk {

class FL_API TextEditor : public TextDisplay {
  public:
    typedef int (*KeyFunc)(int key, TextEditor* editor);

    struct FL_API KeyBinding {
      int          key;
      int          state;
      KeyFunc     function;
      KeyBinding* next;
    };

    TextEditor(int X, int Y, int W, int H, const char* l = 0);
    ~TextEditor() { remove_all_key_bindings(); }
    static NamedStyle* default_style;
    virtual int handle(int e);
    void insert_mode(int b) { insert_mode_ = b; }
    int insert_mode() { return insert_mode_; }

    void add_key_binding(int key, int state, KeyFunc f, KeyBinding** list);
    void add_key_binding(int key, int state, KeyFunc f)
      { add_key_binding(key, state, f, &key_bindings); }
    void remove_key_binding(int key, int state, KeyBinding** list);
    void remove_key_binding(int key, int state)
      { remove_key_binding(key, state, &key_bindings); }
    void remove_all_key_bindings(KeyBinding** list);
    void remove_all_key_bindings() { remove_all_key_bindings(&key_bindings); }
    void add_default_key_bindings(KeyBinding** list);
    KeyFunc bound_key_function(int key, int state, KeyBinding* list);
    KeyFunc bound_key_function(int key, int state)
      { return bound_key_function(key, state, key_bindings); }
    void default_key_function(KeyFunc f) { default_key_function_ = f; }

    // functions for the built in default bindings
    static int kf_default(int c, TextEditor* e);
    static int kf_ignore(int c, TextEditor* e);
    static int kf_backspace(int c, TextEditor* e);
    static int kf_enter(int c, TextEditor* e);
    static int kf_move(int c, TextEditor* e);
    static int kf_shift_move(int c, TextEditor* e);
    static int kf_ctrl_move(int c, TextEditor* e);
    static int kf_c_s_move(int c, TextEditor* e);
    static int kf_home(int, TextEditor* e);
    static int kf_end(int c, TextEditor* e);
    static int kf_left(int c, TextEditor* e);
    static int kf_up(int c, TextEditor* e);
    static int kf_right(int c, TextEditor* e);
    static int kf_down(int c, TextEditor* e);
    static int kf_page_up(int c, TextEditor* e);
    static int kf_page_down(int c, TextEditor* e);
    static int kf_insert(int c, TextEditor* e);
    static int kf_delete(int c, TextEditor* e);
    static int kf_copy(int c, TextEditor* e);
    static int kf_cut(int c, TextEditor* e);
    static int kf_paste(int c, TextEditor* e);
    static int kf_select_all(int c, TextEditor* e);

  protected:
    int handle_key();
    int insert_mode_;
    KeyBinding* key_bindings;
    static KeyBinding* global_key_bindings;
    KeyFunc default_key_function_;
};

}

#endif

//
// End of "$Id: TextEditor.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $".
//

