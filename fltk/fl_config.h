//
//  "$Id: fl_config.h,v 1.2 2001/07/24 04:44:26 clip Exp $"
//
//  Configuration file routines for the Fast Light Tool Kit (FLTK).
//
//  Carl Everard Thompson (clip@home.net)
//
//  fl_conf.H
//
//  FLTK C++ wrapper for config file C routines.
//  Please see conf.h for information on how the C routines work.
//
//  Last modified:	22Mar1999
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
//  USA.

#ifndef fl_config_H
#define fl_config_H

#include <stdlib.h>
#include <string.h>
#include <fltk/Fl.h>
#include <fltk/Enumerations.h>
#include <fltk/conf.h>

class FL_API Fl_Config
{
  protected:
    char      *_configFile;
    conf_list _list;
    int       _error;

  public:
    // constructor
    Fl_Config(const char *configFile = 0)
        { config_file(configFile); _list = 0; _error = 0; }

    // destructor
    virtual ~Fl_Config()
        { if (_configFile) free(_configFile); conf_list_free(&_list); }

    // get the list returned by get_sections() or get_keys()
    conf_list list() { return _list; }

    // get/set the name of the config file
    const char *config_file() { return _configFile; }
    void config_file(const char *configFile)
        { _configFile = configFile ? strdup(fl_find_config_file(configFile, true)) : 0; }

    // get sections in configuration file
    int sections(const char *section)
        { return _error = getconf_sections(_configFile, section, &_list); }
    int sections()
        { return _error = getconf_sections(_configFile, "", &_list); }

    // get keys in configuration file
    int keys(const char *section)
        { return _error = getconf_keys(_configFile, section, &_list); }
    int keys()
        { return _error = getconf_keys(_configFile, "", &_list); }

    // get the string value of a key from the config file
    int get(const char *key, char *value, int value_length)
        { return _error = getconf(_configFile, key, value, value_length); }

    // get the long value of a key from the config file
    int get(const char *key, long &lvalue)
        { return _error = getconf_long(_configFile, key, &lvalue); }

    // get the int value of a key from the config file
    int get(const char *key, int &ivalue) {
      long l;
      _error = getconf_long(_configFile, key, &l);
      ivalue = (int)l;
      return _error;
    }

    // get the uchar value of a key from the config file
    int get(const char *key, uchar &ucvalue) {
      long l;
      _error = getconf_long(_configFile, key, &l);
      ucvalue = (uchar)l;
      return _error;
    }

    // get the boolean value of a key from the config file
    int get_boolean(const char *key, int &bvalue)
        { return _error = getconf_boolean(_configFile, key, &bvalue); }

    // set the string value of a key in the config file
    int set(const char *key, const char *value = "")
        { return _error = setconf(_configFile, key, value); }

    // clear the string value of a key in the config file
    int clear(const char *key)
        { return _error = setconf(_configFile, key, ""); }

    // set the long value of a key in the config file
    int set(const char *key, long lvalue)
        { return _error = setconf_long(_configFile, key, lvalue); }

    // set the int value of a key in the config file
    int set(const char *key, int ivalue)
        { return _error = setconf_long(_configFile, key, (long)ivalue); }

    // set the boolean value of a key in the config file
    int set_boolean(const char *key, int bvalue)
        { return _error = setconf_boolean(_configFile, key, bvalue); }

    // delete a key/value from the config file
    int del(const char *key)
        { return _error = delconf(_configFile, key); }

    // return error string associated with conf result code
    const char *strerror()
        { return getconf_error(_error); }

    const char *strerror(int errnum)
        { return getconf_error(errnum); }

    static void clear_cache() { conf_clear_cache(); }
};

#endif

//
// End of "$Id: fl_config.h,v 1.2 2001/07/24 04:44:26 clip Exp $".
//
