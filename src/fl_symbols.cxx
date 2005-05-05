//
// "$Id$"
//
// Symbol drawing code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

// These are small graphics drawn by the normal label-drawing
// code when the string starts with an '@' sign.

// Adapted from original code written by:

// Written by Mark Overmars
// Version 2.1 a
// Date: Oct  2, 1992
// Modified by 

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Symbol.H>
#include "flstring.h"
#include <FL/Fl_Image.H>
#include <FL/Fl_Hash_Table.H>

#define DELETED ((const char*)1)

// Hash tables were stolen from fltk-2.0 but modified to be more generic.
// so they can be used not only for Symbols but also for other purposes.



Fl_Hash_Table::Fl_Hash_Table(int s){
  size = s;
  no_items = 0;
  table = new Fl_Hash_Item[s];
  memset(table, 0, s * sizeof(Fl_Hash_Item));
}

void * Fl_Hash_Table::find(const char *name){
  return table[index(name, strlen(name), false)].data;
}

// returns hash entry if it exists, or first empty slot:
unsigned Fl_Hash_Table::index(const char *name, unsigned n, bool delok) {
  // Calculate the hash index:
  unsigned pos = 0;
  unsigned i; for (i = 0; i < n; i++) pos = 37*pos + name[i];
  pos %= size;
  // Quadratic probing through the hash table to find empty slot
  // or a null:
  for (i = 0;;) {
    Fl_Hash_Item * s = table+pos;
    if (!(s->name)) break;
    if (s->name == DELETED) {if (delok) break;}
    else if (!strncmp(s->name, name, n) && !s->name[n]) break;
    pos += 2 * ++i - 1;
    pos %= size;
  }
  return pos;
}

void Fl_Hash_Table::double_table() {
  // Remember the old table:
  Fl_Hash_Item * oldtable = table;
  int oldsize = size;
  // figure out a new size that is prime and >= 2 * currentsize.
  int n = 2*size+1;
  for (int i = 3; i*i <= n; i += 2) while (n%i == 0) {i = 3; n += 2;}
  //printf("Realloc to new table size of %d\n", n);
  // allocate new table and set it all to zero:
  size = n;
  table = new Fl_Hash_Item[size];
  memset(table, 0, size * sizeof(Fl_Hash_Item));
  // copy all the symbols over:
  for (n = 0; n < oldsize; n++) {
    Fl_Hash_Item s = oldtable[n];
    if (s.name && (s.name != DELETED)) {
      int pos = index(s.name, strlen(s.name), true);
      table[pos] = s;
    }
  }
  // throw away old table:
  delete[] oldtable;
}


void * Fl_Hash_Table::set(const char* name, void * data) {
  if(!name) return 0;
  int pos = index(name, strlen(name), true);
  if(!(table[pos].name)  || (table[pos].name !=DELETED))
    no_items ++;
  void * olddata = table[pos].data;
  table[pos].name = name;
  table[pos].data = data;
  if (no_items > size/2) double_table();
  return olddata;
}

void * Fl_Hash_Table::remove(const char* name){
  int pos = index(name, strlen(name), true);
  void * olddata = 0;
  if(table[pos].name){
    table[pos].name = DELETED;
    olddata = table[pos].data;
    table[pos].data = 0;
  }
  return olddata;
}



static Fl_Hash_Table * symbol_table = 0;

static void fl_init_symbols();


/**************** The routines seen by the user *************************/



Fl_Symbol * fl_set_symbol(const char * name, Fl_Symbol * s){
  fl_init_symbols();
  return (Fl_Symbol *) symbol_table->set(name,s);
}

Fl_Symbol * fl_find_symbol(const char * name){
  fl_init_symbols();
  return (Fl_Symbol *) (symbol_table->find(name));
};
  


static double rotangle = 0;


//extern FL_EXPORT void fl_return_arrow(int, int, int, int, Fl_Color); 


