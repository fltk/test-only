/
// "$Id$
/
// Fl_File_Icon system icon routines
/
// KDE icon code donated by Maarten De Boer
/
// Copyright 1999-2004 by Michael Sweet
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
// Contents
/
//   Fl_File_Icon::load_fti()          - Load an SGI-format FTI file..
//   Fl_File_Icon::load_image()        - Load an image icon file..
//   Fl_File_Icon::load_system_icons() - Load the standard system icons/filetypes
/

/
// Include necessary header files..
/

#include <stdio.h
#include <stdlib.h
#include "flstring.h
#include <ctype.h
#include <errno.h
#include <FL/math.h
#include <sys/types.h
#include <sys/stat.h
#if defined(WIN32) && ! defined(__CYGWIN__
#  include <io.h
#  define F_OK	
#els
#  include <unistd.h
#endif // WIN3

#include <FL/Fl_File_Icon.H
#include <FL/Fl_Shared_Image.H
#include <FL/Fl_Widget.H
#include <FL/fl_draw.H
#include <FL/filename.H


/
// Define missing POSIX/XPG4 macros as needed..
/

#ifndef S_ISDI
#  define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK
#  define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR
#  define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR
#  define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO
#  define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK
#endif /* !S_ISDIR *


/
// Local functions..
/

static void	load_kde_icons(const char *directory, const char *icondir)
static void	load_kde_mimelnk(const char *filename, const char *icondir)
static char	*kde_to_fltk_pattern(const char *kdepattern)
static char	*get_kde_val(char *str, const char *key)


/
// Local globals..
/

static const char *kdedir = NULL


/
// 'Fl_File_Icon::load()' - Load an icon file..
/

