//
// "$Id: fl_file_chooser.cxx,v 1.34 2004/07/27 10:34:00 laza2000 Exp $"
//
// File chooser widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
char* fltk::file_chooser(const char* message,
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
    fc->value(fname);
    fc->label(message);
  }
  fc->exec();
  return ((char *)fc->value());
}

//
// End of "$Id: fl_file_chooser.cxx,v 1.34 2004/07/27 10:34:00 laza2000 Exp $".
//
