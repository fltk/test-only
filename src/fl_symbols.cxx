//
// "$Id: fl_symbols.cxx,v 1.31 2003/07/01 07:03:15 spitzak Exp $"
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

#include <fltk/draw.h>
#include <fltk/Symbol.h>
#include <fltk/math.h>
#include <string.h>
#include <ctype.h>
using namespace fltk;

#define MAXSYMBOL       211
   /* Maximal number of symbols in table. Only half of them are
      used. Should be prime. */
static Symbol* symbols[MAXSYMBOL];      /* The symbols */
static int symbnumb = 0;              /* Their number */

// returns hash entry if it exists, or first empty slot:
static int hashindex(const char *name, int n) {
  int pos = n ? (
    n > 1 ? (
      n > 2 ? 71*name[0]+31*name[1]+name[2] : 31*name[0]+name[1]
    ) :
      name[0]
  ) : 0;
  pos %= MAXSYMBOL;
  int hh2 = n ? (
    (n>1) ? 51*name[0]+3*name[1] : 3*name[0]
    )+n : 1;
  hh2 %= MAXSYMBOL; if (!hh2) hh2 = 1;
  for (;;) {
    if (!symbols[pos]) return pos;
    if (!strncmp(symbols[pos]->name(), name, n) && !symbols[pos]->name()[n]) return pos;
    pos = (pos + hh2) % MAXSYMBOL;
  }
}

static void init_symbols(void);

/** The constructor for a Symbol adds it's name to a hash table that
    is looked up in when "@" signs are found in a label. See find()
    for details.

    Pass null for the name if you are making some internal symbol that
    cannot be called from a label.
*/
Symbol::Symbol(const char* name) : name_(name) {
  if (!name) return; // ignore internal ones
  if (symbnumb > MAXSYMBOL / 2) return;	// table is full
  int pos = hashindex(name, strlen(name));
  symbols[pos] = this;
  symbnumb++;
}

/** Locate a symbol by the name used to construct it. \a name points
    at the start of the name, \a end points to the character after
    the end (this allows the name to be extracted from a longer string
    without having to copy it).

    To allow symbols to read "arguments" from the text, only the last
    block of non-digits is used to look up the symbol. The symbol
    itself can read all the leading and trailing digits and characters
    to control it's own behavior. For instance "a23bz98+90sym900" will
    look up "sym".

    Trailing uppercase hex digits and the letter 'x' are also skipped
    to allow arguments to be typed in hex.
*/
const Symbol* Symbol::find(const char* name, const char* end) {
  init_symbols();
  // strip off trailing digits:
  const char* e = end;
  for (;; e--) {
    if (e <= name) break;
    char c = *(e-1);
    if (isxdigit(c) || c=='x' || c=='X'	|| c == '+' || c == '-' || c=='.')
      continue;
    else
      break;
  }
  // skip past any leading hex digits:
  while (e < end && isalpha(*e)) e++;
  // if there is just numbers, use entire number as symbol:
  if (e <= name) e = end;
  // go to start of non-digits:
  const char* p = e-1;
  for (;; p--) {
    if (p <= name) break;
    if (isdigit(*(p-1))) break;
  }
  // now look it up:
  int pos = hashindex(p, e-p);
  return symbols[pos];
}

/**************** The routines seen by the user *************************/

// Internal class to define scalable square symbols:
class SymbolSymbol : public Symbol {
  void (*drawit)(Color);
public:
  SymbolSymbol(const char* name, void (*f)(Color)) : Symbol(name), drawit(f) {}
  void draw(const char* start, const char* end, float x, float y) const;
  void measure(const char* start, const char* end, float& w, float& h) const {
    w = h = int(getsize()+.5);
  }
};