// provided for back compatability:
int fl_draw_symbol(const char *label,int x,int y,int w,int h,Fl_Color col) {  
  const char *p = label;
  if (*p++ != '@') return 0;
  fl_init_symbols();
  int equalscale = 0;
  if (*p == '#') {equalscale = 1; p++;}
  if (*p == '-' && p[1]>='1' && p[1]<='9') {
    int n = p[1]-'0';
    x += n; y += n; w -= 2*n; h -= 2*n;
    p += 2;
  } else if (*p == '+' && p[1]>='1' && p[1]<='9') {
    int n = p[1]-'0';
    x -= n; y -= n; w += 2*n; h += 2*n;
    p += 2;
  }
  if (w < 10) {x -= (10-w)/2; w = 10;}
  if (h < 10) {y -= (10-h)/2; h = 10;}
  w = (w-1)|1; h = (h-1)|1;
  switch (*p++) {
  case '0':
    rotangle = 100*(p[1]-'0') + 10*(p[2]-'0') + 1*(p[3]-'0');
    p += 4;
    break;
  case '1': rotangle = 225; break;
  case '2': rotangle = 270; break;
  case '3': rotangle = 315; break;
  case '4': rotangle = 180; break;
  case '5':
  case '6': rotangle = 0; break;
  case '7': rotangle = 135; break;
  case '8': rotangle =  90; break;
  case '9': rotangle =  45; break;
  default: rotangle = 0; p--; break;
  }
  Fl_Symbol * s = fl_find_symbol(p);
  if(s){
    s->draw(x,y,w,h,col);
    rotangle = 0;
    return 1;
  }
  rotangle = 0;
  /*
  int pos = find(p);
  if (!symbols[pos].notempty) return 0;
  if (symbols[pos].scalable == 3) { // kludge to detect return arrow
    fl_color(col);
    ((Fl_Draw_Symbol_)symbols[pos].drawit)(x,y,w,h,col);
    return 1;
  }
  fl_push_matrix();
  fl_translate(x+w/2,y+h/2);
  if (symbols[pos].scalable) {
    if (equalscale) {if (w<h) h = w; else w = h;}
    fl_scale(0.5*w, 0.5*h);
    fl_rotate(rotangle/10.0);
  }
  ((Fl_Draw_Symbol)symbols[pos].drawit)(col);
  fl_pop_matrix();
  */
  return 0;
}


/******************** THE DEFAULT SYMBOLS ****************************/

/* Some help stuff */

#define BP fl_begin_polygon()
#define EP fl_end_polygon()
#define BCP fl_begin_complex_polygon()
#define ECP fl_end_complex_polygon()
#define BL fl_begin_line()
#define EL fl_end_line()
#define BC fl_begin_loop()
#define EC fl_end_loop()
#define vv(x,y) fl_vertex(x,y)

//for the outline color
static void set_outline_color(Fl_Color c) {
  fl_color(fl_darker(c));
}

static void rectangle(double x,double y,double x2,double y2,Fl_Color col) {
  fl_color(col);
  BP; vv(x,y); vv(x2,y); vv(x2,y2); vv(x,y2); EP;
  set_outline_color(col);
  BC; vv(x,y); vv(x2,y); vv(x2,y2); vv(x,y2); EC;
}

/* The drawing routines */

