//
// "$Id$"
//
// A Plugin system for FLTK, implemented in fltk3::Preferences.cxx.
//
// Copyright 2002-2010 by Matthias Melcher.
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

/* \file
 fltk3::Plugin class . */

#ifndef Fltk3_Plugin_H
#define Fltk3_Plugin_H

#include <fltk3/Preferences.h>

namespace fltk3 {
  
  /**
   \brief fltk3::Plugin allows link-time and run-time integration of binary modules.
   
   fltk3::Plugin and fltk3::PluginManager provide a small and simple solution for
   linking C++ classes at run-time, or optionally linking modules at compile
   time without the need to change the main application.
   
   fltk3::PluginManager uses static initialisation to create the plugin interface
   early during startup. Plugins are stored in a temporary database, organized
   in classes.
   
   Plugins should derive a new class from fltk3::Plugin as a base:
   \code
   class My_Plugin : public fltk3::Plugin {
   public:
     My_Plugin() : fltk3::Plugin("effects", "blur") { }
     void do_something(...);
   };
   My_Plugin blur_plugin();
   \endcode
   
   Plugins can be put into modules and either linked before distribution, or loaded
   from dynamically linkable files. An fltk3::PluginManager is used to list and 
   access all currently loaded plugins.
   \code
   fltk3::PluginManager mgr("effects");
   int i, n = mgr.plugins();
   for (i=0; i<n; i++) {
   My_Plugin *pin = (My_Plugin*)mgr.plugin(i);
   pin->do_something();
   }
   \endcode
   */
  class FLTK3_EXPORT Plugin  {
    fltk3::Preferences::ID id;
  public:
    Plugin(const char *klass, const char *name);
    virtual ~Plugin();
  };
  
  
  /**
   \brief fltk3::PluginManager manages link-time and run-time plugin binaries.
   \see fltk3::Plugin
   */
  class FLTK3_EXPORT PluginManager : public fltk3::Preferences {
  public:
    PluginManager(const char *klass);
    ~PluginManager();
    
    /** \brief Return the number of plugins in the klass.
     */
    int plugins() { return groups(); }
    fltk3::Plugin *plugin(int index);
    fltk3::Plugin *plugin(const char *name);
    fltk3::Preferences::ID addPlugin(const char *name, fltk3::Plugin *plugin);
    
    static void removePlugin(fltk3::Preferences::ID id);
    static int load(const char *filename);
    static int loadAll(const char *filepath, const char *pattern=0);
  };
  
}

#endif // !Fltk3_Preferences_H

//
// End of "$Id$".
//
