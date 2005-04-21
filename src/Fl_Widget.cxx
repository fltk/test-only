//
// "$Id$"
//
// Base widget class for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tooltip.H>
#include <FL/fl_draw.H>
#include <stdlib.h>
#include "flstring.h"

#include <FL/Fl_Style.H>
#include <FL/Fl_Style_List.H>


////////////////////////////////////////////////////////////////
// for compatability with Forms, all widgets without callbacks are
// inserted into a "queue" when they are activated, and the forms
// compatability interaction functions (fl_do_events, etc) will
// read one widget at a time from this queue and return it:

const int QUEUE_SIZE = 20;

static Fl_Widget *obj_queue[QUEUE_SIZE];
static int obj_head, obj_tail;

void Fl_Widget::default_callback(Fl_Widget *o, void * /*v*/) {
#if 0
  // This is necessary for strict forms compatability but is confusing.
  // Use the parent's callback if this widget does not have one.
  for (Fl_Widget *p = o->parent(); p; p = p->parent())
    if (p->callback() != default_callback) {
      p->do_callback(o,v);
      return;
    }
#endif
  obj_queue[obj_head++] = o;
  if (obj_head >= QUEUE_SIZE) obj_head = 0;
  if (obj_head == obj_tail) {
    obj_tail++;
    if (obj_tail >= QUEUE_SIZE) obj_tail = 0;
  }
}

Fl_Widget *Fl::readqueue() {
  if (obj_tail==obj_head) return 0;
  Fl_Widget *o = obj_queue[obj_tail++];
  if (obj_tail >= QUEUE_SIZE) obj_tail = 0;
  return o;
}
    
////////////////////////////////////////////////////////////////

int Fl_Widget::handle(int event) {
  return 0;
}


int FL_NORMAL_SIZE = 14;


//////////////// parameters ///////////////////////////



Fl_Boxtype Fl_Widget::box() const{
  return style_->box();
}
void Fl_Widget::box(Fl_Boxtype a) {
  if (Style::STATIC & style_->flags()) style_ = style_->clone();
  style_->box_ = a;
  style_->flags_ |= Style::BOX;
}


Fl_Color Fl_Widget::color() const{
  return style_->color();
}
void Fl_Widget::color(unsigned a) {
  if (Style::STATIC & style_->flags()) style_ = style_->clone();
  style_->color_=a;
  style_->flags_ |= Style::COLOR;
}

Fl_Color Fl_Widget::selection_color() const {
  return style_->selection_color();
}

void Fl_Widget::selection_color(unsigned a) {
  if (Style::STATIC & style_->flags()) style_ = style_->clone();
  style_->selection_color_ = a;
  style_->flags_ |= Style::SELECTION_COLOR;
}


Fl_Labeltype Fl_Widget::labeltype()  const{
  return style_->labeltype();
}

void Fl_Widget::labeltype(Fl_Labeltype a)  {
  if (Style::STATIC & style_->flags()) style_ = style_->clone(); 
  style_->labeltype_ = a;
  style_->flags_ |= Style::LABELTYPE;
}

Fl_Color Fl_Widget::labelcolor()  const{
  return style_->labelcolor();
}

void Fl_Widget::labelcolor(unsigned a)  {
  if (Style::STATIC & style_->flags()) style_ = style_->clone();
  style_->labelcolor_ = a;
  style_->flags_ |= Style::LABELCOLOR;
}

Fl_Font Fl_Widget::labelfont() const {
  return style_->labelfont();
}

void Fl_Widget::labelfont(uchar a)  {
  if (Style::STATIC & style_->flags()) style_ = style_->clone();
  style_->labelfont_ = a;
  style_->flags_ |= Style::LABELFONT;
}

uchar Fl_Widget::labelsize() const {
  return style_->labelsize();
}

void Fl_Widget::labelsize(uchar a) {
  if (Style::STATIC & style_->flags()) style_ = style_->clone();
  style_->labelsize_ = a;
  style_->flags_ |= Style::LABELSIZE;
}

 

 


////////////////////// Style List /////////////////////////



Fl_Style_List::Fl_Style_List(Fl_Widget::Style * parent, Fl_Widget::Style * style):
  parent_(0),
  children_(0),
  normal_(style),
  highlight_(0),
  prev_(0),
  next_(0),
  mask_(0),
  parent_mask_(0)
{
  if(parent){
    parent_ = parent->list();
    if(parent_->children_)  parent_->children_->prev_=this;
    next_=parent_->children_;
    parent_->children_=this;
  }
}




Fl_Style_List::~Fl_Style_List(){
  delete highlight_;
  if(prev_) 
    prev_->next_ = next_;
  else if(parent_)
    parent_->children_ = next_;
  if(next_){
    next_->prev_ = prev_;
    if(!prev_ && next_->parent_)
      next_->parent_->children_ = next_;
  };
  Fl_Style_List * c = children_;
  while(c){
    c->parent_ = 0;
    c = c->next_;
  }
}

