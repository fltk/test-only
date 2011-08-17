//
// "$Id$"
//
// Widget type code for the Fast Light Tool Kit (FLTK).
//
// Each object described by Fluid is one of these objects.  They
// are all stored in a double-linked list.
//
// They "type" of the object is covered by the virtual functions.
// There will probably be a lot of these virtual functions.
//
// The type browser is also a list of these objects, but they
// are "factory" instances, not "real" ones.  These objects exist
// only so the "make" method can be called on them.  They are
// not in the linked list and are not written to files or
// copied or otherwise examined.
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

#include <fltk3/run.h>
#include <fltk3/ask.h>
#include <fltk3/Browser_.h>
#include <fltk3/draw.h>
#include <fltk3/filename.h>
#include <stdlib.h>
#include "../src/flstring.h"
#include <stdio.h>

#include "Fl_Type.h"
#include "workspace_panel.h"
#include "undo.h"

#include <fltk3/Pixmap.h>
#include "pixmaps/lock.xpm"
#include "pixmaps/protected.xpm"
//#include "pixmaps/unlock.xpm"

static fltk3::Pixmap	lock_pixmap(lock_xpm);
static fltk3::Pixmap	protected_pixmap(protected_xpm);
//static fltk3::Pixmap	unlock_pixmap(unlock_xpm);

#include "pixmaps/flWindow.xpm"
#include "pixmaps/flButton.xpm"
#include "pixmaps/flCheckButton.xpm"
#include "pixmaps/flRoundButton.xpm"
#include "pixmaps/flBox.xpm"
#include "pixmaps/flGroup.xpm"
#include "pixmaps/flFunction.xpm"
#include "pixmaps/flCode.xpm"
#include "pixmaps/flCodeBlock.xpm"
#include "pixmaps/flComment.xpm"
#include "pixmaps/flData.xpm"
#include "pixmaps/flDeclaration.xpm"
#include "pixmaps/flDeclarationBlock.xpm"
#include "pixmaps/flClass.xpm"
#include "pixmaps/flTabs.xpm"
#include "pixmaps/flInput.xpm"
#include "pixmaps/flChoice.xpm"
#include "pixmaps/flMenuitem.xpm"
#include "pixmaps/flMenubar.xpm"
#include "pixmaps/flSubmenu.xpm"
#include "pixmaps/flScroll.xpm"
#include "pixmaps/flTile.xpm"
#include "pixmaps/flWizard.xpm"
#include "pixmaps/flPack.xpm"
#include "pixmaps/flReturnButton.xpm"
#include "pixmaps/flLightButton.xpm"
#include "pixmaps/flRepeatButton.xpm"
#include "pixmaps/flMenuButton.xpm"
#include "pixmaps/flOutput.xpm"
#include "pixmaps/flTextDisplay.xpm"
#include "pixmaps/flTextEdit.xpm"
#include "pixmaps/flFileInput.xpm"
#include "pixmaps/flBrowser.xpm"
#include "pixmaps/flCheckBrowser.xpm"
#include "pixmaps/flFileBrowser.xpm"
#include "pixmaps/flClock.xpm"
#include "pixmaps/flHelp.xpm"
#include "pixmaps/flProgress.xpm"
#include "pixmaps/flSlider.xpm"
#include "pixmaps/flScrollBar.xpm"
#include "pixmaps/flValueSlider.xpm"
#include "pixmaps/flAdjuster.xpm"
#include "pixmaps/flCounter.xpm"
#include "pixmaps/flDial.xpm"
#include "pixmaps/flRoller.xpm"
#include "pixmaps/flValueInput.xpm"
#include "pixmaps/flValueOutput.xpm"
#include "pixmaps/flSpinner.xpm"
#include "pixmaps/flWidgetClass.xpm"
#include "pixmaps/flTree.xpm"
#include "pixmaps/flTable.xpm"
#include "pixmaps/flAppTarget.xpm"
#include "pixmaps/flLibTarget.xpm"
#include "pixmaps/flFile.xpm"
#include "pixmaps/flFolder.xpm"
#include "pixmaps/flFluidFile.xpm"
#include "pixmaps/flCodeFile.xpm"
#include "pixmaps/flMenuNone.xpm"
#include "pixmaps/flMenuMulti.xpm"
#include "pixmaps/flMenuAll.xpm"
#include "pixmaps/flWorkspace.xpm"
#include "pixmaps/flDependency.xpm"

static fltk3::Pixmap	window_pixmap(flWindow_xpm);
static fltk3::Pixmap	button_pixmap(flButton_xpm);
static fltk3::Pixmap	checkbutton_pixmap(flCheckButton_xpm);
static fltk3::Pixmap	roundbutton_pixmap(flRoundButton_xpm);
static fltk3::Pixmap	box_pixmap(flBox_xpm);
static fltk3::Pixmap	group_pixmap(flGroup_xpm);
static fltk3::Pixmap	function_pixmap(flFunction_xpm);
static fltk3::Pixmap	code_pixmap(flCode_xpm);
static fltk3::Pixmap	codeblock_pixmap(flCodeBlock_xpm);
static fltk3::Pixmap	comment_pixmap(flComment_xpm);
static fltk3::Pixmap	declaration_pixmap(flDeclaration_xpm);
static fltk3::Pixmap	declarationblock_pixmap(flDeclarationBlock_xpm);
static fltk3::Pixmap	class_pixmap(flClass_xpm);
static fltk3::Pixmap	tabs_pixmap(flTabs_xpm);
static fltk3::Pixmap	input_pixmap(flInput_xpm);
static fltk3::Pixmap	choice_pixmap(flChoice_xpm);
static fltk3::Pixmap	menuitem_pixmap(flMenuitem_xpm);
static fltk3::Pixmap	menubar_pixmap(flMenubar_xpm);
static fltk3::Pixmap	submenu_pixmap(flSubmenu_xpm);
static fltk3::Pixmap	scroll_pixmap(flScroll_xpm);
static fltk3::Pixmap	tile_pixmap(flTile_xpm);
static fltk3::Pixmap	wizard_pixmap(flWizard_xpm);
static fltk3::Pixmap	pack_pixmap(flPack_xpm);
static fltk3::Pixmap	returnbutton_pixmap(flReturnButton_xpm);
static fltk3::Pixmap	lightbutton_pixmap(flLightButton_xpm);
static fltk3::Pixmap	repeatbutton_pixmap(flRepeatButton_xpm);
static fltk3::Pixmap	menubutton_pixmap(flMenuButton_xpm);
static fltk3::Pixmap	output_pixmap(flOutput_xpm);
static fltk3::Pixmap	textdisplay_pixmap(flTextDisplay_xpm);
static fltk3::Pixmap	textedit_pixmap(flTextEdit_xpm);
static fltk3::Pixmap	fileinput_pixmap(flFileInput_xpm);
static fltk3::Pixmap	browser_pixmap(flBrowser_xpm);
static fltk3::Pixmap	checkbrowser_pixmap(flCheckBrowser_xpm);
static fltk3::Pixmap	filebrowser_pixmap(flFileBrowser_xpm);
static fltk3::Pixmap	clock_pixmap(flClock_xpm);
static fltk3::Pixmap	help_pixmap(flHelp_xpm);
static fltk3::Pixmap	progress_pixmap(flProgress_xpm);
static fltk3::Pixmap	slider_pixmap(flSlider_xpm);
static fltk3::Pixmap	scrollbar_pixmap(flScrollBar_xpm);
static fltk3::Pixmap	valueslider_pixmap(flValueSlider_xpm);
static fltk3::Pixmap	adjuster_pixmap(flAdjuster_xpm);
static fltk3::Pixmap	counter_pixmap(flCounter_xpm);
static fltk3::Pixmap	dial_pixmap(flDial_xpm);
static fltk3::Pixmap	roller_pixmap(flRoller_xpm);
static fltk3::Pixmap	valueinput_pixmap(flValueInput_xpm);
static fltk3::Pixmap	valueoutput_pixmap(flValueOutput_xpm);
static fltk3::Pixmap	spinner_pixmap(flSpinner_xpm);
static fltk3::Pixmap	widgetclass_pixmap(flWidgetClass_xpm);
static fltk3::Pixmap	data_pixmap(flData_xpm);
static fltk3::Pixmap	tree_pixmap(flTree_xpm);
static fltk3::Pixmap	table_pixmap(flTable_xpm);
static fltk3::Pixmap	app_target_pixmap(flAppTarget_xpm);
static fltk3::Pixmap	lib_target_pixmap(flLibTarget_xpm);
static fltk3::Pixmap	file_pixmap(flFile_xpm);
static fltk3::Pixmap	folder_pixmap(flFolder_xpm);
static fltk3::Pixmap	fluid_file_pixmap(flFluidFile_xpm);
static fltk3::Pixmap	code_file_pixmap(flCodeFile_xpm);
static fltk3::Pixmap	workspace_pixmap(flWorkspace_xpm);
static fltk3::Pixmap	dependency_pixmap(flDependency_xpm);
fltk3::Pixmap	menu_none_pixmap(flMenuNone_xpm);
fltk3::Pixmap	menu_multi_pixmap(flMenuMulti_xpm);
fltk3::Pixmap	menu_all_pixmap(flMenuAll_xpm);

