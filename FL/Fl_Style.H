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

#ifndef Fl_Style_H
  #define Fl_Style_H
  class Fl_Style_List;
#endif


////////////////////////  Fl_Widget::Style  ////////////////////////////

#if defined(Fl_Widget_H) && !defined(Fl_Widget_Style_H)
# define Fl_Widget_Style_H


class FL_EXPORT Fl_Widget_Style {
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

    enum  {STATIC = 1,  HIGHLIGHT = 2,
      LABELTYPE=0x10, LABELCOLOR=0x20,
      LABELFONT=0x40, LABELSIZE=0x80, 
      COLOR=0x200, SELECTION_COLOR=0x400,
      BOX=0x800,   DOWN_BOX = 0x1000,
      SYMBOL_ALIGN = 0x2000,
      TEXTCOLOR = 0x4000, TEXT_COLOR = TEXTCOLOR,
      TEXTFONT = 0x8000, TEXT_FONT = TEXTFONT, 
      TEXTSIZE = 0x10000, TEXT_SIZE = TEXTSIZE,
      ARROWS =   0x20000,

       // masks to calculate damage to perform redwaw() and/or redraw_label() for highlighting
      REDRAW_LABEL = 0x1F0, REDRAW = 0xFFFFFE00,
      // constructor patameters for highlighting and its inheritance
      ALL = ~0xF, NO_HIGHLIGHT = 0, NORMAL_HIGHLIGHT = HIGHLIGHT,
      BASE = 1 // BASE should be always used for initialization of base class within the constructor
    };


    unsigned  flags() const {return flags_;}
    Fl_Widget_Style * highlight() const;
    Fl_Widget_Style * normal() const;


  protected:

    void init(Fl_Widget_Style * parent, unsigned mode); // for use in constructors 

    Fl_Widget_Style(Fl_Widget_Style * s): flags_(0), next_(s){};
    void next(Fl_Widget_Style * n){next_ = n;}
    Fl_Widget_Style * next() const {return (Fl_Widget_Style *)next_;}
    int damage(); // returns what needs to be redrawn when highlight is changed.
    Fl_Style_List * list() const {return (flags_ & STATIC) ? (Fl_Style_List *)next_ : 0;}


    // "Text" parameters are comolly used in various classes.
    // Unfortunately they do not have the same base class with
    // the same implementation of textfont textcolor and textsize. To overcome this we have 
    // a text virtual function which should be overriden in a style subclass which uses a text 
    // and which should return an address of Text structure placement. Also protected members 
    // textfont(), textcolor()m textsize()  should be "published" using "using" parameter.
    // Particular widget "text" methods should be implemented as usual.
    
    struct Text{
        unsigned color_;
        uchar font_;
        uchar size_;
    };
    // This should be reimplemented in a style introducting "text" visuals and returning a pointer
    // to particular placement of "text" structure
    virtual const Text * text() const {return 0;};

    // Following should be just made public

    Fl_Color textcolor() const  {return ((STATIC|TEXTCOLOR) & flags_) ? (Fl_Color)(text()->color_) : (Fl_Color)(((Fl_Widget_Style * )next_)->text()->color_);}
    Fl_Font textfont() const  {return ((STATIC|TEXTFONT) & flags_) ? (Fl_Font)(text()->font_) : (Fl_Font)(((Fl_Widget_Style * )next_)->text()->font_);}
    uchar textsize() const  {return ((STATIC|TEXTSIZE) & flags_) ? text()->size_ : next()->text()->size_;}

    void textcolor(unsigned c);
    void textfont(uchar f);
    void textsize(uchar s);




    Fl_Widget_Style * highlight_() const { return highlight();}
    Fl_Widget_Style * normal_() const {return normal();}
    


    void set_flag(unsigned c){flags_ |=c;}
    



 
    // Following virtual functions must be overriden for each Style subclass



    /* "Virtual" constructor for making dynamic styles 
    * Must be overriden in every new subclass of Fl_Widget_Style */
    virtual Fl_Widget_Style * clone(){return new Fl_Widget_Style(this);} 






  public:
    void clear_flag(unsigned c){flags_ &= ~c;}

//    static void update_(Fl_Widget_Style * s1, Fl_Widget_Style * s, unsigned what=ALL);
  protected:

    virtual void update(Fl_Widget_Style * s, unsigned what);//{update_(this, s, what);};
  public:

    Fl_Widget_Style(Fl_Widget_Style * parent, unsigned highlight_parent);
 

    

  // for hacking

    virtual ~Fl_Widget_Style();

