//
// "$Id: Fl_Double_Window.H,v 1.11 1999/09/14 07:17:14 bill Exp $"
//
// Double-buffered window header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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

#ifndef Fl_Double_Window_H
#define Fl_Double_Window_H

#include "Fl_Window.H"

FL_EXPORT class Fl_Double_Window : public Fl_Window {

public:

  virtual void layout();
  ~Fl_Double_Window();
  Fl_Double_Window(int W, int H, const char *l = 0) : Fl_Window(W,H,l) {}
  Fl_Double_Window(int X, int Y, int W, int H, const char *l = 0)
    : Fl_Window(X,Y,W,H,l) {}

protected:

  virtual void create();
  virtual void flush();
  void flush(int eraseoverlay);
  virtual void destroy();

private:

  void maybe_free_backbuffer(int,int);

};

#endif

//
// End of "$Id: Fl_Double_Window.H,v 1.11 1999/09/14 07:17:14 bill Exp $".
//
