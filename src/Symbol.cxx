//
// "$Id$"
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

#include <fltk/draw.h>
#include <fltk/Symbol.h>
#include <fltk/math.h>
#include <fltk/error.h>
#include <string.h>
#include <ctype.h>
using namespace fltk;

/*! \class fltk::Symbol

    All small reusable graphics drawn by fltk are based on this class.
    This includes bitmapped images, the boxes drawn around widgets,
    symbols drawn into buttons, small symbols drawn between the letters
    in labels, and a number of formatting symbols to change the color
    or fontsize or alignment of labels.

    Symbols are typically statically allocated and exist for the
    life of the program. They may either be identified directly by
    a pointer to them, or by a string name. The strings are stored in
    a simple hash table that should be quite efficient up to a few
    thousand named symbols.

*/

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
  hashtablesize = 113;
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

/*! You can call this to get a list of all Symbols that have
  a non-null name(). The symbols are returned out of the hash
  table and are in random order.
  \code
  for (int i = 0;;) {
    const Symbol* symbol = Symbol::iterate(i);
    if (!symbol) break;
    ...
  }
  \endcode
*/
const Symbol* Symbol::iterate(int& i) {
  while (i < hashtablesize) {
    const Symbol* ret = hashtable[i++];
    if (ret && ret != DELETED) return ret;
  }
  return 0;
}

/** Locate a symbol by the name used to construct it. Returns either
    a pointer to the symbol, or null if it is undefined.
*/
const Symbol* Symbol::find(const char* name) {
  if (!symbols_initialized) return 0; // does this ever happen?
  return hashtable[hashindex(name, strlen(name), false)];
}

/** Locate a symbol by the substring after an '@' sign as used by
    drawtext(). \a name points at the start of the name, \a end
    points to the character after the end (this allows the name to be
    extracted from a longer string without having to copy it).

    drawtext() can pass "arguments" to symbols as extra text
    before and after the actual name. This function strips off likely
    arguments: it removes a leading '#', then it will remove a leading
    +/- sign and integer, and any number of trailing comma-seperated
    integers, hex, or floating point constants. So the symbol
    "+400foo1,2.3,0xbeef" will lookup "foo".

    When the symbol's draw() function is called, text() will have been
    set to point at \a name, so the arguments can be read by the
    symbol method. If draw() is called by something other than
    drawtext() then text() will be a zero-length string.

*/
const Symbol* Symbol::find(const char* name, const char* end) {
  if (!symbols_initialized) return 0; // does this ever happen?

  // Try the name without any changes:
  int pos = hashindex(name, end-name, false);
  if (hashtable[pos]) return hashtable[pos];

  // Now strip text off both ends and try again:

  // for back-compatability a leading # is ignored:
  const char* a = name;
  if (a[0] == '#') a++;

  // Remove any leading integer:
  if (a < end-1 && (a[0]=='+'||a[0]=='-') && isdigit(a[1])) a++;
  while (a < end && isdigit(a[0])) a++;

  // Remove trailing "arguments":
  const char* p = a;
  while (p < end) {
    if (isdigit(*p)) break;
    if (p < end-1 && (*p=='+'||*p=='-'||*p=='.') && isdigit(p[1])) break;
    p++;
  }
  if (a < p && (a > name || p < end)) {
    const char* q = p+1;
    while (q < end) {
      if (isxdigit(*q) || strchr("+-.eExX,",*q)) q++;
      else return 0;
    }
    pos = hashindex(a, p-a, false);
    return hashtable[pos];
  }
  return 0;
}

/** \fn void Symbol::text(const char*)
    Set the value returned by text()
*/

/** \fn const char* Symbol::text()

    Return a pointer to right after the '@' sign to the text used to
    invoke this symbol. This is a zero-length string if not being
    called from drawtext(). This is useful for extracting the
    arguments that are skipped by the find() method.
*/
const char* Symbol::text_ = "";

/** \fn void Symbol::measure(int& w, int& h) const {}

    Returns the size a Symbol will draw.

    The referenced variables w and h should be preset to a size
    you \e want to draw the symbol, many Symbols can scale and will
    return without changing these values. Or they may alter the values to
    preserve aspect ratio. Or they may just return constant sizes.
*/

/** This is the actual virtual function that measure() calls.
    It is named _measure so a subclass can implement it
    without hiding variations of the function that are also
    called measure() with different arguments.

    The default version returns with w and h unchanged.
*/
void Symbol::_measure(int& w, int& h) const {}

