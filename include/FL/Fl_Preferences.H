//
// "$Id$"
//
// Preferences .
//
// Copyright 2002-2012 by Matthias Melcher.
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

#ifndef Fl_Preferences_H
#define Fl_Preferences_H

#include <fltk3/Preferences.h>
#include <stdio.h>
#include "Fl_Export.H" 


class FL_EXPORT Fl_Preferences : public fltk3::Preferences {

public: 

  Fl_Preferences( Root root, const char *vendor, const char *application ) 
  : fltk3::Preferences(root, vendor, application)
  {
  }
  
  Fl_Preferences( const char *path, const char *vendor, const char *application ) 
  : fltk3::Preferences(path, vendor, application)
  {
  }
  
  Fl_Preferences( Fl_Preferences &parent, const char *group ) 
  : fltk3::Preferences(parent, group)
  {
  }
  
  Fl_Preferences( Fl_Preferences *parent, const char *group ) 
  : fltk3::Preferences(parent, group)  
  {
  }
  
  Fl_Preferences( Fl_Preferences &parent, int groupIndex ) 
  : fltk3::Preferences(parent, groupIndex)
  {
  }
  
  Fl_Preferences( Fl_Preferences *parent, int groupIndex ) 
  : fltk3::Preferences(parent, groupIndex)
  {
  }
  
  Fl_Preferences(const Fl_Preferences &p)
  : fltk3::Preferences(p)
  {
  }
  
  Fl_Preferences( ID id ) 
  : fltk3::Preferences(id)
  {
  }
  
};

#endif // !Fl_Preferences_H

//
// End of "$Id$".
//
