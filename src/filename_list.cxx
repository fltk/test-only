/
// "$Id$
/
// Filename list routines for the Fast Light Tool Kit (FLTK)
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

// Wrapper for scandir with const-correct function prototypes

#include <FL/filename.H
#include "flstring.h


extern "C" 
#ifndef HAVE_SCANDI
  int fl_scandir (const char *dir, dirent ***namelist
	          int (*select)(dirent *)
	          int (*compar)(dirent **, dirent **))
#  define scandir	fl_scandi
#endi


int fl_alphasort(struct dirent **a, struct dirent **b) 
  return strcmp((*a)->d_name, (*b)->d_name)


int fl_casealphasort(struct dirent **a, struct dirent **b) 
  return strcasecmp((*a)->d_name, (*b)->d_name)



int fl_filename_list(const char *d, dirent ***list
                     Fl_File_Sort_F *sort) 
#ifndef HAVE_SCANDI
  return scandir(d, list, 0, sort)
#elif defined(__hpux) || defined(__CYGWIN__
  // HP-UX, Cygwin define the comparison function like this
  return scandir(d, list, 0, (int(*)(const dirent **, const dirent **))sort)
#elif defined(__osf__
  // OSF, DU 4.0
  return scandir(d, list, 0, (int(*)(dirent **, dirent **))sort)
#elif defined(_AIX
  // AIX is almost standard..
  return scandir(d, list, 0, (int(*)(void*, void*))sort)
#elif !defined(__sgi
  // The vast majority of UNIX systems want the sort function to have thi
  // prototype, most likely so that it can be passed to qsort without an
  // changes
  return scandir(d, list, 0, (int(*)(const void*,const void*))sort)
#els
  // This version is when we define our own scandir (WIN32 and perhap
  // some Unix systems) and apparently on IRIX
  return scandir(d, list, 0, sort)
#endi


/
// End of "$Id$"
/
