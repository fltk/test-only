//
// "$Id: Fl_Menu_Item.H,v 1.31 1999/11/05 21:43:38 carl Exp $"
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

enum { // values for flags:
  FL_MENU_INACTIVE	= 0x001,
  FL_MENU_TOGGLE	= 0x002,
  FL_MENU_VALUE		= 0x004,
  FL_MENU_RADIO		= 0x008,
  FL_MENU_INVISIBLE	= 0x010,
  FL_SUBMENU_POINTER	= 0x020,
  FL_SUBMENU		= 0x040,
  FL_MENU_DIVIDER	= 0x080,
  FL_MENU_HORIZONTAL	= 0x100,
  FL_MENU_STAYS_UP	= 0x200,
  FL_MENU_TITLE         = 0x400
};

extern FL_API int fl_old_shortcut(const char*);

class Fl_Menu_;
class Fl_Image;

struct FL_API Fl_Menu_Item {
  const char *text;	// label()
  int shortcut_;
  Fl_Callback *callback_;
  void *user_data_;
  unsigned flags_;

#ifndef FLTK_2
  // Fltk 1.0 had 4 extra fields here: labeltype, labelfont, labelsize,
  // labelcolor.  These are no longer supported with the style code and
  // must eventually be deleted from your initializers.
  // The following are dummy variables to handle those 4 extra fields.
  // They are not used-- you can't initialize style information with the
  // initializers anymore.
  uchar clobberMe1;
  uchar clobberMe2;
  uchar clobberMe3;
  uchar clobberMe4;
#endif

  Fl_Image* image_;
  const Fl_Style* style_;

#ifdef DONT_LEAK_MEMORY
  ~Fl_Menu_Item();
#endif

  // advance N items, skipping submenus:
  const Fl_Menu_Item *next(int=1) const;
  Fl_Menu_Item *next(int i=1) {
    return (Fl_Menu_Item*)(((const Fl_Menu_Item*)this)->next(i));}

  // methods on menu items:
  const char* label() const {return text;}
  void label(const char* a) {text=a;}
  Fl_Image* image() const {return image_;}
  void image(Fl_Image* a) {image_ = a;}
  void image(Fl_Image& a) {image_ = &a;}

  const Fl_Style* style() const;

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
  int submenu() const {return flags_&(FL_SUBMENU|FL_SUBMENU_POINTER);}
  int checkbox() const {return flags_&FL_MENU_TOGGLE;}
  int radio() const {return flags_&FL_MENU_RADIO;}
  int value() const {return flags_&FL_MENU_VALUE;}
  void value(int b) { if (b) set(); else clear(); }
  void set() {flags_ |= FL_MENU_VALUE;}
  void clear() {flags_ &= ~FL_MENU_VALUE;}
  void setonly();
  int visible() const {return !(flags_&FL_MENU_INVISIBLE);}
  void show() {flags_ &= ~FL_MENU_INVISIBLE;}
  void hide() {flags_ |= FL_MENU_INVISIBLE;}
  int active() const {return !(flags_&FL_MENU_INACTIVE);}
  void activate() {flags_ &= ~FL_MENU_INACTIVE;}
  void deactivate() {flags_ |= FL_MENU_INACTIVE;}
  int activevisible() const {return !(flags_&(FL_MENU_INACTIVE|FL_MENU_INVISIBLE));}
  Fl_Flags flags() const {return Fl_Flags(flags_);}

  // used by menubar:
  int measure(int* h, const Fl_Menu_*, int title) const;
  void draw(int x, int y, int w, int h, const Fl_Menu_*, int t=0, int draw_box = 1) const;

  // popup menus without using an Fl_Menu_ widget: (highly depreciated!)
  const Fl_Menu_Item* popup(
    int X, int Y,
    const char *title = 0,
    const Fl_Menu_Item* picked=0,
    const Fl_Menu_* = 0) const;
  const Fl_Menu_Item* pulldown(
    int X, int Y, int W, int H,
    const Fl_Menu_Item* picked = 0,
    const Fl_Menu_* = 0,
    const Fl_Menu_Item* title = 0,
    int menubar=0) const;
  const Fl_Menu_Item* test_shortcut() const;
  const Fl_Menu_Item* find_shortcut(int *ip=0) const;

  void do_callback(Fl_Widget* o) const {callback_(o, user_data_);}
  void do_callback(Fl_Widget* o,void* arg) const {callback_(o, arg);}
  void do_callback(Fl_Widget* o,long arg) const {callback_(o, (void*)arg);}

  int size() const ;
  static Fl_Style default_style;
  static Fl_Style title_style;

#ifndef FLTK_2
  // This is temporary and bad.  Setting a style will create a new Fl_Style
  // structure that is not deleted when the menu item is destroyed, because
  // that adds a destructor that causes C++ to produce exit code for static
  // menu item arrays.  Plan is to replace the whole Fl_Menu_Item design...

