//
// "$Id$"
//
// A Plugin system for FLTK, implemented in Fl_Plugin_Manager.cxx.
// FLTK 123 wrapper completed
//
// Copyright 2002-2011 by Matthias Melcher.
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

#ifndef Fl_Plugin_H
#define Fl_Plugin_H

#include <fltk3/Plugin.h>
#include "Fl_Preferences.H"


class FL_EXPORT Fl_Plugin : public fltk3::Plugin {
public:
  Fl_Plugin(const char *klass, const char *name) 
  : fltk3::Plugin(klass, name) 
  {
  }
};


class FL_EXPORT Fl_Plugin_Manager : public fltk3::PluginManager {
public:
  Fl_Plugin_Manager(const char *klass) 
  : fltk3::PluginManager(klass)
  {
  }
   
  Fl_Plugin *plugin(int index) {
    return (Fl_Plugin*)fltk3::PluginManager::plugin(index);
  }

  Fl_Plugin *plugin(const char *name) {
    return (Fl_Plugin*)fltk3::PluginManager::plugin(name);
  }

};


#endif // !Fl_Plugin_Manager_H

//
// End of "$Id$".
//
