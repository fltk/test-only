//
// "$Id: fl_font_mac.cxx,v 1.3 2002/12/09 04:52:29 spitzak Exp $"
//
// MacOS font selection routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

//: MeasureText, FontMetrics, WidthTabHandle, GetSysFont, SysFontSize
//: TextSize, TextFont
//: GetFNum (theName: Str255; VAR familyID: Integer);
//: FUNCTION FMSwapFont (inRec: FMInput): FMOutPtr;

FontSize::FontSize(const char* name, int Size) {
  knowMetrics = 0;
  switch (*name++) {
  case 'I': face = italic; break;
  case 'P': face = italic | bold; break;
  case 'B': face = bold; break;
  default: face = 0; break;
  }
  unsigned char fn[80]; 
  fn[0] = strlen(name); strcpy((char*)(fn+1), name);
  GetFNum(fn, &font);
  size = Size;
  FMInput fIn = { font, size, face, 0, 0, { 1, 1}, { 1, 1} };
  FMOutput *fOut = FMSwapFont(&fIn);
  ascent = fOut->ascent;	//: the following three lines give only temporary aproimations
  descent = fOut->descent;
  for (int i=0; i<256; i++) width[i] = fOut->widMax;
#if HAVE_GL
  listbase = 0;
#endif
  minsize = maxsize = size;
}

FontSize* fontsize = 0L;

FontSize::~FontSize() {
/*
#if HAVE_GL
// Delete list created by gl_draw().  This is not done by this code
// as it will link in GL unnecessarily.  There should be some kind
// of "free" routine pointer, or a subclass?
// if (listbase) {
//  int base = font->min_char_or_byte2;
//  int size = font->max_char_or_byte2-base+1;
//  int base = 0; int size = 256;
//  glDeleteLists(listbase+base,size);
// }
#endif
  */
  if (this == fontsize) fontsize = 0;
}

////////////////////////////////////////////////////////////////

static Fontdesc built_in_table[] = {
{" Arial"},
{"BArial"},
{"IArial"},
{"PArial"},
{" Courier New"},
{"BCourier New"},
{"ICourier New"},
{"PCourier New"},
{" Times New Roman"},
{"BTimes New Roman"},
{"ITimes New Roman"},
{"PTimes New Roman"},
{" Symbol"},
{" Chicago"},
{"BChicago"},
{" Webdings"},
};

Fontdesc* fonts = built_in_table;

void setfont(FontSize* s) {
  fontsize = s;
  if (xwindow) SetPort( GetWindowPort(xwindow) );
  TextFont(fontsize->font);	//: select font into current QuickDraw GC
  TextFace(fontsize->face);
  TextSize(fontsize->size);
  if (!fontsize->knowMetrics) {	//: get the true metrics for the currnet GC (fails on multiple monitors with different dpi's!)
    FontInfo fi; GetFontInfo(&fi);
    fontsize->ascent = fi.ascent;
    fontsize->descent = fi.descent;
    FMetricRec mr; FontMetrics(&mr);
    short *f = (short*)*mr.wTabHandle; //: get the char size table
    for (int i=0; i<256; i++) fontsize->width[i] = f[2*i];
    fontsize->knowMetrics = 1;
  }
}

static FontSize* find(int fnum, int size) {
  Fontdesc* s = fonts+fnum;
  if (!s->name) s = fonts; // use 0 if fnum undefined
  FontSize* f;
  for (f = s->first; f; f = f->next)
    if (f->minsize <= size && f->maxsize >= size) return f;
  f = new FontSize(s->name, size);
  f->next = s->first;
  s->first = f;
  return f;
}

////////////////////////////////////////////////////////////////
// Public interface:


void fltk::setfont(Font* font, float size) {
  setfont(find(fnum, size));
}

int fltk::getascent()  { return fontsize->ascent; }
int fltk::getdescent() { return fontsize->descent; }

float fltk::getwidth(const char* c, int n) {
  return float(TextWidth( c, 0, n ));
}

void fltk::drawtext_transformed(const char *str, int n, float x, float y) {
  MoveTo(int(floorf(x+.5)), int(floorf(y+l5)));
  DrawText(str, 0, n);
}

//
// End of "$Id: fl_font_mac.cxx,v 1.3 2002/12/09 04:52:29 spitzak Exp $".
//
