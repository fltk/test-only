//
// "$Id: fl_symbols.cxx,v 1.29 2002/12/10 02:01:03 easysw Exp $"
//
// Symbol drawing code for the Fast Light Tool Kit (FLTK).
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

// These are small graphics drawn by the normal label-drawing
// code when the string starts with an '@' sign.

// Adapted from original code written by:

// Written by Mark Overmars
// Version 2.1 a
// Date: Oct  2, 1992

#include <fltk/Style.h>
#include <fltk/draw.h>
#include <fltk/events.h>
#include <string.h>
using namespace fltk;

typedef struct {
  const char *name;
  void (*drawit)(Color);
  char scalable;
  char notempty;
} SYMBOL;

#define MAXSYMBOL       211
   /* Maximal number of symbols in table. Only half of them are
      used. Should be prime. */

static SYMBOL symbols[MAXSYMBOL];      /* The symbols */
static int symbnumb = -1;              /* Their number */

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

static void init_symbols(void);

/**************** The routines seen by the user *************************/

int fltk::add_symbol(const char *name, void (*drawit)(Color), int scalable)
/* Adds a symbol to the system. Returns whether correct. */
{
  init_symbols();
  int pos;
  if (symbnumb > MAXSYMBOL / 2) return 0;	// table is full
  pos = find(name);
  symbols[pos].name = name;
  symbols[pos].drawit = drawit;
  symbols[pos].notempty = 1;
  symbols[pos].scalable = scalable;
  symbnumb++;
  return 1;
}

// this function is in ReturnButton.cxx:
void fl_glyph_return(const Widget*, int,
		     int x,int y,int w,int h, Flags);

// provided for back compatability:
int fltk::draw_symbol(const char *label,int x,int y,int w,int h,Color col) {
  const char *p = label;
  if (*p++ != '@') return 0;
  init_symbols();
  bool equalscale = false;
  if (*p == '#') {
    equalscale = true;
    p++;
  } else {
    // if it is nearly square, make it square:
    if (w < h) {if (w*3 > h*2) equalscale = true;}
    else {if (h*3 > w*2) equalscale = true;}
  }
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
  w = (w-1)|1; h = (h-1)|1; // odd sizes so arrow points are centered
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
    fl_glyph_return(0,0, x,y,w,h, 0);
    return 1;
  }
  push_matrix();
  translate(x+w/2,y+h/2);
  if (symbols[pos].scalable) {
    if (equalscale) {if (w<h) h = w; else w = h;}
    scale(0.5f*w, 0.5f*h);
    rotate(rotangle/10.0f);
  }
  (symbols[pos].drawit)(col);
  pop_matrix();
  return 1;
}

/******************** THE DEFAULT SYMBOLS ****************************/

/* Some help stuff */

#define BP
#define EP fillpath()
#define BL
#define EL strokepath()
#define BC
#define EC closepath(); setcolor(BLACK); strokepath()
#define EF(c) fillstrokepath(BLACK)
#define vv(x,y) addvertex(x,y)

static void rectangle(float x,float y,float x2,float y2,Color col) {
  setcolor(col);
  BP; vv(x,y); vv(x2,y); vv(x2,y2); vv(x,y2); EF(col);
}

/* The drawing routines */

static void draw_arrow1(Color col)
{
  setcolor(col);
  BP; vv(-0.8f,-0.4f); vv(-0.8f,0.4f); vv(0.0f,0.4f); vv(0.0f,-0.4f); EP;
  BP; vv(0.0f,0.8f); vv(0.8f,0.0f); vv(0.0f,-0.8f); vv(0.0f,-0.4f); vv(0.0f,0.4f); EP;
  BC; vv(-0.8f,-0.4f); vv(-0.8f,0.4f); vv(0.0f,0.4f); vv(0.0f,0.8f); vv(0.8f,0.0f);
      vv(0.0f,-0.8f); vv(0.0f,-0.4f); EC;
}

static void draw_arrow2(Color col)
{
  setcolor(col);
  BP; vv(-0.3f,0.8f); vv(0.50f,0.0f); vv(-0.3f,-0.8f); EF(col);
}

static void draw_arrow3(Color col)
{
  setcolor(col);
  BP; vv(0.1f,0.8f); vv(0.9f,0.0f); vv(0.1f,-0.8f); EF(col);
  BP; vv(-0.7f,0.8f); vv(0.1f,0.0f); vv(-0.7f,-0.8f); EF(col);
}

static void draw_arrowbar(Color col)
{
  setcolor(col);
  BP; vv(0.2f,0.8f); vv(0.6f,0.8f); vv(0.6f,-0.8f); vv(0.2f,-0.8f); EF(col);
  BP; vv(-0.6f,0.8f); vv(0.2f,0.0f); vv(-0.6f,-0.8f); EF(col);
}

static void draw_arrowbox(Color col)
{
  setcolor(col);
  BP; vv(-0.6f,0.8f); vv(0.2f,0.0f); vv(-0.6f,-0.8f); EF(col);
  BC; vv(0.2f,0.8f); vv(0.6f,0.8f); vv(0.6f,-0.8f); vv(0.2f,-0.8f); EC;
}

static void draw_bararrow(Color col)
{
  setcolor(col);
  BP; vv(0.1f,0.8f); vv(0.9f,0.0f); vv(0.1f,-0.8f); EF(col);
  BP; vv(-0.5f,0.8f); vv(-0.1f,0.8f); vv(-0.1f,-0.8f); vv(-0.5f,-0.8f); EF(col);
}

static void draw_doublebar(Color col) { 
  rectangle(-0.6f,-0.8f,-.1f,.8f,col);
  rectangle(.1f,-0.8f,.6f,.8f,col); 
}

