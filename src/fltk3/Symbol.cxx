//
// "$Id: Symbol.cxx 9041 2011-09-16 21:30:05Z matt $"
//
// Symbol widget class for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <fltk3/Symbol.h>

#include <fltk3/draw.h>
#include <fltk3/math.h>
#include <string.h>
#include <ctype.h>


using namespace fltk3;


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
Symbol::Symbol(const char* name) : name_(0), inset_(0,0,0,0) {
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

/*!
 The destructor does name(0) and removes it from the hash table.
 
 Unfortunately C++ calls the destructor for static objects on program
 exit. I consider this a mistake, particularily because the
 destructor cannot assumme other objects (like the display
 connection) still exist.  It also slows down program exit. As all
 useable systems can harvest resources used by crashed programs,
 calling these destructors serve no useful purpose.
 */
Symbol::~Symbol() {
  name(0);
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
 
 drawtext() can pass "arguments" to symbols as extra text before
 and after the actual name. If the text does not seem to correspond
 to a symbol name, this function tries to strip off these argments
 and try again. The current rules are to remove a leading '#' and
 '+' or '-' sign, remove a leading and trailing integer, so
 "@+400foo21" will locate the symbol "foo". If that still does not
 work, it tries the first letter as a 1-letter symbol, so
 "@Ccolorname" will work.
 
 When the symbol's draw() function is called, text() is set
 to \a name and text_length() is set to \a end minus \a name,
 so the draw() method can examine these arguments.
 */
const Symbol* Symbol::find(const char* name, const char* end) {
  if (!symbols_initialized) return 0; // does this ever happen?
  
  // Try the name without any changes:
  int pos = hashindex(name, end-name, false);
  if (hashtable[pos]) return hashtable[pos];
  
  // Strip leading argument:
  const char* a = name;
  
  // for back-compatability a leading # is ignored:
  // This was used in fltk1 and forms to indicate non-distored scaling:
  if (a[0] == '#') a++;
  
  // Remove any leading integer:
  if (a < end-1 && (a[0]=='+'||a[0]=='-') && isdigit(a[1])) a++;
  while (a < end && isdigit(a[0])) a++;
  
  // Remove trailing integer:
  while (end>a+1 && isdigit(*(end-1))) --end;
  
  pos = hashindex(a, end-a, false);
  if (hashtable[pos]) return hashtable[pos];
  
  // Try 1-letter name:
  pos = hashindex(a, 1, false);
  if (hashtable[pos]) return hashtable[pos];
  
  return 0;
}

/** \fn void Symbol::text(const char*, unsigned)
 Set the values returned by text() and text_length()
 */

/** \fn const char* Symbol::text()
 Return a pointer to right after the '@' sign to the text used to
 invoke this symbol. This is a zero-length string if not being
 called from drawtext(). This is useful for extracting the
 arguments that are skipped by the find() method.
 \see text_length()
 */
const char* Symbol::text_ = "";

/** \fn const char* Symbol::text_length()
 Return the number of bytes between the '@' sign and the ';' or
 null or space that terminates the symbol when called from drawtext().
 This is useful for parsing the arguments. This returns zero if
 it is not being called from drawtext().
 \see text()
 */
unsigned Symbol::text_length_ = 0;

/** \fn void Symbol::measure(int& w, int& h) const {}
 
 Returns the size a Symbol will draw.
 
 The referenced variables w and h should be preset to a size
 you \e want to draw the symbol, many Symbols can scale and will
 return without changing these values. Or they may alter the values to
 preserve aspect ratio. Or they may just return constant sizes.
 
 The most recent values sent to fltk::setcolor(),
 fltk::setbgcolor(), fltk::setdrawflags(), fltk::setfont(), etc,
 may influence the value that this returns.
 */

/** This is the actual virtual function that measure() calls.
 It is named _measure so a subclass can implement it
 without hiding variations of the function that are also
 called measure() with different arguments.
 
 The default version returns with w and h unchanged.
 */
void Symbol::_measure(int& w, int& h) const {}

/** \fn void Symbol::draw(const Rectangle& r) const;
 
 Draw the symbol in the Rectangley \a r. Depending on the actual
 symbol it may scale to fit this rectangle, or just draw in the
 upper-left corner, in most cases clipping to the rectangle if
 too big.
 
 The most recent values sent to fltk::setcolor(),
 fltk::setbgcolor(), fltk::setdrawflags(), fltk::setfont(), etc,
 may be used by the symbol to change how it draws. The symbol
 should not change any of these values, if it needs to it should
 save and restore the original value (some fake built-in symbols
 will change these to affect the color used to draw the rest of
 a string of text).
 */

/** \fn void Symbol::_draw(const Rectangle& r) const;
 
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

/** Change \a r to be the "interior" of the symbol if it was drawn to
 fill it. This is used when the symbol is a Widget::box() to
 define the interior area where the widget can draw it's contents.
 
 The default version adds the value of the inset() rectangle
 (and thus if set_inset() was not called it does nothing).
 
 The most recent values sent to fltk::setcolor(),
 fltk::setbgcolor(), fltk::setdrawflags(), fltk::setfont(), etc,
 may influence the value that this returns (such as to return
 a different edge for a pushed button).
 */
void Symbol::inset(fltk3::Rectangle& r) const {
  r.move(inset_.x(), inset_.y());
  r.move_r(inset_.w());
  r.move_b(inset_.h());
}

/** \fn void Symbol::set_inset(const Rectangle&)
 Set the inset rectangle. This is normally done by the constructor
 for a subclass. If the inset() method is not overridden, the
 values in this rectangle define the edges.
 */

/** \fn int Symbol::dx() const
 Returns getInset().x(). This is usally the width of the left
 inset for the image, though if the inset() method was overridden
 it may return a different number.
 */
/** \fn int Symbol::dy() const
 Returns getInset().y(). This is usally the height of the top
 inset for the image, though if the inset() method was overridden
 it may return a different number.
 */
/** \fn int Symbol::dw() const
 Returns -getInset().w(). This is usally the width of the left
 and right insets added together, though if the inset() method was overridden
 it may return a different number.
 */
/** \fn int Symbol::dh() const
 Returns -getInset().h(). This is usally the height of the top
 and bottom insets added together, though if the inset() method was overridden
 it may return a different number.
 */

/** Return true if the symbol will completely fill all the pixels
 in the Rectangle passed to draw(). Widgets use this to test
 whether they need to erase their area before drawing the box.
 The default implementation returns false.
 */
bool Symbol::fills_rectangle() const {return false;}

/** Return true to indicate that the area returned by inset() is
 drawn as a solid rectangle filled with fltk::getbgcolor(),
 and that if INVISIBLE is set in drawflags() the interior
 is not filled in at all and instead left with it's previous
 contents.
 
 The default implementation returns false, but most of the useful
 values for a Widget's box() return true, and widgets can take
 advantage of this to reduce drawing overhead. In fact quite
 a few widgets currently assumme this is true, but I hope
 to fix this so that image boxes can be used anywhere.
 */
bool Symbol::is_frame() const {return false;}

/**************** The routines seen by the user *************************/

// Internal class to define scalable square symbols:
class SymbolSymbol : public Symbol {
  void (*drawit)(Color);
public:
  SymbolSymbol(const char* name, void (*f)(Color)) : Symbol(name), drawit(f) {}
  void _draw(const fltk3::Rectangle&) const;
};

/* Adds a symbol to the system. Returns whether correct. */
int fltk3::add_symbol(const char *name, void (*drawit)(Color), int scalable)
{
  new SymbolSymbol(name,drawit);
  return 1;
}

void SymbolSymbol::_draw(const fltk3::Rectangle& r) const
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
  if (w < 2 || h < 2) {rectf(x,y,1,1); return;} // too small
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
    case 6: rotangle =   0; break;
    case 7: rotangle = 135; break;
    case 8: rotangle =  90; break;
    case 9: rotangle =  45; break;
  }
  push_matrix();
  translate(x, y);
  scale(float(w), float(h));
  rotate(float(rotangle));
  if (drawit)
    drawit(color());
  pop_matrix();
}


