//
// "$Id$"
//
// FileBrowser definitions.
//
// Copyright 1999-2012 by Michael Sweet and others.
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

#ifndef _Fl_File_Browser_H_
#define _Fl_File_Browser_H_

#include <fltk3/FileBrowser.h>
#include "Fl_Browser.H"
#include "Fl_File_Icon.H"
#include "filename.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_File_Browser, FileBrowser)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_File_Browser, FileBrowser)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_File_Browser : public Fl_Browser {
  
public:

  FLTK3_WIDGET_VCALLS(Fl_File_Browser, FileBrowser)

  enum { FILES, DIRECTORIES };

  Fl_File_Browser() { /* empty */ }

  Fl_File_Browser(int x, int y, int w, int h, const char *label = 0) {
    _p = new fltk3::FileBrowser_I(_ctor_dx(x), _ctor_dy(y), w, h, label);
    _p->wrapper(this);
  }
  
  uchar iconsize() const { 
    return ((fltk3::FileBrowser*)_p)->iconsize();
  }
  
  void iconsize(uchar s) { 
    ((fltk3::FileBrowser*)_p)->iconsize(s);
  }
  
  void filter(const char *pattern) {
    ((fltk3::FileBrowser*)_p)->filter(pattern);
  }
  
  const char *filter() const {
    return ((fltk3::FileBrowser*)_p)->filter();
  }
  
  int load(const char *directory, Fl_File_Sort_F *sort = fl_numericsort) {
    return ((fltk3::FileBrowser*)_p)->load(directory, (fltk3::FileSortF*)sort);
  }
  
  Fl_Fontsize textsize() const { 
    return fltk3::_3to1_fontsize( ((fltk3::FileBrowser*)_p)->textsize() );
  }
  
  void textsize(Fl_Fontsize size) {
    ((fltk3::FileBrowser*)_p)->textsize( fltk3::_1to3_fontsize(size) );
  }
  
  int filetype() const { 
    return ((fltk3::FileBrowser*)_p)->filetype();
  }
  
  void filetype(int t) {
    ((fltk3::FileBrowser*)_p)->filetype(t);
  }
  
};

#endif // !_Fl_File_Browser_H_

//
// End of "$Id$".
//
