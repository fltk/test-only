/
// "$Id$
/
// Menu item header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#ifndef Fl_Menu_Item_
#define Fl_Menu_Item_

#  include "Fl_Widget.H
#  include "Fl_Image.H

#  if defined(__APPLE__) && defined(__MWERKS__) && defined(check
#    undef chec
#  endi

enum { // values for flags
  FL_MENU_INACTIVE = 1
  FL_MENU_TOGGLE= 2
  FL_MENU_VALUE = 4
  FL_MENU_RADIO = 8
  FL_MENU_INVISIBLE = 0x10
  FL_SUBMENU_POINTER = 0x20
  FL_SUBMENU = 0x40
  FL_MENU_DIVIDER = 0x80
  FL_MENU_HORIZONTAL = 0x10
}

extern FL_EXPORT int fl_old_shortcut(const char*)

class Fl_Menu_

/** The Fl_Menu_Item structure defines a single menu item that is used b
 * the Fl_Menu_class. This structure is defined in <FL/Fl_Menu_Item.H> 
 \cod
struct Fl_Menu_Item 
  const char*   text; // label(
  ulong         shortcut_
  Fl_Callback*  callback_
  void*         user_data_
  int           flags
  uchar         labeltype_
  uchar         labelfont_
  uchar         labelsize_
  uchar         labelcolor_
}
\endcod
 * Typically menu items are statically defined; for example
\cod
Fl_Menu_Item popup[] = 
  {"&alpha",   FL_ALT+'a', the_cb, (void*)1}
  {"&beta",    FL_ALT+'b', the_cb, (void*)2}
  {"gamma",    FL_ALT+'c', the_cb, (void*)3, FL_MENU_DIVIDER}
  {"&strange",  0,   strange_cb}
  {"&charm",    0,   charm_cb}
  {"&truth",    0,   truth_cb}
  {"b&eauty",   0,   beauty_cb}
  {"sub&menu",  0,   0, 0, FL_SUBMENU}
    {"one"}
    {"two"}
    {"three"}
  {0}
  {"inactive", FL_ALT+'i', 0, 0, FL_MENU_INACTIVE|FL_MENU_DIVIDER}
  {"invisible",FL_ALT+'i', 0, 0, FL_MENU_INVISIBLE}
  {"check",    FL_ALT+'i', 0, 0, FL_MENU_TOGGLE|FL_MENU_VALUE}
  {"box",      FL_ALT+'i', 0, 0, FL_MENU_TOGGLE}
{0}}
\endcod
 * produces
 *   \image html menu.gi
 * A submenu title is identified by the bit \c FL_SUBMENU in the flags
 * field, and ends with a label() that is \c NULL. You can nest menus to an
 * depth. A pointer to the first item in the submenu can be treated as a
 * Fl_Menu array itself. It is also possible to make seperate submenu array
 * with \c FL_SUBMENU_POINTER flags
 
 * You should use the method functions to access structure members and no
 * access them directly to avoid compatibility problems with future release
 * of FLTK
 *
struct FL_EXPORT Fl_Menu_Item 
  const char *text;	// label(
  int shortcut_
  Fl_Callback *callback_
  void *user_data_
  int flags
  uchar labeltype_
  uchar labelfont_
  uchar labelsize_
  unsigned labelcolor_

  // advance N items, skipping submenus
  const Fl_Menu_Item *next(int=1) const
    /** Advance a pointer by \a n items through a menu array, skipping th
     * contents of submenus and invisible items. There are two calls so tha
     * you can advance through const and non-const data. *
  Fl_Menu_Item *next(int i=1) 
    return (Fl_Menu_Item*)(((const Fl_Menu_Item*)this)->next(i));
  const Fl_Menu_Item *first() const { return next(0); 
  Fl_Menu_Item *first() { return next(0); 

  // methods on menu items
  const char* label() const {return text;
    /** This is the title of the item. A \c NULL here indicates the end o
     * the menu (or of a submenu). A '&' in the item will print an
     * underscore under the next letter, and if the menu is popped up tha
     * letter will be a "shortcut" to pick that item. To get a real '&' put
     * two in a row. *
  void label(const char* a) {text=a;
  void label(Fl_Labeltype a,const char* b) {labeltype_ = a; text = b;
  Fl_Labeltype labeltype() const {return (Fl_Labeltype)labeltype_;
    /** A labeltype identifies a routine that draws the label of the widget
     * This can be used for special effects such as emboss, or to use th
     * label() pointer as another form of data such as a bitmap. The valu
     * \c FL_NORMAL_LABEL prints the label as text. *
  void labeltype(Fl_Labeltype a) {labeltype_ = a;
  Fl_Color labelcolor() const {return (Fl_Color)labelcolor_;
    /** This color is passed to the labeltype routine, and is typically th
     * color of the label text. This defaults to \c FL_BLACK. If this colo
     * is not black fltk will not use overlay bitplanes to draw the menu -
     * this is so that images put in the menu draw correctly. *
  void labelcolor(unsigned a) {labelcolor_ = a;
  Fl_Font labelfont() const {return (Fl_Font)labelfont_;
    /** Fonts are identified by small 8-bit indexes into a table. See th
     * enumeration list for predefined fonts. The default value is 
     * Helvetica font. The function <tt>Fl::set_font()</tt> can define
     * new fonts. *
  void labelfont(uchar a) {labelfont_ = a;
    /** Gets the label font pixel size/height. *
  uchar labelsize() const {return labelsize_;
    /** Sets the label font pixel size/height. *
  void labelsize(uchar a) {labelsize_ = a;
  Fl_Callback_p callback() const {return callback_;
    /** Each item has space for a callback function and an argument for
     * that function. Due to back compatability, the Fl_Menu_Item itself
     * is not passed to the callback, instead you have to get it by callin
     * <tt>((Fl_Menu_*)w)->mvalue()</tt> where w is the widget argument. *
  void callback(Fl_Callback* c, void* p) {callback_=c; user_data_=p;
  void callback(Fl_Callback* c) {callback_=c;
  void callback(Fl_Callback0*c) {callback_=(Fl_Callback*)c;
  void callback(Fl_Callback1*c, long p=0) {callback_=(Fl_Callback*)c; user_data_=(void*)p;
    /** Get the user_data argument that is sent to the callback function. *
  void* user_data() const {return user_data_;
    /** Set the user_data argument that is sent to the callback function. *
  void user_data(void* v) {user_data_ = v;
    /** For convenience you can also define the callback as taking a \c lon
     * argument. This is implemented by casting this to a Fl_Callback an
     * casting the \c long to a <tt>void*</tt> and may not be portable
     * to some machines. *
  long argument() const {return (long)user_data_;
  void argument(long v) {user_data_ = (void*)v;
  int shortcut() const {return shortcut_;
    /** Sets exactly what key combination will trigger the menu item. The
     * value is a logical 'or' of a key and a set of shift flags, for
     * instance <tt>FL_ALT+'a'</tt> or <tt>FL_ALT+FL_F+10</tt> or just 'a'
     * A value of zero disables the shortcut
     *
     * The key can be any value returned by Fl::event_key(), but will
     * usually be an ASCII letter. Use a lower-case letter unless you
     * require the shift key to be held down
     
     * The shift flags can be any set of values accepted by
     * Fl::event_state(). If the bit is on that shift key must be pushed.
     * \c Meta, \c Alt, \c Ctrl, and \c Shift must be off if they are not
     * in the shift flags (zero for the other bits indicates 
     * "don't care" setting). *
  void shortcut(int s) {shortcut_ = s;
    /** Returns true if either \c FL_SUBMENU or \c FL_SUBMENU_POINTER is o
     * in the flags. \c FL_SUBMENU indicates an embedded submenu that goe
     * from the next item through the next one with a \c NULL label().
     * \c FL_SUBMENU_POINTER indicates that user_data() is a pointer to
     * another menu array. *
  int submenu() const {return flags&(FL_SUBMENU|FL_SUBMENU_POINTER);
    /** Returns true if a checkbox will be drawn next to this item. This i
     * true if \c FL_MENU_TOGGLE or \c FL_MENU_RADIO is set in the flags. *
  int checkbox() const {return flags&FL_MENU_TOGGLE;
    /** Returns true if this item is a radio item. When a radio button i
     * selected all "adjacent" radio buttons are turned off. A set of radi
     * items is delimited by an item that has radio() false, or by an ite
     * with \c FL_MENU_DIVIDER turned on. *
  int radio() const {return flags&FL_MENU_RADIO;
    /** Returns the current value of the check or radio item. *
  int value() const {return flags&FL_MENU_VALUE;
    /** Turns the check or radio item "on" for the menu item. Note that thi
     * does not turn off any adjacent radio items like set_only() does. *
  void set() {flags |= FL_MENU_VALUE;
    /** Turns the check or radio item "off" for the menu item. *
  void clear() {flags &= ~FL_MENU_VALUE;
    /** Turns the radio item "on" for the menu item and turns off adjacent
     * radio item. *
  void setonly()
    /** Gets the visibility of an item. *
  int visible() const {return !(flags&FL_MENU_INVISIBLE);
    /** Makes an item visible in the menu. *
  void show() {flags &= ~FL_MENU_INVISIBLE;
    /** Hides an item in the menu. *
  void hide() {flags |= FL_MENU_INVISIBLE;
    /** Get whether or not the item can be picked. *
  int active() const {return !(flags&FL_MENU_INACTIVE);
    /** Allows a menu item to be picked. *
  void activate() {flags &= ~FL_MENU_INACTIVE;
    /** Prevents a menu item from being picked. Note that this will als
     * cause the menu item to appear grayed-out. *
  void deactivate() {flags |= FL_MENU_INACTIVE;
  int activevisible() const {return !(flags&0x11);

  // compatibility for FLUID so it can set the image of a menu item..
  void image(Fl_Image* a) {a->label(this);
  void image(Fl_Image& a) {a.label(this);

  // used by menubar
  int measure(int* h, const Fl_Menu_*) const
  void draw(int x, int y, int w, int h, const Fl_Menu_*, int t=0) const

  // popup menus without using an Fl_Menu_ widget
    /** This method is called by widgets that want to display menus. The
     * menu stays up until the user picks an item or dismisses it. Th
     * selected item (or \c NULL if none) is returned. This does not do th
     * callbacks or change the state of check or radio items.
     
     * \a X, \a Y is the position of the mouse cursor, relative to the
     * window that got the most recent event (usually you can pass
     * Fl::event_x() and Fl::event_y() unchanged here)
     
     * \a title is a character string title for the menu. If non-zero a
     * small box appears above the menu with the title in it
     
     * The menu is positioned so the cursor is centered over the item
     * picked. This will work even if picked is in a submenu. If picked i
     * zero or not in the menu item table the menu is positioned with th
     * cursor in the top-left corner
     
     * \a button is a pointer to an Fl_Menu_ from which the color an
     * boxtypes for the menu are pulled. If NULL then defaults are used. *
  const Fl_Menu_Item* popup
    int X, int Y
    const char *title = 0
    const Fl_Menu_Item* picked=0
    const Fl_Menu_* = 0) const
    /** pulldown() is similar to popup(), but a rectangle is provided t
     * position the menu. The menu is made at least \a W wide, and the
     * picked item is centered over the rectangle (like Fl_Choice uses)
     * If picked is zero or not found, the menu is aligned just below th
     * rectangle (like a pulldown menu)
     
     * The title and menubar arguments are used internally by the
     * Fl_Menu_Bar widget. *
  const Fl_Menu_Item* pulldown
    int X, int Y, int W, int H
    const Fl_Menu_Item* picked = 0
    const Fl_Menu_* = 0
    const Fl_Menu_Item* title = 0
    int menubar=0) const
    /** This is designed to be called by a widgets handle() method in
     * response to a \c FL_SHORTCUT event. If the current event matches one o
     * the items shortcut, that item is returned. If the keystroke does no
     * match any shortcuts then \c NULL is returned. This only matches th
     * shortcut() fields, not the letters in the title preceeded by '&' *
  const Fl_Menu_Item* test_shortcut() const
  const Fl_Menu_Item* find_shortcut(int *ip=0) const

    /** Call the Fl_Menu_Item item's callback, and provide the Fl_Widge
     * argument (and optionally override the user_data() argument). Yo
     * must first check that callback() is non-zero before calling this. *
  void do_callback(Fl_Widget* o) const {callback_(o, user_data_);
  void do_callback(Fl_Widget* o,void* arg) const {callback_(o, arg);
  void do_callback(Fl_Widget* o,long arg) const {callback_(o, (void*)arg);

  // back-compatability, do not use
  int checked() const {return flags&FL_MENU_VALUE;
  void check() {flags |= FL_MENU_VALUE;
  void uncheck() {flags &= ~FL_MENU_VALUE;
  int add(const char*, int shortcut, Fl_Callback*, void* =0, int = 0)
  int add(const char*a, const char* b, Fl_Callback* c
	  void* d = 0, int e = 0) 
    return add(a,fl_old_shortcut(b),c,d,e);
    /** Returns the number of Fl_Menu_Item structures that make up this
     * menu, correctly counting submenus. This includes the "terminator"
     * item at the end. So to copy a menu you need to copy
     * <tt>size()*sizeof(Fl_Menu_Item)</tt> bytes. *
  int size() const 
}

typedef Fl_Menu_Item Fl_Menu; // back compatabilit

enum {	// back-compatability enum
  FL_PUP_NONE	= 0
  FL_PUP_GREY	= FL_MENU_INACTIVE
  FL_PUP_GRAY	= FL_MENU_INACTIVE
  FL_MENU_BOX	= FL_MENU_TOGGLE
  FL_PUP_BOX	= FL_MENU_TOGGLE
  FL_MENU_CHECK	= FL_MENU_VALUE
  FL_PUP_CHECK	= FL_MENU_VALUE
  FL_PUP_RADIO	= FL_MENU_RADIO
  FL_PUP_INVISIBLE = FL_MENU_INVISIBLE
  FL_PUP_SUBMENU = FL_SUBMENU_POINTE
}

#endi

/
// End of "$Id$"
/
