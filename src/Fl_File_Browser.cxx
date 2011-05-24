//
// "$Id$"
//
// fltk3::FileBrowser routines.
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
//   fltk3::FileBrowser::full_height()     - Return the height of the list.
//   fltk3::FileBrowser::item_height()     - Return the height of a list item.
//   fltk3::FileBrowser::item_width()      - Return the width of a list item.
//   fltk3::FileBrowser::item_draw()       - Draw a list item.
//   fltk3::FileBrowser::fltk3::FileBrowser() - Create a fltk3::FileBrowser widget.
//   fltk3::FileBrowser::load()            - Load a directory into the browser.
//   fltk3::FileBrowser::filter()          - Set the filename filter.
//

//
// Include necessary header files...
//

#include <fltk3/FileBrowser.h>
#include <fltk3/draw.h>
#include <fltk3/filename.h>
#include <fltk3/Image.h>	// icon
#include <stdio.h>
#include <stdlib.h>
#include "flstring.h"

#ifdef __CYGWIN__
#  include <mntent.h>
#elif defined(WIN32)
#  include <windows.h>
#  include <direct.h>
// Apparently Borland C++ defines DIRECTORY in <direct.h>, which
// interfers with the fltk3::FileIcon enumeration of the same name.
#  ifdef DIRECTORY
#    undef DIRECTORY
#  endif // DIRECTORY
#endif // __CYGWIN__

#ifdef __EMX__
#  define  INCL_DOS
#  define  INCL_DOSMISC
#  include <os2.h>
#endif // __EMX__

#if defined(__APPLE__)
#  include <sys/param.h>
#  include <sys/ucred.h>
#  include <sys/mount.h>
#endif // __APPLE__

//
// fltk3::BrowserLine_ definition from "fltk3::Browser.cxx"...
//

#define SELECTED 1
#define NOTDISPLAYED 2

// TODO -- Warning: The definition of fltk3::BrowserLine_ here is a hack.
//    fltk3::FileBrowser should not do this. PLEASE FIX.
//    fltk3::BrowserLine_ should be private to fltk3::Browser, and not re-defined here.
//    For now, make sure this struct is precisely consistent with fltk3::Browser.cxx.
//
struct fltk3::BrowserLine_			// data is in a linked list of these
{
  fltk3::BrowserLine_	*prev;		// Previous item in list
  fltk3::BrowserLine_	*next;		// Next item in list
  void		*data;		// Pointer to data (function)
  fltk3::Image      *icon;		// Pointer to optional icon
  short		length;		// sizeof(txt)-1, may be longer than string
  char		flags;		// selected, displayed
  char		txt[1];		// start of allocated array
};


//
// 'fltk3::FileBrowser::full_height()' - Return the height of the list.
//

int					// O - Height in pixels
fltk3::FileBrowser::full_height() const
{
  int	i,				// Looping var
	th;				// Total height of list.


  for (i = 0, th = 0; i < size(); i ++)
    th += item_height(find_line(i));

  return (th);
}


//
// 'fltk3::FileBrowser::item_height()' - Return the height of a list item.
//

int					// O - Height in pixels
fltk3::FileBrowser::item_height(void *p) const	// I - List item data
{
  fltk3::BrowserLine_	*line;			// Pointer to line
  char		*t;			// Pointer into text
  int		height;			// Width of line
  int		textheight;		// Height of text


  // Figure out the standard text height...
  fltk3::font(textfont(), textsize());
  textheight = fltk3::height();

  // We always have at least 1 line...
  height = textheight;

  // Scan for newlines...
  line = (fltk3::BrowserLine_ *)p;

  if (line != NULL)
    for (t = line->txt; *t != '\0'; t ++)
      if (*t == '\n')
	height += textheight;

  // If we have enabled icons then add space for them...
  if (fltk3::FileIcon::first() != NULL && height < iconsize_)
    height = iconsize_;

  // Add space for the selection border..
  height += 2;

  // Return the height
  return (height);
}


//
// 'fltk3::FileBrowser::item_width()' - Return the width of a list item.
//

