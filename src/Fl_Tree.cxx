//
// "$Id$"
//
// Simple Tree Widget for the Fast Light ToolKit (FLTK). 
//
//                       Copyright (c) 2003, O'ksi'D
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
//
// Author : Jean-Marc Lienher (http://www.oksid.ch)
//

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Tree.H>
#include <stdlib.h>

static void scrollbar_callback(Fl_Widget* s, void*) {
  ((Fl_Tree*)(s->parent()))->position(int(((Fl_Scrollbar*)s)->value()));
}

static void hscrollbar_callback(Fl_Widget* s, void*) {
  ((Fl_Tree*)(s->parent()))->hposition(int(((Fl_Scrollbar*)s)->value()));
}

Fl_Tree::Fl_Tree(int X, int Y, int W, int H, const char* t) : 
    Fl_Group(X, Y, W,H, t) 
{
  pos = hpos = 0;
  width = 0;
  height = 0;
  level = 0;
  nb_lines = 0;
  nb_alloc_lines = 0;
  datas = NULL;
  sel = 0;
  otop_y = top_line = top_liney = 0; 
  levels = heights = flags = widths = NULL;
  scrollbar = new Fl_Scrollbar(X + W - 16, Y, 16, H - 16, 0);
  scrollbar->callback(scrollbar_callback);
  hscrollbar = new Fl_Scrollbar(X, Y + H - 16, W - 16, 16, 0); 
  hscrollbar->callback(hscrollbar_callback);
  hscrollbar->type(FL_HORIZONTAL);
  color(FL_BACKGROUND2_COLOR, FL_SELECTION_COLOR);
  box(FL_FLAT_BOX);
  scrollbox = new Fl_Box(X + W - 16, Y + H -16, 16, 16);
  scrollbox->box(FL_FLAT_BOX);
  set_scroll();
  when(FL_WHEN_NOT_CHANGED);
  end();
}

Fl_Tree::~Fl_Tree() {
  while (nb_lines > 0) {
    item_free(--nb_lines);
  }
  if (nb_alloc_lines > 0) {
    free(widths);
    free(heights);
    free(flags);
    free(datas);
  }
}

void Fl_Tree::grow_lines(int l) {
  if (nb_lines + 1 + l < nb_alloc_lines) return;
  nb_alloc_lines = nb_lines + l + 32;
  heights = (int*)realloc(heights, sizeof(int) * nb_alloc_lines);
  widths = (int*)realloc(widths, sizeof(int) * nb_alloc_lines);
  flags = (int*)realloc(flags, sizeof(int) * nb_alloc_lines);
  levels = (int*)realloc(levels, sizeof(int) * nb_alloc_lines);
  datas = (void**)realloc(datas, sizeof(void*) * nb_alloc_lines);
}

