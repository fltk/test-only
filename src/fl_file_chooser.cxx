//
// "$Id: fl_file_chooser.cxx,v 1.17 1999/10/25 21:12:12 mike Exp $"
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
#include <FL/Fl_FileChooser.h>


static Fl_FileChooser	*fc = (Fl_FileChooser *)0;

static void default_callback(const char*) {}
static void (*current_callback)(const char*) = default_callback;
void fl_file_chooser_callback(void (*cb)(const char*)) {
  current_callback = cb ? cb : default_callback;
}
  

char* fl_file_chooser(const char* message, const char* pat, const char* fname)
{
  if (!fc) fc = new Fl_FileChooser(fname, pat, Fl_FileChooser::SINGLE, message);
  else {
    fc->filter(pat);
    fc->value(fname);
    fc->label(message);
  }

  if (fname == NULL || !fname[0])
    fc->directory(fname);

  fc->show();
  while (fc->visible())
  {
    printf("fc->visible() = %d\n", fc->visible());
    printf("fc->window->visible() = %d\n", fc->window->visible());
    printf("fc->window->flags() = %x\n", fc->window->flags());
    Fl::wait();
  }

  return ((char *)fc->value());
}

//
// End of "$Id: fl_file_chooser.cxx,v 1.17 1999/10/25 21:12:12 mike Exp $".
//
