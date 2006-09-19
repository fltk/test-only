//
// "$Id$"
//
// Copyright 1998-2006 by Bill Spitzak and others.
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


// Shortcuts are stored entirely using hidden data structures, and are
// looked up by searching for pointers in tables. This keeps the size
// of widgets with no shortcut small, and allows us freedom to change
// the implementation. The current implementation just does binary
// search for shortcuts and uses insertion sort to put things into the
// tables, but a hash table is probably better and it should be
// changed to this someday.

// Two parallel arrays are maintained with exactly the same data.
// One is sorted by widget, the other by key. If the structure gets
// any more complex we may want to sort arrays of pointers instead.

#include <config.h>
#include <fltk/events.h>
#include <fltk/Widget.h>
#include <fltk/string.h>
#include <ctype.h>

using namespace fltk;

static ShortcutAssignment* keylist = 0;
static ShortcutAssignment* widgetlist = 0;
static unsigned num_shortcuts = 0;
static unsigned array_size = 0;

// Returns entry, or pointer where to insert it:
static ShortcutAssignment* findkey(unsigned key) {
  unsigned a = 0;
  unsigned b = num_shortcuts;
  while (a < b) {
    unsigned c = (a+b)/2;
    ShortcutAssignment* p = keylist+c;
    int n = (p->key&0xffff)-(key&0xffff);
    if (n < 0) a = c+1;
    else if (n > 0) b = c;
    else {
      n = p->key - key;
      if (n < 0) a = c+1;
      else if (n > 0) b = c;
      else {
	// found it, but search backwards for first one
	while (p > keylist && (p-1)->key == key) --p;
	return p;
      }
    }
  }
  return keylist+a;
}

// Search the sorted-by-widget list
static ShortcutAssignment* findwidget(const Widget* widget) {
  unsigned a = 0;
  unsigned b = num_shortcuts;
  while (a < b) {
    int c = (a+b)/2;
    ShortcutAssignment* p = widgetlist+c;
    if (p->widget < widget) a = c+1;
    else if (p->widget > widget) b = c;
    else {
      // found it, but search backwards for first one
      while (p > widgetlist && (p-1)->widget == widget) --p;
      return p;
    }
  }
  return widgetlist+a;
}

static unsigned last_list = 0;
static const ShortcutAssignment* last_list_pointer = 0;
static unsigned last_list_length = 0;

/*!

  Add a new shortcut assignment. Returns true if successful.  If \a key
  is zero or the assignment already exists this does nothing and
  returns false.

  There can be any number of shortcut assignments, fltk stores them in
  internal tables shared by all widgets. A widget can have any number
  of shortcuts (though most have zero or one), and a given shortcut
  value can be assigned to more than one widget. You can examine
  the assignments with fltk::list_shortcuts().

  If you only want one shortcut use shortcut() to assign it.

  The shortcut value is a bitwise OR (or sum) of a any set of shift flags
  returned by fltk::event_state(), and either a key symbol returned by
  fltk::event_key(), or an ASCII character.  Examples:
  - <code>fltk::CTRL+'a'</code>
  - <code>fltk::ACCELERATOR+fltk::CTRL+'A'</code>
  - just <code>'a'</code>
  - <code>fltk::SHIFT+'#'</code>
  - <code>fltk::SHIFT+fltk::UpKey</code>
  See fltk::list_matching_shortcuts() for the exact rules for how
  a KEY event is matched to a shortcut assignment. Case is ignored
  (the lower-case version of any letter is actually put in the table).

  When FLTK gets a keystroke, it sends it to the fltk::focus()
  widget. If that widget's handle() returns 0, it will also send
  the keystroke to all parents of that widget (this is mostly for
  keyboard navigation to work). If all of them return 0, or the
  fltk::focus() is null, then it will try sending a SHORTCUT event
  to every single widget inside the same window as the focus until
  one of them returns non-zero. In most cases widgets will call
  Widget::test_shortcut() to see if the keystroke is registered here (many
  widgets will also directly test the key to see if it is something
  they are interested in).  */