/*!
 The drawing of the symbol can be split into two parts, a background
 and an overlay. When the symbol is used as a box, the widget draws
 the background (using draw()), then draws any labels or images on
 the widget, then draws the overlay by calling this. If this alpha-composites
 an image then the widget label will look like it is embedded in glass
 or plastic.
 
 Even without compositing, this can be used to draw highlights or
 indicators that should go atop the image.
 
 The default version draws a dotted line around the edge, using
 a system-specific XOR mode, if the FOCUSED flag is on in drawflags().
 */
void Symbol::draw_symbol_overlay(const fltk3::Rectangle& r1) const {
  /* FIXME: port this to FLTK 3
  if (!drawflags(FOCUSED)) return;
  fltk3::Rectangle r(r1);
  inset(r);
  if (r.w() > 12) {r.move_x(1); r.move_r(-1);}
  else if (r.w() <= 3) return;
  if (r.h() > 15) {r.move_y(1); r.move_b(-1);}
  else if (r.h() <= 3) return;
  drawFocusRect(r);
   */
}

/**
 Draw the named symbol in the given rectangle using the given color
 \param[in] label name of symbol
 \param[in] x,y   position of symbol
 \param[in] w,h   size of symbol
 \param[in] col   color of symbox
 \returns 1 on success, 0 on failure
 */
