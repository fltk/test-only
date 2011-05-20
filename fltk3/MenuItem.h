//
// "$Id: MenuItem.h 7983 2010-12-09 00:04:06Z AlbrechtS $"
//
// Menu item header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

#ifndef Fltk3_Menu_Item_H
#define Fltk3_Menu_Item_H

#  include "Widget.h"
#  include "Image.h"

#  if defined(__APPLE__) && defined(check)
#    undef check
#  endif


namespace fltk3 {
  
  class Menu_;
  
  enum { // values for flags:
    MENU_INACTIVE = 1,		///< Deactivate menu item (gray out)
    MENU_TOGGLE= 2,		///< Item is a checkbox toggle (shows checkbox for on/off state)
    MENU_VALUE = 4,		///< The on/off state for checkbox/radio buttons (if set, state is 'on')
    MENU_RADIO = 8,		///< Item is a radio button (one checkbox of many can be on)
    MENU_INVISIBLE = 0x10,	///< Item will not show up (shortcut will work)
    SUBMENU_POINTER = 0x20,	///< Indicates user_data() is a pointer to another menu array
    SUBMENU = 0x40,		///< This item is a submenu to other items
    MENU_DIVIDER = 0x80,	///< Creates divider line below this item. Also ends a group of radio buttons.
    MENU_HORIZONTAL = 0x100	///< ??? -- reserved
  };
  
  extern FLTK3_EXPORT Fl_Shortcut old_shortcut(const char*);
    
  /**
   The fltk3::MenuItem structure defines a single menu item that
   is used by the fltk3::Menu_ class.  
   \code
   struct fltk3::MenuItem {
   const char*		text;     // label()
   ulong		shortcut_;
   fltk3::Callback*		callback_;
   void*		user_data_;
   int			flags;
   uchar		labeltype_;
   uchar		labelfont_;
   uchar		labelsize_;
   uchar		labelcolor_;
   };
   
   enum { // values for flags:
   fltk3::MENU_INACTIVE	= 1,      // Deactivate menu item (gray out)
   fltk3::MENU_TOGGLE	= 2,      // Item is a checkbox toggle (shows checkbox for on/off state)
   fltk3::MENU_VALUE	= 4,      // The on/off state for checkbox/radio buttons (if set, state is 'on')
   fltk3::MENU_RADIO	= 8,      // Item is a radio button (one checkbox of many can be on)
   fltk3::MENU_INVISIBLE	= 0x10,   // Item will not show up (shortcut will work)
   fltk3::SUBMENU_POINTER	= 0x20,   // Indicates user_data() is a pointer to another menu array
   fltk3::SUBMENU		= 0x40,   // This item is a submenu to other items
   fltk3::MENU_DIVIDER	= 0x80,   // Creates divider line below this item. Also ends a group of radio buttons.
   fltk3::MENU_HORIZONTAL	= 0x100   // ??? -- reserved
   };
   \endcode
   Typically menu items are statically defined; for example:
   \code
   fltk3::MenuItem popup[] = {
   {"&alpha",   fltk3::ALT+'a', the_cb, (void*)1},
   {"&beta",    fltk3::ALT+'b', the_cb, (void*)2},
   {"gamma",    fltk3::ALT+'c', the_cb, (void*)3, fltk3::MENU_DIVIDER},
   {"&strange",  0,   strange_cb},
   {"&charm",    0,   charm_cb},
   {"&truth",    0,   truth_cb},
   {"b&eauty",   0,   beauty_cb},
   {"sub&menu",  0,   0, 0, fltk3::SUBMENU},
   {"one"},
   {"two"},
   {"three"},
   {0},
   {"inactive", fltk3::ALT+'i', 0, 0, fltk3::MENU_INACTIVE|fltk3::MENU_DIVIDER},
   {"invisible",fltk3::ALT+'i', 0, 0, fltk3::MENU_INVISIBLE},
   {"check",    fltk3::ALT+'i', 0, 0, fltk3::MENU_TOGGLE|fltk3::MENU_VALUE},
   {"box",      fltk3::ALT+'i', 0, 0, fltk3::MENU_TOGGLE},
   {0}};
   \endcode
   produces:
   
   \image html   menu.png
   \image latex  menu.png "menu" width=10cm
   
   A submenu title is identified by the bit fltk3::SUBMENU in the 
   flags field, and ends with a label() that is NULL.
   You can nest menus to any depth.  A pointer to the first item in the
   submenu can be treated as an Fl_Menu array itself.  It is also
   possible to make separate submenu arrays with fltk3::SUBMENU_POINTER flags.
   
   You should use the method functions to access structure members and
   not access them directly to avoid compatibility problems with future
   releases of FLTK.
   */
  struct FLTK3_EXPORT MenuItem {
    const char *text;	    ///< menu item text, returned by label()
    int shortcut_;	    ///< menu item shortcut
    fltk3::Callback *callback_;   ///< menu item callback
    void *user_data_;	    ///< menu item user_data for the menu's callback
    int flags;		    ///< menu item flags like fltk3::MENU_TOGGLE, fltk3::MENU_RADIO
    uchar labeltype_;	    ///< how the menu item text looks like
    fltk3::Font labelfont_;	    ///< which font for this menu item text
    fltk3::Fontsize labelsize_;   ///< size of menu item text
    fltk3::Color labelcolor_;	    ///< menu item text color
    
