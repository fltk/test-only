//
// "$Id: Input.h,v 1.4 2003/12/15 03:03:13 spitzak Exp $"
//
// One-line text input field.
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

#ifndef fltk_Input_h
#define fltk_Input_h

#ifndef fltk_Widget_h
#include "Widget.h"
#endif

namespace fltk {

class FL_API Input : public Widget {
public:
  enum { // values for type()
    NORMAL = 0,
    // this hole was used by float & int types
    SECRET = 3,
    MULTILINE = 4,
    WORDWRAP = 5
  };

  Input(int, int, int, int, const char* = 0);
  ~Input();
  static NamedStyle* default_style;

  void draw();
  void draw(int, int, int, int);
  int handle(int);
  int handle(int event, int, int, int, int);
  bool handle_key();

  bool value(const char*);
  bool value(const char*, int);
  bool static_value(const char*);
  bool static_value(const char*, int);
  const char* value() const {return value_;}
  char index(int i) const {return value_[i];}
  int size() const {return size_;}
  void reserve(int newsize);

  int position() const {return position_;}
  int mark() const {return mark_;}
  void position(int p, int m);
  void position(int p) {position(p, p);}
  void up_down_position(int position, bool extend);
  void mark(int m) { position(position(), m);}

  virtual bool replace(int, int, const char*, int);
  bool cut() {return replace(position(), mark(), 0, 0);}
  bool cut(int n) {return replace(position(), position()+n, 0, 0);}
  bool cut(int a, int b) {return replace(a, b, 0, 0);}
  bool insert(const char* t);
  bool insert(const char* t, int l){return replace(position_, mark_, t, l);}
  bool replace(int a, int b, char c) {return replace(a,b,&c,1);}
  bool copy(bool clipboard = true);
  bool undo();
  void maybe_do_callback();

  int word_start(int i) const;
  int word_end(int i) const;
  int line_start(int i) const;
  int line_end(int i) const;
  int mouse_position(int, int, int, int) const;
  int xscroll() const {return xscroll_;}
  int yscroll() const {return yscroll_;}

private:

  const char* value_;
  char* buffer;

  int size_;
  int bufsize;
  int position_;
  int mark_;
  int xscroll_, yscroll_;
  int mu_p;
  int label_width;

  const char* expand(const char*, char*, int) const;
  float expandpos(const char*, const char*, const char*, int*) const;
  void minimal_update(int, int);
  void minimal_update(int p);
  void erase_cursor_at(int p);

  void setfont() const;

  bool key_is_shortcut();
  void shift_position(int p);
  void shift_up_down_position(int p);

};

}

#endif

//
// End of "$Id: Input.h,v 1.4 2003/12/15 03:03:13 spitzak Exp $".
//
