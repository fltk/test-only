//
// "$Id: Fl_Menu_.H,v 1.7.2.4.2.4.2.6 2004/03/18 08:00:34 matthiaswm Exp $"
//
// Menu base class header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Menu__H
#define Fl_Menu__H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif
#include "Fl_Menu_Item.H"

/** All widgets that have a menu in FLTK are subclassed off of this class.
 * Currently FLTK provides you with Fl_Menu_Button, Fl_Menu_Bar, and
 * Fl_Choice.
 *
 * The class contains a pointer to an array of structures of type
 * Fl_Menu_Item. The array may either be supplied directly by the user
 * program, or it may be "private": a dynamically allocated array managed by
 * the Fl_Menu_. */
class FL_EXPORT Fl_Menu_ : public Fl_Widget {

  Fl_Menu_Item *menu_;
  const Fl_Menu_Item *value_;

protected:

  uchar alloc;
  uchar down_box_;
  uchar textfont_;
  uchar textsize_;
  unsigned textcolor_;

public:
  int item_pathname(char *name, int namelen, const Fl_Menu_Item *finditem=0) const;
  const Fl_Menu_Item* picked(const Fl_Menu_Item*);

    /** Creates a new Fl_Menu_ widget using the given position, size, and
     * label string. menu() is initialized to \c NULL. */
  Fl_Menu_(int x,int y,int w,int h,const char *label =0);
    /** If the menu array is private the memory it uses is freed. */
  ~Fl_Menu_();

    /** Only call this in response to FL_SHORTCUT events. If the event
     * matches an entry in the menu that entry is selected and the callback
     * will be done (or changed() will be set). This allows shortcuts
     * directed at one window to call menus in another. */
  const Fl_Menu_Item* test_shortcut() {return picked(menu()->test_shortcut());}
    /** Make the shortcuts for this menu work no matter what window has the
     * focus when you type it. This is done by using Fl::add_handler(). This
     * Fl_Menu_ widget does not have to be visible (ie the window it is in
     * can be hidden, or it does not have to be put in a window at all).
     *
     * Currently there can be only one global() menu. Setting a new one will
     * replace the old one. There is no way to remove the global() setting
     * (so don't destroy the widget!) */
  void global();