    // advance N items, skipping submenus:
    const fltk3::MenuItem *next(int=1) const;
    
    /**
     Advances a pointer by n items through a menu array, skipping
     the contents of submenus and invisible items. There are two calls so
     that you can advance through const and non-const data.
     */
    fltk3::MenuItem *next(int i=1) {
      return (fltk3::MenuItem*)(((const fltk3::MenuItem*)this)->next(i));}
    
    /** Returns the first menu item, same as next(0). */
    const fltk3::MenuItem *first() const { return next(0); }
    
    /** Returns the first menu item, same as next(0). */
    fltk3::MenuItem *first() { return next(0); }
    
    // methods on menu items:
    /**
     Returns the title of the item.
     A NULL here indicates the end of the menu (or of a submenu).
     A '&' in the item will print an underscore under the next letter,
     and if the menu is popped up that letter will be a "shortcut" to pick
     that item.  To get a real '&' put two in a row.
     */
    const char* label() const {return text;}
    
    /**    See const char* fltk3::MenuItem::label() const   */
    void label(const char* a) {text=a;}
    
    /**    See const char* fltk3::MenuItem::label() const   */
    void label(fltk3::Labeltype a,const char* b) {labeltype_ = a; text = b;}
    
    /**
     Returns the menu item's labeltype.
     A labeltype identifies a routine that draws the label of the
     widget.  This can be used for special effects such as emboss, or to use
     the label() pointer as another form of data such as a bitmap.
     The value fltk3::NORMAL_LABEL prints the label as text.
     */
    fltk3::Labeltype labeltype() const {return (fltk3::Labeltype)labeltype_;}
    
    /**
     Sets the menu item's labeltype.
     A labeltype identifies a routine that draws the label of the
     widget.  This can be used for special effects such as emboss, or to use
     the label() pointer as another form of data such as a bitmap.
     The value fltk3::NORMAL_LABEL prints the label as text.
     */
    void labeltype(fltk3::Labeltype a) {labeltype_ = a;}
    
    /**
     Gets the menu item's label color.
     This color is passed to the labeltype routine, and is typically the
     color of the label text.  This defaults to fltk3::BLACK.  If this
     color is not black fltk will \b not use overlay bitplanes to draw
     the menu - this is so that images put in the menu draw correctly.
     */
    fltk3::Color labelcolor() const {return labelcolor_;}
    
    /**
     Sets the menu item's label color.
     \see fltk3::Color fltk3::MenuItem::labelcolor() const
     */
    void labelcolor(fltk3::Color a) {labelcolor_ = a;}
    /**
     Gets the menu item's label font.
     Fonts are identified by small 8-bit indexes into a table. See the
     enumeration list for predefined fonts. The default value is a
     Helvetica font. The function fltk3::set_font() can define new fonts.
     */
    fltk3::Font labelfont() const {return labelfont_;}
    
    /**
     Sets the menu item's label font.
     Fonts are identified by small 8-bit indexes into a table. See the
     enumeration list for predefined fonts. The default value is a
     Helvetica font.  The function fltk3::set_font() can define new fonts.
     */
    void labelfont(fltk3::Font a) {labelfont_ = a;}
    
    /** Gets the label font pixel size/height. */
    fltk3::Fontsize labelsize() const {return labelsize_;}
    
    /** Sets the label font pixel size/height.*/
    void labelsize(fltk3::Fontsize a) {labelsize_ = a;}
    