int Fl_Tree::handle(int e) {
  int r;

  if (e == FL_KEYDOWN) {
    if (sel >= nb_lines) sel = 0;
    if (Fl::event_key() == FL_Enter) {
      if (nb_lines && (flags[sel] & FLAG_OPEN)) {
        item_close(sel);
      } else {
        item_open(sel);
        damage(FL_DAMAGE_ALL);
      }
      if (!(Fl::event_state() & FL_CTRL)) {
        int i = 0;
        while (i < nb_lines) {
          if (i != sel) item_select(i, 0);
          i++;
        }  
      }
      if (!(flags[sel] & FLAG_FOLDER)) {
        item_select(sel, !item_selected(sel));
        if (when() & (FL_WHEN_NOT_CHANGED|FL_WHEN_CHANGED)) {
          do_callback(this, user_data());      
        }
      } else {
        if (when() & (FL_WHEN_NOT_CHANGED)) {
          do_callback(this, user_data());
        }
      }
      return 1; 
    }
    if (Fl::event_key() == FL_Up) {
      item_damage(sel);
      sel--;
      if (sel < 0) sel = 0;
      item_damage(sel);
      was_key = 1;
      return 1;
    }
    if (Fl::event_key() == FL_Down) {
     item_damage(sel);
     sel++;
     if (sel >= nb_lines) sel = nb_lines - 1;
     item_damage(sel);
     was_key = 1;
     return 1;
    }
  }
  if (e == FL_PUSH) {
    was_key = 0;
  }
  if (e == FL_FOCUS) {
    Fl::focus(this);
    return 1;
  }
  r = Fl_Group::handle(e);
  if (r) return r;

  if (e == FL_PUSH && (Fl::event_state() & FL_BUTTON1)) {
    int n = find_below();
    if (n >= 0) {
      int dx = x() - hpos + 20 * (levels[n] - 1);
      if (Fl::event_x() >= dx &&  
           Fl::event_x() <= dx + 20) 
      {
        if (flags[n] & FLAG_OPEN) {
           item_close(n);
         } else {
           item_open(n);
         }
         if (when() & (FL_WHEN_NOT_CHANGED)) {
           do_callback(this, user_data());
         }
         return 1; 
      }
      if (Fl::event_x() >= x() - hpos + 20 * levels[n]) {
	if (sel != n) {
		item_damage(sel);
        	sel = n;
		Fl::event_clicks(0);
	}
        if (!(Fl::event_state() & FL_CTRL)) {
          int i = 0;
          while (i < nb_lines) {
            if (i != n) item_select(i, 0);
            i++;
          }  
        } 
        if (Fl::event_clicks()) {
          int s = item_selected(n);
          item_select(n);
          if (when() & (FL_WHEN_NOT_CHANGED) || 
             ((s != item_selected(n)) && (when() & FL_WHEN_CHANGED))) 
          {
            do_callback(this, user_data());      
          }
          //Fl::event_clicks(0);  
        } else {
	  item_select(n, !item_selected(n));
          if (when() & (FL_WHEN_NOT_CHANGED|FL_WHEN_CHANGED)) {
            do_callback(this, user_data());      
          }
        } 
      }
    }
  }
 // if (e == FL_DND_ENTER) return 1;
 // if (e == FL_DND_LEAVE)  return 1;
  if (e == FL_DND_RELEASE || e == FL_DND_DRAG) {
	int n = find_below();
	int os = sel;
	if (n >= 0) sel = n;
	else return 0;
	if (sel != os) {
          item_damage(sel);
          item_damage(os);
	} 
  }
  return item_handle(sel, e);
}

void  Fl_Tree::draw_clipped(void* v,int X, int Y, int W, int H) {
  ((Fl_Tree*)v)->draw_clip(X, Y, W, H);
}

void Fl_Tree::draw() {
  if (was_key && sel < nb_lines) {
    int i = 0;
    int dy = y() - pos;
    while (i < sel) {
       dy += heights[i++];
    }
    was_key = 0;
    if (dy < y()) {
      pos -= y() - dy;
      set_scroll();
      was_key = 2;  
    } else if (dy + heights[sel] > y() + h() - 16) { 
      pos += (dy + heights[sel]) - (y() + h() - 16);
      set_scroll(); 
      was_key = 2;  
    }
  }
  if (damage() == (FL_DAMAGE_SCROLL|FL_DAMAGE_CHILD)) {
    fl_scroll(x(), y(), w() - 16, h() - 16, ohpos-hpos, opos-pos, draw_clipped, this);
    if (was_key == 2) {
      was_key = 0;
      clear_damage();
      damage(FL_DAMAGE_CHILD);
      draw_clip(x(), y(), w(), h());
    }
  } else {
    draw_clip(x(), y(), w(), h());
  }
  was_key = 0;
  draw_scroll();
  opos = pos;
  ohpos = hpos;
}

