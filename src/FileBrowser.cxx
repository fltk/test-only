//
// "$Id$"
//
// FileBrowser routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1997-1999 by Easy Software Products.
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
// Contents:
//
//   FileBrowser::item_width()  - Return the width of a list item.
//   FileBrowser::item_draw()   - Draw a list item.
//   FileBrowser::FileBrowser() - Create a FileBrowser widget.
//   FileBrowser::load()        - Load a directory into the browser.
//   FileBrowser::filter()      - Set the filename filter.
//

//
// Include necessary header files...
//

#include <fltk/FileBrowser.h>
#include <fltk/draw.h>
#include <fltk/filename.h>
#include <fltk/string.h>
#include <fltk/utf.h>
#include <stdlib.h>
#include <config.h>

#if defined(_WIN32)
#  include <windows.h>
#  ifndef __CYGWIN__
#    include <direct.h>
#  endif
#endif /* _WIN32 */

#if defined(__EMX__)
#define  INCL_DOS
#define  INCL_DOSMISC
#include <os2.h>
#endif /* __EMX__ */

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
// 'FileBrowser::item_height()' - Return the height of a list item.
//

int					// O - Height in pixels
FileBrowser::item_height(void *p) const	// I - List item data
{
  BLINE	*line;			// Pointer to line
  char		*text;			// Pointer into text
  int		height;			// Width of line
  int		textheight;		// Height of text


  // Figure out the standard text height...
  textheight = int(textsize()+leading());

  // We always have at least 1 line...
  height = textheight;

  // Scan for newlines...
  line = (BLINE *)p;

  if (line != NULL)
    for (text = line->txt; *text != '\0'; text ++)
      if (*text == '\n')
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
  BLINE	*line;			// Pointer to line
  char		*text,			// Pointer into text
		*ptr,			// Pointer into fragment
		fragment[10240];	// Fragment of text
  double	width,			// Width of line
		tempwidth;		// Width of fragment
  int		column;			// Current column


  // Set the font and size...
  setfont(textfont(), textsize());

  // Scan for newlines...
  line = (BLINE *)p;

  if (strchr(line->txt, '\n') == NULL &&
      strchr(line->txt, '\t') == NULL)
  {
    // Do a fast width calculation...
    width = getwidth(line->txt);
  }
  else
  {
    // More than 1 line or have columns; find the maximum width...
    width     = 0;
    tempwidth = 0;
    column    = 0;

    for (text = line->txt, ptr = fragment; *text != '\0'; text ++)
      if (*text == '\n')
      {
        // Newline - nul terminate this fragment and get the width...
        *ptr = '\0';

	tempwidth += getwidth(fragment);

        // Update the max width as needed...
	if (tempwidth > width)
	  width = tempwidth;

        // Point back to the start of the fragment...
	ptr       = fragment;
	tempwidth = 0;
      }
      else if (*text == '\t')
      {
        // Advance to the next column...
        column ++;
        tempwidth = column * getwidth("        ");

        if (tempwidth > width)
	  width = tempwidth;

	ptr = fragment;
      }
      else
        *ptr++ = *text;

    if (ptr > fragment)
    {
      // Nul terminate this fragment and get the width...
      *ptr = '\0';

      tempwidth += getwidth(fragment);

      // Update the max width as needed...
      if (tempwidth > width)
	width = tempwidth;
    }
  }

  // If we have enabled icons then add space for them...
  if (FileIcon::first() != NULL)
    width += icon_size_ + 8;

  // Return the width, including space for the selection border:
  return int(width+2.5);
}


//
// 'FileBrowser::item_draw()' - Draw a list item.
//

void
FileBrowser::item_draw(void *p,	// I - List item data
                          int  x,	// I - Upper-lefthand X coordinate
	 	          int  y,	// I - Upper-lefthand Y coordinate
		          int  w,	// I - Width of item
		          int  h) const	// I - Height of item
{
  Color	c;			// Color of text
  BLINE	*line;			// Pointer to line


  puts("FileBrowser::item_draw()");
  // Draw the list item text...
  line = (BLINE *)p;

  setfont(textfont(), textsize());
  if (line->flags & SELECTED)
    c = contrast(textcolor(), selection_color());
  else
    c = textcolor();

  setcolor(active_r() ? c : inactive(c));

  if (FileIcon::first() == NULL)
  {
    // No icons, just draw the text...
    drawtext(line->txt, Rectangle(x + 1, y, w - 2, h), ALIGN_LEFT);
  }
  else
  {
    // Icons; draw the text offset to the right...
    drawtext(line->txt,
	     Rectangle(x + icon_size_ + 9, y, w - icon_size_ - 10, h),
	     ALIGN_LEFT);

    // And then draw the icon if it is set...
    if (line->data)
      ((FileIcon *)line->data)->draw(x, y, icon_size_, icon_size_,
				     (line->flags & SELECTED) ? YELLOW :
				     GRAY90,
				     active_r());
  }
}


//
// 'FileBrowser::FileBrowser()' - Create a FileBrowser widget.
//