bool Widget::add_shortcut(unsigned key) {
  if (!key) return false;
  if (!(key&0xff00u)) key = key&0xffff0000u|tolower(key&0xffu);
  // return false if any duplicates, then point after last one:
  ShortcutAssignment* p = findwidget(this);
  while (p < widgetlist+num_shortcuts && p->widget == this) {
    if (p->key == key) return false;
    p++;
  }
  if (num_shortcuts >= array_size) {
    array_size = array_size ? 2*array_size : 64;
    ShortcutAssignment* newlist = new ShortcutAssignment[array_size];
    memcpy(newlist, keylist, num_shortcuts*sizeof(*keylist));
    delete[] keylist;
    keylist = newlist;
    newlist = new ShortcutAssignment[array_size];
    memcpy(newlist, widgetlist, num_shortcuts*sizeof(*widgetlist));
    p = p-widgetlist+newlist;
    delete[] widgetlist;
    widgetlist = newlist;
  }
  // insert the new assignment:
  memmove(p+1, p, (widgetlist+num_shortcuts-p)*sizeof(*widgetlist));
  p->widget = this;
  p->key = key;
  // also insert it into keylist, after all other matching keys:
  p = findkey(key);
  while (p < keylist+num_shortcuts && p->key==key) p++;
  memmove(p+1, p, (keylist+num_shortcuts-p)*sizeof(*keylist));
  p->widget = this;
  p->key = key;
  ++num_shortcuts;
  last_list = 0;
  return true;
}

/*! Delete a shortcut after widget entry is found */
static void remove_keylist(ShortcutAssignment* p) {
  // Find the matching one in shortcut list (this will crash if missing):
  ShortcutAssignment* q = findkey(p->key);
  for (;;) {
    //assert(q < keylist+num_shortcuts);
    if (q->widget == p->widget) break;
    ++q;
  }
  memmove(q, q+1, (keylist+num_shortcuts-q-1)*sizeof(*keylist));
  last_list = 0;
}

/*! Delete a shortcut assignment. Returns true if it actually existed. */
bool Widget::remove_shortcut(unsigned key) {
  if (!key) return false;
  if (!(key&0xff00u)) key = key&0xffff0000u|tolower(key&0xffu);
  ShortcutAssignment* p = findwidget(this);
  for (;;) {
    if (p >= widgetlist+num_shortcuts) return false;
    if (p->widget != this) return false;
    if (p->key == key) {
      ::remove_keylist(p);
      // now delete the widgetlist entry:
      memmove(p, p+1, (widgetlist+num_shortcuts-p-1)*sizeof(*widgetlist));
      --num_shortcuts;
      return true;
    }
    ++p;
  }
}

/*! Remove all shortcuts for the widget.  Returns true if there were
    any. This is automatically done by the Widget destructor.
*/
bool Widget::remove_shortcuts() {
  ShortcutAssignment* p = findwidget(this);
  unsigned n = 0;
  while (p+n < widgetlist+num_shortcuts && p[n].widget == this) {
    remove_keylist(p+n);
    n++;
  }
  if (!n) return false;
  memmove(p, p+n, (widgetlist+num_shortcuts-p-n)*sizeof(*widgetlist));
  num_shortcuts -= n;
  return true;
}

/*! Returns an array of shortcut assignments for this widget.
    The number of matching ones is put in \a count. Do not
    assumme any particular order for the returned array. Also
    there may be entries with key==0, ignore them.

    The returned array is temporary storage and can be overwritten
    by the next call to alter or query shortcuts!
*/
const ShortcutAssignment*
fltk::list_shortcuts(const Widget* widget, unsigned& count) {
  ShortcutAssignment* p = findwidget(widget);
  unsigned n = 0;
  for (; p+n < widgetlist+num_shortcuts && p[n].widget==widget; n++);
  count = n;
  return p;
}