/** Define a (hidden) subclass of Symbol that will draw a square icon.
    All the predefined displaying symbols in fltk use this.

    This is called by "@name;" in a label. By default the square is
    the current font size. The name may be prefixed with some extra
    characters:

    "+digit" will enlarge the square by 1/12 of a point
    size, "-digit" will shrink it. For instance "@+2square;" will
    make a larger than default square.

    Any digits after that are taken as an angle to rotate. A single
    digit is a "keypad rotation" where the x axis points
    at the given number on a keypad, for instance '9' it 45 degrees up,
    and '4' is upside-down. '5' and '6' cause no rotation. For instance
    "@4>" will make a triangle pointing left. "@+14>" is a bigger triangle
    pointing left.

    Multiple digits are a rotation in degrees. To do a rotation less than
    10 degrees use a leading zero. For instance "@25->" is an arrow pointing
    25 degrees up. "@-125->" is a slightly smaller arrow pointing 25
    degrees up.

    The \a drawit function should draw the required image inside the
    -1,-1 to 1,1 square. The current transformation will already be
    set to place this in the right place. The current color is passed
    as an argument, you do not have to use this, but if you change the
    color you are expected to set it back.

    The \a scalable argument is for back-compatability and is ignored.
    Pass 1 for compatability. If you want to make a non-scalable symbol
    you should define your own subclass of Symbol.
*/
void fltk::add_symbol(const char *name, void (*drawit)(Color), int scalable)
/* Adds a symbol to the system. Returns whether correct. */
{
  new SymbolSymbol(name,drawit);
}

void SymbolSymbol::draw(const char* p, const char* end, float x, float y) const
{
  if (*p == '#') p++; // ignore equalscale indicator from fltk1.1
  float w,h; w = h = getsize();
  // move x,y to center of square:
  x += w/2;
  y -= (getascent()-getdescent())/2;
  // adjust the scale
  if (*p == '-' && isdigit(p[1])) {
    int n = p[1]-'0';
    w *= 12.0f/(12+n);
    h *= 12.0f/(12+n);
    p += 2;
  } else if (*p == '+' && isdigit(p[1])) {
    int n = p[1]-'0';
    w *= (12+n)/12.0f;
    h *= (12+n)/12.0f;
    p += 2;
  }
  if (w < 2 || h < 2) {drawpoint(x,y); return;} // too small
  // read rotation in degrees. One digit does "numeric pad" orientation,
  // for back compatability, start degrees with a '0' to avoid this.
  bool firstzero = *p=='0';
  int rotangle = 0;
  while (isdigit(*p)) rotangle = 10*rotangle+(*p++ - '0');
  if (rotangle < 10 && !firstzero) switch(rotangle) {
  case 1: rotangle = 225; break;
  case 2: rotangle = 270; break;
  case 3: rotangle = 315; break;
  case 4: rotangle = 180; break;
  case 5:
  case 6: rotangle = 0; break;
  case 7: rotangle = 135; break;
  case 8: rotangle =  90; break;
  case 9: rotangle =  45; break;
  }
  push_matrix();
  // use integer translate & scale to avoid crooked shapes:
  translate(int(floorf(x)), int(floorf(y)));
  scale(int(w+.5)/2, int(h+.5)/2); // note that w,h are integers!
  rotate(rotangle);
  drawit(getcolor());
  pop_matrix();
}

/******************** THE DEFAULT SYMBOLS ****************************/

/* Some help stuff */

#define BP
#define EP fillpath()
#define BL
#define EL strokepath()
#define BC
#define EC closepath(); strokepath()
#define EF(c) fillstrokepath(c)
#define vv(x,y) addvertex(x,y)

static void rectangle(float x,float y,float x2,float y2,Color col) {
  BP; vv(x,y); vv(x2,y); vv(x2,y2); vv(x,y2); EF(col);
}

/* The drawing routines */

static void draw_arrow1(Color col)
{
  BC; vv(0.0f,1.0f); vv(1.0f,0.0f); vv(0.0f,-1.0f); EF(col);
  rectangle(-1.0,-0.4f,0.1f,0.4f,col);
}

static void draw_arrow2(Color col)
{
  BP; vv(-0.4f,1.0f); vv(0.6f,0.0f); vv(-0.4f,-1.0f); EF(col);
}

static void draw_arrow3(Color col)
{
  BP; vv(0.0f,1.0f); vv(1.0f,0.0f); vv(0.0f,-1.0f); EF(col);
  BP; vv(-1.0f,1.0f); vv(0.0f,0.0f); vv(-1.0f,-1.0f); EF(col);
}

static void draw_arrowbar(Color col)
{
  BP; vv(0.2f,1.0f); vv(0.7f,1.0f); vv(0.7f,-1.0f); vv(0.2f,-1.0f); EF(col);
  BP; vv(-0.8f,1.0f); vv(0.2f,0.0f); vv(-0.8f,-1.0f); EF(col);
}

static void draw_arrowbox(Color col)
{
  BP; vv(-0.8f,1.0f); vv(0.2f,0.0f); vv(-0.8f,-1.0f); EF(col);
  BP; vv(0.2f,1.0f); vv(0.7f,1.0f); vv(0.7f,-1.0f); vv(0.2f,-1.0f); EC;
}