  // These are inlined and fast - max one level digging...

    Fl_Labeltype labeltype() const {return ((STATIC|LABELTYPE) & flags_) ? (Fl_Labeltype)labeltype_ : (Fl_Labeltype)(next()->labeltype_);}
    Fl_Color labelcolor() const  {return ((STATIC|LABELCOLOR) & flags_) ? (Fl_Color)labelcolor_ : (Fl_Color)(next()->labelcolor_);}
    Fl_Font labelfont() const  {return ((STATIC|LABELFONT) & flags_) ? (Fl_Font)labelfont_ : (Fl_Font)(next()->labelfont_);}
    uchar labelsize() const  {return ((STATIC|LABELSIZE) & flags_) ? labelsize_ : next()->labelsize_;}

    Fl_Color color()const {return ((STATIC|COLOR) & flags_) ? (Fl_Color)color_ : (Fl_Color)(next()->color_);}
    Fl_Color selection_color() const  {return ((STATIC|SELECTION_COLOR) & flags_) ? (Fl_Color)selection_color_ : (Fl_Color)(next()->selection_color_);}
    Fl_Boxtype box() const {return ((STATIC|BOX) & flags_) ? (Fl_Boxtype)box_ : (Fl_Boxtype)(next()->box_);}

    void labeltype(Fl_Labeltype t);
    void labelcolor(unsigned c);
    void labelfont(uchar f);
    void labelsize(uchar s);
    void color(unsigned c);
    void selection_color(unsigned c);
    void box(Fl_Boxtype b);
  };


#endif // Fl_Widget_Style_H


///////////////////////  Fl_Button_Style  //////////////////////

#if defined(Fl_Button_H) && !defined(Fl_Button_Style_H)
# define Fl_Button_Style_H

class FL_EXPORT Fl_Button_Style : public Fl_Widget_Style {
  friend class Fl_Button;
  Fl_Boxtype down_box_;
  //uchar down_box_;
public:
  //static void update_(Fl_Button_Style * s1, Fl_Widget_Style * s, unsigned what=ALL);
protected:
  Fl_Button_Style(Fl_Button_Style * s):Fl_Widget_Style(s){};

  //reimplementation of virtual functions for new style
  Fl_Widget_Style * clone(){return new Fl_Button_Style(this);} 


  void update(Fl_Widget_Style * s, unsigned what);//{update_(this, s,what);}
public:

  Fl_Button_Style * highlight(){return (Fl_Button_Style *)highlight_();}
  Fl_Button_Style * normal(){return (Fl_Button_Style *)normal_();}


  Fl_Boxtype down_box() const {return ((STATIC|DOWN_BOX) & flags()) ? down_box_ : (Fl_Boxtype)(((Fl_Button_Style *)next())->down_box_);};
  void down_box(Fl_Boxtype b);
  Fl_Button_Style(Fl_Widget::Style * parent, unsigned mode);
};


#endif  // Fl_Button_Style_H



///////////////////////  Fl_Tool_Button::Style  //////////////////////

#if defined(Fl_Tool_Button_H) && !defined(Fl_Tool_Button_Style_H)
# define Fl_Tool_Button_Style_H

class FL_EXPORT Fl_Tool_Button_Style : public Fl_Button_Style {
  friend class Fl_Tool_Button;
  uchar symbol_align_;
public:
    Fl_Widget_Style * clone(){return new Fl_Tool_Button_Style(this);} 

protected:
  Fl_Tool_Button_Style(Fl_Tool_Button_Style * s):Fl_Button_Style(s){};

  //reimplementation of virtual functions for new style
  //void clear();

  //static void update_(Fl_Tool_Button_Style * s1, Fl_Widget_Style * s, unsigned what=ALL);
  void update(Fl_Widget_Style * s, unsigned what);//{update_(this, s,what);};
public:

  Fl_Tool_Button_Style * highlight(){return (Fl_Tool_Button_Style *)highlight_();}
  Fl_Tool_Button_Style * normal(){return (Fl_Tool_Button_Style *)normal_();}

  uchar symbol_align() const {return ((STATIC|SYMBOL_ALIGN) & flags()) ? symbol_align_ : (((Fl_Tool_Button_Style *)next())->symbol_align_);};
  void symbol_align(uchar a);

  Fl_Tool_Button_Style(Fl_Widget_Style * parent, unsigned mode);
};

typedef Fl_Tool_Button_Style Fl_Tool_Button_Style;


#endif  // Fl_Tool_Button_Style_H


