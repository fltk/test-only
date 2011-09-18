//
// "$Id$"
//
// Help Viewer widget definitions.
// FLTK 123 wrapper finished
//
// Copyright 1997-2011 by Easy Software Products.
// Image support by Matthias Melcher, Copyright 2000-2011.
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

#ifndef Fl_Help_View_H
#define Fl_Help_View_H

#include <fltk3/HelpView.h>
#include <stdio.h>
#include "Fl.H"
#include "Fl_Group.H"
#include "Fl_Scrollbar.H"
#include "fl_draw.H"
#include "Fl_Shared_Image.H"
#include "filename.H"


typedef const char *(Fl_Help_Func)(Fl_Widget *, const char *);


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Help_View, HelpView)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Help_View, HelpView)
FLTK3_WRAPPER_INTERFACE_END()


#if 0 // FIXME: 123

struct Fl_Help_Block {
  const char	*start,		// Start of text
		*end;		// End of text
  uchar		border;		// Draw border?
  Fl_Color	bgcolor;	// Background color
  int		x,		// Indentation/starting X coordinate
		y,		// Starting Y coordinate
		w,		// Width
		h;		// Height
  int		line[32];	// Left starting position for each line
};


struct Fl_Help_Link {
  char		filename[192],	///< Reference filename
		name[32];	///< Link target (blank if none)
  int		x,		///< X offset of link text
		y,		///< Y offset of link text
		w,		///< Width of link text
		h;		///< Height of link text
};


struct FL_EXPORT Fl_Help_Font_Style {
  Fl_Font      f;  ///< Font
  Fl_Fontsize  s;  ///< Font Size
  Fl_Color     c;  ///< Font Color
  void get(Fl_Font &afont, Fl_Fontsize &asize, Fl_Color &acolor) {afont=f; asize=s; acolor=c;} ///< Gets current font attributes
  void set(Fl_Font afont, Fl_Fontsize asize, Fl_Color acolor) {f=afont; s=asize; c=acolor;} ///< Sets current font attributes
  Fl_Help_Font_Style(Fl_Font afont, Fl_Fontsize asize, Fl_Color acolor) {set(afont, asize, acolor);}
  Fl_Help_Font_Style(){} // For in table use
};


const size_t MAX_FL_HELP_FS_ELTS = 100;


struct FL_EXPORT Fl_Help_Font_Stack {
  Fl_Help_Font_Stack() {
    nfonts_ = 0;
  }

  void init(Fl_Font f, Fl_Fontsize s, Fl_Color c) { 
    nfonts_ = 0;
    elts_[nfonts_].set(f, s, c);
    fl_font(f, s); 
    fl_color(c);
  }

  void top(Fl_Font &f, Fl_Fontsize &s, Fl_Color &c) { elts_[nfonts_].get(f, s, c); }
  
  void push(Fl_Font f, Fl_Fontsize s, Fl_Color c) { 
    if (nfonts_ <  MAX_FL_HELP_FS_ELTS-1) nfonts_ ++;
    elts_[nfonts_].set(f, s, c);
    fl_font(f, s); fl_color(c); 
  }
  
  void pop(Fl_Font &f, Fl_Fontsize &s, Fl_Color &c) { 
    if (nfonts_ > 0) nfonts_ --;
    top(f, s, c);
    fl_font(f, s); fl_color(c);
  }
  
  size_t count() const {return nfonts_;}

protected:
  size_t nfonts_;
  Fl_Help_Font_Style elts_[100];
};


struct Fl_Help_Target {
  char		name[32];	///< Target name
  int		y;		///< Y offset of target
};

#endif


class FL_EXPORT Fl_Help_View : public Fl_Group {

public:
  
  FLTK3_WIDGET_VCALLS(HelpView)
  
  Fl_Help_View(int xx, int yy, int ww, int hh, const char *l = 0) {
    _p = new fltk3::HelpView_I(xx, yy, ww, hh, l);
    _p->wrapper(this);
  }

  const char *directory() const {
    return ((fltk3::HelpView*)_p)->directory();
  }

  const char *filename() const {
    return ((fltk3::HelpView*)_p)->filename();
  }

  int find(const char *s, int p = 0) {
    return ((fltk3::HelpView*)_p)->find(s, p);
  }
  
  void link(Fl_Help_Func *fn) {
    ((fltk3::HelpView*)_p)->link((fltk3::HelpFunc*)fn);
  }
  
  int load(const char *f) {
    return ((fltk3::HelpView*)_p)->load(f);
  }
  
  int size() const {
    return ((fltk3::HelpView*)_p)->size();
  }

  void size(int W, int H) {
    ((fltk3::HelpView*)_p)->size(W, H);
  }

  Fl_Font textfont() const { 
    return fltk3::_3to1_font( ((fltk3::HelpView*)_p)->textfont() ); 
  }
  
  void textfont(Fl_Font font) { 
    ((fltk3::HelpView*)_p)->textfont( fltk3::_1to3_font(font) );
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::HelpView*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::HelpView*)_p)->textsize( fltk3::_1to3_fontsize(size) );
  }
  
  Fl_Color textcolor() const { 
    return fltk3::_3to1_color( ((fltk3::HelpView*)_p)->textcolor() );
  }
  
  void textcolor(Fl_Color col) { 
    ((fltk3::HelpView*)_p)->textcolor( fltk3::_1to3_color(col) );
  }
  const char *title() {
    return ((fltk3::HelpView*)_p)->title();
  }
  
  void topline(const char *n) {
    ((fltk3::HelpView*)_p)->topline(n);
  }
  
  void topline(int t) {
    ((fltk3::HelpView*)_p)->topline(t);
  }
  
  int topline() const {
    return ((fltk3::HelpView*)_p)->topline();
  }
  
  void leftline(int n) {
    ((fltk3::HelpView*)_p)->leftline(n);
  }

  int leftline() const {
    return ((fltk3::HelpView*)_p)->leftline();
  }
  
  void value(const char *val) {
    ((fltk3::HelpView*)_p)->value(val);
  }

  const char *value() const {
    return ((fltk3::HelpView*)_p)->value();
  }
  
  void clear_selection() {
    ((fltk3::HelpView*)_p)->clear_selection();
  }
  
  void select_all() {
    ((fltk3::HelpView*)_p)->select_all();
  }

  int scrollbar_size() const {
    return ((fltk3::HelpView*)_p)->scrollbar_size();
  }

  void scrollbar_size(int size) {
    ((fltk3::HelpView*)_p)->scrollbar_size(size);
  }

};


#endif // !Fl_Help_View_H

//
// End of "$Id$".
//
