//
// "$Id: fl_set_fonts.cxx,v 1.12 1999/11/10 19:27:34 carl Exp $"
//
// More font utilities for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifdef WIN32
#include "fl_set_fonts_win32.cxx"
#else

// This horrible mess is to to provide fltk with a friendly font-naming
// interface, despite the incredible stupidity in the X font mechanism.

// There are three font names of interest:

// The "X font name" is a long string without any '*' characters in it.
// This is the name accepted by X.  For "standard" font names this is
// something like:

// -foundry-family-weight-slant-width1-style-8-180-75-75-p-104-iso8859-1

// Internally fonts are stored with a "system name", which is a string
// that is different for every font and can be used to quickly locate
// the font on the system.  For X this is a string that when passed to
// XListFonts returns all sizes and encodings of that font, and is of
// the form:

// "-*-family-weight-slant-width1-style-*"

// The fltk interface, and much of the innards of this, uses "nice" names.
// This is a straght-forward translation of the name to compress it to
// a readable form.  However there is no guarantee that all nice names
// are unique, this makes some fonts unaccessible from fltk.

// This file has converters between all three of these forms.

// Fltk can also handle fonts with names that don't start with '-'.  There
// have a system name of prefix*suffix, where the * is any point size
// identified by fltk.

#include "Fl_FontSize.H"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// turn word N of a X font name into either some attribute bits
// (right now 0, FL_BOLD, or FL_ITALIC), or into -1 indicating that
// the word should be put into the name:

static int attribute(int n, const char *p) {
  // don't put blank things into name:
  if (!*p || *p=='-' || *p=='*') return 0;
  if (n == 3) { // weight
    if (!strncmp(p,"normal",6) ||
	!strncmp(p,"light",5) ||
	!strncmp(p,"medium",6) ||
	!strncmp(p,"book",4)) return 0;
    if (!strncmp(p,"bold",4) || !strncmp(p,"demi",4)) return FL_BOLD;
  } else if (n == 4) { // slant
    if (*p == 'r') return 0;
    if (*p == 'i' || *p == 'o') return FL_ITALIC;
  } else if (n == 5) { // sWidth
    if (!strncmp(p,"normal",6)) return 0;
  }
  return -1;
}

// turn the system name into a nice name, write it to output buffer,
// and return the accumulated attribute:

static int to_nice(char* o, const char* p) {

  int type = 0;

  if (*p == '-') {
    // get the family:
    const char *x = fl_font_word(p,2); if (*x) x++; if (*x=='*') x++;
    const char *e = fl_font_word(x,1);
    strncpy(o,x,e-x); o += e-x;

    // collect all the attribute words:
    for (int n = 3; n <= 6; n++) {
      // get the next word:
      if (*e) e++; x = e; e = fl_font_word(x,1);
      int t = attribute(n,x);
      if (t < 0) {*o++ = ' '; strncpy(o,x,e-x); o += e-x;}
      else type |= t;
    }

  } else {
    // non standard font name, I look for the words bold & italic and
    // strip them out.  Also strip out all *, -, and spaces and replace
    // them with spaces.

    int punctuation = 0;
    for (;*p; p++) {
      if (!strncmp(p,"bold",4)) {type |= FL_BOLD; p += 3; continue;}
      if (!strncmp(p,"italic",6)) {type |= FL_ITALIC; p += 5; continue;}
      if (*p == '*' || *p == ' ' || *p == '-') punctuation = 1;
      else {
	if (punctuation) {*o++ = ' '; punctuation = 0;}
	*o++ = *p;
      }
    }
  }

  *o = 0;
  return type;
}

// public function that calls the converter.  If ap is null it will also
// add bold/italic words to the end of the nice name.
const char* Fl_Font_::name(int* ap) const {
  static char *buffer; if (!buffer) buffer = new char[128];
  int type = to_nice(buffer, name_);
  if (ap) *ap = type;
  else {
    if (type & FL_BOLD) strcat(buffer," bold");
    if (type & FL_ITALIC) strcat(buffer, " italic");
  }
  return buffer;
}

////////////////////////////////////////////////////////////////
// Generate a list of every font known by X server:

// converts an X font name to a system name, returns point size:
// returns -1 if no change was made to font name
static int to_canonical(char *to, const char *from) {
  char* c = fl_find_fontsize((char*)from);
  if (!c) return -1; // no point size found...
  // don't return the 10x14 style fonts as sizes:
  if (c >= from+2 && isdigit(*(c-2))) return -1;
  char* endptr;
  int size = strtol(c,&endptr,10);
  if (from[0] == '-') {
    // replace the "foundry" with -*-:
    *to++ = '-'; *to++ = '*';
    for (from++; *from && *from != '-'; from++);
    endptr = "";
  }
  int n = c-from;
  strncpy(to,from,n);
  to[n++] = '*';
  strcpy(to+n,endptr);
  return size;
}

// sort raw (non-'*') X font names into perfect order:
// This is horrendously slow, but only needs to be done once....

static int numericsort(const char* a, const char *b) {
  for (;;) {
    if (isdigit(*a) && isdigit(*b)) {
      int na = strtol(a, (char **)&a, 10);
      int nb = strtol(b, (char **)&b, 10);
      if (na != nb) return na-nb;
    } else if (*a != *b) {
      return (*a-*b);
    } else if (!*a) {
      return 0;
    } else {
      a++; b++;
    }
  }
}

