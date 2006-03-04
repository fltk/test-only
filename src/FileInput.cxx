//
// "$Id$"
//
// File_Input header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
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

#include <fltk/FileInput.h>
#include <fltk/Window.h>
#include <fltk/draw.h>
#include <fltk/damage.h>
#include <fltk/events.h>
#include <fltk/Cursor.h>
#include <fltk/Rectangle.h>
#include <fltk/run.h>
#include "flstring.h"

// temporary for new 1.1.x button FileInput features to come
//#define DIR_HEIGHT	10

//
// Height of directory buttons...
//

#define DIR_HEIGHT	10


//
// Redraw bit for directory bar...
//

#define DAMAGE_BAR	0x10


using namespace fltk;
//
// 'FileInput::FileInput()' - Create a FileInput widget.
//

FileInput::FileInput(int X, int Y, int W, int H, const char *l)
  : Input(X, Y, W, H, l) {
  buttons_[0] = 0;
  errorcolor_ = RED;
  ok_entry_   = 1;
  pressed_    = -1;
  fltk::damage(DAMAGE_ALL);
  down_box(UP_BOX);
}

//
// 'FileInput::draw_buttons()' - Draw directory buttons.
//

void
FileInput::draw_buttons() {
  int	i,					// Looping var
	X;					// Current X position


  if (fltk::damage() & (DAMAGE_BAR | DAMAGE_ALL)) {
    update_buttons();
  }

  for (X = 0, i = 0; buttons_[i]; i ++)
  {
    FrameBox * b = (FrameBox*) (pressed_ ==i ? DOWN_BOX : UP_BOX);
    Rectangle r;
    if ((X + buttons_[i]) > xscroll()) {
      if (X < xscroll()) 
             r = Rectangle(x(), y(), X + buttons_[i] - xscroll(), DIR_HEIGHT);
      else if ((X + buttons_[i] - xscroll()) > w()) 
       	 r = Rectangle(x() + X - xscroll(), y(), w() - X + xscroll(), DIR_HEIGHT);
      else 
        r = Rectangle(x() + X - xscroll(), y(), buttons_[i], DIR_HEIGHT);
	b->draw(r);    
    }

    X += buttons_[i];
  }

  if (X < w()) {
    FrameBox * b = (FrameBox*) (pressed_ ==i ? DOWN_BOX : UP_BOX);
    Rectangle r = Rectangle( x() + X - xscroll(), y(), w() - X + xscroll(), DIR_HEIGHT);
    b->draw(r);
  }
}

//
// 'FileInput::update_buttons()' - Update the sizes of the directory buttons.
//

void
FileInput::update_buttons() {
  int		i;				// Looping var
  const char	*start,				// Start of path component
		*end;				// End of path component


//  puts("update_buttons()");

  // Set the current font & size...
  labelfont(textfont());
  labelsize(textsize());

  // Loop through the value string, setting widths...
  for (i = 0, start = value();
       start && i < (int)(sizeof(buttons_) / sizeof(buttons_[0]) - 1);
       start = end, i ++) {
//    printf("    start = \"%s\"\n", start);
    if ((end = strchr(start, '/')) == NULL)
#if defined(WIN32) || defined(__EMX__)
      if ((end = strchr(start, '\\')) == NULL)
#endif // WIN32 || __EMX__
      break;

    end ++;

    buttons_[i] = (short)getwidth(start, end - start);
    if (!i) buttons_[i] += ((FrameBox*)box())->dx() + 6;
  }

//  printf("    found %d components/buttons...\n", i);

  buttons_[i] = 0;
}


//
// 'FileInput::value()' - Set the value of the widget...
//

int						// O - TRUE on success
FileInput::value(const char *str,		// I - New string value
                     int        len) {		// I - Length of value
  fltk::damage(DAMAGE_BAR);
  return Input::value(str,len);
}


int						// O - TRUE on success
FileInput::value(const char *str) {		// I - New string value
  fltk::damage(DAMAGE_BAR);
  return Input::value(str);
}


//
// 'FileInput::draw()' - Draw the file input widget...
//

void
FileInput::draw() {
#ifdef NEW_BTNS
  FrameBox* b = (FrameBox*)box();
  if (fltk::damage() & (DAMAGE_BAR | DAMAGE_ALL)) draw_buttons();
  // this flag keeps Input_::drawtext from drawing a bogus box!
  char must_trick_Input_ = 
      fltk::focus()!=this && !size() && !(fltk::damage()&DAMAGE_ALL);
  if ((fltk::damage() & DAMAGE_ALL) || must_trick_Input_) {
    Rectangle r = Rectangle(x(),y()+DIR_HEIGHT,w(),h()-DIR_HEIGHT);
    b->draw (r);
  }
  
  Rectangle r = Rectangle(x() + b->dx()+3, y()+b->dy()+DIR_HEIGHT,
		        w()-b->dw()-6, h()-b->dh()-DIR_HEIGHT);
  if (!must_trick_Input_) 
#endif
      Input::draw();
}


//
// 'FileInput::handle()' - Handle events in the widget...
//

int						// O - TRUE if we handled event
FileInput::handle(int event) 		// I - Event
{
#ifdef NEW_BTN
    //  printf("handle(event = %d)\n", event);

  switch (event) {
    case MOVE :
    case ENTER :
      if (active_r()) {
	if (event_y() < (y() + DIR_HEIGHT)) window()->cursor(CURSOR_DEFAULT);
	else window()->cursor(CURSOR_INSERT);
      }

      return 1;

    case PUSH :
    case RELEASE :
    case DRAG :
      if (event_y() < (y() + DIR_HEIGHT) || pressed_ >= 0) return handle_button(event);

      return Input::handle(event);

    default :
      if (Input::handle(event)) {
	  fltk::damage(DAMAGE_BAR);
	return 1;
      }

      return 0;
  }
#else
  return Input::handle(event);
#endif

}


//
// 'FileInput::handle_button()' - Handle button events in the widget...
//

int						// O - TRUE if we handled event
FileInput::handle_button(int event)		// I - Event
{
  int		i,				// Looping var
		X;				// Current X position
  char		*start,				// Start of path component
		*end;				// End of path component
  char		newvalue[1024];			// New value


  // Figure out which button is being pressed...
  for (X = 0, i = 0; buttons_[i]; i ++)
  {
    X += buttons_[i];

    if (X > xscroll() && event_x() < (x() + X - xscroll())) break;
  }

//  printf("handle_button(event = %d), button = %d\n", event, i);

  // Redraw the directory bar...
  if (event == RELEASE) pressed_ = -1;
  else pressed_ = (short)i;

  window()->make_current();
  draw_buttons();

  // Return immediately if the user is clicking on the last button or
  // has not released the mouse button...
  if (!buttons_[i] || event != RELEASE) return 1;

  // Figure out where to truncate the path...
  strlcpy(newvalue, value(), sizeof(newvalue));

  for (start = newvalue, end = start; start && i >= 0; start = end, i --) {
//    printf("    start = \"%s\"\n", start);
    if ((end = strchr(start, '/')) == NULL)
#if defined(WIN32) || defined(__EMX__)
      if ((end = strchr(start, '\\')) == NULL)
#endif // WIN32 || __EMX__
      break;

    end ++;
  }

  if (i < 0) {
    // Found the end; truncate the value and update the buttons...
    *start = '\0';
    value(newvalue, start - newvalue);

    // Then do the callbacks, if necessary...
    set_changed();
    if (when() & WHEN_CHANGED) do_callback();
  }

  return 1;
}


//
// End of "$Id$".
//
