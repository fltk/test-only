//
// "$Id: fl_set_fonts_mac.cxx,v 1.1.2.7.2.2 2003/11/07 03:47:25 easysw Exp $"
//
// MacOS font utilities for the Fast Light Tool Kit (FLTK).
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

// This function fills in the fltk font table with all the fonts that
// are found on the X server.  It tries to place the fonts into families
// and to sort them so the first 4 in a family are normal, bold, italic,
// and bold italic.

// Bug: older versions calculated the value for *ap as a side effect of
// making the name, and then forgot about it. To avoid having to change
// the header files I decided to store this value in the last character
// of the font name array.
#define ENDOFBUFFER 127 // sizeof(Fl_Font.fontname)-1

// turn a stored font name into a pretty name:
const char* Fl::get_font_name(Fl_Font fnum, int* ap) {
  Fl_Fontdesc *f = fl_fonts + fnum;
  if (!f->fontname[0]) {
    const char* p = f->name;
    if (!p || !*p) {if (ap) *ap = 0; return "";}
    int type;
    switch (*p) {
    case 'B': type = FL_BOLD; break;
    case 'I': type = FL_ITALIC; break;
    case 'P': type = FL_BOLD | FL_ITALIC; break;
    default:  type = 0; break;
    }
    strlcpy(f->fontname, p+1, ENDOFBUFFER);
    if (type & FL_BOLD) strlcat(f->fontname, " bold", ENDOFBUFFER);
    if (type & FL_ITALIC) strlcat(f->fontname, " italic", ENDOFBUFFER);
    f->fontname[ENDOFBUFFER] = (char)type;
  }
  if (ap) *ap = f->fontname[ENDOFBUFFER];

/*
    if (ap) *ap = type;
    strlcpy(f->fontname, p+1, sizeof(f->fontname));
    if (!type) {
      if (p[1] == ':') return f->fontname + 1;
      return f->fontname;
    }
    if (type & FL_BOLD) strlcat(f->fontname, " bold", sizeof(f->fontname));
    if (type & FL_ITALIC) strlcat(f->fontname, " italic", sizeof(f->fontname));
  }
*/
  return f->fontname;
}

static int fl_free_font = FL_FREE_FONT;


Fl_Font Fl::set_fonts(const char* xstarname) {
#pragma unused ( xstarname )
  if (fl_free_font != FL_FREE_FONT) 
    return (Fl_Font)fl_free_font;
  ItemCount o = 0;
  ATSUFontCount(&o);
  ATSUFontID *id = (ATSUFontID*) malloc(sizeof(ATSUFontID) * (o + 1));
  ATSUGetFontIDs(id, o, &o);
  char b[256];
  char b2[2048];
  ByteCount l;
  OSStatus status;
  for (int oo = 0; oo < o; oo++) {
    ItemCount ox;
    status = ATSUFindFontName(id[oo], kFontFullName, 
                kFontNoPlatform, kFontNoScript,
                kFontNoLanguage, 256, b, &l, &ox);
    int ii;
    if (!b[0]) {
	ii = fl_unicode2utf((xchar*)b, l / 2, b2 + 2);
        if (ii < 0) ii = 0;
        ii += 2;
        b2[1] = ':';
        b2[0] = ' ';
    } else {
      for (int i = 0; i < l; i++) {
        b2[i+1] = b[i];
      } 
      ii = l+1;
      b2[0] = ' ';
    }
    b2[ii] = 0;
    int i;
    for (i=0; i<FL_FREE_FONT; i++) // skip if one of our built-in fonts
      if (!strcmp(Fl::get_font_name((Fl_Font)i),(char*)b2+1)) break;
    if ( i < FL_FREE_FONT ) continue;
    Fl::set_font((Fl_Font)(fl_free_font++), strdup((char*)b2));
    }
  free(id);
  printf("yes %d\n", fl_free_font);
  return (Fl_Font)fl_free_font;
}

static int array[128];
int Fl::get_font_sizes(Fl_Font fnum, int*& sizep) {
  sizep = array;
  for (int i = 1; i < 64; i++) { array[i - 1] = i;}
  return 63;
}

//
// End of "$Id: fl_set_fonts_mac.cxx,v 1.1.2.7.2.2 2003/11/07 03:47:25 easysw Exp $".
//
