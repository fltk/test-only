//
// "$Id: Fl_Style.H,v 1.39.2.1 2005/01/27 21:24:33 rokan Exp $"
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

#ifndef Fl_Style_H
#define Fl_Style_H

class Fl_Widget;
typedef int (Fl_Draw_Style)(Fl_Widget *, Fl_Style *);
typedef int (Fl_Handle_Style)(Fl_Widget *, Fl_Style *, int);

const int FL_RETURN_ZERO = 1<<(8*sizeof(int)-2);

class Fl_Style{
protected:
  // Following points to a function which partialy or fully overrides handle() method if non-zero and handle() allows that.
  // The function is usually used at the beginning of particular handle() method. The function should return:
  // - 0 if you want to continue execution within the original handle method
  // - FL_RETURN_ZERO if you want to return immediately from original handle() method without following execution, handle then returns 0
  // - any other value will cause immediate return from original handle() method. The handle() handle will return the same non-zero value as this function.
  // See fl_handle_highlight(...) function in Fl_Style.cxx to see possible  implementation for widget highlighting.
public:
  Fl_Handle_Style * handle_; 
 
  // draw_style points to a function which partialy or fully overrides draw() method. The function should return:
  // - 0 if you want to contunure the exexution within original draw() method after its execution
  // - non-zero if you want to immediately return fromoriginal draw() method and in that way completely override its functionality
  Fl_Draw_Style * draw_;
protected:

  Fl_Handle_Style * handle_function(){return handle_;}
  Fl_Draw_Style * draw_function(){return draw_;}
protected:
  void handle_function(Fl_Handle_Style * f){handle_ = f;}
  void draw_function(Fl_Draw_Style * f){draw_ = f;}
public:
  Fl_Style():handle_(0),draw_(0){};
  virtual void apply(Fl_Widget *)=0; // updates the widget properties when FL_DAMAGE_STYLE damage bit is set
  virtual ~Fl_Style(){}

};


class Fl_Scheme_{
friend class Fl_Group;
  static Fl_Scheme_ * current_;
protected:
  static int index_;
public:
  static inline int index(){return index_;};
  virtual void current()=0;
  virtual ~Fl_Scheme_();
};


#define FL_IMPLEMENT_STYLE(Fl_Class) \
  Fl_Style * Fl_Class::style_=0; \
  void Fl_Class::apply_style(){ \
    if(style_) \
      style_->apply(this); \
    else if (Fl_Scheme_::index()) \
      revert_style(); \
  } \
  Fl_Style * Fl_Class::style(){return style_;} \
  void Fl_Class::style(Fl_Style * s){style_ = s;}


// Following macros can be used as a first comamd within widget's draw() and handle() methods. They allow modification of class draw() method for 
// complicated widgets (like Fl_Scroll) and allow to execute  alternative draw() and handle implementation supplied by particular style/theme. When used, you probably want to use
// also  FL_HANDLE_STYLE( ...) macro (see below)to match the widget response to its appereance. See also description of Fl_Style::draw member
// NOTE: these macros do not use style_handle() and and style_draw() getter functions but directly access style_ pointer which is private.
// This is SECURITY feature: subclasses should NOT use parents't styling functions directly because they can never know how styling engine will modify/mess up with them.
// style-specific handle and draw should always be associated with pariticular class, although many classes can use the same function (like highlighting button)
// as long as each class class style sets the pointer EXPLICITLY - no inheritance here.

#define fl_draw_style() { if(style_ && style_->draw_ && (*(style_->draw_)(this, style_)) return;}

#define fl_handle_style(event) { int return__; if(style_ && style_->handle_ && (return__ = (*(style_->handle_))(this, style_, event))) return ~FL_RETURN_ZERO & return__;}

#endif

//
// End of "$Id: Fl_Style.H,v 1.39.2.1 2005/01/27 21:24:33 rokan Exp $".
//
