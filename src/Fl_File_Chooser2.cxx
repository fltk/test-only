/
// "$Id$
/
// More Fl_File_Chooser routines
/
// Copyright 1999-2004 by Michael Sweet
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
// Contents
/
//   Fl_File_Chooser::count()             - Return the number of selected files
//   Fl_File_Chooser::directory()         - Set the directory in the file chooser
//   Fl_File_Chooser::filter()            - Set the filter(s) for the chooser
//   Fl_File_Chooser::newdir()            - Make a new directory
//   Fl_File_Chooser::value()             - Return a selected filename
//   Fl_File_Chooser::rescan()            - Rescan the current directory
//   Fl_File_Chooser::favoritesButtonCB() - Handle favorites selections
//   Fl_File_Chooser::fileListCB()        - Handle clicks (and double-clicks
//                                          in the Fl_File_Browser
//   Fl_File_Chooser::fileNameCB()        - Handle text entry in the FileBrowser
//   Fl_File_Chooser::showChoiceCB()      - Handle show selections
//   quote_pathname()                     - Quote a pathname for a menu
//   unquote_pathname()                   - Unquote a pathname from a menu
/

/
// Include necessary headers
/

#include <FL/Fl_File_Chooser.H
#include <FL/filename.H
#include <FL/fl_ask.H
#include <FL/x.H
#include <FL/Fl_Shared_Image.H

#include <stdio.h
#include <stdlib.h
#include "flstring.h
#include <errno.h
#include <sys/types.h
#include <sys/stat.h

#if defined(WIN32) && ! defined (__CYGWIN__
#  include <direct.h
#  include <io.h
// Apparently Borland C++ defines DIRECTORY in <direct.h>, whic
// interfers with the Fl_File_Icon enumeration of the same name
#  ifdef DIRECTOR
#    undef DIRECTOR
#  endif // DIRECTOR
#els
#  include <unistd.h
#  include <pwd.h
#endif /* WIN32 *


/
// File chooser label strings and sort function..
/

Fl_Preferences	Fl_File_Chooser::prefs_(Fl_Preferences::USER, "fltk.org", "filechooser")

const char	*Fl_File_Chooser::add_favorites_label = "Add to Favorites"
const char	*Fl_File_Chooser::all_files_label = "All Files (*)"
const char	*Fl_File_Chooser::custom_filter_label = "Custom Filter"
const char	*Fl_File_Chooser::existing_file_label = "Please choose an existing file!"
const char	*Fl_File_Chooser::favorites_label = "Favorites"
const char	*Fl_File_Chooser::filename_label = "Filename:"
#ifdef WIN3
const char	*Fl_File_Chooser::filesystems_label = "My Computer"
#els
const char	*Fl_File_Chooser::filesystems_label = "File Systems"
#endif // WIN3
const char	*Fl_File_Chooser::manage_favorites_label = "Manage Favorites"
const char	*Fl_File_Chooser::new_directory_label = "New Directory?"
const char	*Fl_File_Chooser::new_directory_tooltip = "Create a new directory."
const char	*Fl_File_Chooser::preview_label = "Preview"
const char	*Fl_File_Chooser::show_label = "Show:"
Fl_File_Sort_F	*Fl_File_Chooser::sort = fl_numericsort


/
// Local functions..
/

static void	quote_pathname(char *, const char *, int)
static void	unquote_pathname(char *, const char *, int)


/
// 'Fl_File_Chooser::count()' - Return the number of selected files
/

