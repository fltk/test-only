//
// "$Id$"
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
  class Fl_Style_List;
#endif


////////////////////////  Fl_Widget::Style  ////////////////////////////

#if defined(Fl_Widget_H) && !defined(Fl_Widget_Style_H)
# define Fl_Widget_Style_H


class FL_EXPORT Fl_Widget::Style {
    friend class Fl_Widget;
    friend class Fl_Style_List;

    // For dynamic stylles (STATIC in flags_ not set) this points to default style to geet particular parameter(type is Fl_Widget_Style *)
    // For static styles this points to the array of pointers (minimum size is 2) which depends on this style.
    // First one - if non-zero - points to the "highlighted" version, 


 

  private:
    unsigned flags_;
    void * next_;

    unsigned color_;
    Fl_Boxtype box_;
    //uchar box_;
    uchar labeltype_;
    unsigned labelcolor_;
    uchar labelfont_;
    uchar labelsize_;
    unsigned selection_color_;

  public:
    unsigned  flags() const {return flags_;}
    Style * highlight() const;
    Style * normal() const;


  protected:
    Style * highlight_() const { return highlight();}
    Style * normal_() const {return normal();}
    


    void set_flag(unsigned c){flags_ |=c;}
    void clear_flag(unsigned c){flags_ &= ~c;}


    // for use with clone()
    void init(Style * parent, unsigned mode); // for use in constructors 
    Style(Style * s): flags_(0), next_(s){};
    void next(Style * n){next_ = n;}
    Style * next() const {return (Style *)next_;}
    int damage(); // returns what needs to be redrawn when highlight is changed.
    Fl_Style_List * list() const {return (flags_ & STATIC) ? (Fl_Style_List *)next_ : 0;}

 
    // Following virtual functions must be overriden for each Style subclass
    void clear();
    // clears style s using this as a template
    // Funny stuff: using this function in the constructor can ""emulate"" the use of virtual function :-)
    



    /* "Virtual" constructor for making dynamic styles 
    * Must be overriden in every new subclass of Fl_Widget::Style */
    virtual Style * clone(){return new Style(this);} 


  protected:

  public:
    enum  {STATIC = 1,  HIGHLIGHT = 2, LABELTYPE=0x10, LABELCOLOR=0x20,
      LABELFONT=0x40, LABELSIZE=0x80, 
      COLOR=0x200, SELECTION_COLOR=0x400, BOX=0x800, 
      REDRAW_LABEL = 0x1F0, REDRAW = 0xFFFFFE00, ALL = 0xFFFFFFF0, NO_HIGHLIGHT = 0, NORMAL_HIGHLIGHT = HIGHLIGHT,
      BASE = 1, };
    static void update_(Style * s1, Style * s, unsigned what=ALL);
  protected:

    virtual void update(Style * s, unsigned what){update_(this, s, what);};
  public:

    Style(Fl_Widget::Style * parent, unsigned highlight_parent);
 

    

  // for hacking

    virtual ~Style();

  // These are inlined and fast - max one level digging...

    Fl_Labeltype labeltype() const {return ((STATIC|LABELTYPE) & flags_) ? (Fl_Labeltype)labeltype_ : (Fl_Labeltype)(((Style *)next_)->labeltype_);}
    Fl_Color labelcolor() const  {return ((STATIC|LABELCOLOR) & flags_) ? (Fl_Color)labelcolor_ : (Fl_Color)(((Style *)next_)->labelcolor_);}
    Fl_Font labelfont() const  {return ((STATIC|LABELFONT) & flags_) ? (Fl_Font)labelfont_ : (Fl_Font)(((Style *)next_)->labelfont_);}
    uchar labelsize() const  {return ((STATIC|LABELSIZE) & flags_) ? labelsize_ : ((Style * )next_)->labelsize_;}
    Fl_Color color()const {return ((STATIC|COLOR) & flags_) ? (Fl_Color)color_ : (Fl_Color)(((Style *)next_)->color_);}
    Fl_Color selection_color() const  {return ((STATIC|SELECTION_COLOR) & flags_) ? (Fl_Color)selection_color_ : (Fl_Color)(((Style *)next_)->selection_color_);}
    Fl_Boxtype box() const {return ((STATIC|BOX) & flags_) ? (Fl_Boxtype)box_ : (Fl_Boxtype)(((Style *)next_)->box_);}

    void labeltype(Fl_Labeltype t);
    void labelcolor(unsigned c);
    void labelfont(uchar f);
    void labelsize(uchar s);
    void color(unsigned c);
    void selection_color(unsigned c);
    void box(Fl_Boxtype b);
  };

typedef Fl_Widget::Style Fl_Widget_Style;

#endif // Fl_Widget_Style_H


///////////////////////  Fl_Button::Style  //////////////////////

#if defined(Fl_Button_H) && !defined(Fl_Button_Style_H)
# define Fl_Button_Style_H

class FL_EXPORT Fl_Button::Style : public Fl_Widget::Style {
  friend class Fl_Button;
  Fl_Boxtype down_box_;
  //uchar down_box_;
public:
    static void update_(Style* s1,  Fl_Widget::Style * s, unsigned what);
protected:
  Style(Style * s):Fl_Widget::Style(s){};

  //reimplementation of virtual functions for new style
  void clear();
  Fl_Widget::Style * clone(){return new Style(this);} 


  void update(Fl_Widget::Style * s, unsigned what){update_(this, s,what);}
public:
  enum {DOWN_BOX = 0x1000};
  Style * highlight(){return (Style *)highlight_();}
  Style * normal(){return (Style *)normal_();}


