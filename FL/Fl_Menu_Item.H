//
// "$Id: Fl_Menu_Item.H,v 1.16 1999/04/01 21:32:39 carl Exp $"
//
// Menu item header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

#ifndef Fl_Menu_Item_H
#define Fl_Menu_Item_H

#ifndef Fl_Widget_H
// used to get the Fl_Callback typedefs:
#include "Fl_Widget.H"
#endif

#define MENU_ITEM_STYLE ((Fl_Menu_Item::Style*)_style)

enum { // values for flags:
  FL_MENU_INACTIVE = 1,
  FL_MENU_TOGGLE= 2,
  FL_MENU_VALUE = 4,
  FL_MENU_RADIO = 8,
  FL_MENU_INVISIBLE = 0x10,
  FL_SUBMENU_POINTER = 0x20,
  FL_SUBMENU = 0x40,
  FL_MENU_DIVIDER = 0x80,
  FL_MENU_HORIZONTAL = 0x100,
  FL_MENU_STYLES_LOADED = 0x200
};

extern int fl_old_shortcut(const char*);

class Fl_Menu_;

struct Fl_Menu_Item {
  enum Attribute {
    LABELCOLOR = 0,
    LABELSIZE,
    LABELTYPE,
    LABELFONT,
    DOWN_LABELCOLOR,
    DOWN_COLOR,
    DOWN_BOX,
    TITLE_DOWN_COLOR,
    TITLE_DOWN_LABELCOLOR,
    TITLE_DOWN_BOX,
    TITLE_FLY_COLOR,
    TITLE_FLY_BOX,
    TITLE_FLY_LABELCOLOR,
    MIA_END
  };

  struct Style {
    uchar menu_item_[MIA_END];
    uchar sbf;

    Style();
    uchar& menu_item(Attribute a) { return menu_item_[a]; }
  };

  static Fl_Menu_Item::Style* _default_style;

  const char *text;	// label()
  int shortcut_;
  Fl_Callback *callback_;
  void *user_data_;
  int flags;

  // Dummy variables to handle static initializers.  These are not used--
  // you can't initialize style information with the initializers anymore.
  uchar clobberMe1;
  uchar clobberMe2;
  uchar clobberMe3;
  uchar clobberMe4;

  Style* _style;

#ifdef BROKEN_SGI_COMPILER
  Fl_Menu_Item::Style _dynamic_style;
#else
  ~Fl_Menu_Item();
#endif

  Style* style() const { if (_style) return _style; return default_style(); }

  int is_style_forced(Attribute a) const
    { if (!_style) return 0; return ((Style*)_style)->sbf & bf(a); }

  void copy_style(Fl_Menu_Item* o)
    { mstyle(&_style); *((Style*)_style) = *((Style*)o->style()); }

  void loadstyle() const;
  uchar bf(uchar a) const {return (uchar)1 << a;}

  // advance N items, skipping submenus:
  const Fl_Menu_Item *next(int=1) const;
  Fl_Menu_Item *next(int i=1) {
    return (Fl_Menu_Item*)(((const Fl_Menu_Item*)this)->next(i));}

  // methods on menu items:
  const char* label() const {return text;}
  void label(const char* a) {text=a;}

  Fl_Labeltype labeltype() const;
  Fl_Color labelcolor() const;
  Fl_Font labelfont() const;
  uchar labelsize() const;
  Fl_Boxtype down_box() const;
  Fl_Color down_color() const;
  Fl_Color down_labelcolor() const;

  void label(Fl_Labeltype a,const char* b);
  void labeltype(Fl_Labeltype a);
  void labelcolor(uchar a);
  void labelfont(uchar a);
  void labelsize(uchar a);
  void down_box(uchar b);
  void down_color(uchar c);
  void down_labelcolor(uchar c);

  Fl_Callback_p callback() const {return callback_;}

  void callback(Fl_Callback* c, void* p) {callback_=c; user_data_=p;}
  void callback(Fl_Callback* c) {callback_=c;}
  void callback(Fl_Callback0*c) {callback_=(Fl_Callback*)c;}
  void callback(Fl_Callback1*c, long p=0) {callback_=(Fl_Callback*)c; user_data_=(void*)p;}
  void* user_data() const {return user_data_;}
  void user_data(void* v) {user_data_ = v;}
  long argument() const {return (long)user_data_;}
  void argument(long v) {user_data_ = (void*)v;}
  int shortcut() const {return shortcut_;}
  void shortcut(int s) {shortcut_ = s;}
  int submenu() const {return flags&(FL_SUBMENU|FL_SUBMENU_POINTER);}
  int checkbox() const {return flags&FL_MENU_TOGGLE;}
  int radio() const {return flags&FL_MENU_RADIO;}
  int value() const {return flags&FL_MENU_VALUE;}
  void set() {flags |= FL_MENU_VALUE;}
  void clear() {flags &= ~FL_MENU_VALUE;}
  void setonly();
  int visible() const {return !(flags&FL_MENU_INVISIBLE);}
  void show() {flags &= ~FL_MENU_INVISIBLE;}
  void hide() {flags |= FL_MENU_INVISIBLE;}
  int active() const {return !(flags&FL_MENU_INACTIVE);}
  void activate() {flags &= ~FL_MENU_INACTIVE;}
  void deactivate() {flags |= FL_MENU_INACTIVE;}
  int activevisible() const {return !(flags&0x11);}

  // used by menubar:
  int measure(int* h, const Fl_Menu_*) const;
  void draw(int x, int y, int w, int h, const Fl_Menu_*, int t=0) const;

  // popup menus without using an Fl_Menu_ widget:
  const Fl_Menu_Item* popup(
    int X, int Y,
    const char *title = 0,
    const Fl_Menu_Item* picked=0,
    const Fl_Menu_* = 0);
  const Fl_Menu_Item* pulldown(
    int X, int Y, int W, int H,
    const Fl_Menu_Item* picked = 0,
    const Fl_Menu_* = 0,
    const Fl_Menu_Item* title = 0,
    int menubar=0);
  const Fl_Menu_Item* test_shortcut() const;
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
  int size() const ;

  void modify_default_style() {
    if (_style && _style != default_style()) delete _style;
    _style = default_style();
  }

  Fl_Menu_Item::Style* default_style() const { mstyle(&_default_style); return _default_style; }
#ifndef BROKEN_SGI_COMPILER
  void mstyle(Fl_Menu_Item::Style** s) const { if (!(*s)) (*s) = new Style; }
#else
  void mstyle(Fl_Menu_Item::Style** s) const { if (!(*s)) (*s) = (Style*)&_dynamic_style; }
#endif
};

typedef Fl_Menu_Item Fl_Menu; // back compatability

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

//
// End of "$Id: Fl_Menu_Item.H,v 1.16 1999/04/01 21:32:39 carl Exp $".
//
