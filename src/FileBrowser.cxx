//
// "$Id$"
//
// FileBrowser routines.
//
// Copyright 1999-2005 by Michael Sweet.
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
//   FileBrowser::full_height()     - Return the height of the list.
//   FileBrowser::item_height()     - Return the height of a list item.
//   FileBrowser::item_width()      - Return the width of a list item.
//   FileBrowser::item_draw()       - Draw a list item.
//   FileBrowser::FileBrowser() - Create a FileBrowser widget.
//   FileBrowser::load()            - Load a directory into the browser.
//   FileBrowser::filter()          - Set the filename filter.
//

//
// Include necessary header files...
//

#include <fltk/FileBrowser.h>
#include <fltk/Browser.h>
#include <fltk/draw.h>
#include <fltk/Color.h>
#include <fltk/Flags.h>
#include <fltk/Font.h>
#include <fltk/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __CYGWIN__
#  include <mntent.h>
#elif defined(WIN32)
#  include <windows.h>
#  include <direct.h>
// Apparently Borland C++ defines DIRECTORY in <direct.h>, which
// interfers with the FileIcon enumeration of the same name.
#  ifdef DIRECTORY
#    undef DIRECTORY
#  endif // DIRECTORY
#endif // __CYGWIN__

#ifdef __EMX__
#  define  INCL_DOS
#  define  INCL_DOSMISC
#  include <os2.h>
#endif // __EMX__

// CodeWarrior (__MWERKS__) gets its include paths confused, so we
// temporarily disable this...
#if defined(__APPLE__) && !defined(__MWERKS__)
#  include <sys/param.h>
#  include <sys/ucred.h>
#  include <sys/mount.h>
#endif // __APPLE__ && !__MWERKS__

using namespace fltk;

//
// BLINE definition from "Browser.cxx"...
//

#define SELECTED 1
#define NOTDISPLAYED 2

struct BLINE			// data is in a linked list of these
{
  BLINE	*prev;		// Previous item in list
  BLINE	*next;		// Next item in list
  void		*data;		// Pointer to data (function)
  short		length;		// sizeof(txt)-1, may be longer than string
  char		flags;		// selected, displayed
  char		txt[1];		// start of allocated array
};


//
// 'FileBrowser::full_height()' - Return the height of the list.
//

int					// O - Height in pixels
FileBrowser::full_height() const
{
  int	i,				// Looping var
	th;				// Total height of list.

  for (i = 0, th = 0; i < size(); i ++)
    th += (int) (textsize()+leading()) ; //item_height(find_line(i));
  return (th);
}


//
// 'FileBrowser::item_height()' - Return the height of a list item.
//

int					// O - Height in pixels
FileBrowser::item_height(void *p) const	// I - List item data
{
  BLINE	*line;			// Pointer to line
  char		*t;			// Pointer into text
  int		height;			// Width of line
  int		textheight;		// Height of text


  // Figure out the standard text height...
  setfont(textfont(), textsize());
  textheight = int(textsize()+leading());

  // We always have at least 1 line...
  height = textheight;

  // Scan for newlines...
  line = (BLINE *)p;

  if (line != NULL)
    for (t = line->txt; *t != '\0'; t ++)
      if (*t == '\n')
	height += textheight;

  // If we have enabled icons then add space for them...
  if (FileIcon::first() != NULL && height < icon_size_)
    height = icon_size_;

  // Add space for the selection border..
  height += 2;

  // Return the height
  return (height);
}


//
// 'FileBrowser::item_width()' - Return the width of a list item.
//