  Fl_Boxtype down_box() const {return ((STATIC|DOWN_BOX) & flags()) ? down_box_ : (Fl_Boxtype)(((Fl_Button::Style *)next())->down_box_);};
  void down_box(Fl_Boxtype b);
  Style(Fl_Widget::Style * parent, unsigned mode);
};

typedef Fl_Button::Style Fl_Button_Style;


#endif  // Fl_Button_Style_H



///////////////////////  Fl_Cancel_Button::Style  //////////////////////

#if defined(Fl_Cancel_Button_H) && !defined(Fl_Cancel_Button_Style_H)
# define Fl_Cancel_Button_Style_H

class FL_EXPORT Fl_Cancel_Button::Style : public Fl_Button::Style {
  friend class Fl_Cancel_Button;
  uchar symbol_align_;

protected:
  Style(Style * s):Fl_Button::Style(s){};

  //reimplementation of virtual functions for new style
  void clear();
  Fl_Widget::Style * clone(){return new Style(this);} 
  void cancel_button_update(Fl_Widget::Style * s, unsigned what);
  void update(Fl_Widget::Style * s, unsigned what){cancel_button_update(s,what);}
public:
  enum {SYMBOL_ALIGN = 0x2000};
  Style * highlight(){return (Style *)highlight_();}
  Style * normal(){return (Style *)normal_();}



  uchar symbol_align() const {return ((STATIC|SYMBOL_ALIGN) & flags()) ? symbol_align_ : (((Fl_Cancel_Button::Style *)next())->symbol_align_);};
  void symbol_align(uchar a);
  Style(Fl_Widget::Style * parent, unsigned mode);
};

typedef Fl_Cancel_Button::Style Fl_Cancel_Style;


#endif  // Fl_Button_Style_H



/** 
* \defgroup styles Styles and schemes within FLTK 
*
* The appereance and behaviour of widgets can be changed by changing current scheme of FLTK application.
* A scheme is an instance of Fl_Scheme class and can be made active using Fl::scheme(Fl_Scheme * s) function.
* A scheme is a collection of styles for particular classess. Each FLTK class has its own style which is changed when 
* a new scheme becomes active. A style defines default appereance of all widgets of particular class. Styles can change
* changing default values of widget parameters (such as color(), box() etc) as long as they are <b>not</b> explicitly set by
* the user. This means that once user sets particular parameter, this parameter will not be changed if hew scheme is applied.
* For all widgets styles can influence:

  <ul>
  <li> color() </li>
  <li> selection_color() </li>
  <li> box() </li>
  <li> labeltype() </li>
  <li> labelcolor() </li>
  <li> labelfont() </li>
  <li> labelsize() </li>
  </ul>

* For other widget classess  styles can influence patameters like

  <ul>
  <li> down_box() </li>
  <li> textcolor() </li>
  <li> textfont() </li>
  <li> textsize() </li>
  <li> ... </li>
  </ul>

* depending on the relevance.

* Schemes are also responsible for widget highlighting. Usualy they alternate widget color but all (or some) parameters 
* from above list can be changed if particular style wants to do so. Note that the alternation can not be performed
* (and highlighting is lost) if particular parameter  - such as color() -  is set explicitly.
* 
* One can apply partucular scheme by creating an instance of Fl_Scheme class, modifying some style values, updating scheme and setting it current:
*
* @code
* Fl_Scheme scheme; // creating new scheme
* scheme.button.color(0x99999900); // setting default color
* scheme.highlighted_button.color(0xAAAAAA00));  //setting highlight color somewhat lighter than hormal one
* scheme.update(); // updating style dependencies: you have always to call this function after you make some changes to a scheme
* scheme.set_current(); // applying scheme
* @endcode
*/







/** \name Style functions 
* @{ */

// Following macros can be used as a first comamd within widget's draw() and handle() methods. They allow modification of class draw() method for 
// complicated widgets (like Fl_Scroll) and allow to execute  alternative draw() and handle implementation supplied by particular style/theme. When used, you probably want to use
// also  FL_HANDLE_STYLE( ...) macro (see below)to match the widget response to its appereance. See also description of Fl_Style::draw member
// NOTE: these macros do not use style_handle() and and style_draw() getter functions but directly access style_ pointer which is private.
// This is SECURITY feature: subclasses should NOT use parents't styling functions directly because they can never know how styling engine will modify/mess up with them.
// style-specific handle and draw should always be associated with pariticular class, although many classes can use the same function (like highlighting button)
// as long as each class class style sets the pointer EXPLICITLY - no inheritance here.



// Invokes scheme-specific handle function.
/** This macro should be used as a first call within a draw() function with unique style, e.g.:
* \code
* int Fl_Class::handle(int event){
*   fl_handle_style(event);
*   my_other_handling code()
*   ...
* }
* \endcode 
* Normaly style-specific handle functions performs widget highlighting: they alternate widget parameters (such as widget colors)
* depending if the mouse cursor entered or leaved the widget. Note that this macro may imeriately return from
* handle() depenfing on style-specific handle function and particular event - see also Fl_Scheme::handle_function() */
// #define fl_handle_style(event) { int return__; if(style_ && (return__ = style_->handle(this, event))) return ~FL_RETURN_ZERO & return__;}

/// Calls scheme-specific draw function.
/** This macro  is used as a first call within draw() function of a class with unique style. It rarely does anything but it allows 
* partial or complete reimplementation of the widget appereance by new binary schemes. See also Fl_Scheme::draw_function() 
* Example:
* \code
* void Fl_Class::draw(){
*   fl_draw_style();
*   ... // my other drawing code
* }
* \endcode
* Depending on particular style draw function it may return from encapsulating draw() function
* without execution of other drawing code.*/
// #define fl_draw_style() { if(style_ && style_->draw(this)) return;}


//@}



//
// End of "$Id$".
//
