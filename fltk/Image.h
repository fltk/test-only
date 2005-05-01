//
// "$Id$"
//
// Image object used to label widgets. This caches the image in a
// server pixmap. Subclasses are used to decide how to change data
// into an image.
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

#ifndef fltk_Image_h
#define fltk_Image_h

#include "Symbol.h"

namespace fltk {

class FL_API Widget;

class FL_API Image : public Symbol {
protected:
  int w_, h_;
  void* rgb; // system-specific thing holding rgb (and maybe alpha)
  void* alpha; // system-specific thing holding alpha if necessary

public:

  Image(const char* name=0) : Symbol(name), w_(-1), h_(-1), rgb(0), alpha(0) {}
  Image(int w, int h, const char* name=0) : Symbol(name), w_(w), h_(h), rgb(0), alpha(0) {}

  int w() const {return w_;}
  int width() const {return w_;}
  int h() const {return h_;}
  int height() const {return h_;}

  void setsize(int w, int h);
  bool drawn() const {return rgb || alpha;}
  void make_current();
  void set_alpha_bitmap(const uchar* bitmap, int w, int h);
  void copy(const Rectangle&, int src_x, int src_y) const;
  void over(const Rectangle&, int src_x, int src_y) const;
  void fill(const Rectangle&, int src_x, int src_y) const;
  void destroy_cache();

  // implementation as Symbol subclass:
  void _draw(const Rectangle&) const;
  void _measure(int& W, int& H) const;
  bool fills_rectangle() const;

  // for back compatability with fltk1 only:
  void label(Widget* o);

#if 0 // FLTK 1.1 interface, we should emulate some of this!
  int w_, h_, d_, ld_, count_;
  const char * const *data_;

  protected:

  void		w(int W) {w_ = W;}
  void		h(int H) {h_ = H;}
  void		d(int D) {d_ = D;}
  void		ld(int LD) {ld_ = LD;}
  void		data(const char * const *p, int c) {data_ = p; count_ = c;}
  void		draw_empty(int X, int Y);

  public:

  int		w() const { return w_; }
  int		h() const { return h_; }
  int		d() const { return d_; }
  int		ld() const { return ld_; }
  int		count() const { return count_; }
  const char * const *data() const { return data_; }

  		Image(int W, int H, int D) { w_ = W; h_ = H; d_ = D; ld_ = 0; count_ = 0; data_ = 0;}
  virtual	~Image();
  virtual	Image *copy(int W, int H);
  Image		*copy() { return copy(w(), h()); }
  virtual void	color_average(Fl_Color c, float i);
  void		inactive() { color_average(FL_GRAY, .33f); }
  virtual void	desaturate();
  virtual void	draw(int X, int W, int W, int H);
  void		draw(int X, int Y) { draw(X, Y, w(), h()); }
  virtual void	measure(int& W, int& H);
  virtual void	uncache();

#endif // 0
};

}

#endif

//
// End of "$Id$".
//
