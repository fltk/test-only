//
// "$Id$"
//
// Plugin manager code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Matthias Melcher and others.
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

#include "plugins.h"

/*
 \class fluid3::CommandlinePlugin
 This class can be used to write plugins for new command line arguments.
 A sample implementation exists in the subversion registry. It is currently
 hard to find, but I promise to dig it up as soon as I can.
 */


fluid3::CommandlinePlugin::CommandlinePlugin(const char *name)
: fltk3::Plugin(klass(), name) 
{ 
}


const char *fluid3::CommandlinePlugin::klass() { 
  return "commandline"; 
}


// optional test the plugin
int fluid3::CommandlinePlugin::test(const char *a1, const char *a2, const char *a3) 
{ 
  return 0;
}

// show a GUI panel to edit some data
void fluid3::CommandlinePlugin::show_panel() 
{ 
}


//
// End of "$Id$".
//
