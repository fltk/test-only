//
// "$Id: fl_font_mac.cxx,v 1.1.2.10.2.2 2003/11/07 03:47:24 easysw Exp $"
//
// MacOS font selection routines for the Fast Light Tool Kit (FLTK).
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

//: MeasureText, FontMetrics, WidthTabHandle, GetSysFont, SysFontSize
//: TextSize, TextFont
//: GetFNum (theName: Str255; VAR familyID: Integer);
//: FUNCTION FMSwapFont (inRec: FMInput): FMOutPtr;
//: SetFractEnable

Fl_FontSize::Fl_FontSize(const char* name, int Size) {
  xchar b[2048];
  bold = italic = false;
  switch (*name++) {
  case 'I': italic = true; break;
  case 'P': italic = bold = true; break;
  case 'B': bold = true; break;
  default: break;
  }
  const char *out;
  unsigned long l;
  if (name[0] == ':') {
    l = fl_utf2unicode((unsigned char*)name + 1, strlen(name + 1), b);
    out = (char*) b;
    l *= 2;
  } else {
    out = name;
    l = strlen(out);
  }
  ATSUFindFontFromName((char*)out, l,  4,
                       kFontNoPlatform, kFontNoScript, kFontNoLanguage,
                       &font);
  size = Size;
  knowMetrics = 0;
  ascent = Size * 4 / 5;	
  descent = Size / 5;
#if HAVE_GL
  listbase = 0;
  for (int u = 0; u < 64; u++) glok[u] = 0;
#endif
  for (int i = 0; i < 64; i++) width[i] = NULL;
  minsize = maxsize = size;
}

Fl_FontSize* fl_fontsize = 0L;

