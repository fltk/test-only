/*
   "$Id: conf.h,v 1.9 1999/10/26 16:26:23 mike Exp $"

    Configuration file routines for the Fast Light Tool Kit (FLTK).

    Carl Thompson's config file routines version 0.21
    Copyright 1995-1999 Carl Everard Thompson (clip@home.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
    USA.
*/
/*
        file:           conf.h
        author:         Carl Thompson (clip@home.net)
        source type:    Ansi C
        start date:     16 Dec 1995
        last modified:  8 Aug 1999

        Typical config file format:

        ------- test.conf --------------------------------------------------------------------------------------------------------
		
		key1 = value1							# this key is not in a section
		key2								# this key has no value associated
		
                [section1]
		# Three standard key/value pairs
            	    key1 = value1
            	    key2 = value2
            	    key3 = value3

                    [section1/subsection]					# this is how a subsection is defined
                	key1 = value1
                	key2 = value2

                [section2]
        	    key1 = value1
            	    key2 = value2

        --------------------------------------------------------------------------------------------------------------------------


        Notes:
        - comments may be on a line by themselves or at the end of a line
	  - for comments at the end of the line, the comment separator
	    character must followed by whitespace
        - indentation/formatting is handled automatically (sorry!)
*/



#ifndef CONF_H
#  define CONF_H


/* includes */
#  include <sys/types.h>
#  include <stdlib.h>
#  include <stdio.h>
#  include <string.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <ctype.h>
#  include <errno.h>
#  if defined(WIN32) || defined(__EMX__)
#    include <io.h>
#  else
#    include <unistd.h>
#  endif /* WIN32 || __EMX__ */


/* misc defines */
#  define CONF_WHITESPACE	" \t\n\r"	/* all whitespace characters */
#  define CONF_MAX_LEVEL	10		/* maximum depth of sections in config file */
#  define CONF_INDENT		2		/* number of spaces to use for indenting */
#  define CONF_MAX_LINE_LEN	512		/* maximum length of line in config file */
#  define CONF_MAX_SECT_LEN	128		/* maximum length of section name */
#  define CONF_MAXPATHLEN	128		/* maximum length of pathname */


/* error defines */
#  define CONF_SUCCESS		0		/* successful return */
#  define CONF_ERR_ARGUMENT	1		/* argument invalid (NULL?) */
#  define CONF_ERR_FILE		2		/* trouble accessing config file or directory */
#  define CONF_ERR_SECTION	3		/* requested section was not found */
#  define CONF_ERR_KEY		4		/* requested key was not found */
#  define CONF_ERR_DEPTH	5		/* nested sections in config file too deep */
#  define CONF_ERR_MEMORY	6		/* memory allocation error */
#  define CONF_ERR_NOVALUE	7		/* key found, but no value associated with it */
#  define CONF_ERR_AGAIN	8		/* try operation again (lockfile existed?) */

