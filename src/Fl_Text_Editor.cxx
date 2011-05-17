//
// "$Id$"
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

#include <stdio.h>
#include <stdlib.h>
#include "flstring.h"
#include <ctype.h>
#include <fltk3/run.h>
#include <fltk3/Window.h>
#include <fltk3/TextEditor.h>
#include <fltk3/ask.h>


/* Keyboard Control Matrix

key\modifier   plain  Ctrl   Alt  Meta  
  left          1/1  13/9   0/13  0/9  
  right         2/2  14/10  0/14  0/10  
  up            3/19 21/7   0/15  0/17 
  down          4/20 22/8   0/16  0/18
  home          9/5  17/0   0/0   0/0
  end          10/6  18/0   0/0   0/0
  page up      11/7  23/0   0/11  0/0
  page down    12/8  24/0   0/12  0/0
    (FLTK action / OS X action)
    (adding the shift key extends the selection, all other combinations are no-op)

  0: no-op
  1: move cursor to the left, at line beginning wrap to end of prev line, at doc start no-op
  2: move cursor to the right, at line end move to beginning of the next line, at doc end no-op
  3: move cursor up, at doc top no-op
  4: move cursor down, at doc bottom no-op
  5: scroll display to top of text (cursor unchanged)
  6: scroll display to end of text (cursor unchanged)
  7: scroll text down one page (cursor unchanged)
  8: scroll text up one page (cursor unchanged)
  9: move cursor to beginning of line
 10: move cursor to end of line
 11: move cursor up one page and scroll down
 12: move cursor down one page and scroll up
 13: move to the beginning of the word or the previous word
 14: move to the end of the word or the next word
 15: if start of line: start of prev line, else start of this line
 16: if end of line: end of next line, else end of this line
 17: move cursor to the beginning of the document
 18: move cursor to the end of the document
 19: move cursor up, at doc top: home, at doc start: no-op)
 20: move cursor down, at doc bot: end, at doc end: no-op)
 21: scroll text down one line (cursor unchanged)
 22: scroll text up one line (cursor unchanged)
 23: move cursor to the beginning of the top of the screen
 24: move cursor to the beginning of the bottom of the window
*/

/**  The constructor creates a new text editor widget.*/
Fl_Text_Editor::Fl_Text_Editor(int X, int Y, int W, int H,  const char* l)
    : Fl_Text_Display(X, Y, W, H, l) {
  mCursorOn = 1;
  insert_mode_ = 1;
  key_bindings = 0;

  // handle the default key bindings
  add_default_key_bindings(&key_bindings);

  // handle everything else
  default_key_function(kf_default);
}

#ifndef FL_DOXYGEN
Fl_Text_Editor::Key_Binding* Fl_Text_Editor::global_key_bindings = 0;
#endif

