//
// "$Id: Fl_Scheme.cxx,v 1.1.2.1 2005/01/27 21:24:40 rokan Exp $"
//
// Style header file for the Fast Light Tool Kit (FLTK).
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


#include <FL/Fl_Scheme.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Tool_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Browser_.H>

//  enum {COLOR=1, SELECTION_COLOR=2, BOX=4, ALIGN=8, LABELTYPE=16, LABELCOLOR=32, LABELFONT=64, LABELSIZE=128};

void Fl_Widget_Style::apply(Fl_Widget * w){
  w->try_color(color());
  w->try_selection_color(color());
  w->try_box(box());
  w->try_labeltype(labeltype());
  w->try_labelcolor(labelcolor());
  w->try_labelfont(labelfont());
  w->try_labelsize(labelsize());
};


void Fl_Button_Style::apply(Fl_Widget * w){
  Fl_Widget_Style::apply(w);
  ((Fl_Button *)w)->try_down_box(0); //down_box is deprecated for buttons
};

// This handle function just do color highlighting
static int fl_button_handle(Fl_Widget * w, Fl_Style * style, int event){
  Fl_Button * b = (Fl_Button *)w;
  Fl_Button_Style * s = (Fl_Button_Style *)style;
  uchar redraw = 0;
  switch (event){
    case FL_ENTER:
      if(b->takesevents() && b->active_r()){
        if(w->try_color(s->highlighted_color()) && !(b->value())) redraw = 1;
        if(w->try_selection_color(s->highlighted_color()) && b->value()) redraw = 1;
        if(redraw) b->redraw();
      }
      return 1;
    case FL_LEAVE:
      if(b->takesevents() && b->active_r()){
        if(w->try_color(s->color()) && !(b->value())) redraw = 1;
        if(w->try_selection_color(s->selection_color()) && b->value()) redraw = 1;
        if(redraw) b->redraw();
      }
      return 1;
  }
  return 0;
};

Fl_Widget_Style::Fl_Widget_Style():Fl_Style(),

  color_(FL_GRAY),
  box_(FL_NO_BOX),
  labeltype_(FL_NORMAL_LABEL),
  labelcolor_(FL_FOREGROUND_COLOR),
  labelfont_(FL_HELVETICA),
  labelsize_((uchar)FL_NORMAL_SIZE),
  selection_color_(FL_GRAY),
  set_flags_(0)

{
}

Fl_Button_Style::Fl_Button_Style(Fl_Widget_Style * parent):parent_(parent)
// ,down_box_(FL_NO_BOX)
{
 box_ = FL_UP_BOX;
 handle_ = &fl_button_handle;
};

Fl_Scheme::Fl_Scheme(){
  global = new Fl_Global_Style();
  button = new Fl_Button_Style(global);
  tool_button = new Fl_Button_Style(button);
  light_button = new Fl_Light_Button_Style(button);
  browser = new Fl_Browser__Style(global);
  update();
};


Fl_Scheme::~Fl_Scheme(){
  delete browser;
  delete button;
  delete tool_button;
  delete light_button;
  delete global;
};



void Fl_Scheme_::current(){
  Fl_Scheme_::current_=this;
  index_++;
}
void Fl_Scheme::current(){

//  if (Fl_Scheme_::current_) Fl_Scheme_::current_->remove();
  Fl_Button::style(button);
  Fl_Tool_Button::style(tool_button);
  tool_button->box(FL_NO_BOX);
  Fl_Light_Button::style(light_button);
  Fl_Browser_::style(browser);
}


void Fl_Scheme::update(){
  ((Fl_Widget_Style * )global)->update();
  ((Fl_Widget_Style * )button)->update();
  ((Fl_Widget_Style * )tool_button)->update();
  ((Fl_Widget_Style * )light_button)->update();

}
void Fl_Global_Style::update(){
  if(!(set_flags_ & COLOR)) color_ = FL_GRAY;
  if(!(set_flags_ & LABELTYPE)) labeltype_ = FL_NORMAL_LABEL;
  if(!(set_flags_ & LABELCOLOR)) labelcolor_ = FL_FOREGROUND_COLOR;
  if(!(set_flags_ & LABELFONT)) labelfont_ = FL_HELVETICA;
  if(!(set_flags_ & LABELSIZE)) labelsize_ = FL_NORMAL_SIZE;
  if(!(set_flags_ & SELECTION_COLOR)) selection_color_ = FL_SELECTION_COLOR;
};


void Fl_Browser__Style::update(){

  if (!(LABELTYPE & set_flags_)) labeltype_ =  parent_->labeltype();
  if (!(LABELCOLOR & set_flags_)) labelcolor_ =  parent_->labelcolor();
  if (!(LABELFONT & set_flags_)) labelfont_ = parent_->labelfont();
  if (!(LABELSIZE & set_flags_)) labelsize_ = parent_->labelsize();
  if (!(SELECTION_COLOR & set_flags_)) selection_color_ = parent_->selection_color();
  if (!(TEXTCOLOR & set_flags_)) labelcolor_ =  parent_->labelcolor();
  if (!(TEXTFONT & set_flags_)) textfont_ = parent_->textfont();
  if (!(TEXTSIZE & set_flags_)) textsize_ = parent_->textsize();
  
  // Different meaning....
  if (!(COLOR & set_flags_)) color_ = parent_->canvas_color();
};

void Fl_Button_Style::update(){
  // Following inherits values from the global parameters
  if (!(COLOR & set_flags_)) color_ = parent_->color();
  if (!(LABELTYPE & set_flags_)) labeltype_ =  parent_->labeltype();
  if (!(LABELCOLOR & set_flags_)) labelcolor_ =  parent_->labelcolor();
  if (!(LABELFONT & set_flags_)) labelfont_ = parent_->labelfont();
  if (!(LABELSIZE & set_flags_)) labelsize_ = parent_->labelsize();

  // this does not inherit parent color, different meaning
  if (!(set_flags_ & SELECTION_COLOR)) selection_color_ = color();

  // default values of new parameters
  if (!(set_flags_ & HIGHLIGHTED_COLOR)) highlighted_color_ = fl_lighter(color());
  if (!(set_flags_ & HIGHLIGHTED_BOX)) highlighted_box_ = box();
  //if (!(set_flags_ & HIGHLIGHTED_SELECTION_COLOR)) highlighted_selection_color_ = highlighted_color();
};

void Fl_Light_Button_Style::update(){
  if (!(BOX & set_flags_)) box_ = parent_->box();
  if (!(COLOR & set_flags_)) color_ = parent_->color();
  if (!(LABELTYPE & set_flags_)) labeltype_ =  parent_->labeltype();
  if (!(LABELCOLOR & set_flags_)) labelcolor_ =  parent_->labelcolor();
  if (!(LABELFONT & set_flags_)) labelfont_ = parent_->labelfont();
  if (!(LABELSIZE & set_flags_)) labelsize_ = parent_->labelsize();

  if (!(set_flags_ & HIGHLIGHTED_COLOR)) highlighted_color_ = ((Fl_Button_Style *)parent_)->highlighted_color();
  if (!(set_flags_ & HIGHLIGHTED_BOX)) highlighted_box_ = ((Fl_Button_Style *)parent_)->highlighted_box();
};


Fl_Light_Button_Style::Fl_Light_Button_Style(Fl_Button_Style * parent):Fl_Button_Style(parent){
  selection_color(FL_RED);
};












//
// End of "$Id: Fl_Scheme.cxx,v 1.1.2.1 2005/01/27 21:24:40 rokan Exp $".
//