#  ifdef __cplusplus
extern "C" {
#  endif

  /* data types */
  typedef struct _conf_entry
  {
        char *data;
        struct _conf_entry *next;
  } conf_entry;

  typedef conf_entry* conf_list;

  /* global variables */
  extern FL_API int    conf_DOS;                                                       /* write files with DOS end of line? */
  extern FL_API char   conf_sep;                                                       /* seperator charactor used in config files */
  extern FL_API char   conf_level_sep;                                                 /* seperator used to denote nested sections */
  extern FL_API char	conf_comment_sep;						/* denotes comments */
  extern FL_API int	conf_comment_column;						/* the column comments start in */


  /* function declarations */

  /*
        int getconf(const char *configfile, const char *key, char *svalue,
                    int slen)

        description:
                gets the string associated with a key in a config file
        arguments:
                configfile: path of config file
                key: section/key to look for
                slen: length of passed string buffer
        return values:
                returns 0 for OK or error code defined in conf.h
                svalue: string associated with key
  */
  int getconf(const char *configfile, const char *key, char *svalue,
              int slen);                                                        /* get the string associated with key */



  /*
        int getconf_keys(const char *configfile, const char *section,
                         conf_list *list)

        description:
                gets the list of keys that are in the specified section
        arguments:
                configfile: path of config file
                section: section in config file (must be unique in file)
                list: pointer to a conf_list (same as pointer to pointer
                      to conf_entry) of keys
        return values:
                returns 0 for OK or error code defined in conf.h
                returns a pointer to first in list of entries in "list"
  */
  int getconf_keys(const char *configfile, const char *section,
                   conf_list *list);



  /*
        int getconf_sections(const char *configfile, const char *section,
                             conf_list *list)

        description:
                gets the list of subsections that are in the specified section
        arguments:
                configfile: path of config file
                section: section in config file (must be unique in file)
                list: pointer to a conf_list (same as pointer to pointer
                      to conf_entry) of subsections
        return values:
                returns 0 for OK or error code defined in conf.h
                returns a pointer to first in list of entries in "list"
  */
  int getconf_sections(const char *configfile, const char *section,
                       conf_list *list);



  /*
        int conf_list_free(conf_list* list)

        description:
                frees all memory used by a conf_list
        arguments:
                list: the conf_list to free
        return values:
                returns 0 for OK or error code defined in conf.h
  */
  int conf_list_free(conf_list *list);



  /*
        int setconf(const char *configfile, const char *key,
                    const char *svalue)

        description:
                sets the string associated with a key in a config file
        arguments:
                configfile: path of config file
                key: section/key to set
                svalue: string associated with key
        return values:
                returns 0 for OK or error code defined in conf.h
  */
  int setconf(const char *configfile, const char *key, const char *svalue);     /* set the string associated with key */



  /*
        int delconf(const char *configfile, const char *key)

        description:
                deletes an entry from a section in a config file
        arguments:
                configfile: path of config file
                key: section/key to delete
        return values:
                returns 0 for OK or error code defined in conf.h
  */
  int delconf(const char *configfile, const char *key);                         /* delete an entry from section in config file */



  /*
        int getconf_long(const char *configfile, const char *key,
                         long *lvalue)

        description:
                gets the long value associated with a key in a config file
        arguments:
                configfile: path of config file
                key: section/key to look for
        return values:
                returns 0 for OK or error code defined in conf.h
                lvalue: the long integer associated with key in section
  */
  int getconf_long(const char *configfile, const char *key,
                   long *lvalue);                                               /* get the long value associated with key */



  /*
        int setconf_long(const char *configfile, const char *key, long lvalue)

        description:
                sets the long value associated with a key in a config file
        arguments:
                configfile: path of config file
                key: section/key to set
                lvalue: the long integer associated with key in section
        return values:
                returns 0 for OK or error code defined in conf.h
  */
  int setconf_long(const char *configfile, const char *key, long lvalue);       /* set the long value associated with key */



  /*
        int getconf_boolean(const char *configfile, const char *key,
                            int *bvalue)

        description:
                gets the boolean value associated with a key in a config file
        arguments:
                configfile: path of config file
                key: section/key to look for
        return values:
                returns 0 for OK or error code defined in conf.h
                bvalue: boolean value associated with key (ON = 1, OFF = 0)
  */
  int getconf_boolean(const char *configfile, const char *key, int *bvalue);    /* get the boolean value associated with key */



  /*
        int setconf_boolean(const char *configfile, const char *key, int bvalue)

        description:
               sets the boolean value associated with a key in a config file
        arguments:
               configfile: path of config file
               key: section/key to set
               bvalue: boolean value associated with key (on = 1, off = 0)
        return values:
                returns 0 for OK or error code defined in conf.h
  */
  int setconf_boolean(const char *configfile, const char *key, int bvalue);     /* set the boolean value associated with key */



  /*
        const char *getconf_error(int error)

        description:
                returns string error message associated with error number
        arguments:
                error: error number returned by [gs]etconf* functions
        return value:
                returns pointer to a static error message string
  */
  const char *getconf_error(int error);                                         /* get error string associated with error number */




  /*
        char *trim(char *s)

        description:
                removes leading and trailing whitespace from a string
                whitespace is any character in literal string WHITESPACE
        arguments:
                s: string to be modified
        return value:
                returns s
  */
  char *trim(char *s);



  /*
        char *endtrim(char *s)

        description:
                removes just trailing whitespace from a string
                whitespace is any character in literal string WHITESPACE
        arguments:
                s: string to be modified
        return value:
                returns s
  */
  char *endtrim(char *s);



  /*
        int *strcnt(const char *s, char c)

        description:
                counts the number of instances of char c in string s
        arguments:
                s: string in which to look for c
                c: character to look for in s
        return value:
                returns the number of instances of c in s
  */
  int strcnt(const char *s, char c);



  /*
        long conf_atol(const char *decstr)

        description:
                this function coverts the given decimal string to a long int
                on illegally formatted input, returns zero
        arguments:
                decstr: decimal string to be converted
        return value:
                returns the long integer represented by the decimal string
  */
  long conf_atol(const char *decstr);



  /*
        long conf_strcascmp(const char *s1, const char *s2)

        description:
                this function compares the strings s1 and s2 ignoring case
        arguments:
		s1: string 1
		s2: string 2
        return value:
                returns 0 if the strings are equal, a negative integer if
		s1 < s2, or a positive interger if s1 > s2.
  */
  int conf_strcasecmp(const char *s1, const char *s2);
  

  const char *level_indent(int l);                                       /* returns right amount of leading whitespace */


#  ifdef __cplusplus
}
#  endif

#endif /* !CONF_H */

/*
 * End of "$Id: conf.h,v 1.9 1999/10/26 16:26:23 mike Exp $".
 */
