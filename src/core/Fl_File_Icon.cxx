//
// "$Id$"
//
// fltk3::FileIcon routines.
//
// KDE icon code donated by Maarten De Boer.
//
// Copyright 1999-2010 by Michael Sweet.
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
// Contents:
//
//   fltk3::FileIcon::fltk3::FileIcon()       - Create a new file icon.
//   fltk3::FileIcon::~Fl_File_Icon()      - Remove a file icon.
//   fltk3::FileIcon::add()               - Add data to an icon.
//   fltk3::FileIcon::find()              - Find an icon based upon a given file.
//   fltk3::FileIcon::draw()              - Draw an icon.
//   fltk3::FileIcon::label()             - Set the widgets label to an icon.
//   fltk3::FileIcon::labeltype()         - Draw the icon label.
//

//
// Include necessary header files...
//

#include <stdio.h>
#include <stdlib.h>
#include <fltk3/utf8.h>
#include "flstring.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#if (defined(WIN32) && ! defined(__CYGWIN__)) || defined(__EMX__)
#  include <io.h>
#  define F_OK	0
#else
#  include <unistd.h>
#endif /* WIN32 || __EMX__ */

#include <fltk3/FileIcon.h>
#include <fltk3/Widget.h>
#include <fltk3/draw.h>
#include <fltk3/filename.h>


//
// Define missing POSIX/XPG4 macros as needed...
//

#ifndef S_ISDIR
#  define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK)
#  define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
#  define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#  define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#  define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
#endif /* !S_ISDIR */


//
// Icon cache...
//

fltk3::FileIcon	*fltk3::FileIcon::first_ = (fltk3::FileIcon *)0;


/**
  Creates a new fltk3::FileIcon with the specified information.
  \param[in] p filename pattern
  \param[in] t file type
  \param[in] nd number of data values
  \param[in] d data values
*/
fltk3::FileIcon::FileIcon(const char *p,	/* I - Filename pattern */
                	   int        t,	/* I - File type */
			   int        nd,	/* I - Number of data values */
			   short      *d)	/* I - Data values */
{
  // Initialize the pattern and type...
  pattern_ = p;
  type_    = t;

  // Copy icon data as needed...
  if (nd)
  {
    num_data_   = nd;
    alloc_data_ = nd + 1;
    data_       = (short *)calloc(sizeof(short), nd + 1);
    memcpy(data_, d, nd * sizeof(short));
  }
  else
  {
    num_data_   = 0;
    alloc_data_ = 0;
  }

  // And add the icon to the list of icons...
  next_  = first_;
  first_ = this;
}


/**
  The destructor destroys the icon and frees all memory that has been
  allocated for it.
*/
fltk3::FileIcon::~FileIcon() {
  fltk3::FileIcon	*current,	// Current icon in list
		*prev;		// Previous icon in list


  // Find the icon in the list...
  for (current = first_, prev = (fltk3::FileIcon *)0;
       current != this && current != (fltk3::FileIcon *)0;
       prev = current, current = current->next_);

  // Remove the icon from the list as needed...
  if (current)
  {
    if (prev)
      prev->next_ = current->next_;
    else
      first_ = current->next_;
  }

  // Free any memory used...
  if (alloc_data_)
    free(data_);
}


/**
  Adds a keyword value to the icon array, returning a pointer to it.
  \param[in] d data value
*/
short *				// O - Pointer to new data value
fltk3::FileIcon::add(short d)	// I - Data to add
{
  short	*dptr;			// Pointer to new data value


  // Allocate/reallocate memory as needed
  if ((num_data_ + 1) >= alloc_data_)
  {
    alloc_data_ += 128;

    if (alloc_data_ == 128)
      dptr = (short *)malloc(sizeof(short) * alloc_data_);
    else
      dptr = (short *)realloc(data_, sizeof(short) * alloc_data_);

    if (dptr == NULL)
      return (NULL);

    data_ = dptr;
  }

  // Store the new data value and return
  data_[num_data_++] = d;
  data_[num_data_]   = END;

  return (data_ + num_data_ - 1);
}


