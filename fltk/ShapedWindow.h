//
// "$Id: ShapedWindow.h,v 1.3 2003/04/20 03:17:49 easysw Exp $"
//
// Window that can have a shape other than a rectangle. Right now the
// shape is controlled by a bitmap, which matches X, but we really could
// use a more powerful interface, perhaps tied into the path construction.
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

#ifndef fltk_ShapedWindow_h
#define fltk_ShapedWindow_h

#include <fltk/DoubleBufferWindow.h>
#include <fltk/xbmImage.h>

namespace fltk {

class FL_API ShapedWindow : public DoubleBufferWindow {
  public:
    ShapedWindow(int W, int H, const char *l = 0)
      : DoubleBufferWindow(W,H,l) { shape_ = 0; lw = lh = 0; changed = 0; }
    ShapedWindow(int X, int Y, int W, int H, const char *l = 0)
      : DoubleBufferWindow(X,Y,W,H,l) { shape_ = 0; lw = lh = 0; }
    void shape(xbmImage* b) { shape_ = b; changed = 1; }
    void shape(xbmImage& b) { shape(&b); }

  protected:
    virtual void draw();
    xbmImage* shape_;
    int lw, lh;
    int changed;
};

}
#endif

//
// End of "$Id: ShapedWindow.h,v 1.3 2003/04/20 03:17:49 easysw Exp $"
//