static void draw_arrow01(Color col)
  { rotate(180); draw_arrow1(col); }

static void draw_arrow02(Color col)
  { rotate(180); draw_arrow2(col); }

static void draw_arrow03(Color col)
  { rotate(180); draw_arrow3(col); }

static void draw_0arrowbar(Color col)
  { rotate(180); draw_arrowbar(col); }

static void draw_0arrowbox(Color col)
  { rotate(180); draw_arrowbox(col); }

static void draw_0bararrow(Color col)
  { rotate(180); draw_bararrow(col); }

static void draw_square(Color col)
  { rectangle(-1,-1,1,1,col); }

static void draw_uparrow(Color) {
  setcolor(GRAY99);
  BL; vv(-.8f,.8f); vv(-.8f,-.8f); vv(.8f,0.0f); EL;
  setcolor(GRAY33);
  BL; vv(-.8f,.8f); vv(.8f, 0.0f); EL;
}

static void draw_downarrow(Color) {
  setcolor(GRAY33);
  BL; vv(-.8f,.8f); vv(-.8f,-.8f); vv(.8f,0.0f); EL;
  setcolor(GRAY99);
  BL; vv(-.8f,.8f); vv(.8f, 0.0f); EL;
}

static void draw_arrow1bar(Color col)
{
  draw_arrow1(col);
  rectangle(.6f,-.8f,.9f,.8f, col);
}

static void draw_doublearrow(Color col)
{
  setcolor(col);
  BP; vv(-0.35f,-0.4f); vv(-0.35f,0.4f); vv(0.35f,0.4f); vv(0.35f,-0.4f); EP;
  BP; vv(0.15f,0.8f); vv(0.95f,0.0f); vv(0.15f,-0.8f); EP;
  BP; vv(-0.15f,0.8f); vv(-0.95f,0.0f); vv(-0.15f,-0.8f); EP;

  BC; vv(-0.15f,0.4f); vv(0.15f,0.4f); vv(0.15f,0.8f); vv(0.95f,0.0f);
      vv(0.15f,-0.8f); vv(0.15f,-0.4f); vv(-0.15f,-0.4f); vv(-0.15f,-0.8f);
      vv(-0.95f,0.0f); vv(-0.15f,0.8f); EC;
}

static void draw_arrow(Color col)
{
  setcolor(col);
  BP; vv(0.65f,0.1f); vv(1.0f,0.0f); vv(0.65f,-0.1f); EF(col);
  BL; vv(-1.0f,0.0f); vv(0.65f,0.0f); EL;
}

static void draw_circle(Color col) {
  setcolor(col); BP; addellipse(-1,-1,2,2); EF(col);
}

static void draw_line(Color col)
  { setcolor(col); BL; vv(-1.0f,0.0f); vv(1.0f,0.0f); EL; }

static void draw_plus(Color col)
{
  setcolor(col);
  BP; vv(-0.9f,-0.15f); vv(-0.9f,0.15f); vv(0.9f,0.15f); vv(0.9f,-0.15f); EP;
  BP; vv(-0.15f,-0.9f); vv(-0.15f,0.9f); vv(0.15f,0.9f); vv(0.15f,-0.9f); EP;

  BC;
  vv(-0.9f,-0.15f); vv(-0.9f,0.15f); vv(-0.15f,0.15f); vv(-0.15f,0.9f);
  vv(0.15f,0.9f); vv(0.15f,0.15f); vv(0.9f,0.15f); vv(0.9f,-0.15f);
  vv(0.15f,-0.15f); vv(0.15f,-0.9f); vv(-0.15f,-0.9f); vv(-0.15f,-0.15f);
  EC;
}

static void draw_menu(Color col)
{
  rectangle(-0.65f, 0.85f, 0.65f, -0.25f, col);
  rectangle(-0.65f, -0.6f, 0.65f, -1.0f, col);
}

static void init_symbols(void) {
  static char beenhere;
  if (beenhere) return;
  beenhere = 1;
  symbnumb = 0;

  add_symbol("",		draw_arrow1,		1);
  add_symbol("->",		draw_arrow1,		1);
  add_symbol(">",		draw_arrow2,		1);
  add_symbol(">>",		draw_arrow3,		1);
  add_symbol(">|",		draw_arrowbar,		1);
  add_symbol(">[]",		draw_arrowbox,		1);
  add_symbol("|>",		draw_bararrow,		1);
  add_symbol("<-",		draw_arrow01,		1);
  add_symbol("<",		draw_arrow02,		1);
  add_symbol("<<",		draw_arrow03,		1);
  add_symbol("|<",		draw_0arrowbar,		1);
  add_symbol("[]<",		draw_0arrowbox,		1);
  add_symbol("<|",		draw_0bararrow,		1);
  add_symbol("<->",		draw_doublearrow,	1);
  add_symbol("-->",		draw_arrow,		1);
  add_symbol("+",		draw_plus,		1);
  add_symbol("->|",		draw_arrow1bar,		1);
  add_symbol("arrow",	draw_arrow,		1);
  add_symbol("returnarrow",	0,			3);
  add_symbol("square",	draw_square,		1);
  add_symbol("circle",	draw_circle,		1);
  add_symbol("line",		draw_line,		1);
  add_symbol("plus",		draw_plus,		1);
  add_symbol("menu",		draw_menu,		1);
  add_symbol("UpArrow",	draw_uparrow,		1);
  add_symbol("DnArrow",	draw_downarrow,		1);
  add_symbol("||",		draw_doublebar,		1);
}

//
// End of "$Id: fl_symbols.cxx,v 1.29 2002/12/10 02:01:03 easysw Exp $".
//
