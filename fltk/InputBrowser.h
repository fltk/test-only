//
// "$Id: InputBrowser.h,v 1.3 2003/04/20 03:17:47 easysw Exp $"
//
// MicroSoft style "ComboBox" with the menu appearing below with a
// scrollbar. I would like to use the name "ComboBox" or "InputChoice"
// for a more user-friendly version which uses pop-up menus and
// positions the menu with the cursor pointing at the current item,
// but this version can be used to get what MicroSoft users expect.
// The implementation is a good example of how to get a widget to appear
// in a modal pop-up window.
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#ifndef fltk_InputBrowser_h
#define fltk_InputBrowser_h

#include <fltk/Menu.h>
#include <fltk/Input.h>

namespace fltk {

class FL_API InputBrowser : public Menu {
public:
  enum { // values for type()
    NORMAL = 0,
    NONEDITABLE = 1,
    INDENTED = 2,
    NONEDITABLE_INDENTED = 3
  };

  InputBrowser(int,int,int,int,const char * = 0);
  ~InputBrowser() { input.parent(0); }
  static NamedStyle* default_style;

  virtual int handle(int);

  void maxw(int i) { maxw_ = i; }
  void maxh(int i) { maxh_ = i; }
  int maxw() { return maxw_; }
  int maxh() { return maxh_; }

  void value(const char *v) { input.value(v); }
  const char *value() { return input.value(); }

protected:
  virtual void draw();
  Input input;
  int minw_, minh_, maxw_, maxh_;
  int over_now, over_last;
};

}

#endif

//
// End of "$Id: InputBrowser.h,v 1.3 2003/04/20 03:17:47 easysw Exp $".
//
