//
// "$Id: FileIcon.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $"
//
// Fl_FileIcon definitions for the Fast Light Tool Kit (FLTK).
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef fltk_FileIcon_h
#define fltk_FileIcon_h

#include "FL_API.h"
#include "Color.h"

namespace fltk {

  // This should be some kind of subclass of Image

class FL_API FileIcon
{
  static FileIcon *first_;	// Pointer to first icon/filetype
  FileIcon	*next_;		// Pointer to next icon/filetype
  const char	*pattern_;	// Pattern string
  int		type_;		// Match only if directory or file?
  int		num_data_;	// Number of data elements
  int		alloc_data_;	// Number of allocated elements
  short		*data_;		// Icon data

  public:

  enum				// File types
  {
    ANY,			// Any kind of file
    PLAIN,			// Only plain files
    FIFO,			// Only named pipes
    DEVICE,			// Only character and block devices
    LINK,			// Only symbolic links
    DIR 			// Only directories
  };

  enum				// Data opcodes
  {
    END,			// End of primitive/icon
    COLOR,			// Followed by color index
    LINE,			// Start of line
    CLOSEDLINE,			// Start of closed line
    POLYGON,			// Start of polygon
    OUTLINEPOLYGON,		// Followed by outline color
    VERTEX			// Followed by scaled X,Y
  };

  FileIcon(const char *p, int t, int nd = 0, short *d = 0);
  ~FileIcon();

  short		*add(short d);
  short		*add_color(short c)
		{ short *d = add(COLOR); add(c); return (d); }
  short		*add_vertex(int x, int y)
		{ short *d = add(VERTEX); add(x); add(y); return (d); }
  short		*add_vertex(float x, float y)
		{ short *d = add(VERTEX); add((int)(x * 10000.0));
		  add((int)(y * 10000.0)); return (d); }
  void		clear() { num_data_ = 0; }
  void		draw(int x, int y, int w, int h, Color ic, int active = 1);
  //  void		label(Widget *w);
//  static void	labeltype(const Label *o, int x, int y, int w, int h, Align a);
  void		load(const char *f);
  void		load_fti(const char *fti);
  void		load_xpm(const char *xpm);
  const char	*pattern() { return (pattern_); }
  int		size() { return (num_data_); }
  int		type() { return (type_); }
  short		*value() { return (data_); }

  static FileIcon *find(const char *filename, int filetype = ANY);
  static FileIcon *first() { return (first_); }
  static void	load_system_icons(void);
};

}

#endif

//
// End of "$Id: FileIcon.h,v 1.1 2002/12/09 04:47:59 spitzak Exp $".
//