fltk3::Pixmap *pixmap[] = { 0, &window_pixmap, &button_pixmap, &checkbutton_pixmap, &roundbutton_pixmap, /* 0..4 */
 &box_pixmap, &group_pixmap, &function_pixmap, &code_pixmap, &codeblock_pixmap, &declaration_pixmap, /* 5..10 */ 
 &declarationblock_pixmap, &class_pixmap, &tabs_pixmap, &input_pixmap, &choice_pixmap,               /* 11..15 */
 &menuitem_pixmap, &menubar_pixmap, &submenu_pixmap, &scroll_pixmap, &tile_pixmap, &wizard_pixmap,   /* 16..21 */
 &pack_pixmap, &returnbutton_pixmap, &lightbutton_pixmap, &repeatbutton_pixmap, &menubutton_pixmap,  /* 22..26 */
 &output_pixmap, &textdisplay_pixmap, &textedit_pixmap, &fileinput_pixmap, &browser_pixmap,          /* 27..32 */
 &checkbrowser_pixmap, &filebrowser_pixmap, &clock_pixmap, &help_pixmap, &progress_pixmap,	     /* 33..36 */
 &slider_pixmap, &scrollbar_pixmap, &valueslider_pixmap, &adjuster_pixmap, &counter_pixmap,          /* 37..41 */
 &dial_pixmap, &roller_pixmap, &valueinput_pixmap, &valueoutput_pixmap, &comment_pixmap,             /* 42..46 */
 &spinner_pixmap, &widgetclass_pixmap, &data_pixmap, &tree_pixmap, &table_pixmap,                    /* 47..51 */
 &app_target_pixmap, &file_pixmap, &folder_pixmap, &fluid_file_pixmap, &code_file_pixmap,            /* 52..56 */
 &lib_target_pixmap, &workspace_pixmap, &dependency_pixmap};                                         /* 57..59 */

extern int show_comments;
extern Fl_Panel *the_file_panel;

////////////////////////////////////////////////////////////////

class Widget_Browser : public fltk3::Browser_ {
  friend class Fl_Type;

  // required routines for fltk3::Browser_ subclass:
  void *item_first() const ;
  void *item_next(void *) const ;
  void *item_prev(void *) const ;
  int item_selected(void *) const ;
  void item_select(void *,int);
  int item_width(void *) const ;
  int item_height(void *) const ;
  void item_draw(void *,int,int,int,int) const ;
  int incr_height() const ;

public:	

  int handle(int);
  void callback();
  Widget_Browser(int,int,int,int,const char * =0);
};

static Widget_Browser *widget_browser;
fltk3::Widget *make_widget_browser(int x,int y,int w,int h) {
  return (widget_browser = new Widget_Browser(x,y,w,h));
}

void select(Fl_Type *o, int v) {
  widget_browser->select(o,v,1);
  //  Fl_Type::current = o;
}

void select_only(Fl_Type *o) {
  widget_browser->select_only(o,1);
}

void deselect() {
  widget_browser->deselect();
  //Fl_Type::current = 0; // this breaks the paste & merge functions
}

Fl_Type *Fl_Type::first;
Fl_Type *Fl_Type::last;

static void Widget_Browser_callback(fltk3::Widget *o,void *) {
  ((Widget_Browser *)o)->callback();
}

Widget_Browser::Widget_Browser(int X,int Y,int W,int H,const char*l)
: fltk3::Browser_(X,Y,W,H,l) {
  type(fltk3::MULTI_BROWSER);
  Widget::callback(Widget_Browser_callback);
  when(fltk3::WHEN_RELEASE);
}

void *Widget_Browser::item_first() const {return Fl_Type::first;}

void *Widget_Browser::item_next(void *l) const {return ((Fl_Type*)l)->next;}

void *Widget_Browser::item_prev(void *l) const {return ((Fl_Type*)l)->prev;}

int Widget_Browser::item_selected(void *l) const {return ((Fl_Type*)l)->new_selected;}

void Widget_Browser::item_select(void *l,int v) {((Fl_Type*)l)->new_selected = v;}

int Widget_Browser::item_height(void *l) const {
  Fl_Type *t = (Fl_Type*)l;
  if (t->visible) {
    if (show_comments && t->comment())
      return textsize()*2+1;
    else
      return textsize()+2;
  } else {
    return 0;
  }
  return ((Fl_Type *)l)->visible ? textsize()+2 : 0;
}

int Widget_Browser::incr_height() const {return textsize()+2;}

static Fl_Type* pushedtitle;

// Generate a descriptive text for this item, to put in browser & window titles
const char* Fl_Type::title() {
  const char* c = name(); if (c) return c;
  return type_name();
}

extern const char* subclassname(Fl_Type*);