static void draw_fltk(Fl_Color col) 
{
  fl_color(col);
  // F fill
  BCP; vv(-2.0, -0.5); vv(-1.0, -0.5); vv(-1.0, -0.3); vv(-1.8, -0.3);
  vv(-1.8, -0.1); vv(-1.2, -0.1); vv(-1.2, 0.1); vv(-1.8, 0.1);
  vv(-1.8, 0.5); vv(-2.0, 0.5); ECP;
  // L fill
  BCP; vv(-1.0, -0.5); vv(-0.8, -0.5); vv(-0.8, 0.3); vv(0.0, 0.3);
  vv(0.0, 0.5); vv(-1.0, 0.5); ECP;
  // T outline
  BCP; vv(-0.1, -0.5); vv(1.1, -0.5); vv(1.1, -0.3); vv(0.6, -0.3);
  vv(0.6, 0.5); vv(0.4, 0.5); vv(0.4, -0.3); vv(-0.1, -0.3); ECP;
  // K outline
  BCP; vv(1.1, -0.5); vv(1.3, -0.5); vv(1.3, -0.15); vv(1.70, -0.5);
  vv(2.0, -0.5); vv(1.43, 0.0); vv(2.0, 0.5); vv(1.70, 0.5);
  vv(1.3, 0.15); vv(1.3, 0.5); vv(1.1, 0.5); ECP;
  set_outline_color(col);
  // F outline
  BC; vv(-2.0, -0.5); vv(-1.0, -0.5); vv(-1.0, -0.3); vv(-1.8, -0.3);
  vv(-1.8, -0.1); vv(-1.2, -0.1); vv(-1.2, 0.1); vv(-1.8, 0.1);
  vv(-1.8, 0.5); vv(-2.0, 0.5); EC;
  // L outline
  BC; vv(-1.0, -0.5); vv(-0.8, -0.5); vv(-0.8, 0.3); vv(0.0, 0.3);
  vv(0.0, 0.5); vv(-1.0, 0.5); EC;
  // T outline
  BC; vv(-0.1, -0.5); vv(1.1, -0.5); vv(1.1, -0.3); vv(0.6, -0.3);
  vv(0.6, 0.5); vv(0.4, 0.5); vv(0.4, -0.3); vv(-0.1, -0.3); EC;
  // K outline
  BC; vv(1.1, -0.5); vv(1.3, -0.5); vv(1.3, -0.15); vv(1.70, -0.5);
  vv(2.0, -0.5); vv(1.43, 0.0); vv(2.0, 0.5); vv(1.70, 0.5);
  vv(1.3, 0.15); vv(1.3, 0.5); vv(1.1, 0.5); EC;
}

static void draw_search(Fl_Color col) 
{
  fl_color(col);
  BP; vv(.4, .13); vv(1.0, .73); vv(.73, 1.0); vv(.13, .4); EP;
  set_outline_color(col);
  fl_line_style(FL_SOLID, 3, 0);
  BC; fl_circle(-.2, -.2, .7); EC;
  fl_line_style(FL_SOLID, 1, 0);
  BC; vv(.4, .13); vv(1.0, .73); vv(.73, 1.0); vv(.13, .4); EC;
}

static void draw_arrow1(Fl_Color col)
{
  fl_color(col);
  BP; vv(-0.8,-0.4); vv(-0.8,0.4); vv(0.0,0.4); vv(0.0,-0.4); EP;
  BP; vv(0.0,0.8); vv(0.8,0.0); vv(0.0,-0.8); vv(0.0,-0.4); vv(0.0,0.4); EP;
  set_outline_color(col);
  BC; vv(-0.8,-0.4); vv(-0.8,0.4); vv(0.0,0.4); vv(0.0,0.8); vv(0.8,0.0);
      vv(0.0,-0.8); vv(0.0,-0.4); EC;
}

static void draw_arrow1bar(Fl_Color col)
{
  draw_arrow1(col);
  rectangle(.6,-.8,.9,.8,col);
}

static void draw_arrow2(Fl_Color col)
{
  fl_color(col);
  BP; vv(-0.3,0.8); vv(0.50,0.0); vv(-0.3,-0.8); EP;
  set_outline_color(col);
  BC; vv(-0.3,0.8); vv(0.50,0.0); vv(-0.3,-0.8); EC;
}