int					// O - Width in pixels
FileBrowser::item_width(void *p) const	// I - List item data
{
  int		i;			// Looping var
  BLINE	*line;			// Pointer to line
  char		*t,			// Pointer into text
		*ptr,			// Pointer into fragment
		fragment[10240];	// Fragment of text
  int		width,			// Width of line
		tempwidth;		// Width of fragment
  int		column;			// Current column
  const int	*columns;		// Columns


  // Set the font and size...
  setfont(textfont(), textsize());

  // Scan for newlines...
  line    = (BLINE *)p;
  columns = column_widths();

  if (strchr(line->txt, '\n') == NULL &&
      strchr(line->txt, '\t') == NULL)
  {
    // Do a fast width calculation...
    width = (int) getwidth(line->txt);
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

	tempwidth += (int)getwidth(fragment);

        // Update the max width as needed...
	if (tempwidth > width)
	  width = tempwidth;

        // Point back to the start of the fragment...
	ptr       = fragment;
	tempwidth = 0;
	column    = 0;
      }
      else if (*t == '\t')
      {
        // Advance to the next column...
        column ++;
	if (columns)
	{
	  for (i = 0, tempwidth = 0; i < column && columns[i]; i ++)
	    tempwidth += columns[i];
	}
	else
          tempwidth = column * (int)((textsize()+leading()) * 0.6 * 8.0);

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

      tempwidth += (int)getwidth(fragment);

      // Update the max width as needed...
      if (tempwidth > width)
	width = tempwidth;
    }
  }

  // If we have enabled icons then add space for them...
  if (FileIcon::first() != NULL)
    width += icon_size_ + 8;

  // Add space for the selection border..
  width += 2;

  // Return the width
  return (width);
}


//
// 'FileBrowser::item_draw()' - Draw a list item.
//

void
FileBrowser::item_draw(void *p,	// I - List item data
                 	   int  X,	// I - Upper-lefthand X coordinate
		 	   int  Y,	// I - Upper-lefthand Y coordinate
		 	   int  W,	// I - Width of item
			   int) const	// I - Height of item
{
  int		i;			// Looping var
  BLINE	*line;			// Pointer to line
  Color	c;			// Text color
  char		*t,			// Pointer into text
		*ptr,			// Pointer into fragment
		fragment[10240];	// Fragment of text
  int		width,			// Width of line
		height;			// Height of line
  int		column;			// Current column
  const int	*columns;		// Columns


  // Draw the list item text...
  line = (BLINE *)p;

  if (line->txt[strlen(line->txt) - 1] == '/')
    setfont(textfont()->bold(), textsize());
  else
    setfont(textfont(), textsize());

  if (line->flags & SELECTED)
      c = fltk::contrast(textcolor(), selection_color());
  else
    c = textcolor();

  if (FileIcon::first() == NULL)
  {
    // No icons, just draw the text...
    X ++;
    W -= 2;
  }
  else
  {
    // Draw the icon if it is set...
    if (line->data)
      ((FileIcon *)line->data)->draw(X, Y, icon_size_, icon_size_,
                                	(line->flags & SELECTED) ? fltk::YELLOW :
				                                   fltk::GRAY90,
					active_r());

    // Draw the text offset to the right...
    X += icon_size_ + 9;
    W -= icon_size_ - 10;

    // Center the text vertically...
    height = (int) (textsize()+leading());

    for (t = line->txt; *t != '\0'; t ++)
      if (*t == '\n')
	height += (int) (textsize()+leading());

    if (height < icon_size_)
      Y += (icon_size_ - height) / 2;
  }

  // Draw the text...
  line    = (BLINE *)p;
  columns = column_widths();
  width   = 0;
  column  = 0;

  if (active_r())
    setcolor(c);
  else
    setcolor(inactive(c));

  for (t = line->txt, ptr = fragment; *t != '\0'; t ++)
    if (*t == '\n')
    {
      // Newline - nul terminate this fragment and draw it...
      *ptr = '\0';

      drawtext(fragment, Rectangle(X + width, Y, W - width, (int) (textsize()+leading())),
	fltk::ALIGN_LEFT | fltk::ALIGN_CLIP);

      // Point back to the start of the fragment...
      ptr    = fragment;
      width  = 0;
      Y      += (int) (textsize()+leading());
      column = 0;
    }
    else if (*t == '\t')
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

      drawtext(fragment, Rectangle(X + width, Y, cW, (int) (textsize()+leading())),
              fltk::ALIGN_LEFT | fltk::ALIGN_CLIP);

      // Advance to the next column...
      column ++;
      if (columns)
      {
	for (i = 0, width = 0; i < column && columns[i]; i ++)
	  width += columns[i];
      }
      else
        width = column * (int)((textsize()+leading()) * 0.6 * 8.0);

      ptr = fragment;
    }
    else
      *ptr++ = *t;

  if (ptr > fragment)
  {
    // Nul terminate this fragment and draw it...
    *ptr = '\0';

    drawtext(fragment, Rectangle(X + width, Y, W - width, (int) (textsize()+leading())),
	fltk::ALIGN_LEFT | fltk::ALIGN_CLIP);
  }
}


