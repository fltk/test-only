// Carl Everard Thompson (clip@home.net)
//
// fl_conf.H
//
// C++ wrapper for my config file C routines.
// Please see conf.h for information on how this really works.
//
// Added configuration file routines for FLTK.
//
// Last modified:	19Aug1998

#ifndef fl_config_H
#define fl_config_H

#include <stdlib.h>
#include <string.h>
#include <FL/Enumerations.H>
#include <FL/conf.h>

class Fl_Config
{
    protected:
    
    int       _dos;
    char      *_configFile;
    conf_list _list;
        
    public:
    
    // constructor
    Fl_Config(const char *configFile = 0)
#ifdef WIN32
        { config_file(configFile); _list = 0; _dos = 1; }
#else
        { config_file(configFile); _list = 0; _dos = 0; }
#endif

    // destructor
    virtual ~Fl_Config()
        { if (_configFile) free(_configFile); conf_list_free(&_list); }
    
    // get the list returned by get_sections() or get_keys()
    conf_list list() { return _list; }
    
    // get/set DOS mode
    int dos_mode() { return _dos; }
    void dos_mode(int val) { _dos = val ? 1 : 0; }
        
    // get/set the name of the config file
    const char *config_file() { return _configFile; }
    void config_file(const char *configFile)
        { _configFile = configFile ? strdup(configFile) : 0; }
    
    // get sections in configuration file
    int sections(const char *section)
        { return getconf_sections(_configFile, section, &_list); }
    int sections()
        { return getconf_sections(_configFile, "", &_list); }

    // get keys in configuration file
    int keys(const char *section)
        { return getconf_keys(_configFile, section, &_list); }
    int keys()
        { return getconf_keys(_configFile, "", &_list); }

    // get the string value of a key from the config file
    int get(const char *key, char *value, int value_length)
        { return getconf(_configFile, key, value, value_length); }

    // get the string value of a key from the global or user's config file
    int find(const char *key, char *value, int value_length);

    // get the long value of a key from the config file
    int get(const char *key, long &lvalue)
        { return getconf_long(_configFile, key, &lvalue); }

    // get the long value of a key from the global or user's config file
    int find(const char *key, long &lvalue);

    // get the int value of a key from the config file
    int get(const char *key, int &ivalue)
        { int r; long l; r = getconf_long(_configFile, key, &l); ivalue = (int)l; return r; }

    // get the int value of a key from the global or user's config file
    int find(const char *key, int &ivalue);

    // get the uchar value of a key from the config file
    int get(const char *key, uchar &ucvalue)
        { int r; long l; r = getconf_long(_configFile, key, &l); ucvalue = (uchar)l; return r; }

    // get the uchar value of a key from the global or user's config file
    int find(const char *key, uchar &ucvalue);

    // get the boolean value of a key from the config file
    int get_boolean(const char *key, int &bvalue)
        { return getconf_boolean(_configFile, key, &bvalue); }

    // get the boolean value of a key from the global or user's config file
    int find_boolean(const char *key, int &bvalue);

    // set the string value of a key in the config file
    int set(const char *key, const char *value = "")
        { conf_DOS = _dos; return setconf(_configFile, key, value); }
        
    // clear the string value of a key in the config file
    int clear(const char *key)
        { conf_DOS = _dos; return setconf(_configFile, key, ""); }
        
    // set the long value of a key in the config file
    int set(const char *key, long lvalue)
        { conf_DOS = _dos; return setconf_long(_configFile, key, lvalue); }
        
    // set the int value of a key in the config file
    int set(const char *key, int ivalue)
        { conf_DOS = _dos; return setconf_long(_configFile, key, (long)ivalue); }
        
    // set the boolean value of a key in the config file
    int set_boolean(const char *key, int bvalue)
        { conf_DOS = _dos; return setconf_boolean(_configFile, key, bvalue); }
        
    // delete a key/value from the config file
    int del(const char *key)
        { conf_DOS = _dos; return delconf(_configFile, key); }

   // return error string associated with conf result code
   const char *strerror(int errnum)
       { return getconf_error(errnum); }
};

#endif
