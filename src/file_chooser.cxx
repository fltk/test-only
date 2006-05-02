//
// "$Id$"
//
// File chooser widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2006 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <config.h>
#include <fltk/file_chooser.h>
#include <fltk/FileChooser.h>
#include <fltk/run.h>
#include <fltk/string.h>

using namespace fltk;

static FileChooser	*fc = (FileChooser *)0;
static void		(*current_callback)(const char*) = 0;
static const char	*current_label = "Ok";


// Do a file chooser callback...
static void callback(FileChooser *, void*) {
  if (current_callback && fc->value())
    (*current_callback)(fc->value());
}


// Set the file chooser callback
void fltk::file_chooser_callback(void (*cb)(const char*)) {
  current_callback = cb;
}


// Set the "OK" button label
void fltk::file_chooser_ok_label(const char *l) {
  if (l) current_label = l;
  else current_label = "Ok";
}


//
// 'fl_file_chooser()' - Show a file chooser dialog and get a filename.
//

const char *					// O - Filename or NULL
fltk::file_chooser(const char *message,	// I - Message in titlebar
                const char *pat,	// I - Filename pattern
		const char *fname,	// I - Initial filename selection
		int        relative) {	// I - 0 for absolute path
  static char	retname[1024];		// Returned filename

  if (!fc) {
    if (!fname || !*fname) fname = ".";

    fc = new FileChooser(fname, pat, FileChooser::CREATE, message);
    fc->callback(callback, 0);
  }
    fc->type(FileChooser::CREATE);
    fc->filter(pat);
    fc->label(message);

    if (!fname || !*fname) {
      if (fc->filter() != pat && (!pat || !fc->filter() ||
          strcmp(pat, fc->filter())) && fc->value()) {
	// if pattern is different, remove name but leave old directory:
	strlcpy(retname, fc->value(), sizeof(retname));

	char *p = strrchr(retname, '/');

        if (p) {
	  // If the filename is "/foo", then the directory will be "/", not
	  // ""...
	  if (p == retname)
	    retname[1] = '\0';
	  else
	    *p = '\0';
	}

	// Set the directory...
	fc->directory(retname);
      }
      fc->show();
      fc->value(fname);
    }
    else {
      fc->ok_label(current_label);
      fc->show();
      fc->value(fname);
    }  

  while (fc->visible()) fltk::wait();

  if (fc->value() && relative) {
      fltk::filename_relative(retname, sizeof(retname), fc->value());

    return retname;
  } else if (fc->value()) {
      fltk::filename_absolute(retname, sizeof(retname), fc->value());
    return retname;
  }
  else return 0;
}


//
// 'fl_dir_chooser()' - Show a file chooser dialog and get a directory.
//

const char *					// O - Directory or NULL
fltk::dir_chooser(const char *message,	// I - Message for titlebar
               const char *fname,	// I - Initial directory name
	       int        relative)	// I - 0 for absolute
{
  static char	retname[1024];		// Returned directory name

  if (!fc) {
    if (!fname || !*fname) fname = ".";

    fc = new FileChooser(fname, "*", FileChooser::CREATE |
                                         FileChooser::DIRECTORY, message);
    fc->callback(callback, 0);
  } else {
    fc->type(FileChooser::CREATE | FileChooser::DIRECTORY);
    fc->filter("*");
    if (fname && *fname) fc->value(fname);
    fc->label(message);
  }

  fc->show();

  while (fc->shown())
    fltk::wait();

  if (fc->value() && relative) {
      fltk::filename_relative(retname, sizeof(retname), fc->value());

    return retname;
  } else if (fc->value()) return (char *)fc->value();
  else return 0;
}

























#if 0







static bool use_system_fc = false;
/*! On Windows this makes file_chooser() call the Win32 file chooser
  API instead of using the one constructed in fltk. Ignored on other
  systems.
*/
void fltk::use_system_file_chooser(bool useit) {use_system_fc = useit;}

#ifdef _WIN32
# include <windows.h>
# include <commdlg.h>
# ifndef __CYGWIN__
#  include <direct.h>
#  ifndef OPENFILENAME_SIZE_VERSION_400
#   define OPENFILENAME_SIZE_VERSION_400 sizeof(OPENFILENAME)
#  endif
# endif
# if defined(__BORLANDC__)
#  include <dos.h> // _getdrive()
# endif
#endif

static void default_callback(const char*) {}
static void (*current_callback)(const char*) = default_callback;
/*! This function is called every time the user navigates to a new file
  or directory in the file chooser. It can be used to preview the result
  in the main window. */
void fltk::file_chooser_callback(void (*cb)(const char*)) {
  current_callback = cb ? cb : default_callback;
}

/*!
  \image html filechooser.gif

  pops up the file chooser, waits for the user to pick a file or
  Cancel, and then returns a pointer to that filename or NULL if
  Cancel is chosen.

  \a message is a string used to title the window.

  \a pattern is used to limit the files listed in a directory to those
  matching the pattern. This matching is done by filename_match(). Use
  NULL to show all files.

  \a fname is a default folder/filename to fill in the chooser
  with. If this ends with a '/' then this is a default folder and
  no file is preselected.

  If \a fname is NULL then the last filename that was choosen is used,
  unless the \a pattern changes, in which case only the last directory
  is used. The first time the file chooser is called this defaults to
  a blank string.

  The returned value points at a static buffer that is only good until
  the next time the file_chooser() is called.
*/
const char* fltk::file_chooser(const char* message,
			       const char* pattern,
			       const char* fname,
			       bool save)
{
#if defined( _WIN32 ) && !defined( __CYGWIN__ )
  if (use_system_fc) {
    static char filenamebuffer[MAX_PATH];
    static OPENFILENAME wreq;
    memset(&wreq, 0, sizeof(wreq));
    wreq.lStructSize = OPENFILENAME_SIZE_VERSION_400; // needed for Win < Win2k
    wreq.lpstrFilter = pattern;
    wreq.nFilterIndex = (pattern) ? 1 : 0;	
    wreq.lpstrFile = filenamebuffer;
    wreq.nMaxFile = MAX_PATH;
    wreq.lpstrTitle = message ? message : "Select the filename";
    if(fname) {
      memset(filenamebuffer, 0, MAX_PATH);
      if((filenamebuffer[1] == ':') && (_getdrive() + 'A' - 1 == filenamebuffer[0]))
	strncpy(filenamebuffer, fname + 2, MAX_PATH);
      else
	strncpy(filenamebuffer, fname, MAX_PATH);
    }
    wreq.Flags = OFN_NOCHANGEDIR;
    if (save ? GetSaveFileName(&wreq) : GetOpenFileName(&wreq))
      return wreq.lpstrFile;
    return 0;
  }
#endif
  static FileChooser *fc;
  if (!fc)
    fc = new FileChooser(fname, pattern, FileChooser::CREATE, message);
  else {
    fc->filter(pattern);
    fc->text(fname);
    fc->label(message);
  }
  fc->show();
  while(fc->shown()) fltk::check();
  return fc->text();
}

#endif

//
// End of "$Id$".
//
