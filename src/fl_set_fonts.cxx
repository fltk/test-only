//
// "$Id: fl_set_fonts.cxx,v 1.8 1999/08/16 07:31:29 bill Exp $"
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

// Internally fonts are stored with a "system name", which is a string
// that is different for every font and can be used to quickly locate
// the font on the system.  On X this is a string that can be passed
// to XListFonts to cause it to return all sizes of that font.

// A function is provided to turn the "system name" into a "nice" name
// and a set of attribute flags.  This is a straght-forward
// translation of the name to compress it to a readable form.  However
// there is no guarantee that all system names are unique.

// An inverse function is turn a "nice" name and attribute flags into
// a font.  Unfortunatly X fonts are such a mess that I see no way to
// do this other than to list every font known and compare the nice
// names.  So that is what this does.  A direct call to function to
// list every font is also provided here.

// Fltk assummes that all fonts whose names start with '-' are of this
// form, this is what is used as the "system name":

// "-*-family-weight-slant-width1-style-*-registry-encoding"

// If the registry-encoding matches fl_encoding then a * is stored to
// make the names shorter.

// Fltk can also locate non-standard names that don't start with '-'.
// The system name for this will be "prefix*suffix", where the '*' is
// where fltk thinks the point size is, or by the actual font name if
// no point size is found.

// You can outwit this by defining your own Fl_Font_ with any string
// desired as the system name.  Fltk will pass it to XListFonts.

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

// turn a stored (with *'s) X font name into a pretty name:
const char* Fl_Font_::name(int* ap) const {
  const char* p = name_;
  static char *buffer; if (!buffer) buffer = new char[128];
  char *o = buffer;

  if (*p != '-') { // non-standard font, just replace * with spaces:
    if (ap) {
      int type = 0;
      if (strstr(p,"bold")) type = FL_BOLD;
      if (strstr(p,"ital")) type |= FL_ITALIC;
      *ap = type;
    }
    for (;*p; p++) {
      if (*p == '*' || *p == ' ' || *p == '-') {
	do p++; while (*p == '*' || *p == ' ' || *p == '-');
	if (!*p) break;
	*o++ = ' ';
      }
      *o++ = *p;
    }
    *o = 0;
    return buffer;
  }

  // get the family:
  const char *x = fl_font_word(p,2); if (*x) x++; if (*x=='*') x++;
  if (!*x) return p;
  const char *e = fl_font_word(x,1);
  strncpy(o,x,e-x); o += e-x;

  // collect all the attribute words:
  int type = 0;
  for (int n = 3; n <= 6; n++) {
    // get the next word:
    if (*e) e++; x = e; e = fl_font_word(x,1);
    int t = attribute(n,x);
    if (t < 0) {*o++ = ' '; strncpy(o,x,e-x); o += e-x;}
    else type |= t;
  }

  // skip over the '*' for the size and get the registry-encoding:
  x = fl_font_word(e,2);
  if (*x) {x++; *o++ = '('; while (*x) *o++ = *x++; *o++ = ')';}

  *o = 0;
  if (type & FL_BOLD) {strcpy(o, " bold"); o += 5;}
  if (type & FL_ITALIC) {strcpy(o, " italic"); o += 7;}

  if (ap) *ap = type;

  return buffer;
}

////////////////////////////////////////////////////////////////
// Generate a list of every font known by X server:

// return non-zero if the registry-encoding should be used:
static int use_registry(const char *p) {
  return *p && *p!='*' && strcmp(p, fl_encoding);
}

// converts a X font name to a standard starname, returns point size:
static int to_canonical(char *to, const char *from) {
  char* c = fl_find_fontsize((char*)from);
  if (!c) return -1; // no point size found...
  char* endptr;
  int size = strtol(c,&endptr,10);
  if (from[0] == '-') {
    // replace the "foundry" with -*-:
    *to++ = '-'; *to++ = '*';
    for (from++; *from && *from != '-'; from++);
    // skip to the registry-encoding:
    endptr = (char*)fl_font_word(endptr,6);
    if (*endptr && !use_registry(endptr+1)) endptr = "";
  }
  int n = c-from;
  strncpy(to,from,n);
  to[n++] = '*';
  strcpy(to+n,endptr);
  return size;
}