/*! Returns one of the add_shortcut() assignments for this widget,
    or returns zero if there are none. If you want to look at more
    than onle you must use fltk::list_shortcuts(this).
*/
unsigned Widget::shortcut() const {
  ShortcutAssignment* p = findwidget(this);
  if (p < widgetlist+num_shortcuts && p->widget==this) return p->key;
  return 0;
}

/*! Returns an array of shortcut assignments that match the given
    value for \a key. The number of matching ones is put in \a count.
    If there are no matches \a count is set to zero. Do not assumme
    any particular order for the returned array. Also there may be
    entries with widget==0, ignore them.

    The returned array is temporary storage and can be overwritten
    by the next call to alter or query shortcuts!  */
const ShortcutAssignment*
fltk::list_shortcuts(unsigned key, unsigned& count) {
  if (!(key&0xff00u)) key = key&0xffff0000u|tolower(key&0xffu);
  ShortcutAssignment* p = findkey(key);
  unsigned n = 0;
  for (; p+n < keylist+num_shortcuts && p[n].key==key; n++);
  count = n;
  return p;
}

/*!
  Returns an array of shortcut assignments that match the current
  event, which should be a KEY, SHORTCUT, or KEYUP event.  The number
  of matching ones is put in \a count.  If there are no matches null
  is returned and \a count is set to zero.

  The returned array is temporary storage and can be overwritten
  by the next call to alter or query shortcuts! Also there may be
  entries with widget==0, ignore them.

  This is very similar to list_shortcuts(event_key()|event_state())
  except if no \e exact match is found, it will try to locate a
  "close" matching key assignment and return that:

  - Shift flags \e other than META, ALT, SHIFT, and CTRL do not have to
    be "off". For instance a shortcut with SCROLL_LOCK requires the
    SCROLL_LOCK to be on, but one without it does not require it to
    be off. If there are several matches the numerically-highest
    one is chosen.
  - If no shortcut matches fltk::event_key(), then the first letter
    of fltk::event_text() is converted to upper-case and tried instead,
    and SHIFT can be held down even if the shortcut does not indicate
    it. This means that a shortcut that is a capital 'A' will match
    the 'a' key, and '3' will match both the main and keypad 3, and
    a shortcut of '#' or SHIFT+'#' will work.
*/
const ShortcutAssignment*
fltk::list_matching_shortcuts(unsigned& count) {
  // This will get called a LOT for the same event, so cache the return value:
  if ((event_key()|event_state())==last_list) {
    count = last_list_length;
    return last_list_pointer;
  }
  ShortcutAssignment* ret = 0;
  count = 0;
  // Check against the actual keysym, return highest one (most likely the
  // one with the most shift keys down):
  ShortcutAssignment* p = findkey(event_key());
  for (; p < keylist+num_shortcuts ;) {
    unsigned mismatch = p->key ^ (event_key()|event_state());
    if (mismatch & 0xffff) break; // we are in the next key
    // find out how many identical shortcut assignments there are:
    unsigned n = 1;
    for (; p+n < keylist+num_shortcuts && p[n].key==p->key; n++);
    // we must match all shifts in the shortcut, plus the 4 main shifts:
    if (!(mismatch & (p->key|META|ALT|CTRL|SHIFT))) {
      ret = p;
      count = n;
    }
    // skip to next different shortcut:
    p += n;
  }
  // If nothing matched, try using the ASCII letters. Again find the highest
  // one, but we don't require shift to be up:
  if (!ret && event_text() && event_text()[0]) {
    unsigned c = tolower(event_text()[0]);
    p = findkey(c);
    for (; p < keylist+num_shortcuts ;) {
      unsigned mismatch = p->key ^ (c|event_state());
      if (mismatch & 0xffff) break; // we are in the next key
      // find out how many identical shortcut assignments there are:
      unsigned n = 1;
      for (; p+n < keylist+num_shortcuts && p[n].key==p->key; n++);
      // we must match all shifts in the shortcut, plus the 3 main shifts:
      if (!(mismatch & (p->key|META|ALT|CTRL))) {
	ret = p;
	count = n;
      }
      // skip to next different shortcut:
      p += n;
    }
  }
#if 0
  if (ret) {
    printf("For key %x found %d matches starting at index %d in:\n",
	   event_key()|event_state(), count, ret-keylist);
    for (unsigned n = 0; n < num_shortcuts; n++)
      printf(" %s %x\n", keylist[n].widget->label(), keylist[n].key);
  }
#endif
  last_list = event_key()|event_state();
  last_list_length = count;
  last_list_pointer = ret;
  return ret;
}