static int ultrasort(const void *aa, const void *bb) {
  const char *a = *(char **)aa;
  const char *b = *(char **)bb;

  // all standard fonts go first:
  if (*a == '-') {if (*b != '-') return -1;} else if (*b == '-') return 1;

  char acanon_buf[128];
  int asize = to_canonical(acanon_buf, a);
  const char* acanon = acanon_buf;
  if (asize < 0) acanon = a;

  char aname[128];
  int aattrib = to_nice(aname, acanon);

  char bcanon_buf[128];
  int bsize = to_canonical(bcanon_buf, b);
  const char* bcanon = bcanon_buf;
  if (bsize < 0) bcanon = b;

  char bname[128];
  int battrib = to_nice(bname, bcanon);

  int ret = numericsort(aname, bname); if (ret) return ret;

  if (aattrib != battrib) return (aattrib-battrib);

  ret = numericsort(acanon, bcanon); if (ret) return ret;

  return (asize - bsize);
}

int fl_list_fonts(Fl_Font*& arrayp, int everything) {
  static Fl_Font* array = 0;
  static int num_fonts = 0;
  if (array) {arrayp = array; return num_fonts;}

  fl_open_display();
  int xlistsize;
  char **xlist = XListFonts(fl_display, everything ? "*" : "-*",
			    10000, &xlistsize);
  if (!xlist) return 0; // ???
  qsort(xlist, xlistsize, sizeof(*xlist), ultrasort);

  Fl_Font_* family = 0; // current family
  char family_name[128]; family_name[0] = 0; // nice name of current family

  int array_size = 0;
  for (int i=0; i<xlistsize;) {
    int first_xlist = i;
    const char *p = xlist[i++];
    char canon[1024];
    int size = to_canonical(canon, p);
    if (size >= 0) {
      for (;;) { // find all matching fonts:
	if (i >= xlistsize) break;
	const char *q = xlist[i];
	char this_canon[1024];
	if (to_canonical(this_canon, q) < 0) break;
	if (strcmp(canon, this_canon)) break;
	i++;
      }
      /*if (*p=='-' || i > first_xlist+1)*/ p = canon;
    }
    Fl_Font_* newfont;
    for (int j = 0; ; j++) {
      if (j >= 16) { // create a new font
	newfont = new Fl_Font_;
	newfont->name_ = (p == canon) ? strdup(p) : p;
	newfont->bold = newfont;
	newfont->italic = newfont;
	newfont->first = 0;
	newfont->xlist = xlist+first_xlist;
	newfont->n = -(i-first_xlist);
	break;
      }
      // see if it is one of our built-in fonts:
      // if so, set the list of x fonts, since we have it anyway
      if (!strcmp(fl_fonts[j].name_, p)) {
	newfont = fl_fonts+j;
	if (!newfont->xlist) {
	  newfont->xlist = xlist+first_xlist;
	  newfont->n = -(i-first_xlist);
	}
	break;
      }
    }
    char newname[128];
    int a = to_nice(newname, newfont->name_);
    if (a && !strcmp(newname, family_name)) {
      switch (a) {
      case FL_BOLD: family->bold = newfont; break;
      case FL_ITALIC: family->italic = newfont; break;
      case FL_BOLD|FL_ITALIC:
	family->bold->italic = family->italic->bold = newfont;
	break;
      }
    } else {
      family = newfont;
      strcpy(family_name, newname);
      if (num_fonts >= array_size) {
	array_size = 2*array_size+128;
	array = (Fl_Font*)realloc(array, array_size*sizeof(Fl_Font));
      }
      array[num_fonts++] = newfont;
    }
  }
  arrayp = array;
  return num_fonts;
}

////////////////////////////////////////////////////////////////
// Return all the encodings for this font:

int Fl_Font_::encodings(const char**& arrayp) const {
  if (name_[0] != '-') return 0; // non-standard fonts have no encodings
  if (!xlist) {
    fl_open_display();
    Fl_Font_* t = (Fl_Font_*)this; // cast away const
    t->xlist = XListFonts(fl_display, name_, 100, &(t->n));
    if (!t->xlist) return 0;
  }
  int listsize = n;
  if (listsize < 0) listsize = -listsize;
  static const char* array[128];
  int count = 0;
  for (int i = 0; i < listsize; i++) {
    char *q = xlist[i];
    const char *c = fl_font_word(q,13);
    if (!*c++ || !*c) continue;
    // insert-sort the new encoding into list:
    int n;
    int m;
    for (n = count; n > 0; n--) {
      int cmp = numericsort(array[n-1], c);
      if (cmp < 0) break;
      if (cmp == 0) goto CONTINUE;
    }
    for (m = count; m > n; m--) array[m] = array[m-1];
    array[n] = c;
    count++;
    if (count >= 128) break;
  CONTINUE:;
  }
  arrayp = array;
  return count;
}

////////////////////////////////////////////////////////////////

// Return all the point sizes supported by this font:
int Fl_Font_::sizes(int*& sizep) const {
  if (!xlist) {
    fl_open_display();
    Fl_Font_* t = (Fl_Font_*)this; // cast away const
    t->xlist = XListFonts(fl_display, name_, 100, &(t->n));
    if (!t->xlist) return 0;
  }
  int listsize = n;
  if (listsize < 0) listsize = -listsize;
  static int array[128];
  int count = 0;
  for (int i = 0; i < listsize; i++) {
    char *q = xlist[i];
    char *d = fl_find_fontsize(q);
    if (!d) continue;
    int s = strtol(d,0,10);
    // insert-sort the new size into list:
    int m;
    int n;
    for (n = count; n > 0; n--) {
      int cmp = array[n-1]-s;
      if (cmp < 0) break;
      if (cmp == 0) goto CONTINUE;
    }
    for (m = count; m > n; m--) array[m] = array[m-1];
    array[n] = s;
    count++;
    if (count >= 128) break;
  CONTINUE:;
  }
  sizep = array;
  return count;
}

#endif

//
// End of "$Id: fl_set_fonts.cxx,v 1.12 1999/11/10 19:27:34 carl Exp $".
//