    /**
     Returns the callback function that is set for the menu item.
     Each item has space for a callback function and an argument for that
     function. Due to back compatibility, the fltk3::MenuItem itself
     is not passed to the callback, instead you have to get it by calling
     ((fltk3::Menu_*)w)->mvalue() where w is the widget argument.
     */
    fltk3::CallbackPtr callback() const {return callback_;}
    
    /**
     Sets the menu item's callback function and userdata() argument.
     \see fltk3::CallbackPtr Fl_MenuItem::callback() const
     */
    void callback(fltk3::Callback* c, void* p) {callback_=c; user_data_=p;}
    
    /**
     Sets the menu item's callback function.
     This method does not set the userdata() argument.
     \see fltk3::CallbackPtr Fl_MenuItem::callback() const
     */
    void callback(fltk3::Callback* c) {callback_=c;}
    
    /**
     Sets the menu item's callback function.
     This method does not set the userdata() argument.
     \see fltk3::CallbackPtr Fl_MenuItem::callback() const
     */
    void callback(fltk3::Callback0*c) {callback_=(fltk3::Callback*)c;}
    
    /**
     Sets the menu item's callback function and userdata() argument.
     This method does not set the userdata() argument.
     The argument \p is cast to void* and stored as the userdata()
     for the menu item's callback function.
     \see fltk3::CallbackPtr Fl_MenuItem::callback() const
     */
    void callback(fltk3::Callback1*c, long p=0) {callback_=(fltk3::Callback*)c; user_data_=(void*)p;}
    
    /**
     Gets the user_data() argument that is sent to the callback function.
     */
    void* user_data() const {return user_data_;}
    /**
     Sets the user_data() argument that is sent to the callback function.
     */
    void user_data(void* v) {user_data_ = v;}
    /**
     Gets the user_data() argument that is sent to the callback function.
     For convenience you can also define the callback as taking a long
     argument.  This method casts the stored userdata() argument to long
     and returns it as a \e long value.
     */
    long argument() const {return (long)(fl_intptr_t)user_data_;}
    /**
     Sets the user_data() argument that is sent to the callback function.
     For convenience you can also define the callback as taking a long
     argument.  This method casts the given argument \p v to void*
     and stores it in the menu item's userdata() member.
     This may not be portable to some machines.
     */
    void argument(long v) {user_data_ = (void*)v;}
    
    /** Gets what key combination shortcut will trigger the menu item. */
    int shortcut() const {return shortcut_;}
    
    /**
     Sets exactly what key combination will trigger the menu item.  The
     value is a logical 'or' of a key and a set of shift flags, for instance 
     fltk3::ALT+'a' or fltk3::ALT+fltk3::FKey+10 or just 'a'.  A value of
     zero disables the shortcut.
     
     The key can be any value returned by fltk3::event_key(), but will usually 
     be an ASCII letter. Use a lower-case letter unless you require the shift 
     key to be held down.
     
     The shift flags can be any set of values accepted by fltk3::event_state().
     If the bit is on that shift key must be pushed.  Meta, Alt, Ctrl, 
     and Shift must be off if they are not in the shift flags (zero for the 
     other bits indicates a "don't care" setting).
     */
    void shortcut(int s) {shortcut_ = s;}
    /**
     Returns true if either fltk3::SUBMENU or fltk3::SUBMENU_POINTER
     is on in the flags. fltk3::SUBMENU indicates an embedded submenu
     that goes from the next item through the next one with a NULL
     label(). fltk3::SUBMENU_POINTER indicates that user_data()
     is a pointer to another menu array.
     */
    int submenu() const {return flags&(SUBMENU|SUBMENU_POINTER);}
    /**
     Returns true if a checkbox will be drawn next to this item.
     This is true if fltk3::MENU_TOGGLE or fltk3::MENU_RADIO is set in the flags.
     */
    int checkbox() const {return flags&MENU_TOGGLE;}
    /**
     Returns true if this item is a radio item.
     When a radio button is selected all "adjacent" radio buttons are
     turned off.  A set of radio items is delimited by an item that has
     radio() false, or by an item with fltk3::MENU_DIVIDER turned on.
     */
    int radio() const {return flags&MENU_RADIO;}
    /** Returns the current value of the check or radio item. */
    int value() const {return flags&MENU_VALUE;}
    /**
     Turns the check or radio item "on" for the menu item. Note that this
     does not turn off any adjacent radio items like set_only() does.
     */
    void set() {flags |= MENU_VALUE;}
    