// These are the default key bindings every widget should start with
static struct {
  int key;
  int state;
  Fl_Text_Editor::Key_Func func;
} default_key_bindings[] = {
  { fltk3::EscapeKey,    FL_TEXT_EDITOR_ANY_STATE, Fl_Text_Editor::kf_ignore     },
  { fltk3::EnterKey,     FL_TEXT_EDITOR_ANY_STATE, Fl_Text_Editor::kf_enter      },
  { fltk3::KPEnterKey,  FL_TEXT_EDITOR_ANY_STATE, Fl_Text_Editor::kf_enter      },
  { fltk3::BackSpaceKey, FL_TEXT_EDITOR_ANY_STATE, Fl_Text_Editor::kf_backspace  },
  { fltk3::InsertKey,    FL_TEXT_EDITOR_ANY_STATE, Fl_Text_Editor::kf_insert     },
  { fltk3::DeleteKey,    FL_TEXT_EDITOR_ANY_STATE, Fl_Text_Editor::kf_delete     },
  { fltk3::HomeKey,      0,                        Fl_Text_Editor::kf_move       },
  { fltk3::EndKey,       0,                        Fl_Text_Editor::kf_move       },
  { fltk3::LeftKey,      0,                        Fl_Text_Editor::kf_move       },
  { fltk3::UpKey,        0,                        Fl_Text_Editor::kf_move       },
  { fltk3::RightKey,     0,                        Fl_Text_Editor::kf_move       },
  { fltk3::DownKey,      0,                        Fl_Text_Editor::kf_move       },
  { fltk3::PageUpKey,   0,                        Fl_Text_Editor::kf_move       },
  { fltk3::PageDownKey, 0,                        Fl_Text_Editor::kf_move       },
  { fltk3::HomeKey,      fltk3::SHIFT,                 Fl_Text_Editor::kf_shift_move },
  { fltk3::EndKey,       fltk3::SHIFT,                 Fl_Text_Editor::kf_shift_move },
  { fltk3::LeftKey,      fltk3::SHIFT,                 Fl_Text_Editor::kf_shift_move },
  { fltk3::UpKey,        fltk3::SHIFT,                 Fl_Text_Editor::kf_shift_move },
  { fltk3::RightKey,     fltk3::SHIFT,                 Fl_Text_Editor::kf_shift_move },
  { fltk3::DownKey,      fltk3::SHIFT,                 Fl_Text_Editor::kf_shift_move },
  { fltk3::PageUpKey,   fltk3::SHIFT,                 Fl_Text_Editor::kf_shift_move },
  { fltk3::PageDownKey, fltk3::SHIFT,                 Fl_Text_Editor::kf_shift_move },
  { fltk3::HomeKey,      fltk3::CTRL,                  Fl_Text_Editor::kf_ctrl_move  },
  { fltk3::EndKey,       fltk3::CTRL,                  Fl_Text_Editor::kf_ctrl_move  },
  { fltk3::LeftKey,      fltk3::CTRL,                  Fl_Text_Editor::kf_ctrl_move  },
  { fltk3::UpKey,        fltk3::CTRL,                  Fl_Text_Editor::kf_ctrl_move  },
  { fltk3::RightKey,     fltk3::CTRL,                  Fl_Text_Editor::kf_ctrl_move  },
  { fltk3::DownKey,      fltk3::CTRL,                  Fl_Text_Editor::kf_ctrl_move  },
  { fltk3::PageUpKey,   fltk3::CTRL,                  Fl_Text_Editor::kf_ctrl_move  },
  { fltk3::PageDownKey, fltk3::CTRL,                  Fl_Text_Editor::kf_ctrl_move  },
  { fltk3::HomeKey,      fltk3::CTRL|fltk3::SHIFT,         Fl_Text_Editor::kf_c_s_move   },
  { fltk3::EndKey,       fltk3::CTRL|fltk3::SHIFT,         Fl_Text_Editor::kf_c_s_move   },
  { fltk3::LeftKey,      fltk3::CTRL|fltk3::SHIFT,         Fl_Text_Editor::kf_c_s_move   },
  { fltk3::UpKey,        fltk3::CTRL|fltk3::SHIFT,         Fl_Text_Editor::kf_c_s_move   },
  { fltk3::RightKey,     fltk3::CTRL|fltk3::SHIFT,         Fl_Text_Editor::kf_c_s_move   },
  { fltk3::DownKey,      fltk3::CTRL|fltk3::SHIFT,         Fl_Text_Editor::kf_c_s_move   },
  { fltk3::PageUpKey,   fltk3::CTRL|fltk3::SHIFT,         Fl_Text_Editor::kf_c_s_move   },
  { fltk3::PageDownKey, fltk3::CTRL|fltk3::SHIFT,         Fl_Text_Editor::kf_c_s_move   },
//{ FL_Clear,	  0,                        Fl_Text_Editor::delete_to_eol },
  { 'z',          fltk3::CTRL,                  Fl_Text_Editor::kf_undo	  },
  { '/',          fltk3::CTRL,                  Fl_Text_Editor::kf_undo	  },
  { 'x',          fltk3::CTRL,                  Fl_Text_Editor::kf_cut        },
  { fltk3::DeleteKey,    fltk3::SHIFT,                 Fl_Text_Editor::kf_cut        },
  { 'c',          fltk3::CTRL,                  Fl_Text_Editor::kf_copy       },
  { fltk3::InsertKey,    fltk3::CTRL,                  Fl_Text_Editor::kf_copy       },
  { 'v',          fltk3::CTRL,                  Fl_Text_Editor::kf_paste      },
  { fltk3::InsertKey,    fltk3::SHIFT,                 Fl_Text_Editor::kf_paste      },
  { 'a',          fltk3::CTRL,                  Fl_Text_Editor::kf_select_all },

#ifdef __APPLE__
  // Define CMD+key accelerators...
  { 'z',          fltk3::COMMAND,               Fl_Text_Editor::kf_undo       },
  { 'x',          fltk3::COMMAND,               Fl_Text_Editor::kf_cut        },
  { 'c',          fltk3::COMMAND,               Fl_Text_Editor::kf_copy       },
  { 'v',          fltk3::COMMAND,               Fl_Text_Editor::kf_paste      },
  { 'a',          fltk3::COMMAND,               Fl_Text_Editor::kf_select_all },
  { fltk3::LeftKey,      fltk3::COMMAND,               Fl_Text_Editor::kf_meta_move  },
  { fltk3::RightKey,     fltk3::COMMAND,               Fl_Text_Editor::kf_meta_move  },
  { fltk3::UpKey,        fltk3::COMMAND,               Fl_Text_Editor::kf_meta_move  },
  { fltk3::DownKey,      fltk3::COMMAND,               Fl_Text_Editor::kf_meta_move  },
  { fltk3::LeftKey,      fltk3::COMMAND|fltk3::SHIFT,      Fl_Text_Editor::kf_m_s_move   },
  { fltk3::RightKey,     fltk3::COMMAND|fltk3::SHIFT,      Fl_Text_Editor::kf_m_s_move   },
  { fltk3::UpKey,        fltk3::COMMAND|fltk3::SHIFT,      Fl_Text_Editor::kf_m_s_move   },
  { fltk3::DownKey,      fltk3::COMMAND|fltk3::SHIFT,      Fl_Text_Editor::kf_m_s_move   },
#endif // __APPLE__

  { 0,            0,                        0                             }
};