///////////////////// Fl_Widget_Style ///////////////////////

void Fl_Widget::style(Style * s){
  if(!(Style::STATIC & s->flags())) return;
  
  if(Style::STATIC & style_->flags())
    style_ = s;
  else // dynamic style
    style_->next(s);
  
}

Fl_Widget::Style * Fl_Widget::style() const {
  //return style_;
  if(Style::STATIC & style_->flags())
    return style_;
  else // dynamic style
    return style_->next();
}


void Fl_Widget::dynamic_style(){
  if (Style::STATIC & style_->flags()) style_ = style_->clone();
}

Fl_Widget_Style * Fl_Widget_Style::highlight() const {
  return list()->highlight_;
}

Fl_Widget_Style * Fl_Widget_Style::normal() const {
  return list()->normal_;
}



Fl_Widget_Style::~Fl_Widget_Style(){
  if((flags_ & STATIC) && !(flags_ & HIGHLIGHT)) delete (Fl_Style_List *)next_;
}



void Fl_Widget_Style::init(Fl_Widget_Style * parent, unsigned mode){
  if(mode == BASE) return;  // not calling init at this stage
  Fl_Style_List * l = new Fl_Style_List(parent, this);
  next_ = l;
  l->mask_ = ~flags() & ~0xF;
  l->parent_mask_ = 0;

  if(parent) parent->update(this,ALL);
  if(!mode) return;
  l->highlight_ = clone();
  l->highlight_->flags_ = flags()| HIGHLIGHT;
  l->highlight_->next_ = l;
  update(l->highlight_, ALL);
  if(parent && parent->list()->highlight_){
    l->parent_mask_ = mode & l->mask_ & parent->list()->mask_;
    parent->list()->highlight_->update(list()->highlight_, mode); //updating according to parent highlight
  }
}




void Fl_Widget::highlight_(){
  Fl_Widget::Style * s = style()->highlight();
  if(s) style(s);
}

void Fl_Widget::normal_(){
  style(style()->normal());
}





void Fl_Widget::highlight(){
  if(!takesevents() || !active_r()) return;
  Style * s = style();
  Fl_Style_List * l = s->list();
  Style * h = l->highlight_;
  if(!h || (h == s)) return;
  style(h);
  int damage = h->flags();
  if(l->parent_ && l->parent_->highlight_)
    damage |= l->parent_mask_ & (l->normal_->flags() | l->parent_->highlight_->flags() | l->parent_->parent_mask_);
  damage &= l->mask_;
  if(damage & Style::REDRAW_LABEL) redraw_label();
  if(damage & Style::REDRAW) redraw();
}

void Fl_Widget::normal(){
  if(!takesevents() || !active_r()) return;
  Style * s = style();
  Fl_Style_List * l = s->list();
  Style * h = l->normal_;
  if(!h || (h == s)) return;
  style(h);
  int damage = l->highlight_->flags();
  if(l->parent_ && l->parent_->highlight_)
    damage |= l->parent_mask_ & (l->normal_->flags() | l->parent_->highlight_->flags() | l->parent_->parent_mask_);
  damage &= l->mask_;
  if(damage & Style::REDRAW_LABEL) redraw_label();
  if(damage & Style::REDRAW) redraw();
}









Fl_Widget_Style::Fl_Widget_Style(Fl_Widget_Style * parent, unsigned  mode){
  flags_ =  (~(COLOR|BOX|LABELTYPE|LABELCOLOR|LABELFONT|LABELSIZE|SELECTION_COLOR) & 0xFFFFFFF0) | STATIC;
  color_ = FL_GRAY;
  box_ = FL_NO_BOX;
  labeltype_ = FL_NORMAL_LABEL;
  labelcolor_ = FL_FOREGROUND_COLOR;
  labelfont_ = FL_HELVETICA;
  labelsize_ = FL_NORMAL_SIZE;
  selection_color_ = FL_GRAY;
  init(parent,mode);
}



Fl_Widget::Style * Fl_Widget::default_style(){
  static Fl_Widget::Style s(0, Style::NO_HIGHLIGHT);
  return &s;
}


