//
// "$Id: fl_list_fonts_win32.cxx,v 1.6 2000/05/27 01:17:32 carl Exp $"
//
// WIN32 font utilities for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/win32.H>
#include "Fl_FontSize.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <config.h>

int Fl_Font_::encodings(const char**& arrayp) const {
  // CET - FIXME - What about this encoding stuff?
  return 0;
}

// turn a stored font name into a pretty name:
const char* Fl_Font_::name(int* ap) const {
  int type;
  switch (name_[0]) {
  case 'B': type = FL_BOLD; break;
  case 'I': type = FL_ITALIC; break;
  case 'P': type = FL_BOLD | FL_ITALIC; break;
  default:  type = 0; break;
  }
  if (ap) {*ap = type; return name_+1;}
  if (!type) {return name_+1;}
  static char *buffer; if (!buffer) buffer = new char[128];
  strcpy(buffer, name_+1);
  if (type & FL_BOLD) strcat(buffer, " bold");
  if (type & FL_ITALIC) strcat(buffer, " italic");
  return buffer;
}

int Fl_Font_::sizes(int*& sizep) const {
  // pretend all fonts are scalable (most are and I don't know how
  // to tell anyways)
  static int array[1];
  sizep = array;
  return 1;
}

// list fonts:

static Fl_Font_* make_a_font(char attrib, const char* name) {
  // see if it is one of our built-in fonts and return it:
  for (int j = 0; j < 16; j++) {
    if (fl_fonts[j].name_[0] == attrib &&
	  !strcasecmp(fl_fonts[j].name_+1, name)) return fl_fonts+j;
  }
  // no, lets create a font:
  Fl_Font_* newfont = new Fl_Font_;
  char *n = new char[strlen(name)+2];
  n[0] = attrib;
  strcpy(n+1, name);
  newfont->name_ = n;
  newfont->bold_ = newfont;
  newfont->italic_ = newfont;
  newfont->first = 0;
  return newfont;
}

static Fl_Font* array = 0;
static int num_fonts = 0;
static int array_size = 0;

static int CALLBACK enumcb(ENUMLOGFONT FAR *lpelf,
  NEWTEXTMETRIC FAR *lpntm, int FontType, LPARAM)
{
  // we need to do something about different encodings of the same font
  // in order to match X!  I can't tell if each different encoding is
  // returned sepeartely or not.  This is what fltk 1.0 did:
  if (lpelf->elfLogFont.lfCharSet != ANSI_CHARSET) return 1;

  const char *name = (const char*)(lpelf->elfFullName);

  Fl_Font_* base = make_a_font(' ', name);
  base->italic_ = make_a_font('I', name);
  if (lpelf->elfLogFont.lfWeight <= 400) {
    base->bold_ = make_a_font('B', name);
    base->italic_->bold_ = base->bold_->italic_ = make_a_font('P', name);
  }    

  if (num_fonts >= array_size) {
    array_size = 2*array_size+128;
    array = (Fl_Font*)realloc(array, array_size*sizeof(Fl_Font));
  }
  array[num_fonts++] = base;

  return 1;
}

// Sort fonts by their "nice" name (it is possible Win32 always returns
// them in this order, but I'm not sure):
static int sort_function(const void *aa, const void *bb) {
  const char* name_a = (*(Fl_Font_**)aa)->name_;
  const char* name_b = (*(Fl_Font_**)bb)->name_;
  int ret = stricmp(name_a+1, name_b+1); if (ret) return ret;
  return name_a[0]-name_b[0]; // sort by attribute
}

int fl_list_fonts(Fl_Font*& arrayp) {
  if (array) {arrayp = array; return num_fonts;}
  HDC dc = GetDC(0);
  EnumFontFamilies(dc, NULL, (FONTENUMPROC)enumcb, 0);
  ReleaseDC(0, dc);
  qsort(array, num_fonts, sizeof(*array), sort_function);
  arrayp = array;

  return num_fonts;
}

// deallocate Win32 fonts
void fl_font_rid() {
  Fl_FontSize* fs;
  int i;
  for (i = 0; i < 16; i++) {
    while ( (fs = fl_fonts[i].first) ) {
      fl_fonts[i].first = fs->next;
      delete fs;
    }
  }
  for (int j = 0; j < num_fonts; j++) {
    while ( (fs = array[j]->first) ) {
      ((Fl_Font_*)(array[j]))->first = fs->next;
      delete fs;
    }
  }
}

//
// End of "$Id: fl_list_fonts_win32.cxx,v 1.6 2000/05/27 01:17:32 carl Exp $"
//