    /** Returns a pointer to the array of Fl_Menu_Items. This will either be
     * the value passed to <tt>menu(value)</tt> or the private copy. */
  const Fl_Menu_Item *menu() const {return menu_;}
    /** Set the menu array pointer directly. If the old menu is private it
     * is deleted. \c NULL is allowed and acts the same as a zero-length
     * menu. If you try to modify the array (with add(), replace(), or
     * remove()) a private copy is automatically done. */
  void menu(const Fl_Menu_Item *m);
    /** The menu is set to a private copy of the passed Fl_Menu_Item array.
     * This is useful if you want to modify the flags of the menu items. */
  void copy(const Fl_Menu_Item *m, void* user_data = 0);
    /** Adds a new menu item, with a title string, shortcut string, callback,
     * argument to the callback, and flags. If the menu array was directly
     * set with menu(x) then copy() is done to make a private array.
     *
     * Text is a string of the form "foo/bar/baz", this example will result
     * in a submenu called "foo" and one in that called "bar" and and entry
     * called "baz". The text is copied to new memory and can be freed. The
     * other arguments (including the shortcut) are copied into the menu
     * item unchanged.
     *
     * If an item exists already with that name then it is replaced with
     * this new one. Otherwise this new one is added to the end of the
     * correct menu or submenu. The return value is the offset into the
     * array that the new entry was placed at.
     *
     * Shortcut can be \c 0L, or either a modifier/key combination (for
     * example <tt>FL_CTRL+'A'</tt>) or a string describing the shortcut in
     * one of two ways:
     * \arg <tt>[#+^]int</tt>, eg. "97", "^97", "+97", "#97"
     * \arg <tt>[#+^]char</tt>, eg. "a", "^a", "+a", "#a"
     *
     * ... where is a decimal value representing an ascii character (eg. 97
     * is the ascii for 'a'), and the optional prefixes enhance the value 
     * that follows. Multiple prefixes must appear in the above order.
     * \arg \c # - Alt
     * \arg \c + - Shift
     * \arg \c ^ - Control
     *
     * Text shortcuts are converted to integer shortcut by calling <tt>int
     * fl_old_shortcut(const char*)</tt>.
     *
     * The return value is the index into the array that the entry was put. */
  int  add(const char*text, int shortcut, Fl_Callback*, void* = 0, int = 0);
  int  add(const char* a, const char* b, Fl_Callback* c,
	  void* d = 0, int e = 0) {return add(a,fl_old_shortcut(b),c,d,e);}
    /** This returns the number of Fl_Menu_Item structures that make up the
     * menu, correctly counting submenus. This includes the "terminator"
     * item at the end. To copy a menu array you need to copy
     * <tt>size()*sizeof(Fl_Menu_Item)</tt> bytes. If the menu is \c NULL
     * this returns zero (an empty menu will return 1). */
  int  size() const ;
  void size(int W, int H) { Fl_Widget::size(W, H); }
    /** Same as <tt>menu(NULL)</tt>, set the array pointer to null,
     * indicating a zero-length menu. */
  void clear();
    /** The passed string is split at any '|' characters and then
     * add(s,0,0,0,0) is done with each section. This is often useful if
     * you are just using the value, and is compatable with Forms and other
     * GL programs. */
  int  add(const char *menutext);
    /** Changes the text of item n. This is the only way to get slash into
     * an add()'ed menu item. If the menu array was directly set with
     * menu(x) then copy() is done to make a private array. */
  void replace(int index,const char *text);
    /** Deletes item n from the menu. If the menu array was directly set
     * with menu(x) then copy() is done to make a private array. */
  void remove(int index);
    /** Changes the shortcut of item \a i to \a n. */
  void shortcut(int i, int s) {menu_[i].shortcut(s);}
    /** Changes the flags of item i. For a list of the flags, see Fl_Menu_Item. */
  void mode(int i,int fl) {menu_[i].flags = fl;}
  int  mode(int i) const {return menu_[i].flags;}

  const Fl_Menu_Item *mvalue() const {return value_;}
    /** The value is the index into menu() of the last item chosen by the
     * user. It is zero initially. */
  int value() const {return (int)(value_-menu_);}
  int value(const Fl_Menu_Item*);
    /** The value is the index into menu() of the last item chosen by the
     * user. You can set it as an integer, or set it with a pointer to a
     * menu item. The set routines return non-zero if the new value is
     * different than the old one. */
  int value(int i) {return value(menu_+i);}
    /** Returns the title of the last item chosen. */
  const char *text() const {return value_ ? value_->text : 0;}
    /** Returns the title of item \a i. */
  const char *text(int i) const {return menu_[i].text;}

    /** Get the current font of menu item labels. */
  Fl_Font textfont() const {return (Fl_Font)textfont_;}
    /** Set the current font of menu item labels. */
  void textfont(uchar c) {textfont_=c;}
    /** Get the font size of menu item labels. */
  uchar textsize() const {return textsize_;}
    /** Set the font size of menu item labels. */
  void textsize(uchar c) {textsize_=c;}
    /** Get the current color of menu item labels. */
  Fl_Color textcolor() const {return (Fl_Color)textcolor_;}
    /** Set the current color of menu item labels. */
  void textcolor(unsigned c) {textcolor_=c;}

  Fl_Boxtype down_box() const {return (Fl_Boxtype)down_box_;}
    /** This box type is used to surround the currently-selected items in
     * the menus. If this is \c FL_NO_BOX then it acts like \c FL_THIN_UP_BOX
     * and selection_color() acts like \c FL_WHITE, for back compatability. */ 
  void down_box(Fl_Boxtype b) {down_box_ = b;}

  // back compatability:
  Fl_Color down_color() const {return selection_color();}
  void down_color(unsigned c) {selection_color(c);}
};

#endif

//
// End of "$Id: Fl_Menu_.H,v 1.7.2.4.2.4.2.6 2004/03/18 08:00:34 matthiaswm Exp $".
//