/** \fn void Symbol::draw(const Rectangle& r, const Style*, Flags=0) const;

    Draw the symbol in the Rectangley \a r. Depending on the actual
    symbol it may scale to fit this rectangle, or just draw in the
    upper-left corner, in most cases clipping to the rectangle if
    too big.

    The \a Style may be used to color in the symbol. Many symbols
    will use color() for blank areas and textcolor() for labels.

    The \a Flags can be used to control how the symbol is drawn. Useful
    flags are:

    - INACTIVE : Draw the image in inactive version of the colors from
    the \a style.
    - OUTPUT: Instead of color() and textcolor(), use boxcolor() and
    labelcolor() from the style.
    - VALUE: Draw turned on or checked
    - SELECTED: Draw as though selected in a browser or menu.
    - HIGHLIGHT: Draw with highlight colors from \a style.
    - PUSHED: Indicates that the mouse is pushing this button. This also
    inverts VALUE.
    - FOCUSED: Indicates that the current object has keyboard focus.

*/

/** \fn void Symbol::_draw(const Rectangle& r, const Style* style, Flags flags) const;

    Virtual function to draw the symbol. This is named with an
    underscore so that a subclass can define it without hiding
    alternative functions named draw() that some intermediate
    classes define.

    If your symbol is designed to be imbedded with an @-command into
    drawtext() then:
    - text() points after the '@' sign
    - getfont() is the current font
    - getsize() is the current font size
    - getcolor() is the current color
    - r.baseline_y() is where to put the baseline
*/

/** Move the edges of \a r to be the "interior" of the symbol. If
    this symbol is used as a Widget::box() then this method is used
    to determine where to put the widget interior.

    The default implementation returns \a r unchanged.
*/
void Symbol::inset(Rectangle& r) const {}

/** Return true if the symbol will completely fill all the pixels
    in the Rectangle passed to draw(). Widgets use this to test
    whether they need to erase their area before drawing the box.
    The default implementation returns false.
*/
bool Symbol::fills_rectangle() const {return false;}

/** Return true to indicate that the area returned by inset() is
    filled with a solid rectangle of the color() from the Style
    passed to draw(). Many widgets assumme this is true of their box().

    If the INVISIBLE flag is passed to draw(), this symbol is allowed
    to skip drawing the interior. This is used by widgets to indicate
    they are going to fill it anyway, and can save some time.

    The default implementation returns false, but most of the useful
    values for a Widget's box() return true.
*/
bool Symbol::is_frame() const {return false;}

/**************** The routines seen by the user *************************/

// Internal class to define scalable square symbols:
class SymbolSymbol : public Symbol {
  void (*drawit)(Color);
public:
  SymbolSymbol(const char* name, void (*f)(Color)) : Symbol(name), drawit(f) {}
  void _draw(const Rectangle&, const Style*, Flags) const;
};

/*! \ingroup symbols

    Define a (hidden) subclass of Symbol that will draw a square icon.
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

void SymbolSymbol::_draw(const Rectangle& r, const Style*, Flags) const
{
  const char* p = text();
  if (*p == '#') p++; // ignore equalscale indicator from fltk1.1
  // move x,y to center of square:
  int x = r.center_x();
  int y = r.center_y();
  // adjust the scale
  // Scale is kept to an integer to avoid crooked shapes:
  int w = (r.w()+1)/2;
  int h = (r.h()+1)/2;
  if (*p == '-' && isdigit(p[1])) {
    int n = p[1]-'0';
    w = (w*12+6+n/2)/(n+12);
    h = (h*12+6+n/2)/(n+12);
    p += 2;
  } else if (*p == '+' && isdigit(p[1])) {
    int n = p[1]-'0';
    w = (w*(12+n)+6)/12;
    h = (h*(12+n)+6)/12;
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
  translate(x, y);
  scale(w,h);
  rotate(rotangle);
  drawit(getcolor());
  pop_matrix();
}

/******************** THE DEFAULT SYMBOLS ****************************/

/** \defgroup symbols @-commands
    When calling drawtext() (or assigning labels or tooltips or any
    other strings that call drawtext()) you can imbed @-commands
    into the text. These actually look up instances of Symbol
    which then can modify the drawing.

    \image html symbols.gif
*/

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

// Warning this extends outside the box:
static void draw_doublearrow(Color col)
{
  BP; vv(-0.4f,-1.0f); vv(-1.4f,0.0f); vv(-0.4f,1.0f); EF(col);
  BP; vv(0.4f,-1.0f); vv(1.4f,0.0f); vv(0.4f,1.0f); EF(col);
  rectangle(-0.5f,-.4f, 0.5f, .4f, col);
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

static void draw_circle(Color col) {
  setcolor(col);
  addchord(Rectangle(-1,-1,2,2),0,360);
  fillpath();
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
// End of "$Id$".
//
