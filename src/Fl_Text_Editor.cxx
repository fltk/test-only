//
// "$Id: Fl_Text_Editor.cxx,v 1.17 2002/12/18 08:34:22 spitzak Exp $"
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


#include <fltk/TextEditor.h>
#include <fltk/Style.h>
#include <fltk/events.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
using namespace fltk;

static void revert(Style*) {}
static NamedStyle style("Text_Editor", revert, &TextEditor::default_style);
NamedStyle* TextEditor::default_style = &::style;

TextEditor::TextEditor(int X, int Y, int W, int H,  const char* l)
    : TextDisplay(X, Y, W, H, l) {
  style(default_style);
  mCursorOn = 1;
  insert_mode_ = 1;
  key_bindings = 0;

  // handle the default key bindings
  add_default_key_bindings(&key_bindings);

  // handle everything else
  default_key_function(kf_default);
}

TextEditor::KeyBinding* TextEditor::global_key_bindings = 0;

static int ctrl_a(int, TextEditor* e);

// key will match in any state
#define ANY_STATE  (-1L)

// These are the default key bindings every widget should start with
static struct {
  int key;
  int state;
  TextEditor::KeyFunc func;
} default_key_bindings[] = {
//{ EscapeKey,	ANY_STATE,	TextEditor::kf_ignore     },
  { ReturnKey,	ANY_STATE,	TextEditor::kf_enter      },
  { KeypadEnter,ANY_STATE,	TextEditor::kf_enter     },
  { BackSpaceKey,ANY_STATE,	TextEditor::kf_backspace  },
  { InsertKey,	ANY_STATE,	TextEditor::kf_insert     },
  { DeleteKey,	ANY_STATE,	TextEditor::kf_delete     },
  { HomeKey,	0,                        TextEditor::kf_move       },
  { EndKey,	0,                        TextEditor::kf_move       },
  { LeftKey,	0,                        TextEditor::kf_move       },
  { UpKey,	0,                        TextEditor::kf_move       },
  { RightKey,	0,                        TextEditor::kf_move       },
  { DownKey,	0,                        TextEditor::kf_move       },
  { PageUpKey,	0,                        TextEditor::kf_move       },
  { PageDownKey,0,                        TextEditor::kf_move       },
  { HomeKey,	SHIFT,                 TextEditor::kf_shift_move },
  { EndKey,	SHIFT,                 TextEditor::kf_shift_move },
  { LeftKey,	SHIFT,                 TextEditor::kf_shift_move },
  { UpKey,	SHIFT,                 TextEditor::kf_shift_move },
  { RightKey,	SHIFT,                 TextEditor::kf_shift_move },
  { DownKey,	SHIFT,                 TextEditor::kf_shift_move },
  { PageUpKey,	SHIFT,                 TextEditor::kf_shift_move },
  { PageDownKey,SHIFT,                 TextEditor::kf_shift_move },
  { HomeKey,	CTRL,                  TextEditor::kf_ctrl_move  },
  { EndKey,	CTRL,                  TextEditor::kf_ctrl_move  },
  { LeftKey,	CTRL,                  TextEditor::kf_ctrl_move  },
  { UpKey,	CTRL,                  TextEditor::kf_ctrl_move  },
  { RightKey,	CTRL,                  TextEditor::kf_ctrl_move  },
  { DownKey,	CTRL,                  TextEditor::kf_ctrl_move  },
  { PageUpKey,	CTRL,                  TextEditor::kf_ctrl_move  },
  { PageDownKey,CTRL,                  TextEditor::kf_ctrl_move  },
  { HomeKey,	CTRL|SHIFT,         TextEditor::kf_c_s_move   },
  { EndKey,	CTRL|SHIFT,         TextEditor::kf_c_s_move   },
  { LeftKey,	CTRL|SHIFT,         TextEditor::kf_c_s_move   },
  { UpKey,	CTRL|SHIFT,         TextEditor::kf_c_s_move   },
  { RightKey,	CTRL|SHIFT,         TextEditor::kf_c_s_move   },
  { DownKey,	CTRL|SHIFT,         TextEditor::kf_c_s_move   },
  { PageUpKey,	CTRL|SHIFT,         TextEditor::kf_c_s_move   },
  { PageDownKey,CTRL|SHIFT,         TextEditor::kf_c_s_move   },
  { 'a',          CTRL,                  ctrl_a                        },
//{ 'z',          CTRL,                  TextEditor::undo	  },
  { 'x',          CTRL,                  TextEditor::kf_cut        },
  { 'c',          CTRL,                  TextEditor::kf_copy       },
  { 'v',          CTRL,                  TextEditor::kf_paste      },
  { 0,            0,                        0                             }
};

