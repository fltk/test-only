/
// "$Id$
/
// Fl_Wizard widget routines
/
// Copyright 1997-2003 by Easy Software Products
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
// Contents
/
//   Fl_Wizard::Fl_Wizard() - Create an Fl_Wizard widget
//   Fl_Wizard::draw()      - Draw the wizard border and visible child
//   Fl_Wizard::next()      - Show the next child
//   Fl_Wizard::prev()      - Show the previous child
//   Fl_Wizard::value()     - Return the current visible child
//   Fl_Wizard::value()     - Set the visible child
/

/
// Include necessary header files..
/

#include <FL/Fl_Wizard.H
#include <FL/Fl_Window.H
#include <FL/fl_draw.H


/
// 'Fl_Wizard::Fl_Wizard()' - Create an Fl_Wizard widget
/

Fl_Wizard::Fl_Wizard(int        xx,	// I - Lefthand positio
                     int        yy,	// I - Upper positio
		     int        ww,	// I - Widt
		     int        hh,	// I - Heigh
		     const char *l) :	// I - Labe
    Fl_Group(xx, yy, ww, hh, l

  box(FL_THIN_UP_BOX)

  value_ = (Fl_Widget *)0



/
// 'Fl_Wizard::draw()' - Draw the wizard border and visible child
/

voi
Fl_Wizard::draw(

  Fl_Widget	*kid;	// Visible chil


  kid = value()

  if (damage() & FL_DAMAGE_ALL
  
    // Redraw everything..
    if (kid
    
      draw_box(box(), x(), y(), w(), h(), kid->color())
      draw_child(*kid)
    
    els
      draw_box(box(), x(), y(), w(), h(), color())

  
  else if (kid
    update_child(*kid)



/
// 'Fl_Wizard::next()' - Show the next child
/

voi
Fl_Wizard::next(

  int			num_kids
  Fl_Widget	* const *kids


  if ((num_kids = children()) == 0
    return

  for (kids = array(); num_kids > 0; kids ++, num_kids --
    if ((*kids)->visible()
      break

  if (num_kids > 1
    value(kids[1])



/
// 'Fl_Wizard::prev()' - Show the previous child
/


voi
Fl_Wizard::prev(

  int			num_kids
  Fl_Widget	* const *kids


  if ((num_kids = children()) == 0
    return

  for (kids = array(); num_kids > 0; kids ++, num_kids --
    if ((*kids)->visible()
      break

  if (num_kids > 0 && num_kids < children()
    value(kids[-1])



/
// 'Fl_Wizard::value()' - Return the current visible child
/

Fl_Widget 
Fl_Wizard::value(

  int			num_kids
  Fl_Widget	* const *kids
  Fl_Widget		*kid


  if ((num_kids = children()) == 0
    return ((Fl_Widget *)0)

  for (kids = array(), kid = (Fl_Widget *)0; num_kids > 0; kids ++, num_kids --
  
    if ((*kids)->visible()
    
      if (kid
        (*kids)->hide()
      els
        kid = *kids
    
  

  if (!kid
  
    kids --
    kid = *kids
    kid->show()
  

  return (kid)



/
// 'Fl_Wizard::value()' - Set the visible child
/

voi
Fl_Wizard::value(Fl_Widget *kid

  int			num_kids
  Fl_Widget	* const *kids


  if ((num_kids = children()) == 0
    return

  for (kids = array(); num_kids > 0; kids ++, num_kids --
  
    if (*kids == kid
    
      if (!kid->visible()
        kid->show()
    
    els
      (*kids)->hide()
  

  // This will restore the mouse pointer to the window's default curso
  // whenever the wizard pane is changed.  Otherwise text widgets tha
  // show the next pane may leave the cursor set to the I beam, etc..
  if (window()) window()->cursor(FL_CURSOR_DEFAULT)



/
// End of "$Id$"
/
