//
// "$Id$"
//

#ifndef Fltk1_Menu__H
#define Fltk1_Menu__H

#include <fltk3/Menu_.h>

#include "Fl_Widget.H"
#include "Fl_Menu_Item.H"

class FL_EXPORT Fl_Menu_ : public Fl_Widget {
protected:
  Fl_Menu_() {}
  
public:
  Fl_Menu_(int x, int y, int w, int h, const char *l=0) {
    _p = new fltk3::Menu_(x, y, w, h, l);
    _p->wrapper(this);
  }
#if 0 // fltk123:
  ~Fl_Menu_();
  int item_pathname(char *name, int namelen, const Fl_Menu_Item *finditem=0) const;
  const Fl_Menu_Item* picked(const Fl_Menu_Item*);
  const Fl_Menu_Item* find_item(const char *name);
  const Fl_Menu_Item* find_item(Fl_Callback*);
  const Fl_Menu_Item* test_shortcut() {return picked(menu()->test_shortcut());}
  void global();
  const Fl_Menu_Item *menu() const {return menu_;}
#endif
  void menu(const Fl_Menu_Item *m) { ((fltk3::Menu_*)_p)->menu((fltk3::MenuItem*)m); }
#if 0 // fltk123:
  void copy(const Fl_Menu_Item *m, void* user_data = 0);
  int  add(const char*, int shortcut, Fl_Callback*, void* = 0, int = 0);
  int  add(const char* a, const char* b, Fl_Callback* c, void* d = 0, int e = 0) {
      return add(a,fl_old_shortcut(b),c,d,e);}
  int  add(const char *);
  int  size() const ;
  void size(int W, int H) { fltk3::Widget::size(W, H); }
  void clear();
  void replace(int,const char *);
  void remove(int);
  void shortcut(int i, int s) {menu_[i].shortcut(s);}
  void mode(int i,int fl) {menu_[i].flags = fl;}
  int  mode(int i) const {return menu_[i].flags;}
  const Fl_Menu_Item *mvalue() const {return value_;}
  int value() const {return value_ ? (int)(value_-menu_) : -1;}
  int value(const Fl_Menu_Item*);
  int value(int i) {return value(menu_+i);}
  const char *text() const {return value_ ? value_->text : 0;}
  const char *text(int i) const {return menu_[i].text;}
  Fl_Font textfont() const {return textfont_;}
  void textfont(Fl_Font c) {textfont_=c;}
  Fl_Fontsize textsize() const {return textsize_;}
  void textsize(Fl_Fontsize c) {textsize_=c;}
  Fl_Color textcolor() const {return textcolor_;}
  void textcolor(Fl_Color c) {textcolor_=c;}
  Fl_Boxtype down_box() const {return (Fl_Boxtype)down_box_;}
  void down_box(Fl_Boxtype b) {down_box_ = b;}
  Fl_Color down_color() const {return selection_color();}
  void down_color(unsigned c) {selection_color(c);}
#endif
};

#endif

//
// End of "$Id$".
//
