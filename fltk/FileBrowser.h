//
// "$Id$"
//
// FileBrowser definitions.
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

//
// Include necessary header files...
//

#ifndef fltk_FileBrowser_h
#define fltk_FileBrowser_h

#include <fltk/Browser.h>
#include <fltk/FileIcon.h>
#include <fltk/filename.h>

namespace fltk {


//
// FileBrowser class...
//

class FL_API FileBrowser : public Browser
{
  int		filetype_;
  const char	*directory_;
  uchar		icon_size_;
  const char	*pattern_;

  int		full_height() const;
  int		item_height(void *) const;
  int		item_width(void *) const;
  void		item_draw(void *, int, int, int, int) const;
  int		incr_height() const { return (item_height(0)); }

public:
  enum { FILES, DIRECTORIES };

  FileBrowser(int, int, int, int, const char * = 0);

  uchar		icon_size() const { return (icon_size_); };
  void		icon_size(uchar s) { icon_size_ = s; redraw(); };

  void	filter(const char *pattern);
  const char	*filter() const { return (pattern_); };

  int		load(const char *directory, File_Sort_F *sort = (File_Sort_F*) fltk::numericsort);

  float		textsize() const { return (Browser::textsize()); };
  void		textsize(float s) { Browser::textsize(s); icon_size_ = (uchar)(3 * s / 2); };

  int		filetype() const { return (filetype_); };
  void		filetype(int t) { filetype_ = t; };
  const char *  directory() const {return directory_;}
};

}

#endif // !_Fl_File_Browser_H_

//
// End of "$Id$".
//
