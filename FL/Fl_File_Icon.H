/
// "$Id$
/
// Fl_File_Icon definitions
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

/
// Include necessary header files..
/

#ifndef _Fl_Fl_File_Icon_H
#  define _Fl_Fl_File_Icon_H

#  include "Fl.H


/
// Special color value for the icon color
/

#  define FL_ICON_COLOR (Fl_Color)0xfffffff


/
// Fl_File_Icon class..
/

/** The Fl_File_Icon class manages icon images that can be used as label
 * in other widgets and as icons in the FileBrowser widget.
 *
class FL_EXPORT Fl_File_Icon			//// Icon dat

  static Fl_File_Icon *first_;	// Pointer to first icon/filetyp
  Fl_File_Icon	*next_;		// Pointer to next icon/filetyp
  const char	*pattern_;	// Pattern strin
  int		type_;		// Match only if directory or file
  int		num_data_;	// Number of data element
  int		alloc_data_;	// Number of allocated element
  short		*data_;		// Icon dat

  public

  enum				/// File type
  
    ANY,			///< Any kind of fil
    PLAIN,			///< Only plain file
    FIFO,			///< Only named pipe
    DEVICE,			///< Only character and block device
    LINK,			///< Only symbolic link
    DIRECTORY			///< Only directorie
  }

  enum				// Data opcode
  
    END,			// End of primitive/ico
    COLOR,			// Followed by color value (2 shorts
    LINE,			// Start of lin
    CLOSEDLINE,			// Start of closed lin
    POLYGON,			// Start of polygo
    OUTLINEPOLYGON,		// Followed by outline color (2 shorts
    VERTEX			// Followed by scaled X,
  }

    /** The constructor creates a new Fl_File_Icon with the specified
     * information. *
  Fl_File_Icon(const char *p, int t, int nd = 0, short *d = 0)
    /** The destructor destroys the icon and frees all memory that has
     * been allocated for it. *
  ~Fl_File_Icon()

    /** Adds a keyword value to the icon array, returning a pointer to it. *
  short		*add(short d)
    /** Adds a color value to the icon array, returning a pointer to it. *
  short		*add_color(Fl_Color c
		{ short *d = add((short)COLOR); add((short)(c >> 16)); add((short)c); return (d); 
    /** Adds a vertex value to the icon array, returning a pointer to it.
     * The integer version accepts coordinates from 0 to 10000. *
  short		*add_vertex(int x, int y
		{ short *d = add((short)VERTEX); add((short)x); add((short)y); return (d); 
    /** Adds a vertex value to the icon array, returning a pointer to it.
     * The floating point version goes from 0.0 to 1.0. The origin (0.0) i
     * in the lower-lefthand corner of the icon. *
  short		*add_vertex(float x, float y
		{ short *d = add((short)VERTEX); add((short)(x * 10000.0))
		  add((short)(y * 10000.0)); return (d); 
    /** Clears all icon data from the icon. *
  void		clear() { num_data_ = 0; 
    /** Draws the icon in the indicated area. *
  void		draw(int x, int y, int w, int h, Fl_Color ic, int active = 1)
    /** Applies the icon to the widget, registering the Fl_File_Icon label type as needed. *
  void		label(Fl_Widget *w)
    /** The labeltype function for icons. *
  static void	labeltype(const Fl_Label *o, int x, int y, int w, int h, Fl_Align a)
    /** Loads the specified icon image. The format is deduced from the filename. *
  void		load(const char *f)
    /** Loads an SGI icon file. *
  int		load_fti(const char *fti)
    /** Loads an icon file. *
  int		load_image(const char *i)
  Fl_File_Icon	*next() { return (next_); 
    /** Returns the filename matching pattern for the icon. *
  const char	*pattern() { return (pattern_); 
    /** Returns the number of words of data used by the icon. *
  int		size() { return (num_data_); 
    /** Returns the filetype associated with the icon, which can be one o
     * the following
     * \arg \c Fl_File_Icon::ANY, any kind of fil
     * \arg \c Fl_File_Icon::PLAIN, plain file
     * \arg \c Fl_File_Icon::FIFO, named pipe
     * \arg \c Fl_File_Icon::DEVICE, character and block device
     * \arg \c Fl_File_Icon::LINK, symbolic link
     * \arg \c Fl_File_Icon::DIRECTORY, directories *
  int		type() { return (type_); 
    /** Returns the data array for the icon.  *
  short		*value() { return (data_); 

    /** Finds an icon that matches the given filename and file type. *
  static Fl_File_Icon *find(const char *filename, int filetype = ANY)
    /** Returns a pointer to the first icon in the list. *
  static Fl_File_Icon *first() { return (first_); 
    /** Loads all system-defined icons. This call is useful when using th
     * FileChooser widget and should be used when the application starts:<br
     * <tt>  Fl_File_Icon::load_system_icons();</tt> *
  static void	load_system_icons(void)
}

#endif // !_Fl_Fl_File_Icon_H

/
// End of "$Id$"
/
