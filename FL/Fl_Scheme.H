/
// "$Id$
/
// Style header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#include <FL/Fl_Style.H

#ifndef Fl_Scheme_
#define Fl_Scheme_



class Fl_Widget_Style: public Fl_Style
  friend class Fl_Scheme
 

  //enum {COLOR=1, SELECTION_COLOR=2, BOX=4, ALIGN=8, LABELTYPE=16, LABELCOLOR=32, LABELFONT=64, LABELSIZE=128}

protected
  unsigned color_
 
  uchar box_
  uchar labeltype_
  unsigned labelcolor_
  uchar labelfont_
  uchar labelsize_
  unsigned selection_color_

  int set_flags_; // these flags are used when system values are read

   enum {COLOR=1, SELECTION_COLOR=2, BOX=4, LABELTYPE=8, LABELCOLOR=0x10, LABELFONT=0x20, LABELSIZE=0x40, ALIGN=0x80
    DOWN_BOX =0x100, HIGHLIGHTED_COLOR= 0x200, HIGHLIGHTED_SELECTION_COLOR =0x400, HIGHLIGHTED_BOX = 0x800, HIGHLIGHTED_LABELCOLOR = 0x1000
    HIGHLIGHTED_DOWN_BOX = 0x2000, TEXTCOLOR = 0x4000, TEXTFONT = 0x8000, TEXTSIZE = 0x10000}
 



  virtual void update()=0

public
  void apply(Fl_Widget * w)

  Fl_Color color() const {return (Fl_Color)color_;}
  void color(unsigned c){color_ = c; set_flags_ |=COLOR;}
  Fl_Labeltype labeltype() const {return (Fl_Labeltype)labeltype_;
  void labeltype(Fl_Labeltype t){labeltype_=t; set_flags_|= LABELTYPE;
  Fl_Color labelcolor() const {return (Fl_Color)labelcolor_;
  void labelcolor(unsigned c){ labelcolor_ = c; set_flags_ |= LABELCOLOR;
  Fl_Font labelfont() const {return (Fl_Font)labelfont_;
  void labelfont(uchar f){labelfont_ = f; set_flags_ |= LABELFONT;
  uchar labelsize() const {return labelsize_;
  void labelsize(uchar s){labelsize_=s; set_flags_ |= LABELSIZE;
  Fl_Color selection_color() const {return (Fl_Color)selection_color_;
  void selection_color(unsigned c){selection_color_ = c; set_flags_ |= SELECTION_COLOR;}
  Fl_Boxtype box() const {return (Fl_Boxtype) box_;}
  void box(uchar b) {box_= b; set_flags_ |= BOX;

  Fl_Widget_Style()

}


class Fl_Button_Style: public Fl_Widget_Style
protected
  Fl_Widget_Style * parent_
  //uchar down_box_
  unsigned highlighted_color_
  //unsigned highlighted_selection_color_
  uchar highlighted_box_
  //static int handle(Fl_Widget * w, int event)
protected
  void update()
public


  // These are new parameters applicable to buttons onl
 // Fl_Boxtype down_box() const {return (Fl_Boxtype) down_box_;}
 // void down_box(uchar b) {down_box_= b; set_flags_ |= DOWN_BOX;
   // Highlightin
  Fl_Color highlighted_color() const {return (Fl_Color) highlighted_color_;}
  void highlighted_color(unsigned c){highlighted_color_ = c; set_flags_ |=HIGHLIGHTED_COLOR;}
  Fl_Boxtype highlighted_box() const {return (Fl_Boxtype) highlighted_box_; }
  void highlighted_box(uchar b) {highlighted_box_= b; set_flags_ |= HIGHLIGHTED_BOX; 

  // These are highlighted colors when button is "down"
  //Fl_Color highlighted_selection_color() const { return (Fl_Color) highlighted_selection_color_;
  //void highlighted_selection_color(unsigned c){highlighted_selection_color_ = c; set_flags_ |= HIGHLIGHTED_SELECTION_COLOR;}

  // overriding function to apply the style
  void apply(Fl_Widget *)
  Fl_Button_Style(Fl_Widget_Style * parent)
}



class Fl_Light_Button_Style: public Fl_Button_Style
protected
  void update()
public
  Fl_Light_Button_Style(Fl_Button_Style * parent)
}


class Fl_Global_Style

class Fl_Browser__Style: public Fl_Widget_Style
  Fl_Global_Style * parent_
  unsigned textcolor_
  uchar textfont_
  uchar textsize_
protected
  void update()
public
   Fl_Color textcolor() const {return (Fl_Color)textcolor_;
  void textcolor(unsigned c){ textcolor_ = c; set_flags_ |= TEXTCOLOR;
  Fl_Font textfont() const {return (Fl_Font)labelfont_;
  void textfont(uchar f){textfont_ = f; set_flags_ |= TEXTFONT;
  uchar textsize() const {return textsize_;
  void textsize(uchar s){textsize_=s; set_flags_ |= TEXTSIZE;
  Fl_Browser__Style(Fl_Global_Style * parent):parent_(parent){}
}



class Fl_Global_Style: public Fl_Browser__Style

protected
  unsigned canvas_color_; // this is background2 colo
  void update()
  enum {CANVAS_COLOR = DOWN_BOX}; // global style don not need downbox, spare this bit for background
public
  Fl_Color canvas_color() const {return (Fl_Color)canvas_color_;}
  void canvas_color(unsigned c){canvas_color_ = c; set_flags_ |= CANVAS_COLOR;}
  Fl_Global_Style():Fl_Browser__Style(0),canvas_color_(FL_BACKGROUND2_COLOR){}
}



class Fl_Scheme:public Fl_Scheme_
  Fl_Global_Style * global

public
  void update()

   // default color of widgets and window
  Fl_Color color() const {return global->color();};
  void color(unsigned c){global->color(c);}
 
  // Default label for all widget
  Fl_Labeltype labeltype() const {return global->labeltype();} //default
  void labeltype(Fl_Labeltype t){global->labeltype(t);
  Fl_Color labelcolor() const {return global->labelcolor();
  void labelcolor(unsigned c){ global->labelcolor(c);
  Fl_Font labelfont() const {return global->labelfont();
  void labelfont(uchar f){global->labelfont(f);
  uchar labelsize() const {return global->labelsize();
  void labelsize(uchar s){global->labelsize(s);
 

  // This is background color of text widgets, inputs etc (usualy white), it has the meaning of FL_BACLGROUND
  Fl_Color canvas_color() const {return global->canvas_color();
  void canvas_color(unsigned c){global->canvas_color(c);}

  // This is selection color for text etc, not buttons when dow
  Fl_Color selection_color() const {return global->selection_color();
  void selection_color(unsigned c){global->selection_color(c);}


  // These are the defaults for their tex
  Fl_Color textcolor() const {return global->textcolor();
  void textcolor(unsigned c){ global->textcolor(c);
  Fl_Font textfont() const {return global->textfont();
  void textfont(uchar f){global->labelfont(f);
  uchar textsize() const {return global->textsize();
  void textsize(uchar s){global->textsize(s);

  Fl_Button_Style * button
  Fl_Button_Style * tool_button
  Fl_Browser__Style * browser
  Fl_Light_Button_Style * light_button

  Fl_Scheme()
  ~Fl_Scheme()
  void current()
}


     


#endi

/
// End of "$Id$"
/
