//
// "$Id: Fl_FileIcon2.cxx,v 1.15 2002/12/15 10:42:53 spitzak Exp $"
//
// FileIcon loading routines for the Fast Light Tool Kit (FLTK).
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
//   FileIcon::load()              - Load an icon file...
//   FileIcon::load_fti()          - Load an SGI-format FTI file...
//   FileIcon::load_xpm()          - Load an XPM icon file...
//   FileIcon::load_system_icons() - Load the standard system icons/filetypes.
//

//
// Include necessary header files...
//

#include <fltk/FileIcon.h>
#include <fltk/filename.h>
#include <fltk/draw.h>
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(_WIN32)
# include <io.h>
# define access(a,b) _access(a,b)
# define F_OK 04
#else
# include <unistd.h>
#endif

using namespace fltk;

//
// Local functions...
//

static void	load_kde_icons(const char *directory);
static void	load_kde_mimelnk(const char *filename);
static char	*kde_to_fltk_pattern(const char *kdepattern);
static char	*get_kde_val(char *str, const char *key);


//
// 'FileIcon::load()' - Load an icon file...
//

void
FileIcon::load(const char *f)	// I - File to read from
{
  const char	*ext;			// File extension


  if ((ext = filename_ext(f)) == NULL)
    load_xpm(f);
  else if (strcmp(ext, ".fti") == 0)
    load_fti(f);
#if 0
  else if (strcmp(ext, ".png") == 0)
    load_png(f);
#endif /* 0 */
  else
    load_xpm(f);
}


//
// 'FileIcon::load_fti()' - Load an SGI-format FTI file...
//

