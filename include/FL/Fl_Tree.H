//
// "$Id$"
//

#ifndef FL_TREE_H
#define FL_TREE_H

#include <fltk3/Tree.h>
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/fl_draw.H>

#include <FL/Fl_Tree_Item.H>
#include <FL/Fl_Tree_Prefs.H>

//////////////////////
// FL/Fl_Tree.H
//////////////////////
//
// Fl_Tree -- This file is part of the Fl_Tree widget for FLTK
// Copyright (C) 2009-2011 by Greg Ercolano.
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
// 123 Wrapper started
//  - TODO: 123 - many methods not yet wrapped!
//

enum Fl_Tree_Reason {
  FL_TREE_REASON_NONE=0,
  FL_TREE_REASON_SELECTED,
  FL_TREE_REASON_DESELECTED,
  FL_TREE_REASON_OPENED,
  FL_TREE_REASON_CLOSED
};


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Tree, Tree)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Tree, Tree)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Tree : public Fl_Group {

protected:

  // Fl_Scrollbar *_vscroll;
  
  void item_clicked(Fl_Tree_Item* val) {
    ((fltk3::Tree_I*)_p)->item_clicked( (fltk3::TreeItem*)val );
  }

  //void do_callback_for_item(Fl_Tree_Item* item, Fl_Tree_Reason reason) {

  //Fl_Tree_Item *next_visible_item(Fl_Tree_Item *start, int dir);

public:
  
  FLTK3_WIDGET_VCALLS(Fl_Tree, Tree)

  Fl_Tree(int X, int Y, int W, int H, const char *L=0) {
    _p = new fltk3::Tree_I(X, Y, W, H, L);
    _p->wrapper(this);
  }
  
  void root_label(const char *new_label) {
    ((fltk3::Tree*)_p)->root_label(new_label);
  }

  Fl_Tree_Item* root() {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->root();
  }
  
  Fl_Tree_Item *add(const char *path) {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->add(path);
  }
  
  Fl_Tree_Item* add(Fl_Tree_Item *item, const char *name) {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->add(item, name);
  }
  
  Fl_Tree_Item *insert_above(Fl_Tree_Item *above, const char *name) {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->insert_above((fltk3::TreeItem*)above, name);
  }

  Fl_Tree_Item* insert(Fl_Tree_Item *item, const char *name, int pos) {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->insert((fltk3::TreeItem*)item, name, pos);
  }

  int remove(Fl_Tree_Item *item) {
    return ((fltk3::Tree*)_p)->remove((fltk3::TreeItem*)item);
  }

  void clear() {
    ((fltk3::Tree*)_p)->clear();
  }

  void clear_children(Fl_Tree_Item *item) {
    ((fltk3::Tree*)_p)->clear_children((fltk3::TreeItem*)item);
  } 

  Fl_Tree_Item *find_item(const char *path) {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->find_item(path);
  }

  const Fl_Tree_Item *find_item(const char *path) const {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->find_item(path);
  }

  int item_pathname(char *pathname, int pathnamelen, const Fl_Tree_Item *item) const {
    return ((fltk3::Tree*)_p)->item_pathname(pathname, pathnamelen, (fltk3::TreeItem*)item);
  }

  const Fl_Tree_Item *find_clicked() const {
    return (const Fl_Tree_Item*)((fltk3::Tree*)_p)->find_clicked();
  }

  Fl_Tree_Item *item_clicked() {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->item_clicked();
  }

  Fl_Tree_Item *first() {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->first();
  }

  Fl_Tree_Item *next(Fl_Tree_Item *item=0) {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->next((fltk3::TreeItem*)item);
  }

  Fl_Tree_Item *prev(Fl_Tree_Item *item=0) {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->prev((fltk3::TreeItem*)item);
  }

  Fl_Tree_Item *last() {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->last();
  }

  Fl_Tree_Item *first_selected_item() {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->first_selected_item();
  }

  Fl_Tree_Item *next_selected_item(Fl_Tree_Item *item=0) {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->next_selected_item((fltk3::TreeItem*)item);
  }

  int open(Fl_Tree_Item *item, int docallback=1) {
    return ((fltk3::Tree*)_p)->open((fltk3::TreeItem*)item, docallback);
  }

  int open(const char *path, int docallback=1) {
    return ((fltk3::Tree*)_p)->open(path, docallback);
  }

  void open_toggle(Fl_Tree_Item *item, int docallback=1) {
    return ((fltk3::Tree*)_p)->open_toggle((fltk3::TreeItem*)item, docallback);
  }

  int close(Fl_Tree_Item *item, int docallback=1) {
    return ((fltk3::Tree*)_p)->close((fltk3::TreeItem*)item, docallback);
  }

  int close(const char *path, int docallback=1) {
    return ((fltk3::Tree*)_p)->open(path, docallback);
  }

  int is_open(Fl_Tree_Item *item) const {
    return ((fltk3::Tree*)_p)->is_open((fltk3::TreeItem*)item);
  }

  int is_open(const char *path) const {
    return ((fltk3::Tree*)_p)->is_open(path);
  }

  int is_close(Fl_Tree_Item *item) const {
    return ((fltk3::Tree*)_p)->is_close((fltk3::TreeItem*)item);
  }

  int is_close(const char *path) const {
    return ((fltk3::Tree*)_p)->is_close(path);
  }

  int select(Fl_Tree_Item *item, int docallback=1) {
    return ((fltk3::Tree*)_p)->select((fltk3::TreeItem*)item, docallback);
  }

  int select(const char *path, int docallback=1) {
    return ((fltk3::Tree*)_p)->select(path, docallback);
  }

  void select_toggle(Fl_Tree_Item *item, int docallback=1) {
    ((fltk3::Tree*)_p)->select_toggle((fltk3::TreeItem*)item, docallback);
  }

  int deselect(Fl_Tree_Item *item, int docallback=1) {
    return ((fltk3::Tree*)_p)->deselect((fltk3::TreeItem*)item, docallback);
  }

  int deselect(const char *path, int docallback=1) {
    return ((fltk3::Tree*)_p)->deselect(path, docallback);
  }

  int deselect_all(Fl_Tree_Item *item=0, int docallback=1) {
    return ((fltk3::Tree*)_p)->deselect_all((fltk3::TreeItem*)item, docallback);
  }

  int select_only(Fl_Tree_Item *selitem, int docallback=1) {
    return ((fltk3::Tree*)_p)->select_only((fltk3::TreeItem*)selitem, docallback);
  }

  int select_all(Fl_Tree_Item *item=0, int docallback=1) {
    return ((fltk3::Tree*)_p)->select_all((fltk3::TreeItem*)item, docallback);
  }

  void set_item_focus(Fl_Tree_Item *o) {
    ((fltk3::Tree*)_p)->set_item_focus((fltk3::TreeItem*)o);
  }

  int is_selected(Fl_Tree_Item *item) const {
    return ((fltk3::Tree*)_p)->is_selected((fltk3::TreeItem*)item);
  }

  int is_selected(const char *path) {
    return ((fltk3::Tree*)_p)->is_selected(path);
  }

  void show_self() {
    ((fltk3::Tree*)_p)->show_self();
  }

  Fl_Fontsize item_labelsize() const {
    return fltk3::_3to1_fontsize( ((fltk3::Tree*)_p)->item_labelsize() );
  }

  void item_labelsize(Fl_Fontsize val) {
    ((fltk3::Tree*)_p)->item_labelsize( fltk3::_1to3_fontsize(val) );
  }

  Fl_Font item_labelfont() const {
    return fltk3::_3to1_font( ((fltk3::Tree*)_p)->item_labelfont() );
  }

  void item_labelfont(Fl_Font val) {
    ((fltk3::Tree*)_p)->item_labelfont( fltk3::_1to3_font(val) );
  }

  Fl_Color item_labelfgcolor(void) const {
    return fltk3::_3to1_color( ((fltk3::Tree*)_p)->item_labelfgcolor() );
  }

  void item_labelfgcolor(Fl_Color val) {
    ((fltk3::Tree*)_p)->item_labelfgcolor( fltk3::_1to3_color(val) );
  }

  Fl_Color item_labelbgcolor(void) const {
    return fltk3::_3to1_color( ((fltk3::Tree*)_p)->item_labelbgcolor() );
  }

  void item_labelbgcolor(Fl_Color val) {
    ((fltk3::Tree*)_p)->item_labelbgcolor( fltk3::_1to3_color(val) );
  }

  Fl_Color connectorcolor() const {
    return fltk3::_3to1_color( ((fltk3::Tree*)_p)->connectorcolor() );
  }

  void connectorcolor(Fl_Color val) {
    ((fltk3::Tree*)_p)->connectorcolor( fltk3::_1to3_color(val) );
  }

  int marginleft() const {
    return ((fltk3::Tree*)_p)->marginleft();
  }

  void marginleft(int val) {
    ((fltk3::Tree*)_p)->marginleft(val);
  }

  int margintop() const {
    return ((fltk3::Tree*)_p)->margintop();
  }

  void margintop(int val) {
    ((fltk3::Tree*)_p)->margintop(val);
  }

  int openchild_marginbottom() const {
    return ((fltk3::Tree*)_p)->openchild_marginbottom();
  }

  void openchild_marginbottom(int val) {
    ((fltk3::Tree*)_p)->openchild_marginbottom(val);
  }

  int connectorwidth() const {
    return ((fltk3::Tree*)_p)->connectorwidth();
  }
  void connectorwidth(int val) {
    ((fltk3::Tree*)_p)->connectorwidth(val);
  }

  Fl_Image *usericon() const {
    return fltk3::_3to1_image( ((fltk3::Tree*)_p)->usericon() );
  }

  void usericon(Fl_Image *val) {
    ((fltk3::Tree*)_p)->usericon( fltk3::_1to3_image(val) );
  }

  Fl_Image *openicon() const {
    return fltk3::_3to1_image( ((fltk3::Tree*)_p)->openicon() );
  }

  void openicon(Fl_Image *val) {
    ((fltk3::Tree*)_p)->openicon( fltk3::_1to3_image(val) );
  }

  Fl_Image *closeicon() const {
    return fltk3::_3to1_image( ((fltk3::Tree*)_p)->closeicon() );
  }

  void closeicon(Fl_Image *val) {
    ((fltk3::Tree*)_p)->closeicon( fltk3::_1to3_image(val) );
  }

  int showcollapse() const {
    return ((fltk3::Tree*)_p)->showcollapse();
  }

  void showcollapse(int val) {
    ((fltk3::Tree*)_p)->showcollapse(val);
  }

  int showroot() const {
    return ((fltk3::Tree*)_p)->showroot();
  }

  void showroot(int val) {
    ((fltk3::Tree*)_p)->showroot(val);
  }

  Fl_Tree_Connector connectorstyle() const {
    return (Fl_Tree_Connector)((fltk3::Tree*)_p)->connectorstyle();
  }

  void connectorstyle(Fl_Tree_Connector val) {
    ((fltk3::Tree*)_p)->connectorstyle((fltk3::TreeConnector)val);
  }

  Fl_Tree_Sort sortorder() const {
    return (Fl_Tree_Sort)((fltk3::Tree*)_p)->sortorder();
  }

  void sortorder(Fl_Tree_Sort val) {
    ((fltk3::Tree*)_p)->sortorder( (fltk3::TreeSort)val );
  }

#if 0
  Fl_Boxtype selectbox() const {
  }
  void selectbox(Fl_Boxtype val) {
  }
#endif

  Fl_Tree_Select selectmode() const {
    return (Fl_Tree_Select)((fltk3::Tree*)_p)->selectmode();
  }

  void selectmode(Fl_Tree_Select val) {
    ((fltk3::Tree*)_p)->selectmode((fltk3::TreeSelect)val);
  }

  int displayed(Fl_Tree_Item *item) {
    return ((fltk3::Tree*)_p)->displayed((fltk3::TreeItem*)item);
  }

  void show_item(Fl_Tree_Item *item, int yoff){
    ((fltk3::Tree*)_p)->show_item((fltk3::TreeItem*)item, yoff);
  }

  void show_item(Fl_Tree_Item *item) {
    ((fltk3::Tree*)_p)->show_item((fltk3::TreeItem*)item);
  }

  void show_item_bottom(Fl_Tree_Item *item) {
    ((fltk3::Tree*)_p)->show_item_bottom((fltk3::TreeItem*)item);
  }

  void show_item_middle(Fl_Tree_Item *item) {
    ((fltk3::Tree*)_p)->show_item_middle((fltk3::TreeItem*)item);
  }

  void show_item_top(Fl_Tree_Item *item) {
    ((fltk3::Tree*)_p)->show_item_top((fltk3::TreeItem*)item);
  }

  void display(Fl_Tree_Item *item) {
    ((fltk3::Tree*)_p)->display((fltk3::TreeItem*)item);
  }

  int  vposition() const {
    return ((fltk3::Tree*)_p)->vposition();
  }

  void vposition(int ypos) {
    ((fltk3::Tree*)_p)->vposition(ypos);
  }

  int is_scrollbar(Fl_Widget *w) {
    return ((fltk3::Tree*)_p)->is_scrollbar(fltk3::_1to3_widget(w));
  }
  
  int scrollbar_size() const {
    return ((fltk3::Tree*)_p)->scrollbar_size();
  }
  
  void scrollbar_size(int size) {
    ((fltk3::Tree*)_p)->scrollbar_size(size);
  }   
  
  void callback_item(Fl_Tree_Item* item) {
    ((fltk3::Tree*)_p)->callback_item((fltk3::TreeItem*)item);
  }

  Fl_Tree_Item* callback_item() {
    return (Fl_Tree_Item*)((fltk3::Tree*)_p)->callback_item();
  }

  void callback_reason(Fl_Tree_Reason reason) {
    ((fltk3::Tree*)_p)->callback_reason((fltk3::TreeReason)reason);
  }

  Fl_Tree_Reason callback_reason() const {
    return (Fl_Tree_Reason)((fltk3::Tree*)_p)->callback_reason();
  }

  void load(class Fl_Preferences &p) {
    ((fltk3::Tree*)_p)->load((fltk3::Preferences&)p);
  }  

};

#endif /*FL_TREE_H*/

//
// End of "$Id$".
//