void Fl_Tree::draw_clip(int X, int Y, int W, int H) {
  int all = 0;
  int i = 0;
  int p = 0;
  int d = 0;
  int draw = 0;

  fl_clip(X,Y,W,H);

  if ((width + 20 * level) < w() && height < h()) {
    scrollbar->clear_visible();
    hscrollbar->clear_visible();
    scrollbox->box(FL_NO_BOX);
    pos = hpos = 0;
  } else if (!hscrollbar->visible()) {
    scrollbar->set_visible();
    hscrollbar->set_visible();
    scrollbox->box(FL_FLAT_BOX);
    d = 16;
    damage(FL_DAMAGE_ALL);
  } else  {
    d = 16;
  }

  if (damage() & ~FL_DAMAGE_CHILD) {
    fl_clip(x(), y(), w() - d, h() - d); 
    draw_box();
    draw_label();
    fl_pop_clip();
    all = 1;
  }
  
  p = y() - pos; 
  while (i < nb_lines) {
    int op = p;
    if (op > y() + h()) break;
    p += heights[i];
    if (p >= y() && (all || flags[i] & FLAG_REDRAW)) {
      int Y, H;
      int dl;
      Y = op;
      H = heights[i];
      if (Y < y()) {
	Y = y();
        H -= y() - op;
      }
      dl = (Y + H) - (y() + h() - d);
      if (dl > 0) {
	H -= dl;
      } else {
        dl = 0;
      }
      fl_clip(x(), Y, w() - d, H);
      draw++;
      if (!all) draw_box(); 
      item_draw(i, x()-hpos+20 * levels[i], op, x(), Y, w() - d, H);
      if (was_key == 2 && draw < 2)  draw_lines();   
      fl_pop_clip();  
    }
    if (was_key != 2) flags[i] &= ~FLAG_REDRAW;
    i++;
  }

  if (draw > 0 && (was_key != 2 || draw >= 2)) {
    fl_clip(x(), y(), w() - d, h() - d); 
    draw_lines();
    fl_pop_clip();
  }
  fl_pop_clip();
}

void Fl_Tree::draw_scroll() {
  if (damage() & ~(FL_DAMAGE_CHILD|FL_DAMAGE_SCROLL)) { 
    Fl_Widget& o = *scrollbar;
    draw_child(o);
    draw_outside_label(o);
    Fl_Widget& oo = *hscrollbar;
    draw_child(oo);
    draw_outside_label(oo);
    Fl_Widget& oi = *scrollbox;
    draw_child(oi);
    draw_outside_label(oi);
  } else {
    Fl_Widget& o = *scrollbar;
    update_child(o);
    Fl_Widget&oo = *hscrollbar;
    update_child(oo);
    Fl_Widget& oi = *scrollbox;
    update_child(oi);
  }
}


void Fl_Tree::resize(int X, int Y, int W, int H) {
  Fl_Group::resize(X, Y, W, H);
  scrollbar->resize(X + W - 16, Y, 16, H - 16);
  hscrollbar->resize(X, Y + H - 16, W - 16, 16);  
  scrollbox->resize(X + W - 16, Y + H - 16, 16, 16);
}

void Fl_Tree::item_close(int n) {
  int nb, l;
  if (!(flags[n] & FLAG_OPEN) || 
      !(flags[n] & FLAG_FOLDER)) return;
  flags[n] &= ~FLAG_OPEN;
  flags[n] |= FLAG_REDRAW;
  damage(FL_DAMAGE_CHILD);
  item_damage(sel);
  sel = n;
  l = levels[n];
  nb = n + 1;
  while (nb < nb_lines && levels[nb] > l) {
    height -= heights[nb];
    item_free(nb);
    nb++;
  }
  n++;
  if (nb == n) return;

  damage(FL_DAMAGE_ALL);
  while (nb < nb_lines) {
    levels[n] = levels[nb];
    datas[n] = datas[nb];
    widths[n] = widths[nb];
    heights[n] = heights[nb];
    flags[n] = flags[nb];
    nb++; n++;
  }
  nb_lines = n;
  
  nb = 0;
  width = 0;
  level = 0;
  while (nb < nb_lines) {
    if (widths[nb] > width) width = widths[nb]; 
    if (levels[nb] > level) level = levels[nb];
    nb++;
  }
  set_scroll();
}

void Fl_Tree::item_free(int n) {
  //free(data[n]);
}

void Fl_Tree::item_open(int n) {
  int nb, i, no, lv;
  if ((flags[n] & FLAG_OPEN) ||
      !(flags[n] & FLAG_FOLDER)) return;
  flags[n] |= FLAG_OPEN;
  flags[n] |= FLAG_REDRAW;
  damage(FL_DAMAGE_CHILD);
  no = nb = item_nb_children(n);
  item_damage(sel);
  sel = n;
  if (nb < 1) return;
  damage(FL_DAMAGE_ALL);
  grow_lines(nb);
  nb = nb_lines - 1;
  nb_lines += no;
  i = nb_lines - 1;
  while (nb > n) {
    datas[i] = datas[nb];
    levels[i] = levels[nb];
    widths[i] = widths[nb];
    heights[i] = heights[nb];
    flags[i] = flags[nb] | FLAG_REDRAW;
    i--; nb--;
  }
  
  i = 0;
  lv = levels[n] + 1;
  while (i < no) {
    int it = i + n + 1;
    datas[n+i+1] = item_get_child(n, i + 1);
    flags[n+i+1] = FLAG_REDRAW;
    levels[it] = lv;
    i++;	
  }  

  i = 0;
  while (i < no) {
    int it = i + n + 1;    
    if (item_has_children(it) >= 0) {
      flags[it] |= FLAG_FOLDER;
    } 
    flags[it] |= FLAG_REDRAW;
    item_measure(it);
    height += heights[it];
    if (widths[it] > width) width = widths[it];
    if (levels[it] > level) level = levels[it];
    i++;
  }	
  set_scroll();
}

