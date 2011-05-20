//
// "$Id$"
//
// Message test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

#include <fltk3/run.h>
#include <fltk3/Window.h>
#include <fltk3/ask.h>
#include <stdio.h>

int main(int argc, char **argv) {
  fltk3::scheme(NULL);
  fltk3::args(argc, argv);
  fltk3::get_system_colors();

  fltk3::message("Spelling check sucessful, %d errors found with %g%% confidence",
	     1002, 100*(15/77.0));

  fltk3::alert(
		"Quantum fluctuations in the space-time continuum detected,\n"
	  "you have %g seconds to comply.\n\n"
		"\"In physics, spacetime is any mathematical model that combines\n"
		"space and time into a single construct called the space-time\n"
		"continuum. Spacetime is usually interpreted with space being\n"
		"three-dimensional and time playing the role of the\n"
		"fourth dimension.\" - Wikipedia",
		10.0);

  printf("fltk3::choice returned %d\n",
    fltk3::choice("Do you really want to %s?", "No", "Yes", 0L, "continue"));

  printf("fltk3::choice returned %d\n",
    fltk3::choice("Choose one of the following:","choice0","choice1","choice2"));
  const char *r;

  r = fltk3::input("Please enter a string for '%s':", "this is the default value",
	       "testing");
  printf("fltk3::input returned \"%s\"\n", r ? r : "NULL");

  r = fltk3::password("Enter %s's password:", 0, "somebody");
  printf("fltk3::password returned \"%s\"\n", r ? r : "NULL");

  return 0;
}

//
// End of "$Id$".
//
