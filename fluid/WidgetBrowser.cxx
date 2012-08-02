//
// "$Id$"
//
// WidgetBrowser code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#include "WidgetBrowser.h"
#include "pixmaps.h"
#include "fluid.h"
#include "widget_panel_actions.h"

#include <fltk3/run.h>
#include <fltk3/ask.h>
#include <fltk3/Browser_.h>
#include <fltk3/MenuWindow.h>
#include <fltk3/draw.h>
#include <fltk3/filename.h>
#include <stdlib.h>
#include "../src/fltk3/flstring.h"
#include <stdio.h>
#include <fltk3/Pixmap.h>

#include "Fl_Type.h"
#include "WorkspaceType.h"
#include "workspace_panel.h"
#include "undo.h"


fluid3::WidgetBrowser *widget_browser = 0L;

extern int show_comments;
extern Fl_Panel *the_workspace_panel;
extern char *get_temporary_return_buffer(int size);



fltk3::Widget *make_widget_browser(int x,int y,int w,int h) {
  return (widget_browser = new fluid3::WidgetBrowser(x,y,w,h));
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

static void Widget_Browser_callback(fltk3::Widget *o,void *) {
  ((fluid3::WidgetBrowser*)o)->callback();
}

fluid3::WidgetBrowser::WidgetBrowser(int X,int Y,int W,int H,const char*l)
: fltk3::Browser_(X,Y,W,H,l) {
  type(fltk3::MULTI_BROWSER);
  Widget::callback(Widget_Browser_callback);
  when(fltk3::WHEN_RELEASE|fltk3::WHEN_NOT_CHANGED);
}

void *fluid3::WidgetBrowser::item_first() const {return Fl_Type::first;}

void *fluid3::WidgetBrowser::item_next(void *l) const {return ((Fl_Type*)l)->next;}

void *fluid3::WidgetBrowser::item_prev(void *l) const {return ((Fl_Type*)l)->prev;}

int fluid3::WidgetBrowser::item_selected(void *l) const {return ((Fl_Type*)l)->new_selected;}

void fluid3::WidgetBrowser::item_select(void *l,int v) {((Fl_Type*)l)->new_selected = v;}

int fluid3::WidgetBrowser::item_height(void *l) const {
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


int fluid3::WidgetBrowser::incr_height() const 
{
  return textsize()+2;
}


static Fl_Type* pushedtitle;

void fluid3::WidgetBrowser::item_draw(void *v, int X, int Y, int, int) const {
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
      int b,l=strlen(c);           // size in bytes
      for (int i = 20; i>0;i--) {  // maximum 20 characters
        if (*c==0) break;          // end of string
        fltk3::utf8decode(c, c+l, &b); // b=size of char in bytes
        if (b==-1) break;          // some error - leave
        l-=b;                      // l = bytes left in string
        while (b--)*p++ = *c++;    // copy that character into the buffer
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

int fluid3::WidgetBrowser::item_width(void *v) const {
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
    // FIXME: utf8!
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


extern Fl_Panel* the_widget_panel;

/*
 This method allow editing simple text inline.
 
 When clicking an already selected editable item, this method will pop up a 
 small window that allow for quick editing of the item value or label.
 
 Making sure that a second click is not confused with a double click takes
 some effort in the callback code, causing a timeout, which in turn is canceled
 by a double click.
 */
void fluid3::WidgetBrowser::handle_second_single_click()
{
  if (!last_clicked) 
    return;
  // find the text that we will edit first
  const char* text = 0L;
  if (last_clicked->is_file()) {
    text = ((Fl_File_Type*)last_clicked)->filename();
  } else if (last_clicked->name()) {
    text = last_clicked->name();
  } else if (last_clicked->label()) {
    text = last_clicked->label();
  } else {
    return;
  }
  // count the lines in the text
  int nlines = 1;
  const char *s = text;
  while (*s) { if (*s=='\n') nlines++; s++; }
  if (nlines>10) nlines = 10; 
  // find the y position of the clicked item (the mouse event is no longer reliable)
  int yp = -position();
  Fl_Type *t = (Fl_Type*)item_first();
  while (t && t!=last_clicked) {
    yp += item_height(t);
    t = (Fl_Type*)item_next(t);
  }
  if (!t) 
    return;
  // if the item has a comment, skip that
  if (t->comment())
    yp += textsize()-1;
  // find the x position and width of the name
  fltk3::font(textfont(), textsize());
  int xp = x() + fltk3::box_dx(box()) + 3 + 18 + 8 + t->level * 12;
  if (t->is_widget() || t->is_class()) {
    const char* c = subclassname(t);
    if (!strncmp(c,"fltk3::",7)) c += 7;
    xp += int(fltk3::width(c)+fltk3::width('n'));
  }
  // use the widget width minus the x position, but use some minimum width 
  int wp = w()-xp-fltk3::box_dx(box());
  if (scrollbar.visible()) wp-=scrollbar_width();
  if (nlines>1) {
    if (wp<320) wp = 320;
  } else {
    if (wp<200) wp = 200;
  }
  int hp = nlines*fltk3::height()+fltk3::DOWN_BOX->dh(); 
  // adjust coordinates to global and use a menu window
  xp += dx_window() + window()->x();
  yp += dy_window() + window()->y();
  // open a modal dialog with an input widget
  fltk3::PopupWindow* w = new fltk3::PopupWindow(xp, yp, wp, hp);
  fltk3::Input *input = new fltk3::Input(0, 0, wp, hp);
  if (nlines) input->type(fltk3::MULTILINE_INPUT);
  input->box(fltk3::DOWN_BOX);
  input->textfont(textfont());
  input->textsize(textsize());
  input->value(text);
  input->position(0, strlen(input->value()));
  input->callback(w->hide_i, w);
  input->when(fltk3::WHEN_ENTER_KEY_ALWAYS);
  if (w->popup()==input) {
    int mod = 0;
    if (strcmp(input->value(), text)) {
      // FIXME: there are certain illegal values that must be avoided!
      if (last_clicked->is_file()) {
        ((Fl_File_Type*)t)->filename(input->value());
      } else if (t->name()) {
        t->name(input->value());
      } else if (t->label()) {
        t->label(input->value());
      }
      mod = 1;
    }
    redraw_browser();
    if (mod) {
      if (the_workspace_panel)
        Fl_Panel::propagate_load(the_workspace_panel);
      if (the_widget_panel) 
        Fl_Panel::propagate_load(the_widget_panel);
      set_modflag(1);
    }
  }
  delete w;
}


void fluid3::WidgetBrowser::handle_second_single_click_i(void* d)
{
  ((fluid3::WidgetBrowser*)d)->handle_second_single_click();
}


void fluid3::WidgetBrowser::callback() {
  if (changed()) {
    // update evrything around the new selection
    selection_changed((Fl_Type*)selection());
  } else {
    // on a single click, we may be able to edit some value directly
    if ( fltk3::event()==fltk3::RELEASE ) {
      last_clicked = (Fl_Type*)find_item(fltk3::event_y());
      if (last_clicked && last_clicked->selected) {
        // add a timeout to make sure that we have a single click, not a double click
        fltk3::add_timeout(0.5, handle_second_single_click_i, this);      
      }
    }
  }
}


int fluid3::WidgetBrowser::handle(int e) {
  static Fl_Type *title;
  Fl_Type *l;
  int X,Y,W,H; bbox(X,Y,W,H);
  if (e!=fltk3::MOVE) {
    fltk3::remove_timeout(handle_second_single_click_i, this);
  }
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
        if (l && l->new_selected && (fltk3::event_clicks() || fltk3::event_state(fltk3::CTRL))) {
            l->open();
        }
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


//
// End of "$Id$".
//
