//
// "$Id$"
//
// Button header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

/* \file
 fltk3::Button widget . */

#ifndef FLTK3_BUTTON_H
#define FLTK3_BUTTON_H

#include "Widget.h"
#include "Box.h"
#include <fltk3/Wrapper.h>


namespace fltk3 {
  // values for type()
  const uchar NORMAL_BUTTON = 0;   /**< value() will be set to 1 during the press of the button and 
                                    reverts back to 0 when the button is released */
  const uchar TOGGLE_BUTTON = 1;   ///< value() toggles between 0 and 1 at every click of the button
  const uchar RADIO_BUTTON = RESERVED_TYPE+2; /**< is set to 1 at button press, and all other
                                               buttons in the same group with <tt>type() == fltk3::RADIO_BUTTON</tt>
                                               are set to zero.*/
  const uchar HIDDEN_BUTTON = 3;   ///< for Forms compatibility
  
  extern FLTK3_EXPORT fltk3::Shortcut old_shortcut(const char*);
  
  class WidgetTracker;
  
  /**
   \class fltk3::Button
   \brief Buttons generate callbacks when they are clicked by the user.
   
   You control exactly when and how by changing the values for type() and
   when().  Buttons can also generate callbacks in response to \c fltk3::SHORTCUT
   events.  The button can either have an explicit shortcut(int s) value or a
   letter shortcut can be indicated in the label() with an '\&' character
   before it.  For the label shortcut it does not matter if \e Alt is held
   down, but if you have an input field in the same window, the user will have
   to hold down the \e Alt key so that the input field does not eat the event
   first as an \c fltk3::KEYBOARD event.
   
   \todo Refactor the doxygen comments for fltk3::Button type() documentation.
   
   For an fltk3::Button object, the type() call returns one of:
   \li \c fltk3::NORMAL_BUTTON (0): value() remains unchanged after button press.
   \li \c fltk3::TOGGLE_BUTTON: value() is inverted after button press.
   \li \c fltk3::RADIO_BUTTON: value() is set to 1 after button press, and all other
   buttons in the current group with <tt>type() == fltk3::RADIO_BUTTON</tt>
   are set to zero.
   
   \todo Refactor the doxygen comments for fltk3::Button when() documentation.
   
   For an fltk3::Button object, the following when() values are useful, the default
   being \c fltk3::WHEN_RELEASE:
   \li \c 0: The callback is not done, instead changed() is turned on.
   \li \c fltk3::WHEN_RELEASE: The callback is done after the user successfully
   clicks the button, or when a shortcut is typed.
   \li \c fltk3::WHEN_CHANGED: The callback is done each time the value() changes
   (when the user pushes and releases the button, and as the mouse is
   dragged around in and out of the button).
   */
  class FLTK3_EXPORT Button : public Widget {
    
    static Style* default_button_style_;
    
    unsigned int shortcut_;
    char value_;
    char oldval;
    Box* down_box_;
    
  protected:
    
    static fltk3::WidgetTracker *key_release_tracker;
    static void key_release_timeout(void*);
    void simulate_key_action();
    
    void draw(Box*, int, int, int, int, Color, Box::Flags);
    
    virtual void draw();
    
  public:
    
    virtual int handle(int);
    
    Button(int X, int Y, int W, int H, const char *L = 0);
    
    int value(int v);
    
    /**
     Returns the current value of the button (0 or 1).
     */
    char value() const {return value_;}
    
    /**
     Same as \c value(1).
     \see value(int v)
     */
    int set() {return value(1);}
    
    /**
     Same as \c value(0).
     \see value(int v)
     */
    int clear() {return value(0);}
    
    void setonly(); // this should only be called on fltk3::RADIO_BUTTONs
    
    /**
     Returns the current shortcut key for the button.
     \retval int
     */
    unsigned int shortcut() const {return shortcut_;}
    
    /**
     Sets the shortcut key to \c s.
     Setting this overrides the use of '\&' in the label().
     The value is a bitwise OR of a key and a set of shift flags, for example:
     <tt>fltk3::ALT | 'a'</tt>, or
     <tt>fltk3::ALT | (fltk3::FKey + 10)</tt>, or just
     <tt>'a'</tt>.
     A value of 0 disables the shortcut.
     
     The key can be any value returned by fltk3::event_key(), but will usually be
     an ASCII letter.  Use a lower-case letter unless you require the shift key
     to be held down.
     
     The shift flags can be any set of values accepted by fltk3::event_state().
     If the bit is on, that shift key must be pushed.  Meta, Alt, Ctrl, and
     Shift must be off if they are not in the shift flags (zero for the other
     bits indicates a "don't care" setting).
     \param[in] s bitwise OR of key and shift flags
     */
    void shortcut(unsigned int s) {shortcut_ = s;}
    
    /**
     Returns the current down box type, which is drawn when value() is non-zero.
     \retval fltk3::Boxtype
     */
    Box* down_box() const {return down_box_;}
    
    /**
     Sets the down box type. The default value of 0 causes FLTK to figure out
     the correct matching down version of box().
     \param[in] b down box type
     */
    void down_box(fltk3::Box* b) {down_box_ = b;}
    
    /// (for backwards compatibility)
    void shortcut(const char *s) {shortcut(fltk3::old_shortcut(s));}
    
    /// (for backwards compatibility)
    Color down_color() const {return selection_color();}
    
    /// (for backwards compatibility)
    void down_color(unsigned c) {selection_color(c);}
  };
  
} // namespace

#endif

//
// End of "$Id$".
//
