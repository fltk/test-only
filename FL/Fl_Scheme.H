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

#include <FL/Fl_Style.H>

#ifndef Fl_Scheme_H
#define Fl_Scheme_H

/// A base style class for all widget styles within a scheme (see Fl_Scheme)
/** @ingroup styles */


class Fl_Widget_Style: public Fl_Style{
  friend class Fl_Scheme;
 
  
protected:
  Fl_Widget_Style * parent_;
  unsigned color_;
  uchar box_;
  uchar labeltype_;
  unsigned labelcolor_;
  uchar labelfont_;
  uchar labelsize_;
  unsigned selection_color_;

  void * data_;
  int set_flags_; // these flags are used when system values are read.

  void apply(Fl_Widget * w);

public:
  virtual void update();
  /** \internal \c set_flags inform if a style parameter was explicitly set, or should be inherited from its parent. */
  enum Parameter {STATIC = 1, COLOR=2, SELECTION_COLOR=4, BOX=8, LABELTYPE=0x10, LABELCOLOR=0x20, LABELFONT=0x40, LABELSIZE=0x80, ALIGN=0x100,
    DOWN_BOX =0x200, TEXTCOLOR = 0x400, TEXTFONT = 0x800, TEXTSIZE = 0x1000, DATA = 0x10000000, HANDLE = 0x20000000, DRAW = 0x40000000};
  

  /** @internal Gets \c set_flags flags from a style. \see Fl_Widget_Style::Parameter*/
  int set_flags() {return set_flags_;};
  
  /** @internal */
  void set_flags(int flags) {set_flags_ = flags;};
  
  /** @interval  Compares values with parent ones and returns values with bits set if particular data differ */
  virtual int compare_flags();

  Fl_Handle_Style * handle_function(){return handle_;}
  void handle_function(Fl_Handle_Style * f){handle_ = f; set_flags_ |= HANDLE;};


  Fl_Draw_Style * draw_function(){return draw_;}
  void draw_function(Fl_Draw_Style * f){draw_ = f;  set_flags_ |= DRAW;}

  
  
  
 

  void * data(){return data_;};
  void data(void * d){data_ = d; set_flags_ |= DATA;};

  Fl_Widget_Style(Fl_Widget_Style * parent);
  
  

  Fl_Color color() const {return (Fl_Color)color_;};
  void color(unsigned c){color_ = c; set_flags_ |=COLOR;};
  Fl_Labeltype labeltype() const {return (Fl_Labeltype)labeltype_;}
  void labeltype(Fl_Labeltype t){labeltype_=t; set_flags_|= LABELTYPE;}
  Fl_Color labelcolor() const {return (Fl_Color)labelcolor_;}
  void labelcolor(unsigned c){ labelcolor_ = c; set_flags_ |= LABELCOLOR;}
  Fl_Font labelfont() const {return (Fl_Font)labelfont_;}
  void labelfont(uchar f){labelfont_ = f; set_flags_ |= LABELFONT;}
  uchar labelsize() const {return labelsize_;}
  void labelsize(uchar s){labelsize_=s; set_flags_ |= LABELSIZE;}
  Fl_Color selection_color() const {return (Fl_Color)selection_color_;}
  void selection_color(unsigned c){selection_color_ = c; set_flags_ |= SELECTION_COLOR;};
  Fl_Boxtype box() const {return (Fl_Boxtype) box_;};
  void box(uchar b) {box_= b; set_flags_ |= BOX;}



};

class Fl_Browser_Style: public Fl_Widget_Style{
  unsigned textcolor_;
  uchar textfont_;
  uchar textsize_;
protected:
  

public:
  void update();
  Fl_Browser_Style(Fl_Browser_Style * parent=0):Fl_Widget_Style(parent){};
  
  Fl_Color background_color(){return color();};
  void background_color(unsigned c){color(c);};

  Fl_Color textcolor() const {return (Fl_Color)textcolor_;}
  void textcolor(unsigned c){ textcolor_ = c; set_flags_ |= TEXTCOLOR;}
  Fl_Font textfont() const {return (Fl_Font)labelfont_;}
  void textfont(uchar f){textfont_ = f; set_flags_ |= TEXTFONT;}
  uchar textsize() const {return textsize_;}
  void textsize(uchar s){textsize_=s; set_flags_ |= TEXTSIZE;}


};




class Fl_Button_Style: public Fl_Widget_Style{
protected:
  uchar down_box_;
protected:

public:
  void update();
  int compare_flags();
  Fl_Color background_color(){return color();};
  void background_color(unsigned c){color(c);};


  // These are new parameters applicable to buttons only
 Fl_Boxtype down_box() const {return (Fl_Boxtype) down_box_;};
 void down_box(uchar b) {down_box_= b; set_flags_ |= DOWN_BOX;}

  // overriding function to apply the style 
  void apply(Fl_Widget *);

  Fl_Button_Style(Fl_Button_Style * parent = 0);
};


/** \ingroup styles 
* Scheme defines default appereance and highlighting od FLTK widgets. It conmprises of instances
* of Fl_Style classes (ot ots subclassess) like Fl_Scheme::button, Fl_Scheme::browser, ... to define styles of 
* corresponding classess */

class Fl_Scheme:public Fl_Scheme_{

public:
  void update();


  /** Defines appereance of Fl_Button class */
  Fl_Button_Style button;
  Fl_Button_Style highlighted_button;

  Fl_Button_Style tool_button;
  Fl_Button_Style highlighted_tool_button;

  Fl_Browser_Style browser;

  Fl_Scheme();
  ~Fl_Scheme();
  void current();
};


      


#endif

//
// End of "$Id$".
//
