/
// "$Id$
/
// Button header file for the Fast Light Tool Kit (FLTK)
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

#ifndef Fl_Button_
#define Fl_Button_

#ifndef Fl_Widget_
#include "Fl_Widget.H
#endi

// values for type(
#define FL_NORMAL_BUTTON	
#define FL_TOGGLE_BUTTON	
#define FL_RADIO_BUTTON		(FL_RESERVED_TYPE+2
#define FL_HIDDEN_BUTTON	3 // for Forms compatabilit

extern FL_EXPORT int fl_old_shortcut(const char*)

/*
 * Buttons generate callbacks when they are clicked by the user.
 * You control exactly when and how by changing the values for type() and when()
 
 * Buttons can also generate callbacks in response to \c FL_SHORTCUT events.
 * The button can either have an explicit shortcut() value or a letter shortcu
 * can be indicated in the label() with an '&' character before it.
 * For the label shortcut it does not matter if \a Alt is held down, but if you
 * have an input field in the same window, the user will have to hold down
 * the \a Alt key so that the input field does not eat the event first as an
 * \c FL_KEYBOARD event
 *

class FL_EXPORT Fl_Button : public Fl_Widget 


  int shortcut_
  char value_
  char oldval
  Fl_Boxtype * down_box_


protected


  virtual void draw()


public

  class Style
  static Style * default_style()
  void style(Style *s )


  virtual int handle(int event)
    /** The constructor creates the button using the \a position, \a size,
     * and \a label. *
  Fl_Button(int x,int y,int width,int height,const char *label = 0)
    /** The second form sets the current value. *
  int value(int v)
    /** Returns the current value (0 or 1). *
  char value() const {return value_;
    /** Same as value(0). *
  int set() {return value(1);
    /** Same as value(0). *
  int clear() {return value(0);
    /** Turns on this button and turns off all other radio buttons in the group
     * (calling value(1) or set() does not do this). this should only be called
     * on \c FL_RADIO_BUTTONs *
  void setonly()
    /** Returns the current shortcut key for the button. *
  int shortcut() const {return shortcut_;
    /** Sets the shortcut key to key. Setting this overrides the use of '&'
     * in the label(). The value is a bitwise OR of a key and a set of shift flags,
     * for example <tt>FL_ALT | 'a'</tt>, <tt>FL_ALT | (FL_F + 10)</tt>, or just 'a'.
     * A value of 0 disables the shortcut
     
     * The key can be any value returned by Fl::event_key(), but will usually be a
     * ASCII letter. Use a lower-case letter unless you require the shift key to b
     * held down
     
     * The shift flags can be any set of values accepted by Fl::event_state(). If
     * the bit is on that shift key must be pushed. Meta, Alt, Ctrl, and Shift mus
     * be off if they are not in the shift flags (zero for the other bits indicates
     * a "don't care" setting). *
  void shortcut(int s) {shortcut_ = s;
    /** Returns the current down box type, which is drawn when value() is non-zero. *
  Fl_Boxtype down_box() const;
    /** Sets the down box type. The default value of 0 causes FLTK to figure out
     * the correct matching down version of box(). *
  void down_box(Fl_Boxtype b)

    /** \deprecated old way to set shortcut *
  void shortcut(const char *s) {shortcut(fl_old_shortcut(s));
    /** \deprecated same as selection_color() *
  Fl_Color down_color() const {return selection_color();
    /** \deprecated same as selection_color() *
  void down_color(unsigned c) {selection_color(c);

  //~ char try_down_box(uchar b){if ((style_flags2_ & DOWN_BOX) || (b == down_box_)) return 0; down_box_ = b; return 1;


};


#ifdef Fl_Style_H
#include <FL/Fl_Style.H>
#endif





#endi

/
// End of "$Id$"
/