void TextEditor::add_default_key_bindings(KeyBinding** list) {
  for (int i = 0; default_key_bindings[i].key; i++) {
    add_key_binding(default_key_bindings[i].key,
                    default_key_bindings[i].state,
                    default_key_bindings[i].func,
                    list);
  }
}

static void kill_selection(TextEditor* e) {
  if (e->buffer()->selected()) {
    e->insert_position(e->buffer()->primary_selection()->start());
    e->buffer()->remove_selection();
  }
}

// Any keys not in above table go to this:

int TextEditor::kf_default(int c, TextEditor* e) {

  // See if the key is a shortcut assigned to some other widget or menu item:
  static bool recursion;
  if (!recursion) {
    recursion = true;
    bool ret = fltk::handle(SHORTCUT, e->window()) != 0;
    recursion = false;
    if (ret) return 1;
  }

  // Emulate Emacs for a lot of keys:
  int key = 0;
  switch (c) {
  case 'b': key = LeftKey; goto MOVE;
  case 'f': key = RightKey; goto MOVE;
  case 'p': key = UpKey; goto MOVE;
  case 'n': key = DownKey; goto MOVE;
  case 'e': key = EndKey; goto MOVE;
  MOVE:
  if (event_state(ALT)) {
    if (event_state(SHIFT))
      return TextEditor::kf_c_s_move(key,e);
    else
      return TextEditor::kf_ctrl_move(key,e);
  } else {
    if (event_state(SHIFT))
      return TextEditor::kf_shift_move(key,e);
    else
      return TextEditor::kf_move(key,e);
  }
  case 'd':
    return TextEditor::kf_delete(c,e);
  case 'h':
    return TextEditor::kf_backspace(c,e);
//case 'k':
//  return TextEditor::delete_to_eol(c,e);
//case 'o':  
//  return TextEditor::open_line(c,e);
//case 'q':  
//  return TextEditor::quote_next(c,e);
//case 'r':  
//  return TextEditor::reverse_search(c,e);
//case 's':  
//  return TextEditor::search(c,e);
//case 't':  
//  return TextEditor::transpose_chars(c,e);
//case 'u':  
//  return TextEditor::repeat prefix(c,e);
  case 'w':  
    return TextEditor::kf_cut(c,e);
  case 'y':  
    return TextEditor::kf_paste(c,e);
//case '/':  
//  return TextEditor::undo(c,e);
  }

  // insert other control characters into the text:
  if (event_length()) {
    kill_selection(e);
    if (e->insert_mode()) e->insert(event_text());
    else e->overstrike(event_text());
    e->show_insert_position();
    return 1;
  }
  return 0;
}

TextEditor::KeyFunc
TextEditor::bound_key_function(int key, int state, KeyBinding* list) {
  KeyBinding* current;
  for (current = list; current; current = current->next)
    if (current->key == key)
      if (current->state == ANY_STATE || current->state == state)
        break;
  if (!current) return 0;
  return current->function;
}

void
TextEditor::remove_all_key_bindings(KeyBinding** list) {
  KeyBinding *current, *next;
  for (current = *list; current; current = next) {
    next = current->next;
    delete current;
  }
  *list = 0;
}

