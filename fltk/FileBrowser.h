//
// "$Id: FileBrowser.h,v 1.2 2002/12/10 02:00:29 easysw Exp $"
//
// Fl_FileBrowser definitions for the Fast Light Tool Kit (FLTK).
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

//
// Include necessary header files...
//

#ifndef fltk_FileBrowser_h
#define fltk_FileBrowser_h

#include <fltk/Browser.h>
#include <fltk/FileIcon.h>

namespace fltk {

class FL_API FileBrowser : public Browser
{
  const char	*directory_;
  uchar		icon_size_;
  const char	*pattern_;

  int		item_height(void *) const;
  int		item_width(void *) const;
  void		item_draw(void *, int, int, int, int) const;
  int		incr_height() const { return (item_height(0)); }

public:
  FileBrowser(int, int, int, int, const char * = 0);

  uchar		icon_size() const { return (icon_size_); };
  void		icon_size(uchar s) { icon_size_ = s; redraw(); };

  void		directory(const char *directory) { load(directory); }
  const char	*directory(void) const { return (directory_); }

  void		filter(const char *pattern);
  const char	*filter() const { return (pattern_); };

  int		load(const char *directory);

};

}
#endif

//
// End of "$Id: FileBrowser.h,v 1.2 2002/12/10 02:00:29 easysw Exp $".
//
