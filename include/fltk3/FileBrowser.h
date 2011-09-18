//
// "$Id: FileBrowser.h 8306 2011-01-24 17:04:22Z matt $"
//
// FileBrowser definitions.
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

/* \file
 fltk3::FileBrowser widget . */

//
// Include necessary header files...
//

#ifndef _Fltk3_File_Browser_H_
#  define _Fltk3_File_Browser_H_

#  include "Browser.h"
#  include "FileIcon.h"
#  include "filename.h"


namespace fltk3 {
  
  //
  // fltk3::FileBrowser class...
  //
  
  /** The fltk3::FileBrowser widget displays a list of filenames, optionally with file-specific icons. */
  class FLTK3_EXPORT FileBrowser : public fltk3::Browser {
    
    int		filetype_;
    const char	*directory_;
    uchar		iconsize_;
    const char	*pattern_;
    
    int		full_height() const;
    int		item_height(void *) const;
    int		item_width(void *) const;
    void		item_draw(void *, int, int, int, int) const;
    int		incr_height() const { return (item_height(0)); }
    
  public:
    enum { FILES, DIRECTORIES };
    
    /**
     The constructor creates the fltk3::FileBrowser widget at the specified position and size.
     The destructor destroys the widget and frees all memory that has been allocated.
     */
    FileBrowser(int, int, int, int, const char * = 0);
    
    /**    Sets or gets the size of the icons. The default size is 20 pixels.  */
    uchar		iconsize() const { return (iconsize_); };
    /**    Sets or gets the size of the icons. The default size is 20 pixels.  */
    void		iconsize(uchar s) { iconsize_ = s; redraw(); };
    
    /**
     Sets or gets the filename filter. The pattern matching uses
     the fltk3::filename_match()
     function in FLTK.
     */
    void	filter(const char *pattern);
    /**
     Sets or gets the filename filter. The pattern matching uses
     the fltk3::filename_match()
     function in FLTK.
     */
    const char	*filter() const { return (pattern_); };
    
    /**
     Loads the specified directory into the browser. If icons have been
     loaded then the correct icon is associated with each file in the list.
     
     <P>The sort argument specifies a sort function to be used with
     fltk3::filename_list().
     */
    int		load(const char *directory, fltk3::FileSortF *sort = fltk3::numericsort);
    
    fltk3::Fontsize  textsize() const { return Browser::textsize(); };
    void		textsize(fltk3::Fontsize s) { Browser::textsize(s); iconsize_ = (uchar)(3 * s / 2); };
    
    /**
     Sets or gets the file browser type, FILES or
     DIRECTORIES. When set to FILES, both
     files and directories are shown. Otherwise only directories are
     shown.
     */
    int		filetype() const { return (filetype_); };
    /**
     Sets or gets the file browser type, FILES or
     DIRECTORIES. When set to FILES, both
     files and directories are shown. Otherwise only directories are
     shown.
     */
    void		filetype(int t) { filetype_ = t; };
  };
  
}

#endif // !_Fl_File_Browser_H_

//
// End of "$Id: FileBrowser.h 8306 2011-01-24 17:04:22Z matt $".
//
