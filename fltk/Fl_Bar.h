//
// "$Id: Fl_Bar.h,v 1.2 2002/10/29 00:37:22 easysw Exp $"
//
// Bar header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Bar_H
#define Fl_Bar_H

#include <fltk/Fl_Group.h>

/**

  This group widget can be "closed" by the user by clicking on the 
  small box that is at the left edge. When it is closed it 
  resizes to a small horizontal strip with the box at the left 
  edge and draws the label() (if any), and all child widgets are 
  hidden and do not get any events. When opened the label disappears 
  and all child widgets reappear. 

  This must be made a child of an Fl_Pack for the resizing to be useful.
  
  \image Fl_Bar.gif

  If type() of the bar is set to Fl_Bar::VERTICAL then it will be a 
  vertical strip with the control box at the top. 

  The default box is FL_FLAT_BOX. If there are several bars you 
  probably need to change this in order to see the dividing 
  lines between them. 

*/
class FL_API Fl_Bar : public Fl_Group {
  bool open_;
  bool highlighted;
  bool pushed;
  int glyph_size_;
  int saved_size;
  void glyph_box(int& x, int& y, int& w, int& h);
public:
  /**
   Creates a new empty Fl_Bar. 
  */
  Fl_Bar(int x,int y,int w ,int h,const char *l = 0);
  static Fl_Named_Style* default_style;
  enum { // values for type(int)
    VERTICAL	= GROUP_TYPE,
    HORIZONTAL	= GROUP_TYPE+1
  };
  void layout();
  int handle(int);
  void draw();
  /** True if the bar is opened, false if it is closed. */
  bool opened() const {return open_;}
  /**
   Setting this value returns true if the new value is different 
   to the old one. 
  */
  bool opened(bool);
  /** Same as opened(true) */
  bool open() {return opened(true);}
  /** Same as opened(false) */
  bool close() {return opened(false);}
  /**
   Get the width of the box (or the height if this is a vertical bar), 
   this is also the thickness the bar resizes to when closed. 
  */
  int glyph_size() const {return glyph_size_;}
  /**
   Set the width of the box (or the height if this is a vertical bar), 
   this is also the thickness the bar resizes to when closed. 
  */
  void glyph_size(int v) {glyph_size_ = v;}
};

#endif

//
// End of "$Id: Fl_Bar.h,v 1.2 2002/10/29 00:37:22 easysw Exp $".
//