void Widget_Browser::item_draw(void *v, int X, int Y, int, int) const {
  Fl_Type *l = (Fl_Type *)v;
  X += 3 + 18 + l->level * 12;
  int comment_incr = 0;
  if (show_comments && l->comment()) {
    char buf[82], *d = buf;
    const char *s = l->comment();
    for (int i=0; i<80; i++) {
      char c = *s++;
      if (c==0 || c=='\n') break;
      *d++ = c;
    }
    *d = 0;
    comment_incr = textsize()-1;
    if (l->new_selected) fltk3::color(fltk3::contrast(fltk3::DARK_GREEN,fltk3::SELECTION_COLOR));
    else fltk3::color(fltk3::contrast(fltk3::DARK_GREEN,color()));
    fltk3::font(textfont()+fltk3::ITALIC, textsize()-2);
    fltk3::draw(buf, X+12, Y+12);
    Y += comment_incr/2;
    comment_incr -= comment_incr/2;
  }
  if (l->new_selected) fltk3::color(fltk3::contrast(fltk3::FOREGROUND_COLOR,fltk3::SELECTION_COLOR));
  else fltk3::color(fltk3::FOREGROUND_COLOR);
  fltk3::Pixmap *pm = pixmap[l->pixmapID()];
  if (pm) pm->draw(X-18+11, Y);
  switch (l->is_public()) {
    case 0: lock_pixmap.draw(X-17+11, Y); break;
    case 2: protected_pixmap.draw(X-17+11, Y); break;
  }
  X -= 18;
  if (l->is_parent()) {
    if (!l->next || l->next->level <= l->level) {
      if (l->open_!=(l==pushedtitle)) {
        fltk3::loop(X,Y+7,X+5,Y+12,X+10,Y+7);
      } else {
        fltk3::loop(X+2,Y+2,X+7,Y+7,X+2,Y+12);
      }
    } else {
      if (l->open_!=(l==pushedtitle)) {
        fltk3::polygon(X,Y+7,X+5,Y+12,X+10,Y+7);
      } else {
        fltk3::polygon(X+2,Y+2,X+7,Y+7,X+2,Y+12);
      }
    }
  }
  X += 10;
  X += 18;
  Y += comment_incr;
  if (l->is_widget() || l->is_class()) {
    const char* c = subclassname(l);
    if (!strncmp(c,"fltk3::",7)) c += 7;
    fltk3::font(textfont(), textsize());
    fltk3::draw(c, X, Y+13);
    X += int(fltk3::width(c)+fltk3::width('n'));
    c = l->name();
    if (c) {
      fltk3::font(textfont()|fltk3::BOLD, textsize());
      fltk3::draw(c, X, Y+13);
    } else if ((c=l->label())) {
      char buf[50]; char* p = buf;
      *p++ = '"';
      for (int i = 20; i--;) {
        if (! (*c & -32)) break;
        *p++ = *c++;
      }
      if (*c) {strcpy(p,"..."); p+=3;}
      *p++ = '"';
      *p = 0;
      fltk3::draw(buf, X, Y+13);
    }
  } else {
    const char* c = l->title();
    char buf[60]; char* p = buf;
    for (int i = 55; i--;) {
      if (! (*c & -32)) break;
      *p++ = *c++;
    }
    if (*c) {strcpy(p,"..."); p+=3;}
    *p = 0;
    fltk3::font(textfont() | (l->is_code_block() && (l->level==0 || l->parent->is_class())?0:fltk3::BOLD), textsize());
    fltk3::draw(buf, X, Y+13);
  }
}

int Widget_Browser::item_width(void *v) const {
  Fl_Type *l = (Fl_Type *)v;

  if (!l->visible) return 0;

  int W = 3 + 16 + 18 + l->level*10;
  if (l->is_parent()) W += 10;

  if (l->is_widget() || l->is_class()) {
    const char* c = l->type_name();
    if (!strncmp(c,"fltk3::",7)) c += 7;
    fltk3::font(textfont(), textsize());
    W += int(fltk3::width(c) + fltk3::width('n'));
    c = l->name();
    if (c) {
      fltk3::font(textfont()|fltk3::BOLD, textsize());
      W += int(fltk3::width(c));
    } else if ((c=l->label())) {
      char buf[50]; char* p = buf;
      *p++ = '"';
      for (int i = 20; i--;) {
	if (! (*c & -32)) break;
	*p++ = *c++;
      }
      if (*c) {strcpy(p,"..."); p+=3;}
      *p++ = '"';
      *p = 0;
      W += int(fltk3::width(buf));
    }
  } else {
    const char* c = l->title();
    char buf[60]; char* p = buf;
    for (int i = 55; i--;) {
      if (! (*c & -32)) break;
      *p++ = *c++;
    }
    if (*c) {strcpy(p,"..."); p+=3;}
    *p = 0;
    fltk3::font(textfont() | (l->is_code_block() && (l->level==0 || l->parent->is_class())?0:fltk3::BOLD), textsize());
    W += int(fltk3::width(buf));
  }

  return W;
}

void redraw_browser() {
  widget_browser->redraw();
}

void Widget_Browser::callback() {
  selection_changed((Fl_Type*)selection());
}

int Widget_Browser::handle(int e) {
  static Fl_Type *title;
  Fl_Type *l;
  int X,Y,W,H; bbox(X,Y,W,H);
  switch (e) {
    case fltk3::PUSH:
      if (!fltk3::event_inside(X,Y,W,H)) break;
      l = (Fl_Type*)find_item(fltk3::event_y());
      if (l) {
        X += 12*l->level + 18 - hposition();
        if (l->is_parent() && fltk3::event_x()>X-18 && fltk3::event_x()<X+13-18) {
          title = pushedtitle = l;
          redraw_line(l);
          return 1;
        }
      }
      break;
    case fltk3::DRAG:
      if (!title) break;
      l = (Fl_Type*)find_item(fltk3::event_y());
      if (l) {
        X += 12*l->level + 18 - hposition();
        if (l->is_parent() && fltk3::event_x()>X-18 && fltk3::event_x()<X+13-18) ;
        else l = 0;
      }
      if (l != pushedtitle) {
        if (pushedtitle) redraw_line(pushedtitle);
        if (l) redraw_line(l);
        pushedtitle = l;
      }
      return 1;
    case fltk3::RELEASE:
      if (!title) {
        l = (Fl_Type*)find_item(fltk3::event_y());
        if (l && l->new_selected && (fltk3::event_clicks() || fltk3::event_state(fltk3::CTRL)))
          l->open();
        break;
      }
      l = pushedtitle;
      title = pushedtitle = 0;
      if (l) {
        if (l->open_) {
          l->open_ = 0;
          for (Fl_Type*k = l->next; k&&k->level>l->level; k = k->next)
            k->visible = 0;
        } else {
          l->open_ = 1;
          for (Fl_Type*k=l->next; k&&k->level>l->level;) {
            k->visible = 1;
            if (k->is_parent() && !k->open_) {
              Fl_Type *j;
              for (j = k->next; j && j->level>k->level; j = j->next);
              k = j;
            } else
              k = k->next;
          }
        }
        redraw();
      }
      return 1;
    case fltk3::DND_ENTER:
    case fltk3::DND_DRAG:
    case fltk3::DND_RELEASE:
      if (!fltk3::event_inside(X,Y,W,H)) break;
      l = (Fl_Type*)find_item(fltk3::event_y());
      if (l && l->dnd_available()) {
        title = l;
        return 1;
      } else {
        title = 0;
        return 0;
      }
    case fltk3::PASTE:
      if (title && title->dnd_available())
        title->dnd_paste();
      return 1;
  }
  return Browser_::handle(e);
}

Fl_Type::Fl_Type() {
  factory = 0;
  parent = 0;
  next = prev = 0;
  selected = new_selected = 0;
  visible = 0;
  name_ = 0;
  label_ = 0;
  user_data_ = 0;
  user_data_type_ = 0;
  callback_ = 0;
  comment_ = 0;
  rtti = 0;
  level = 0;
  code_position = header_position = -1;
  code_position_end = header_position_end = -1;
}

