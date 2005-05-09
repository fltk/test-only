//
// "$Id$"
//
// Style header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
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

////////////////////////Basic handle finction for highlighting /////////////////




static int fl_button_highlight(Fl_Widget * w, Fl_Style * style, int event){

  Fl_Button_Style * s;
  Fl_Button_Style * bs = (Fl_Button_Style * )style;
  switch(event){
  case FL_LEAVE:
    s = bs;
    break;
  case FL_ENTER:
    s = (Fl_Button_Style * ) bs->data();
    break;
  default:
    return 0;
  }

  if(!(w->takesevents() && w->active_r())) return 1;
  unsigned long flags = (unsigned long)(((Fl_Widget_Style *)(bs->data()))->data());
  
  uchar redraw = 0;
  uchar redraw_label = 0;

  if((flags & Fl_Widget_Style::COLOR) && (w->try_color(s->color()))) redraw = 1;
  if((flags & Fl_Widget_Style::SELECTION_COLOR) && (w->try_selection_color(s->selection_color()))) redraw = 1;
  if((flags & Fl_Widget_Style::BOX) && (w->try_box(s->box()))) redraw = 1;
  if((flags & Fl_Widget_Style::DOWN_BOX) && (((Fl_Button * )w)->try_down_box(s->down_box()))) redraw = 1;
  
  if((flags & Fl_Widget_Style::LABELFONT) && (w->try_labelfont(s->labelfont()))) redraw_label = 1;
  if((flags & Fl_Widget_Style::LABELTYPE) && (w->try_labeltype(s->labeltype()))) redraw_label = 1;
  if((flags & Fl_Widget_Style::LABELCOLOR) && (w->try_labelcolor(s->labelcolor()))) redraw_label = 1;
  if((flags & Fl_Widget_Style::LABELSIZE) && (w->try_labelsize(s->labelsize()))) redraw_label = 1;
  
  if(redraw) w->redraw();
  if(redraw_label) w->redraw_label();
  
  return 1;
};











////////////////////////////////////   Scheme functions ///////////////////////

Fl_Scheme::~Fl_Scheme(){
  /*
  delete browser;
  delete highlighted_button;
  delete button;
  delete highlighted_tool_button;
  delete tool_button;
  delete highlighted_tool_button;
  */
};

Fl_Scheme::Fl_Scheme():
  button(0),
  highlighted_button(&button),
  tool_button(&button),
  highlighted_tool_button(&tool_button)
{
  button.selection_color(FL_GRAY);
  button.box(FL_UP_BOX);
  button.data(&highlighted_button);
 
  tool_button.box(FL_NO_BOX);
  tool_button.down_box(FL_NO_BOX);
  tool_button.color(FL_NO_COLOR);
  tool_button.selection_color(FL_NO_COLOR);
  
  highlighted_tool_button.box(FL_UP_FRAME);

  update();
};

void Fl_Scheme_::current(){
  Fl_Scheme_::current_=this;
  index_++;
}
void Fl_Scheme::current(){

//  if (Fl_Scheme_::current_) Fl_Scheme_::current_->remove();
  Fl_Button::style(&button);
  Fl_Tool_Button::style(&tool_button);
  Fl_Browser_::style(&browser);
}

static void set_general_highlight(Fl_Widget_Style * style, Fl_Widget_Style * highlighted_style, Fl_Handle_Style * function){
  if(!((highlighted_style == style->data())||(Fl_Widget_Style::HANDLE & style->set_flags()) || (Fl_Widget_Style::DATA & highlighted_style->set_flags()))){ // setting handle function for highlighting
    highlighted_style->data((void *)(highlighted_style->compare_flags())); // setting flags for comparison
    highlighted_style->set_flags(~Fl_Widget_Style::DATA & highlighted_style->set_flags()); //setting flag back
    style->handle_function(function); // setting highlighting function
    style->set_flags(~Fl_Widget_Style::HANDLE & style->set_flags()); //setting flag back
    style->data(highlighted_style); // setting pointer to highlighted version
    style->set_flags(~Fl_Widget_Style::DATA & style->set_flags()); //setting flag back
  }
}

 


