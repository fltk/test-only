/
// "$Id$
/
// File chooser widget for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
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

#include "flstring.h
#include <FL/filename.H
#include <FL/Fl_File_Chooser.H
#include <FL/Fl_File_Chooser.H

static Fl_File_Chooser	*fc = (Fl_File_Chooser *)0
static void		(*current_callback)(const char*) = 0


static void callback(Fl_File_Chooser *, void*) 
  if (current_callback && fc->value()
    (*current_callback)(fc->value())



void fl_file_chooser_callback(void (*cb)(const char*)) 
  current_callback = cb



/
// 'fl_file_chooser()' - Show a file chooser dialog and get a filename
/

char *					// O - Filename or NUL
fl_file_chooser(const char *message,	// I - Message in titleba
                const char *pat,	// I - Filename patter
		const char *fname,	// I - Initial filename selectio
		int        relative) {	// I - 0 for absolute pat
  static char	retname[1024];		// Returned filenam

  if (!fc) 
    if (!fname || !*fname) fname = "."

    fc = new Fl_File_Chooser(fname, pat, Fl_File_Chooser::CREATE, message)
    fc->callback(callback, 0)
  } else 
    fc->type(Fl_File_Chooser::CREATE)
    fc->filter(pat)
    fc->label(message)

    if (!fname || !*fname) 
      if (fc->filter() != pat && (!pat || !fc->filter() |
          strcmp(pat, fc->filter())) && fc->value()) 
	// if pattern is different, remove name but leave old directory
	strlcpy(retname, fc->value(), sizeof(retname))

	char *p = strrchr(retname, '/')

        if (p) 
	  // If the filename is "/foo", then the directory will be "/", no
	  // ""..
	  if (p == retname
	    retname[1] = '\0'
	  els
	    *p = '\0'
	

	// Set the directory..
	fc->directory(retname)
      
    
    els
      fc->value(fname)
  

  fc->show()

  while (fc->shown()
    Fl::wait()

  if (fc->value() && relative) 
    fl_filename_relative(retname, sizeof(retname), fc->value())

    return retname
  } else if (fc->value()) return (char *)fc->value()
  else return 0



/
// 'fl_dir_chooser()' - Show a file chooser dialog and get a directory
/

char *					// O - Directory or NUL
fl_dir_chooser(const char *message,	// I - Message for titleba
               const char *fname,	// I - Initial directory nam
	       int        relative)	// I - 0 for absolut

  static char	retname[1024];		// Returned directory nam

  if (!fc) 
    if (!fname || !*fname) fname = "."

    fc = new Fl_File_Chooser(fname, "*", Fl_File_Chooser::CREATE 
                                         Fl_File_Chooser::DIRECTORY, message)
    fc->callback(callback, 0)
  } else 
    fc->type(Fl_File_Chooser::CREATE | Fl_File_Chooser::DIRECTORY)
    fc->filter("*")
    if (fname && *fname) fc->value(fname)
    fc->label(message)
  

  fc->show()

  while (fc->shown()
    Fl::wait()

  if (fc->value() && relative) 
    fl_filename_relative(retname, sizeof(retname), fc->value())

    return retname
  } else if (fc->value()) return (char *)fc->value()
  else return 0



/
// End of "$Id$"
/
