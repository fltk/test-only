//
// "$Id: Fl_Menu_Item.H,v 1.43 2000/06/10 04:10:19 carl Exp $"
//
// Back-compatability menu items for FLTK.  The new Fl_Menu_ class
// converts these tables into child Fl_Item and Fl_Group_Item widgets.
// You should not use this for new programs.
//
// Copyright 1998-2000 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef FLTK_2
#ifndef Fl_Menu_Item_H
#define Fl_Menu_Item_H

#ifndef Fl_Widget_H
// used to get the Fl_Callback typedefs:
#include "Fl_Widget.H"
#endif

enum { // values for flags:
  FL_MENU_INACTIVE = 1,
  FL_MENU_TOGGLE= 2,
  FL_MENU_VALUE = 4,
  FL_MENU_RADIO = 8,
  FL_MENU_INVISIBLE = 0x10,
  FL_SUBMENU_POINTER = 0x20,
  FL_SUBMENU = 0x40,
  FL_MENU_DIVIDER = 0x80,
  FL_MENU_HORIZONTAL = 0x100 // not used!
};

//extern FL_API int fl_old_shortcut(const char*);

class FL_API Fl_Menu_;

struct FL_API Fl_Menu_Item {
  const char *text;
  int shortcut_;
  Fl_Callback *callback_;
  void *user_data_;
  int flags;
  Fl_Labeltype labeltype_;
  Fl_Font labelfont_;
  unsigned labelsize_;
  Fl_Color labelcolor_;

  // popup menus without using an Fl_Menu_ widget:
  // Warning: this is now quite expensive, as it creates a temporary
  // Fl_Menu_ and child widgets!  These also do not match the fltk 1.0
  // calls very much.  This does the callback directly, in fltk 1.0 the
  // menu item was retruned and you had to do the callback yourself.
  int popup(int X, int Y, const char *title=0, void* user_data = 0) const;
  int test_shortcut(void* user_data = 0) const;

  // return offset of terminating null item:
  int size() const ;

  // advance a pointer by N items, skipping submenus:
  const Fl_Menu_Item *next(int=1) const;
  Fl_Menu_Item *next(int i=1) {
    return (Fl_Menu_Item*)(((const Fl_Menu_Item*)this)->next(i));}

  const char* label() const {return text;}
  Fl_Labeltype labeltype() const {return labeltype_;}
  Fl_Color labelcolor() const {return labelcolor_;}
  Fl_Font labelfont() const {return labelfont_;}
  unsigned labelsize() const {return labelsize_;}
  Fl_Callback_p callback() const {return callback_;}
  void* user_data() const {return user_data_;}
  long argument() const {return (long)user_data_;}
  int shortcut() const {return shortcut_;}
  int submenu() const {return flags&(FL_SUBMENU|FL_SUBMENU_POINTER);}
  int checkbox() const {return flags&FL_MENU_TOGGLE;}
  int radio() const {return flags&FL_MENU_RADIO;}
  int value() const {return flags&FL_MENU_VALUE;}
  int visible() const {return !(flags&FL_MENU_INVISIBLE);}
  int active() const {return !(flags&FL_MENU_INACTIVE);}
  int activevisible() const {return !(flags&0x11);}

#if 0
  // The rest of these fltk 1.0 methods are not emulated currently.
  // Since the menu items are copied to the Fl_Menu_ widgets these
  // will only change the originals and will not produce the expected
  // results:

  void label(const char* a) {text=a;}
  void label(Fl_Labeltype a,const char* b) {labeltype_ = a; text = b;}
  void labeltype(Fl_Labeltype a) {labeltype_ = a;}
  void labelcolor(uchar a) {labelcolor_ = a;}
  void labelfont(uchar a) {labelfont_ = a;}
  void labelsize(uchar a) {labelsize_ = a;}
  void callback(Fl_Callback* c, void* p) {callback_=c; user_data_=p;}
  void callback(Fl_Callback* c) {callback_=c;}
  void callback(Fl_Callback0*c) {callback_=(Fl_Callback*)c;}
  void callback(Fl_Callback1*c, long p=0) {callback_=(Fl_Callback*)c; user_data_=(void*)p;}
  void user_data(void* v) {user_data_ = v;}
  void argument(long v) {user_data_ = (void*)v;}
  void shortcut(int s) {shortcut_ = s;}
  void set() {flags |= FL_MENU_VALUE;}
  void clear() {flags &= ~FL_MENU_VALUE;}
  void setonly();
  void show() {flags &= ~FL_MENU_INVISIBLE;}
  void hide() {flags |= FL_MENU_INVISIBLE;}
  void activate() {flags &= ~FL_MENU_INACTIVE;}
  void deactivate() {flags |= FL_MENU_INACTIVE;}

  // used by menubar:
  int measure(int* h, const Fl_Menu_*) const;
  void draw(int x, int y, int w, int h, const Fl_Menu_*, int t=0) const;

  const Fl_Menu_Item* find_shortcut(int *ip=0) const;

  void do_callback(Fl_Widget* o) const {callback_(o, user_data_);}
  void do_callback(Fl_Widget* o,void* arg) const {callback_(o, arg);}
  void do_callback(Fl_Widget* o,long arg) const {callback_(o, (void*)arg);}

  // back-compatability, do not use:
  int checked() const {return flags&FL_MENU_VALUE;}
  void check() {flags |= FL_MENU_VALUE;}
  void uncheck() {flags &= ~FL_MENU_VALUE;}
  int add(const char*, int shortcut, Fl_Callback*, void* =0, int = 0);
  int add(const char*a, const char* b, Fl_Callback* c,
	  void* d = 0, int e = 0) {
    return add(a,fl_old_shortcut(b),c,d,e);}
#endif
};

typedef Fl_Menu_Item Fl_Menu; // back compatability with fltk < 1.0

enum {	// back-compatability enum:
  FL_PUP_NONE	= 0,
  FL_PUP_GREY	= FL_MENU_INACTIVE,
  FL_PUP_GRAY	= FL_MENU_INACTIVE,
  FL_MENU_BOX	= FL_MENU_TOGGLE,
  FL_PUP_BOX	= FL_MENU_TOGGLE,
  FL_MENU_CHECK	= FL_MENU_VALUE,
  FL_PUP_CHECK	= FL_MENU_VALUE,
  FL_PUP_RADIO	= FL_MENU_RADIO,
  FL_PUP_INVISIBLE = FL_MENU_INVISIBLE,
  FL_PUP_SUBMENU = FL_SUBMENU_POINTER
};

#endif
#endif

//
// End of "$Id: Fl_Menu_Item.H,v 1.43 2000/06/10 04:10:19 carl Exp $".
//