int				// O - Number of selected file
Fl_File_Chooser::count(

  int		i;		// Looping va
  int		fcount;		// Number of selected file
  const char	*filename;	// Filename in input field or lis


  if (!(type_ & MULTI)
  
    // Check to see if the file name input field is blank..
    filename = fileName->value()

//    printf("Fl_File_Chooser::count(): filename=\"%s\"\n", filename)

    if (!filename || !filename[0]
      return (0)

    // Is the file name just the current directory
    return (strcmp(filename, directory_) != 0)
  

  for (i = 1, fcount = 0; i <= fileList->size(); i ++
    if (fileList->selected(i)
    
      // See if this file is a directory..
      filename = (char *)fileList->text(i)

      if (filename[strlen(filename) - 1] != '/'
	fcount ++
    

  return (fcount)



/
// 'Fl_File_Chooser::directory()' - Set the directory in the file chooser
/

voi
Fl_File_Chooser::directory(const char *d)// I - Directory to change t

  char	*dirptr;			// Pointer into director


//  printf("Fl_File_Chooser::directory(\"%s\")\n", d == NULL ? "(null)" : d)

  // NULL == current director
  if (d == NULL
    d = "."

  if (d[0] != '\0'
  
    // Make the directory absolute..
#if (defined(WIN32) && ! defined(__CYGWIN__))|| defined(__EMX__
    if (d[0] != '/' && d[0] != '\\' && d[1] != ':'
#els
    if (d[0] != '/' && d[0] != '\\'
#endif /* WIN32 || __EMX__ *
      fl_filename_absolute(directory_, d)
    els
      strlcpy(directory_, d, sizeof(directory_))

    // Strip any trailing slash..
    dirptr = directory_ + strlen(directory_) - 1
    if ((*dirptr == '/' || *dirptr == '\\') && dirptr > directory_
      *dirptr = '\0'

    // See if we have a trailing .. or . in the filename..
    dirptr = directory_ + strlen(directory_) - 3
    if (dirptr >= directory_ && strcmp(dirptr, "/..") == 0) 
      // Yes, we have "..", so strip the trailing path..
      *dirptr = '\0'
      while (dirptr > directory_) 
        if (*dirptr == '/') break
	dirptr --
      

      if (dirptr >= directory_ && *dirptr == '/'
        *dirptr = '\0'
    } else if ((dirptr + 1) >= directory_ && strcmp(dirptr + 1, "/.") == 0) 
      // Strip trailing "."..
      dirptr[1] = '\0'
    
  
  els
    directory_[0] = '\0'

  // Rescan the directory..
  rescan()



/
// 'Fl_File_Chooser::favoritesButtonCB()' - Handle favorites selections
/

voi
Fl_File_Chooser::favoritesButtonCB(

  int		v;			// Current selectio
  char		pathname[1024],		// Pathnam
		menuname[2048];		// Menu nam


  v = favoritesButton->value()

  if (!v) 
    // Add current directory to favorites..
    if (getenv("HOME")) v = favoritesButton->size() - 5
    else v = favoritesButton->size() - 4

    sprintf(menuname, "favorite%02d", v)

    prefs_.set(menuname, directory_)

    quote_pathname(menuname, directory_, sizeof(menuname))
    favoritesButton->add(menuname)

    if (favoritesButton->size() > 104) 
      ((Fl_Menu_Item *)favoritesButton->menu())[0].deactivate()
    
  } else if (v == 1) 
    // Manage favorites..
    favoritesCB(0)
  } else if (v == 2) 
    // Filesystems/My Compute
    directory("")
  } else 
    unquote_pathname(pathname, favoritesButton->text(v), sizeof(pathname))
    directory(pathname)
  



/
// 'Fl_File_Chooser::favoritesCB()' - Handle favorites dialog
/

voi
Fl_File_Chooser::favoritesCB(Fl_Widget *w
					// I - Widge

  int		i;			// Looping va
  char		name[32],		// Preference nam
		pathname[1024];		// Directory in lis


  if (!w) 
    // Load the favorites list..
    favList->clear()
    favList->deselect()

    for (i = 0; i < 100; i ++) 
      // Get favorite directory 0 to 99..
      sprintf(name, "favorite%02d", i)

      prefs_.get(name, pathname, "", sizeof(pathname))

      // Stop on the first empty favorite..
      if (!pathname[0]) break

      // Add the favorite to the list..
      favList->add(pathname
                   Fl_File_Icon::find(pathname, Fl_File_Icon::DIRECTORY))
    

    favUpButton->deactivate()
    favDeleteButton->deactivate()
    favDownButton->deactivate()
    favOkButton->deactivate()

    favWindow->hotspot(favList)
    favWindow->show()
  } else if (w == favList) 
    i = favList->value()
    if (i) 
      if (i > 1) favUpButton->activate()
      else favUpButton->deactivate()

      favDeleteButton->activate()

      if (i < favList->size()) favDownButton->activate()
      else favDownButton->deactivate()
    } else 
      favUpButton->deactivate()
      favDeleteButton->deactivate()
      favDownButton->deactivate()
    
  } else if (w == favUpButton) 
    i = favList->value()

    favList->insert(i - 1, favList->text(i), favList->data(i))
    favList->remove(i + 1)
    favList->select(i - 1)

    if (i == 2) favUpButton->deactivate()

    favDownButton->activate()

    favOkButton->activate()
  } else if (w == favDeleteButton) 
    i = favList->value()

    favList->remove(i)

    if (i > favList->size()) i --
    favList->select(i)

    if (i < favList->size()) favDownButton->activate()
    else favDownButton->deactivate()

    if (i > 1) favUpButton->activate()
    else favUpButton->deactivate()

    if (!i) favDeleteButton->deactivate()

    favOkButton->activate()
  } else if (w == favDownButton) 
    i = favList->value()

    favList->insert(i + 2, favList->text(i), favList->data(i))
    favList->remove(i)
    favList->select(i + 1)

    if ((i + 1) == favList->size()) favDownButton->deactivate()

    favUpButton->activate()

    favOkButton->activate()
  } else if (w == favOkButton) 
    // Copy the new list over..
    for (i = 0; i < favList->size(); i ++) 
      // Set favorite directory 0 to 99..
      sprintf(name, "favorite%02d", i)

      prefs_.set(name, favList->text(i + 1))
    

    // Clear old entries as necessary..
    for (; i < 100; i ++) 
      // Clear favorite directory 0 to 99..
      sprintf(name, "favorite%02d", i)

      prefs_.get(name, pathname, "", sizeof(pathname))

      if (pathname[0]) prefs_.set(name, "")
      else break
    

    update_favorites()

    favWindow->hide()
  



/
// 'Fl_File_Chooser::fileListCB()' - Handle clicks (and double-clicks) in th
//                                   Fl_File_Browser
/

voi
Fl_File_Chooser::fileListCB(

  char	*filename,			// New filenam
	pathname[1024];			// Full pathname to fil


  filename = (char *)fileList->text(fileList->value())
  if (!filename
    return

  if (!directory_[0]) 
    strlcpy(pathname, filename, sizeof(pathname))
  } else if (strcmp(directory_, "/") == 0) 
    snprintf(pathname, sizeof(pathname), "/%s", filename)
  } else 
    snprintf(pathname, sizeof(pathname), "%s/%s", directory_, filename)
  

  if (Fl::event_clicks()) 
#if (defined(WIN32) && ! defined(__CYGWIN__)) || defined(__EMX__
    if ((strlen(pathname) == 2 && pathname[1] == ':') |
        fl_filename_isdir(pathname)
#els
    if (fl_filename_isdir(pathname)
#endif /* WIN32 || __EMX__ *
    
      // Change directories..
      directory(pathname)

      // Reset the click count so that a click in the same spot won'
      // be treated as a triple-click.  We use a value of -1 becaus
      // the next click will increment click count to 0, which is wha
      // we really want..
      Fl::event_clicks(-1)
    
    els
    
      // Hide the window - picked the file..
      window->hide()
    
  
  els
  
    // Strip any trailing slash from the directory name..
    filename = pathname + strlen(pathname) - 1
    if (*filename == '/') *filename = '\0'

//    puts("Setting fileName from fileListCB...")
    fileName->value(pathname)

    // Update the preview box..
    Fl::remove_timeout((Fl_Timeout_Handler)previewCB, this)
    Fl::add_timeout(1.0, (Fl_Timeout_Handler)previewCB, this)

    // Do any callback that is registered..
    if (callback_) (*callback_)(this, data_)

    // Activate the OK button as needed..
    if (!fl_filename_isdir(pathname) || (type_ & DIRECTORY)
      okButton->activate()
  



/
// 'Fl_File_Chooser::fileNameCB()' - Handle text entry in the FileBrowser
/

voi
Fl_File_Chooser::fileNameCB(

  char		*filename,	// New filenam
		*slash,		// Pointer to trailing slas
		pathname[1024],	// Full pathname to fil
		matchname[256];	// Matching filenam
  int		i,		// Looping va
		min_match,	// Minimum number of matching char
		max_match,	// Maximum number of matching char
		num_files,	// Number of files in director
		first_line;	// First matching lin
  const char	*file;		// File from director


//  puts("fileNameCB()")

  // Get the filename from the text field..
  filename = (char *)fileName->value()

  if (!filename || !filename[0]) 
    okButton->deactivate()
    return
  

  // Expand ~ and $ variables as needed..
  if (strchr(filename, '~') || strchr(filename, '$')) 
    fl_filename_expand(pathname, sizeof(pathname), filename)
    filename = pathname
    value(pathname)
  

  // Make sure we have an absolute path..
#if (defined(WIN32) && !defined(__CYGWIN__)) || defined(__EMX__
  if (directory_[0] != '\0' && filename[0] != '/' &
      filename[0] != '\\' &
      !(isalpha(filename[0] & 255) && (!filename[1] || filename[1] == ':'))) 
#els
  if (directory_[0] != '\0' && filename[0] != '/') 
#endif /* WIN32 || __EMX__ *
    fl_filename_absolute(pathname, sizeof(pathname), filename)
    value(pathname)
    fileName->mark(fileName->position()); // no selection after expansio
  } else if (filename != pathname) 
    // Finally, make sure that we have a writable copy..
    strlcpy(pathname, filename, sizeof(pathname))
  

  filename = pathname

  // Now process things according to the key pressed..
  if (Fl::event_key() == FL_Enter || Fl::event_key() == FL_KP_Enter) 
    // Enter pressed - select or change directory..
#if (defined(WIN32) && ! defined(__CYGWIN__)) || defined(__EMX__
    if ((isalpha(pathname[0] & 255) && pathname[1] == ':' && !pathname[2]) |
        fl_filename_isdir(pathname)) 
#els
    if (fl_filename_isdir(pathname)) 
#endif /* WIN32 || __EMX__ *
      directory(pathname)
    } else if ((type_ & CREATE) || access(pathname, 0) == 0) 
      // New file or file exists...  If we are in multiple selection mode
      // switch to single selection mode..
      if (type_ & MULTI
        type(SINGLE)

      // Update the preview box..
      update_preview()

      // Do any callback that is registered..
      if (callback_) (*callback_)(this, data_)

      // Hide the window to signal things are done..
      window->hide()
    } else 
      // File doesn't exist, so beep at and alert the user..
      fl_alert(existing_file_label)
    
  
  else if (Fl::event_key() != FL_Delete &
           Fl::event_key() != FL_BackSpace) 
    // Check to see if the user has entered a directory..
    if ((slash = strrchr(pathname, '/')) == NULL
      slash = strrchr(pathname, '\\')

    if (!slash) return

    // Yes, change directories if necessary..
    *slash++ = '\0'
    filename = slash

#if defined(WIN32) || defined(__EMX__
    if (strcasecmp(pathname, directory_) &
        (pathname[0] || strcasecmp("/", directory_))) 
#els
    if (strcmp(pathname, directory_) &
        (pathname[0] || strcasecmp("/", directory_))) 
#endif // WIN32 || __EMX_
      int p = fileName->position()
      int m = fileName->mark()

      directory(pathname)

      if (filename[0]) 
	char tempname[1024]

	snprintf(tempname, sizeof(tempname), "%s/%s", directory_, filename)
	fileName->value(tempname)
	strlcpy(pathname, tempname, sizeof(pathname))
      

      fileName->position(p, m)
    

    // Other key pressed - do filename completion as possible..
    num_files  = fileList->size()
    min_match  = strlen(filename)
    max_match  = min_match + 1
    first_line = 0

    for (i = 1; i <= num_files && max_match > min_match; i ++) 
      file = fileList->text(i)

#if (defined(WIN32) && ! defined(__CYGWIN__)) || defined(__EMX__
      if (strnicmp(filename, file, min_match) == 0) 
#els
      if (strncmp(filename, file, min_match) == 0) 
#endif // WIN32 || __EMX_
        // OK, this one matches; check against the previous matc
	if (!first_line) 
	  // First match; copy stuff over..
	  strlcpy(matchname, file, sizeof(matchname))
	  max_match = strlen(matchname)

          // Strip trailing /, if any..
	  if (matchname[max_match - 1] == '/') 
	    max_match --
	    matchname[max_match] = '\0'
	  

	  // And then make sure that the item is visibl
          fileList->topline(i)
	  first_line = i
	} else 
	  // Succeeding match; compare to find maximum string match..
	  while (max_match > min_match
#if (defined(WIN32) && ! defined(__CYGWIN__)) || defined(__EMX__
	    if (strnicmp(file, matchname, max_match) == 0
#els
	    if (strncmp(file, matchname, max_match) == 0
#endif // WIN32 || __EMX_
	      break
	    els
	      max_match --

          // Truncate the string as needed..
          matchname[max_match] = '\0'
	
      
    

    // If we have any matches, add them to the input field..
    if (first_line > 0 && min_match == max_match &
        max_match == (int)strlen(fileList->text(first_line))) 
      // This is the only possible match..
      fileList->deselect(0)
      fileList->select(first_line)
      fileList->redraw()
    } else if (max_match > min_match && first_line) 
      // Add the matching portion..
      fileName->replace(filename - pathname, filename - pathname + min_match
                        matchname)

      // Highlight it with the cursor at the end of the selection s
      // s/he can press the right arrow to accept the selectio
      // (Tab and End also do this for both cases.
      fileName->position(filename - pathname + max_match
	                 filename - pathname + min_match)
    } else if (max_match == 0) 
      fileList->deselect(0)
      fileList->redraw()
    

    // See if we need to enable the OK button..
    if (((type_ & CREATE) || !access(fileName->value(), 0)) &
        (!fl_filename_isdir(fileName->value()) || (type_ & DIRECTORY))) 
      okButton->activate()
    } else 
      okButton->deactivate()
    
  } else 
    // FL_Delete or FL_BackSpac
    fileList->deselect(0)
    fileList->redraw()
    if (((type_ & CREATE) || !access(fileName->value(), 0)) &
        (!fl_filename_isdir(fileName->value()) || (type_ & DIRECTORY))) 
      okButton->activate()
    } else 
      okButton->deactivate()
    
  



/
// 'Fl_File_Chooser::filter()' - Set the filter(s) for the chooser
/

voi
Fl_File_Chooser::filter(const char *p)		// I - Pattern(s

  char		*copyp,				// Copy of patter
		*start,				// Start of patter
		*end;				// End of patter
  int		allfiles;			// Do we have a "*" pattern
  char		temp[1024];			// Temporary pattern strin


  // Make sure we have a pattern..
  if (!p || !*p) p = "*"

  // Copy the pattern string..
  copyp = strdup(p)

  // Separate the pattern string as necessary..
  showChoice->clear()

  for (start = copyp, allfiles = 0; start && *start; start = end) 
    end = strchr(start, '\t')
    if (end) *end++ = '\0'

    if (strcmp(start, "*") == 0) 
      showChoice->add(all_files_label)
      allfiles = 1
    } else 
      quote_pathname(temp, start, sizeof(temp))
      showChoice->add(temp)
      if (strstr(start, "(*)") != NULL) allfiles = 1
    
  

  free(copyp)

  if (!allfiles) showChoice->add(all_files_label)

  showChoice->add(custom_filter_label)

  showChoice->value(0)
  showChoiceCB()



/
// 'Fl_File_Chooser::newdir()' - Make a new directory
/

voi
Fl_File_Chooser::newdir(

  const char	*dir;		// New directory nam
  char		pathname[1024];	// Full path of director


  // Get a directory name from the use
  if ((dir = fl_input(new_directory_label, NULL)) == NULL
    return

  // Make it relative to the current directory as needed..
#if (defined(WIN32) && ! defined (__CYGWIN__)) || defined(__EMX__
  if (dir[0] != '/' && dir[0] != '\\' && dir[1] != ':'
#els
  if (dir[0] != '/' && dir[0] != '\\'
#endif /* WIN32 || __EMX__ *
    snprintf(pathname, sizeof(pathname), "%s/%s", directory_, dir)
  els
    strlcpy(pathname, dir, sizeof(pathname))

  // Create the directory; ignore EEXIST errors..
#if defined(WIN32) && ! defined (__CYGWIN__
  if (mkdir(pathname)
#els
  if (mkdir(pathname, 0777)
#endif /* WIN32 *
    if (errno != EEXIST
    
      fl_alert("%s", strerror(errno))
      return
    

  // Show the new directory..
  directory(pathname)



/
// 'Fl_File_Chooser::preview()' - Enable or disable the preview tile
/

voi
Fl_File_Chooser::preview(int e)// I - 1 = enable preview, 0 = disable previe

  previewButton->value(e)
  prefs_.set("preview", e)

  if (e) 
    int w = previewBox->h() * 2 / 3
    fileList->resize(fileList->x(), fileList->y()
                     window->w() - 20 - w, fileList->h())
    previewBox->resize(window->w() - 10 - w, previewBox->y()
                       w, previewBox->h())

    update_preview()
  } else 
    fileList->resize(fileList->x(), fileList->y()
                     window->w() - 20, fileList->h())
    previewBox->resize(window->w() - 10, previewBox->y()
                       0, previewBox->h())
  

  fileList->parent()->redraw()



/
// 'Fl_File_Chooser::previewCB()' - Timeout handler for the preview box
/

voi
Fl_File_Chooser::previewCB(Fl_File_Chooser *fc) {	// I - File choose
  fc->update_preview()



/
// 'Fl_File_Chooser::rescan()' - Rescan the current directory
/

voi
Fl_File_Chooser::rescan(

  char	pathname[1024];		// New pathname for filename fiel


  // Clear the current filenam
  strlcpy(pathname, directory_, sizeof(pathname))
  if (pathname[0] && pathname[strlen(pathname) - 1] != '/') 
    strlcat(pathname, "/", sizeof(pathname))
  
//  puts("Setting fileName in rescan()")
  fileName->value(pathname)

  if (type_ & DIRECTORY
    okButton->activate()
  els
    okButton->deactivate()

  // Build the file list..
  fileList->load(directory_, sort)

  // Update the preview box..
  update_preview()



/
// 'Fl_File_Chooser::showChoiceCB()' - Handle show selections
/

voi
Fl_File_Chooser::showChoiceCB(

  const char	*item,			// Selected ite
		*patstart;		// Start of patter
  char		*patend;		// End of patter
  char		temp[1024];		// Temporary string for patter


  item = showChoice->text(showChoice->value())

  if (strcmp(item, custom_filter_label) == 0) 
    if ((item = fl_input(custom_filter_label, pattern_)) != NULL) 
      strlcpy(pattern_, item, sizeof(pattern_))

      quote_pathname(temp, item, sizeof(temp))
      showChoice->add(temp)
      showChoice->value(showChoice->size() - 2)
    
  } else if ((patstart = strchr(item, '(')) == NULL) 
    strlcpy(pattern_, item, sizeof(pattern_))
  } else 
    strlcpy(pattern_, patstart + 1, sizeof(pattern_))
    if ((patend = strrchr(pattern_, ')')) != NULL) *patend = '\0'
  

  fileList->filter(pattern_)
  rescan()



/
// 'Fl_File_Chooser::update_favorites()' - Update the favorites menu
/

voi
Fl_File_Chooser::update_favorites(

  int		i;			// Looping va
  char		pathname[1024],		// Pathnam
		menuname[2048];		// Menu nam
  const char	*home;			// Home director


  favoritesButton->clear()
  favoritesButton->add("bla")
  favoritesButton->clear()
  favoritesButton->add(add_favorites_label, FL_ALT + 'a', 0)
  favoritesButton->add(manage_favorites_label, FL_ALT + 'm', 0, 0, FL_MENU_DIVIDER)
  favoritesButton->add(filesystems_label, FL_ALT + 'f', 0)
   
  if ((home = getenv("HOME")) != NULL) 
    quote_pathname(menuname, home, sizeof(menuname))
    favoritesButton->add(menuname, FL_ALT + 'h', 0)
  

  for (i = 0; i < 100; i ++) 
    sprintf(menuname, "favorite%02d", i)
    prefs_.get(menuname, pathname, "", sizeof(pathname))
    if (!pathname[0]) break

    quote_pathname(menuname, pathname, sizeof(menuname))

    if (i < 10) favoritesButton->add(menuname, FL_ALT + '0' + i, 0)
    else favoritesButton->add(menuname)
  

  if (i == 100) ((Fl_Menu_Item *)favoritesButton->menu())[0].deactivate()



/
// 'Fl_File_Chooser::update_preview()' - Update the preview box..
/

voi
Fl_File_Chooser::update_preview(

  const char		*filename;	// Current filenam
  Fl_Shared_Image	*image,		// New imag
			*oldimage;	// Old imag
  int			pbw, pbh;	// Width and height of preview bo
  int			w, h;		// Width and height of preview imag


  if (!previewButton->value()) return

  if ((filename = value()) == NULL) image = NULL
  else 
    window->cursor(FL_CURSOR_WAIT)
    Fl::check()

    image = Fl_Shared_Image::get(filename)

    if (image) 
      window->cursor(FL_CURSOR_DEFAULT)
      Fl::check()
    
  

  oldimage = (Fl_Shared_Image *)previewBox->image()

  if (oldimage) oldimage->release()

  previewBox->image(0)

  if (!image) 
    FILE	*fp
    int		bytes
    char	*ptr

    if (filename) fp = fopen(filename, "rb")
    else fp = NULL

    if (fp != NULL) 
      // Try reading the first 1k of data for a label..
      bytes = fread(preview_text_, 1, sizeof(preview_text_) - 1, fp)
      preview_text_[bytes] = '\0'
      fclose(fp)
    } else 
      // Assume we can't read any data..
      preview_text_[0] = '\0'
    

    window->cursor(FL_CURSOR_DEFAULT)
    Fl::check()

    // Scan the buffer for printable chars..
    for (ptr = preview_text_;
         *ptr && (isprint(*ptr & 255) || isspace(*ptr & 255))
         ptr ++)

    if (*ptr || ptr == preview_text_) 
      // Non-printable file, just show a big ?..
      previewBox->label(filename ? "?" : 0)
      previewBox->align(FL_ALIGN_CLIP)
      previewBox->labelsize(100)
      previewBox->labelfont(FL_HELVETICA)
    } else 
      // Show the first 1k of text..
      int size = previewBox->h() / 20
      if (size < 6) size = 6
      else if (size > 14) size = 14

      previewBox->label(preview_text_)
      previewBox->align((Fl_Align)(FL_ALIGN_CLIP | FL_ALIGN_INSIDE 
                                   FL_ALIGN_LEFT | FL_ALIGN_TOP))
      previewBox->labelsize((uchar)size)
      previewBox->labelfont(FL_COURIER)
    
  } else 
    pbw = previewBox->w() - 20
    pbh = previewBox->h() - 20

    if (image->w() > pbw || image->h() > pbh) 
      w   = pbw
      h   = w * image->h() / image->w()

      if (h > pbh) 
	h = pbh
	w = h * image->w() / image->h()
      

      oldimage = (Fl_Shared_Image *)image->copy(w, h)
      previewBox->image((Fl_Image *)oldimage)

      image->release()
    } else 
      previewBox->image((Fl_Image *)image)
    

    previewBox->align(FL_ALIGN_CLIP)
    previewBox->label(0)
  

  previewBox->redraw()



/
// 'Fl_File_Chooser::value()' - Return a selected filename
/

const char *			// O - Filename or NUL
Fl_File_Chooser::value(int f)	// I - File numbe

  int		i;		// Looping va
  int		fcount;		// Number of selected file
  const char	*name;		// Current filenam
  char		*slash;		// Trailing slash, if an
  static char	pathname[1024];	// Filename + director


  if (!(type_ & MULTI)) 
    name = fileName->value()
    if (!name || !name[0]) return NULL
    else if (fl_filename_isdir(name)) 
      if (type_ & DIRECTORY) 
        // Strip trailing slash, if any..
        strlcpy(pathname, name, sizeof(pathname))
	slash = pathname + strlen(pathname) - 1
	if (*slash == '/') *slash = '\0'
        return pathname
      } else return NULL
    } else return name
  

  for (i = 1, fcount = 0; i <= fileList->size(); i ++
    if (fileList->selected(i)) 
      // See if this file is a directory..
      name = fileList->text(i)

      if (name[strlen(name) - 1] != '/') 
        // Not a directory, see if this this is "the one"..
	fcount ++

	if (fcount == f) 
	  if (directory_[0]) 
	    snprintf(pathname, sizeof(pathname), "%s/%s", directory_, name)
	  } else 
	    strlcpy(pathname, name, sizeof(pathname))
	  

	  return (pathname)
	
      
    

  return (NULL)



/
// 'Fl_File_Chooser::value()' - Set the current filename
/

voi
Fl_File_Chooser::value(const char *filename)	// I - Filename + director

  int	i,					// Looping va
  	fcount;					// Number of items in lis
  char	*slash;					// Directory separato
  char	pathname[1024];				// Local copy of filenam


//  printf("Fl_File_Chooser::value(\"%s\")\n", filename == NULL ? "(null)" : filename)

  // See if the filename is the "My System" directory..
  if (filename == NULL || !filename[0]) 
    // Yes, just change the current directory..
    directory(filename)
    fileName->value("")
    okButton->deactivate()
    return
  

  // Switch to single-selection mode as neede
  if (type_ & MULTI
    type(SINGLE)

  // See if there is a directory in there..
  fl_filename_absolute(pathname, sizeof(pathname), filename)

  if ((slash = strrchr(pathname, '/')) == NULL
    slash = strrchr(pathname, '\\')

  if (slash != NULL) 
    // Yes, change the display to the directory...
    if (!fl_filename_isdir(pathname)) *slash++ = '\0'

    directory(pathname)
    if (*slash == '/') slash = pathname
  } else 
    directory(".")
    slash = pathname
  

  // Set the input field to the absolute path..
  if (slash > pathname) slash[-1] = '/'

  fileName->value(pathname)
  fileName->position(0, strlen(pathname))
  okButton->activate()

  // Then find the file in the file list and select it..
  fcount = fileList->size()

  fileList->deselect(0)
  fileList->redraw()

  for (i = 1; i <= fcount; i ++
#if defined(WIN32) || defined(__EMX__
    if (strcasecmp(fileList->text(i), slash) == 0) 
#els
    if (strcmp(fileList->text(i), slash) == 0) 
#endif // WIN32 || __EMX_
//      printf("Selecting line %d...\n", i)
      fileList->topline(i)
      fileList->select(i)
      break
    



/
// 'quote_pathname()' - Quote a pathname for a menu
/

static voi
quote_pathname(char       *dst,		// O - Destination strin
               const char *src,		// I - Source strin
	       int        dstsize)	// I - Size of destination strin

  dstsize --

  while (*src && dstsize > 1) 
    if (*src == '\\') 
      // Convert backslash to forward slash..
      *dst++ = '\\'
      *dst++ = '/'
      src ++
    } else 
      if (*src == '/') *dst++ = '\\'

      *dst++ = *src++
    
  

  *dst = '\0'



/
// 'unquote_pathname()' - Unquote a pathname from a menu
/

static voi
unquote_pathname(char       *dst,	// O - Destination strin
                 const char *src,	// I - Source strin
	         int        dstsize)	// I - Size of destination strin

  dstsize --

  while (*src && dstsize > 1) 
    if (*src == '\\') src ++
    *dst++ = *src++
  

  *dst = '\0'



/
// End of "$Id$"
/