    /** Turns the check or radio item "off" for the menu item. */
    void clear() {flags &= ~MENU_VALUE;}
    
    void setonly();
    
    /** Gets the visibility of an item. */
    int visible() const {return !(flags&MENU_INVISIBLE);}
    
    /** Makes an item visible in the menu. */
    void show() {flags &= ~MENU_INVISIBLE;}
    
    /** Hides an item in the menu. */
    void hide() {flags |= MENU_INVISIBLE;}
    
    /** Gets whether or not the item can be picked. */
    int active() const {return !(flags&MENU_INACTIVE);}
    
    /** Allows a menu item to be picked. */
    void activate() {flags &= ~MENU_INACTIVE;}
    /**
     Prevents a menu item from being picked. Note that this will also cause
     the menu item to appear grayed-out.
     */
    void deactivate() {flags |= MENU_INACTIVE;}
    /** Returns non 0 if FL_INACTIVE and FL_INVISIBLE are cleared, 0 otherwise. */
    int activevisible() const {return !(flags & (MENU_INACTIVE|MENU_INVISIBLE));}
    
    // compatibility for FLUID so it can set the image of a menu item...
    
    /** compatibility api for FLUID, same as a->label(this) */
    void image(fltk3::Image* a) {a->label(this);}
    
    /** compatibility api for FLUID, same as a.label(this) */
    void image(fltk3::Image& a) {a.label(this);}
    
    // used by menubar:
    int measure(int* h, const fltk3::Menu_*) const;
    void draw(int x, int y, int w, int h, const fltk3::Menu_*, int t=0) const;
    
    // popup menus without using an fltk3::Menu_ widget:
    const fltk3::MenuItem* popup(
                                 int X, int Y,
                                 const char *title = 0,
                                 const fltk3::MenuItem* picked=0,
                                 const fltk3::Menu_* = 0) const;
    const fltk3::MenuItem* pulldown(
                                    int X, int Y, int W, int H,
                                    const fltk3::MenuItem* picked = 0,
                                    const fltk3::Menu_* = 0,
                                    const fltk3::MenuItem* title = 0,
                                    int menubar=0) const;
    const fltk3::MenuItem* test_shortcut() const;
    const fltk3::MenuItem* find_shortcut(int *ip=0, const bool require_alt = false) const;
    
    /**
     Calls the fltk3::MenuItem item's callback, and provides the fltk3::Widget argument.
     The callback is called with the stored user_data() as its second argument.
     You must first check that callback() is non-zero before calling this.
     */
    void do_callback(fltk3::Widget* o) const {callback_(o, user_data_);}
    
    /**
     Calls the fltk3::MenuItem item's callback, and provides the fltk3::Widget argument.
     This call overrides the callback's second argument with the given value \p arg.
     You must first check that callback() is non-zero before calling this.
     */
    void do_callback(fltk3::Widget* o,void* arg) const {callback_(o, arg);}
    
    /**
     Calls the fltk3::MenuItem item's callback, and provides the fltk3::Widget argument.
     This call overrides the callback's second argument with the
     given value \p arg. long \p arg is cast to void* when calling
     the callback.
     You must first check that callback() is non-zero before calling this.
     */
    void do_callback(fltk3::Widget* o,long arg) const {callback_(o, (void*)arg);}
    
    // back-compatibility, do not use:
    
    /** back compatibility only \deprecated. */
    int checked() const {return flags&MENU_VALUE;}
    
    /** back compatibility only \deprecated. */
    void check() {flags |= MENU_VALUE;}
    
    /** back compatibility only \deprecated. */
    void uncheck() {flags &= ~MENU_VALUE;}
    
    int insert(int,const char*,int,fltk3::Callback*,void* =0, int =0);
    int add(const char*, int shortcut, fltk3::Callback*, void* =0, int = 0);
    
    /** See int add(const char*, int shortcut, fltk3::Callback*, void*, int) */
    int add(const char*a, const char* b, fltk3::Callback* c,
            void* d = 0, int e = 0) {
      return add(a,fltk3::old_shortcut(b),c,d,e);}
    
    int size() const ;
  };
  
}

#endif

//
// End of "$Id: MenuItem.h 7983 2010-12-09 00:04:06Z AlbrechtS $".
//