int					// O - Width in pixels
fltk3::FileBrowser::item_width(void *p) const	// I - List item data
{
  int		i;			// Looping var
  fltk3::BrowserLine_	*line;			// Pointer to line
  char		*t,			// Pointer into text
		*ptr,			// Pointer into fragment
		fragment[10240];	// Fragment of text
  int		width,			// Width of line
		tempwidth;		// Width of fragment
  int		column;			// Current column
  const int	*columns;		// Columns


  // Scan for newlines...
  line    = (fltk3::BrowserLine_ *)p;
  columns = column_widths();

  // Set the font and size...
  if (line->txt[strlen(line->txt) - 1] == '/')
    fltk3::font(textfont() | fltk3::BOLD, textsize());
  else
    fltk3::font(textfont(), textsize());

  if (strchr(line->txt, '\n') == NULL &&
      strchr(line->txt, column_char()) == NULL)
  {
    // Do a fast width calculation...
    width = (int)fltk3::width(line->txt);
  }
  else
  {
    // More than 1 line or have columns; find the maximum width...
    width     = 0;
    tempwidth = 0;
    column    = 0;

    for (t = line->txt, ptr = fragment; *t != '\0'; t ++)
      if (*t == '\n')
      {
        // Newline - nul terminate this fragment and get the width...
        *ptr = '\0';

	tempwidth += (int)fltk3::width(fragment);

        // Update the max width as needed...
	if (tempwidth > width)
	  width = tempwidth;

        // Point back to the start of the fragment...
	ptr       = fragment;
	tempwidth = 0;
	column    = 0;
      }
      else if (*t == column_char())
      {
        // Advance to the next column...
        column ++;
	if (columns)
	{
	  for (i = 0, tempwidth = 0; i < column && columns[i]; i ++)
	    tempwidth += columns[i];
	}
	else
          tempwidth = column * (int)(fltk3::height() * 0.6 * 8.0);

        if (tempwidth > width)
	  width = tempwidth;

	ptr = fragment;
      }
      else
        *ptr++ = *t;

    if (ptr > fragment)
    {
      // Nul terminate this fragment and get the width...
      *ptr = '\0';

      tempwidth += (int)fltk3::width(fragment);

      // Update the max width as needed...
      if (tempwidth > width)
	width = tempwidth;
    }
  }

  // If we have enabled icons then add space for them...
  if (fltk3::FileIcon::first() != NULL)
    width += iconsize_ + 8;

  // Add space for the selection border..
  width += 2;

  // Return the width
  return (width);
}


//
// 'fltk3::FileBrowser::item_draw()' - Draw a list item.
//

void
fltk3::FileBrowser::item_draw(void *p,	// I - List item data
                 	   int  X,	// I - Upper-lefthand X coordinate
		 	   int  Y,	// I - Upper-lefthand Y coordinate
		 	   int  W,	// I - Width of item
			   int) const	// I - Height of item
{
  int		i;			// Looping var
  fltk3::BrowserLine_	*line;			// Pointer to line
  fltk3::Color	c;			// Text color
  char		*t,			// Pointer into text
		*ptr,			// Pointer into fragment
		fragment[10240];	// Fragment of text
  int		width,			// Width of line
		height;			// Height of line
  int		column;			// Current column
  const int	*columns;		// Columns


  // Draw the list item text...
  line = (fltk3::BrowserLine_ *)p;

  if (line->txt[strlen(line->txt) - 1] == '/')
    fltk3::font(textfont() | fltk3::BOLD, textsize());
  else
    fltk3::font(textfont(), textsize());

  if (line->flags & SELECTED)
    c = fltk3::contrast(textcolor(), selection_color());
  else
    c = textcolor();

  if (fltk3::FileIcon::first() == NULL)
  {
    // No icons, just draw the text...
    X ++;
    W -= 2;
  }
  else
  {
    // Draw the icon if it is set...
    if (line->data)
      ((fltk3::FileIcon *)line->data)->draw(X, Y, iconsize_, iconsize_,
                                	(line->flags & SELECTED) ? fltk3::YELLOW :
				                                   fltk3::LIGHT2,
					active_r());

    // Draw the text offset to the right...
    X += iconsize_ + 9;
    W -= iconsize_ - 10;

    // Center the text vertically...
    height = fltk3::height();

    for (t = line->txt; *t != '\0'; t ++)
      if (*t == '\n')
	height += fltk3::height();

    if (height < iconsize_)
      Y += (iconsize_ - height) / 2;
  }

  // Draw the text...
  line    = (fltk3::BrowserLine_ *)p;
  columns = column_widths();
  width   = 0;
  column  = 0;

  if (active_r())
    fltk3::color(c);
  else
    fltk3::color(fltk3::inactive(c));

  for (t = line->txt, ptr = fragment; *t != '\0'; t ++)
    if (*t == '\n')
    {
      // Newline - nul terminate this fragment and draw it...
      *ptr = '\0';

      fltk3::draw(fragment, X + width, Y, W - width, fltk3::height(),
              (fltk3::Align)(fltk3::ALIGN_LEFT | fltk3::ALIGN_CLIP), 0, 0);

      // Point back to the start of the fragment...
      ptr    = fragment;
      width  = 0;
      Y      += fltk3::height();
      column = 0;
    }
    else if (*t == column_char())
    {
      // Tab - nul terminate this fragment and draw it...
      *ptr = '\0';

      int cW = W - width; // Clip width...

      if (columns)
      {
        // Try clipping inside this column...
	for (i = 0; i < column && columns[i]; i ++);

        if (columns[i])
          cW = columns[i];
      }

      fltk3::draw(fragment, X + width, Y, cW, fltk3::height(),
              (fltk3::Align)(fltk3::ALIGN_LEFT | fltk3::ALIGN_CLIP), 0, 0);

      // Advance to the next column...
      column ++;
      if (columns)
      {
	for (i = 0, width = 0; i < column && columns[i]; i ++)
	  width += columns[i];
      }
      else
        width = column * (int)(fltk3::height() * 0.6 * 8.0);

      ptr = fragment;
    }
    else
      *ptr++ = *t;

  if (ptr > fragment)
  {
    // Nul terminate this fragment and draw it...
    *ptr = '\0';

    fltk3::draw(fragment, X + width, Y, W - width, fltk3::height(),
            (fltk3::Align)(fltk3::ALIGN_LEFT | fltk3::ALIGN_CLIP), 0, 0);
  }
}