static void draw_arrow3(Fl_Color col)
{
  fl_color(col);
  BP; vv(0.1,0.8); vv(0.9,0.0); vv(0.1,-0.8); EP;
  BP; vv(-0.7,0.8); vv(0.1,0.0); vv(-0.7,-0.8); EP;
  set_outline_color(col);
  BC; vv(0.1,0.8); vv(0.9,0.0); vv(0.1,-0.8); EC;
  BC; vv(-0.7,0.8); vv(0.1,0.0); vv(-0.7,-0.8); EC;
}

static void draw_arrowbar(Fl_Color col)
{
  fl_color(col);
  BP; vv(0.2,0.8); vv(0.6,0.8); vv(0.6,-0.8); vv(0.2,-0.8); EP;
  BP; vv(-0.6,0.8); vv(0.2,0.0); vv(-0.6,-0.8); EP;
  set_outline_color(col);
  BC; vv(0.2,0.8); vv(0.6,0.8); vv(0.6,-0.8); vv(0.2,-0.8); EC;
  BC; vv(-0.6,0.8); vv(0.2,0.0); vv(-0.6,-0.8); EC;
}

static void draw_arrowbox(Fl_Color col)
{
  fl_color(col);
  BP; vv(-0.6,0.8); vv(0.2,0.0); vv(-0.6,-0.8); EP;
  BC; vv(0.2,0.8); vv(0.6,0.8); vv(0.6,-0.8); vv(0.2,-0.8); EC;
  set_outline_color(col);
  BC; vv(0.2,0.8); vv(0.6,0.8); vv(0.6,-0.8); vv(0.2,-0.8); EC;
  BC; vv(-0.6,0.8); vv(0.2,0.0); vv(-0.6,-0.8); EC;
}

static void draw_bararrow(Fl_Color col)
{
  fl_color(col);
  BP; vv(0.1,0.8); vv(0.9,0.0); vv(0.1,-0.8); EP;
  BP; vv(-0.5,0.8); vv(-0.1,0.8); vv(-0.1,-0.8); vv(-0.5,-0.8); EP;
  set_outline_color(col);
  BC; vv(0.1,0.8); vv(0.9,0.0); vv(0.1,-0.8); EC;
  BC; vv(-0.5,0.8); vv(-0.1,0.8); vv(-0.1,-0.8); vv(-0.5,-0.8); EC;
}

static void draw_doublebar(Fl_Color col) { 
  rectangle(-0.6,-0.8,-.1,.8,col);
  rectangle(.1,-0.8,.6,.8,col); 
}

static void draw_arrow01(Fl_Color col)
  { fl_rotate(180); draw_arrow1(col); }

static void draw_arrow02(Fl_Color col)
  { fl_rotate(180); draw_arrow2(col); }

static void draw_arrow03(Fl_Color col)
  { fl_rotate(180); draw_arrow3(col); }

static void draw_0arrowbar(Fl_Color col)
  { fl_rotate(180); draw_arrowbar(col); }

static void draw_0arrowbox(Fl_Color col)
  { fl_rotate(180); draw_arrowbox(col); }

static void draw_0bararrow(Fl_Color col)
  { fl_rotate(180); draw_bararrow(col); }

static void draw_doublearrow(Fl_Color col)
{
  fl_color(col);
  BP; vv(-0.35,-0.4); vv(-0.35,0.4); vv(0.35,0.4); vv(0.35,-0.4); EP;
  BP; vv(0.15,0.8); vv(0.95,0.0); vv(0.15,-0.8); EP;
  BP; vv(-0.15,0.8); vv(-0.95,0.0); vv(-0.15,-0.8); EP;
  set_outline_color(col);
  BC; vv(-0.15,0.4); vv(0.15,0.4); vv(0.15,0.8); vv(0.95,0.0);
      vv(0.15,-0.8); vv(0.15,-0.4); vv(-0.15,-0.4); vv(-0.15,-0.8);
      vv(-0.95,0.0); vv(-0.15,0.8); EC;
}

