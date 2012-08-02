//
// "$Id$"
//
// Plugin manager header file for the Fast Light Tool Kit (FLTK).
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

#ifndef FLUID3_PLUGINS_H
#define FLUID3_PLUGINS_H

#include <fltk3/Plugin.h>

namespace fluid3 {
  
  class CommandlinePlugin : public fltk3::Plugin {
  public:
    CommandlinePlugin(const char *name);
    virtual const char *klass();
    
    // return a unique name for this plugin
    virtual const char *name() = 0;
    virtual const char *help() = 0;
    virtual int arg(int argc, char **argv, int &i) = 0;
    virtual int test(const char *a1=0L, const char *a2=0L, const char *a3=0L);
    virtual void show_panel();
  };
  
};

#endif

//
// End of "$Id$".
//