  Fl_Boxtype box()		const {return (Fl_Boxtype)getp((const void* const*)&style_->box);}
  Fl_Boxtype glyph_box()	const {return (Fl_Boxtype)getp((const void* const*)&style_->glyph_box);}
  Fl_Boxtype box2()	        const {return (Fl_Boxtype)getp((const void* const*)&style_->glyph_box);}
  Fl_Glyph glyph()		const {return (Fl_Glyph)getp((const void* const*)&style_->glyph);}
  Fl_Font label_font()		const {return (Fl_Font)getp((const void* const*)&style_->label_font);}
  Fl_Font text_font()		const {return (Fl_Font)getp((const void* const*)&style_->text_font);}
  Fl_Labeltype label_type()	const {return (Fl_Labeltype)getp((const void* const*)&style_->label_type);}
  Fl_Color color()		const {return (Fl_Color)geti((const unsigned*)&style_->color);}
  Fl_Color up_color()		const {return (Fl_Color)geti((const unsigned*)&style_->color);}
  Fl_Color label_color()	const {return (Fl_Color)geti((const unsigned*)&style_->label_color);}
  Fl_Color selection_color()	const {return (Fl_Color)geti((const unsigned*)&style_->selection_color);}
  Fl_Color selection_text_color() const {return (Fl_Color)geti((const unsigned*)&style_->selection_text_color);}
  Fl_Color on_color()		const {return (Fl_Color)geti((const unsigned*)&style_->selection_color);}
  Fl_Color off_color()		const {return (Fl_Color)geti((const unsigned*)&style_->off_color);}
  Fl_Color highlight_color()	const {return (Fl_Color)geti((const unsigned*)&style_->highlight_color);}
  Fl_Color highlight_label_color() const {return (Fl_Color)geti((const unsigned*)&style_->highlight_label_color);}
  Fl_Color text_color()		const {return (Fl_Color)geti((const unsigned*)&style_->text_color);}
  unsigned label_size()		const {return geti((const unsigned*)&style_->label_size);}
  unsigned text_size()		const {return geti((const unsigned*)&style_->text_size);}

  unsigned geti(const unsigned* a) const;
  void* getp(const void* const* a) const;
  void setp(const void* const *, const void*);
  void seti(const unsigned*, unsigned);

  void box(Fl_Boxtype a)	{setp((const void**)&style_->box, a);}
  void glyph_box(Fl_Boxtype a)	{setp((const void**)&style_->glyph_box, a);}
  void glyph(Fl_Glyph a)	{setp((const void**)&style_->glyph, (void*)a);}
  void label_font(Fl_Font a)	{setp((const void**)&style_->label_font, a);}
  void text_font(Fl_Font a)	{setp((const void**)&style_->text_font, a);}
  void label_type(Fl_Labeltype a) {setp((const void**)&style_->label_type, a);}
  void color(Fl_Color a)	{seti(&style_->color, a);}
  void label_color(Fl_Color a)	{seti(&style_->label_color, a);}
  void selection_color(Fl_Color a) {seti(&style_->selection_color, a);}
  void selection_text_color(Fl_Color a) {seti(&style_->selection_text_color, a);}
  void highlight_color(Fl_Color a) {seti(&style_->highlight_color, a);}
  void highlight_label_color(Fl_Color a) {seti(&style_->highlight_label_color, a);}
  void text_color(Fl_Color a)	{seti(&style_->text_color, a);}
  void label_size(unsigned a)	{seti(&style_->label_size, a);}
  void text_size(unsigned a)	{seti(&style_->text_size, a);}

  // even more back-comptability stuff:
  Fl_Boxtype down_box()		const {return FL_DOWN_BOX;}
  Fl_Boxtype fly_box()		const {return box();}
  Fl_Color down_color()		const {return selection_color();}
  Fl_Color color2()		const {return selection_color();}
  Fl_Color light_color()	const {return selection_color();}
  Fl_Labeltype labeltype()	const {return label_type();}
  Fl_Color labelcolor()		const {return label_color();}
  Fl_Color down_labelcolor()	const {return selection_text_color();}
  Fl_Font labelfont()		const {return label_font();}
  unsigned labelsize()		const {return label_size();}
  Fl_Color fly_color()		const {return highlight_color();}
  Fl_Font textfont()		const {return text_font();}
  unsigned textsize()		const {return text_size();}
  Fl_Color textcolor()		const {return text_color();}
  Fl_Color selected_textcolor()	const {return selection_text_color();}

  void down_box(Fl_Boxtype)	{}
  void fly_box(Fl_Boxtype)	{}
  void down_color(Fl_Color a)	{selection_color(a);}
  void color2(Fl_Color a)	{selection_color(a);}
  void light_color(Fl_Color a)	{selection_color(a);}
  void labeltype(Fl_Labeltype a) {label_type(a);}
  void labelcolor(Fl_Color a)	{label_color(a);}
  void down_labelcolor(Fl_Color a) {selection_text_color(a);}
  void labelfont(Fl_Font a)	{label_font(a);}
  void labelfont(unsigned a)	{label_font(fl_fonts+a);}
  void labelsize(unsigned a)	{label_size(a);}
  void fly_color(Fl_Color a)	{highlight_color(a);}
  void textfont(Fl_Font a)	{text_font(a);}
  void textfont(unsigned a)	{text_font(fl_fonts+a);}
  void textsize(unsigned a)	{text_size(a);}
  void textcolor(Fl_Color a)	{text_color(a);}
  void selected_textcolor(Fl_Color a) {selection_text_color(a);}

  int checked() const {return flags_&FL_MENU_VALUE;}
  void check() {flags_ |= FL_MENU_VALUE;}
  void uncheck() {flags_ &= ~FL_MENU_VALUE;}
  int add(const char*, int shortcut, Fl_Callback*, void* =0, int = 0);
  int add(const char*a, const char* b, Fl_Callback* c,void* d = 0,int e = 0) {
    return add(a,fl_old_shortcut(b),c,d,e);}
#endif

};

#ifndef FLTK_2
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

#endif

//
// End of "$Id: Fl_Menu_Item.H,v 1.31 1999/11/05 21:43:38 carl Exp $".
//