void Fl_Widget_Style::update_(Fl_Widget_Style * s1, Fl_Widget_Style * s, unsigned what){
  if(s1 && s){
    what &= ~s->flags();
    if(COLOR & what) s->color_ = s1->color_;
    if(BOX & what) s->box_ = s1->box_;
    if(LABELTYPE & what) s->labeltype_ = s1->labeltype_;
    if(LABELCOLOR & what) s->labelcolor_ = s1->labelcolor_;
    if(LABELFONT & what) s->labelfont_ = s1->labelfont_;
    if(LABELSIZE & what) s->labelsize_ = s1->labelsize_;
    if(SELECTION_COLOR & what) s->selection_color_ = s1->selection_color_;
    Text * t1 = (Text *) s1->text();
    Text * t = (Text *) s->text();
    if(t1 && t){
      if(TEXTCOLOR & what) t->color_ = t1->color_;
      if(TEXTFONT & what) t->font_ = t1->font_;
      if(TEXTSIZE & what) t->size_ = t1->size_;
    }


  }
};
/*
void Fl_Widget::Style::clear(){
  color(FL_GRAY);
  box(FL_NO_BOX);
  labeltype(FL_NORMAL_LABEL);
  labelcolor(FL_FOREGROUND_COLOR);
  labelfont(FL_HELVETICA);
  labelsize(FL_NORMAL_SIZE);
  selection_color(FL_GRAY);
};
*/





void Fl_Widget_Style::textcolor(unsigned c){ 
  ((Text *)text())->color_ = c; 
  flags_ |= TEXTCOLOR;
  fl_update_styles(Fl_Widget_Style,textcolor, TEXTCOLOR, c);
}

void Fl_Widget_Style::textfont(uchar f){
  ((Text *)text())->font_ = f; 
  flags_ |= TEXTFONT;
  fl_update_styles(Fl_Widget_Style,textfont, TEXTFONT, f);
}

void Fl_Widget_Style::textsize(uchar s){
  ((Text *)text())->size_ = s; 
  flags_ |= TEXTSIZE;
  fl_update_styles(Fl_Widget_Style,textsize, TEXTSIZE, s);
}

///////////////



void Fl_Widget_Style::color(unsigned c){
  color_ = c;
  flags_ |=COLOR;
  fl_update_styles(Fl_Widget_Style,color, COLOR, c);
}


void Fl_Widget_Style::labeltype(Fl_Labeltype t){
  labeltype_=t;
  flags_|= LABELTYPE;
  fl_update_styles(Fl_Widget_Style,labeltype,LABELTYPE, t);
}


void Fl_Widget_Style::labelcolor(unsigned c){ 
  labelcolor_ = c; 
  flags_ |= LABELCOLOR;
  fl_update_styles(Fl_Widget_Style,labelcolor, LABELCOLOR, c);
}

void Fl_Widget_Style::labelfont(uchar f){
  labelfont_ = f; 
  flags_ |= LABELFONT;
  fl_update_styles(Fl_Widget_Style,labelfont, LABELFONT, f);
}

void Fl_Widget_Style::labelsize(uchar s){
  labelsize_ = s; 
  flags_ |= LABELSIZE;
  fl_update_styles(Fl_Widget_Style,labelsize, LABELSIZE, s);
}
void Fl_Widget_Style::selection_color(unsigned c){
  selection_color_ = c;
  flags_ |= SELECTION_COLOR;
  fl_update_styles(Fl_Widget_Style,selection_color, SELECTION_COLOR, c);

}
void Fl_Widget_Style::box(Fl_Boxtype b) {
  box_= b;
  flags_ |= BOX;
  fl_update_styles(Fl_Widget_Style,box,BOX,b);
}







  

Fl_Widget::Fl_Widget(int X, int Y, int W, int H, const char* L) {

  x_ = X; y_ = Y; w_ = W; h_ = H;

  label_	 = L;
  image_   = 0;
  deimage_ = 0;
  style_ =  default_style();
  tooltip_       = 0;
  callback_	 = default_callback;
  user_data_ 	 = 0;
  type_		 = 0;
  flags_	 = VISIBLE_FOCUS;

  align_	 = FL_ALIGN_CENTER;
  when_		 = FL_WHEN_RELEASE;

  parent_ = 0;
  if (Fl_Group::current()) Fl_Group::current()->add(this);
}




void Fl_Widget::resize(int X, int Y, int W, int H) {
  x_ = X; y_ = Y; w_ = W; h_ = H;
}

// this is useful for parent widgets to call to resize children:
int Fl_Widget::damage_resize(int X, int Y, int W, int H) {
  if (x() == X && y() == Y && w() == W && h() == H) return 0;
  resize(X, Y, W, H);
  redraw();
  return 1;
}

int Fl_Widget::take_focus() {
  if (!takesevents()) return 0;
  if (!visible_focus()) return 0;
  if (!handle(FL_FOCUS)) return 0; // see if it wants it
  if (contains(Fl::focus())) return 1; // it called Fl::focus for us
  Fl::focus(this);
  return 1;
}

extern void fl_throw_focus(Fl_Widget*); // in Fl_x.cxx