static void fixvisible(Fl_Type *p) {
  Fl_Type *t = p;
  for (;;) {
    if (t->parent) t->visible = t->parent->visible && t->parent->open_;
    else t->visible = 1;
    t = t->next;
    if (!t || t->level <= p->level) break;
  }
}

// turn a click at x,y on this into the actual picked object:
Fl_Type* Fl_Type::click_test(int,int) {return 0;}
void Fl_Type::add_child(Fl_Type*, Fl_Type*) {}
void Fl_Type::move_child(Fl_Type*, Fl_Type*) {}
void Fl_Type::remove_child(Fl_Type*) {}

// add a list of widgets as a new child of p:
void Fl_Type::add(Fl_Type *p) {
  if (p && parent == p) return;
  undo_checkpoint();
  parent = p;
  Fl_Type *end = this;
  while (end->next) end = end->next;
  Fl_Type *q;
  int newlevel;
  if (p) {
    for (q = p->next; q && q->level > p->level; q = q->next);
    newlevel = p->level+1;
  } else {
    q = 0;
    newlevel = 0;
  }
  for (Fl_Type *t = this->next; t; t = t->next) t->level += (newlevel-level);
  level = newlevel;
  if (q) {
    prev = q->prev;
    prev->next = this;
    q->prev = end;
    end->next = q;
  } else if (first) {
    prev = last;
    prev->next = this;
    end->next = 0;
    last = end;
  } else {
    first = this;
    last = end;
    prev = end->next = 0;
  }
  if (p) p->add_child(this,0);
  open_ = 1;
  fixvisible(this);
  set_modflag(1);
  widget_browser->redraw();
}

// add to a parent before another widget:
void Fl_Type::insert(Fl_Type *g) {
  Fl_Type *end = this;
  while (end->next) end = end->next;
  parent = g->parent;
  int newlevel = g->level;
  visible = g->visible;
  for (Fl_Type *t = this->next; t; t = t->next) t->level += newlevel-level;
  level = newlevel;
  prev = g->prev;
  if (prev) prev->next = this; else first = this;
  end->next = g;
  g->prev = end;
  fixvisible(this);
  if (parent) parent->add_child(this, g);
  widget_browser->redraw();
}

// Return message number for I18N...
int
Fl_Type::msgnum() {
  int		count;
  Fl_Type	*p;

  for (count = 0, p = this; p;) {
    if (p->label()) count ++;
    if (p != this && p->is_widget() && ((Fl_Widget_Type *)p)->tooltip()) count ++;

    if (p->prev) p = p->prev;
    else p = p->parent;
  }

  return count;
}


// delete from parent:
Fl_Type *Fl_Type::remove() {
  Fl_Type *end = this;
  for (;;) {
    if (!end->next || end->next->level <= level) break;
    end = end->next;
  }
  if (prev) prev->next = end->next;
  else first = end->next;
  if (end->next) end->next->prev = prev;
  else last = prev;
  Fl_Type *r = end->next;
  prev = end->next = 0;
  if (parent) parent->remove_child(this);
  parent = 0;
  widget_browser->redraw();
  selection_changed(0);
  return r;
}

// update a string member:
int storestring(const char *n, const char * & p, int nostrip) {
  if (n == p) return 0;
  undo_checkpoint();
  int length = 0;
  if (n) { // see if blank, strip leading & trailing blanks
    if (!nostrip) while (isspace(*n)) n++;
    const char *e = n + strlen(n);
    if (!nostrip) while (e > n && isspace(*(e-1))) e--;
    length = e-n;
    if (!length) n = 0;
  }    
  if (n == p) return 0;
  if (n && p && !strncmp(n,p,length) && !p[length]) return 0;
  if (p) free((void *)p);
  if (!n || !*n) {
    p = 0;
  } else {
    char *q = (char *)malloc(length+1);
    strlcpy(q,n,length+1);
    p = q;
  }
  set_modflag(1);
  return 1;
}

void Fl_Type::name(const char *n) {
  int nostrip = is_comment();
  if (storestring(n,name_,nostrip)) {
    if (visible) widget_browser->redraw();
  }
}

void Fl_Type::label(const char *n) {
  if (storestring(n,label_,1)) {
    setlabel(label_);
    if (visible && !name_) widget_browser->redraw();
  }
}

void Fl_Type::callback(const char *n) {
  storestring(n,callback_);
}

void Fl_Type::user_data(const char *n) {
  storestring(n,user_data_);
}

void Fl_Type::user_data_type(const char *n) {
  storestring(n,user_data_type_);
}

void Fl_Type::comment(const char *n) {
  storestring(n, comment_, 1);
}

void Fl_Type::open() {
  printf("Open of '%s' is not yet implemented\n",type_name());
}

void Fl_Type::setlabel(const char *) {}

Fl_Type::~Fl_Type() {
  // warning: destructor only works for widgets that have been add()ed.
  if (widget_browser) widget_browser->deleting(this);
  if (prev) prev->next = next; else first = next;
  if (next) next->prev = prev; else last = prev;
  if (current == this) current = 0;
  if (parent) parent->remove_child(this);
  if (name_) free((void*)name_);
  if (label_) free((void*)label_);
  if (callback_) free((void*)callback_);
  if (user_data_) free((void*)user_data_);
  if (user_data_type_) free((void*)user_data_type_);
  if (comment_) free((void*)comment_);
}

int Fl_Type::is_parent() const {return 0;}
int Fl_Type::is_widget() const {return 0;}
int Fl_Type::is_valuator() const {return 0;}
int Fl_Type::is_spinner() const {return 0;}
int Fl_Type::is_button() const {return 0;}
int Fl_Type::is_input() const {return 0;}
int Fl_Type::is_value_input() const {return 0;}
int Fl_Type::is_text_display() const {return 0;}
int Fl_Type::is_menu_item() const {return 0;}
int Fl_Type::is_menu_button() const {return 0;}
int Fl_Type::is_group() const {return 0;}
int Fl_Type::is_window() const {return 0;}
int Fl_Type::is_code_block() const {return 0;}
int Fl_Type::is_decl_block() const {return 0;}
int Fl_Type::is_comment() const {return 0;}
int Fl_Type::is_class() const {return 0;}
int Fl_Type::is_public() const {return 1;}

int Fl_Code_Type::is_public()const { return -1; }
int Fl_CodeBlock_Type::is_public()const { return -1; }


////////////////////////////////////////////////////////////////

Fl_Type *in_this_only; // set if menu popped-up in window

void select_all_cb(fltk3::Widget *,void *) {
  Fl_Type *p = Fl_Type::current ? Fl_Type::current->parent : 0;
  if (in_this_only) {
    Fl_Type *t = p;
    for (; t && t != in_this_only; t = t->parent);
    if (t != in_this_only) p = in_this_only;
  }
  for (;;) {
    if (p) {
      int foundany = 0;
      for (Fl_Type *t = p->next; t && t->level>p->level; t = t->next) {
	if (!t->new_selected) {widget_browser->select(t,1,0); foundany = 1;}
      }
      if (foundany) break;
      p = p->parent;
    } else {
      for (Fl_Type *t = Fl_Type::first; t; t = t->next)
	widget_browser->select(t,1,0);
      break;
    }
  }
  selection_changed(p);
}

