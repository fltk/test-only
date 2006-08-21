//
// "$Id$"
//
// Simple Tree Widget for the Fast Light ToolKit (FLTK). 
//
//                   Copyright (c) 2003, O'ksi'D
//                      All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//      Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//      Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//      Neither the name of O'ksi'D nor the names of its contributors
//      may be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author : Jean-Marc Lienher (http://www.oksid.ch)
//

#ifndef FL_TREE_H
#define FL_TREE_H

#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>

/**
 * Not yet documented.
 */
class FL_EXPORT Fl_Tree : public Fl_Group {

  int pos;
  int hpos;
  int opos;
  int ohpos;
  int nb_alloc_lines;
  int top_line;
  int top_liney;
  int otop_y;

protected:
  char was_key;
  int height;
  int width;
  int level;
  int nb_lines;
  int *widths;
  int *heights; 
  int *flags;
  int *levels;
  void **datas;
  int sel;
  Fl_Scrollbar *scrollbar;	
  Fl_Scrollbar *hscrollbar;	
  Fl_Box *scrollbox;
  
  void draw_scroll();
  void draw_clip(int,int,int,int);
  static void draw_clipped(void*,int,int,int,int);
  void set_scroll();
  void draw_lines(int = 0);
  int find_below();
  void grow_lines(int l = 1);
  void dot_rect(int,int,int,int);

public:
  enum {
    FLAG_OPEN	= 0x0001,
    FLAG_FOLDER = 0x0002,
    FLAG_REDRAW = 0x0004,
    FLAG_SELECTED = 0x0008,
  };

  Fl_Tree(int, int, int, int, const char* = 0);
  ~Fl_Tree();

  int handle(int);
  void resize(int,int,int,int);
  void draw();
  void position(int);
  void hposition(int);
  int position() {return pos;}
  int hposition(){return hpos;}
  void *item_data(int);
  int get_selection(void) {return sel;}
  int item_max() {return nb_lines;}
  void *get_item(int n) {return datas[n];}
  int item_flags(int n) {return flags[n];}
  void item_set_flags(int n, int f) {flags[n] |= f;}
  void item_clear_flags(int n, int f) {flags[n] &= ~f;}
  void item_set(int n, void *d) {datas[n] = d; flags[n] = FLAG_REDRAW;}
  void item_size(int n, int w, int h) {widths[n] = w; heights[n] = h;}
  int item_width(int n) {return widths[n];} 
  int item_height(int n) {return heights[n];}

  // these functions _must_ be implemented in sub-classes :
  virtual void item_draw(int,int,int, int,int,int,int);
  virtual void item_measure(int);
  virtual int item_nb_children(int);
  virtual int item_has_children(int);
  virtual void *item_get_child(int, int);
  virtual void item_free(int);

  // optional 
  virtual int item_handle(int, int);
  virtual void item_open(int);
  virtual void item_close(int);
  virtual void item_select(int,int=1);
  virtual int item_selected(int);
  virtual void item_damage(int);
  virtual void root(void *);
  virtual void *root(void); 
};

#endif

//
// End of "$Id$".
//