static void draw_bararrow(Color col)
{
  BP; vv(0.0f,1.0f); vv(1.0f,0.0f); vv(0.0f,-1.0f); EF(col);
  BP; vv(-0.8f,1.0f); vv(-0.3f,1.0f); vv(-0.3f,-1.0f); vv(-0.8f,-1.0f); EF(col);
}

static void draw_doublebar(Color col) { 
  rectangle(-0.6f, -1.0f, -0.1f, 1.0f,col);
  rectangle(0.1f, -1.0f, 0.6f, 1.0f,col); 
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

static void draw_box(Color col)
  { BC; vv(-1,-1); vv(1,-1); vv(1,1); vv(-1,1); EC; }

static void draw_uparrow(Color col) {
  setcolor(GRAY99);
  BL; vv(-1.0f,1.0f); vv(-1.0f,-1.0f); vv(1.0f,0.0f); EL;
  setcolor(GRAY33);
  drawline(-1.0f,1.0f,1.0f, 0.0f);
  setcolor(col);
}

static void draw_downarrow(Color col) {
  setcolor(GRAY33);
  BL; vv(-1.0f,1.0f); vv(-1.0f,-1.0f); vv(1.0f,0.0f); EL;
  setcolor(GRAY99);
  drawline(-1.0f,1.0f,1.0f, 0.0f);
  setcolor(col);
}

static void draw_arrow1bar(Color col)
{
  draw_arrow1(col);
  rectangle(.7f,-1.0f,1.0f,1.0f, col);
}

static void draw_doublearrow(Color col)
{
  BP; vv(-0.2f,-.8f); vv(-1.0f,0.0f); vv(-0.2f,.8f); EF(col);
  BP; vv(0.2f,-.8f); vv(1.0f,0.0f); vv(0.2f,.8f); EF(col);
  rectangle(-0.3f,-.4f, 0.3f, .4f, col);
}

static void draw_circle(Color col) {
  BP; addellipse(-1,-1,2,2); EF(col);
}

static void draw_line(Color col)
  { drawline(-1.0f,0.0f,1.0f,0.0f); }

static void draw_plus(Color col)
{
  BP; vv(-1.0f,-0.2f); vv(-1.0f,0.2f); vv(1.0f,0.2f); vv(1.0f,-0.2f); EF(col);
  BP; vv(-0.2f,-1.0f); vv(-0.2f,1.0f); vv(0.2f,1.0f); vv(0.2f,-1.0f); EF(col);
#if 0
  BC;
  vv(-1.0f,-0.2f); vv(-1.0f,0.2f); vv(-0.2f,0.2f); vv(-0.2f,1.0f);
  vv(0.2f,1.0f); vv(0.2f,0.2f); vv(1.0f,0.2f); vv(1.0f,-0.2f);
  vv(0.2f,-0.2f); vv(0.2f,-1.0f); vv(-0.2f,-1.0f); vv(-0.2f,-0.2f);
  EC;
#endif
}

// These last two are probably obsolete:
static void draw_arrow(Color col)
{
  BP; vv(0.6f,0.2f); vv(1.0f,0.0f); vv(0.6f,-0.2f); EF(col);
  drawline(-1.0f,0.0f,0.65f,0.0f);
}

static void draw_menu(Color col)
{
  rectangle(-0.65f, 0.85f, 0.65f, -0.25f, col);
  rectangle(-0.65f, -0.6f, 0.65f, -1.0f, col);
}

static void init_symbols(void) {
  static bool beenhere;
  if (beenhere) return;
  beenhere = true;

  //add_symbol("",		draw_arrow1,		1);
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
  //add_symbol("+",		draw_plus,		1);
  add_symbol("->|",		draw_arrow1bar,		1);
  add_symbol("[]",		draw_box,		1);
  add_symbol("arrow",		draw_arrow,		1);
  //add_symbol("returnarrow",	0,			3);
  add_symbol("square",		draw_square,		1);
  add_symbol("circle",		draw_circle,		1);
  add_symbol("line",		draw_line,		1);
  add_symbol("plus",		draw_plus,		1);
  add_symbol("menu",		draw_menu,		1);
  add_symbol("UpArrow",		draw_uparrow,		1);
  add_symbol("DnArrow",		draw_downarrow,		1);
  add_symbol("||",		draw_doublebar,		1);
}

//
// End of "$Id: fl_symbols.cxx,v 1.31 2003/07/01 07:03:15 spitzak Exp $".
//