void select_none_cb(fltk3::Widget *,void *) {
  Fl_Type *p = Fl_Type::current ? Fl_Type::current->parent : 0;
  if (in_this_only) {
    Fl_Type *t = p;
    for (; t && t != in_this_only; t = t->parent);
    if (t != in_this_only) p = in_this_only;
  }
  for (;;) {
    if (p) {
      int foundany = 0;
      for (Fl_Type *t = p->next; t && t->level>p->level; t = t->next) {
	if (t->new_selected) {widget_browser->select(t,0,0); foundany = 1;}
      }
      if (foundany) break;
      p = p->parent;
    } else {
      for (Fl_Type *t = Fl_Type::first; t; t = t->next)
	widget_browser->select(t,0,0);
      break;
    }
  }
  selection_changed(p);
}

static void delete_children(Fl_Type *p) {
  Fl_Type *f;
  for (f = p; f && f->next && f->next->level > p->level; f = f->next);
  for (; f != p; ) {
    Fl_Type *g = f->prev;
    delete f;
    f = g;
  }
}

void delete_all(int selected_only) {
  for (Fl_Type *f = Fl_Type::first; f;) {
    if (f->selected || !selected_only) {
      delete_children(f);
      Fl_Type *g = f->next;
      delete f;
      f = g;
    } else f = f->next;
  }
  if(!selected_only) {
		include_H_from_C=1;
		use_FL_COMMAND=0;
	}

  selection_changed(0);
}

// move f (and it's children) into list before g:
// returns pointer to whatever is after f & children
void Fl_Type::move_before(Fl_Type* g) {
  if (level != g->level) printf("move_before levels don't match! %d %d\n",
				level, g->level);
  Fl_Type* n;
  for (n = next; n && n->level > level; n = n->next);
  if (n == g) return;
  Fl_Type *l = n ? n->prev : Fl_Type::last;
  prev->next = n;
  if (n) n->prev = prev; else Fl_Type::last = prev;
  prev = g->prev;
  l->next = g;
  if (prev) prev->next = this; else Fl_Type::first = this;
  g->prev = l;
  if (parent && is_widget()) parent->move_child(this,g);
  widget_browser->inserting(g, this);
  widget_browser->display(this);
  widget_browser->redraw();
}

// move selected widgets in their parent's list:
void earlier_cb(fltk3::Widget*,void*) {
  Fl_Type *f;
  int mod = 0;
  for (f = Fl_Type::first; f; ) {
    Fl_Type* nxt = f->next;
    if (f->selected) {
      Fl_Type* g;
      for (g = f->prev; g && g->level > f->level; g = g->prev);
      if (g && g->level == f->level && !g->selected) {
        f->move_before(g);
        mod = 1;
      }
    }
    f = nxt;
  }
  if (mod) set_modflag(1);
}

void later_cb(fltk3::Widget*,void*) {
  Fl_Type *f;
  int mod = 0;
  for (f = Fl_Type::last; f; ) {
    Fl_Type* prv = f->prev;
    if (f->selected) {
      Fl_Type* g;
      for (g = f->next; g && g->level > f->level; g = g->next);
      if (g && g->level == f->level && !g->selected) {
        g->move_before(f);
        mod = 1;
      }
    }
    f = prv;
  }
  if (mod) set_modflag(1);
}

////////////////////////////////////////////////////////////////

// write a widget and all it's children:
void Fl_Type::write() {
    write_indent(level);
    write_word(type_name());
    
    if (is_class()) {
      const char * p = 	((Fl_Class_Type*)this)->prefix();
      if (p &&	strlen(p))
        write_word(p);
    }

    write_word(name());
    write_open(level);
    write_properties();
    write_close(level);
    if (!is_parent()) return;
    // now do children:
    write_open(level);
    Fl_Type *child;
    for (child = next; child && child->level > level; child = child->next)
	if (child->level == level+1) child->write();
    write_close(level);
}

void Fl_Type::write_properties() {
  // repeat this for each attribute:
  if (label()) {
    write_indent(level+1);
    write_word("label");
    write_word(label());
  }
  if (user_data()) {
    write_indent(level+1);
    write_word("user_data");
    write_word(user_data());
  }
  if (user_data_type()) {
    write_word("user_data_type");
    write_word(user_data_type());
  }
  if (callback()) {
    write_indent(level+1);
    write_word("callback");
    write_word(callback());
  }
  if (comment()) {
    write_indent(level+1);
    write_word("comment");
    write_word(comment());
  }
  if (is_parent() && open_) write_word("open");
  if (selected) write_word("selected");
}

char Fl_Type::read_property(const char *c) {
  if (!strcmp(c,"label"))
    label(read_word());
  else if (!strcmp(c,"user_data"))
    user_data(read_word());
  else if (!strcmp(c,"user_data_type"))
    user_data_type(read_word());
  else if (!strcmp(c,"callback"))
    callback(read_word());
  else if (!strcmp(c,"comment"))
    comment(read_word());
  else if (!strcmp(c,"open"))
    open_ = 1;
  else if (!strcmp(c,"selected"))
    select(this,1);
  else {
    read_error("Unknown property \"%s\"", c);
    return 0;
  }
  return 1;
}

/**
 * Return 1 if the list contains a function with the given signature at the top level
 */
int has_toplevel_function(const char *rtype, const char *sig) {
  Fl_Type *child;
  for (child = Fl_Type::first; child; child = child->next) {
    if (!child->is_in_class() && strcmp(child->type_name(), "Function")==0) {
      const Fl_Function_Type *fn = (const Fl_Function_Type*)child;
      if (fn->has_signature(rtype, sig))
        return 1;
    }
  }
  return 0;
}

/**
 * Write a comment into the header file.
 */
void Fl_Type::write_comment_h(const char *pre)
{
  if (comment()) {
    write_h("%s/**\n", pre);
    const char *s = comment();
    write_h("%s   ", pre);
    while(*s) {
      if (*s=='\n') {
        if (s[1]) {
          write_h("\n%s   ", pre);
        }
      } else {
        write_h("%c", *s); // FIXME this is much too slow!
      }
      s++;
    }
    write_h("\n%s*/\n", pre);
  }
}

/**
 * Write a comment inot the header file.
 */
void Fl_Type::write_comment_c(const char *pre)
{
  if (comment()) {
    write_c("%s/**\n", pre);
    const char *s = comment();
    write_c("%s   ", pre);
    while(*s) {
      if (*s=='\n') {
        if (s[1]) {
          write_c("\n%s   ", pre);
        }
      } else {
        write_c("%c", *s); // FIXME this is much too slow!
      }
      s++;
    }
    write_c("\n%s*/\n", pre);
  }
}

/**
 * Make sure that the given item is visible in the browser by opening
 * all parent groups and moving the item into the visible space.
 */
void reveal_in_browser(Fl_Type *t) {
  Fl_Type *p = t->parent;
  if (p) {
    for (;;) {
      if (!p->open_)
        p->open_ = 1;
      if (!p->parent) break;
      p = p->parent;
    }
    fixvisible(p);
  }
  widget_browser->display(t);
  redraw_browser();
}

