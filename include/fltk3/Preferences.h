//
// "$Id: Preferences.h 7949 2010-12-05 00:38:16Z greg.ercolano $"
//
// Preferences .
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
 fltk3::Preferences class . */

#ifndef Fltk3_Preferences_H
#  define Fltk3_Preferences_H

#  include <stdio.h>
#  include "Export.h"

namespace fltk3 {

  /**
   \brief fltk3::Preferences provides methods to store user
   settings between application starts.

   It is similar to the
   Registry on WIN32 and Preferences on MacOS, and provides a
   simple configuration mechanism for UNIX.

   fltk3::Preferences uses a hierarchy to store data. It
   bundles similar data into groups and manages entries into those
   groups as name/value pairs.

   Preferences are stored in text files that can be edited
   manually. The file format is easy to read and relatively
   forgiving. Preferences files are the same on all platforms. User
   comments in preference files are preserved. Filenames are unique
   for each application by using a vendor/application naming
   scheme. The user must provide default values for all entries to
   ensure proper operation should preferences be corrupted or not
   yet exist.

   Entries can be of any length. However, the size of each
   preferences file should be kept small for performance
   reasons. One application can have multiple preferences files.
   Extensive binary data however should be stored in separate
   files: see getUserdataPath().

   \note Starting with FLTK 1.3, preference databases are expected to
   be in UTF-8 encoding. Previous databases were stored in the
   current chracter set or code page which renders them incompatible
   for text entries using international characters.
   */
  class FLTK3_EXPORT Preferences {

  public:
    /**
     Define the scope of the preferences.
     */
    enum Root {
      SYSTEM=0,   ///< Preferences are used system-wide
      USER        ///< Preferences apply only to the current user
    };

    /**
     Every fltk3::Preferences-Group has a uniqe ID.

     ID's can be retrieved from an fltk3::Preferences-Group and can then be used
     to create more fltk3::Preferences references to the same data set, as long as the
     database remains open.
     */
    typedef void *ID;

    static const char *newUUID();

    Preferences( Root root, const char *vendor, const char *application );
    Preferences( const char *path, const char *vendor, const char *application );
    Preferences( Preferences &parent, const char *group );
    Preferences( Preferences *parent, const char *group );
    Preferences( Preferences &parent, int groupIndex );
    Preferences( Preferences *parent, int groupIndex );
    Preferences(const Preferences&);
    Preferences( ID id );
    virtual ~Preferences();

    /** Return an ID that can later be reused to open more references to this dataset.
     */
    ID id() { return (ID)node; }

    /** Remove the group with this ID from a database.
     */
    static char remove(ID id_) { return ((Node*)id_)->remove(); }

    /** Return the name of this entry.
     */
    const char *name() { return node->name(); }

    /** Return the full path to this entry.
     */
    const char *path() { return node->path(); }

    int groups();
    const char *group( int num_group );
    char groupExists( const char *key );
    char deleteGroup( const char *group );
    char deleteAllGroups();

    int entries();
    const char *entry( int index );
    char entryExists( const char *key );
    char deleteEntry( const char *entry );
    char deleteAllEntries();

    char clear();

    char set( const char *entry, int value );
    char set( const char *entry, float value );
    char set( const char *entry, float value, int precision );
    char set( const char *entry, double value );
    char set( const char *entry, double value, int precision );
    char set( const char *entry, const char *value );
    char set( const char *entry, const void *value, int size );

    char get( const char *entry, int &value, int defaultValue );
    char get( const char *entry, float &value,  float defaultValue );
    char get( const char *entry, double &value, double defaultValue );
    char get( const char *entry, char *&value,  const char *defaultValue );
    char get( const char *entry, char *value,   const char *defaultValue, int maxSize );
    char get( const char *entry, void *&value,  const void *defaultValue, int defaultSize );
    char get( const char *entry, void *value,   const void *defaultValue, int defaultSize, int maxSize );

    int size( const char *entry );

    char getUserdataPath( char *path, int pathlen );

    void flush();

    // char export( const char *filename, Type fileFormat );
    // char import( const char *filename );

    /**
     'Name' provides a simple method to create numerical or more complex
     procedural names for entries and groups on the fly.

     Example: prefs.set(fltk3::Preferences::Name("File%d",i),file[i]);.

     See test/preferences.cxx as a sample for writing arrays into preferences.

     'Name' is actually implemented as a class inside fltk3::Preferences. It casts
     into const char* and gets automatically destroyed after the enclosing call
     ends.
     */
    class FLTK3_EXPORT Name {

      char *data_;

    public:
      Name( unsigned int n );
      Name( const char *format, ... );

      /**
       Return the Name as a "C" string.
       \internal
       */
      operator const char *() { return data_; }
      ~Name();
    };

    /** \internal An entry associates a preference name to its corresponding value */
    struct Entry {
      char *name, *value;
    };

  private:
    Preferences() : node(0), rootNode(0) { }
    Preferences &operator=(const Preferences&);

    static char nameBuffer[128];
    static char uuidBuffer[40];
    static Preferences *runtimePrefs;

    class RootNode;

    class FLTK3_EXPORT Node {	// a node contains a list to all its entries
            			// and all means to manage the tree structure
      Node *child_, *next_;
      union { 			// these two are mutually exclusive
        Node *parent_;   		// top_ bit clear
        RootNode *root_; 		// top_ bit set
      };
      char *path_;
      Entry *entry_;
      int nEntry_, NEntry_;
      unsigned char dirty_:1;
      unsigned char top_:1;
      unsigned char indexed_:1;
      // indexing routines
      Node **index_;
      int nIndex_, NIndex_;
      void createIndex();
      void updateIndex();
      void deleteIndex();
    public:
      static int lastEntrySet;
    public:
      Node( const char *path );
      ~Node();
      // node methods
      int write( FILE *f );
      const char *name();
      const char *path() { return path_; }
      Node *find( const char *path );
      Node *search( const char *path, int offset=0 );
      Node *childNode( int ix );
      Node *addChild( const char *path );
      void setParent( Node *parent );
      Node *parent() { return top_?0L:parent_; }
      void setRoot(RootNode *r) { root_ = r; top_ = 1; }
      RootNode *findRoot();
      char remove();
      char dirty();
      void deleteAllChildren();
      // entry methods
      int nChildren();
      const char *child( int ix );
      void set( const char *name, const char *value );
      void set( const char *line );
      void add( const char *line );
      const char *get( const char *name );
      int getEntry( const char *name );
      char deleteEntry( const char *name );
      void deleteAllEntries();
      int nEntry() { return nEntry_; }
      Entry &entry(int i) { return entry_[i]; }
    };
    friend class Node;

    class FLTK3_EXPORT RootNode {		// the root node manages file paths and basic reading and writing
      Preferences *prefs_;
      char *filename_;
      char *vendor_, *application_;
    public:
      RootNode( Preferences *, Root root, const char *vendor, const char *application );
      RootNode( Preferences *, const char *path, const char *vendor, const char *application );
      RootNode( Preferences * );
      ~RootNode();
      int read();
      int write();
      char getPath( char *path, int pathlen );
    };
    friend class RootNode;

  protected:
    Node *node;
    RootNode *rootNode;
  };

}

#endif // !Fltk3_Preferences_H

//
// End of "$Id: Preferences.h 7949 2010-12-05 00:38:16Z greg.ercolano $".
//