FileBrowser::FileBrowser(int        x,	// I - Upper-lefthand X coordinate
                               int        y,	// I - Upper-lefthand Y coordinate
			       int        w,	// I - Width in pixels
			       int        h,	// I - Height in pixels
			       const char *l)	// I - Label text
    : Browser(x, y, w, h, l)
{
  // Initialize the filter pattern, current directory, and icon size...
  pattern_   = "*";
  directory_ = "";
  icon_size_ = 20; // This looks best for the default icons, if loaded...
}


//
// 'FileBrowser::load()' - Load a directory into the browser.
//

int					// O - Number of files loaded
FileBrowser::load(const char *directory)// I - Directory to load
{
  int		i;		// Looping var
  int		num_files;	// Number of files in directory
  char		filename[4096];	// Current file
  FileIcon	*icon;		// Icon to use


  clear();
  directory_ = directory;

  if (directory_[0] == '\0')
  {
    //
    // No directory specified; for UNIX list all mount points.  For DOS
    // list all valid drive letters...
    //

    num_files = 0;
    icon      = FileIcon::find("any", FileIcon::DEVICE);

    if (icon == (FileIcon *)0)
      icon = FileIcon::find("any", FileIcon::DIR);

#if defined(_WIN32)
    DWORD	drives;		// Drive available bits

    drives = GetLogicalDrives();
    for (i = 'A'; i <= 'Z'; i ++, drives >>= 1)
      if (drives & 1)
      {
        sprintf(filename, "%c:", i);
        add(filename, icon);

	num_files ++;
      }
#elif defined(__EMX__)
    ULONG	curdrive;	// Current drive
    ULONG	drives;		// Drive available bits
    int		start = 3;      // 'C' (MRS - dunno if this is correct!)


    DosQueryCurrentDisk(&curdrive, &drives);
    drives >>= start - 1;
    for (i = 'A'; i <= 'Z'; i ++, drives >>= 1)
      if (drives & 1)
      {
        sprintf(filename, "%c:", i);
        add(filename, icon);

	num_files ++;
      }
#else
    FILE	*mtab;		// /etc/mtab or /etc/mnttab file
    char	line[1024];	// Input line

    //
    // Open the file that contains a list of mounted filesystems...
    //
#  if defined(__hpux) || defined(__sun)
    mtab = fopen("/etc/mnttab", "r");	// Fairly standard
#  elif defined(__sgi) || defined(linux)
    mtab = fopen("/etc/mtab", "r");	// More standard
#  else
    mtab = fopen("/etc/fstab", "r");	// Otherwise fallback to full list
    if (mtab == NULL)
      mtab = fopen("/etc/vfstab", "r");
#  endif

    if (mtab != NULL)
    {
      while (fgets(line, sizeof(line), mtab) != NULL)
      {
        if (line[0] == '#' || line[0] == '\n')
	  continue;
        if (sscanf(line, "%*s%4095s", filename) != 1)
	  continue;

        add(filename, icon);
	num_files ++;
      }

      fclose(mtab);
    }
#endif // _WIN32
  }
  else
  {
    dirent	**files;	// Files in in directory


    //
    // Build the file list...
    //

#if defined(_WIN32) || defined(__EMX__)
    strncpy(filename, directory_, sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = '\0';

    i = strlen(filename) - 1;

    if (i == 2 && filename[1] == ':' &&
        (filename[2] == '/' || filename[2] == '\\'))
      filename[2] = '/';
    else if (filename[i] != '/' && filename[i] != '\\')
      strcat(filename, "/");

    num_files = filename_list(filename, &files);
#else
    num_files = filename_list(directory_, &files);
#endif /* _WIN32 || __EMX__ */

    if (num_files <= 0)
      return (0);

    // Add directories first...
    for (i = 0; i < num_files; i ++)
      if (strcmp(files[i]->d_name, ".") != 0 &&
          strcmp(files[i]->d_name, "..") != 0)
      {
        char namebuf[FL_PATH_MAX];
        memset(namebuf, 0, sizeof(namebuf));
        utf8frommb(namebuf, sizeof(namebuf), files[i]->d_name, strlen(files[i]->d_name));
	snprintf(filename, sizeof(filename), "%s/%s", directory_, namebuf);

	if (filename_isdir(filename)) {
          add(namebuf, FileIcon::find(filename));
        }
      }

    for (i = 0; i < num_files; i ++)
    {
      if (strcmp(files[i]->d_name, ".") != 0 &&
          strcmp(files[i]->d_name, "..") != 0)
      {
        char namebuf[FL_PATH_MAX];
        memset(namebuf, 0, sizeof(namebuf));
        utf8frommb(namebuf, sizeof(namebuf), files[i]->d_name, strlen(files[i]->d_name));
	snprintf(filename, sizeof(filename), "%s/%s", directory_, namebuf);

	if (!filename_isdir(filename) &&
            filename_match(namebuf, pattern_)) {
          add(namebuf, FileIcon::find(filename));
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

  // Reload the current directory...
  load(directory_);
}


//
// End of "$Id$".
//
