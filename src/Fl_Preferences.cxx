/
// "$Id$
/
// Preferences methods for the Fast Light Tool Kit (FLTK)
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


#include <FL/Fl.H
#include <FL/Fl_Preferences.H
#include <FL/filename.H

#include <stdio.h
#include <stdlib.h
#include <stdarg.h
#include "flstring.h
#include <sys/stat.h

#if defined(WIN32) && !defined(__CYGWIN__
#  include <direct.h
#  include <io.h
#elif defined (__APPLE__
#  include <Carbon/Carbon.H
#  include <unistd.h
#els
#  include <unistd.h
#endi


char Fl_Preferences::nameBuffer[128]


/
 * create the initial preferences bas
 * - root: machine or user preference
 * - vendor: unique identification of author or vendor of applicatio
 *     Must be a valid directory name
 * - application: vendor unique application name, i.e. "PreferencesTest
 *     multiple preferences files can be created per application
 *     Must be a valid file name
 * example: Fl_Preferences base( Fl_Preferences::USER, "fltk.org", "test01")
 *
Fl_Preferences::Fl_Preferences( Root root, const char *vendor, const char *application 

  node = new Node( "." )
  rootNode = new RootNode( this, root, vendor, application )



/
 * create the initial preferences bas
 * \arg \a path: an application-supplied pat
 * \example Fl_Preferences base( "/usr/foo" )
 *
Fl_Preferences::Fl_Preferences( const char *path, const char *vendor, const char *application 

  node = new Node( "." )
  rootNode = new RootNode( this, path, vendor, application )



/
 * create a Preferences node in relation to a parent node for reading and writin
 * - parent: base name for grou
 * - group: group name (can contain '/' seperated group names
 * example: Fl_Preferences colors( base, "setup/colors" )
 *
Fl_Preferences::Fl_Preferences( Fl_Preferences &parent, const char *key 

  rootNode = 0
  node = parent.node->addChild( key )



/
 * create a Preferences node in relation to a parent node for reading and writin
 * \arg \a parent: base name for grou
 * \arg \a group: group name (can contain '/' seperated group names
 * \example Fl_Preferences colors( base, "setup/colors" )
 *
Fl_Preferences::Fl_Preferences( Fl_Preferences *parent, const char *key 

  rootNode = 0
  node = parent->node->addChild( key )



/
 * destroy individual key
 * - destroying the base preferences will flush changes to the prefs fil
 * - after destroying the base, none of the depending preferences must be read or writte
 *
Fl_Preferences::~Fl_Preferences(

  delete rootNode
  // DO NOT delete nodes! The root node will do that after writing the preference



/
 * return the number of groups that are contained within a grou
 * example: int n = base.groups()
 *
int Fl_Preferences::groups(

  return node->nChildren()



/
 * return the group name of the n'th grou
 * - there is no guaranteed order of group name
 * - the index must be within the range given by groups(
 * example: printf( "Group(%d)='%s'\n", ix, base.group(ix) )
 *
const char *Fl_Preferences::group( int ix 

  return node->child( ix )



/
 * return 1, if a group with this name exist
 * example: if ( base.groupExists( "setup/colors" ) ) ..
 *
char Fl_Preferences::groupExists( const char *key 

  return node->search( key ) ? 1 : 0 



/
 * delete a grou
 * example: setup.deleteGroup( "colors/buttons" )
 *
char Fl_Preferences::deleteGroup( const char *key 

  Node *nd = node->search( key )
  if ( nd ) return nd->remove()
  return 0



/
 * return the number of entries (name/value) pairs for a grou
 * example: int n = buttonColor.entries()
 *
int Fl_Preferences::entries(

  return node->nEntry



/
 * return the name of an entr
 * - there is no guaranteed order of entry name
 * - the index must be within the range given by entries(
 * example: printf( "Entry(%d)='%s'\n", ix, buttonColor.entry(ix) )
 *
const char *Fl_Preferences::entry( int ix 

  return node->entry[ix].name



/
 * return 1, if an entry with this name exist
 * example: if ( buttonColor.entryExists( "red" ) ) ..
 *
char Fl_Preferences::entryExists( const char *key 

  return node->getEntry( key )>=0 ? 1 : 0 



/
 * remove a single entry (name/value pair
 * example: buttonColor.deleteEntry( "red" )
 *
char Fl_Preferences::deleteEntry( const char *key 

  return node->deleteEntry( key )



/
 * read an entry from the grou
 *
char Fl_Preferences::get( const char *key, int &value, int defaultValue 

  const char *v = node->get( key )
  value = v ? atoi( v ) : defaultValue
  return ( v != 0 )



/
 * set an entry (name/value pair
 *
char Fl_Preferences::set( const char *key, int value 

  sprintf( nameBuffer, "%d", value )
  node->set( key, nameBuffer )
  return 1



/
 * read an entry from the grou
 *
char Fl_Preferences::get( const char *key, float &value, float defaultValue 

  const char *v = node->get( key )
  value = v ? (float)atof( v ) : defaultValue
  return ( v != 0 )



/
 * set an entry (name/value pair
 *
char Fl_Preferences::set( const char *key, float value 

  sprintf( nameBuffer, "%g", value )
  node->set( key, nameBuffer )
  return 1



/
 * read an entry from the grou
 *
char Fl_Preferences::get( const char *key, double &value, double defaultValue 

  const char *v = node->get( key )
  value = v ? atof( v ) : defaultValue
  return ( v != 0 )



/
 * set an entry (name/value pair
 *
char Fl_Preferences::set( const char *key, double value 

  sprintf( nameBuffer, "%g", value )
  node->set( key, nameBuffer )
  return 1



// remove control sequences from a strin
static char *decodeText( const char *src 

  int len = 0
  const char *s = src
  for ( ; *s; s++, len++ 
  
    if ( *s == '\\' 
      if ( isdigit( s[1] ) ) s+=3; else s+=1
  
  char *dst = (char*)malloc( len+1 ), *d = dst
  for ( s = src; *s; s++ 
  
    char c = *s
    if ( c == '\\' 
    
      if ( s[1] == '\\' ) { *d++ = c; s++; 
      else if ( s[1] == 'n' ) { *d++ = '\n'; s++; 
      else if ( s[1] == 'r' ) { *d++ = '\r'; s++; 
      else if ( isdigit( s[1] ) ) { *d++ = ((s[1]-'0')<<6) + ((s[2]-'0')<<3) + (s[3]-'0'); s+=3; 
      else s++; // erro
    
    els
      *d++ = c
  
  *d = 0
  return dst



/
 * read a text entry from the grou
 * the text will be moved into the given text buffe
 * text will be clipped to the buffer siz
 *
char Fl_Preferences::get( const char *key, char *text, const char *defaultValue, int maxSize 

  const char *v = node->get( key )
  if ( v && strchr( v, '\\' ) ) 
    char *w = decodeText( v )
    strlcpy(text, w, maxSize)
    free( w )
    return 1
  
  if ( !v ) v = defaultValue
  if ( v ) strlcpy(text, v, maxSize)
  else text = 0
  return ( v != defaultValue )



/
 * read a text entry from the grou
 * 'text' will be changed to point to a new text buffe
 * the text buffer must be deleted with 'free(text)' by the user
 *
char Fl_Preferences::get( const char *key, char *&text, const char *defaultValue 

  const char *v = node->get( key )
  if ( v && strchr( v, '\\' ) 
  
    text = decodeText( v )
    return 1
  }   
  if ( !v ) v = defaultValue
  if ( v 
    text = strdup( v )
  els
    text = 0
  return ( v != defaultValue )



/
 * set an entry (name/value pair
 *
char Fl_Preferences::set( const char *key, const char *text 

  const char *s = text
  int n=0, ns=0
  for ( ; *s; s++ ) { n++; if ( *s<32 || *s=='\\' || *s==0x7f ) ns+=4; 
  if ( ns 
  
    char *buffer = (char*)malloc( n+ns+1 ), *d = buffer
    for ( s=text; *s; )
    {
      char c = *s
      if ( c=='\\' ) { *d++ = '\\'; *d++ = '\\'; s++; 
      else if ( c=='\n' ) { *d++ = '\\'; *d++ = 'n'; s++; 
      else if ( c=='\r' ) { *d++ = '\\'; *d++ = 'r'; s++; 
      else if ( c<32 || c==0x7f )
	{ *d++ = '\\'; *d++ = '0'+((c>>6)&3); *d++ = '0'+((c>>3)&7); *d++ = '0'+(c&7);  s++; 
      else *d++ = *s++
    
    *d = 0
    node->set( key, buffer )
    free( buffer )
  
  els
    node->set( key, text )
  return 1



// convert a hex string to binary dat
static void *decodeHex( const char *src, int &size 

  size = strlen( src )/2
  unsigned char *data = (unsigned char*)malloc( size ), *d = data
  const char *s = src
  int i

  for ( i=size; i>0; i-- 
  
    int v
    char x = tolower(*s++)
    if ( x >= 'a' ) v = x-'a'+10; else v = x-'0'
    v = v<<4
    x = tolower(*s++)
    if ( x >= 'a' ) v += x-'a'+10; else v += x-'0'
    *d++ = (uchar)v
  

  return (void*)data



/
 * read a binary entry from the grou
 * the data will be moved into the given destination buffe
 * data will be clipped to the buffer siz
 *
char Fl_Preferences::get( const char *key, void *data, const void *defaultValue, int defaultSize, int maxSize 

  const char *v = node->get( key )
  if ( v 
  
    int dsize
    void *w = decodeHex( v, dsize )
    memmove( data, w, dsize>maxSize?maxSize:dsize )
    free( w )
    return 1
  }   
  if ( defaultValue 
    memmove( data, defaultValue, defaultSize>maxSize?maxSize:defaultSize )
  return 0



/
 * read a binary entry from the grou
 * 'data' will be changed to point to a new data buffe
 * the data buffer must be deleted with 'free(data)' by the user
 *
char Fl_Preferences::get( const char *key, void *&data, const void *defaultValue, int defaultSize 

  const char *v = node->get( key )
  if ( v 
  
    int dsize
    data = decodeHex( v, dsize )
    return 1
  }   
  if ( defaultValue 
  
    data = (void*)malloc( defaultSize )
    memmove( data, defaultValue, defaultSize )
  
  els
    data = 0
  return 0



/
 * set an entry (name/value pair
 *
char Fl_Preferences::set( const char *key, const void *data, int dsize 

  char *buffer = (char*)malloc( dsize*2+1 ), *d = buffer;
  unsigned char *s = (unsigned char*)data
  for ( ; dsize>0; dsize-- 
  
    static char lu[] = "0123456789abcdef"
    unsigned char v = *s++
    *d++ = lu[v>>4]
    *d++ = lu[v&0xf]
  
  *d = 0
  node->set( key, buffer )
  free( buffer )
  return 1



/
 * return the size of the value part of an entr
 *
int Fl_Preferences::size( const char *key 

  const char *v = node->get( key )
  return v ? strlen( v ) : 0 


/
 * creates a path that is related to the preferences fil
 * and that is usable for application data beyond what is covered
 * by Fl_Preferences
 * - 'getUserdataPath' actually creates the director
 * - 'path' must be large enough to receive a complete file pat
 * example
 *   Fl_Preferences prefs( USER, "matthiasm.com", "test" )
 *   char path[FL_PATH_MAX]
 *   prefs.getUserdataPath( path )
 * sample returns
 *   Win32: c:/Documents and Settings/matt/Application Data/matthiasm.com/test
 *   prefs: c:/Documents and Settings/matt/Application Data/matthiasm.com/test.pref
 *
char Fl_Preferences::getUserdataPath( char *path, int pathlen 

  if ( rootNode 
    return rootNode->getPath( path, pathlen )
  return 0


/
 * write all preferences to dis
 * - this function works only with the base preference grou
 * - this function is rarely used as deleting the base preferences flushes automaticall
 *
void Fl_Preferences::flush(

  if ( rootNode && node->dirty() 
    rootNode->write()


//----------------------------------------------------------------------------
// helper class to create dynamic group and entry names on the fl
/

/
 * create a group name or entry name on the fl
 * - this version creates a simple unsigned integer as an entry nam
 * example
 *   int n, i
 *   Fl_Preferences prev( appPrefs, "PreviousFiles" )
 *   prev.get( "n", 0 )
 *   for ( i=0; i<n; i++ 
 *     prev.get( Fl_Preferences::Name(i), prevFile[i], "" )
 *
Fl_Preferences::Name::Name( unsigned int n 

  data_ = (char*)malloc(20)
  sprintf(data_, "%u", n)


/
 * create a group name or entry name on the fl
 * - this version creates entry names as in 'printf
 * example
 *   int n, i
 *   Fl_Preferences prefs( USER, "matthiasm.com", "test" )
 *   prev.get( "nFiles", 0 )
 *   for ( i=0; i<n; i++ 
 *     prev.get( Fl_Preferences::Name( "File%d", i ), prevFile[i], "" )
 *
Fl_Preferences::Name::Name( const char *format, ... 

  data_ = (char*)malloc(1024)
  va_list args
  va_start(args, format)
  vsnprintf(data_, 1024, format, args)
  va_end(args)


// delete the nam
Fl_Preferences::Name::~Name(

  free(data_)


//----------------------------------------------------------------------------
// internal methods, do not modify or use as they will change without notic
/

int Fl_Preferences::Node::lastEntrySet = -1

// recursively create a path in the file syste
static char makePath( const char *path ) 
  if (access(path, 0)) 
    const char *s = strrchr( path, '/' )
    if ( !s ) return 0
    int len = s-path
    char *p = (char*)malloc( len+1 )
    memcpy( p, path, len )
    p[len] = 0
    makePath( p )
    free( p )
#if defined(WIN32) && !defined(__CYGWIN__
    return ( mkdir( path ) == 0 )
#els
    return ( mkdir( path, 0777 ) == 0 )
#endif // WIN32 && !__CYGWIN_
  
  return 1


// strip the filename and create a pat
static void makePathForFile( const char *path 

  const char *s = strrchr( path, '/' )
  if ( !s ) return
  int len = s-path
  char *p = (char*)malloc( len+1 )
  memcpy( p, path, len )
  p[len] = 0
  makePath( p )
  free( p )


// create the root nod
// - construct the name of the file that will hold our preference
Fl_Preferences::RootNode::RootNode( Fl_Preferences *prefs, Root root, const char *vendor, const char *application 

  char filename[ FL_PATH_MAX ]; filename[0] = 0
#ifdef WIN3
#  define FLPREFS_RESOURCE	"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders
  int appDataLen = strlen(vendor) + strlen(application) + 8
  DWORD type, nn
  LONG err
  HKEY key

  switch (root) 
    case SYSTEM
      err = RegOpenKey( HKEY_LOCAL_MACHINE, FLPREFS_RESOURCE, &key )
      if (err == ERROR_SUCCESS) 
	nn = FL_PATH_MAX - appDataLen
	err = RegQueryValueEx( key, "Common AppData", 0L, &type, (BYTE*)filename, &nn )
	if ( ( err != ERROR_SUCCESS ) && ( type == REG_SZ ) 
	  filename[0] = 0
        RegCloseKey(key)
      
      break
    case USER
      err = RegOpenKey( HKEY_CURRENT_USER, FLPREFS_RESOURCE, &key )
      if (err == ERROR_SUCCESS) 
	nn = FL_PATH_MAX - appDataLen
	err = RegQueryValueEx( key, "AppData", 0L, &type, (BYTE*)filename, &nn )
	if ( ( err != ERROR_SUCCESS ) && ( type == REG_SZ ) 
	  filename[0] = 0
        RegCloseKey(key)
      
      break
  

  if (!filename[0]) 
    strcpy(filename, "C:\\FLTK")
  

  snprintf(filename + strlen(filename), sizeof(filename) - strlen(filename)
           "/%s/%s.prefs", vendor, application)
  for (char *s = filename; *s; s++) if (*s == '\\') *s = '/'
#elif defined ( __APPLE__ 
  FSSpec spec = { 0 }
  FSRef ref
  OSErr err = fnfErr
  switch (root) 
    case SYSTEM
      err = FindFolder( kLocalDomain, kPreferencesFolderType
			1, &spec.vRefNum, &spec.parID )
      break
    case USER
      err = FindFolder( kUserDomain, kPreferencesFolderType,
			1, &spec.vRefNum, &spec.parID )
      break
  
  FSpMakeFSRef( &spec, &ref )
  FSRefMakePath( &ref, (UInt8*)filename, FL_PATH_MAX )
  snprintf(filename + strlen(filename), sizeof(filename) - strlen(filename)
           "/%s/%s.prefs", vendor, application )
#els
  const char *e
  switch (root) 
    case USER
      if ((e = getenv("HOME")) != NULL) 
	strlcpy(filename, e, sizeof(filename))

	if (filename[strlen(filename)-1] != '/') 
	  strlcat(filename, "/.fltk/", sizeof(filename))
	} else 
	  strlcat(filename, ".fltk/", sizeof(filename))
	
	break
      

    case SYSTEM
      strcpy(filename, "/etc/fltk/")
      break
  

  snprintf(filename + strlen(filename), sizeof(filename) - strlen(filename)
           "%s/%s.prefs", vendor, application)
#endi

  prefs_       = prefs
  filename_    = strdup(filename)
  vendor_      = strdup(vendor)
  application_ = strdup(application)

  read()


// create the root nod
// - construct the name of the file that will hold our preference
Fl_Preferences::RootNode::RootNode( Fl_Preferences *prefs, const char *path, const char *vendor, const char *application 

  char filename[ FL_PATH_MAX ]; filename[0] = 0

  snprintf(filename, sizeof(filename), "%s/%s.prefs", path, application)

  prefs_       = prefs
  filename_    = strdup(filename)
  vendor_      = strdup(vendor)
  application_ = strdup(application)

  read()


// destroy the root node and all depending node
Fl_Preferences::RootNode::~RootNode(

  if ( prefs_->node->dirty() 
    write()
  if ( filename_ )
    free( filename_ )
  if ( vendor_ 
    free( vendor_ )
  if ( application_ 
    free( application_ )
  delete prefs_->node


// read a preferences file and construct the group tree and with all entry leaf
int Fl_Preferences::RootNode::read(

  char buf[1024]
  FILE *f = fopen( filename_, "rb" )
  if ( !f ) return 0
  fgets( buf, 1024, f )
  fgets( buf, 1024, f )
  fgets( buf, 1024, f )
  Node *nd = prefs_->node
  for (;;
  
    if ( !fgets( buf, 1024, f ) ) break;	// EOF or Erro
    if ( buf[0]=='[' ) // read a new grou
    
      int end = strcspn( buf+1, "]\n\r" )
      buf[ end+1 ] = 0
      nd = prefs_->node->find( buf+1 )
    
    else if ( buf[0]=='+' ) //
    { // value of previous name/value pair spans multiple line
      int end = strcspn( buf+1, "\n\r" )
      if ( end != 0 ) // if entry is not empt
      
	buf[ end+1 ] = 0
	nd->add( buf+1 )
      
    
    else // read a name/value pai
    
      int end = strcspn( buf, "\n\r" )
      if ( end != 0 ) // if entry is not empt
      
	buf[ end ] = 0
	nd->set( buf )
      
    
  
  fclose( f )
  return 0


// write the group tree and all entry leaf
int Fl_Preferences::RootNode::write(

  makePathForFile(filename_)
  FILE *f = fopen( filename_, "wb" )
  if ( !f ) return 1
  fprintf( f, "; FLTK preferences file format 1.0\n" )
  fprintf( f, "; vendor: %s\n", vendor_ )
  fprintf( f, "; application: %s\n", application_ )
  prefs_->node->write( f )
  fclose( f )
  return 0


// get the path to the preferences director
char Fl_Preferences::RootNode::getPath( char *path, int pathlen 

  strlcpy( path, filename_, pathlen)

  char *s
  for ( s = path; *s; s++ ) if ( *s == '\\' ) *s = '/'
  s = strrchr( path, '.' )
  if ( !s ) return 0
  *s = 0
  char ret = makePath( path )
  strcpy( s, "/" )
  return ret


// create a node that represents a grou
// - path must be a single word, prferable alnum(), dot and underscore only. Space is ok
Fl_Preferences::Node::Node( const char *path 

  if ( path ) path_ = strdup( path ); else path_ = 0
  child_ = 0; next_ = 0; parent_ = 0
  entry = 0
  nEntry = NEntry = 0
  dirty_ = 0


// delete this and all depending node
Fl_Preferences::Node::~Node(

  Node *nx
  for ( Node *nd = child_; nd; nd = nx 
  
    nx = nd->next_
    delete nd
  
  if ( entry 
  
    for ( int i = 0; i < nEntry; i++ 
    
      if ( entry[i].name )
	free( entry[i].name )
      if ( entry[i].value 
	free( entry[i].value )
    
    free( entry )
  
  if ( path_ )
    free( path_ )


// recursively check if any entry is dirty (was changed after loading a fresh prefs file
char Fl_Preferences::Node::dirty(

  if ( dirty_ ) return 1
  if ( next_ && next_->dirty() ) return 1
  if ( child_ && child_->dirty() ) return 1
  return 0


// write this node (recursively from the last neighbor back to this
// write all entrie
// write all childre
int Fl_Preferences::Node::write( FILE *f 

  if ( next_ ) next_->write( f )
  fprintf( f, "\n[%s]\n\n", path_ )
  for ( int i = 0; i < nEntry; i++ 
  
    char *src = entry[i].value
    if ( src 
    { // hack it into smaller pieces if neede
      fprintf( f, "%s:", entry[i].name )
      int cnt
      for ( cnt = 0; cnt < 60; cnt++ 
	if ( src[cnt]==0 ) break
      fwrite( src, cnt, 1, f )
      fprintf( f, "\n" )
      src += cnt
      for (;*src;
      
	for ( cnt = 0; cnt < 80; cnt++ 
	  if ( src[cnt]==0 ) break
        fputc( '+', f )
	fwrite( src, cnt, 1, f )
        fputc( '\n', f )
	src += cnt
      
    
    els
      fprintf( f, "%s\n", entry[i].name )
  
  if ( child_ ) child_->write( f )
  dirty_ = 0
  return 0


// set the parent node and create the full pat
void Fl_Preferences::Node::setParent( Node *pn 

  parent_ = pn
  next_ = pn->child_
  pn->child_ = this
  sprintf( nameBuffer, "%s/%s", pn->path_, path_ )
  free( path_ )
  path_ = strdup( nameBuffer )


// add a child to this node and set its path (try to find it first...
Fl_Preferences::Node *Fl_Preferences::Node::addChild( const char *path 

  sprintf( nameBuffer, "%s/%s", path_, path )
  char *name = strdup( nameBuffer )
  Node *nd = find( nameBuffer )
  free( name )
  dirty_ = 1
  return nd


// create and set, or change an entry within this nod
void Fl_Preferences::Node::set( const char *name, const char *value 

  for ( int i=0; i<nEntry; i++ 
  
    if ( strcmp( name, entry[i].name ) == 0 
    
      if ( !value ) return; // annotatio
      if ( strcmp( value, entry[i].value ) != 0 
      
	if ( entry[i].value 
	  free( entry[i].value )
	entry[i].value = strdup( value )
	dirty_ = 1
      
      lastEntrySet = i
      return
    
  
  if ( NEntry==nEntry 
  
    NEntry = NEntry ? NEntry*2 : 10
    entry = (Entry*)realloc( entry, NEntry * sizeof(Entry) )
  
  entry[ nEntry ].name = strdup( name )
  entry[ nEntry ].value = value?strdup( value ):0
  lastEntrySet = nEntry
  nEntry++
  dirty_ = 1


// create or set a value (or annotation) from a single line in the file buffe
void Fl_Preferences::Node::set( const char *line 

  // hmm. If we assume that we always read this file in the beginning
  // we can handle the dirty flag 'quick and dirty
  char dirt = dirty_
  if ( line[0]==';' || line[0]==0 || line[0]=='#' 
  
    set( line, 0 )
  
  els
  
    const char *c = strchr( line, ':' )
    if ( c 
    
      strlcpy( nameBuffer, line, c-line+1)
      set( nameBuffer, c+1 )
    
    els
      set( line, "" )
  
  dirty_ = dirt


// add more data to an existing entr
void Fl_Preferences::Node::add( const char *line 

  if ( lastEntrySet<0 || lastEntrySet>=nEntry ) return
  char *&dst = entry[ lastEntrySet ].value
  int a = strlen( dst )
  int b = strlen( line )
  dst = (char*)realloc( dst, a+b+1 )
  memcpy( dst+a, line, b+1 )
  dirty_ = 1


// get the value for a name, returns 0 if no such nam
const char *Fl_Preferences::Node::get( const char *name 

  int i = getEntry( name )
  return i>=0 ? entry[i].value : 0 


// find the index of an entry, returns -1 if no such entr
int Fl_Preferences::Node::getEntry( const char *name 

  for ( int i=0; i<nEntry; i++ 
  
    if ( strcmp( name, entry[i].name ) == 0 
    
      return i
    
  
  return -1


// remove one entry form this grou
char Fl_Preferences::Node::deleteEntry( const char *name 

  int ix = getEntry( name )
  if ( ix == -1 ) return 0
  memmove( entry+ix, entry+ix+1, (nEntry-ix-1) * sizeof(Entry) )
  nEntry--
  dirty_ = 1
  return 1


// find a group somewhere in the tree starting her
// - this method will always return a valid node (except for memory allocation problems
// - if the node was not found, 'find' will create the required branc
Fl_Preferences::Node *Fl_Preferences::Node::find( const char *path 

  int len = strlen( path_ )
  if ( strncmp( path, path_, len ) == 0 
  
    if ( path[ len ] == 0 )
      return this
    if ( path[ len ] == '/' 
    
      Node *nd
      for ( nd = child_; nd; nd = nd->next_ 
      
	Node *nn = nd->find( path )
	if ( nn ) return nn
      
      const char *s = path+len+1
      const char *e = strchr( s, '/' )
      if (e) strlcpy( nameBuffer, s, e-s+1 )
      else strlcpy( nameBuffer, s, sizeof(nameBuffer))
      nd = new Node( nameBuffer )
      nd->setParent( this )
      return nd->find( path )
    
  
  return 0


// find a group somewhere in the tree starting her
// caller must not set 'offset' argumen
// - if the node does not exist, 'search' returns NUL
// - if the pathname is "." (current node) return this nod
// - if the pathname is "./" (root node) return the topmost nod
// - if the pathname starts with "./", start the search at the root node instea
Fl_Preferences::Node *Fl_Preferences::Node::search( const char *path, int offset 


  if ( offset == 0 
  
    if ( path[0] == '.' 
    
      if ( path[1] == 0 
      
	return this; // user was searching for current nod
      
      else if ( path[1] == '/' 
      
	Node *nn = this
	while ( nn->parent_ ) nn = nn->parent_
	if ( path[2]==0 
	{ // user is searching for root ( "./" 
	  return nn
	
	return nn->search( path+2, 2 ); // do a relative search on the root nod
      
    
    offset = strlen( path_ ) + 1
  
 
  int len = strlen( path_ )
  if ( len < offset-1 ) return 0
  len -= offset
  if ( ( len <= 0 ) || ( strncmp( path, path_+offset, len ) == 0 ) 
  
    if ( len > 0 && path[ len ] == 0 )
      return this
    if ( len <= 0 || path[ len ] == '/' 
    
      for ( Node *nd = child_; nd; nd = nd->next_ 
      
	Node *nn = nd->search( path, offset )
	if ( nn ) return nn
      
      return 0
    
  
  return 0


// return the number of child nodes (groups
int Fl_Preferences::Node::nChildren(

  int cnt = 0
  for ( Node *nd = child_; nd; nd = nd->next_ 
    cnt++
  return cnt


// return the n'th child nod
const char *Fl_Preferences::Node::child( int ix 

  Node *nd
  for ( nd = child_; nd; nd = nd->next_ 
  
    if ( !ix-- ) break
  
  if ( nd && nd->path_ 
  
    char *r = strrchr( nd->path_, '/' )
    return r ? r+1 : nd->path_ 
  
  return 0L 


// remove myself from the list and delete me (and all children
char Fl_Preferences::Node::remove(

  Node *nd = 0, *np
  if ( parent_ 
  
    nd = parent_->child_; np = 0L
    for ( ; nd; np = nd, nd = nd->next_ 
    
      if ( nd == this 
      
	if ( np )
	  np->next_ = nd->next_;
	else
	  parent_->child_ = nd->next_
	break
      
    
  
  delete this
  dirty_ = 1
  return ( nd != 0 )



/
// End of "$Id$"
/