/**  Adds all of the default editor key bindings to the specified key binding list.*/
void Fl_Text_Editor::add_default_key_bindings(Key_Binding** list) {
  for (int i = 0; default_key_bindings[i].key; i++) {
    add_key_binding(default_key_bindings[i].key,
                    default_key_bindings[i].state,
                    default_key_bindings[i].func,
                    list);
  }
}

/**  Returns the function associated with a key binding.*/
Fl_Text_Editor::Key_Func Fl_Text_Editor::bound_key_function(int key, int state, Key_Binding* list) {
  Key_Binding* cur;
  for (cur = list; cur; cur = cur->next)
    if (cur->key == key)
      if (cur->state == FL_TEXT_EDITOR_ANY_STATE || cur->state == state)
        break;
  if (!cur) return 0;
  return cur->function;
}

/**  Removes all of the key bindings associated with the text editor or list.*/
void Fl_Text_Editor::remove_all_key_bindings(Key_Binding** list) {
  Key_Binding *cur, *next;
  for (cur = *list; cur; cur = next) {
    next = cur->next;
    delete cur;
  }
  *list = 0;
}

/** Removes the key binding associated with the key "key" of state "state" */
void Fl_Text_Editor::remove_key_binding(int key, int state, Key_Binding** list) {
  Key_Binding *cur, *last = 0;
  for (cur = *list; cur; last = cur, cur = cur->next)
    if (cur->key == key && cur->state == state) break;
  if (!cur) return;
  if (last) last->next = cur->next;
  else *list = cur->next;
  delete cur;
}
/** Adds a key of state "state" with the function "function" */
void Fl_Text_Editor::add_key_binding(int key, int state, Key_Func function,
                                Key_Binding** list) {
  Key_Binding* kb = new Key_Binding;
  kb->key = key;
  kb->state = state;
  kb->function = function;
  kb->next = *list;
  *list = kb;
}

