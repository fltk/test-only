//
// "$Id: fl_symbols.cxx,v 1.8.2.3.2.3.2.10 2005/01/27 21:24:41 rokan Exp $"
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

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Symbol.H>
#include "flstring.h"

typedef void (* Fl_Draw_Symbol)(Fl_Color);
typedef int (* Fl_Draw_Symbol_)(int, int, int, int);


typedef struct {
  const char *name;
  void *  drawit;
  char scalable;
  char notempty;
} SYMBOL;



#define MAXSYMBOL       211
   /* Maximal number of symbols in table. Only half of them are
      used. Should be prime. */

static SYMBOL symbols[MAXSYMBOL];      /* The symbols */
static int symbnumb = -1;              /* Their number */

static int symbol_index[MAXSYMBOL/2];


static int find(const char *name) {
// returns hash entry if it exists, or first empty slot:
  int pos = name[0] ? (
    name[1] ? (
      name[2] ? 71*name[0]+31*name[1]+name[2] : 31*name[0]+name[1]
    ) :
      name[0]
  ) : 0;
  pos %= MAXSYMBOL;
  int hh2 = name[0] ? (
    (name[1]) ? 51*name[0]+3*name[1] : 3*name[0]
    ) : 1;
  hh2 %= MAXSYMBOL; if (!hh2) hh2 = 1;
  for (;;) {
    if (!symbols[pos].notempty) return pos;
    if (!strcmp(symbols[pos].name,name)) return pos;
    pos = (pos + hh2) % MAXSYMBOL;
  }
}

static void fl_init_symbols(void);

/**************** The routines seen by the user *************************/



static int fl_set_symbol(const char *name, void *drawit, int type)
/* Adds a symbol to the system. Returns whether correct. */
{
  fl_init_symbols();
  int pos = find(name);
  if(! symbols[pos].notempty){
    if (symbnumb > MAXSYMBOL / 2) return -1;	// table is full
    symbnumb++;
  }
  symbols[pos].name = name;
  symbols[pos].drawit = drawit;
  symbols[pos].notempty = 1;
  symbols[pos].scalable = type;
  return pos;
}


int fl_add_symbol(const char *name, void (*drawit)(Fl_Color), int scalable){
  return fl_set_symbol(name, (void *)drawit, scalable);
}


int fl_return_arrow(int x,int y,int w,int h);


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
  int rotangle;
  switch (*p++) {
  case '0':
    rotangle = 1000*(p[1]-'0') + 100*(p[2]-'0') + 10*(p[3]-'0');
    p += 4;
    break;
  case '1': rotangle = 2250; break;
  case '2': rotangle = 2700; break;
  case '3': rotangle = 3150; break;
  case '4': rotangle = 1800; break;
  case '5':
  case '6': rotangle = 0; break;
  case '7': rotangle = 1350; break;
  case '8': rotangle =  900; break;
  case '9': rotangle =  450; break;
  default: rotangle = 0; p--; break;
  }
  int pos = find(p);
  if (!symbols[pos].notempty) return 0;
  if (symbols[pos].scalable == 3) { // kludge to detect return arrow
    fl_color(col);
    ((Fl_Draw_Symbol_)symbols[pos].drawit)(x,y,w,h);
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
  return 1;
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
  static char beenhere;
  if (beenhere) return;
  beenhere = 1;
  symbnumb = 0;

  fl_set_symbol("",		(void *)draw_arrow1,		1);
  fl_set_symbol("->",		(void *)draw_arrow1,		1);
  fl_set_symbol(">",		(void *)draw_arrow2,		1);
  fl_set_symbol(">>",		(void *)draw_arrow3,		1);
  fl_set_symbol(">|",		(void *)draw_arrowbar,		1);
  fl_set_symbol(">[]",		(void *)draw_arrowbox,		1);
  fl_set_symbol("|>",		(void *)draw_bararrow,		1);
  fl_set_symbol("<-",		(void *)draw_arrow01,		1);
  fl_set_symbol("<",		(void *)draw_arrow02,		1);
  fl_set_symbol("<<",		(void *)draw_arrow03,		1);
  fl_set_symbol("|<",		(void *)draw_0arrowbar,		1);
  fl_set_symbol("[]<",		(void *)draw_0arrowbox,		1);
  fl_set_symbol("<|",		(void *)draw_0bararrow,		1);
  fl_set_symbol("<->",		(void *)draw_doublearrow,	1);
  fl_set_symbol("-->",		(void *)draw_arrow,		1);
  fl_set_symbol("+",		(void *)draw_plus,		1);
  fl_set_symbol("->|",		(void *)draw_arrow1bar,		1);
  fl_set_symbol("arrow",	(void *)draw_arrow,		1);
  fl_set_symbol("returnarrow",	(void *)fl_return_arrow,			3);
  fl_set_symbol("square",	(void *)draw_square,		1);
  fl_set_symbol("circle",	(void *)draw_circle,		1);
  fl_set_symbol("line",		(void *)draw_line,		1);
  fl_set_symbol("plus",		(void *)draw_plus,		1);
  fl_set_symbol("menu",		(void *)draw_menu,		1);
  fl_set_symbol("UpArrow",	(void *)draw_uparrow,		1);
  fl_set_symbol("DnArrow",	(void *)draw_downarrow,		1);
  fl_set_symbol("||",		(void *)draw_doublebar,		1);
  fl_set_symbol("search",       (void *)draw_search,            1);
  fl_set_symbol("FLTK",         (void *)draw_fltk,              1);
}