Fl_FontSize::~Fl_FontSize() {
/*
#if HAVE_GL
 // ++ todo: remove OpenGL font alocations
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
  if (this == fl_fontsize) fl_fontsize = 0;
}

////////////////////////////////////////////////////////////////

static Fl_Fontdesc built_in_table[] = {
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

Fl_Fontdesc* fl_fonts = built_in_table;
static int atsu_init = 0;
ATSUStyle fl_atsu_style;
ATSUTextLayout fl_text_layout;

extern unsigned fl_cmap[256];

static void set_color()
{
  ATSUAttributeTag iTag[] = { kATSUColorTag};
  ByteCount iValueSize[] = { sizeof(RGBColor)};
  ATSUAttributeValuePtr iValue[sizeof(iTag) / sizeof(ATSUAttributeTag)];
  RGBColor rgb;
  uchar r, g, b;
  if (fl_color_ & 0xFFFFFF00) {
      r = fl_color_>>24;
      g = fl_color_>>16;
      b = fl_color_>> 8;
  } else {
      unsigned c = fl_cmap[fl_color_ & 0xff];
      r = c>>24;
      g = c>>16;
      b = c>> 8;
  }
  rgb.red   = (r<<8)|r;
  rgb.green = (g<<8)|g;
  rgb.blue  = (b<<8)|b;
  iValue[0] = &rgb;
  ATSUSetAttributes(fl_atsu_style, 1, iTag,  iValueSize, iValue);
}

void fl_font(Fl_FontSize* s) {
  if (!atsu_init) {
    atsu_init = 1;
    ATSUCreateStyle(&fl_atsu_style);
    ATSUCreateTextLayout(&fl_text_layout);
  }
  fl_fontsize = s;
  ATSUAttributeTag iTag[] = { kATSUFontTag, kATSUSizeTag, 
	kATSUQDBoldfaceTag, kATSUQDItalicTag, kATSUColorTag,
       kATSULineDirectionTag, kATSULanguageTag, kATSULineLanguageTag};
  ByteCount iValueSize[] = { sizeof(ATSUFontID),
	sizeof(Fixed), sizeof(Boolean), sizeof(Boolean), 
        sizeof(RGBColor), sizeof(Boolean), sizeof(RegionCode), 
        sizeof(RegionCode)};
  ATSUAttributeValuePtr iValue[sizeof(iTag) / sizeof(ATSUAttributeTag)];
  RGBColor rgb;
  uchar r, g, b;
  if (fl_color_ & 0xFFFFFF00) {
      r = fl_color_>>24;
      g = fl_color_>>16;
      b = fl_color_>> 8;
  } else {
      unsigned c = fl_cmap[fl_color_ & 0xff];
      r = c>>24;
      g = c>>16;
      b = c>> 8;
  }
  rgb.red   = (r<<8)|r;
  rgb.green = (g<<8)|g;
  rgb.blue  = (b<<8)|b;
  Boolean text_dir = false;
  RegionCode reg = verUS;
  Fixed size = fl_fontsize->size << 16;
  iValue[0] = &(fl_fontsize->font);
  iValue[1] = &size;
  iValue[2] = &(fl_fontsize->bold);
  iValue[3] = &(fl_fontsize->italic);
  iValue[4] = &rgb;
  iValue[5] = &text_dir;
  iValue[6] = &reg;
  iValue[7] = &reg;
  ATSUSetAttributes(fl_atsu_style, 5, iTag,  iValueSize, iValue);
  
  if (!fl_fontsize->knowMetrics) {	
    unsigned short wstr[80];
    wstr[0] = 'p'; wstr[1] = '|'; wstr[2] = 'M'; wstr[3] = 'j';
    wstr[4] = 'H'; wstr[5] = 'g'; wstr[6] = 0xa6; wstr[7] = 0xbc;
    wstr[8] = '_'; wstr[9] = 0xbf; wstr[10] = 'y'; wstr[11] = 0xca;
    wstr[12] = 0x3b2; wstr[13] = 0x46e; wstr[14] = 0x569; wstr[15] = 0x5df;
    wstr[16] = 0x63a; wstr[17] = 0x722; wstr[18] = 0xe42; wstr[19] = 0xe0f;
    wstr[20] = 0x10f0; wstr[21] = 0x10da; wstr[22] = 0x264d;
    wstr[23] = 0x2758; wstr[24] = 0x3070; wstr[25] = 0x30c0;
    wstr[26] = 0x2ef0; wstr[27] = 0x3010; wstr[28] = 0xf900;
    wstr[29] = 0xf967; wstr[30] = 0xfe33; wstr[31] = 0xff51;
    wstr[32] = 0xff2d; wstr[33] = 0;
    Rect rect;
    ATSUSetTextPointerLocation(fl_text_layout, wstr, kATSUFromTextBeginning, 
                             kATSUToTextEnd, 33);
    ATSUSetRunStyle(fl_text_layout, fl_atsu_style, kATSUFromTextBeginning,
                    kATSUToTextEnd);
    ATSUMeasureTextImage(fl_text_layout, kATSUFromTextBeginning, 
	kATSUToTextEnd, 0, 0, &rect);
    fl_fontsize->descent = rect.bottom;
    fl_fontsize->ascent = fl_fontsize->size - fl_fontsize->descent;
    if (-rect.top > fl_fontsize->ascent) fl_fontsize->ascent = -rect.top;
    fl_fontsize->knowMetrics = 1;
  }
}

static Fl_FontSize* find(int fnum, int size) {
  Fl_Fontdesc* s = fl_fonts+fnum;
  if (!s->name) s = fl_fonts; // use 0 if fnum undefined
  Fl_FontSize* f;
  for (f = s->first; f; f = f->next)
    if (f->minsize <= size && f->maxsize >= size) return f;
  f = new Fl_FontSize(s->name, size);
  f->next = s->first;
  s->first = f;
  return f;
}

////////////////////////////////////////////////////////////////
// Public interface:
FL_EXPORT int fl_font_ = 0;
FL_EXPORT int fl_size_ = 0;
// Unicode string buffer 
static unsigned short *wstr = NULL;
static int wstr_len     = 0;
static ATSUGlyphInfoArray *array = NULL;

void Fl_Fltk::font(int fnum, int size) {
  fl_font_ = fnum; fl_size_ = size;
  fl_font(find(fnum, size));
}

int Fl_Fltk::height() {
  return fl_fontsize->ascent+fl_fontsize->descent;
}

int Fl_Fltk::descent() {
  return fl_fontsize->descent;
}

double Fl_Fltk::width(const char* c, int n) {
  int wn;
  Rect rect;
  if (n > wstr_len) {
     wstr = (unsigned short*) realloc(wstr, sizeof(short) * (n+1));
     array = (ATSUGlyphInfoArray*)  realloc(array, 
		sizeof(ATSUGlyphInfoArray) + n * sizeof(ATSUGlyphInfo));
     wstr_len = n;
  }
  wn = fl_utf2unicode((const unsigned char *)c, n, (xchar*)wstr);
  if (wn < 1) return 0.0;
  double x = 0;
  for (int i = 0; i < wn; i++) {
    if (!fl_nonspacing(wstr[i])) {
      int w = Fl_Fltk::width(wstr[i]);
      x += w;
    }
  }
  return x;
}

double Fl_Fltk::width(unsigned int c) {
  unsigned short cc = c;
  unsigned int r;
  ATSTrapezoid t; 
  ItemCount ct;
  r = (c & 0xFC00) >> 10;
  if (!fl_fontsize->width[r]) {
    fl_fontsize->width[r] = (short*) malloc(sizeof(short) * 0x0400);
    unsigned short i = 0, cc = r * 0x400;
    for (; i < 0x400; i++) {
      ATSUSetTextPointerLocation(fl_text_layout, &cc, 
          kATSUFromTextBeginning, kATSUToTextEnd, 1);
      ATSUSetRunStyle(fl_text_layout, fl_atsu_style, kATSUFromTextBeginning,
                    kATSUToTextEnd);
      ATSUGetGlyphBounds(fl_text_layout, 0, 0, 0, 1, 
                    kATSUseCaretOrigins, 1, &t, &ct);
      Fixed x;
      x = (t.lowerRight.x - t.lowerLeft.x) >> 16;
      fl_fontsize->width[r][i] = x;
      cc++;
    }
  }
  return (double) fl_fontsize->width[r][c & 0x03FF];
}

void Fl_Fltk::draw(const char* str, int n, int x, int y) {
  int wn;
  if (n > wstr_len) {
     wstr = (unsigned short*) realloc(wstr, sizeof(short) * (n+1));
     array = (ATSUGlyphInfoArray*)  realloc(array, 
		sizeof(ATSUGlyphInfoArray) + n * sizeof(ATSUGlyphInfo));
     wstr_len = n;
  }
  wn = fl_utf2unicode((const unsigned char *)str, n, (xchar*)wstr);
  if (wn < 1) return;
  set_color();
  long yy = y << 16;
  wstr[wn] = 0;
  int lx = 0;
  Float32Point p;
  p.x = x;
  p.y = y;
 
  ATSUSetTextPointerLocation(fl_text_layout, wstr, 
          kATSUFromTextBeginning, kATSUToTextEnd, wn);
             
  ATSUSetRunStyle(fl_text_layout, fl_atsu_style, kATSUFromTextBeginning,
                    kATSUToTextEnd);
              
  ByteCount bc = sizeof(ATSUGlyphInfoArray) + n * sizeof(ATSUGlyphInfo);
  ATSUGetGlyphInfo(fl_text_layout, 0, wn, &bc, array);
  x = 0;
       
  for (int i = 0; i < wn; i++) {
    if (fl_nonspacing(wstr[i])) {
      x -= lx;
    } else {
      lx = Fl_Fltk::width(wstr[i]);
    }
    array->glyphs[i].screenX = x;  
    x += lx;
  }       
  ATSUDrawGlyphInfo(array, p);
}

void Fl_Fltk::rtl_draw(const char* str, int n, int x, int y) {
  int wn;
  if (n > wstr_len) {
     wstr = (unsigned short*) realloc(wstr, sizeof(short) * (n+1));
     array = (ATSUGlyphInfoArray*)  realloc(array, 
		sizeof(ATSUGlyphInfoArray) + n * sizeof(ATSUGlyphInfo));
     wstr_len = n;
  }
  wn = fl_utf2unicode((const unsigned char *)str, n, (xchar*)wstr);
  if (wn < 1) return;
  Float32Point p;
  p.x = x;
  p.y = y;
  set_color();
  long yy = y << 16;
  
  int lx = 0;
  ATSUSetTextPointerLocation(fl_text_layout, wstr, 
          kATSUFromTextBeginning, kATSUToTextEnd, wn);
  ATSUSetRunStyle(fl_text_layout, fl_atsu_style, kATSUFromTextBeginning,
                    kATSUToTextEnd);
  ByteCount bc = sizeof(ATSUGlyphInfoArray) + n * sizeof(ATSUGlyphInfo);
  ATSUGetGlyphInfo(fl_text_layout, 0, wn, &bc, array);
  x = 0;
  for (int i = 0; i < wn; i++) {
    lx = Fl_Fltk::width(wstr[i]);
    x -= lx;
    array->glyphs[i].screenX = x;  
    if (fl_nonspacing(wstr[i])) {
      x += lx;
}
}
  ATSUDrawGlyphInfo(array, p);
}

//
// End of "$Id: fl_font_mac.cxx,v 1.1.2.10.2.2 2003/11/07 03:47:24 easysw Exp $".
//