////////////////////////////////////////////////////////////////

static void kill_selection(Fl_Text_Editor* e) {
  if (e->buffer()->selected()) {
    e->insert_position(e->buffer()->primary_selection()->start());
    e->buffer()->remove_selection();
  }
}

/** Inserts the text associated with the key */
int Fl_Text_Editor::kf_default(int c, Fl_Text_Editor* e) {
  // FIXME: this function is a mess! Fix this!
  if (!c || (!isprint(c) && c != '\t')) return 0;
  char s[2] = "\0";
  s[0] = (char)c;
  kill_selection(e);
  if (e->insert_mode()) e->insert(s);
  else e->overstrike(s);
  e->show_insert_position();
  e->set_changed();
  if (e->when()&fltk3::WHEN_CHANGED) e->do_callback();
  return 1;
}

/** Ignores the keypress */
int Fl_Text_Editor::kf_ignore(int, Fl_Text_Editor*) {
  return 0; // don't handle
}
/**  Does a backspace in the current buffer.*/
int Fl_Text_Editor::kf_backspace(int, Fl_Text_Editor* e) {
  if (!e->buffer()->selected() && e->move_left()) {
    int p1 = e->insert_position();
    int p2 = e->buffer()->next_char(p1);
    e->buffer()->select(p1, p2);
  }
  kill_selection(e);
  e->show_insert_position();
  e->set_changed();
  if (e->when()&fltk3::WHEN_CHANGED) e->do_callback();
  return 1;
}

/** Inserts a newline at the current cursor position */
int Fl_Text_Editor::kf_enter(int, Fl_Text_Editor* e) {
  kill_selection(e);
  e->insert("\n");
  e->show_insert_position();
  e->set_changed();
  if (e->when()&fltk3::WHEN_CHANGED) e->do_callback();
  return 1;
}

extern void fl_text_drag_me(int pos, Fl_Text_Display* d);
/**  Moves the text cursor in the direction indicated by key c.*/
int Fl_Text_Editor::kf_move(int c, Fl_Text_Editor* e) {
  int i;
  int selected = e->buffer()->selected();
  if (!selected)
    e->dragPos = e->insert_position();
  e->buffer()->unselect();
  fltk3::copy("", 0, 0);
  switch (c) {
  case fltk3::HomeKey:
      e->insert_position(e->buffer()->line_start(e->insert_position()));
      break;
    case fltk3::EndKey:
      e->insert_position(e->buffer()->line_end(e->insert_position()));
      break;
    case fltk3::LeftKey:
      e->move_left();
      break;
    case fltk3::RightKey:
      e->move_right();
      break;
    case fltk3::UpKey:
      e->move_up();
      break;
    case fltk3::DownKey:
      e->move_down();
      break;
    case fltk3::PageUpKey:
      for (i = 0; i < e->mNVisibleLines - 1; i++) e->move_up();
      break;
    case fltk3::PageDownKey:
      for (i = 0; i < e->mNVisibleLines - 1; i++) e->move_down();
      break;
  }
  e->show_insert_position();
  return 1;
}

