//
// "$Id$"
//
// Forms free header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

#ifndef Fl_Free_H
#define Fl_Free_H

#ifndef Fl_Widget_H
#include "Fl_Widget.H"
#endif

#define FL_NORMAL_FREE		1
#define FL_SLEEPING_FREE	2
#define FL_INPUT_FREE		3
#define FL_CONTINUOUS_FREE	4
#define FL_ALL_FREE		5

typedef int (*FL_HANDLEPTR)(Fl_Widget *, int , float, float, char);

/** Emulation of the Forms "free" widget. This emulation allows the \a free 
 * demo to run, and appears to be useful for porting programs written in 
 * \a Forms which use the \a free widget or make subclasses of the \a Forms
 * widgets.
 *
 * There are five types of free, which determine when the handle function
 * is called:
\code
#define FL_NORMAL_FREE          1
#define FL_SLEEPING_FREE        2
#define FL_INPUT_FREE           3
#define FL_CONTINUOUS_FREE      4
#define FL_ALL_FREE             5
\endcode
 * An \c FL_INPUT_FREE accepts \c FL_FOCUS events. A \c FL_CONTINUOUS_FREE
 * sets a timeout callback 100 times a second and provides a \c FL_STEP
 * event, this has obvious detrimental effects on machine performance. 
 * \c FL_ALL_FREE does both. \c FL_SLEEPING_FREE are deactivated. 
 */
class FL_EXPORT Fl_Free : public Fl_Widget {
    FL_HANDLEPTR hfunc;
    static void step(void *);
protected:
    void draw();
public:
    int handle(int);
      /** The constructor takes both the type and the handle function. The 
       * handle function should be declared as follows:
\code    
int
handle_function(Fl_Widget *w,
                int       event,
                float     event_x,
                float     event_y,
                char      key)
\endcode
       * This function is called from the the handle() method in response to
       * most events, and is called by the draw() method. The event argument
       * contains the event type:
\code
// old event names for compatability:
#define FL_MOUSE                FL_DRAG
#define FL_DRAW                 100 // NOT USED
#define FL_STEP                 101 
#define FL_FREEMEM              102 // NOT USED
#define FL_FREEZE               103 // NOT USED
#define FL_THAW                 104 // NOT USED
\endcode
       */
    Fl_Free(uchar t,int x,int y,int w,int h,const char *l,FL_HANDLEPTR hdl);
      /** The destructor will call the handle function with the event
       * \c FL_FREE_MEM. */
    ~Fl_Free();
};

// old event names for compatability:
#define FL_MOUSE		FL_DRAG
#define FL_DRAW			0
#define FL_STEP			9
#define FL_FREEMEM		12
#define FL_FREEZE		FL_UNMAP
#define FL_THAW			FL_MAP

#endif

//
// End of "$Id$".
//
