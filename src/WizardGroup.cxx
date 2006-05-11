//
// "$Id: WizardGroup.cxx 4288 2005-04-16 00:13:17Z mike $"
//
// Wizard widget routines.
//
// Copyright 1997-2006 by Easy Software Products and others.
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
//   WizardGroup::Wizard() - Create an Wizard widget.
//   WizardGroup::draw()      - Draw the wizard border and visible child.
//   WizardGroup::next()      - Show the next child.
//   WizardGroup::prev()      - Show the previous child.
//   WizardGroup::value()     - Return the current visible child.
//   WizardGroup::value()     - Set the visible child.
//

//
// Include necessary header files...
//

#include <fltk/Window.h>
#include <fltk/draw.h>
#include <fltk/Cursor.h>
#include <fltk/WizardGroup.h>

using namespace fltk;

void WizardGroup::draw() {
  if (!value()) value(children() ? child(0) : 0);  
  Group::draw();
}

void WizardGroup::next() {
  int	         num_kids, i;
  Widget	*kids;

  if ((num_kids = children()) == 0)  return;
  for (kids = child(0),i=0; i<num_kids; kids=child(++i))
    if (kids->visible()) break;
  if (i < num_kids-1)  {kids->hide(); value(child(i+1));}
}

void WizardGroup::prev() {
  int	         num_kids, i;
  Widget	*kids;

  if ((num_kids = children()) == 0)  return;
  for (kids = child(0),i=0; i<num_kids; kids=child(++i))
    if (kids->visible()) break;
  if (i > 0 && i < num_kids) {kids->hide(); value(child(i-1));}
}

void WizardGroup::value(Widget *kid)
{
  int num_kids, i;

  value_=kid;
  if ((num_kids = children()) == 0)  return;

  for (i=0; i<children(); i++) 
    if (child(i) != kid)  child(i)->hide(); else kid->show();

  // This will restore the mouse pointer to the window's default cursor
  // whenever the wizard pane is changed.  Otherwise text widgets that
  // show the next pane may leave the cursor set to the I beam, etc...
  if (window()) window()->cursor(CURSOR_DEFAULT);
}


//
// End of "$Id: WizardGroup.cxx 4288 2005-04-16 00:13:17Z mike $".
//