void
TextEditor::remove_key_binding(int key, int state, KeyBinding** list) {
  KeyBinding *current, *last = 0;
  for (current = *list; current; last = current, current = current->next)
    if (current->key == key && current->state == state) break;
  if (!current) return;
  if (last) last->next = current->next;
  else *list = current->next;
  delete current;
}

void
TextEditor::add_key_binding(int key, int state, KeyFunc function,
			    KeyBinding** list) {
  KeyBinding* kb = new KeyBinding;
  kb->key = key;
  kb->state = state;
  kb->function = function;
  kb->next = *list;
  *list = kb;
}

////////////////////////////////////////////////////////////////

#define NORMAL_INPUT_MOVE 0

int TextEditor::kf_ignore(int, TextEditor*) {
  return 0; // don't handle
}

int TextEditor::kf_backspace(int, TextEditor* e) {
  if (!e->buffer()->selected() && e->move_left())
    e->buffer()->select(e->insert_position(), e->insert_position()+1);
  kill_selection(e);
  e->show_insert_position();
  return 1;
}

int TextEditor::kf_enter(int, TextEditor* e) {
  kill_selection(e);
  e->insert("\n");
  e->show_insert_position();
  return 1;
}

int TextEditor::kf_move(int c, TextEditor* e) {
  int i;
  int selected = e->buffer()->selected();
  if (!selected)
    e->dragPos = e->insert_position();
  e->buffer()->unselect();
  switch (c) {
  case HomeKey:
      e->insert_position(e->buffer()->line_start(e->insert_position()));
      break;
  case EndKey:
      e->insert_position(e->buffer()->line_end(e->insert_position()));
      break;
  case LeftKey:
      e->move_left();
      break;
  case RightKey:
      e->move_right();
      break;
  case UpKey:
      e->move_up();
      break;
  case DownKey:
      e->move_down();
      break;
  case PageUpKey:
      for (i = 0; i < e->mNVisibleLines - 1; i++) e->move_up();
      break;
  case PageDownKey:
      for (i = 0; i < e->mNVisibleLines - 1; i++) e->move_down();
      break;
  }
  e->show_insert_position();
  return 1;
}

int TextEditor::kf_shift_move(int c, TextEditor* e) {
  kf_move(c, e);
  e->drag_me(e->insert_position());
  return 1;
}

int TextEditor::kf_ctrl_move(int c, TextEditor* e) {
  if (!e->buffer()->selected())
    e->dragPos = e->insert_position();
  if (c != UpKey && c != DownKey) {
    e->buffer()->unselect();
    e->show_insert_position();
  }
  switch (c) {
    case HomeKey:
      e->insert_position(0);
      break;
    case EndKey:
      e->insert_position(e->buffer()->length());
      break;
    case LeftKey:
      e->previous_word();
      break;
    case RightKey:
      e->next_word();
      break;
    case UpKey:
      e->scroll(e->mTopLineNum-1, e->mHorizOffset);
      break;
    case DownKey:
      e->scroll(e->mTopLineNum+1, e->mHorizOffset);
      break;
    case PageUpKey:
      e->insert_position(e->mLineStarts[0]);
      break;
    case PageDownKey:
      e->insert_position(e->mLineStarts[e->mNVisibleLines-2]);
      break;
  }
  return 1;
}

int TextEditor::kf_c_s_move(int c, TextEditor* e) {
  kf_ctrl_move(c, e);
  e->drag_me(e->insert_position());
  return 1;
}

static int ctrl_a(int, TextEditor* e) {
  // make 2+ ^A's in a row toggle select-all:
  int i = e->buffer()->line_start(e->insert_position());
  if (i != e->insert_position())
    return TextEditor::kf_move(HomeKey, e);
  else {
    if (e->buffer()->selected())
      e->buffer()->unselect();
    else
      TextEditor::kf_select_all(0, e);
  }
  return 1;
}