void
FileIcon::load_fti(const char *fti)	// I - File to read from
{
  FILE	*fp;			// File pointer
  int	ch;			// Current character
  char	command[255],		// Command string ("vertex", etc.)
	params[255],		// Parameter string ("10.0,20.0", etc.)
	*ptr;			// Pointer into strings
  int	outline;		// Outline polygon


  // Try to open the file...
  if ((fp = fopen(fti, "rb")) == NULL)
  {
    fprintf(stderr, "FileIcon::load_fti(): Unable to open \"%s\" - %s\n",
            fti, strerror(errno));
    return;
  }

  // Read the entire file, adding data as needed...
  outline = 0;

  while ((ch = getc(fp)) != EOF)
  {
    // Skip whitespace
    if (isspace(ch))
      continue;

    // Skip comments starting with "#"...
    if (ch == '#')
    {
      while ((ch = getc(fp)) != EOF)
        if (ch == '\n')
	  break;

      if (ch == EOF)
        break;
      else
        continue;
    }

    // OK, this character better be a letter...
    if (!isalpha(ch))
    {
      fprintf(stderr, "FileIcon::load_fti(): Expected a letter at file position %ld (saw '%c')\n",
              ftell(fp) - 1, ch);
      break;
    }

    // Scan the command name...
    ptr    = command;
    *ptr++ = ch;

    while ((ch = getc(fp)) != EOF)
    {
      if (ch == '(')
        break;
      else if (ptr < command + sizeof(command) - 1)
        *ptr++ = ch;
    }

    *ptr++ = '\0';

    // Make sure we stopped on a parenthesis...
    if (ch != '(')
    {
      fprintf(stderr, "FileIcon::load_fti(): Expected a ( at file position %ld (saw '%c')\n",
              ftell(fp) - 1, ch);
      break;
    }

    // Scan the parameters...
    ptr = params;

    while ((ch = getc(fp)) != EOF)
    {
      if (ch == ')')
        break;
      else if (ptr < command + sizeof(command) - 1)
        *ptr++ = ch;
    }

    *ptr++ = '\0';

    // Make sure we stopped on a parenthesis...
    if (ch != ')')
    {
      fprintf(stderr, "FileIcon::load_fti(): Expected a ) at file position %ld (saw '%c')\n",
              ftell(fp) - 1, ch);
      break;
    }

    // Make sure the next character is a semicolon...
    if ((ch = getc(fp)) != ';')
    {
      fprintf(stderr, "FileIcon::load_fti(): Expected a ; at file position %ld (saw '%c')\n",
              ftell(fp) - 1, ch);
      break;
    }

    // Now process the command...
    if (strcmp(command, "color") == 0)
    {
      // Set the color; for negative colors blend the two primaries to
      // produce a composite color.  Also, the following symbolic color
      // names are understood:
      //
      //     name           FLTK color
      //     -------------  ----------
      //     iconcolor      256; mapped to the icon color in FileIcon::draw()
      //     shadowcolor    GRAY33
      //     outlinecolor   BLACK
      if (strcmp(params, "iconcolor") == 0)
        add_color(256);
      else if (strcmp(params, "shadowcolor") == 0)
        add_color(GRAY33);
      else if (strcmp(params, "outlinecolor") == 0)
        add_color(BLACK);
      else
      {
        short c = atoi(params);	// Color value


        if (c < 0)
	{
	  // Composite color; compute average...
	  c = -c;
	  add_color(lerp((Color)(c >> 4),
	                             (Color)(c & 15), 0.5));
	}
	else
	  add_color(c);
      }
    }
    else if (strcmp(command, "bgnline") == 0)
      add(LINE);
    else if (strcmp(command, "bgnclosedline") == 0)
      add(CLOSEDLINE);
    else if (strcmp(command, "bgnpolygon") == 0)
      add(POLYGON);
    else if (strcmp(command, "bgnoutlinepolygon") == 0)
    {
      add(OUTLINEPOLYGON);
      outline = add(0) - data_;
    }
    else if (strcmp(command, "endoutlinepolygon") == 0 && outline)
    {
      // Set the outline color; see above for valid values...
      if (strcmp(params, "iconcolor") == 0)
        data_[outline] = 256;
      else if (strcmp(params, "shadowcolor") == 0)
	data_[outline] = GRAY33;
      else if (strcmp(params, "outlinecolor") == 0)
        data_[outline] = BLACK;
      else
      {
        short c = atoi(params);	// Color value


        if (c < 0)
	{
	  // Composite color; compute average...
	  c = -c;
	  data_[outline] = lerp((Color)(c >> 4), (Color)(c & 15), 0.5);
	}
	else
	  data_[outline] = c;
      }

      outline = 0;
      add(END);
    }
    else if (strncmp(command, "end", 3) == 0)
      add(END);
    else if (strcmp(command, "vertex") == 0)
    {
      float x, y;		// Coordinates of vertex


      if (sscanf(params, "%f,%f", &x, &y) != 2)
        break;

      addvertex(x * 100.0f, y * 100.0f);
    }
    else
    {
      fprintf(stderr, "FileIcon::load_fti(): Unknown command \"%s\" at file position %ld.\n",
              command, ftell(fp) - 1);
      break;
    }
  }

  // Close the file and return...
  fclose(fp);

#ifdef DEBUG
  printf("Icon File \"%s\":\n", fti);
  for (int i = 0; i < num_data_; i ++)
    printf("    %d,\n", data_[i]);
#endif /* DEBUG */
}


//
// 'FileIcon::load_xpm()' - Load an XPM icon file...
//

