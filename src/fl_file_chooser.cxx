//
// "$Id: fl_file_chooser.cxx,v 1.26 2001/07/23 09:50:05 spitzak Exp $"
//
// File chooser widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <config.h>
#include <fltk/fl_file_chooser.h>
#include <fltk/Fl_FileChooser.h>

#ifdef WIN32
#include <windows.h>
#include <commdlg.h>
#include <direct.h>
static int guse_windows_filereq = 0;
static char filenamebuffer[MAX_PATH] = "";
static OPENFILENAME wreq;
#ifndef OPENFILENAME_SIZE_VERSION_400
#	define OPENFILENAME_SIZE_VERSION_400 sizeof(OPENFILENAME)
#endif

void use_windows_stock_file_requester(int useit) 
{
	guse_windows_filereq = useit;
}

void init_wreq_struct(const char *message, const char *pat, const char *fname) 
{
	memset(&wreq, 0, sizeof(wreq));
	wreq.lStructSize = OPENFILENAME_SIZE_VERSION_400; // needed for Win < Win2k
	wreq.lpstrFilter = pat;
	wreq.nFilterIndex = (pat) ? 1 : 0;	
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
}
#endif


static Fl_FileChooser	*fc = (Fl_FileChooser *)0;

static void default_callback(const char*) {}
static void (*current_callback)(const char*) = default_callback;
void fl_file_chooser_callback(void (*cb)(const char*)) {
  current_callback = cb ? cb : default_callback;
}

char* fl_file_chooser(const char* message, const char* pat, const char* fname)
{
#ifdef WIN32
  if(guse_windows_filereq) {
    init_wreq_struct(message, pat, fname);
	if(GetSaveFileName(&wreq))
		return wreq.lpstrFile;
	else
		return NULL;
  }
#endif
  if (!fc) fc = new Fl_FileChooser(fname, pat, Fl_FileChooser::CREATE, message);
  else {
    fc->filter(pat);
    fc->value(fname);
    fc->label(message);
  }
  fc->exec();
  return ((char *)fc->value());
}

//
// End of "$Id: fl_file_chooser.cxx,v 1.26 2001/07/23 09:50:05 spitzak Exp $".
//