int TextEditor::kf_home(int, TextEditor* e) {
    return kf_move(HomeKey, e);
}

int TextEditor::kf_end(int, TextEditor* e) {
  return kf_move(EndKey, e);
}

int TextEditor::kf_left(int, TextEditor* e) {
  return kf_move(LeftKey, e);
}

int TextEditor::kf_up(int, TextEditor* e) {
  return kf_move(UpKey, e);
}

int TextEditor::kf_right(int, TextEditor* e) {
  return kf_move(RightKey, e);
}

int TextEditor::kf_down(int, TextEditor* e) {
  return kf_move(DownKey, e);
}

int TextEditor::kf_page_up(int, TextEditor* e) {
  return kf_move(PageUpKey, e);
}

int TextEditor::kf_page_down(int, TextEditor* e) {
  return kf_move(PageDownKey, e);
}


int TextEditor::kf_insert(int, TextEditor* e) {
  e->insert_mode(e->insert_mode() ? 0 : 1);
  return 1;
}

int TextEditor::kf_delete(int, TextEditor* e) {
  if (!e->buffer()->selected())
    e->buffer()->select(e->insert_position(), e->insert_position()+1);
  kill_selection(e);
  e->show_insert_position();
  return 1;
}

int TextEditor::kf_copy(int, TextEditor* e) {
  if (!e->buffer()->selected()) return 1;
  const char *copy = e->buffer()->selection_text();
  if (*copy) fltk::copy(copy, strlen(copy), true);
  free((void*)copy);
  e->show_insert_position();
  return 1;
}

int TextEditor::kf_cut(int c, TextEditor* e) {
  kf_copy(c, e);
  kill_selection(e);
  return 1;
}

int TextEditor::kf_paste(int, TextEditor* e) {
  kill_selection(e);
  paste(*e,true);
  e->show_insert_position();
  return 1;
}

int TextEditor::kf_select_all(int, TextEditor* e) {
  e->buffer()->select(0, e->buffer()->length());
  return 1;
}

int TextEditor::handle_key() {

  // Call fltk's rules to try to turn this into a printing character.
  // This uses the right-hand ctrl key as a "compose prefix" and returns
  // the changes that should be made to the text, as a number of
  // bytes to delete and a string to insert:
  int del;
  if (compose(del)) {
    if (del) buffer()->select(insert_position()-del, insert_position());
    kill_selection(this);
    if (event_length()) {
      if (insert_mode()) insert(event_text());
      else overstrike(event_text());
    }
    show_insert_position();
    return 1;
  }

  int key = event_key();
  int state = event_state() & (SHIFT|CTRL|ALT|COMMAND);
  KeyFunc f = bound_key_function(key, state, global_key_bindings);
  if (!f) f = bound_key_function(key, state, key_bindings);
  if (!f) f = default_key_function_;
  if (!f) f = kf_default;
  return f(key, this);
}

int TextEditor::handle(int event) {
  if (!buffer()) return 0;

  if (TextDisplay::handle(event)) {
    switch (event) {

    case PUSH:
      if (event_button() == 2) {
	dragType = -1;
	paste(*this,false);
      }
      return 1;

    case FOCUS:
      return 3; // indicate that this widget should get initial focus

    default:
      return 1;
    }
  } else {
    switch (event) {

    case KEY:
      return handle_key();

    case PASTE:
	  if (!event_text()) break;
      buffer()->remove_selection();
      if (insert_mode()) insert(event_text());
      else overstrike(event_text());
      show_insert_position();
      return 1;

// CET - FIXME - this will clobber the window's current cursor state!
//    case ENTER:
//    case MOVE:
//    case LEAVE:
//      if (event_inside(text_area)) cursor(CURSOR_INSERT);
//      else cursor(CURSOR_DEFAULT);
    }
    return 0;
  }
}

//
// End of "$Id: Fl_Text_Editor.cxx,v 1.17 2002/12/18 08:34:22 spitzak Exp $".
//