void
FileIcon::load_xpm(const char *xpm)	// I - File to read from
{
  FILE		*fp;			// File pointer
  int		i, j;			// Looping vars
  int		ch;			// Current character
  int		bg;			// Background color
  char		line[1024],		// Line from file
		val[16],		// Color value
		*ptr;			// Pointer into line
  int		x, y;			// X & Y in image
  int		startx;			// Starting X coord
  int		width, height;		// Width and height of image
  int		ncolors;		// Number of colors
  Color		colors[256];		// Colors
  int		red, green, blue;	// Red, green, and blue values


  // Try to open the file...
  if ((fp = fopen(xpm, "rb")) == NULL)
    return;

  // Read the file header until we find the first string...
  ptr = NULL;
  while (fgets(line, sizeof(line), fp) != NULL)
    if ((ptr = strchr(line, '\"')) != NULL)
      break;

  if (ptr == NULL)
  {
    // Nothing to load...
    fclose(fp);
    return;
  }

  // Get the size of the image...
  sscanf(ptr + 1, "%d%d%d", &width, &height, &ncolors);

  // Now read the colormap...
  memset(colors, 0, sizeof(colors));
  bg = ' ';

  for (i = 0; i < ncolors; i ++)
  {
    while (fgets(line, sizeof(line), fp) != NULL)
      if ((ptr = strchr(line, '\"')) != NULL)
	break;

    if (ptr == NULL)
    {
      // Nothing to load...
      fclose(fp);
      return;
    }

    // Get the color's character
    ptr ++;
    ch = *ptr++;

    // Get the color value...
    if ((ptr = strstr(ptr, "c ")) == NULL)
    {
      // No color; make this black...
      colors[ch] = BLACK;
    }
    else if (ptr[2] == '#')
    {
      // Read the RGB triplet...
      ptr += 3;
      for (j = 0; j < 12; j ++)
        if (!isxdigit(ptr[j]))
	  break;

      switch (j)
      {
        case 0 :
	    bg = ch;
	default :
	    red = green = blue = 0;
	    break;

        case 3 :
	    val[0] = ptr[0];
	    val[1] = '\0';
	    red = 255 * strtol(val, NULL, 16) / 15;

	    val[0] = ptr[1];
	    val[1] = '\0';
	    green = 255 * strtol(val, NULL, 16) / 15;

	    val[0] = ptr[2];
	    val[1] = '\0';
	    blue = 255 * strtol(val, NULL, 16) / 15;
	    break;

        case 6 :
        case 9 :
        case 12 :
	    j /= 3;

	    val[0] = ptr[0];
	    val[1] = ptr[1];
	    val[2] = '\0';
	    red = strtol(val, NULL, 16);

	    val[0] = ptr[j + 0];
	    val[1] = ptr[j + 1];
	    val[2] = '\0';
	    green = strtol(val, NULL, 16);

	    val[0] = ptr[2 * j + 0];
	    val[1] = ptr[2 * j + 1];
	    val[2] = '\0';
	    blue = strtol(val, NULL, 16);
	    break;
      }
      colors[ch] = color(red, green, blue);
    }
    else
    {
      // Read a color name...
      if (strncasecmp(ptr + 2, "white", 5) == 0)
        colors[ch] = WHITE;
      else if (strncasecmp(ptr + 2, "black", 5) == 0)
        colors[ch] = BLACK;
      else if (strncasecmp(ptr + 2, "none", 4) == 0)
      {
        colors[ch] = BLACK;
	bg = ch;
      }
      else
        colors[ch] = GRAY75;
    }
  }

  // Read the image data...
  for (y = height - 1; y >= 0; y --)
  {
    while (fgets(line, sizeof(line), fp) != NULL)
      if ((ptr = strchr(line, '\"')) != NULL)
	break;

    if (ptr == NULL)
    {
      // Nothing to load...
      fclose(fp);
      return;
    }

    startx = 0;
    ch     = bg;
    ptr ++;

    for (x = 0; x < width; x ++, ptr ++)
      if (*ptr != ch)
      {
	if (ch != bg)
	{
          add_color(colors[ch]);
	  add(POLYGON);
	  addvertex(startx * 9000 / width + 1000, y * 9000 / height + 500);
	  addvertex(x * 9000 / width + 1000,      y * 9000 / height + 500);
	  addvertex(x * 9000 / width + 1000,      (y + 1) * 9000 / height + 500);
	  addvertex(startx * 9000 / width + 1000, (y + 1) * 9000 / height + 500);
	  add(END);
        }

	ch     = *ptr;
	startx = x;
      }

    if (ch != bg)
    {
      add_color(colors[ch]);
      add(POLYGON);
      addvertex(startx * 9000 / width + 1000, y * 9000 / height + 500);
      addvertex(x * 9000 / width + 1000,      y * 9000 / height + 500);
      addvertex(x * 9000 / width + 1000,      (y + 1) * 9000 / height + 500);
      addvertex(startx * 9000 / width + 1000, (y + 1) * 9000 / height + 500);
      add(END);
    }
  }

  // Close the file and return...
  fclose(fp);

#ifdef DEBUG
  printf("Icon File \"%s\":\n", xpm);
  for (i = 0; i < num_data_; i ++)
    printf("    %d,\n", data_[i]);
#endif /* DEBUG */
}