/*! Returns an array of every shortcut assignment. They are sorted by
    the key, and then by shift flags.

    The returned array is temporary storage and will be overwritten
    by the next call to alter or query shortcuts! Also there may be
    entries with widget==0, ignore them.
*/
const ShortcutAssignment* fltk::list_shortcuts(unsigned& count) {
  count = num_shortcuts;
  return keylist;
}

/*!
  Same as remove_shortcuts(), add_shortcut(key)
  except it may be implemented in a more efficient way.
  The result is exactly one shortcut (or none if \a key is zero).
  Return value is intended to indicate if the shortcut changed, but
  that is nyi.
*/
bool Widget::shortcut(unsigned key) {
  remove_shortcuts();
  return add_shortcut(key);
}

/*!
  Returns a value that can be passed to add_shortcut() so that this
  widget has a real shortcut assignment to match any &x in it's label().
  The returned value is ALT|c where c is the character after
  the first '&' in the label, or zero if there isn't any '&' sign
  or if flags() has RAW_LABEL turned on.
*/
unsigned Widget::label_shortcut() const {
  if (flags() & RAW_LABEL) return 0;
  const char* label = this->label();
  if (!label) for (;*label;) {
    if (*label++ == '&') {
      if (*label == '&') label++;
      else return ACCELERATOR|*label;
    }
  }
  return 0;
}

/*! Test to see if the current KEY or SHORTCUT event matches a shortcut
    specified with &x in the label.

    This will match if the character after the first '&' matches the
    event_text()[0]. Case is ignored. The caller may want to
    check if ALT or some other shift key is held down before calling
    this so that plain keys do not do anything, and should certainly
    make sure no other widgets want the shortcut.

    This is ignored if flags() has RAW_LABEL turned on (which stops
    the &x from printing as an underscore. The sequence "&&" is ignored
    as well because that is used to print a plain '&' in the label.
*/
bool Widget::test_label_shortcut() const {
  if (flags() & RAW_LABEL) return false;
  char c = tolower(event_text()[0]);
  if (c <= 0x1A) c+=0x60; // remove ctrl
  // printf("ok shortcut %c\n",c);
  const char* label = this->label();
  if (!c || !label) return false;
  for (;*label;) {
    if (*label++ == '&') {
      if (*label == '&') label++;
      else return (tolower(*label) == c);
    }
  }
  return false;
}

/*! Same as test_shortcut(true) */
bool Widget::test_shortcut() const {
  return test_shortcut(true);
}

/*! Returns true if the current event matches one of the assignements
    made with add_shortcut(), or if test_label is true and
    test_label_shortcut() returns true. Normally a widget calls this
    in response to a SHORTCUT event to see if the shortcut key is
    assigned to it.

    This is done by doing list_matching_shortcuts() and seeing if this
    widget is in the returned list. If the list is empty and test_label
    is true, it will return test_label_shortcut().

    If the current event matches a \e different widget "better" than
    this one, then false is returned. For instance if this widget has
    'x' as a shortcut, this will return true if the user types
    'X'. But if another widget has 'X' then this will return
    false. See fltk::list_matching_shortcuts() for the rules about
    what ones are "better".
*/
bool Widget::test_shortcut(bool test_label) const {
  unsigned count;
  const ShortcutAssignment* list = list_matching_shortcuts(count);
  if (!count) return test_label && test_label_shortcut();
  for (unsigned i = 0; i < count; i++)
    if (list[i].widget == this) return true;
  return false;
}

// End of $Id$


