//
// "$Id: fl_symbols.cxx,v 1.34 2003/08/04 03:35:43 spitzak Exp $"
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

// Adapted from original code Written by Mark Overmars
// Version 2.1 a
// Date: Oct  2, 1992

#include <fltk/draw.h>
#include <fltk/Symbol.h>
#include <fltk/math.h>
#include <string.h>
#include <ctype.h>
using namespace fltk;

////////////////////////////////////////////////////////////////
// Hash table using Quadratic Probing and reallocation of table
// Hey feel free to replace this with a better hash table implementation!

static Symbol** hashtable = 0;
static int hashtablesize;
static int num_symbols = 0;
#define DELETED ((Symbol*)1)

// returns hash entry if it exists, or first empty slot:
static unsigned hashindex(const char *name, unsigned n, bool delok) {
  // Calculate the hash index:
  unsigned pos = 0;
  unsigned i; for (i = 0; i < n; i++) pos = 37*pos + name[i];
  pos %= hashtablesize;
  // Quadratic probing through the hash table to find empty slot
  // or a null:
  for (i = 0;;) {
    Symbol* s = hashtable[pos];
    if (!s) break;
    if (s == DELETED) {if (delok) break;}
    else if (!strncmp(s->name(), name, n) && !s->name()[n]) break;
    pos += 2 * ++i - 1;
    pos %= hashtablesize;
  }
  return pos;
}

static void double_hashtable() {
  // Remember the old table:
  Symbol** oldtable = hashtable;
  int oldsize = hashtablesize;
  // figure out a new size that is prime and >= 2 * currentsize.
  int n = 2*hashtablesize+1;
  for (int i = 3; i*i <= n; i += 2) while (n%i == 0) {i = 3; n += 2;}
  //printf("Realloc to new table size of %d\n", n);
  // allocate new table and set it all to zero:
  hashtablesize = n;
  hashtable = new Symbol*[hashtablesize];
  memset(hashtable, 0, hashtablesize*sizeof(Symbol*));
  // copy all the symbols over:
  for (n = 0; n < oldsize; n++) {
    Symbol* s = oldtable[n];
    if (s && s != DELETED) {
      int pos = hashindex(s->name(), strlen(s->name()), true);
      hashtable[pos] = s;
    }
  }
  // throw away old table:
  delete[] oldtable;
}

// This is called by init_symbols, below:
static inline void init_hashtable() {
  // This size is chosen so no realloc is done for the built-in set
  // of symbols. If you add a lot of built-in symbols it is probably
  // a good idea to increase this:
  // Number must be prime!!!
  hashtablesize = 79;
  hashtable = new Symbol*[hashtablesize];
  memset(hashtable, 0, hashtablesize*sizeof(Symbol*));
}

// Creates the hash table and populates it with our built-in set:
static void init_symbols();
static bool symbols_initialized = false;

////////////////////////////////////////////////////////////////

/** The constructor for a Symbol adds it's name to a hash table that
    is looked up in when "@" signs are found in a label. See find()
    for details. Pass null for the name to not put it in hash table.
*/
Symbol::Symbol(const char* name) : name_(0) {
  if (!symbols_initialized) {
    symbols_initialized = true;
    init_symbols();
  }
  this->name(name);
}

/** Sets the name() of the symbol. If it is in the hash table under
    the old name it is removed. If the new name is not NULL then it
    is added under the new name to the hash table.

    The strings are assummed to belong to the calling program. You
    can go through the trouble of freeing the old one, or just leak
    it like Fluid does.

    You \e must call name(0) before destroying a Symbol.
    The Symbol class does not have a destructor because C++ will
    call it for all the static symbols and thus slow down program exit.
*/
void Symbol::name(const char* name) {
  if (name_ == name) return;
  if (name_) {
    int pos = hashindex(name_, strlen(name_), false);
    if (hashtable[pos]==this) hashtable[pos] = DELETED;
  }
  name_ = name;
  if (name) {
    int pos = hashindex(name, strlen(name), true);
    // don't expand hash table if we are replacing things:
    if (hashtable[pos]) {hashtable[pos] = this; return;}
    // put it in the hash table:
    hashtable[pos] = this;
    // if table is more than half-full we need to reallocate it:
    if (++num_symbols > hashtablesize/2) double_hashtable();
  }
}

/** Locate a symbol by the name used to construct it. \a name points
    at the start of the name, \a end points to the character after
    the end (this allows the name to be extracted from a longer string
    without having to copy it).

    To allow symbols to read "arguments" this will also try stripping
    off a leading +/- sign and integer and any trailing text that
    looks like comma-seperated integers, hex, or floating point
    constant. For back-compatability it will also strip a leading '#'.
    So the symbol "+400foo1,2,0xbeef" will lookup "foo".
*/
const Symbol* Symbol::find(const char* name, const char* end) {
  if (!symbols_initialized) return 0; // does this ever happen?

  // for back-compatability a leading # is ignored:
  if (name[0] == '#') name++;
  // Remove any leading integer:
  if (name < end-1 && (name[0]=='+' || name[0]=='-') && isdigit(name[1])) name++;
  while (name < end && isdigit(name[0])) name++;

  // Try the name without any more changes:
  int pos = hashindex(name, end-name, false);
  if (hashtable[pos]) return hashtable[pos];

  // Remove trailing "arguments" and try lookup again:
  const char* p = name;
  while (p < end) {
    if (isdigit(*p)) break;
    if (p < end-1 && (*p=='+'||*p=='-'||*p=='.') && isdigit(p[1])) break;
    p++;
  }
  if (p < end) {
    const char* q = p+1;
    while (q < end) {
      if (isxdigit(*q) || strchr("+-.eExX,",*q)) q++;
      else return 0;
    }
    pos = hashindex(name, p-name, false);
    return hashtable[pos];
  }
  return 0;
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
  init_hashtable();

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
  add_symbol("+",		draw_plus,		1);
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
// End of "$Id: fl_symbols.cxx,v 1.34 2003/08/04 03:35:43 spitzak Exp $".
//
