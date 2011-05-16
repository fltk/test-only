//
// "$Id$"
//
// Fl_Wizard widget routines.
//
// Copyright 1997-2010 by Easy Software Products.
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
// Contents:

//
//   Fl_Wizard::Fl_Wizard() - Create an Fl_Wizard widget.
//   Fl_Wizard::draw()      - Draw the wizard border and visible child.
//   Fl_Wizard::next()      - Show the next child.
//   Fl_Wizard::prev()      - Show the previous child.
//   Fl_Wizard::value()     - Return the current visible child.
//   Fl_Wizard::value()     - Set the visible child.
//

//
// Include necessary header files...
//

#include <fltk3/WizardGroup.h>
#include <fltk3/Window.h>
#include <fltk3/draw.h>


//
// 'Fl_Wizard::Fl_Wizard()' - Create an Fl_Wizard widget.
//

/**
  The constructor creates the Fl_Wizard widget at the specified
  position and size.
  <P>The inherited destructor destroys the widget and its children.
*/
Fl_Wizard::Fl_Wizard(int        xx,	// I - Lefthand position
                     int        yy,	// I - Upper position
		     int        ww,	// I - Width
		     int        hh,	// I - Height
		     const char *l) :	// I - Label
    fltk3::Group(xx, yy, ww, hh, l)
{
  box(fltk3::THIN_UP_BOX);

  value_ = (fltk3::Widget *)0;
}


//
/** Draws the wizard border and visible child. */
void Fl_Wizard::draw() {
  fltk3::Widget	*kid;	// Visible child


  kid = value();

  if (damage() & fltk3::DAMAGE_ALL)
  {
    // Redraw everything...
    if (kid)
    {
      draw_box(box(), x(), y(), w(), h(), kid->color());
      draw_child(*kid);
    }
    else
      draw_box(box(), x(), y(), w(), h(), color());

  }
  else if (kid)
    update_child(*kid);
}


/**
  This method shows the next child of the wizard. If the last child
  is already visible, this function does nothing.
*/
void Fl_Wizard::next() {
  int			num_kids;
  fltk3::Widget	* const *kids;


  if ((num_kids = children()) == 0)
    return;

  for (kids = array(); num_kids > 0; kids ++, num_kids --)
    if ((*kids)->visible())
      break;

  if (num_kids > 1)
    value(kids[1]);
}

/** Shows the previous child.*/
void Fl_Wizard::prev()
{
  int			num_kids;
  fltk3::Widget	* const *kids;


  if ((num_kids = children()) == 0)
    return;

  for (kids = array(); num_kids > 0; kids ++, num_kids --)
    if ((*kids)->visible())
      break;

  if (num_kids > 0 && num_kids < children())
    value(kids[-1]);
}

/**  Gets the current visible child widget. */
fltk3::Widget* Fl_Wizard::value()
{
  int			num_kids;
  fltk3::Widget	* const *kids;
  fltk3::Widget		*kid;


  if ((num_kids = children()) == 0)
    return ((fltk3::Widget *)0);

  for (kids = array(), kid = (fltk3::Widget *)0; num_kids > 0; kids ++, num_kids --)
  {
    if ((*kids)->visible())
    {
      if (kid)
        (*kids)->hide();
      else
        kid = *kids;
    }
  }

  if (!kid)
  {
    kids --;
    kid = *kids;
    kid->show();
  }

  return (kid);
}

/**  Sets the child widget that is visible.*/
void Fl_Wizard::value(fltk3::Widget *kid)
{
  int			num_kids;
  fltk3::Widget	* const *kids;


  if ((num_kids = children()) == 0)
    return;

  for (kids = array(); num_kids > 0; kids ++, num_kids --)
  {
    if (*kids == kid)
    {
      if (!kid->visible())
        kid->show();
    }
    else
      (*kids)->hide();
  }

  // This will restore the mouse pointer to the window's default cursor
  // whenever the wizard pane is changed.  Otherwise text widgets that
  // show the next pane may leave the cursor set to the I beam, etc...
  if (window()) window()->cursor(fltk3::CURSOR_DEFAULT);
}



//
// End of "$Id$".
//