void Fl_Tree::item_measure(int n) {
  Fl_Menu_Item *m;
  if (!datas[n]) return;
  m = (Fl_Menu_Item*) datas[n];
  heights[n] = 0;
  widths[n] = m->measure(heights + n, NULL);
}

void Fl_Tree::dot_rect(int x, int y, int w, int h) {
  fl_line_style(FL_DOT);
  fl_line(x,y, x+w-1, y);
  fl_line(x, y, x, y+h);
  fl_line(x+w-1, y, x+w-1, y+h);
  fl_line(x, y+h-1, x+w-1, y+h-1);
  fl_line_style(FL_SOLID);
}

void Fl_Tree::item_draw(int n,int X,int Y, 
	int CX,int CY, int CW,int CH) 
{
  Fl_Menu_Item *m;
  if (!datas[n]) return;
  m = (Fl_Menu_Item*) datas[n];
  m->draw(X, Y, widths[n] + 5, heights[n], 
          NULL, (flags[n] & FLAG_SELECTED) ? 1 : 0);
  if (sel == n) {
     dot_rect(X, Y, widths[n] + 5, heights[n]);   
  }
}

int Fl_Tree::item_handle(int n, int e) {
  if (e == FL_PUSH) {
  }
  return 0;
}

void Fl_Tree::root(void *rt) {
  while (nb_lines > 0) {
    item_free(--nb_lines);
  } 
  grow_lines();
  nb_lines = 1;
  datas[0] = rt;
  levels[0] = 0;
  flags[0] = FLAG_FOLDER|FLAG_REDRAW;
  item_measure(0);
  item_open(0);
}

void *Fl_Tree::root(void) {
  if (!nb_lines) return NULL;
  return datas[0];
}

int Fl_Tree::item_has_children(int n) {
  Fl_Menu_Item *m;
  if (!datas[n]) return 0;
  m = (Fl_Menu_Item*) datas[n];
  if (!(m->flags & FL_SUBMENU)) return -1;
  return 1;
}

int Fl_Tree::item_nb_children(int n) {
  int i = 0;
  int lev = 0;
  Fl_Menu_Item *m;
  if (!datas[n]) return 0;
  m = (Fl_Menu_Item*) datas[n];
  if (!(m->flags & FL_SUBMENU)) return -1;

  while (lev >= 0) {
    m++;
    if (!m->text) {
      lev--;
    } else if (lev == 0) {
      i++;
    } 
    if (m->flags & FL_SUBMENU) lev++;
    if (lev < 0) return i;
  } 
  return i;
}

void *Fl_Tree::item_get_child(int n, int c) {
  Fl_Menu_Item *m;
  int lev = 0;
  int i = 0;
  if (!datas[n]) return NULL;
  m = (Fl_Menu_Item*) datas[n];
  while (i < c) {
    if (lev == 0) i++;
    m++;
    if (m->flags & FL_SUBMENU) lev++;
    if (!m->text) lev--;
    if (lev < 0) return NULL;
  }
  return (void*) m;
}

void Fl_Tree::set_scroll() {
  if (hpos % 2) hpos += 1;
  if (pos % 2) pos += 1;
  scrollbar->value(pos, h() -32, 0, height);
  scrollbar->linesize(20);
  hscrollbar->value(hpos, w()-20, 0, width + 20 * level);
  hscrollbar->linesize(w() / 4); 
  damage(FL_DAMAGE_SCROLL);
 
}

void Fl_Tree::position(int p) {
  if (p < 0) p = 0;
  if (p == pos) return;
  pos = p;
  set_scroll();
}

