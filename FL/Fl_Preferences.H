/
// "$Id$
/
// Preferences definitions for the Fast Light Tool Kit (FLTK)
/
// Copyright 2002-2004 by Matthias Melcher
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

#ifndef Fl_Preferences_
#  define Fl_Preferences_

#  ifdef WIN3
#    include <windows.h
#  endif // WIN3

#  include <stdio.h
#  include "Fl_Export.H


/**
 * Fl_Preferences provides methods to store user setting between application starts
 * They are similar to the Registry on WIN32 and Preferences on MacOS, and provides 
 * simple configuration mechanism for UNIX
 
 * Fl_Preferences uses a hierarchy to store data. It bundles similar data into group
 * and manages entries into those groups as name/value pairs
 
 * Preferences are stored in text files that can be edited manually. The file forma
 * is easy to read and relatively forgiving. Preferences files are the same on al
 * platforms. User comments in preference files are preserved. Filenames are uniqu
 * for each application by using a vendor/application naming scheme. The user mus
 * provide default values for all entries to ensure proper operation should
 * preferences be corrupted or not yet exist
 
 * Entries can be of any length. However, the size of each preferences file should b
 * kept within reasonable size for performance reasons. One application can have
 * multiple preferences files. Extensive binary data however should be stored i
 * seperate files; see the getUserdataPath() method.
 *
class FL_EXPORT Fl_Preferences


public

  enum Root {
    SYSTEM=0, /**< System wide preferences */
    USER /**< User specific preferences */
  }
  // enum Type { win32, macos, fltk }

    /** The constructor creates a group that manages name/value pairs and child groups
     * Groups are ready for reading and writing at any time. The root argument is eithe
     * Fl_Preferences::USER or Fl_Preferences::SYSTEM
     
     * This constructor creates the base instance for all following entries and read
     * existing databases into memory. The vendor argument is a unique text strin
     * identifying the development team or vendor of an application. A domain name o
     * an EMail address are great unique names, e.g. "researchATmatthiasm.com" o
     * "fltk.org". The application argument can be the working title or final name o
     * your application. Both vendor and applicationmust be valid relative UNI
     * pathnames and may contain '/'s to create deeper file structures. *
  Fl_Preferences( Root root, const char *vendor, const char *application )
    /** Create the initial preferences base
     * \arg \a path: an application-supplied pat
     * \arg \a vendor: unique vendor string, e.g. a domain nam
     * \arg \a application: application name
     *
     *
     * Example:
     * \cod
     *  Fl_Preferences base( "/usr/foo" );
     * \endcod
     *
  Fl_Preferences( const char *path, const char *vendor, const char *application )
    /** This version of the constructor accesses a subgroup in relaton to the
     * parent group. The subgroup argument identifies a group of entries. It ca
     * contain '/'s to get quick access to individual elements inside the hierarchy. *
  Fl_Preferences( Fl_Preferences &parent, const char *subgroup )
  Fl_Preferences( Fl_Preferences *parent, const char *subgroup )
    /** The destructor removes allocated resources. When used on the base preference
     * group, the destructor flushes all changes to the preferences file and delete
     * all internal databases. *
  ~Fl_Preferences()
 
    /** Returns the number of groups that are contained within a group. *
  int groups()
    /** Returns the name of the Nth group. There is no guaranteed order of grou
     * names. The index must be within the range given by groups(). *
  const char *group( int index )
    /** Returns non-zero if a group with this name exists. Groupnames are relative t
     * the Preferences node and can contain a path. "." describes the current node
     * "./" describes the topmost node. By preceding a groupname with a "./", its pat
     * becomes relative to the topmost node. *
  char groupExists( const char *group )
    /** Deletes a group. *
  char deleteGroup( const char *group )

    /** Returns the number of entries (name/value) pairs in a group. *
  int entries()
    /** Returns the name of an entry. There is no guaranteed order of entry names. Th
     * index must be within the range given by entries(). *
  const char *entry( int index )
    /** Returns non-zero if an entry with this name exists. *
  char entryExists( const char *entry )
    /** Removes a single entry (name/value pair). *
  char deleteEntry( const char *entry )

    /** Sets an entry (name/value pair). The return value indicates if there was 
     * problem storing the data in memory. However it does not reflect if the valu
     * was actually stored in the preferences file. *
  char set( const char *entry, int value )
  char set( const char *entry, float value )
  char set( const char *entry, double value )
  char set( const char *entry, const char *value )
  char set( const char *entry, const void *value, int size );

    /** Reads an entry from the group. A default value must be supplied. The retur
     * value indicates if the value was available (non-zero) or the default wa
     * used (0). *
  char get( const char *entry, int &value,    int defaultValue )
  char get( const char *entry, float &value,  float defaultValue )
  char get( const char *entry, double &value, double defaultValue )
    /** Read a text entry from the group. 'text' will be changed to point to a ne
     * text buffer. The text buffer must be deleted with 'free(text)' by the user. *
  char get( const char *entry, char *&value,  const char *defaultValue )
    /** Read a text entry from the group. The text will be moved into the given tex
     * buffer. Text will be clipped to the buffer size. *
  char get( const char *entry, char *value,   const char *defaultValue, int maxSize )
    /** Read a binary entry from the group. 'data' will be changed to point to a ne
     * data buffer. The data buffer must be deleted with 'free(data)' by the user. *
  char get( const char *entry, void *&value,  const void *defaultValue, int defaultSize )
    /** Read a binary entry from the group. The data will be moved into the give
     * destination buffer. Data will be clipped to the buffer size. *
  char get( const char *entry, void *value,   const void *defaultValue, int defaultSize, int maxSize )
    /** Returns the size of the value part of an entry. *
  int size( const char *entry )

    /** Creates a path that is related to the preferences file and that is usable fo
     * application data beyond what is covered by Fl_Preferences
     * \arg 'getUserdataPath' actually creates the director
     * \arg 'path' must be large enough to receive a complete file pat
     
     * \cod
     Fl_Preferences prefs( USER, "matthiasm.com", "test" )
     char path[FL_PATH_MAX]
     prefs.getUserdataPath( path )
     * \endcod
     * \returns On Windows systems, this code creates
     * - data dir at c:/Documents and Settings/matt/Application Data/matthiasm.com/test
     * - preferences at c:/Documents and Settings/matt/Application Data/matthiasm.com/test.pref
     *
  char getUserdataPath( char *path, int pathlen )

    /** Write all preferences to disk. This function works only with the bas
     * preference group. This function is rarely used as deleting the base preference
     * flushes automatically. *
  void flush()

  // char export( const char *filename, Type fileFormat )
  // char import( const char *filename )

  /** Fl_Preferences::Name provides a simple method to create numerical or more
   * complex procedural names for entries and groups on the fly, e.g.
   * <tt>prefs.set(Fl_Preferences::Name("File%d",i),file[i]);</tt>.
   * See <tt>test/preferences.cxx</tt> as a sample for writing arrays into preferences
   *
   * Although 'Name' is implemented as a class inside Fl_Preferences, its standard us
   * is similar to an inlined method as described above. 'Name' casts into <tt>cons
   * char*</tt> and gets automatically destroyed after the enclosing call.  *
  class FL_EXPORT Name 
    char *data_
  public
    Name( unsigned int n )
    Name( const char *format, ... )
    operator const char *() { return data_; 
    ~Name()
  }

  struct Entr
  
    char *name, *value
  }

private

  static char nameBuffer[128]

  class FL_EXPORT Node // a node contains a list to all its entries
  {          // and all means to manage the tree structur
    Node *child_, *next_, *parent_
    char *path_
    char dirty_
  public
    Node( const char *path )
    ~Node()
    // node method
    int write( FILE *f )
    Node *find( const char *path )
    Node *search( const char *path, int offset=0 )
    Node *addChild( const char *path )
    void setParent( Node *parent )
    char remove()
    char dirty()
    // entry method
    int nChildren()
    const char *child( int ix )
    void set( const char *name, const char *value )
    void set( const char *line )
    void add( const char *line )
    const char *get( const char *name )
    int getEntry( const char *name )
    char deleteEntry( const char *name )
    // public value
    Entry *entry
    int nEntry, NEntry
    static int lastEntrySet
  }
  friend class Node

  class FL_EXPORT RootNode  // the root node manages file paths and basic reading and writin
  
    Fl_Preferences *prefs_
    char *filename_
    char *vendor_, *application_
  public
    RootNode( Fl_Preferences *, Root root, const char *vendor, const char *application )
    RootNode( Fl_Preferences *, const char *path, const char *vendor, const char *application )
    ~RootNode()
    int read()
    int write()
    char getPath( char *path, int pathlen )
  }
  friend class RootNode

  Node *node
  RootNode *rootNode
 
}


#endif // !Fl_Preferences_

/
// End of "$Id$"
/