/**
  Finds an icon that matches the given filename and file type.
  \param[in] filename name of file
  \param[in] filetype enumerated file type
  \return matching file icon or NULL
*/
fltk3::FileIcon *				// O - Matching file icon or NULL
fltk3::FileIcon::find(const char *filename,// I - Name of file */
                   int        filetype)	// I - Enumerated file type
{
  fltk3::FileIcon	*current;		// Current file in list
#ifndef WIN32
  struct stat	fileinfo;		// Information on file
#endif // !WIN32
  const char	*name;			// Base name of filename


  // Get file information if needed...
  if (filetype == ANY)
  {
#ifdef WIN32
    if (filename[strlen(filename) - 1] == '/')
      filetype = DIRECTORY;
    else if (fltk3::filename_isdir(filename))
      filetype = DIRECTORY;
    else
      filetype = PLAIN;
#else
    if (!fltk3::stat(filename, &fileinfo))
    {
      if (S_ISDIR(fileinfo.st_mode))
        filetype = DIRECTORY;
#  ifdef S_IFIFO
      else if (S_ISFIFO(fileinfo.st_mode))
        filetype = FIFO;
#  endif // S_IFIFO
#  if defined(S_ICHR) && defined(S_IBLK)
      else if (S_ISCHR(fileinfo.st_mode) || S_ISBLK(fileinfo.st_mode))
        filetype = DEVICE;
#  endif // S_ICHR && S_IBLK
#  ifdef S_ILNK
      else if (S_ISLNK(fileinfo.st_mode))
        filetype = LINK;
#  endif // S_ILNK
      else
        filetype = PLAIN;
    }
    else
      filetype = PLAIN;
#endif // WIN32
  }

  // Look at the base name in the filename
  name = fltk3::filename_name(filename);

  // Loop through the available file types and return any match that
  // is found...
  for (current = first_; current != (fltk3::FileIcon *)0; current = current->next_)
    if ((current->type_ == filetype || current->type_ == ANY) &&
        (fltk3::filename_match(filename, current->pattern_) ||
	 fltk3::filename_match(name, current->pattern_)))
      break;

  // Return the match (if any)...
  return (current);
}