static void draw_arrow(Fl_Color col)
{
  fl_color(col);
  BP; vv(0.65,0.1); vv(1.0,0.0); vv(0.65,-0.1); EP;
  BL; vv(-1.0,0.0); vv(0.65,0.0); EL;
  set_outline_color(col);
  BL; vv(-1.0,0.0); vv(0.65,0.0); EL;
  BC; vv(0.65,0.1); vv(1.0,0.0); vv(0.65,-0.1); EC;
}

static void draw_square(Fl_Color col)
  { rectangle(-1,-1,1,1,col); }

static void draw_circle(Fl_Color col) {
  fl_color(col); BP; fl_circle(0,0,1); EP;
  set_outline_color(col);
  BC; fl_circle(0,0,1); EC;
}

static void draw_line(Fl_Color col)
  { fl_color(col); BL; vv(-1.0,0.0); vv(1.0,0.0); EL; }

static void draw_plus(Fl_Color col)
{
  fl_color(col);
  BP; vv(-0.9,-0.15); vv(-0.9,0.15); vv(0.9,0.15); vv(0.9,-0.15); EP;
  BP; vv(-0.15,-0.9); vv(-0.15,0.9); vv(0.15,0.9); vv(0.15,-0.9); EP;
  set_outline_color(col);
  BC;
  vv(-0.9,-0.15); vv(-0.9,0.15); vv(-0.15,0.15); vv(-0.15,0.9);
  vv(0.15,0.9); vv(0.15,0.15); vv(0.9,0.15); vv(0.9,-0.15);
  vv(0.15,-0.15); vv(0.15,-0.9); vv(-0.15,-0.9); vv(-0.15,-0.15);
  EC;
}

static void draw_uparrow(Fl_Color) {
  fl_color(FL_LIGHT3);
  BL; vv(-.8,.8); vv(-.8,-.8); vv(.8,0); EL;
  fl_color(FL_DARK3);
  BL; vv(-.8,.8); vv(.8, 0); EL;
}

static void draw_downarrow(Fl_Color) {
  fl_color(FL_DARK3);
  BL; vv(-.8,.8); vv(-.8,-.8); vv(.8,0); EL;
  fl_color(FL_LIGHT3);
  BL; vv(-.8,.8); vv(.8, 0); EL;
}

static void draw_menu(Fl_Color col)
{
  rectangle(-0.65, 0.85, 0.65, -0.25, col);
  rectangle(-0.65, -0.6, 0.65, -1.0, col);
}





static void fl_init_symbols(void) {
  static char beenhere = 0;
  if (beenhere) return;
  beenhere = 1;
  symbol_table = new Fl_Hash_Table();


  symbol_table->set("",		new Fl_Symbol(draw_arrow1));
  symbol_table->set("->",	new Fl_Symbol(draw_arrow1));
  symbol_table->set(">",	new Fl_Symbol(draw_arrow2));
  symbol_table->set(">>",	new Fl_Symbol(draw_arrow3));
  symbol_table->set(">|",	new Fl_Symbol(draw_arrowbar));
  symbol_table->set(">[]",new Fl_Symbol(draw_arrowbox));
  symbol_table->set("|>",	new Fl_Symbol(draw_bararrow));
  symbol_table->set("<-",	new Fl_Symbol(draw_arrow01));
  symbol_table->set("<",	new Fl_Symbol(draw_arrow02));
  symbol_table->set("<<",	new Fl_Symbol(draw_arrow03));
  symbol_table->set("|<",	new Fl_Symbol(draw_0arrowbar));
  symbol_table->set("[]<",new Fl_Symbol(draw_0arrowbox));
  symbol_table->set("<|",	new Fl_Symbol(draw_0bararrow));
  symbol_table->set("<->",new Fl_Symbol(draw_doublearrow));
  symbol_table->set("-->",new Fl_Symbol(draw_arrow));
  symbol_table->set("+",	new Fl_Symbol(draw_plus));
  symbol_table->set("->|",new Fl_Symbol(draw_arrow1bar));
  symbol_table->set("arrow",	new Fl_Symbol(draw_arrow));
  //symbol_table->set("returnarrow",	new Fl_Symbol((void (*)(int, int, int, int, Fl_Color))(&fl_return_arrow)) );
  symbol_table->set("square",	new Fl_Symbol(draw_square));
  symbol_table->set("circle",	new Fl_Symbol(draw_circle));
  symbol_table->set("line",		new Fl_Symbol(draw_line));
  symbol_table->set("plus",		new Fl_Symbol(draw_plus));
  symbol_table->set("menu",		new Fl_Symbol(draw_menu));
  symbol_table->set("UpArrow", new Fl_Symbol(draw_uparrow));
  symbol_table->set("DnArrow", new Fl_Symbol(draw_downarrow));
  symbol_table->set("||",		new Fl_Symbol(draw_doublebar));
  symbol_table->set("search",       new Fl_Symbol(draw_search));
  symbol_table->set("FLTK",         new Fl_Symbol(draw_fltk));
  fl_set_symbol("returnarrow", FL_RETURN_ARROW); // for backward compatibility
}


