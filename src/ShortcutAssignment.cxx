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


// Shortcuts are stored in the association table

#include <config.h>
#include <fltk/events.h>
#include <fltk/Widget.h>
#include <fltk/WidgetAssociation.h>
#include <fltk/string.h>
#include <ctype.h>

using namespace fltk;

class shortcutAssociationType : public AssociationType {

  // we use the data pointer as unsigned int, so
  // we don't need to free anything
  void destroy(void *data) const {}
};

// this is our association to the shortcuts associated with widgets
static shortcutAssociationType shortcutAssociation;

/*!

  Add a new shortcut assignment. Returns true if successful.  If \a key
  is zero or the assignment already exists this does nothing and
  returns false.

  There can be any number of shortcut assignments, fltk stores them in
  internal tables shared by all widgets. A widget can have any number
  of shortcuts (though most have zero or one), and a given shortcut
  value can be assigned to more than one widget.

  If you only want one shortcut use shortcut() to assign it.

  The shortcut value is a bitwise OR (or sum) of a any set of shift flags
  returned by fltk::event_state(), and either a key symbol returned by
  fltk::event_key(), or an ASCII character.  Examples:
  - <code>fltk::CTRL+'a'</code>
  - <code>fltk::ACCELERATOR+fltk::CTRL+'A'</code>
  - just <code>'a'</code>
  - <code>fltk::SHIFT+'#'</code>
  - <code>fltk::SHIFT+fltk::UpKey</code>
  Case is ignored
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
bool Widget::add_shortcut(size_t key) {
  if (!key) return false;
  key = key&0xffff0000u|tolower(key&0xffu);
  if (find(shortcutAssociation, (void*)key)) return false;
  add(shortcutAssociation, (void*)key);
  return true;
}


/*!
  Delete a shortcut assignment. Returns true if it actually existed.
*/
bool Widget::remove_shortcut(size_t key) {
  return remove(shortcutAssociation, (void*)key);
}


/*!
  Remove all shortcuts for the widget.
  This is automatically done by the Widget destructor.
*/
void Widget::remove_shortcuts() {
  set(shortcutAssociation, 0);
}


/*!
  Returns one of the add_shortcut() assignments for this widget,
  or returns zero if there are none. If you want to look at more
  than onle you must use fltk::list_shortcuts(this).
*/
size_t Widget::shortcut() const {
  return (size_t)get(shortcutAssociation);
}


/*!
  Same as remove_shortcuts(), add_shortcut(key)
  except it may be implemented in a more efficient way.
  The result is exactly one shortcut (or none if \a key is zero).
*/
void Widget::shortcut(size_t key) {
  set(shortcutAssociation, (void*)key);
}


/*!
  Returns a value that can be passed to add_shortcut() so that this
  widget has a real shortcut assignment to match any &x in it's
  label().  The returned value is ALT|c where c is the character after
  the first '&' in the label (except '&&' is ignored), or zero if
  there isn't any '&' sign or if flag(RAW_LABEL) is on.
*/
size_t Widget::label_shortcut() const {
  if (flag(RAW_LABEL)) return 0;
  const char* label = this->label();
  if (!label) for (;*label;) {
    if (*label++ == '&') {
      if (*label == '&') label++;
      else return ACCELERATOR|*label;
    }
  }
  return 0;
}


/*!
  Test to see if the current KEY or SHORTCUT event matches a shortcut
  specified with &x in the label.

  This will match if the character in the label() after a '&'
  matches event_text()[0]. Case is ignored. The caller may want
  to check if ACCELERATOR or some other shift key is held down
  before calling this so that plain keys do not do anything, and
  should certainly make sure no other widgets want the shortcut.

  This is ignored if flag(RAW_LABEL) is on (which stops the &x from
  printing as an underscore. The sequence "&&" is ignored as well
  because that is used to print a plain '&' in the label.
*/
bool Widget::test_label_shortcut() const {
  if (flag(RAW_LABEL)) return false;
  char c = event_text()[0];
  if (!c) return false;
  const char* label = this->label();
  if (!label) return false;
  //if (ACCELERATOR==CTRL)
  if (event_state(CTRL) && (!(c&~0x1f)||c==127)) c ^= 0x40;
  c = tolower(c);
  // printf("ok shortcut %c\n",c);
  for (;*label;) {
    if (*label++ == '&') {
      if (*label) {
	if (*label != '&' && tolower(*label) == c) return true;
	label++;
      }
    }
  }
  return false;
}


// this class is a functor class used in the test_shortcut functions below
// the handle function is called for each associated key of this widget and
// it checks, if that key fist the current key event
// if that is the case, the function returns true, making the foreach loop
// stop and return something != 0, this is a sign for the function that
// a key has been associated to the current widget that fits the current
// key event...
class keyCompareFunctor : public AssociationFunctor {

  public:

    unsigned int count;

    keyCompareFunctor(void) : count(0) {}

    bool handle(const AssociationType&, const Widget*, void* data) {

      count++;

      size_t key = (size_t)data;

      unsigned mismatch = key ^ (event_key() | event_state());

      if (!(mismatch & 0xffff) &&
          !(mismatch & (key|META|ALT|CTRL|SHIFT)))
        return true;

      mismatch = key ^ (tolower(event_text()[0]) | event_state());

      if (!(mismatch & 0xffff) &&
          !(mismatch & (key|META|ALT|CTRL|SHIFT)))
        return true;

      return false;
    }
};


/*! Same as test_shortcut(true) */
bool Widget::test_shortcut() const { return test_shortcut(true); }


/*!
  Returns true if the current event matches one of the assignements
  made with add_shortcut(), or if test_label is true and
  test_label_shortcut() returns true. Normally a widget calls this
  in response to a SHORTCUT event to see if the shortcut key is
  assigned to it.
*/
bool Widget::test_shortcut(bool test_label) const {

  keyCompareFunctor fkt;

  // if there is a suitable assignment, return true
  if (foreach(shortcutAssociation, fkt)) return true;

  // if not, check how many keys had been assigned, if none,
  // check for label shortcut
  if (test_label && fkt.count == 0)
    return test_label_shortcut();

  return false;
}

////////////////////////////////////////////////////////////////

class GlueFunctor : public AssociationFunctor {
public:
  ShortcutFunctor& f;
  GlueFunctor(ShortcutFunctor& g) : f(g) {}
  bool handle(const AssociationType&, const Widget* widget, void* data) {
    return f.handle(widget, (size_t)data);
  }
};

/*!
  Call the handle() method from the passed ShortcutFunctor object
  for every Widget::shortcut() assignment known. If any return true
  then this immediately returns that shortcut value, else this
  returns zero after calling it for the last one. This is most
  useful for making a display of shortcuts for the user, or implementing
  a shortcut editor.

\code
class ListShortcuts : public ShortcutFunctor {
public:
  bool handle(const Widget* widget, unsigned key) {
    printf("Widget=%s shortcut=%s\n",
           widget->label() ? widget->label() : "NULL",
           key_name(key));
    return false;
  }
};
f() {
  ListShortcuts listShortcuts;
  fltk::foreachShortcut(listShortcuts);
}
\endcode

  If \a widget is not null, only do assignments for that widget,
  this is much faster than searching the entire list. This is useful
  for drawing the shortcuts on a widget (though most fltk widgets only
  draw the first one).
*/
unsigned fltk::foreachShortcut(const Widget* widget, ShortcutFunctor& f) {
  GlueFunctor g(f);
  return (size_t)(foreach(&shortcutAssociation, widget, g));
}

// End of $Id$


