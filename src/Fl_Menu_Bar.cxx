//
// "$Id: Fl_Menu_Bar.cxx,v 1.8 1999/03/14 06:46:31 carl Exp $"
//
// Menu bar widget for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#define DEFAULT_STYLE ((Style*)default_style())

class TitleBox : public Fl_Widget {
public:
  Fl_Menu_Item* mi;
  Fl_Menu_* bar;
  int active() { return (mi->active() && bar->active_r()); }
  TitleBox() : Fl_Widget(0,0,0,0) { hide(); }
  void draw();
};

static TitleBox *current = 0, *last = 0;
static int e;

extern char fl_draw_shortcut;

void TitleBox::draw() {
  fl_clip(bar->x()+2, bar->y()+3, bar->w()-4, bar->h()-6);

  Fl_Boxtype bt;
  Fl_Color col, lc;
  if (Fl_Menu_Item::default_style.menu_item(Fl_Menu_Item::TITLE_FLY_BOX) &&
      current == this && active() && !Fl::pushed() && e != FL_LEAVE)
  {
    bt = (Fl_Boxtype)Fl_Menu_Item::default_style.menu_item(Fl_Menu_Item::TITLE_FLY_BOX);
    col = (Fl_Color)Fl_Menu_Item::default_style.menu_item(Fl_Menu_Item::TITLE_FLY_COLOR);
    lc = (Fl_Color)Fl_Menu_Item::default_style.menu_item(Fl_Menu_Item::TITLE_FLY_LABELCOLOR);
  } else {
    bt = FL_FLAT_BOX;
    col = color();
    lc = active() ? mi->labelcolor() : inactive(mi->labelcolor());
  }
  draw_box(bt, col);

  Fl_Label l;
  l.value = mi->label();
  l.type = mi->labelsize() ? mi->labeltype() : FL_NORMAL_LABEL;
  l.font = mi->labelsize() ? mi->labelfont() : bar->textfont();
  l.size = mi->labelsize() ? mi->labelsize() : bar->textsize();
  l.color = lc;
  int tmp = fl_draw_shortcut;
  fl_draw_shortcut = 1;
  l.draw(x()+Fl::box_dx(box()), y()+Fl::box_dy(box()), w()-Fl::box_dw(box()),
         h()-Fl::box_dh(box()), FL_ALIGN_CENTER);
  fl_draw_shortcut = tmp;

  fl_pop_clip();
}

void Fl_Menu_Bar::draw() {
  loadstyle();

  if (damage() & FL_DAMAGE_ALL) {
    draw_box();
    int tx = x() + 3;
    int i;
    Fl_Menu_Item* m;
    for (i = 0; i < FL_MAX_MENU_TITLES; i++) ((TitleBox*)titles[i])->mi = 0;
    for (i = 0, m = menu(); m && m->text; m = m->next(), i++) {
      int tw = m->measure(0, this) + 16;
      ((TitleBox*)titles[i])->mi = m;
      ((TitleBox*)titles[i])->bar = this;
      ((TitleBox*)titles[i])->resize(tx, y()+3, tw, h()-6);
      ((TitleBox*)titles[i])->color(color());
      ((TitleBox*)titles[i])->draw();
      tx += tw;
    }
  } else {
    if (current) current->draw();
    if (last) last->draw();
  }
}

int Fl_Menu_Bar::handle(int event) {
  const Fl_Menu_Item* v;
  int i;
  if (menu() && menu()->text) switch (event) {
  case FL_PUSH:
    v = 0;
J1:
    last = current; current = 0; damage(FL_DAMAGE_CHILD);
    v = menu()->pulldown(x(), y(), w(), h(), v, this, 0, 1);
    picked(v);
    return 1;
  case FL_SHORTCUT:
    v = menu()->test_shortcut();
    if (v) {picked(v); return 1;}
    if (visible_r() && (v = menu()->find_shortcut())) goto J1;
    return 0;
  case FL_ENTER:
  case FL_LEAVE:
  case FL_MOVE:
    e = event;
    last = current;
    current = 0;
    for (i = 0; i < FL_MAX_MENU_TITLES && ((TitleBox*)titles[i])->mi; i++) {
      TitleBox* t = (TitleBox*)titles[i];
      if (Fl::event_inside(t->x(), t->y(), t->w(), t->h())) {current = t; break;}
    }
    if (current != last) damage(FL_DAMAGE_CHILD);
    return 1;
  }

  return 0;
}

Fl_Menu_Bar::Style Fl_Menu_Bar::_default_style;

Fl_Menu_Bar::Style::Style() : Fl_Menu_::Style() {
  widget(BOX) = FL_MEDIUM_UP_BOX;
}

void Fl_Menu_Bar::loadstyle() {
  if (!Fl::s_menu_bar) {
    Fl::s_menu_bar = 1;

    static Fl::Attribute widget_attributes[] = {
      { "label color", LABELCOLOR },
      { "label size", LABELSIZE },
      { "label type", LABELTYPE },
      { "label font", LABELFONT },
      { "color", COLOR },
      { "down color", COLOR2 },
      { "box", BOX },
      { 0 }
    };
    Fl::load_attributes("menu bar", DEFAULT_STYLE->widget_, widget_attributes);

    static Fl::Attribute menu_attributes[] = {
      { "down box", DOWN_BOX },
      { "text font", TEXTFONT },
      { "text size", TEXTSIZE },
      { "text color", TEXTCOLOR },
      { 0 }
    };
    Fl::load_attributes("menu bar", DEFAULT_STYLE->menu_, menu_attributes);
  }
}

Fl_Menu_Bar::Fl_Menu_Bar(int x,int y,int w,int h,const char *l) : Fl_Menu_(x,y,w,h,l) {
  for (int i = 0; i < FL_MAX_MENU_TITLES; i++) titles[i] = new TitleBox;
}

//
// End of "$Id: Fl_Menu_Bar.cxx,v 1.8 1999/03/14 06:46:31 carl Exp $".
//