// decodes from "numeric keypad notation" to 45 deg multiplier
static inline uchar derot(uchar r){
  if(!r) return 0;
  if(r>6) // top line
    return 10 - r;
  if (r < 4) return r + 4;
  if (r == 4) return 4;
  return 0;
}

// encodes from 45 deg multiplier to "numeric keypad notation"

static inline uchar rot(uchar r){
  if(!r) return 0;
  if(r < 4) return 10 - r;
  if(r > 4) return r - 4; 
  if (r == 4) return 4;
  return 0;
}




Fl_Symbol & Fl_Symbol::set(void (* draw)(Fl_Color c), uchar r){
  draw_ = (void *)draw;
  dx_ = dy_ = dw_ = dh_ = 0;
  down_ = 0;
  type_ = 1 | (derot(r) << 5);
  return * this;
};

int Fl_Symbol::rotate() const{
  return 45 * ((type_ & 0x70)>>5);
}



void  Fl_Symbol::rotate(int r){
  uchar s;
  if(r>9)
    s = (uchar)(((r+22)/45) % 8) <<5;
  else
    s =  (derot(r)) <<5;  
  type_ &= ~0x70;
  type_ |= s;
}





typedef void (* Fl_Draw_Symbol_0)(int, int, int, int, int);
typedef void (* Fl_Draw_Symbol_1)(int);
typedef void (* Fl_Draw_Symbol_2)(int, int, int);

//extern FL_EXPORT Fl_Symbol * fl_symbol_table;



void Fl_Symbol::draw(int x, int y, int w, int h, Fl_Color color) const{
  
  if(!draw_) return;
  if(type_ & 0x8){ //resizing
    x += dx_;
    y += dy_;
    w -= dw_;
    h -= dh_;
  }
  if(square()){ 
    if(w>h){
      x += (w-h)/2;
      w=h;
    }else{
      y += (h-w)/2;
      h = w;
    }
  }
  double angle = rotangle;
  switch(type_ & 0x07){
   case 0:
    ((Fl_Draw_Symbol_0)draw_)(x, y, w, h, color);
    break;
   case 1:
    fl_push_matrix();
    fl_translate(x+w/2,y+h/2);
    if(type_ & 0x70)
      angle += (double)45 * ((type_ & 0x70) >>5);
    fl_rotate(angle);
    fl_scale(0.5*w, 0.5*h);
    ((Fl_Draw_Symbol_1)draw_)(color);
    fl_pop_matrix();
    break;

  case 2:
    ((Fl_Draw_Symbol_2)draw_)(x+w/2, y+h/2, color);
    break;

  case 3:
    Fl_Image * i = (Fl_Image *)draw_;
    fl_color(color); // setting current color for bitmaps
    i->draw(x+w/2 - i->w()/2, y+h/2 - i->h()/2);
    break;

  }
};






//
// End of "$Id$".
//
