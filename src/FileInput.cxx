//
// "$Id$"
//
// FileInput routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1997-1999 by Easy Software Products.
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
// Contents:
//
//   FileInput::handle() - Handle tab events.
//

//
// Include necessary header files...
//

#include <fltk/events.h>
#include <fltk/FileInput.h>

using namespace fltk;

//
// Note: This has got to be a runner for the world's shortest class.
//       Basically the whole purpose of this subclass is to alter the
//       behavior of the Tab key; specifically if the user hits the
//       Tab key while text is selected, the cursor is moved to the
//       end of the selected text rather than doing the navigation
//       thing.
//

//
// 'FileInput::handle()' - Handle tab events.
//

int				// O - 1 if we handled the event
FileInput::handle(int event)	// I - Event to handle
{
  if (event == KEY && event_key() == TabKey &&
      mark() != position())
  {
    // Set the current cursor position to the end of the selection...
    if (mark() > position())
      position(mark());
    else
      position(position());

    return (1);
  }
  else
  {
    // Use the Input handler...
    return (Input::handle(event));
  }
}


//
// End of "$Id$".
//