/**
  Draws an icon in the indicated area.
  \param[in] x, y, w, h position and size
  \param[in] ic icon color
  \param[in] active status, default is active [non-zero]
*/
void
fltk3::FileIcon::draw(int      x,		// I - Upper-lefthand X
        	   int      y,		// I - Upper-lefthand Y
		   int      w,		// I - Width of bounding box
		   int      h,		// I - Height of bounding box
        	   fltk3::Color ic,		// I - Icon color...
        	   int      active)	// I - Active or inactive?
{
  fltk3::Color	c,		// Current color
		oc;		// Outline color
  short		*d,		// Pointer to data
		*dend;		// End of data...
  short		*prim;		// Pointer to start of primitive...
  double	scale;		// Scale of icon


  // Don't try to draw a NULL array!
  if (num_data_ == 0)
    return;

  // Setup the transform matrix as needed...
  scale = w < h ? w : h;

  fltk3::push_matrix();
  fltk3::translate((float)x + 0.5 * ((float)w - scale),
               (float)y + 0.5 * ((float)h + scale));
  fltk3::scale(scale, -scale);

  // Loop through the array until we see an unmatched END...
  d    = data_;
  dend = data_ + num_data_;
  prim = NULL;
  c    = ic;

  if (active)
    fltk3::color(c);
  else
    fltk3::color(fltk3::inactive(c));

  while (d < dend)
    switch (*d)
    {
      case END :
          if (prim)
            switch (*prim)
	    {
	      case LINE :
		  fltk3::end_line();
		  break;

	      case CLOSEDLINE :
		  fltk3::end_loop();
		  break;

	      case POLYGON :
		  fltk3::end_complex_polygon();
		  break;

	      case OUTLINEPOLYGON :
		  fltk3::end_complex_polygon();

        	  oc = (fltk3::Color)((((unsigned short *)prim)[1] << 16) | 
	                	  ((unsigned short *)prim)[2]);
                  if (active)
		  {
                    if (oc == fltk3::ICON_COLOR)
		      fltk3::color(ic);
		    else
		      fltk3::color(oc);
		  }
		  else
		  {
                    if (oc == fltk3::ICON_COLOR)
		      fltk3::color(fltk3::inactive(ic));
		    else
		      fltk3::color(fltk3::inactive(oc));
		  }

		  fltk3::begin_loop();

		  prim += 3;
		  while (*prim == VERTEX)
		  {
		    fltk3::vertex(prim[1] * 0.0001, prim[2] * 0.0001);
		    prim += 3;
		  }

        	  fltk3::end_loop();
		  fltk3::color(c);
		  break;
	    }

          prim = NULL;
	  d ++;
	  break;

      case COLOR :
          c = (fltk3::Color)((((unsigned short *)d)[1] << 16) | 
	                   ((unsigned short *)d)[2]);

          if (c == fltk3::ICON_COLOR)
	    c = ic;

          if (!active)
	    c = fltk3::inactive(c);

          fltk3::color(c);
	  d += 3;
	  break;

      case LINE :
          prim = d;
	  d ++;
	  fltk3::begin_line();
	  break;

      case CLOSEDLINE :
          prim = d;
	  d ++;
	  fltk3::begin_loop();
	  break;

      case POLYGON :
          prim = d;
	  d ++;
	  fltk3::begin_complex_polygon();
	  break;

      case OUTLINEPOLYGON :
          prim = d;
	  d += 3;
	  fltk3::begin_complex_polygon();
	  break;

      case VERTEX :
          if (prim)
	    fltk3::vertex(d[1] * 0.0001, d[2] * 0.0001);
	  d += 3;
	  break;

      default : // Ignore invalid data...
          d ++;
    }

  // If we still have an open primitive, close it...
  if (prim)
    switch (*prim)
    {
      case LINE :
	  fltk3::end_line();
	  break;

      case CLOSEDLINE :
	  fltk3::end_loop();
	  break;

      case POLYGON :
	  fltk3::end_polygon();
	  break;

      case OUTLINEPOLYGON :
	  fltk3::end_polygon();

          oc = (fltk3::Color)((((unsigned short *)prim)[1] << 16) | 
	                  ((unsigned short *)prim)[2]);
          if (active)
	  {
            if (oc == fltk3::ICON_COLOR)
	      fltk3::color(ic);
	    else
	      fltk3::color(oc);
	  }
	  else
	  {
            if (oc == fltk3::ICON_COLOR)
	      fltk3::color(fltk3::inactive(ic));
	    else
	      fltk3::color(fltk3::inactive(oc));
	  }

	  fltk3::begin_loop();

	  prim += 3;
	  while (*prim == VERTEX)
	  {
	    fltk3::vertex(prim[1] * 0.0001, prim[2] * 0.0001);
	    prim += 3;
	  }

          fltk3::end_loop();
	  fltk3::color(c);
	  break;
    }

  // Restore the transform matrix
  fltk3::pop_matrix();
}

/**
  Applies the icon to the widget, registering the fltk3::FileIcon
  label type as needed.
  \param[in] w widget for which this icon will become the label
*/
void fltk3::FileIcon::label(fltk3::Widget *w)	// I - Widget to label
{
  w->label(fltk3::ICON_LABEL, (const char*)this);
}


/**
  Draw the icon label.
  \param[in] o label data
  \param[in] x, y, w, h position and size of label
  \param[in] a label alignment [not used]
*/
void fl_icon_labeltype(const fltk3::Label *o,	// I - Label data
                	int            x,	// I - X position of label
			int            y,	// I - Y position of label
			int            w,	// I - Width of label
			int            h,	// I - Height of label
			fltk3::Align       a)	// I - Label alignment (not used)
{
  fltk3::FileIcon *icon;			// Pointer to icon data


  (void)a;

  icon = (fltk3::FileIcon *)(o->value);
  if (icon) icon->draw(x, y, w, h, (fltk3::Color)(o->color));
}


//
// End of "$Id$".
//
