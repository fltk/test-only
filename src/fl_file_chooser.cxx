//
// "$Id: fl_file_chooser.cxx,v 1.22 2000/06/12 06:35:38 bill Exp $"
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
#include <FL/fl_file_chooser.H>
#include <FL/Fl_FileChooser.H>


static Fl_FileChooser	*fc = (Fl_FileChooser *)0;

static void default_callback(const char*) {}
static void (*current_callback)(const char*) = default_callback;
void fl_file_chooser_callback(void (*cb)(const char*)) {
  current_callback = cb ? cb : default_callback;
}
  

char* fl_file_chooser(const char* message, const char* pat, const char* fname)
{
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
// End of "$Id: fl_file_chooser.cxx,v 1.22 2000/06/12 06:35:38 bill Exp $".
//