//
// 'FileIcon::load_system_icons()' - Load the standard system icons/filetypes.

void
FileIcon::load_system_icons(void)
{
  FileIcon	*icon;		// New icons
  static int	init = 0;	// Have the icons been initialized?
  static short	plain[] =	// Plain file icon
		{
		  COLOR, 256, OUTLINEPOLYGON, GRAY75,
		  VERTEX, 2000, 1000, VERTEX, 2000, 9000,
		  VERTEX, 6000, 9000, VERTEX, 8000, 7000,
		  VERTEX, 8000, 1000, END, OUTLINEPOLYGON, GRAY75,
		  VERTEX, 6000, 9000, VERTEX, 6000, 7000,
		  VERTEX, 8000, 7000, END,
		  COLOR, BLACK, LINE, VERTEX, 6000, 7000,
		  VERTEX, 8000, 7000, VERTEX, 8000, 1000,
		  VERTEX, 2000, 1000, END, LINE, VERTEX, 3000, 7000,
		  VERTEX, 5000, 7000, END, LINE, VERTEX, 3000, 6000,
		  VERTEX, 5000, 6000, END, LINE, VERTEX, 3000, 5000,
		  VERTEX, 7000, 5000, END, LINE, VERTEX, 3000, 4000,
		  VERTEX, 7000, 4000, END, LINE, VERTEX, 3000, 3000,
		  VERTEX, 7000, 3000, END, LINE, VERTEX, 3000, 2000,
		  VERTEX, 7000, 2000, END, 
		  END
		};
  static short	image[] =	// Image file icon
		{
		  COLOR, 256, OUTLINEPOLYGON, GRAY75,
		  VERTEX, 2000, 1000, VERTEX, 2000, 9000,
		  VERTEX, 6000, 9000, VERTEX, 8000, 7000,
		  VERTEX, 8000, 1000, END, OUTLINEPOLYGON, GRAY75,
		  VERTEX, 6000, 9000, VERTEX, 6000, 7000,
		  VERTEX, 8000, 7000, END,
		  COLOR, BLACK, LINE, VERTEX, 6000, 7000,
		  VERTEX, 8000, 7000, VERTEX, 8000, 1000,
		  VERTEX, 2000, 1000, END,
		  COLOR, RED, POLYGON, VERTEX, 3500, 2500,
		  VERTEX, 3000, 3000, VERTEX, 3000, 4000,
		  VERTEX, 3500, 4500, VERTEX, 4500, 4500,
		  VERTEX, 5000, 4000, VERTEX, 5000, 3000,
		  VERTEX, 4500, 2500, END,
		  COLOR, GREEN, POLYGON, VERTEX, 5500, 2500,
		  VERTEX, 5000, 3000, VERTEX, 5000, 4000,
		  VERTEX, 5500, 4500, VERTEX, 6500, 4500,
		  VERTEX, 7000, 4000, VERTEX, 7000, 3000,
		  VERTEX, 6500, 2500, END,
		  COLOR, BLUE, POLYGON, VERTEX, 4500, 3500,
		  VERTEX, 4000, 4000, VERTEX, 4000, 5000,
		  VERTEX, 4500, 5500, VERTEX, 5500, 5500,
		  VERTEX, 6000, 5000, VERTEX, 6000, 4000,
		  VERTEX, 5500, 3500, END,
		  END
		};
  static short	dir[] =		// Directory icon
		{
		  COLOR, 256, POLYGON, VERTEX, 1000, 1000,
		  VERTEX, 1000, 7500,  VERTEX, 9000, 7500,
		  VERTEX, 9000, 1000, END,
		  POLYGON, VERTEX, 1000, 7500, VERTEX, 2500, 9000,
		  VERTEX, 5000, 9000, VERTEX, 6500, 7500, END,
		  COLOR, WHITE, LINE, VERTEX, 1500, 1500,
		  VERTEX, 1500, 7000, VERTEX, 9000, 7000, END,
		  COLOR, BLACK, LINE, VERTEX, 9000, 7500,
		  VERTEX, 9000, 1000, VERTEX, 1000, 1000, END,
		  COLOR, GRAY75, LINE, VERTEX, 1000, 1000,
		  VERTEX, 1000, 7500, VERTEX, 2500, 9000,
		  VERTEX, 5000, 9000, VERTEX, 6500, 7500,
		  VERTEX, 9000, 7500, END,
		  END
		};


  // Add symbols if they haven't been added already...
  if (!init)
  {
    if (!access("/usr/share/mimelnk", F_OK))
    {
      // Load KDE icons...
      icon = new FileIcon("*", FileIcon::PLAIN);
      icon->load_xpm("/usr/share/icons/unknown.xpm");

      load_kde_icons("/usr/share/mimelnk");
    }
    else if (!access("/usr/share/icons/folder.xpm", F_OK))
    {
      // Load GNOME icons...
      icon = new FileIcon("*", FileIcon::PLAIN);
      icon->load_xpm("/usr/share/icons/page.xpm");

      icon = new FileIcon("*", FileIcon::DIR);
      icon->load_xpm("/usr/share/icons/folder.xpm");
    }
    else if (!access("/usr/dt/appconfig/icons", F_OK))
    {
      // Load CDE icons...
      icon = new FileIcon("*", FileIcon::PLAIN);
      icon->load_xpm("/usr/dt/appconfig/icons/C/Dtdata.m.pm");

      icon = new FileIcon("*", FileIcon::DIR);
      icon->load_xpm("/usr/dt/appconfig/icons/C/DtdirB.m.pm");

      icon = new FileIcon("core", FileIcon::PLAIN);
      icon->load_xpm("/usr/dt/appconfig/icons/C/Dtcore.m.pm");

      icon = new FileIcon("*.{bmp|bw|gif|jpg|pbm|pcd|pgm|ppm|png|ras|rgb|tif|xbm|xpm}", FileIcon::PLAIN);
      icon->load_xpm("/usr/dt/appconfig/icons/C/Dtimage.m.pm");

      icon = new FileIcon("*.{eps|pdf|ps}", FileIcon::PLAIN);
      icon->load_xpm("/usr/dt/appconfig/icons/C/Dtps.m.pm");

      icon = new FileIcon("*.ppd", FileIcon::PLAIN);
      icon->load_xpm("/usr/dt/appconfig/icons/C/DtPrtpr.m.pm");
    }
    else if (!access("/usr/lib/filetype", F_OK))
    {
      // Load SGI icons...
      icon = new FileIcon("*", FileIcon::PLAIN);
      icon->load_fti("/usr/lib/filetype/iconlib/generic.doc.fti");

      icon = new FileIcon("*", FileIcon::DIR);
      icon->load_fti("/usr/lib/filetype/iconlib/generic.folder.closed.fti");

      icon = new FileIcon("core", FileIcon::PLAIN);
      icon->load_fti("/usr/lib/filetype/default/iconlib/CoreFile.fti");

      icon = new FileIcon("*.{bmp|bw|gif|jpg|pbm|pcd|pgm|ppm|png|ras|rgb|tif|xbm|xpm}", FileIcon::PLAIN);
      icon->load_fti("/usr/lib/filetype/system/iconlib/ImageFile.fti");

      if (!access("/usr/lib/filetype/install/iconlib/acroread.doc.fti", F_OK))
      {
	icon = new FileIcon("*.{eps|ps}", FileIcon::PLAIN);
	icon->load_fti("/usr/lib/filetype/system/iconlib/PostScriptFile.closed.fti");

	icon = new FileIcon("*.pdf", FileIcon::PLAIN);
	icon->load_fti("/usr/lib/filetype/install/iconlib/acroread.doc.fti");
      }
      else
      {
	icon = new FileIcon("*.{eps|pdf|ps}", FileIcon::PLAIN);
	icon->load_fti("/usr/lib/filetype/system/iconlib/PostScriptFile.closed.fti");
      }

      if (!access("/usr/lib/filetype/install/iconlib/html.fti", F_OK))
      {
	icon = new FileIcon("*.{htm|html|shtml}", FileIcon::PLAIN);
        icon->load_fti("/usr/lib/filetype/iconlib/generic.doc.fti");
	icon->load_fti("/usr/lib/filetype/install/iconlib/html.fti");
      }

      if (!access("/usr/lib/filetype/install/iconlib/color.ps.idle.fti", F_OK))
      {
	icon = new FileIcon("*.ppd", FileIcon::PLAIN);
	icon->load_fti("/usr/lib/filetype/install/iconlib/color.ps.idle.fti");
      }
    }
    else
    {
      // Create the default icons...
      new FileIcon("*", FileIcon::PLAIN, sizeof(plain) / sizeof(plain[0]), plain);
      new FileIcon("*.{bmp|bw|gif|jpg|pbm|pcd|pgm|ppm|png|ras|rgb|tif|xbm|xpm}", FileIcon::PLAIN,
                   sizeof(image) / sizeof(image[0]), image);
      new FileIcon("*", FileIcon::DIR, sizeof(dir) / sizeof(dir[0]), dir);
    }

    // Mark things as initialized...
    init = 1;
  }
}


