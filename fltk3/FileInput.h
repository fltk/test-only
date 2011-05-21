//
// "$Id: FileInput.h 8306 2011-01-24 17:04:22Z matt $"
//
// File_Input header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
// Original version Copyright 1998 by Curtis Edwards.
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
 fltk3::FileInput widget . */

#ifndef Fltk3_File_Input_H
#  define Fltk3_File_Input_H

#  include <fltk3/Input.h>

namespace fltk3 {
  
  /**
   \class fltk3::FileInput
   \brief This widget displays a pathname in a text input field.
   
   A navigation bar located above the input field allows the user to
   navigate upward in the directory tree.
   You may want to handle fltk3::WHEN_CHANGED events for tracking text changes
   and also fltk3::WHEN_RELEASE for button release when changing to parent dir.
   fltk3::WHEN_RELEASE callback won't be called if the directory clicked 
   is the same that the current one.
   <P align=CENTER> \image html fltk3::FileInput.png </P> 
   \image latex fltk3::FileInput.png "fltk3::FileInput"  width=6cm
   \note As all fltk3::Input derived objects, fltk3::FileInput may call its callback
   when loosing focus (see fltk3::UNFOCUS) to update its state like its cursor shape.
   One resulting side effect is that you should call clear_changed() early in your callback
   to avoid reentrant calls if you plan to show another window or dialog box in the callback.
   */
  class FLTK3_EXPORT FileInput : public fltk3::Input {
    
    fltk3::Color	errorcolor_;
    char		ok_entry_;
    uchar		down_box_;
    short		buttons_[200];
    short		pressed_;
    
    void		draw_buttons();
    int		handle_button(int event);
    void		update_buttons();
    
  public:
    
    FileInput(int X, int Y, int W, int H, const char *L=0);
    
    virtual int handle(int event);
    
  protected:
    virtual void draw();
    
  public:
    /** Gets the box type used for the navigation bar. */
    fltk3::Boxtype	down_box() const { return (fltk3::Boxtype)down_box_; }
    /** Sets the box type to use for the navigation bar.  */
    void		down_box(fltk3::Boxtype b) { down_box_ = b; }
    
    /**
     Gets the current error color.
     \todo Better docs for fltk3::FileInput::errorcolor() - is it even used?
     */
    fltk3::Color	errorcolor() const { return errorcolor_; }
    /** Sets the current error color to \p c */
    void		errorcolor(fltk3::Color c) { errorcolor_ = c; }
    
    int	value(const char *str);
    int	value(const char *str, int len);
    
    /**
     Returns the current value, which is a pointer to an internal buffer
     and is valid only until the next event is handled.
     */
    const char	*value() { return fltk3::Input_::value(); }
  };
  
}

#endif // !Fltk3_File_Input_H


//
// End of "$Id: FileInput.h 8306 2011-01-24 17:04:22Z matt $".
//