/**
 * Build widgets and dataset needed in live mode.
 * \return a widget pointer that the live mode initiator can 'show()'
 * \see leave_live_mode()
 */
fltk3::Widget *Fl_Type::enter_live_mode(int) {
  return 0L;
}

/**
 * Release all resources created when enetring live mode.
 * \see enter_live_mode()
 */
void Fl_Type::leave_live_mode() {
}

/**
 * Copy all needed properties for this tye into the live object.
 */
void Fl_Type::copy_properties() {
}

/**
 * Check whether callback name is declared anywhere else by the user
 *
 * \b Warning: this just checks that the name is declared somewhere,
 * but it should probably also check that the name corresponds to a
 * plain function or a member function within the same class and that
 * the parameter types match.
 */
int Fl_Type::user_defined(const char* cbname) const {
  for (Fl_Type* p = Fl_Type::first; p ; p = p->next)
    if (strcmp(p->type_name(), "Function") == 0 && p->name() != 0)
      if (strncmp(p->name(), cbname, strlen(cbname)) == 0)
        if (p->name()[strlen(cbname)] == '(')
          return 1;
  return 0;
}


extern const char *filename;
extern void set_filename(const char*);
extern char project_is_workspace();

// ------------ Tool -----------------------------------------------------------

Fl_Tool_Type::Fl_Tool_Type()
: Fl_Type(),
  pBuildEnv(FL_ENV_ALL),
  pListEnv(FL_ENV_ALL),
  pNUUID(0), pnUUID(0), 
  pUUIDName(0L), pUUID(0),
  pFlags(0)
{
}

Fl_Tool_Type::~Fl_Tool_Type() {
  int i;
  for (i=0; i<pnUUID; i++) {
    free(pUUIDName[i]);
    free(pUUID[i]);
  }
  if (pNUUID) {
    free(pUUIDName);
    free(pUUID);
  }
}

int Fl_Tool_Type::find_UUID(const char *name) {
  int i;
  for (i=0; i<pnUUID; i++) {
    if (strcmp(pUUIDName[i], name)==0)
      return i;
  }
  return -1;
}

void Fl_Tool_Type::set_UUID(int i, const char *uuid) {
  if (pUUID[i]) 
    free(pUUID[i]);
  pUUID[i] = 0;
  pUUID[i] = strdup(verify_UUID(uuid));
}

void Fl_Tool_Type::set_UUID(const char *name, const char *uuid) {
  int i = find_UUID(name);
  if (i==-1) {
    if (pNUUID==pnUUID) {
      pNUUID += 4;
      pUUIDName = (char**)realloc(pUUIDName, pNUUID*sizeof(char*));
      pUUID = (char**)realloc(pUUID, pNUUID*sizeof(char*));
    }
    pUUIDName[pnUUID] = strdup(name);
    pUUID[pnUUID] = strdup(verify_UUID(uuid));
    pnUUID++;
  } else {
    set_UUID(i, uuid);
  }
}

/*
 Verify that a Uniqe ID is truly unique.
 */
const char *Fl_Tool_Type::verify_UUID(const char *uuid) {
  Fl_Type *t;
  for (t = first; t; t = t->next) {
    if (t->is_tool()) {
      Fl_Tool_Type *wt = (Fl_Tool_Type*)t;
      int j, n = wt->pnUUID;
      for (j=0; j<n; j++) {
        if (wt->pUUID[j] && strcmp(wt->pUUID[j], uuid)==0) {
          printf("Replacing duplicate UUIDin %s!\n", wt->name());
          return fltk3::Preferences::newUUID();
        }
      }
    }
  }
  return uuid;
}

const char *Fl_Tool_Type::get_UUID(const char *name) {
  int i = find_UUID(name);
  if (i==-1) {
    i = pnUUID;
    const char *uuid = fltk3::Preferences::newUUID();
    set_UUID(name, uuid);
  }
  return pUUID[i];
}

const char *Fl_Tool_Type::get_UUID_Xcode(const char *name) {
  static char buf[25];
  const char *uuid = get_UUID(name);
  // 937C4900-51AA-4C11-8DD3-7AB5 9944F03E
  unsigned int a, b, c, d, e, f;
  sscanf(uuid, "%08X-%04X-%04X-%04X-%04X%08X", &a, &b, &c, &d, &e, &f);
  sprintf(buf, "%08X%04X%04X%08X", a, b^c, d^e, f);
  return buf;
}

void Fl_Tool_Type::write_properties() {
  Fl_Type::write_properties();
  int i;
  for (i=0; i<pnUUID; i++) {
    char buf[80];
    strcpy(buf, "uuid_");
    strcat(buf, pUUIDName[i]); 
    write_indent(level+1);
    write_word(buf);
    write_word(pUUID[i]);
  }
  if (build_env()!=FL_ENV_ALL) {
    write_indent(level+1);
    write_string("build_env %d", build_env());
  }
  if (list_env()!=FL_ENV_ALL) {
    write_indent(level+1);
    write_string("list_env %d", list_env());
  }
}

char Fl_Tool_Type::read_property(const char *name) {
  if (strncmp(name, "uuid_", 5)==0) {
    char buf[80];
    strcpy(buf, name+5);
    set_UUID(buf, read_word());
  } else if (!strcmp(name,"environments")) { // FIXME: delete this
    int v = atoi(read_word());
    build_env(v); list_env(v);
  } else if (!strcmp(name,"build_env")) {
    build_env(atoi(read_word()));
  } else if (!strcmp(name,"list_env")) {
    list_env(atoi(read_word()));
  } else {
    return Fl_Type::read_property(name);
  }
  return 1;
}

int Fl_Tool_Type::dnd_available() {
  // FIXME: we should maybe test if the objects dropped are actually valid file names
  if (filename && *filename && (is_target() || is_folder()))
    return 1;
  return 0;
}

int Fl_Tool_Type::dnd_paste() {
  if (filename && *filename && (is_target() || is_folder())) {
    if (fltk3::event_text() && *fltk3::event_text()) {
      char *basedir = strdup(filename); // global name of workspace file!
      char *fn = (char*)fltk3::filename_name(basedir);
      if (fn) *fn = 0;
      char *files = strdup(fltk3::event_text()), *s = files, *e = s;
      char done = 0;
      for (;!done;) {
        // find the end of the line
        for (;;) {
          if (*e==0) { done = 1; break; }
          if (*e=='\n') { *e++ = 0; break; }
          e++;
        }
        // add the file 's' to this item
        if (e!=s) {
          char buf[2048];
          fltk3::filename_relative(buf, 2048, s, basedir);
          Fl_File_Type *o = new Fl_File_Type();
          o->filename(buf);
          o->add(this);
          o->factory = this;
        }
        s = e;
      }
      free(files);
      free(basedir);
    }
  }
  return 1;
}

// ------------ Workspace ------------------------------------------------------

Fl_Workspace_Type Fl_Workspace_type;

Fl_Type *Fl_Workspace_Type::make() {
  // A workspace must be the first entry
  if (Fl_Type::first) {
    fltk3::message("A Workspace can only be added to an empty project.");
    return 0;
  }
  // add the workspace to the file
  Fl_Workspace_Type *o = new Fl_Workspace_Type();
  o->name("myWorkspace");
  o->add(0L);
  o->factory = this;
  return o;
}