// provided for back compatibility:
int fltk3::draw_symbol(const char *label,int x,int y,int w,int h,fltk3::Color col) {  
  const char *p = label;
  if (*p++ != '@') return 0;
  init_symbols();
  const Symbol *sym = Symbol::find(p, p+strlen(p));
  if (!sym) return 0;
  Color prev_col = color();
  color(col);
  Symbol::text(p, strlen(p));
  sym->draw(Rectangle(x, y, w, h));
  Symbol::text(0, 0);
  color(prev_col);
  return 1;
}


/******************** THE DEFAULT SYMBOLS ****************************/

/* Some help stuff */

#define BP fltk3::begin_polygon()
#define EP fltk3::end_polygon()
#define BCP fltk3::begin_complex_polygon()
#define ECP fltk3::end_complex_polygon()
#define BL fltk3::begin_line()
#define EL fltk3::end_line()
#define BC fltk3::begin_loop()
#define EC fltk3::end_loop()
#define vv(x,y) fltk3::vertex(x,y)

//for the outline color
static void set_outline_color(fltk3::Color c) {
  fltk3::color(fltk3::darker(c));
}

static void rectangle(double x,double y,double x2,double y2,fltk3::Color col) {
  fltk3::color(col);
  BP; vv(x,y); vv(x2,y); vv(x2,y2); vv(x,y2); EP;
  set_outline_color(col);
  BC; vv(x,y); vv(x2,y); vv(x2,y2); vv(x,y2); EC;
}

/* The drawing routines */