//
// 'fltk3::FileBrowser::fltk3::FileBrowser()' - Create a fltk3::FileBrowser widget.
//

fltk3::FileBrowser::FileBrowser(int        X,  // I - Upper-lefthand X coordinate
                        	 int        Y,  // I - Upper-lefthand Y coordinate
				 int        W,  // I - Width in pixels
				 int        H,  // I - Height in pixels
				 const char *l)	// I - Label text
    : fltk3::Browser(X, Y, W, H, l)
{
  // Initialize the filter pattern, current directory, and icon size...
  pattern_   = "*";
  directory_ = "";
  iconsize_  = (uchar)(3 * textsize() / 2);
  filetype_  = FILES;
}


//
// 'fltk3::FileBrowser::load()' - Load a directory into the browser.
//

int						// O - Number of files loaded
fltk3::FileBrowser::load(const char     *directory,// I - Directory to load
                      fltk3::FileSortF *sort)	// I - Sort function to use
{
  int		i;				// Looping var
  int		num_files;			// Number of files in directory
  int		num_dirs;			// Number of directories in list
  char		filename[4096];			// Current file
  fltk3::FileIcon	*icon;				// Icon to use


//  printf("fltk3::FileBrowser::load(\"%s\")\n", directory);

  clear();

  directory_ = directory;

  if (!directory)
    return (0);

  if (directory_[0] == '\0')
  {
    //
    // No directory specified; for UNIX list all mount points.  For DOS
    // list all valid drive letters...
    //

    num_files = 0;
    if ((icon = fltk3::FileIcon::find("any", fltk3::FileIcon::DEVICE)) == NULL)
      icon = fltk3::FileIcon::find("any", fltk3::FileIcon::DIRECTORY);

#ifdef WIN32
#  ifdef __CYGWIN__
    //
    // Cygwin provides an implementation of setmntent() to get the list
    // of available drives...
    //
    FILE          *m = setmntent("/-not-used-", "r");
    struct mntent *p;

    while ((p = getmntent (m)) != NULL) {
      add(p->mnt_dir, icon);
      num_files ++;
    }

    endmntent(m);
#  else
    //
    // Normal WIN32 code uses drive bits...
    //
    DWORD	drives;		// Drive available bits

    drives = GetLogicalDrives();
    for (i = 'A'; i <= 'Z'; i ++, drives >>= 1)
      if (drives & 1)
      {
        sprintf(filename, "%c:/", i);

	if (i < 'C') // see also: GetDriveType and GetVolumeInformation in WIN32
	  add(filename, icon);
	else
	  add(filename, icon);

	num_files ++;
      }
#  endif // __CYGWIN__
#elif defined(__EMX__)
    //
    // OS/2 code uses drive bits...
    //
    ULONG	curdrive;	// Current drive
    ULONG	drives;		// Drive available bits
    int		start = 3;      // 'C' (MRS - dunno if this is correct!)


    DosQueryCurrentDisk(&curdrive, &drives);
    drives >>= start - 1;
    for (i = 'A'; i <= 'Z'; i ++, drives >>= 1)
      if (drives & 1)
      {
        sprintf(filename, "%c:/", i);
        add(filename, icon);

	num_files ++;
      }
#elif defined(__APPLE__)
    // MacOS X and Darwin use getfsstat() system call...
    int			numfs;	// Number of file systems
    struct statfs	*fs;	// Buffer for file system info


    // We always have the root filesystem.
    add("/", icon);

    // Get the mounted filesystems...
    numfs = getfsstat(NULL, 0, MNT_NOWAIT);
    if (numfs > 0) {
      // We have file systems, get them...
      fs = new struct statfs[numfs];
      getfsstat(fs, sizeof(struct statfs) * numfs, MNT_NOWAIT);

      // Add filesystems to the list...
      for (i = 0; i < numfs; i ++) {
	// Ignore "/", "/dev", and "/.vol"...
        if (fs[i].f_mntonname[1] && strcmp(fs[i].f_mntonname, "/dev") &&
	    strcmp(fs[i].f_mntonname, "/.vol")) {
          snprintf(filename, sizeof(filename), "%s/", fs[i].f_mntonname);
          add(filename, icon);
        }
        num_files ++;
      }

      // Free the memory used for the file system info array...
      delete[] fs;
    }
#else
    //
    // UNIX code uses /etc/fstab or similar...
    //
    FILE	*mtab;		// /etc/mtab or /etc/mnttab file
    char	line[FLTK3_PATH_MAX];	// Input line

    //
    // Open the file that contains a list of mounted filesystems...
    //

    mtab = fltk3::fopen("/etc/mnttab", "r");	// Fairly standard
    if (mtab == NULL)
      mtab = fltk3::fopen("/etc/mtab", "r");	// More standard
    if (mtab == NULL)
      mtab = fltk3::fopen("/etc/fstab", "r");	// Otherwise fallback to full list
    if (mtab == NULL)
      mtab = fltk3::fopen("/etc/vfstab", "r");	// Alternate full list file

    if (mtab != NULL)
    {
      while (fgets(line, sizeof(line), mtab) != NULL)
      {
        if (line[0] == '#' || line[0] == '\n')
	  continue;
        if (sscanf(line, "%*s%4095s", filename) != 1)
	  continue;

        strlcat(filename, "/", sizeof(filename));

//        printf("fltk3::FileBrowser::load() - adding \"%s\" to list...\n", filename);
        add(filename, icon);
	num_files ++;
      }

      fclose(mtab);
    }
#endif // WIN32 || __EMX__
  }
  else
  {
    dirent	**files;	// Files in in directory


    //
    // Build the file list...
    //

#if (defined(WIN32) && !defined(__CYGWIN__)) || defined(__EMX__)
    strlcpy(filename, directory_, sizeof(filename));
    i = strlen(filename) - 1;

    if (i == 2 && filename[1] == ':' &&
        (filename[2] == '/' || filename[2] == '\\'))
      filename[2] = '/';
    else if (filename[i] != '/' && filename[i] != '\\')
      strlcat(filename, "/", sizeof(filename));

    num_files = fl_filename_list(filename, &files, sort);
#else
    num_files = fl_filename_list(directory_, &files, sort);
#endif /* WIN32 || __EMX__ */

    if (num_files <= 0)
      return (0);

    for (i = 0, num_dirs = 0; i < num_files; i ++) {
      if (strcmp(files[i]->d_name, "./")) {
	snprintf(filename, sizeof(filename), "%s/%s", directory_,
	         files[i]->d_name);

        icon = fltk3::FileIcon::find(filename);
	if ((icon && icon->type() == fltk3::FileIcon::DIRECTORY) ||
	     _fl_filename_isdir_quick(filename)) {
          num_dirs ++;
          insert(num_dirs, files[i]->d_name, icon);
	} else if (filetype_ == FILES &&
	           fl_filename_match(files[i]->d_name, pattern_)) {
          add(files[i]->d_name, icon);
	}
      }

      free(files[i]);
    }

    free(files);
  }

  return (num_files);
}


//
// 'fltk3::FileBrowser::filter()' - Set the filename filter.
//

void
fltk3::FileBrowser::filter(const char *pattern)	// I - Pattern string
{
  // If pattern is NULL set the pattern to "*"...
  if (pattern)
    pattern_ = pattern;
  else
    pattern_ = "*";
}


//
// End of "$Id$".
//