voi
Fl_File_Icon::load(const char *f)	// I - File to read fro

  int		i;			// Load status..
  const char	*ext;			// File extensio


  ext = fl_filename_ext(f)

  if (ext && strcmp(ext, ".fti") == 0
    i = load_fti(f)
  els
    i = load_image(f)

  if (i
  
    Fl::warning("Fl_File_Icon::load(): Unable to load icon file \"%s\".", f)
    return
  



/
// 'Fl_File_Icon::load_fti()' - Load an SGI-format FTI file..
/

int					// O - 0 on success, non-zero on erro
Fl_File_Icon::load_fti(const char *fti)	// I - File to read fro

  FILE	*fp;			// File pointe
  int	ch;			// Current characte
  char	command[255],		// Command string ("vertex", etc.
	params[255],		// Parameter string ("10.0,20.0", etc.
	*ptr;			// Pointer into string
  int	outline;		// Outline polygo


  // Try to open the file..
  if ((fp = fopen(fti, "rb")) == NULL
  
    Fl::error("Fl_File_Icon::load_fti(): Unable to open \"%s\" - %s"
              fti, strerror(errno))
    return -1
  

  // Read the entire file, adding data as needed..
  outline = 0

  while ((ch = getc(fp)) != EOF
  
    // Skip whitespac
    if (isspace(ch)
      continue

    // Skip comments starting with "#"..
    if (ch == '#'
    
      while ((ch = getc(fp)) != EOF
        if (ch == '\n'
	  break

      if (ch == EOF
        break
      els
        continue
    

    // OK, this character better be a letter..
    if (!isalpha(ch)
    
      Fl::error("Fl_File_Icon::load_fti(): Expected a letter at file position %ld (saw '%c')"
                ftell(fp) - 1, ch)
      break
    

    // Scan the command name..
    ptr    = command
    *ptr++ = ch

    while ((ch = getc(fp)) != EOF
    
      if (ch == '('
        break
      else if (ptr < (command + sizeof(command) - 1)
        *ptr++ = ch
    

    *ptr++ = '\0'

    // Make sure we stopped on a parenthesis..
    if (ch != '('
    
      Fl::error("Fl_File_Icon::load_fti(): Expected a ( at file position %ld (saw '%c')"
                ftell(fp) - 1, ch)
      break
    

    // Scan the parameters..
    ptr = params

    while ((ch = getc(fp)) != EOF
    
      if (ch == ')'
        break
      else if (ptr < (params + sizeof(params) - 1)
        *ptr++ = ch
    

    *ptr++ = '\0'

    // Make sure we stopped on a parenthesis..
    if (ch != ')'
    
      Fl::error("Fl_File_Icon::load_fti(): Expected a ) at file position %ld (saw '%c')"
                ftell(fp) - 1, ch)
      break
    

    // Make sure the next character is a semicolon..
    if ((ch = getc(fp)) != ';'
    
      Fl::error("Fl_File_Icon::load_fti(): Expected a ; at file position %ld (saw '%c')"
                ftell(fp) - 1, ch)
      break
    

    // Now process the command..
    if (strcmp(command, "color") == 0
    
      // Set the color; for negative colors blend the two primaries t
      // produce a composite color.  Also, the following symbolic colo
      // names are understood
      /
      //     name           FLTK colo
      //     -------------  ---------
      //     iconcolor      FL_ICON_COLOR; mapped to the icon color i
      //                    Fl_File_Icon::draw(
      //     shadowcolor    FL_DARK
      //     outlinecolor   FL_BLAC
      if (strcmp(params, "iconcolor") == 0
        add_color(FL_ICON_COLOR)
      else if (strcmp(params, "shadowcolor") == 0
        add_color(FL_DARK3)
      else if (strcmp(params, "outlinecolor") == 0
        add_color(FL_BLACK)
      els
      
        int c = atoi(params);	// Color valu


        if (c < 0
	
	  // Composite color; compute average..
	  c = -c
	  add_color(fl_color_average((Fl_Color)(c >> 4)
	                             (Fl_Color)(c & 15), 0.5f))
	
	els
	  add_color((Fl_Color)c)
      
    
    else if (strcmp(command, "bgnline") == 0
      add(LINE)
    else if (strcmp(command, "bgnclosedline") == 0
      add(CLOSEDLINE)
    else if (strcmp(command, "bgnpolygon") == 0
      add(POLYGON)
    else if (strcmp(command, "bgnoutlinepolygon") == 0
    
      add(OUTLINEPOLYGON)
      outline = add(0) - data_
      add(0)
    
    else if (strcmp(command, "endoutlinepolygon") == 0 && outline
    
      unsigned cval; // Color valu

      // Set the outline color; see above for valid values..
      if (strcmp(params, "iconcolor") == 0
        cval = FL_ICON_COLOR
      else if (strcmp(params, "shadowcolor") == 0
        cval = FL_DARK3
      else if (strcmp(params, "outlinecolor") == 0
        cval = FL_BLACK
      els
      
        int c = atoi(params);	// Color valu


        if (c < 0
	
	  // Composite color; compute average..
	  c = -c
	  cval = fl_color_average((Fl_Color)(c >> 4), (Fl_Color)(c & 15), 0.5f)
	
	els
	  cval = c
      

      // Store outline color..
      data_[outline]     = cval >> 16
      data_[outline + 1] = cval

      outline = 0
      add(END)
    
    else if (strncmp(command, "end", 3) == 0
      add(END)
    else if (strcmp(command, "vertex") == 0
    
      float x, y;		// Coordinates of verte


      if (sscanf(params, "%f,%f", &x, &y) != 2
        break

      add_vertex((short)(int)rint(x * 100.0), (short)(int)rint(y * 100.0))
    
    els
    
      Fl::error("Fl_File_Icon::load_fti(): Unknown command \"%s\" at file position %ld."
                command, ftell(fp) - 1)
      break
    
  

  // Close the file and return..
  fclose(fp)

#ifdef DEBU
  printf("Icon File \"%s\":\n", fti)
  for (int i = 0; i < num_data_; i ++
    printf("    %d,\n", data_[i])
#endif /* DEBUG *

  return 0



/
// 'Fl_File_Icon::load_image()' - Load an image icon file..
/

int					// O - 0 on success, non-0 on erro
Fl_File_Icon::load_image(const char *ifile)	// I - File to read fro

  Fl_Shared_Image	*img;		// Image fil


  img = Fl_Shared_Image::get(ifile)
  if (!img || !img->count() || !img->w() || !img->h()) return -1

  if (img->count() == 1) 
    int		x, y;		// X & Y in imag
    int		startx;		// Starting X coor
    Fl_Color	c,		// Current colo
		temp;		// Temporary colo
    const uchar *row;		// Pointer into imag


    // Loop through grayscale or RGB image..
    for (y = 0, row = (const uchar *)(*(img->data())); y < img->h(); y ++, row += img->ld()
    
      for (x = 0, startx = 0, c = (Fl_Color)-1
           x < img->w()
	   x ++, row += img->d()
      
	switch (img->d()
	
          case 1 
              temp = fl_rgb_color(row[0], row[0], row[0])
	      break
          case 2 
	      if (row[1] > 127
        	temp = fl_rgb_color(row[0], row[0], row[0])
	      els
		temp = (Fl_Color)-1
	      break
	  case 3 
              temp = fl_rgb_color(row[0], row[1], row[2])
	      break
	  default 
	      if (row[3] > 127
        	temp = fl_rgb_color(row[0], row[1], row[2])
	      els
		temp = (Fl_Color)-1
	      break
	

	if (temp != c
	
	  if (x > startx && c != (Fl_Color)-1
	  
	    add_color(c)
	    add(POLYGON)
	    add_vertex(startx * 9000 / img->w() + 1000, 9500 - y * 9000 / img->h())
	    add_vertex(x * 9000 / img->w() + 1000,      9500 - y * 9000 / img->h())
	    add_vertex(x * 9000 / img->w() + 1000,      9500 - (y + 1) * 9000 / img->h())
	    add_vertex(startx * 9000 / img->w() + 1000, 9500 - (y + 1) * 9000 / img->h())
	    add(END)
	  

          c      = temp
	  startx = x
	
      

      if (x > startx && c != (Fl_Color)-1
      
	add_color(c)
	add(POLYGON)
	add_vertex(startx * 9000 / img->w() + 1000, 9500 - y * 9000 / img->h())
	add_vertex(x * 9000 / img->w() + 1000,      9500 - y * 9000 / img->h())
	add_vertex(x * 9000 / img->w() + 1000,      9500 - (y + 1) * 9000 / img->h())
	add_vertex(startx * 9000 / img->w() + 1000, 9500 - (y + 1) * 9000 / img->h())
	add(END)
      
    
  } else 
    int		i, j;			// Looping var
    int		ch;			// Current characte
    int		newch;			// New characte
    int		bg;			// Background colo
    char	val[16];		// Color valu
    const char	*lineptr,		// Pointer into lin
		*const*ptr;		// Pointer into data arra
    int		ncolors,		// Number of color
		chars_per_color;	// Characters per colo
    Fl_Color	*colors;		// Color
    int		red, green, blue;	// Red, green, and blue value
    int		x, y;			// X & Y in imag
    int		startx;			// Starting X coor


    // Get the pixmap data..
    ptr = img->data()
    sscanf(*ptr, "%*d%*d%d%d", &ncolors, &chars_per_color)

    colors = new Fl_Color[1 << (chars_per_color * 8)]

    // Read the colormap..
    memset(colors, 0, sizeof(Fl_Color) << (chars_per_color * 8))
    bg = ' '

    ptr ++

    if (ncolors < 0) 
      // Read compressed colormap..
      const uchar *cmapptr

      ncolors = -ncolors

      for (i = 0, cmapptr = (const uchar *)*ptr; i < ncolors; i ++, cmapptr += 4
        colors[cmapptr[0]] = fl_rgb_color(cmapptr[1], cmapptr[2], cmapptr[3])

      ptr ++
    } else 
      for (i = 0; i < ncolors; i ++, ptr ++) 
	// Get the color's characte
	lineptr = *ptr
	ch      = *lineptr++

        if (chars_per_color > 1) ch = (ch << 8) | *lineptr++

	// Get the color value..
	if ((lineptr = strstr(lineptr, "c ")) == NULL) 
	  // No color; make this black..
	  colors[ch] = FL_BLACK
	} else if (lineptr[2] == '#') 
	  // Read the RGB triplet..
	  lineptr += 3
	  for (j = 0; j < 12; j ++
            if (!isxdigit(lineptr[j])
	      break

	  switch (j) 
            case 0 
		bg = ch
	    default 
		red = green = blue = 0
		break

            case 3 
		val[0] = lineptr[0]
		val[1] = '\0'
		red = 255 * strtol(val, NULL, 16) / 15

		val[0] = lineptr[1]
		val[1] = '\0'
		green = 255 * strtol(val, NULL, 16) / 15

		val[0] = lineptr[2]
		val[1] = '\0'
		blue = 255 * strtol(val, NULL, 16) / 15
		break

            case 6 
            case 9 
            case 12 
		j /= 3

		val[0] = lineptr[0]
		val[1] = lineptr[1]
		val[2] = '\0'
		red = strtol(val, NULL, 16)

		val[0] = lineptr[j + 0]
		val[1] = lineptr[j + 1]
		val[2] = '\0'
		green = strtol(val, NULL, 16)

		val[0] = lineptr[2 * j + 0]
		val[1] = lineptr[2 * j + 1]
		val[2] = '\0'
		blue = strtol(val, NULL, 16)
		break
	  

	  colors[ch] = fl_rgb_color((uchar)red, (uchar)green, (uchar)blue)
	} else 
	  // Read a color name..
	  if (strncasecmp(lineptr + 2, "white", 5) == 0) colors[ch] = FL_WHITE
	  else if (strncasecmp(lineptr + 2, "black", 5) == 0) colors[ch] = FL_BLACK
	  else if (strncasecmp(lineptr + 2, "none", 4) == 0) 
            colors[ch] = FL_BLACK
	    bg = ch
	  } else colors[ch] = FL_GRAY
	
      
    

    // Read the image data..
    for (y = 0; y < img->h(); y ++, ptr ++) 
      lineptr = *ptr
      startx  = 0
      ch      = bg

      for (x = 0; x < img->w(); x ++) 
	newch = *lineptr++

        if (chars_per_color > 1) newch = (newch << 8) | *lineptr++

	if (newch != ch) 
	  if (ch != bg) 
            add_color(colors[ch])
	    add(POLYGON)
	    add_vertex(startx * 9000 / img->w() + 1000, 9500 - y * 9000 / img->h())
	    add_vertex(x * 9000 / img->w() + 1000,      9500 - y * 9000 / img->h())
	    add_vertex(x * 9000 / img->w() + 1000,      9500 - (y + 1) * 9000 / img->h())
	    add_vertex(startx * 9000 / img->w() + 1000, 9500 - (y + 1) * 9000 / img->h())
	    add(END)
          

	  ch     = newch
	  startx = x
	
      

      if (ch != bg) 
	add_color(colors[ch])
	add(POLYGON)
	add_vertex(startx * 9000 / img->w() + 1000, 9500 - y * 9000 / img->h())
	add_vertex(x * 9000 / img->w() + 1000,      9500 - y * 9000 / img->h())
	add_vertex(x * 9000 / img->w() + 1000,      9500 - (y + 1) * 9000 / img->h())
	add_vertex(startx * 9000 / img->w() + 1000, 9500 - (y + 1) * 9000 / img->h())
	add(END)
      
    

    // Free the colormap..
    delete[] colors
  

  img->release()

#ifdef DEBU
  printf("Icon File \"%s\":\n", xpm)
  for (i = 0; i < num_data_; i ++
    printf("    %d,\n", data_[i])
#endif // DEBU

  return 0



/
// 'Fl_File_Icon::load_system_icons()' - Load the standard system icons/filetypes

voi
Fl_File_Icon::load_system_icons(void

  int		i;		// Looping va
  Fl_File_Icon	*icon;		// New icon
  char		filename[1024];	// Filenam
  char		icondir[1024];	// Icon director
  static int	init = 0;	// Have the icons been initialized
  const char * const icondirs[] 
		{		// Icon directories to look for, in orde
		  "Bluecurve"
		  "crystalsvg"
		  "default.kde"
		  "hicolor"
		  NUL
		}
  static short	plain[] =	// Plain file ico
		
		  COLOR, -1, -1, OUTLINEPOLYGON, 0, FL_GRAY
		  VERTEX, 2000, 1000, VERTEX, 2000, 9000
		  VERTEX, 6000, 9000, VERTEX, 8000, 7000
		  VERTEX, 8000, 1000, END, OUTLINEPOLYGON, 0, FL_GRAY
		  VERTEX, 6000, 9000, VERTEX, 6000, 7000
		  VERTEX, 8000, 7000, END
		  COLOR, 0, FL_BLACK, LINE, VERTEX, 6000, 7000
		  VERTEX, 8000, 7000, VERTEX, 8000, 1000
		  VERTEX, 2000, 1000, END, LINE, VERTEX, 3000, 7000
		  VERTEX, 5000, 7000, END, LINE, VERTEX, 3000, 6000
		  VERTEX, 5000, 6000, END, LINE, VERTEX, 3000, 5000
		  VERTEX, 7000, 5000, END, LINE, VERTEX, 3000, 4000
		  VERTEX, 7000, 4000, END, LINE, VERTEX, 3000, 3000
		  VERTEX, 7000, 3000, END, LINE, VERTEX, 3000, 2000
		  VERTEX, 7000, 2000, END,
		  EN
		}
  static short	image[] =	// Image file ico
		
		  COLOR, -1, -1, OUTLINEPOLYGON, 0, FL_GRAY
		  VERTEX, 2000, 1000, VERTEX, 2000, 9000
		  VERTEX, 6000, 9000, VERTEX, 8000, 7000
		  VERTEX, 8000, 1000, END, OUTLINEPOLYGON, 0, FL_GRAY
		  VERTEX, 6000, 9000, VERTEX, 6000, 7000
		  VERTEX, 8000, 7000, END
		  COLOR, 0, FL_BLACK, LINE, VERTEX, 6000, 7000
		  VERTEX, 8000, 7000, VERTEX, 8000, 1000
		  VERTEX, 2000, 1000, END
		  COLOR, 0, FL_RED, POLYGON, VERTEX, 3500, 2500
		  VERTEX, 3000, 3000, VERTEX, 3000, 4000
		  VERTEX, 3500, 4500, VERTEX, 4500, 4500
		  VERTEX, 5000, 4000, VERTEX, 5000, 3000
		  VERTEX, 4500, 2500, END
		  COLOR, 0, FL_GREEN, POLYGON, VERTEX, 5500, 2500
		  VERTEX, 5000, 3000, VERTEX, 5000, 4000
		  VERTEX, 5500, 4500, VERTEX, 6500, 4500
		  VERTEX, 7000, 4000, VERTEX, 7000, 3000
		  VERTEX, 6500, 2500, END
		  COLOR, 0, FL_BLUE, POLYGON, VERTEX, 4500, 3500
		  VERTEX, 4000, 4000, VERTEX, 4000, 5000
		  VERTEX, 4500, 5500, VERTEX, 5500, 5500
		  VERTEX, 6000, 5000, VERTEX, 6000, 4000
		  VERTEX, 5500, 3500, END
		  EN
		}
  static short	dir[] =		// Directory ico
		
		  COLOR, -1, -1, POLYGON, VERTEX, 1000, 1000
		  VERTEX, 1000, 7500,  VERTEX, 9000, 7500
		  VERTEX, 9000, 1000, END
		  POLYGON, VERTEX, 1000, 7500, VERTEX, 2500, 9000
		  VERTEX, 5000, 9000, VERTEX, 6500, 7500, END
		  COLOR, 0, FL_WHITE, LINE, VERTEX, 1500, 1500
		  VERTEX, 1500, 7000, VERTEX, 9000, 7000, END
		  COLOR, 0, FL_BLACK, LINE, VERTEX, 9000, 7500
		  VERTEX, 9000, 1000, VERTEX, 1000, 1000, END
		  COLOR, 0, FL_GRAY, LINE, VERTEX, 1000, 1000
		  VERTEX, 1000, 7500, VERTEX, 2500, 9000
		  VERTEX, 5000, 9000, VERTEX, 6500, 7500
		  VERTEX, 9000, 7500, END
		  EN
		}


  // Add symbols if they haven't been added already..
  if (!init
  
    // This method requires the images library..
    fl_register_images()

    if (!kdedir
    
      // Figure out where KDE is installed..
      if ((kdedir = getenv("KDEDIR")) == NULL
      
        if (!access("/opt/kde", F_OK)
	  kdedir = "/opt/kde"
	else if (!access("/usr/local/share/mimelnk", F_OK)
	  kdedir = "/usr/local"
        els
	  kdedir = "/usr"
      
    

    snprintf(filename, sizeof(filename), "%s/share/mimelnk", kdedir)

    if (!access(filename, F_OK)
    
      // Load KDE icons..
      icon = new Fl_File_Icon("*", Fl_File_Icon::PLAIN)

      for (i = 0; icondirs[i]; i ++
      
	snprintf(icondir, sizeof(icondir), "%s/share/icons/%s", kdedir
		 icondirs[i])

        if (!access(icondir, F_OK)
	  break
      

      if (icondirs[i]
        snprintf(filename, sizeof(filename), "%s/16x16/mimetypes/unknown.png"
	         icondir)
      els
	snprintf(filename, sizeof(filename), "%s/share/icons/unknown.xpm"
	         kdedir)

      if (!access(filename, F_OK)
	icon->load_image(filename)

      icon = new Fl_File_Icon("*", Fl_File_Icon::LINK)

      snprintf(filename, sizeof(filename), "%s/16x16/filesystems/link.png"
               icondir)

      if (!access(filename, F_OK)
        icon->load_image(filename)

      snprintf(filename, sizeof(filename), "%s/share/mimelnk", kdedir)
      load_kde_icons(filename, icondir)
    
    else if (!access("/usr/share/icons/folder.xpm", F_OK)
    
      // Load GNOME icons..
      icon = new Fl_File_Icon("*", Fl_File_Icon::PLAIN)
      icon->load_image("/usr/share/icons/page.xpm")

      icon = new Fl_File_Icon("*", Fl_File_Icon::DIRECTORY)
      icon->load_image("/usr/share/icons/folder.xpm")
    
    else if (!access("/usr/dt/appconfig/icons", F_OK)
    
      // Load CDE icons..
      icon = new Fl_File_Icon("*", Fl_File_Icon::PLAIN)
      icon->load_image("/usr/dt/appconfig/icons/C/Dtdata.m.pm")

      icon = new Fl_File_Icon("*", Fl_File_Icon::DIRECTORY)
      icon->load_image("/usr/dt/appconfig/icons/C/DtdirB.m.pm")

      icon = new Fl_File_Icon("core", Fl_File_Icon::PLAIN)
      icon->load_image("/usr/dt/appconfig/icons/C/Dtcore.m.pm")

      icon = new Fl_File_Icon("*.{bmp|bw|gif|jpg|pbm|pcd|pgm|ppm|png|ras|rgb|tif|xbm|xpm}", Fl_File_Icon::PLAIN)
      icon->load_image("/usr/dt/appconfig/icons/C/Dtimage.m.pm")

      icon = new Fl_File_Icon("*.{eps|pdf|ps}", Fl_File_Icon::PLAIN)
      icon->load_image("/usr/dt/appconfig/icons/C/Dtps.m.pm")

      icon = new Fl_File_Icon("*.ppd", Fl_File_Icon::PLAIN)
      icon->load_image("/usr/dt/appconfig/icons/C/DtPrtpr.m.pm")
    
    else if (!access("/usr/lib/filetype", F_OK)
    
      // Load SGI icons..
      icon = new Fl_File_Icon("*", Fl_File_Icon::PLAIN)
      icon->load_fti("/usr/lib/filetype/iconlib/generic.doc.fti")

      icon = new Fl_File_Icon("*", Fl_File_Icon::DIRECTORY)
      icon->load_fti("/usr/lib/filetype/iconlib/generic.folder.closed.fti")

      icon = new Fl_File_Icon("core", Fl_File_Icon::PLAIN)
      icon->load_fti("/usr/lib/filetype/default/iconlib/CoreFile.fti")

      icon = new Fl_File_Icon("*.{bmp|bw|gif|jpg|pbm|pcd|pgm|ppm|png|ras|rgb|tif|xbm|xpm}", Fl_File_Icon::PLAIN)
      icon->load_fti("/usr/lib/filetype/system/iconlib/ImageFile.fti")

      if (!access("/usr/lib/filetype/install/iconlib/acroread.doc.fti", F_OK)
      
	icon = new Fl_File_Icon("*.{eps|ps}", Fl_File_Icon::PLAIN)
	icon->load_fti("/usr/lib/filetype/system/iconlib/PostScriptFile.closed.fti")

	icon = new Fl_File_Icon("*.pdf", Fl_File_Icon::PLAIN)
	icon->load_fti("/usr/lib/filetype/install/iconlib/acroread.doc.fti")
      
      els
      
	icon = new Fl_File_Icon("*.{eps|pdf|ps}", Fl_File_Icon::PLAIN)
	icon->load_fti("/usr/lib/filetype/system/iconlib/PostScriptFile.closed.fti")
      

      if (!access("/usr/lib/filetype/install/iconlib/html.fti", F_OK)
      
	icon = new Fl_File_Icon("*.{htm|html|shtml}", Fl_File_Icon::PLAIN)
        icon->load_fti("/usr/lib/filetype/iconlib/generic.doc.fti")
	icon->load_fti("/usr/lib/filetype/install/iconlib/html.fti")
      

      if (!access("/usr/lib/filetype/install/iconlib/color.ps.idle.fti", F_OK)
      
	icon = new Fl_File_Icon("*.ppd", Fl_File_Icon::PLAIN)
	icon->load_fti("/usr/lib/filetype/install/iconlib/color.ps.idle.fti")
      
    
    els
    
      // Create the default icons..
      new Fl_File_Icon("*", Fl_File_Icon::PLAIN, sizeof(plain) / sizeof(plain[0]), plain)
      new Fl_File_Icon("*.{bm|bmp|bw|gif|jpg|pbm|pcd|pgm|ppm|png|ras|rgb|tif|xbm|xpm}", Fl_File_Icon::PLAIN
                   sizeof(image) / sizeof(image[0]), image)
      new Fl_File_Icon("*", Fl_File_Icon::DIRECTORY, sizeof(dir) / sizeof(dir[0]), dir)
    

    // Mark things as initialized..
    init = 1
  



/
// 'load_kde_icons()' - Load KDE icon files
/

static voi
load_kde_icons(const char *directory,	// I - Directory to loa
               const char *icondir)	// I - Location of icon

  int		i;			// Looping va
  int		n;			// Number of entries in director
  dirent	**entries;		// Entries in director
  char		full[1024];		// Full name of fil


  entries = (dirent **)0
  n       = fl_filename_list(directory, &entries)

  for (i = 0; i < n; i ++
  
    if (entries[i]->d_name[0] != '.'
    
      snprintf(full, sizeof(full), "%s/%s", directory, entries[i]->d_name)

      if (fl_filename_isdir(full)
	load_kde_icons(full, icondir)
      els
	load_kde_mimelnk(full, icondir)
    

    free((void *)entries[i])
  

  free((void*)entries)



/
// 'load_kde_mimelnk()' - Load a KDE "mimelnk" file
/

static voi
load_kde_mimelnk(const char *filename,	// I - mimelnk filenam
                 const char *icondir)	// I - Location of icon

  FILE		*fp
  char		tmp[1024]
  char		iconfilename[1024]
  char		pattern[1024]
  char		mimetype[1024]
  char		*val
  char		full_iconfilename[1024]
  Fl_File_Icon	*icon


  mimetype[0]     = '\0'
  pattern[0]      = '\0'
  iconfilename[0] = '\0'

  if ((fp = fopen(filename, "rb")) != NULL
  
    while (fgets(tmp, sizeof(tmp), fp)
    
      if ((val = get_kde_val(tmp, "Icon")) != NULL
	strlcpy(iconfilename, val, sizeof(iconfilename))
      else if ((val = get_kde_val(tmp, "MimeType")) != NULL
	strlcpy(mimetype, val, sizeof(mimetype))
      else if ((val = get_kde_val(tmp, "Patterns")) != NULL
	strlcpy(pattern, val, sizeof(pattern))
    

    fclose(fp)

    if (iconfilename[0] && (pattern[0] || strncmp(mimetype, "inode/", 6) == 0)
    
      if (!access(icondir, F_OK)
      
        // KDE 3.x and 2.x icon
	int		i;		// Looping va
	static const char *paths[] = {	// Subdirs to look in..
	  "16x16/actions"
	  "16x16/apps"
	  "16x16/devices"
	  "16x16/filesystems"
	  "16x16/mimetypes"

	  "20x20/actions"
	  "20x20/apps"
	  "20x20/devices"
	  "20x20/filesystems"
	  "20x20/mimetypes"

	  "22x22/actions"
	  "22x22/apps"
	  "22x22/devices"
	  "22x22/filesystems"
	  "22x22/mimetypes"

	  "24x24/actions"
	  "24x24/apps"
	  "24x24/devices"
	  "24x24/filesystems"
	  "24x24/mimetypes"

	  "32x32/actions"
	  "32x32/apps"
	  "32x32/devices"
	  "32x32/filesystems"
	  "32x32/mimetypes"

	  "36x36/actions"
	  "36x36/apps"
	  "36x36/devices"
	  "36x36/filesystems"
	  "36x36/mimetypes"

	  "48x48/actions"
	  "48x48/apps"
	  "48x48/devices"
	  "48x48/filesystems"
	  "48x48/mimetypes"

	  "64x64/actions"
	  "64x64/apps"
	  "64x64/devices"
	  "64x64/filesystems"
	  "64x64/mimetypes"

	  "96x96/actions"
	  "96x96/apps"
	  "96x96/devices"
	  "96x96/filesystems"
	  "96x96/mimetypes
	}

        for (i = 0; i < (int)(sizeof(paths) / sizeof(paths[0])); i ++) 
          snprintf(full_iconfilename, sizeof(full_iconfilename)
	           "%s/%s/%s.png", icondir, paths[i], iconfilename)

          if (!access(full_iconfilename, F_OK)) break
	

        if (i >= (int)(sizeof(paths) / sizeof(paths[0]))) return
      } else 
        // KDE 1.x icon
        snprintf(full_iconfilename, sizeof(full_iconfilename)
	         "%s/%s", tmp, iconfilename)

        if (access(full_iconfilename, F_OK)) return
      

      if (strncmp(mimetype, "inode/", 6) == 0) 
	if (strcmp(mimetype + 6, "directory") == 0
	  icon = new Fl_File_Icon("*", Fl_File_Icon::DIRECTORY)
	else if (strcmp(mimetype + 6, "blockdevice") == 0
	  icon = new Fl_File_Icon("*", Fl_File_Icon::DEVICE)
	else if (strcmp(mimetype + 6, "fifo") == 0
	  icon = new Fl_File_Icon("*", Fl_File_Icon::FIFO)
	else return
      } els
        icon = new Fl_File_Icon(kde_to_fltk_pattern(pattern), Fl_File_Icon::PLAIN)

      icon->load(full_iconfilename)
    
  



/
// 'kde_to_fltk_pattern()' - Convert a KDE pattern to a FLTK pattern
/

static char 
kde_to_fltk_pattern(const char *kdepattern

  char	*pattern
	*patptr


  pattern = (char *)malloc(strlen(kdepattern) + 3)
  strcpy(pattern, "{")
  strcpy(pattern + 1, kdepattern)

  if (pattern[strlen(pattern) - 1] == ';'
    pattern[strlen(pattern) - 1] = '\0'

  strcat(pattern, "}")

  for (patptr = pattern; *patptr; patptr ++
    if (*patptr == ';'
      *patptr = '|'

  return (pattern)



/
// 'get_kde_val()' - Get a KDE value
/

static char 
get_kde_val(char       *str
            const char *key

  while (*str == *key
  
    str ++
    key ++
  

  if (*key == '\0' && *str == '='
  
    if (str[strlen(str) - 1] == '\n'
      str[strlen(str) - 1] = '\0'

    return (str + 1)
  

  return ((char *)0)



/
// End of "$Id$"
/