/**  Extends the current selection in the direction of key c.*/
int Fl_Text_Editor::kf_shift_move(int c, Fl_Text_Editor* e) {
  kf_move(c, e);
  fl_text_drag_me(e->insert_position(), e);
  char *copy = e->buffer()->selection_text();
  if (copy) {
    fltk3::copy(copy, strlen(copy), 0);
    free(copy);
    }
  return 1;
}
/** Moves the current text cursor in the direction indicated by control key */
int Fl_Text_Editor::kf_ctrl_move(int c, Fl_Text_Editor* e) {
  if (!e->buffer()->selected())
    e->dragPos = e->insert_position();
  if (c != fltk3::UpKey && c != fltk3::DownKey) {
    e->buffer()->unselect();
    fltk3::copy("", 0, 0);
    e->show_insert_position();
  }
  switch (c) {
    case fltk3::HomeKey:
      e->insert_position(0);
      e->scroll(0, 0);
      break;
    case fltk3::EndKey:
      e->insert_position(e->buffer()->length());
      e->scroll(e->count_lines(0, e->buffer()->length(), 1), 0);
      break;
    case fltk3::LeftKey:
      e->previous_word();
      break;
    case fltk3::RightKey:
      e->next_word();
      break;
    case fltk3::UpKey:
      e->scroll(e->mTopLineNum-1, e->mHorizOffset);
      break;
    case fltk3::DownKey:
      e->scroll(e->mTopLineNum+1, e->mHorizOffset);
      break;
    case fltk3::PageUpKey:
      e->insert_position(e->mLineStarts[0]);
      break;
    case fltk3::PageDownKey:
      e->insert_position(e->mLineStarts[e->mNVisibleLines-2]);
      break;
  }
  return 1;
}

/** Moves the current text cursor in the direction indicated by meta key */
int Fl_Text_Editor::kf_meta_move(int c, Fl_Text_Editor* e) {
  if (!e->buffer()->selected())
    e->dragPos = e->insert_position();
  if (c != fltk3::UpKey && c != fltk3::DownKey) {
    e->buffer()->unselect();
    fltk3::copy("", 0, 0);
    e->show_insert_position();
  }
  switch (c) {
    case fltk3::UpKey:				// top of buffer
      e->insert_position(0);
      e->scroll(0, 0);
      break;
    case fltk3::DownKey:			// end of buffer
      e->insert_position(e->buffer()->length());
      e->scroll(e->count_lines(0, e->buffer()->length(), 1), 0);
      break;
    case fltk3::LeftKey:			// beginning of line
      kf_move(fltk3::HomeKey, e);
      break;
    case fltk3::RightKey:			// end of line
      kf_move(fltk3::EndKey, e);
      break;
  }
  return 1;
}

/** Extends the current selection in the direction indicated by meta key c. */
int Fl_Text_Editor::kf_m_s_move(int c, Fl_Text_Editor* e) {
  kf_meta_move(c, e);
  fl_text_drag_me(e->insert_position(), e);
  return 1;
}

/** Extends the current selection in the direction indicated by control key c. */
int Fl_Text_Editor::kf_c_s_move(int c, Fl_Text_Editor* e) {
  kf_ctrl_move(c, e);
  fl_text_drag_me(e->insert_position(), e);
  return 1;
}

/**  Moves the text cursor to the beginning of the current line.*/
int Fl_Text_Editor::kf_home(int, Fl_Text_Editor* e) {
    return kf_move(fltk3::HomeKey, e);
}

/**  Moves the text cursor to the end of the current line.*/
int Fl_Text_Editor::kf_end(int, Fl_Text_Editor* e) {
  return kf_move(fltk3::EndKey, e);
}

/**  Moves the text cursor one character to the left.*/
int Fl_Text_Editor::kf_left(int, Fl_Text_Editor* e) {
  return kf_move(fltk3::LeftKey, e);
}

/**  Moves the text cursor one line up.*/
int Fl_Text_Editor::kf_up(int, Fl_Text_Editor* e) {
  return kf_move(fltk3::UpKey, e);
}

/**  Moves the text cursor one character to the right.*/
int Fl_Text_Editor::kf_right(int, Fl_Text_Editor* e) {
  return kf_move(fltk3::RightKey, e);
}
/**  Moves the text cursor one line down.*/
int Fl_Text_Editor::kf_down(int, Fl_Text_Editor* e) {
  return kf_move(fltk3::DownKey, e);
}

/**  Moves the text cursor up one page.*/
int Fl_Text_Editor::kf_page_up(int, Fl_Text_Editor* e) {
  return kf_move(fltk3::PageUpKey, e);
}