void Fl_Tree::hposition(int p) {
  if (p < 0) p = 0;
  if (p == hpos) return;
  hpos = p;
  set_scroll();
}

void Fl_Tree::draw_lines(int l) {
  int i, X;
  int start, p, end;
  int next = 0;
  fl_color(FL_BLACK);
  fl_line_style(FL_DOT);
  i = 0;
  start = 0xffffff;
  end = 0;
  p = y() - pos;
  X = x() - hpos;
  int YY, YH;
  YY = y() - h();
  YH = y() + 2 * h();
  while (i < nb_lines) {
    if (levels[i] == l + 1) {
      if (start == 0xffffff) {
        start = p;
        end = p;
      } else {
        end = p;
      }
      next = 1;
    } else if (levels[i] <= l &&  
               start != 0xffffff) 
    {
      if (start < YY) start = YY;
      if (end > YH) end = YH;
      if (start < YH && end > YY) {
        fl_line(X + 8 + 20 * l, start, 
              X + 8 + 20 * l, end + 8); 
        
      }
      start = 0xffffff;
    } 
    p += heights[i];
    i++;
  }
  if (start != 0xffffff) {
      if (start < YY) start = YY;
      if (end > YH) end = YH;
      if (start < YH && end > YY) {
        fl_line(X + 8 + 20 * l, start, 
              X + 8 + 20 * l, end + 8); 
      }
  }

  i = 0;
  p = y() - pos;
  while (i < nb_lines) {
    if (levels[i] == l + 1 && p >= y() - h() && 
         p + heights[i] < y() + h()) 
    {
      fl_color(FL_BLACK);
      fl_line_style(FL_DOT);
      fl_line(X + 8 + 20 * l, p + 8, 
              X + 20 + 20 * l, p + 8);
      fl_line_style(FL_SOLID);
      if (flags[i] & FLAG_FOLDER) {
        fl_color(FL_DARK3);
        fl_rect(X + 4 + 20 * l, p + 4, 9, 9);
        fl_color(FL_WHITE);
        fl_rectf(X + 5 + 20 * l, p + 5, 7, 7);
        fl_color(FL_BLACK);
        fl_line(X + 6 + 20 * l, p + 8,
                X + 10 + 20 * l, p + 8);
        if (!(flags[i] & FLAG_OPEN)) {
          fl_line(X + 8 + 20 * l, p + 6, 
                  X + 8 + 20 * l, p + 10);
        }
      } 
    }
    p += heights[i];
    i++;
  }
 
  if (next) draw_lines(l + 1);
  fl_line_style(FL_SOLID);
}

int Fl_Tree::find_below() {
  int i = 0;
  int dy = y() - pos;
  int dx = x() - hpos;
  int ey = Fl::event_y();
  int X = x();
  if (otop_y != dy) {
    otop_y = top_liney = dy;
    while (i < nb_lines) {
      top_liney += heights[i];
      if (top_liney >= X) {
        top_liney -= heights[i];
        top_line = i;
        break;
      } 
      i++;
    }
  }
  i = top_line;
  dy = top_liney;
  while (i < nb_lines) {
    if (ey >= dy && 
        Fl::event_y() <= (dy + heights[i]) &&
        Fl::event_x() <= (dx + widths[i] + (20 * levels[i]) + 5))
    {
      return i;
    }
    dy += heights[i];
    i++;
  }
  return -1;
} 
   
int Fl_Tree::item_selected(int n) {
  if (n >= nb_lines) return 0;
  return flags[n] & FLAG_SELECTED;
}

void Fl_Tree::item_select(int n, int s) {
  if (n >= nb_lines) return;
  if (s) {
    if (flags[n] & FLAG_SELECTED) return;
    else flags[n] |= FLAG_SELECTED;
  } else {
    if (!(flags[n] & FLAG_SELECTED)) return;
    else flags[n] &= ~FLAG_SELECTED;
  }
  flags[n] |= FLAG_REDRAW;
  damage(FL_DAMAGE_CHILD);  
}

void Fl_Tree::item_damage(int n) {
  if (n >= nb_lines || n < 0) return;
  flags[n] |= FLAG_REDRAW;
  damage(FL_DAMAGE_CHILD);
}

void* Fl_Tree::item_data(int n) {
  if (n >= nb_lines || n < 0) return NULL;
  return datas[n];
}

//
// End of "$Id$".
//
