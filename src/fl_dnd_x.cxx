//
// "$Id: fl_dnd_x.cxx,v 1.8 2002/06/18 06:47:32 spitzak Exp $"
//
// Drag & Drop code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

#include <fltk/Fl.h>
#include <fltk/Fl_Window.h>
#include <fltk/x.h>

extern Atom fl_XdndAware;
extern Atom fl_XdndSelection;
extern Atom fl_XdndEnter;
extern Atom fl_XdndTypeList;
extern Atom fl_XdndPosition;
extern Atom fl_XdndLeave;
extern Atom fl_XdndDrop;
extern Atom fl_XdndStatus;
extern Atom fl_XdndActionCopy;
extern Atom fl_XdndFinished;
extern Atom fl_textplain;
//extern Atom fl_XdndProxy;
extern Atom *fl_incoming_dnd_source_types;

extern bool fl_i_own_selection[2];

extern void fl_sendClientMessage(Window window, Atom message,
                                 unsigned long d0,
                                 unsigned long d1=0,
                                 unsigned long d2=0,
                                 unsigned long d3=0,
                                 unsigned long d4=0);

// return version # of Xdnd this window supports.  Also change the
// window to the proxy if it uses a proxy:
static int dnd_aware(Window& window) {
  Atom actual; int format; unsigned long count, remaining;
  unsigned char *data = 0;
  XGetWindowProperty(fl_display, window, fl_XdndAware,
		     0, 4, False, XA_ATOM,
		     &actual, &format,
		     &count, &remaining, &data);
  if (actual == XA_ATOM && format==32 && count && data)
    return int(*(Atom*)data);
  return 0;
}

static bool drop_ok;
static bool moved;

static bool grabfunc(int event) {
  if (event == FL_RELEASE) Fl::pushed(0);
  else if (event == FL_MOVE) moved = true;
  else if (!event && fl_xevent.type == ClientMessage
	   && fl_xevent.xclient.message_type == fl_XdndStatus) {
    drop_ok = fl_xevent.xclient.data.l[1] != 0;
    if (drop_ok) fl_dnd_action = fl_xevent.xclient.data.l[3];
  }
  return false;
}

extern bool (*fl_local_grab)(int); // in Fl.cxx

// send an event to an fltk window belonging to this program:
static bool local_handle(int event, Fl_Window* window) {
  fl_local_grab = 0;
  Fl::e_x = Fl::e_x_root-window->x();
  Fl::e_y = Fl::e_y_root-window->y();
  int ret = Fl::handle(event,window);
  fl_local_grab = grabfunc;
  return ret;
}

bool Fl::dnd() {
  // Remember any user presets for the action and types:
  Atom* types;
  Atom action;
  Atom local_source_types[2] = {fl_textplain, 0};
  if (fl_dnd_source_types == fl_incoming_dnd_source_types) {
    types = local_source_types;
    action = fl_XdndActionCopy;
  } else {
    types = fl_dnd_source_types;
    action = fl_dnd_source_action;
  }

  Fl_Window* source_fl_window = Fl::first_window();
  Window source_window = fl_xid(source_fl_window);

  fl_local_grab = grabfunc;
  Window target_window = 0;
  Fl_Window* local_window = 0;
  int version = 4; int dest_x, dest_y;
  XSetSelectionOwner(fl_display, fl_XdndSelection, fl_message_window, fl_event_time);
  Fl_Cursor oldcursor = FL_CURSOR_DEFAULT;
  drop_ok = true;
  moved = true;

  while (Fl::pushed()) {

    // figure out what window we are pointing at:
    Window new_window = 0; int new_version = 0;
    Fl_Window* new_local_window = 0;
    for (Window child = RootWindow(fl_display, fl_screen);;) {
      Window root; unsigned int junk3;
      XQueryPointer(fl_display, child, &root, &child,
		    &e_x_root, &e_y_root, &dest_x, &dest_y, &junk3);
      if (!child) {
	if (!new_window && (new_version = dnd_aware(root))) new_window = root;
	break;
      }
      new_window = child;
      if ((new_local_window = fl_find(child))) break;
      if ((new_version = dnd_aware(new_window))) break;
    }

    if (new_window != target_window) {
      if (local_window) {
	fl_dnd_source_window = 0;
	local_handle(FL_DND_LEAVE, local_window);
      } else if (version) {
	fl_sendClientMessage(target_window, fl_XdndLeave, source_window);
      }
      version = new_version;
      target_window = new_window;
      local_window = new_local_window;
      if (local_window) {
	fl_dnd_source_window = source_window;
	fl_dnd_source_types = types;
	fl_dnd_type = fl_textplain;
	local_handle(FL_DND_ENTER, local_window);
      } else if (version) {
	fl_sendClientMessage(target_window, fl_XdndEnter, source_window,
			     version<<24,
			     types[0], types[1], types[1] ? types[2] : 0);
      }
    }
    if (local_window) {
      fl_dnd_source_window = source_window;
      fl_dnd_source_types = types;// ?? is this needed?
      fl_dnd_action = fl_XdndActionCopy;
      drop_ok = local_handle(FL_DND_DRAG, local_window);
    } else if (version) {
      if (moved)
	fl_sendClientMessage(target_window, fl_XdndPosition, source_window,
			     0, (e_x_root<<16)|e_y_root, fl_event_time,
			     fl_dnd_source_action);
    } else {
      // Windows that don't support DnD are reported as ok because
      // we are going to try the middle-mouse click on them:
      drop_ok = types[0]==fl_textplain;
    }
    Fl_Cursor cursor = Fl_Cursor(drop_ok ? 21 : 1);
    if (cursor != oldcursor) {
      oldcursor = cursor;
      source_fl_window->cursor(cursor);
    }
    moved = false;
    Fl::wait();
  }

  if (!drop_ok) ;
  else if (local_window) {
    fl_i_own_selection[0] = true;
    if (local_handle(FL_DND_RELEASE, local_window)) paste(*belowmouse(),false);
  } else if (version) {
    fl_sendClientMessage(target_window, fl_XdndDrop, source_window,
		      0, fl_event_time);
  } else if (target_window) {
    // fake a drop by clicking the middle mouse button:
    XButtonEvent msg;
    msg.type = ButtonPress;
    msg.window = target_window;
    msg.root = RootWindow(fl_display, fl_screen);
    msg.subwindow = 0;
    msg.time = fl_event_time+1;
    msg.x = dest_x;
    msg.y = dest_y;
    msg.x_root = Fl::e_x_root;
    msg.y_root = Fl::e_y_root;
    msg.state = 0x0;
    msg.button = Button2;
    XSendEvent(fl_display, target_window, False, 0L, (XEvent*)&msg);
    msg.time++;
    msg.state = 0x200;
    msg.type = ButtonRelease;
    XSendEvent(fl_display, target_window, False, 0L, (XEvent*)&msg);
  }

  fl_local_grab = 0;
  if (oldcursor) source_fl_window->cursor(FL_CURSOR_DEFAULT);

  // reset the action and type:
  fl_dnd_source_types = fl_incoming_dnd_source_types;

  return drop_ok;
}


//
// End of "$Id: fl_dnd_x.cxx,v 1.8 2002/06/18 06:47:32 spitzak Exp $".
//