/**  Moves the text cursor down one page.*/
int Fl_Text_Editor::kf_page_down(int, Fl_Text_Editor* e) {
  return kf_move(fltk3::PageDownKey, e);
}
/**  Toggles the insert mode in the text editor.*/
int Fl_Text_Editor::kf_insert(int, Fl_Text_Editor* e) {
  e->insert_mode(e->insert_mode() ? 0 : 1);
  return 1;
}

/**  Does a delete of selected text or the current character in the current buffer.*/
int Fl_Text_Editor::kf_delete(int, Fl_Text_Editor* e) {
  if (!e->buffer()->selected()) {
    int p1 = e->insert_position();
    int p2 = e->buffer()->next_char(p1);
    e->buffer()->select(p1, p2);
  }

  kill_selection(e);
  e->show_insert_position();
  e->set_changed();
  if (e->when()&fltk3::WHEN_CHANGED) e->do_callback();
  return 1;
}

/**  Does a copy of selected text or the current character in the current buffer.*/
int Fl_Text_Editor::kf_copy(int, Fl_Text_Editor* e) {
  if (!e->buffer()->selected()) return 1;
  const char *copy = e->buffer()->selection_text();
  if (*copy) fltk3::copy(copy, strlen(copy), 1);
  free((void*)copy);
  e->show_insert_position();
  return 1;
}

/**  Does a cut of selected text in the current buffer.*/
int Fl_Text_Editor::kf_cut(int c, Fl_Text_Editor* e) {
  kf_copy(c, e);
  kill_selection(e);
  e->set_changed();
  if (e->when()&fltk3::WHEN_CHANGED) e->do_callback();
  return 1;
}

/**  Does a paste of selected text in the current buffer.*/
int Fl_Text_Editor::kf_paste(int, Fl_Text_Editor* e) {
  kill_selection(e);
  fltk3::paste(*e, 1);
  e->show_insert_position();
  e->set_changed();
  if (e->when()&fltk3::WHEN_CHANGED) e->do_callback();
  return 1;
}

/**  Selects all text in the current buffer.*/
int Fl_Text_Editor::kf_select_all(int, Fl_Text_Editor* e) {
  e->buffer()->select(0, e->buffer()->length());
  const char *copy = e->buffer()->selection_text();
  if (*copy) fltk3::copy(copy, strlen(copy), 0);
  free((void*)copy);
  return 1;
}
/**  Undo last edit in the current buffer. Also deselect previous selection. */
int Fl_Text_Editor::kf_undo(int , Fl_Text_Editor* e) {
  e->buffer()->unselect();
  fltk3::copy("", 0, 0);
  int crsr;
  int ret = e->buffer()->undo(&crsr);
  e->insert_position(crsr);
  e->show_insert_position();
  e->set_changed();
  if (e->when()&fltk3::WHEN_CHANGED) e->do_callback();
  return ret;
}

/** Handles a key press in the editor */
int Fl_Text_Editor::handle_key() {
  // Call FLTK's rules to try to turn this into a printing character.
  // This uses the right-hand ctrl key as a "compose prefix" and returns
  // the changes that should be made to the text, as a number of
  // bytes to delete and a string to insert:
  int del = 0;
  if (fltk3::compose(del)) {
    if (del) {
      int dp = insert_position(), di = del;
      while (di--) dp = buffer()->prev_char_clipped(dp);
      buffer()->select(dp, insert_position());
    }
    kill_selection(this);
    if (fltk3::event_length()) {
      if (insert_mode()) insert(fltk3::event_text());
      else overstrike(fltk3::event_text());
    }
    show_insert_position();
    set_changed();
    if (when()&fltk3::WHEN_CHANGED) do_callback();
    return 1;
  }

  int key = fltk3::event_key(), state = fltk3::event_state(), c = fltk3::event_text()[0];
  state &= fltk3::SHIFT|fltk3::CTRL|fltk3::ALT|fltk3::META; // only care about these states
  Key_Func f;
  f = bound_key_function(key, state, global_key_bindings);
  if (!f) f = bound_key_function(key, state, key_bindings);
  if (f) return f(key, this);
  if (default_key_function_ && !state) return default_key_function_(c, this);
  return 0;
}