// sort raw (non-'*') X font names into perfect order:

static int ultrasort(const void *aa, const void *bb) {
  const char *a = *(char **)aa;
  const char *b = *(char **)bb;

  // sort all non x-fonts at the end:
  if (*a != '-') {
    if (*b == '-') return 1;
    // 2 non-x fonts are matched by "numeric sort"
    int ret = 0;
    for (;;) {
      if (isdigit(*a) && isdigit(*b)) {
	int na = strtol(a, (char **)&a, 10);
	int nb = strtol(b, (char **)&b, 10);
	if (!ret) ret = na-nb;
      } else if (*a != *b) {
	return (*a-*b);
      } else if (!*a) {
	return ret;
      } else {
	a++; b++;
      }
    }
  } else {
    if (*b != '-') return -1;
  }

  // skip the foundry (assumme equal):
  for (a++; *a && *a++!='-';);
  for (b++; *b && *b++!='-';);

  // compare the family and all the attribute words:
  int atype = 0;
  int btype = 0;
  for (int n = 2; n <= 6; n++) {
    int at = attribute(n,a);
    int bt = attribute(n,b);
    if (at < 0) {
      if (bt >= 0) return 1;
      for (;;) {if (*a!=*b) return *a-*b; b++; if (!*a || *a++=='-') break;}
    } else {
      if (bt < 0) return -1;
      a = fl_font_word(a,1); if (*a) a++;
      b = fl_font_word(b,1); if (*b) b++;
      atype |= at; btype |= bt;
    }
  }

  // remember the pixel size:
  int asize = atoi(a);
  int bsize = atoi(b);

  // compare the registry/encoding:
  a = fl_font_word(a,6); if (*a) a++;
  b = fl_font_word(b,6); if (*b) b++;
  if (use_registry(a)) {
    if (!use_registry(b)) return 1;
    int r = strcmp(a,b); if (r) return r;
  } else {
    if (use_registry(b)) return -1;
  }

  if (atype != btype) return atype-btype;
  if (asize != bsize) return asize-bsize;

  // something wrong, just do a string compare...
  return strcmp(*(char**)aa, *(char**)bb);
}

int fl_list_fonts(Fl_Font*& arrayp, bool everything) {
  static Fl_Font* array = 0;
  static int num_fonts = 0;
  if (array) {arrayp = array; return num_fonts;}

  fl_open_display();
  int xlistsize;
  char **xlist = XListFonts(fl_display, everything ? "*" : "-*",
			    10000, &xlistsize);
  if (!xlist) return 0; // ???
  qsort(xlist, xlistsize, sizeof(*xlist), ultrasort);
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
    if (num_fonts >= array_size) {
      array_size = 2*array_size+128;
      array = (Fl_Font*)realloc(array, array_size*sizeof(Fl_Font));
    }
    array[num_fonts++] = newfont;
  }
  arrayp = array;
  return num_fonts;
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
  static int sizes[128];
  int numsizes = 0;
  for (int i = 0; i < listsize; i++) {
    char *q = xlist[i];
    char *d = fl_find_fontsize(q);
    if (!d) continue;
    int s = strtol(d,0,10);
    if (!numsizes || sizes[numsizes-1] < s) {
      sizes[numsizes++] = s;
    } else {
      // insert-sort the new size into list:
      int n;
      for (n = numsizes-1; n > 0; n--) if (sizes[n-1] < s) break;
      if (sizes[n] != s) {
	for (int m = numsizes; m > n; m--) sizes[m] = sizes[m-1];
	sizes[n] = s;
	numsizes++;
      }
    }
    if (numsizes >= 128) break;
  }
  sizep = sizes;
  return numsizes;
}

#endif

//
// End of "$Id: fl_set_fonts.cxx,v 1.8 1999/08/16 07:31:29 bill Exp $".
//