void Fl_Workspace_Type::open() {
  if (!the_file_panel) the_file_panel = make_file_panel();
  the_file_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_file_panel->show();
}


// ------------ Target Dependency ----------------------------------------------

Fl_Target_Dependency_Type Fl_Target_Dependency_type;

Fl_Type *Fl_Target_Dependency_Type::make() {
  // A workspace must be the first entry
  if (!project_is_workspace()) {
    fltk3::message("A Target Dependency can only be added to another Target in a Workspace.");
    return 0;
  }
  // add the dependency to the target
  Fl_Type *p = Fl_Type::current;
  while (p && !p->is_folder() && !p->is_target())
    p = p->parent;  
  if (!p) {
    fltk3::message("A Target Dependency reference can only be added to a Target or Folder!");
    return 0;
  }
  Fl_Target_Dependency_Type *o = new Fl_Target_Dependency_Type();
  o->name("otherTarget");
  o->add(p);
  o->factory = this;
  return o;
}

void Fl_Target_Dependency_Type::open() {
  if (!the_file_panel) the_file_panel = make_file_panel();
  the_file_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_file_panel->show();
}


// ------------ Target ---------------------------------------------------------

Fl_Target_Type Fl_Target_type;

Fl_Type *Fl_Target_Type::make() {
  // no generic targets!
  return 0L;
}

// Nasty: we assume that the name is not longer than 63 characters... .
const char *Fl_Target_Type::caps_name() {
  if (!pCapsName) pCapsName = (char*)malloc(64);
  const char *s = name();
  char *d = pCapsName;
  while (*s) {
    *d++ = *s++;
  }
  *d++ = 0;
  return pCapsName;
}

Fl_Target_Type *Fl_Target_Type::find(const char *name, char end) {
  // find a partial string, if 'end' is set to a character
  char buf[2048];
  strcpy(buf, name);
  if (end) {
    char *sep = strchr(buf, end);
    if (sep) *sep = 0;
  }
  // now find the target by name (stored in 'buf')
  Fl_Type *tgt = first;
  while (tgt) {
    if (tgt->is_target() && strcmp(tgt->name(), buf)==0)
      return (Fl_Target_Type*)tgt;
    tgt = tgt->next;
  }
  return 0;
}

Fl_Target_Type *Fl_Target_Type::first_target(Fl_Type *base) {
  Fl_Type *src = base->next;
  while (src && src->level>base->level) {
    if (src->is_target()) 
      return (Fl_Target_Type*)src;
    src = src->next;
  }
  return 0;
}

Fl_Target_Type *Fl_Target_Type::next_target(Fl_Type *base) {
  Fl_Type *src = this->next;
  while (src && src->level>base->level) {
    if (src->is_target()) 
      return (Fl_Target_Type*)src;
    src = src->next;
  }
  return 0;
}

void Fl_Target_Type::target_path(const char *path) {
  if (pTargetPath) {
    free(pTargetPath);
    pTargetPath = 0L;
  }
  if (path) {
    pTargetPath = strdup(path);
  } else {
    pTargetPath = strdup("");
  }
}

void Fl_Target_Type::makefile_path(const char *path) {
  if (pMakefilePath) {
    free(pMakefilePath);
    pMakefilePath = 0L;
  }
  if (path) {
    pMakefilePath = strdup(path);
  } else {
    pMakefilePath = strdup("");
  }
}

char Fl_Target_Type::read_property(const char *c) {
  if (!strcmp(c,"target_path")) {
    target_path(read_word());
  } else if (!strcmp(c,"makefile_path")) {
    makefile_path(read_word());
  } else {
    return Fl_Tool_Type::read_property(c);
  }
  return 1;
}

void Fl_Target_Type::write_properties() {
  Fl_Tool_Type::write_properties();
  if (target_path() && *target_path()) {
    write_indent(level+1);
    write_string("target_path");
    write_word(target_path());
  }
  if (makefile_path() && *makefile_path()) {
    write_indent(level+1);
    write_string("makefile_path");
    write_word(makefile_path());
  }
}


// ------------ Application Target ---------------------------------------------

Fl_App_Target_Type Fl_App_Target_type;

Fl_Type *Fl_App_Target_Type::make() {
  // a target can only go into a workspace file
  if (!project_is_workspace()) {
    fltk3::message("A Target can only be added to a Workspace.");
    return 0;
  }
  // find out where we can add this target
  Fl_Type *p = Fl_Type::current;
  while (p && !p->is_category() && !p->is_workspace())
    p = p->parent;  
  Fl_App_Target_Type *o = new Fl_App_Target_Type();
  o->name("myProgram");
  o->add(p);
  o->factory = this;
  return o;
}

void Fl_App_Target_Type::open() {
  if (!the_file_panel) the_file_panel = make_file_panel();
  the_file_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_file_panel->show();
}

// ------------ Library Target -------------------------------------------------

Fl_Lib_Target_Type Fl_Lib_Target_type;

Fl_Type *Fl_Lib_Target_Type::make() {
  // a target con only go into a workspace file
  // TODO: we can offer to create a workspace if this is currently a GUI file
  if (!project_is_workspace()) {
    fltk3::message("A Target can only be added to a Workspace.");
    return 0;
  }
  // find out where we can add this target
  Fl_Type *p = Fl_Type::current;
  while (p && !p->is_category() && !p->is_workspace())
    p = p->parent;  
  Fl_Lib_Target_Type *o = new Fl_Lib_Target_Type();
  o->name("myLibrary");
  o->add(p);
  o->factory = this;
  return o;
}

void Fl_Lib_Target_Type::open() {
  if (!the_file_panel) the_file_panel = make_file_panel();
  the_file_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_file_panel->show();
}

// ------------ Generic File ---------------------------------------------------

Fl_File_Type Fl_File_type;

Fl_Type *Fl_File_Type::make() {
  // make sure that this is a workspace file
  if (Fl_Type::first && !project_is_workspace()) {
    fltk3::message("File references can only be used in Workspace files.");
    return 0;
  }
  // files can be children of folders and children of targets
  Fl_Type *p = Fl_Type::current;
  while (p && !p->is_target() && !p->is_folder()) 
    p = p->parent;
  if (!p) {
    fltk3::message("A File reference can only be added to a Target or Folder!");
    return 0;
  }
  Fl_File_Type *o = new Fl_File_Type();
  o->name("readme.txt");
  o->add(p);
  o->factory = this;
  return o;
}

void Fl_File_Type::set_default_type() {
  const char *fn = filename();
  if (fn) {
    const char *ext = fltk3::filename_ext(fn);
    filetype(FL_FILE_TEXT);
    if (ext) {
      if (strcmp(ext, ".cxx")==0 || strcmp(ext, ".cpp")==0) {
        filetype(FL_FILE_CPP_SOURCE);
      } else if (strcmp(ext, ".c")==0) {
        filetype(FL_FILE_C_SOURCE);
      } else if (strcmp(ext, ".H")==0) {
        filetype(FL_FILE_CPP_HEADER);
      } else if (strcmp(ext, ".h")==0) {
        filetype(FL_FILE_C_HEADER);
      } else if (strcmp(ext, ".mm")==0) {
        filetype(FL_FILE_OBJC_SOURCE);
      } else if (strcmp(ext, ".framework")==0) {
        filetype(FL_FILE_FRAMEWORK);
      } else if (strcmp(ext, ".fl")==0) {
        filetype(FL_FILE_FLUID_UI);
      } else if (strcmp(ext, ".lib")==0) {
        filetype(FL_FILE_LIBRARY);
      }
    }
  }
}