//
// 'FileBrowser::FileBrowser()' - Create a FileBrowser widget.
//

FileBrowser::FileBrowser(int        X,  // I - Upper-lefthand X coordinate
                        	 int        Y,  // I - Upper-lefthand Y coordinate
				 int        W,  // I - Width in pixels
				 int        H,  // I - Height in pixels
				 const char *l)	// I - Label text
 : Browser(X, Y, W, H, l)
{
  // Initialize the filter pattern, current directory, and icon size...
  pattern_   = "*";
  directory_ = "";
  icon_size_  = (uchar)(3 * textsize() / 2);
  filetype_  = FILES;
}


//
// 'FileBrowser::load()' - Load a directory into the browser.
//

int						// O - Number of files loaded
FileBrowser::load(const char     *directory,// I - Directory to load
                      File_Sort_F *sort)	// I - Sort function to use
{
  int		i;				// Looping var
  int		num_files;			// Number of files in directory
  int		num_dirs;			// Number of directories in list
  char		filename[4096];			// Current file
  FileIcon	*icon;				// Icon to use


//  printf("FileBrowser::load(\"%s\")\n", directory);

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
    if ((icon = FileIcon::find("any", FileIcon::DEVICE)) == NULL)
      icon = FileIcon::find("any", FileIcon::DIRECTORY);

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

	if (i < 'C')
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
#elif defined(__APPLE__) && !defined(__MWERKS__)
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
    char	line[1024];	// Input line

    //
    // Open the file that contains a list of mounted filesystems...
    //

    mtab = fopen("/etc/mnttab", "r");	// Fairly standard
    if (mtab == NULL)
      mtab = fopen("/etc/mtab", "r");	// More standard
    if (mtab == NULL)
      mtab = fopen("/etc/fstab", "r");	// Otherwise fallback to full list
    if (mtab == NULL)
      mtab = fopen("/etc/vfstab", "r");	// Alternate full list file

    if (mtab != NULL)
    {
      while (fgets(line, sizeof(line), mtab) != NULL)
      {
        if (line[0] == '#' || line[0] == '\n')
	  continue;
        if (sscanf(line, "%*s%4095s", filename) != 1)
	  continue;

        strlcat(filename, "/", sizeof(filename));

//        printf("FileBrowser::load() - adding \"%s\" to list...\n", filename);
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

    num_files = fltk::filename_list(filename, &files, sort);
#else
    num_files = fltk::filename_list(directory_, &files, sort);
#endif /* WIN32 || __EMX__ */

    if (num_files <= 0)
      return (0);

    for (i = 0, num_dirs = 0; i < num_files; i ++) {
      if (strcmp(files[i]->d_name, "./")) {
	snprintf(filename, sizeof(filename), "%s/%s", directory_,
	         files[i]->d_name);

        icon = FileIcon::find(filename);
	if ((icon && icon->type() == FileIcon::DIRECTORY) ||
	     fltk::filename_isdir(filename)) {
          num_dirs ++;
          insert(num_dirs-1, files[i]->d_name, icon);
	} else if (filetype_ == FILES &&
	           fltk::filename_match(files[i]->d_name, pattern_)) {
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
// 'FileBrowser::filter()' - Set the filename filter.
//

void
FileBrowser::filter(const char *pattern)	// I - Pattern string
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
