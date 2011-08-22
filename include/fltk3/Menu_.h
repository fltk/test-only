//
// "$Id: Menu_.h 7903 2010-11-28 21:06:39Z matt $"
//
// Menu base class header file for the Fast Light Tool Kit (FLTK).
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

/* \file
 fltk3::Menu_ widget . */

#ifndef Fltk3_Menu__H
#define Fltk3_Menu__H

#include "Widget.h"
#include "MenuItem.h"


namespace fltk3 {
  
  
  /**
   Base class of all widgets that have a menu in FLTK.
   Currently FLTK provides you with 
   fltk3::MenuButton, fltk3::MenuBar, and fltk3::Choice.
   
   <P>The class contains a pointer to an array of structures of type  fltk3::MenuItem.  The
   array may either be supplied directly by the user program, or it may
   be "private": a dynamically allocated array managed by the fltk3::Menu_.
   */
  class FLTK3_EXPORT Menu_ : public fltk3::Widget {
    
    fltk3::MenuItem *menu_;
    const fltk3::MenuItem *value_;
    
  protected:
    
    uchar alloc;			// flag indicates if menu_ is a dynamic copy (=1) or not (=0)
    uchar down_box_;
    
  public:
    Menu_(int,int,int,int,const char * =0);
    ~Menu_();
    
    int item_pathname(char *name, int namelen, const fltk3::MenuItem *finditem=0) const;
    const fltk3::MenuItem* picked(const fltk3::MenuItem*);
    const fltk3::MenuItem* find_item(const char *name);
    const fltk3::MenuItem* find_item(fltk3::Callback*);
    int find_index(const char *name) const;
    int find_index(const fltk3::MenuItem *item) const;
    int find_index(fltk3::Callback *cb) const;
    
    /**
     Returns the menu item with the entered shortcut (key value).
     
     This searches the complete menu() for a shortcut that matches the
     entered key value.  It must be called for a fltk3::KEYBOARD or fltk3::SHORTCUT
     event.
     
     If a match is found, the menu's callback will be called.
     
     \return matched fltk3::MenuItem or NULL.
     */
    const fltk3::MenuItem* test_shortcut() {return picked(menu()->test_shortcut());}
    void global();
    
    /**
     Returns a pointer to the array of fltk3::MenuItems.  This will either be
     the value passed to menu(value) or the private copy.
     */  
    const fltk3::MenuItem *menu() const {return menu_;}
    void menu(const fltk3::MenuItem *m);
    void copy(const fltk3::MenuItem *m, void* user_data = 0);
    int insert(int index, const char*, unsigned int shortcut, fltk3::Callback*, void* = 0, int = 0);
    int  add(const char*, unsigned int shortcut, fltk3::Callback*, void* = 0, int = 0);
    /** See int fltk3::Menu_::add(const char* label, unsigned int shortcut, fltk3::Callback*, void *user_data=0, int flags=0) */
    int  add(const char* a, const char* b, fltk3::Callback* c, void* d = 0, int e = 0) {
      return add(a,fltk3::old_shortcut(b),c,d,e);
    }
    /** See int fltk3::Menu_::insert(const char* label, unsigned int shortcut, fltk3::Callback*, void *user_data=0, int flags=0) */
    int insert(int index, const char* a, const char* b, fltk3::Callback* c, void* d = 0, int e = 0) {
      return insert(index,a,fltk3::old_shortcut(b),c,d,e);
    }
    int  add(const char *);
    int  size() const ;
    void size(int W, int H) { Widget::size(W, H); }
    void clear();
    int clear_submenu(int index);
    void replace(int,const char *);
    void remove(int);
    /** Changes the shortcut of item i to n.  */
    void shortcut(int i, unsigned int s) {menu_[i].shortcut(s);}
    /** Sets the flags of item i.  For a list of the flags, see fltk3::MenuItem.  */
    void mode(int i,int fl) {menu_[i].flags = fl;}
    /** Gets the flags of item i.  For a list of the flags, see fltk3::MenuItem.  */
    int  mode(int i) const {return menu_[i].flags;}
    
    /** Returns a pointer to the last menu item that was picked.  */
    const fltk3::MenuItem *mvalue() const {return value_;}
    /** Returns the index into menu() of the last item chosen by the user.  It is zero initially. */
    int value() const {return value_ ? (int)(value_-menu_) : -1;}
    int value(const fltk3::MenuItem*);
    /**
     The value is the index into menu() of the last item chosen by
     the user.  It is zero initially.  You can set it as an integer, or set
     it with a pointer to a menu item.  The set routines return non-zero if
     the new value is different than the old one.
     */
    int value(int i) {return value(menu_+i);}
    /** Returns the title of the last item chosen.  */
    const char *text() const {return value_ ? value_->text : 0;}
    /** Returns the title of item i.  */
    const char *text(int i) const {return menu_[i].text;}
    
    /**
     This box type is used to surround the currently-selected items in the
     menus.  If this is fltk3::NO_BOX then it acts like 
     fltk3::THIN_UP_BOX and selection_color() acts like 
     fltk3::WHITE, for back compatibility.    
     */
    fltk3::Boxtype down_box() const {return (fltk3::Boxtype)down_box_;}
    /**    See fltk3::Boxtype fltk3::Menu_::down_box() const   */
    void down_box(fltk3::Boxtype b) {down_box_ = b;}
    
    /** For back compatibility, same as selection_color() */
    fltk3::Color down_color() const {return selection_color();}
    /** For back compatibility, same as selection_color() */
    void down_color(unsigned c) {selection_color(c);}
  };
  
}

#endif

//
// End of "$Id: Menu_.h 7903 2010-11-28 21:06:39Z matt $".
//