Fl_Symbol::Fl_Symbol(void (* draw)(Fl_Color c), char  size, bool square, uchar rot): scale_(size), draw_((void *)draw){
  if(rot >4)
    if(rot >6)
      rot -=2;
    else 
      rot=0;
  type_ = 1 | (rot << 5);
  if(square) type_ |= 0x10;

};

  // draw functions for these constructors draw dirrectly within x,y,w,h area. If square is set
  // the dwawimg is resized so that w==h whichever is smaller.and centered in the area
  // 4 least significant bits of type_ must be unique for particular function. Type nost be even ( |1) it it is scalable
  // and secind bit (|2) must be set if it is tatat-able. 
  // Fifth least significant bit ( | 0x10) is used to indicate that the drawing should be square-sized,
  // three most significant bites of type_ describe rotation angle (if any)


 


Fl_Symbol::Fl_Symbol(void (* draw)(int x, int y, int w, int h, Fl_Color c), char size, bool square ):type_(3), scale_(size), draw_((void *)draw){
  if(square) type_ |= 0x10;
};

  // Following draw functions are not scalled. x, y coordinates describe center of the symbol and when symbol is drawn
  // it is placed in the centre of x,y,w,h drawing area.
Fl_Symbol::Fl_Symbol(void (* draw)(int x, int y, Fl_Color c)):type_(5), scale_(0),  draw_((void *)draw){};

  // Label symbol strings can be used too!. use trailling @ as well, eg Fl_Symbol("@+92->")
Fl_Symbol::Fl_Symbol(const char * s):type_(6), scale_(0), draw_((void *)s){};

  // Even box-types can be used as symbols!
Fl_Symbol::Fl_Symbol(Fl_Boxtype box,  char size, bool square): type_(7),  scale_(size), draw_((void *)((long)box)){
  if(square) type_ |= 0x10;
};



typedef void (* Fl_Draw_Symbol_0)();
typedef void (* Fl_Draw_Symbol_1)(int);
typedef void (* Fl_Draw_Symbol_2)(int, int, int, int);
typedef void (* Fl_Draw_Symbol_3)(int, int, int, int, int);
typedef void (* Fl_Draw_Symbol_4)(int, int);
typedef void (* Fl_Draw_Symbol_5)(int, int, int);





void Fl_Symbol::draw(int x, int y, int w, int h, Fl_Color color){
  if(!draw_) return;
  if(type_ & 0x10)
    if(w>h){
      x += (w-h)/2;
      w=h;
    }else{
      y += (h-w)/2;
      h = w;
    }
  if(scale_){
    w += 2 * scale_;
    h += 2 * scale_;
    x -= scale_;
    y -= scale_;
  };
  double angle = 0;
  if(type_ & 0xA0){ // rotation!
    switch ((type_ & 0xA0) >> 5){
      case 1: angle = 225; break;
      case 2: angle = 270; break;
      case 3: angle = 315; break;
      case 4: angle = 180; break;
      case 5: angle = 135; break;
      case 6: angle = 90; break;
      case 7: angle = 45;
    }
  };

  switch(type_ & 0x0F){
  case 1:
    fl_push_matrix();
    fl_translate(x+w/2,y+h/2);
    fl_scale(0.5*w, 0.5*h);
    fl_rotate(angle);
    ((Fl_Draw_Symbol_1)draw_)(color);
    fl_pop_matrix();
    break;
  case 3:
    ((Fl_Draw_Symbol_3)draw_)(x, y, w, h, color);
    break;
  case 5:
    ((Fl_Draw_Symbol_5)draw_)(x+w/2, y+h/2, color);
    break;
  case 6:
    fl_draw_symbol((const char *)draw_, x, y, w, h, color);
    break;
  case 8:
    fl_draw_box((Fl_Boxtype) ((long)draw_), x, y, w, h, color);
  }
};






//
// End of "$Id: fl_symbols.cxx,v 1.8.2.3.2.3.2.10 2005/01/27 21:24:41 rokan Exp $".
//
