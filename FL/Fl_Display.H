/
// "$Id$
/
// Fl_Display class for screen devices the Fast Light Tool Kit (FLTK)
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


#ifndef Fl_Display_
#define Fl_Display_

#include <FL/Fl_Device.H


/*
* Fl_Display is a base class for all screen devices
* In the future it will also define methods for access of keyboard, mous
* and window damage events to encapsulate all interaction with the use
* and forming a "virtual terminal"
*

class FL_EXPORT Fl_Display: public Fl_Device
protected
#ifdef  WIN3
 friend class Fl_GDI_Printer; //for arc and curve mplementatio
#endi
 // common independent methods, can be overriden if better implementations are know
 void arc(double x, double y, double r, double start, double end)
 void curve(double X0, double Y0, double X1, double Y1, double X2, double Y2, double X3, double Y3)
};


#endi

/
// "$Id$
/