/*

///////////////////////  Fl_Text_Style  //////////////////////
// 
// This is a base style class not only for browser but wor many text widgets


#if (defined(Fl_Input__H) || defined(Fl_Browser__H)) && !defined(Fl_Text_Style_H)
# define Fl_Text_Style_H


class FL_EXPORT Fl_Text_Style : public Fl_Widget::Style {
  unsigned textcolor_;
  uchar textfont_;
  uchar textsize_;
public:
  Fl_Widget::Style * clone(){return new Fl_Text_Style(this);} 

protected:
  Fl_Text_Style(Fl_Text_Style * s):Fl_Widget::Style(s){};

  //reimplementation of virtual functions for new style

  static void update_(Fl_Text_Style * s1, Fl_Widget::Style * s, unsigned what=ALL);
  void update(Fl_Widget::Style * s, unsigned what){update_(this, s,what);};
public:
  Fl_Text_Style * highlight(){return (Fl_Text_Style *)highlight_();}
  Fl_Text_Style * normal(){return (Fl_Text_Style *)normal_();}


  Fl_Color textcolor() const  {return ((STATIC|TEXTCOLOR) & flags()) ? (Fl_Color)textcolor_ : (Fl_Color)(((Fl_Text_Style *)next())->textcolor_);}
  Fl_Font textfont() const  {return ((STATIC|TEXTFONT) & flags()) ? (Fl_Font)textfont_ : (Fl_Font)(((Fl_Text_Style *)next())->textfont_);}
  uchar textsize() const  {return ((STATIC|TEXTSIZE) & flags()) ? textsize_ : ((Fl_Text_Style * )next())->textsize_;}

  void textcolor(unsigned c);
  void textfont(uchar f);
  void textsize(uchar s);


  Fl_Text_Style(Fl_Widget::Style * parent, unsigned mode);
};





#endif  // Fl_Text_Style_H

*/




///////////////////////  Fl_Text_Style  //////////////////////

#if !defined(Fl_Text_Style_H) && (defined(Fl_Browser__H) || defined (Fl_Input_H))
# define Fl_Text_Style_H

class FL_EXPORT Fl_Text_Style : public Fl_Widget_Style {
  Text text_;
protected:
  const Text * text() const {return &text_;};
public:
  using Fl_Widget_Style::textcolor;
  using Fl_Widget_Style::textsize;
  using Fl_Widget_Style::textfont;

    Fl_Widget_Style * clone(){return new Fl_Text_Style(this);} 

protected:
  Fl_Text_Style(Fl_Text_Style * s):Fl_Widget_Style(s){};

  //reimplementation of virtual functions for new style


//  static void update_(Fl_Text_Style * s1, Fl_Widget_Style * s, unsigned what=ALL);
//  void update(Fl_Widget_Style * s, unsigned what){update_(this, s,what);};
public:

  Fl_Text_Style * highlight(){return (Fl_Text_Style *)highlight_();}
  Fl_Text_Style * normal(){return (Fl_Text_Style *)normal_();}


  Fl_Text_Style(Fl_Widget_Style * parent, unsigned mode);
};



#endif  // Fl_Tool_Button_Style_H

/*



#if defined(Fl_Valuator_H) && !defined(Fl_Valuator_Style_H)
# define Fl_Valuator_Style_H


class FL_EXPORT Fl_Valuator_Style : public Fl_Widget_Style {
  Fl_Boxtype arrows_;
  //uchar down_box_;
public:
  static void update_(Fl_Valuator_Style * s1, Fl_Widget_Style * s, unsigned what=ALL);
protected:
  Fl_Valuator_Style(Fl_Valuator_Style * s):Fl_Widget_Style(s){};

  //reimplementation of virtual functions for new style
  //void clear();
  Fl_Widget_Style * clone(){return new Fl_Valuator_Style(this);} 


  void update(Fl_Widget_Style * s, unsigned what){update_(this, s,what);}
public:

  Fl_Valuator_Style * highlight(){return (Fl_Valuator_Style *)highlight_();}
  Fl_Valuator_Style * normal(){return (Fl_Valuator_Style *)normal_();}


  Fl_Boxtype arrows() const {return ((STATIC|ARROWS) & flags()) ? arrows_ : (Fl_Boxtype)(((Fl_Valuator_Style *)next())->arrows_);};
  void arrows(Fl_Boxtype b);
  Fl_Valuator_Style(Fl_Widget::Style * parent, unsigned mode);
};


#endif


*/










//#if defined(Fl_Adjuster_H) && !defined(Fl_Adjuster_Style_H)
//#endif





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