//
// 'load_kde_icons()' - Load KDE icon files.
//

static void
load_kde_icons(const char *directory)	// I - Directory to load
{
  int		i;			// Looping var
  int		n;			// Number of entries in directory
  dirent	**entries;		// Entries in directory
  char		full[1024];		// Full name of file


  entries = (dirent **)0;
  n       = filename_list(directory, &entries);

  for (i = 0; i < n; i ++)
  {
    if (entries[i]->d_name[0] != '.')
    {
      strcpy(full, directory);
      strcat(full,"/");
      strcat(full, entries[i]->d_name);

      if (filename_isdir(full))
	load_kde_icons(full);
      else
	load_kde_mimelnk(full);				
    }

    free((void *)entries[i]);
  }

  free((void*)entries);
}


//
// 'load_kde_mimelnk()' - Load a KDE "mimelnk" file.
//

static void
load_kde_mimelnk(const char *filename)
{
  FILE		*fp;
  char		tmp[256];
  char		iconfilename[1024];
  char		pattern[1024];
  char		mimetype[1024];
  char		*val;
  char		full_iconfilename[1024];
  FileIcon	*icon;


  if ((fp = fopen(filename, "rb")) != NULL)
  {
    while (fgets(tmp, sizeof(tmp), fp))
    {
      if ((val = get_kde_val(tmp, "Icon")) != NULL)
	strcpy(iconfilename, val);
      else if ((val = get_kde_val(tmp, "MimeType")) != NULL)
	strcpy(mimetype, val);
      else if ((val = get_kde_val(tmp, "Patterns")) != NULL)
	strcpy(pattern, val);
    }

    if (iconfilename && pattern)
    {
      sprintf(full_iconfilename, "/usr/share/icons/%s", iconfilename);

      if (strcmp(mimetype, "inode/directory") == 0)
	icon = new FileIcon("*", FileIcon::DIR);
      else
        icon = new FileIcon(kde_to_fltk_pattern(pattern), FileIcon::PLAIN);

      icon->load_xpm(full_iconfilename);
    }

    fclose(fp);
  }
}


//
// 'kde_to_fltk_pattern()' - Convert a KDE pattern to a FLTK pattern.
//

static char *
kde_to_fltk_pattern(const char *kdepattern)
{
  char	*pattern,
	*patptr;


  pattern = (char *)malloc(strlen(kdepattern) + 3);
  strcpy(pattern, "{");
  strcat(pattern, kdepattern);

  if (pattern[strlen(pattern) - 1] == ';')
    pattern[strlen(pattern) - 1] = '\0';

  strcat(pattern, "}");

  for (patptr = pattern; *patptr; patptr ++)
    if (*patptr == ';')
      *patptr = '|';

  return (pattern);
}


//
// 'get_kde_val()' - Get a KDE value.
//

static char *
get_kde_val(char       *str,
            const char *key)
{
  while (*str == *key)
  {
    str ++;
    key ++;
  }

  if (*key == '\0' && *str == '=')
  {
    if (str[strlen(str) - 1] == '\n')
      str[strlen(str) - 1] = '\0';

    return (str + 1);
  }

  return ((char *)0);
}


//
// End of "$Id: Fl_FileIcon2.cxx,v 1.15 2002/12/15 10:42:53 spitzak Exp $".
//