void Fl_Scheme::update(){


  ////////// Button ////////////////
  button.update();
  highlighted_button.update();
  if(!(Fl_Widget_Style::COLOR & highlighted_button.set_flags())){
    highlighted_button.color(fl_lighter(button.color()));
    highlighted_button.set_flags(~Fl_Widget_Style::COLOR & highlighted_button.set_flags()); //clearing flag back
  };
  if(!(Fl_Widget_Style::SELECTION_COLOR & highlighted_button.set_flags())){
    highlighted_button.selection_color(fl_lighter(button.color()));
    highlighted_button.set_flags(~Fl_Widget_Style::SELECTION_COLOR & highlighted_button.set_flags()); //clearing flag back
  };
  set_general_highlight(&button, &highlighted_button, &fl_button_highlight);

  /////////// Tool_Button ///////////
  tool_button.update();
  highlighted_tool_button.update();
  set_general_highlight(&tool_button, &highlighted_tool_button, &fl_button_highlight);

  
  //////////// Browser /////////////////
  browser.update();


}





///////////////////////////// Base style ///////////////////////////////////////

int Fl_Widget_Style::compare_flags(){
  if(!parent_) return 0;
  int flags = 0;
  if(parent_->color() != color()) flags = COLOR;
  if(parent_->selection_color() != selection_color()) flags |= SELECTION_COLOR;
  if(parent_->box() != box()) flags |= BOX;
  if(parent_->labeltype() != labeltype()) flags |= LABELTYPE;
  if(parent_->labelcolor() != labelcolor()) flags |= LABELCOLOR;
  if(parent_->labelfont() != labelfont()) flags |= LABELFONT;
  if(parent_->labelsize() != labelsize()) flags |= LABELSIZE;
  return flags;
};


void Fl_Widget_Style::apply(Fl_Widget * w){
  w->try_color(color());
  w->try_selection_color(selection_color());
  w->try_box(box());
  w->try_labeltype(labeltype());
  w->try_labelcolor(labelcolor());
  w->try_labelfont(labelfont());
  w->try_labelsize(labelsize());
};


Fl_Widget_Style::Fl_Widget_Style(Fl_Widget_Style * parent):
  Fl_Style(),
  parent_(parent),
  color_(FL_GRAY),
  box_(FL_NO_BOX),
  labeltype_(FL_NORMAL_LABEL),
  labelcolor_(FL_FOREGROUND_COLOR),
  labelfont_(FL_HELVETICA),
  labelsize_((uchar)FL_NORMAL_SIZE),
  selection_color_(FL_SELECTION_COLOR),
  set_flags_(0),
  data_(0)

{
}

  
void Fl_Widget_Style::update(){
  if(!parent_) return;
  if (!(COLOR & set_flags_)) color_ = parent_->color();
  if (!(LABELTYPE & set_flags_)) labeltype_ =  parent_->labeltype();
  if (!(LABELCOLOR & set_flags_)) labelcolor_ =  parent_->labelcolor();
  if (!(LABELFONT & set_flags_)) labelfont_ = parent_->labelfont();
  if (!(LABELSIZE & set_flags_)) labelsize_ = parent_->labelsize();
  if (!(set_flags_ & SELECTION_COLOR)) selection_color_ = parent_->selection_color();
  if (!(set_flags_ & BOX)) box_ = parent_->box();
  /*
  if (!(set_flags_ & background_color_)){
    background_color(parent_->background_color());
    set_flags_ &= ~background_color_; // we need to reset background_color_ bit back because above call sets it
  };
  */
};

////////////////////////////// Button styles ////////////////////////////////////////////
// This handle function just do color highlighting

Fl_Button_Style::Fl_Button_Style(Fl_Button_Style * parent):Fl_Widget_Style(parent)
{
 down_box_ = 0;
};

int Fl_Button_Style::compare_flags(){
  if(!parent_) return 0;
  int flags = Fl_Widget_Style::compare_flags();
  if(((Fl_Button_Style *)parent_)->down_box() != down_box()) flags |= DOWN_BOX;
  return flags;
};

void Fl_Button_Style::apply(Fl_Widget * w){
  Fl_Widget_Style::apply(w);
  ((Fl_Button *)w)->try_down_box(0); 
};



void Fl_Button_Style::update(){
  if(!parent_) return;
  Fl_Widget_Style::update();
  if (!(DOWN_BOX & set_flags_)) down_box_ = ((Fl_Button_Style *)parent_)->down_box();
};
////////////////////////////////////////////////////////////





void Fl_Browser_Style::update(){
  if(!parent_) return;

  Fl_Widget_Style::update();
  if (!(TEXTCOLOR & set_flags_)) labelcolor_ =  ((Fl_Browser_Style *)parent_)->labelcolor();
  if (!(TEXTFONT & set_flags_)) textfont_ = ((Fl_Browser_Style *)parent_)->textfont();
  if (!(TEXTSIZE & set_flags_)) textsize_ = ((Fl_Browser_Style *)parent_)->textsize();
};











//
// End of "$Id$".
//