// Destruction does not remove from any parent group!  And groups when
// destroyed destroy all their children.  This is convienent and fast.
// However, it is only legal to destroy a "root" such as an Fl_Window,
// and automatic destructors may be called.
Fl_Widget::~Fl_Widget() {
  if (flags() & COPIED_LABEL) free((void *)(label_));
  if(!(style_->flags() & Style::STATIC)) delete style_;
  parent_ = 0; // Don't throw focus to a parent widget.
  fl_throw_focus(this);
}

// draw a focus box for the widget...
void
Fl_Widget::draw_focus(Fl_Boxtype B, int X, int Y, int W, int H) const {
  if (!Fl::visible_focus()) return;
  if(B == FL_DOWN_BOX ||
     B == FL_DOWN_FRAME ||
     B == FL_THIN_DOWN_BOX ||
     B == FL_THIN_DOWN_FRAME){
      X ++;
      Y ++;
  }
  fl_color(fl_contrast(FL_BLACK, color()));

#if defined(WIN32) || defined(__APPLE__)
  // Windows 95/98/ME do not implement the dotted line style, so draw
  // every other pixel around the focus area...
  //
  // Also, QuickDraw (MacOS) does not support line styles specifically,
  // and the hack we use in fl_line_style() will not draw horizontal lines
  // on odd-numbered rows...
  int i, xx, yy;

  X += Fl::box_dx(B);
  Y += Fl::box_dy(B);
  W -= Fl::box_dw(B) + 2;
  H -= Fl::box_dh(B) + 2;

  for (xx = 0, i = 1; xx < W; xx ++, i ++) if (i & 1) fl_point(X + xx, Y);
  for (yy = 0; yy < H; yy ++, i ++) if (i & 1) fl_point(X + W, Y + yy);
  for (xx = W; xx > 0; xx --, i ++) if (i & 1) fl_point(X + xx, Y + H);
  for (yy = H; yy > 0; yy --, i ++) if (i & 1) fl_point(X, Y + yy);
#else
  fl_line_style(FL_DOT);
  fl_rect(X + Fl::box_dx(B), Y + Fl::box_dy(B),
          W - Fl::box_dw(B) - 1, H - Fl::box_dh(B) - 1);
  fl_line_style(FL_SOLID);
#endif // WIN32
}


void Fl_Widget::activate() {
  if (!active()) {
    clear_flag(INACTIVE);
    if (active_r()) {
      redraw();
      redraw_label();
      handle(FL_ACTIVATE);
      if (inside(Fl::focus())) Fl::focus()->take_focus();
    }
  }
}

void Fl_Widget::deactivate() {
  if (active_r()) {
    set_flag(INACTIVE);
    redraw();
    redraw_label();
    handle(FL_DEACTIVATE);
    fl_throw_focus(this);
  } else {
    set_flag(INACTIVE);
  }
}

int Fl_Widget::active_r() const {
  for (const Fl_Widget* o = this; o; o = o->parent())
    if (!o->active()) return 0;
  return 1;
}

void Fl_Widget::show() {
  if (!visible()) {
    clear_flag(INVISIBLE);
    if (visible_r()) {
      redraw();
      redraw_label();
      handle(FL_SHOW);
      if (inside(Fl::focus())) Fl::focus()->take_focus();
    }
  }
}

void Fl_Widget::hide() {
  if (visible_r()) {
    set_flag(INVISIBLE);
    for (Fl_Widget *p = parent(); p; p = p->parent())
      if (p->box() || !p->parent()) {p->redraw(); break;}
    handle(FL_HIDE);
    fl_throw_focus(this);
  } else {
    set_flag(INVISIBLE);
  }
}

int Fl_Widget::visible_r() const {
  for (const Fl_Widget* o = this; o; o = o->parent())
    if (!o->visible()) return 0;
  return 1;
}

// return true if widget is inside (or equal to) this:
// Returns false for null widgets.
int Fl_Widget::contains(const Fl_Widget *o) const {
  for (; o; o = o->parent_) if (o == this) return 1;
  return 0;
}


void
Fl_Widget::label(const char *a) {
  if (flags() & COPIED_LABEL) {
    free((void *)(label_));
    clear_flag(COPIED_LABEL);
  }
  label_=a;
  redraw_label();
}


void
Fl_Widget::copy_label(const char *a) {
  if (flags() & COPIED_LABEL) free((void *)(label_));
  set_flag(COPIED_LABEL);
  label_=strdup(a);
  redraw_label();
}




Fl_Label Fl_Widget::get_label() const {
  Fl_Label l;
  l.value = label_;
  l.image = image_;
  l.deimage = deimage_;
  l.type = labeltype();
  l.font = labelfont();
  l.size = labelsize();
  l.color = labelcolor();
  return l;
};


void Fl_Widget::measure_label(int &xx, int &yy){
  (get_label()).measure(xx,yy);
};





//
// End of "$Id$".
//
