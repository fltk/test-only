//
// "$Id: fl_set_fonts_mac.cxx,v 1.3 2002/12/09 04:52:30 spitzak Exp $"
//
// MacOS font utilities for the Fast Light Tool Kit (FLTK).
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

// This function fills in the fltk font table with all the fonts that
// are found on the X server.  It tries to place the fonts into families
// and to sort them so the first 4 in a family are normal, bold, italic,
// and bold italic.

// turn a stored font name into a pretty name:
const char* get_font_name(Font* fnum, int* ap) {
  const char* p = fonts[fnum].name;
  if (!p || !*p) {if (ap) *ap = 0; return "";}
  int type;
  switch (*p) {
  case 'B': type = BOLD; break;
  case 'I': type = ITALIC; break;
  case 'P': type = BOLD | ITALIC; break;
  default:  type = 0; break;
  }
  if (ap) *ap = type;
  if (!type) return p+1;
  static char *buffer; if (!buffer) buffer = new char[128];
  strcpy(buffer, p+1);
  if (type & BOLD) strcat(buffer, " bold");
  if (type & ITALIC) strcat(buffer, " italic");
  return buffer;
}

static int free_font = FREE_FONT;

Font* setfonts(const char* xstarname) {
#pragma unused ( xstarname )
  if (free_font != FREE_FONT) 
    return (Font*)free_font;
  static char styleLU[] = " BIP";
  FMFontFamilyInstanceIterator ffiIterator;
  FMFontFamilyIterator ffIterator;
  FMFontFamily family;
  FMFont font;
  FMFontStyle style; // bits 0..6: bold, italic underline, outline, shadow, condens, extended (FLTK supports 0 and 1 )
  FMFontSize size;
  //FMFilter filter; // do we need to set a specific (or multiple) filter(s) to get ALL fonts?
  
  Str255 buf;
  //filter.format = kFMCurrentFilterFormat;
  //filter.selector = kFMGenerationFilterSelector;
  //filter.filter.generationFilter = 
  FMCreateFontFamilyIterator( NULL, NULL, kFMUseGlobalScopeOption, &ffIterator );
  OSStatus listFamilies, listInstances;
  for (;;)
  {
    listFamilies = FMGetNextFontFamily( &ffIterator, &family );
    if ( listFamilies != 0 ) break;
    FMGetFontFamilyName( family, buf );
    buf[ buf[0]+1 ] = 0;
    //printf( "Font Family: %s\n", buf+1 );
    int i;
    for (i=0; i<FREE_FONT; i++) // skip if one of our built-in fonts
      if (!strcmp(get_font_name((Font*)i),(char*)buf+1)) break;
    if ( i < FREE_FONT ) continue;
    FMCreateFontFamilyInstanceIterator( family, &ffiIterator );
    char pStyle = 0, nStyle;
    for (;;)
    {
      listInstances = FMGetNextFontFamilyInstance( &ffiIterator, &font, &style, &size );
      if ( listInstances != 0 ) break;
      // printf(" %d %d %d\n", font, style, size );
      nStyle = styleLU[style&0x03];
      if ( ( pStyle & ( 1<<(style&0x03) ) ) == 0 )
      {
        buf[0] = nStyle;
        setfont((Font*)(free_font++), strdup((char*)buf));
        pStyle |= ( 1<<(style&0x03) );
      }
    }
    FMDisposeFontFamilyInstanceIterator( &ffiIterator );
  }
  FMDisposeFontFamilyIterator( &ffIterator );
  return (Font*)free_font;
}

static int array[128];
int get_font_sizes(Font* fnum, int*& sizep) {
  Fontdesc *s = fonts+fnum;
  if (!s->name) s = fonts; // empty slot in table, use entry 0

  Str255 name;
  int len = strlen( s->name ); strncpy( (char*)(name+1), s->name+1, len ); name[0] = len-1;
  FMFontFamily family = FMGetFontFamilyFromName( name );
  if ( family == kInvalidFontFamily ) return 0;

  sizep = array;
  FMFont font;
  FMFontStyle style, fStyle;
  switch ( s->name[0] ) {
    default :
      fStyle=0;
      break;
    case 'B' : 
      fStyle=1;
      break;
    case 'I' : 
      fStyle=2;
      break;
    case 'P' :
      fStyle=3;
      break;
  }
  FMFontSize size, pSize = -1;
  FMFontFamilyInstanceIterator ffiIterator;
  FMCreateFontFamilyInstanceIterator( family, &ffiIterator );
  int cnt = 0;
  OSStatus listInstances;
  for (;;)
  {
    listInstances = FMGetNextFontFamilyInstance( &ffiIterator, &font, &style, &size );
    if ( listInstances != 0 ) break;
    if ( style==fStyle )
    {
      if ( size>pSize ) 
      {
        array[ cnt++ ] = size;
        pSize = size;
      }
    }
  }
  FMDisposeFontFamilyInstanceIterator( &ffiIterator );

  return cnt;
}

//
// End of "$Id: fl_set_fonts_mac.cxx,v 1.3 2002/12/09 04:52:30 spitzak Exp $".
//
