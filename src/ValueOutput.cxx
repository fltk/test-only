//
// "$Id: ValueOutput.cxx,v 1.2 2004/01/06 06:43:02 spitzak Exp $"
//
// Copyright 1998-2003 by Bill Spitzak and others.

#include <fltk/ValueOutput.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/draw.h>
#include <fltk/Box.h>
#include <fltk/run.h>
using namespace fltk;

/*! \class fltk::ValueOutput

  A valuator that displays the number like a text box. This is
  indended for showing the user a number, there is no way for the user
  to change the number. It is much lighter weight than using an Output
  widget for this.

  There is no way for the user to change the number, but calling value()
  will change it.
*/

// WAS: I put in some code so that the user can copy the data, by clicking
// on the field. However this is all commented out because the result
// was confusing because it moved the focus to the widget, yet most
// keystrokes do not do anything.

void ValueOutput::draw() {
  if (damage() & DAMAGE_ALL) draw_box();
  int x = 0, y = 0, w = this->w(), h = this->h(); box()->inset(x,y,w,h);
  push_clip(x,y,w,h);
  setfont(textfont(), textsize());
  char buf[40];
  format(buf);
  int xx = 0;
//    if (focused()) {
//      int dy = (h-int(textsize())-1)>>1;
//      setcolor(focused() ? selection_color() : color());
//      xx = int(getwidth(buf))+2;
//      fillrect(x+2, y+dy, xx-2, int(textsize())+2);
//    }
  if (!(damage() & DAMAGE_ALL)) {
    setcolor(color());
    fillrect(x+xx, y, w-xx, h);
  }
  setcolor(/*focused() ? selection_textcolor() :*/ textcolor());
  drawtext(buf, x+3, y+(int(h+getascent()-getdescent()) >> 1));
  pop_clip();
}

int ValueOutput::handle(int event) {
  switch (event) {
  case FOCUS:
    return false;
//    case PUSH:
//      take_focus();
//      // copy it to selection for middle-mouse paste:
//      {char buf[40]; fltk::copy(buf, format(buf), false);}
//      return true;
//    case DRAG:
//    case RELEASE:
//      // maybe it should do DnD?
//      return true;
//    case KEY:
//      if (event_state(CTRL) && (event_key() == 'x' || event_key() == 'c')) {
//        // copy it to clipboard for normal paste:
//        {char buf[40]; fltk::copy(buf, format(buf), true);}
//        return true;
//      } else return false;
  default:
    return Valuator::handle(event);
  }
}
 
