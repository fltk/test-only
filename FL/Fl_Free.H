/
// "$Id$
/
// Forms free header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
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

#ifndef Fl_Free_
#define Fl_Free_

#ifndef Fl_Widget_
#include "Fl_Widget.H
#endi

#define FL_NORMAL_FREE		
#define FL_SLEEPING_FREE	
#define FL_INPUT_FREE		
#define FL_CONTINUOUS_FREE	
#define FL_ALL_FREE		

typedef int (*FL_HANDLEPTR)(Fl_Widget *, int , float, float, char)

/** Emulation of the Forms "free" widget. This emulation allows the \a free
 * demo to run, and appears to be useful for porting programs written in
 * \a Forms which use the \a free widget or make subclasses of the \a Form
 * widgets
 
 * There are five types of free, which determine when the handle functio
 * is called
\cod
#define FL_NORMAL_FREE          
#define FL_SLEEPING_FREE        
#define FL_INPUT_FREE           
#define FL_CONTINUOUS_FREE      
#define FL_ALL_FREE             
\endcod
 * An \c FL_INPUT_FREE accepts \c FL_FOCUS events. A \c FL_CONTINUOUS_FRE
 * sets a timeout callback 100 times a second and provides a \c FL_STE
 * event, this has obvious detrimental effects on machine performance.
 * \c FL_ALL_FREE does both. \c FL_SLEEPING_FREE are deactivated.
 *
class FL_EXPORT Fl_Free : public Fl_Widget 
    FL_HANDLEPTR hfunc
    static void step(void *)
protected
    void draw()
public
    int handle(int)
      /** The constructor takes both the type and the handle function. The
       * handle function should be declared as follows
\code   
in
handle_function(Fl_Widget *w
                int       event
                float     event_x
                float     event_y
                char      key
\endcod
       * This function is called from the the handle() method in response t
       * most events, and is called by the draw() method. The event argumen
       * contains the event type
\cod
// old event names for compatability
#define FL_MOUSE                FL_DRA
#define FL_DRAW                 100 // NOT USE
#define FL_STEP                 101
#define FL_FREEMEM              102 // NOT USE
#define FL_FREEZE               103 // NOT USE
#define FL_THAW                 104 // NOT USE
\endcod
       *
    Fl_Free(uchar t,int x,int y,int w,int h,const char *l,FL_HANDLEPTR hdl)
      /** The destructor will call the handle function with the even
       * \c FL_FREE_MEM. *
    ~Fl_Free()
}

// old event names for compatability
#define FL_MOUSE		FL_DRA
#define FL_DRAW			
#define FL_STEP			
#define FL_FREEMEM		1
#define FL_FREEZE		FL_UNMA
#define FL_THAW			FL_MA

#endi

/
// End of "$Id$"
/