void Fl_File_Type::filename(const char *fn) {
  if (pFilename) {
    free(pFilename);
    pFilename = 0;
    name(0);
  }
  if (fn) {
    pFilename = strdup(fn);
    name(fltk3::filename_name(fn));
    set_default_type();
  }
}

char Fl_File_Type::read_property(const char *c) {
  if (!strcmp(c,"filename_and_path")) {
    filename(read_word());
  } else if (!strcmp(c,"filetype")) {
    filetype(atoi(read_word()));
  } else if (!strcmp(c,"location")) {
    location(atoi(read_word()));
  } else {
    return Fl_Tool_Type::read_property(c);
  }
  return 1;
}

void Fl_File_Type::write_properties() {
  Fl_Tool_Type::write_properties();
  if (filename() && *filename()) {
    write_indent(level+1);
    write_string("filename_and_path");
    write_word(filename());
  }
  if (filetype()&FL_FILE_EXPLICIT) {
    write_indent(level+1);
    write_string("filetype %d", filetype());
  }
  if (location()!=FL_LOCATION_WORKSPACE) {
    write_indent(level+1);
    write_string("location %d", location());
  }
}

Fl_File_Type *Fl_File_Type::first_file(Fl_Type *base) {
  Fl_Type *src = base->next;
  while (src && src->level>base->level) {
    if (src->is_file()) 
      return (Fl_File_Type*)src;
    src = src->next;
  }
  return 0;
}

Fl_File_Type *Fl_File_Type::next_file(Fl_Type *base) {
  Fl_Type *src = this->next;
  while (src && src->level>base->level) {
    if (src->is_file()) 
      return (Fl_File_Type*)src;
    src = src->next;
  }
  return 0;
}

Fl_Target_Dependency_Type *Fl_Target_Dependency_Type::first_dependency(Fl_Type *base) {
  Fl_Type *src = base->next;
  while (src && src->level>base->level) {
    if (src->is_target_dependency()) 
      return (Fl_Target_Dependency_Type*)src;
    src = src->next;
  }
  return 0;
}

Fl_Target_Dependency_Type *Fl_Target_Dependency_Type::next_dependency(Fl_Type *base) {
  Fl_Type *src = this->next;
  while (src && src->level>base->level) {
    if (src->is_target_dependency()) 
      return (Fl_Target_Dependency_Type*)src;
    src = src->next;
  }
  return 0;
}

char Fl_File_Type::file_is_cplusplus_code() {
  return (pFileType==FL_FILE_CPP_SOURCE);
}

char Fl_File_Type::file_is_cplusplus_header() {
  return (pFileType==FL_FILE_CPP_HEADER);
}

char Fl_File_Type::file_is_c_code() {
  return (pFileType==FL_FILE_C_SOURCE);
}

char Fl_File_Type::file_is_c_header() {
  return (pFileType==FL_FILE_C_HEADER);
}

char Fl_File_Type::file_is_objc_code() {
  return (pFileType==FL_FILE_OBJC_SOURCE);
}

char Fl_File_Type::file_is_objc_header() {
  return (pFileType==FL_FILE_OBJC_HEADER);
}

char Fl_File_Type::file_is_framework() {
  return (pFileType==FL_FILE_FRAMEWORK);
}

char Fl_File_Type::file_is_library() {
  return (pFileType==FL_FILE_LIBRARY);
}

char Fl_File_Type::file_is_fluid_ui() {
  return (pFileType==FL_FILE_FLUID_UI);
}

char Fl_File_Type::file_is_other() {
  return (pFileType==FL_FILE_TEXT);
}

char Fl_File_Type::file_is_code() {
  return (pFileType==FL_FILE_CPP_SOURCE)||(pFileType==FL_FILE_C_SOURCE)||(pFileType==FL_FILE_OBJC_SOURCE);
}

char Fl_File_Type::file_is_header() {
  return (pFileType==FL_FILE_CPP_HEADER)||(pFileType==FL_FILE_C_HEADER)||(pFileType==FL_FILE_OBJC_HEADER);
}

const char *Fl_File_Type::filename_name() {
  const char *fn = filename();
  if (fn) {
    return fltk3::filename_name(fn);
  }
  return 0;
}

const char *Fl_File_Type::filename_relative(const char *fnbase, const char *tgtbase) {
  char src_name[2048];
  static char result[2048];
  const char *fn = filename();
  if (fn) {
    strcpy(src_name, fnbase);
    strcat(src_name, fn);
    fltk3::filename_relative(result, 2048, src_name, tgtbase);
    return result;
  } else {
    return 0;
  }
}

void Fl_File_Type::open() {
  if (!the_file_panel) the_file_panel = make_file_panel();
  the_file_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_file_panel->show();
}

// ------------ Folder ---------------------------------------------------------

Fl_Folder_Type Fl_Folder_type;

Fl_Type *Fl_Folder_Type::make() {
  // make sure that this is a workspace file
  if (Fl_Type::first && !project_is_workspace()) {
    fltk3::message("Folders can only be used in Workspace files.");
    return 0;
  }
  // Folders can be inside folders of inside targets
  // Categories can be at the top level or inside categories
  Fl_Type *p = Fl_Type::current;
  while (p && !p->is_category() && !p->is_folder() && !p->is_target() && !p->is_workspace()) 
    p = p->parent;
  Fl_Folder_Type *o = new Fl_Folder_Type();
  o->name("Group");
  o->add(p);
  o->factory = this;
  return o;
}

void Fl_Folder_Type::open() {
  if (!the_file_panel) the_file_panel = make_file_panel();
  the_file_panel->load(&Fl_Type::is_tool);
  if (Fl_Panel::numselected) the_file_panel->show();
}

// ------------ Panel Base Class -----------------------------------------------

void *const Fl_Panel::LOAD = (void *)"LOAD"; // "magic" pointer to indicate that we need to load values into the dialog
int Fl_Panel::numselected = 0;
Fl_Type *Fl_Panel::current = 0L;

Fl_Panel::Fl_Panel(int x, int y, int w, int h, const char *name) 
: fltk3::DoubleWindow(w, h, name) {
}

Fl_Panel::~Fl_Panel() {
}

void Fl_Panel::propagate_load(fltk3::Group* g, void* v) {
  if (v == Fl_Panel::LOAD) {
    fltk3::Widget*const* a = g->array();
    for (int i=g->children(); i--;) {
      fltk3::Widget* o = *a++;
      o->do_callback(o,Fl_Panel::LOAD);
    }
  }
}


// FIXME: make this a method of Fl_Panel
// update the panel according to current widget set:
void Fl_Panel::load(RTTI_Query type_query) {
  if (!this) return;
  
  // find all the fltk3::Widget subclasses currently selected:
  numselected = 0;
  current = 0;
  if (Fl_Type::current) {
    if ((Fl_Type::current->*type_query)())
      current = Fl_Type::current;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if ((o->*type_query)() && o->selected) {
	numselected++;
	if (!current) current = o;
      }
    }
  }
  if (numselected)
    propagate_load(this);
  else
    hide();
}



//
// End of "$Id$".
//