static void draw_fltk(fltk3::Color col) 
{
  fltk3::color(col);
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

static void draw_search(fltk3::Color col) 
{
  fltk3::color(col);
  BP; vv(-.4, .13); vv(-1.0, .73); vv(-.73, 1.0); vv(-.13, .4); EP;
  set_outline_color(col);
  fltk3::line_style(fltk3::SOLID, 3, 0);
  BC; fltk3::circle(.2, -.2, .6); EC;
  fltk3::line_style(fltk3::SOLID, 1, 0);
  BC; vv(-.4, .13); vv(-1.0, .73); vv(-.73, 1.0); vv(-.13, .4); EC;
}

static void draw_arrow1(fltk3::Color col)
{
  fltk3::color(col);
  BP; vv(-0.8,-0.4); vv(-0.8,0.4); vv(0.0,0.4); vv(0.0,-0.4); EP;
  BP; vv(0.0,0.8); vv(0.8,0.0); vv(0.0,-0.8); vv(0.0,-0.4); vv(0.0,0.4); EP;
  set_outline_color(col);
  BC; vv(-0.8,-0.4); vv(-0.8,0.4); vv(0.0,0.4); vv(0.0,0.8); vv(0.8,0.0);
  vv(0.0,-0.8); vv(0.0,-0.4); EC;
}

static void draw_arrow1bar(fltk3::Color col)
{
  draw_arrow1(col);
  rectangle(.6,-.8,.9,.8,col);
}

static void draw_arrow2(fltk3::Color col)
{
  fltk3::color(col);
  BP; vv(-0.3,0.8); vv(0.50,0.0); vv(-0.3,-0.8); EP;
  set_outline_color(col);
  BC; vv(-0.3,0.8); vv(0.50,0.0); vv(-0.3,-0.8); EC;
}

static void draw_arrow3(fltk3::Color col)
{
  fltk3::color(col);
  BP; vv(0.1,0.8); vv(0.9,0.0); vv(0.1,-0.8); EP;
  BP; vv(-0.7,0.8); vv(0.1,0.0); vv(-0.7,-0.8); EP;
  set_outline_color(col);
  BC; vv(0.1,0.8); vv(0.9,0.0); vv(0.1,-0.8); EC;
  BC; vv(-0.7,0.8); vv(0.1,0.0); vv(-0.7,-0.8); EC;
}

static void draw_arrowbar(fltk3::Color col)
{
  fltk3::color(col);
  BP; vv(0.2,0.8); vv(0.6,0.8); vv(0.6,-0.8); vv(0.2,-0.8); EP;
  BP; vv(-0.6,0.8); vv(0.2,0.0); vv(-0.6,-0.8); EP;
  set_outline_color(col);
  BC; vv(0.2,0.8); vv(0.6,0.8); vv(0.6,-0.8); vv(0.2,-0.8); EC;
  BC; vv(-0.6,0.8); vv(0.2,0.0); vv(-0.6,-0.8); EC;
}

static void draw_arrowbox(fltk3::Color col)
{
  fltk3::color(col);
  BP; vv(-0.6,0.8); vv(0.2,0.0); vv(-0.6,-0.8); EP;
  BC; vv(0.2,0.8); vv(0.6,0.8); vv(0.6,-0.8); vv(0.2,-0.8); EC;
  set_outline_color(col);
  BC; vv(0.2,0.8); vv(0.6,0.8); vv(0.6,-0.8); vv(0.2,-0.8); EC;
  BC; vv(-0.6,0.8); vv(0.2,0.0); vv(-0.6,-0.8); EC;
}

static void draw_bararrow(fltk3::Color col)
{
  fltk3::color(col);
  BP; vv(0.1,0.8); vv(0.9,0.0); vv(0.1,-0.8); EP;
  BP; vv(-0.5,0.8); vv(-0.1,0.8); vv(-0.1,-0.8); vv(-0.5,-0.8); EP;
  set_outline_color(col);
  BC; vv(0.1,0.8); vv(0.9,0.0); vv(0.1,-0.8); EC;
  BC; vv(-0.5,0.8); vv(-0.1,0.8); vv(-0.1,-0.8); vv(-0.5,-0.8); EC;
}

static void draw_doublebar(fltk3::Color col) { 
  rectangle(-0.6,-0.8,-.1,.8,col);
  rectangle(.1,-0.8,.6,.8,col); 
}

static void draw_arrow01(fltk3::Color col)
{ fltk3::rotate(180); draw_arrow1(col); }

static void draw_arrow02(fltk3::Color col)
{ fltk3::rotate(180); draw_arrow2(col); }

static void draw_arrow03(fltk3::Color col)
{ fltk3::rotate(180); draw_arrow3(col); }

static void draw_0arrowbar(fltk3::Color col)
{ fltk3::rotate(180); draw_arrowbar(col); }

static void draw_0arrowbox(fltk3::Color col)
{ fltk3::rotate(180); draw_arrowbox(col); }

static void draw_0bararrow(fltk3::Color col)
{ fltk3::rotate(180); draw_bararrow(col); }

static void draw_doublearrow(fltk3::Color col)
{
  fltk3::color(col);
  BP; vv(-0.35,-0.4); vv(-0.35,0.4); vv(0.35,0.4); vv(0.35,-0.4); EP;
  BP; vv(0.15,0.8); vv(0.95,0.0); vv(0.15,-0.8); EP;
  BP; vv(-0.15,0.8); vv(-0.95,0.0); vv(-0.15,-0.8); EP;
  set_outline_color(col);
  BC; vv(-0.15,0.4); vv(0.15,0.4); vv(0.15,0.8); vv(0.95,0.0);
  vv(0.15,-0.8); vv(0.15,-0.4); vv(-0.15,-0.4); vv(-0.15,-0.8);
  vv(-0.95,0.0); vv(-0.15,0.8); EC;
}

static void draw_arrow(fltk3::Color col)
{
  fltk3::color(col);
  BP; vv(0.65,0.1); vv(1.0,0.0); vv(0.65,-0.1); EP;
  BL; vv(-1.0,0.0); vv(0.65,0.0); EL;
  set_outline_color(col);
  BL; vv(-1.0,0.0); vv(0.65,0.0); EL;
  BC; vv(0.65,0.1); vv(1.0,0.0); vv(0.65,-0.1); EC;
}

static void draw_square(fltk3::Color col)
{ rectangle(-1,-1,1,1,col); }

static void draw_circle(fltk3::Color col) {
  fltk3::color(col); BP; fltk3::circle(0,0,1); EP;
  set_outline_color(col);
  BC; fltk3::circle(0,0,1); EC;
}

static void draw_line(fltk3::Color col)
{ fltk3::color(col); BL; vv(-1.0,0.0); vv(1.0,0.0); EL; }

static void draw_plus(fltk3::Color col)
{
  fltk3::color(col);
  BP; vv(-0.9,-0.15); vv(-0.9,0.15); vv(0.9,0.15); vv(0.9,-0.15); EP;
  BP; vv(-0.15,-0.9); vv(-0.15,0.9); vv(0.15,0.9); vv(0.15,-0.9); EP;
  set_outline_color(col);
  BC;
  vv(-0.9,-0.15); vv(-0.9,0.15); vv(-0.15,0.15); vv(-0.15,0.9);
  vv(0.15,0.9); vv(0.15,0.15); vv(0.9,0.15); vv(0.9,-0.15);
  vv(0.15,-0.15); vv(0.15,-0.9); vv(-0.15,-0.9); vv(-0.15,-0.15);
  EC;
}

static void draw_uparrow(fltk3::Color) {
  fltk3::color(fltk3::LIGHT3);
  BL; vv(-.8,.8); vv(-.8,-.8); vv(.8,0); EL;
  fltk3::color(fltk3::DARK3);
  BL; vv(-.8,.8); vv(.8, 0); EL;
}

static void draw_downarrow(fltk3::Color) {
  fltk3::color(fltk3::DARK3);
  BL; vv(-.8,.8); vv(-.8,-.8); vv(.8,0); EL;
  fltk3::color(fltk3::LIGHT3);
  BL; vv(-.8,.8); vv(.8, 0); EL;
}

static void draw_menu(fltk3::Color col)
{
  rectangle(-0.65, 0.85, 0.65, -0.25, col);
  rectangle(-0.65, -0.6, 0.65, -1.0, col);
}

// Standard UI icons...
static void draw_filenew(fltk3::Color c) {
  fltk3::color(c);
  BCP;
  vv(-0.7, -1.0);
  vv(0.1, -1.0);
  vv(0.1, -0.4);
  vv(0.7, -0.4);
  vv(0.7, 1.0);
  vv(-0.7, 1.0);
  ECP;
  
  fltk3::color(fltk3::lighter(c));
  BP;
  vv(0.1, -1.0);
  vv(0.1, -0.4);
  vv(0.7, -0.4);
  EP;
  
  fltk3::color(fltk3::darker(c));
  BC;
  vv(-0.7, -1.0);
  vv(0.1, -1.0);
  vv(0.1, -0.4);
  vv(0.7, -0.4);
  vv(0.7, 1.0);
  vv(-0.7, 1.0);
  EC;
  
  BL;
  vv(0.1, -1.0);
  vv(0.7, -0.4);
  EL;
}

static void draw_fileopen(fltk3::Color c) {
  fltk3::color(c);
  BP;
  vv(-1.0, -0.7);
  vv(-0.9, -0.8);
  vv(-0.4, -0.8);
  vv(-0.3, -0.7);
  vv(0.6, -0.7);
  vv(0.6, 0.7);
  vv(-1.0, 0.7);
  EP;
  
  fltk3::color(fltk3::darker(c));
  BC;
  vv(-1.0, -0.7);
  vv(-0.9, -0.8);
  vv(-0.4, -0.8);
  vv(-0.3, -0.7);
  vv(0.6, -0.7);
  vv(0.6, 0.7);
  vv(-1.0, 0.7);
  EC;
  
  fltk3::color(fltk3::lighter(c));
  BP;
  vv(-1.0, 0.7);
  vv(-0.6, -0.3);
  vv(1.0, -0.3);
  vv(0.6, 0.7);
  EP;
  
  fltk3::color(fltk3::darker(c));
  BC;
  vv(-1.0, 0.7);
  vv(-0.6, -0.3);
  vv(1.0, -0.3);
  vv(0.6, 0.7);
  EC;
}

static void draw_filesave(fltk3::Color c) {
  fltk3::color(c);
  BP;
  vv(-0.9, -1.0);
  vv(0.9, -1.0);
  vv(1.0, -0.9);
  vv(1.0, 0.9);
  vv(0.9, 1.0);
  vv(-0.9, 1.0);
  vv(-1.0, 0.9);
  vv(-1.0, -0.9);
  EP;
  
  fltk3::color(fltk3::lighter(c));
  BP;
  vv(-0.7, -1.0);
  vv(0.7, -1.0);
  vv(0.7, -0.4);
  vv(-0.7, -0.4);
  EP;
  
  BP;
  vv(-0.7, 0.0);
  vv(0.7, 0.0);
  vv(0.7, 1.0);
  vv(-0.7, 1.0);
  EP;
  
  fltk3::color(c);
  BP;
  vv(-0.5, -0.9);
  vv(-0.3, -0.9);
  vv(-0.3, -0.5);
  vv(-0.5, -0.5);
  EP;
  
  fltk3::color(fltk3::darker(c));
  BC;
  vv(-0.9, -1.0);
  vv(0.9, -1.0);
  vv(1.0, -0.9);
  vv(1.0, 0.9);
  vv(0.9, 1.0);
  vv(-0.9, 1.0);
  vv(-1.0, 0.9);
  vv(-1.0, -0.9);
  EC;
}

static void draw_filesaveas(fltk3::Color c) {
  draw_filesave(c);
  
  fltk3::color(fltk3::color_average(c, fltk3::WHITE, 0.25f));
  BP;
  vv(0.6, -0.8);
  vv(1.0, -0.4);
  vv(0.0, 0.6);
  vv(-0.4, 0.6);
  vv(-0.4, 0.2);
  EP;
  
  fltk3::color(fltk3::darker(c));
  BC;
  vv(0.6, -0.8);
  vv(1.0, -0.4);
  vv(0.0, 0.6);
  vv(-0.4, 0.6);
  vv(-0.4, 0.2);
  EC;
  
  BP;
  vv(-0.1, 0.6);
  vv(-0.4, 0.6);
  vv(-0.4, 0.3);
  EP;
}

static void draw_fileprint(fltk3::Color c) {
  fltk3::color(c);
  BP;
  vv(-0.8, 0.0);
  vv(0.8, 0.0);
  vv(1.0, 0.2);
  vv(1.0, 1.0);
  vv(-1.0, 1.0);
  vv(-1.0, 0.2);
  EP;
  
  fltk3::color(fltk3::color_average(c, fltk3::WHITE, 0.25f));
  BP;
  vv(-0.6, 0.0);
  vv(-0.6, -1.0);
  vv(0.6, -1.0);
  vv(0.6, 0.0);
  EP;
  
  fltk3::color(fltk3::lighter(c));
  BP;
  vv(-0.6, 0.6);
  vv(0.6, 0.6);
  vv(0.6, 1.0);
  vv(-0.6, 1.0);
  EP;
  
  fltk3::color(fltk3::darker(c));
  BC;
  vv(-0.8, 0.0);
  vv(-0.6, 0.0);
  vv(-0.6, -1.0);
  vv(0.6, -1.0);
  vv(0.6, 0.0);
  vv(0.8, 0.0);
  vv(1.0, 0.2);
  vv(1.0, 1.0);
  vv(-1.0, 1.0);
  vv(-1.0, 0.2);
  EC;
  
  BC;
  vv(-0.6, 0.6);
  vv(0.6, 0.6);
  vv(0.6, 1.0);
  vv(-0.6, 1.0);
  EC;
}

static void draw_round_arrow(fltk3::Color c, float da=5.0) {
  double a, r, dr1=0.005, dr2=0.015;
  int i, j;
  for (j=0; j<2; j++) {
    if (j&1) {
      fltk3::color(c);
      set_outline_color(c);
      BC;
    } else {
      fltk3::color(c);
      BCP;
    }
    vv(-0.1, 0.0);
    vv(-1.0, 0.0);
    vv(-1.0, 0.9);
    for (i=27, a=140.0, r=1.0; i>0; i--, a-=da, r-=dr1) {
      double ar = a/180.0 * M_PI;
      vv(cos(ar)*r, sin(ar)*r);
    }
    for (i=27; i>=0; a+=da, i--, r-=dr2) {
      double ar = a/180.0 * M_PI;
      vv(cos(ar)*r, sin(ar)*r);
    }
    if (j&1) {
      EC;
    } else {
      ECP;
    }
  }
}

static void draw_refresh(fltk3::Color c) {
  draw_round_arrow(c);
  fltk3::rotate(180.0);
  draw_round_arrow(c);
  fltk3::rotate(-180.0);
}

static void draw_reload(fltk3::Color c) {
  fltk3::rotate(-135.0);
  draw_round_arrow(c, 10);
  fltk3::rotate(135.0);
}

static void draw_undo(fltk3::Color c) {
  fltk3::translate(0.0, 0.2);
  fltk3::scale(1.0, -1.0);
  draw_round_arrow(c, 6);
  fltk3::scale(1.0, -1.0);
  fltk3::translate(0.0, -0.2);
}

static void draw_redo(fltk3::Color c) {
  fltk3::scale(-1.0, 1.0);
  draw_undo(c);
  fltk3::scale(-1.0, 1.0);
}

static void draw_return_arrow(fltk3::Color c) {
  double scl = 1.0;
  double x = -scl, y = -scl, w = 2*scl, h = 2*scl;
  double size = w; if (h<size) size = h;
  double d = size*0.3; //if (d<3) d = 3;
  double t = size*0.1; //if (t<1) t = 1;
  double x0 = x+(w-2*d-2*t)/2;
  double x1 = x0+d;
  double y0 = y+h/2;

  fltk3::color(fltk3::LIGHT3);
  
  fltk3::begin_line();
  fltk3::vertex(x0, y0);
  fltk3::vertex(x1, y0+d);
  fltk3::vertex(x1, y0+t);
  fltk3::vertex(x1+d+2*t, y0+t);
  fltk3::vertex(x1+d+2*t, y0-d);
  fltk3::end_line();
  
  fltk3::begin_line();
  fltk3::vertex(x1, y0-t);
  fltk3::vertex(x1, y0-d);
  fltk3::end_line();
  
  fltk3::color(fltk3::gray_ramp(0));
  
  fltk3::begin_line();
  fltk3::vertex(x0, y0);
  fltk3::vertex(x1, y0-d);
  fltk3::end_line();
  
  fltk3::color(fltk3::DARK3);
  
  fltk3::begin_line();
  fltk3::vertex(x1, y0-t);
  fltk3::vertex(x1+d, y0-t);
  fltk3::vertex(x1+d, y0-d);
  fltk3::vertex(x1+d+2*t, y0-d);
  fltk3::end_line();
}

static void init_symbols(void) {
  static char beenhere;
  if (beenhere) return;
  beenhere = 1;
  
  init_hashtable();
  
  fltk3::add_symbol("",		draw_arrow1,		1);
  fltk3::add_symbol("->",		draw_arrow1,		1);
  fltk3::add_symbol(">",		draw_arrow2,		1);
  fltk3::add_symbol(">>",		draw_arrow3,		1);
  fltk3::add_symbol(">|",		draw_arrowbar,		1);
  fltk3::add_symbol(">[]",		draw_arrowbox,		1);
  fltk3::add_symbol("|>",		draw_bararrow,		1);
  fltk3::add_symbol("<-",		draw_arrow01,		1);
  fltk3::add_symbol("<",		draw_arrow02,		1);
  fltk3::add_symbol("<<",		draw_arrow03,		1);
  fltk3::add_symbol("|<",		draw_0arrowbar,		1);
  fltk3::add_symbol("[]<",		draw_0arrowbox,		1);
  fltk3::add_symbol("<|",		draw_0bararrow,		1);
  fltk3::add_symbol("<->",		draw_doublearrow,	1);
  fltk3::add_symbol("-->",		draw_arrow,		1);
  fltk3::add_symbol("+",		draw_plus,		1);
  fltk3::add_symbol("->|",		draw_arrow1bar,		1);
  fltk3::add_symbol("arrow",	draw_arrow,		1);
  fltk3::add_symbol("returnarrow",	draw_return_arrow,			1);
  fltk3::add_symbol("square",	draw_square,		1);
  fltk3::add_symbol("circle",	draw_circle,		1);
  fltk3::add_symbol("line",		draw_line,		1);
  fltk3::add_symbol("plus",		draw_plus,		1);
  fltk3::add_symbol("menu",		draw_menu,		1);
  fltk3::add_symbol("UpArrow",	draw_uparrow,		1);
  fltk3::add_symbol("DnArrow",	draw_downarrow,		1);
  fltk3::add_symbol("||",		draw_doublebar,		1);
  fltk3::add_symbol("search",       draw_search,            1);
  fltk3::add_symbol("FLTK",         draw_fltk,              1);
  
  fltk3::add_symbol("filenew",      draw_filenew,           1);
  fltk3::add_symbol("fileopen",     draw_fileopen,          1);
  fltk3::add_symbol("filesave",     draw_filesave,          1);
  fltk3::add_symbol("filesaveas",   draw_filesaveas,        1);
  fltk3::add_symbol("fileprint",    draw_fileprint,         1);
  
  fltk3::add_symbol("refresh",      draw_refresh,           1);
  fltk3::add_symbol("reload",       draw_reload,            1);
  fltk3::add_symbol("undo",         draw_undo,              1);
  fltk3::add_symbol("redo",         draw_redo,              1);
  
  //  fltk3::add_symbol("file",      draw_file,           1);
}


//
// End of "$Id: Symbol.cxx 9041 2011-09-16 21:30:05Z matt $".
//