/** does or does not a callback according to changed() and when() settings */
void Fl_Text_Editor::maybe_do_callback() {
//  printf("Fl_Text_Editor::maybe_do_callback()\n");
//  printf("changed()=%d, when()=%x\n", changed(), when());
  if (changed() || (when()&fltk3::WHEN_NOT_CHANGED)) do_callback();
}

int Fl_Text_Editor::handle(int event) {
  static int dndCursorPos;
  
  if (!buffer()) return 0;

  switch (event) {
    case fltk3::FOCUS:
      show_cursor(mCursorOn); // redraws the cursor
      if (buffer()->selected()) redraw(); // Redraw selections...
      fltk3::focus(this);
      return 1;

    case fltk3::UNFOCUS:
      show_cursor(mCursorOn); // redraws the cursor
      if (buffer()->selected()) redraw(); // Redraw selections...
    case fltk3::HIDE:
      if (when() & fltk3::WHEN_RELEASE) maybe_do_callback();
      return 1;

    case fltk3::KEYBOARD:
      if (active_r() && window() && this == fltk3::belowmouse()) 
        window()->cursor(fltk3::CURSOR_NONE);
      return handle_key();

    case fltk3::PASTE:
      if (!fltk3::event_text()) {
        fl_beep();
	return 1;
      }
      buffer()->remove_selection();
      if (insert_mode()) insert(fltk3::event_text());
      else overstrike(fltk3::event_text());
      show_insert_position();
      set_changed();
      if (when()&fltk3::WHEN_CHANGED) do_callback();
      return 1;

    case fltk3::ENTER:
// MRS: WIN32 only?  Need to test!
//    case fltk3::MOVE:
      show_cursor(mCursorOn);
      return 1;

    case fltk3::PUSH:
      if (fltk3::event_button() == 2) {
        // don't let the text_display see this event
        if (fltk3::Group::handle(event)) return 1;
        dragType = DRAG_NONE;
	if(buffer()->selected()) {
	  buffer()->unselect();
	  }
	int pos = xy_to_position(fltk3::event_x(), fltk3::event_y(), CURSOR_POS);
        insert_position(pos);
        fltk3::paste(*this, 0);
        fltk3::focus(this);
        set_changed();
        if (when()&fltk3::WHEN_CHANGED) do_callback();
        return 1;
      }
      break;

    case fltk3::SHORTCUT:
      if (!(shortcut() ? fltk3::test_shortcut(shortcut()) : test_shortcut()))
        return 0;
      if (fltk3::visible_focus() && handle(fltk3::FOCUS)) {
        fltk3::focus(this);
        return 1;
      }
      break;
      
      // Handle drag'n'drop attempt by the user. This is a simplified 
      // implementation which allows dnd operations onto the scroll bars.
    case fltk3::DND_ENTER: // save the current cursor position
      if (fltk3::visible_focus() && handle(fltk3::FOCUS))
        fltk3::focus(this);
      show_cursor(mCursorOn);
      dndCursorPos = insert_position();
      /* fall through */
    case fltk3::DND_DRAG: // show a temporary insertion cursor
      insert_position(xy_to_position(fltk3::event_x(), fltk3::event_y(), CURSOR_POS));
      return 1;      
    case fltk3::DND_LEAVE: // restore original cursor
      insert_position(dndCursorPos);
      return 1;      
    case fltk3::DND_RELEASE: // keep insertion cursor and wait for the fltk3::PASTE event
      buffer()->unselect(); // fltk3::PASTE must not destroy current selection!
      return 1;
  }

  return Fl_Text_Display::handle(event);
}

//
// End of "$Id$".
//
