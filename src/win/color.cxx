//
// "$Id$"
//
// WIN32 color functions for the Fast Light Tool Kit (FLTK).
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

// The fltk "colormap".  This allows ui colors to be stored in 8-bit
// locations, and provides a level of indirection so that global color
// changes can be made.  Not to be confused with the X colormap, which
// I try to hide completely.

// SGI compiler seems to have problems with unsigned char arguments
// being used to index arrays.  So I always copy them to an integer
// before use.

#include <config.h>
#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/fl_draw.H>
#include "Fl_Win_Display.H"




// New color and line_style code.
// Stock pen is used for 1 (0) width  solid lines
// Other lines are use pens from pen_cache

#ifndef DC_BRUSH
# define DC_BRUSH 18
#endif
#ifndef DC_PEN
# define DC_PEN 19
#endif


typedef COLORREF (WINAPI *pfSetDCPenColor)(HDC hdc, COLORREF crColor);
typedef COLORREF (WINAPI *pfSetDCBrushColor)(HDC hdc, COLORREF crColor);

static pfSetDCPenColor	 __SetDCPenColor = NULL;
static pfSetDCBrushColor __SetDCBrushColor = NULL;
static bool dc_funcs_init = false;


static void load_dc_funcs()
{
  dc_funcs_init = true;
  HINSTANCE hmod = LoadLibraryW(L"Gdi32.dll");  
  if(hmod) {
    __SetDCPenColor = (pfSetDCPenColor)GetProcAddress(hmod, "SetDCPenColor");
    __SetDCBrushColor = (pfSetDCBrushColor)GetProcAddress(hmod, "SetDCBrushColor");
    FreeLibrary(hmod);
  }
}




FL_EXPORT COLORREF for_color = RGB(0,0,0);
static int for_style = 0;
static int for_width = 1;
FL_EXPORT  DWORD _for_dashes[16]; 
static DWORD *for_dashes = _for_dashes;





#define PEN_CACHE_SIZE  16  // size of cache for non-stock pens

struct Pen_Cache{
  Pen_Cache * next;
	int lstyle;
	int width;
	COLORREF color;
	DWORD dashes[16];
	HPEN pen;
} pen_cache[PEN_CACHE_SIZE];

static Pen_Cache * first_pen = 0;
static int no_pens = 0;

static char inline cmp(DWORD * d1, DWORD * d2){
  for (int n=0; n<16; n++){
    if(!*d1)
      if(*d2)
        return 1;
      else
        return 0;
    if( *d1++ != *d2++) return 1;
  }
  return 0;
}


#if HAVE_DCBRUSH
  static HPEN stockpen = (HPEN)GetStockObject(DC_PEN);
#else
  static HPEN stockpen = 0;
#endif


FL_EXPORT HPEN fl_pen(){
  static COLORREF current_color = RGB(0,0,0);
  static int current_style = 0;
  static int current_width = 1;
  static DWORD * current_dashes =0; 


#if HAVE_DCBRUSH
  if(stockpen){
    if (!for_style && for_width <= 1) {
    if (!dc_funcs_init) load_dc_funcs();
    if (__SetDCPenColor) {
      SelectObject(fl_gc, stockpen);
      (*__SetDCPenColor)(fl_gc, for_color);
      return stockpen;
    }
	}
  }
#endif


  Pen_Cache * pen = first_pen;
	Pen_Cache * prev = 0;
	Pen_Cache * prev_prev = 0;
	for(;pen; prev_prev = prev, prev = pen, pen = pen->next)
		if(for_color == pen->color && for_style  == pen->lstyle && for_width == pen->width){
       if((for_style & PS_USERSTYLE) && cmp(for_dashes, pen->dashes))	continue;
			// we have found the pen! We pull it to the front, if not allreadu first...
      if(first_pen !=pen){
			  if(prev) prev->next = pen->next;
			  pen->next = first_pen;
			  first_pen = pen;
      }
			current_style = for_style;
			current_width = for_width;
			current_dashes = pen->dashes;
			current_color = for_color;
      SelectObject(fl_gc, pen->pen);
			return pen->pen;
		}
	// pen not found, need to create one
	if(no_pens == PEN_CACHE_SIZE){ // cache is full, need to delete the one used the longest time ago. We know that the pen is not selected.
		prev_prev->next = 0;
		DeleteObject(prev->pen);
	}else
		prev = pen_cache + no_pens++; // where to place new pen
  if ((prev->lstyle = for_style)) {
    LOGBRUSH penbrush = {BS_SOLID, prev->color = for_color, 0};
    int n = 0; 
    DWORD * das = for_dashes;
    if(for_style & PS_USERSTYLE){
      while(n < 16) if(!(prev->dashes[n++] = *das++)) break;
      n--;
    }else{
      prev->dashes[0] = 0;
      n=0;
    }
    prev->pen = ExtCreatePen(for_style|PS_GEOMETRIC, prev->width = for_width, &penbrush, n, n ? prev->dashes : 0);
  }else{
    prev->pen = CreatePen(PS_SOLID, prev->width = for_width, prev->color=for_color);
	  prev->dashes[0] = 0;
    prev->lstyle=for_style;
	}
	prev->next = first_pen;
	first_pen  = prev;
	current_style = for_style;
	current_width = for_width;
	current_dashes = first_pen->dashes;
	current_color = for_color;
  SelectObject(fl_gc, first_pen->pen);
  return  first_pen->pen;
};
#if HAVE_DCBRUSH
  static HBRUSH stockbrush = (HBRUSH)GetStockObject(DC_BRUSH);
#else
  static HBRUSH stockbrush = 0;
#endif

static HBRUSH current_brush = 0;

HBRUSH fl_brush() {

  static COLORREF current_brush_color;

  int f_style = for_style;
  int f_width = for_width;
  for_dashes = 0;
  for_style = 0;
  for_width = 0;
  fl_pen();
  for_dashes = _for_dashes;
  for_style = f_style;
  for_width = f_width;
  if(current_brush && (for_color == current_brush_color)){
	  SelectObject(fl_gc, current_brush); //assure that we have selected the brush for proper gc if it changed by any chance
	  return current_brush;
  }
  current_brush_color = for_color;


#if HAVE_DCBRUSH
  if(stockbrush){
	if (!dc_funcs_init) load_dc_funcs();
	if (__SetDCBrushColor) {
		SelectObject(fl_gc, stockbrush);
		(*__SetDCBrushColor)(fl_gc, for_color); 
		//current_brush = stockbrush;
		return stockbrush;
	}
  }
#endif
  if(current_brush){
	  SelectObject(fl_gc, GetStockObject(WHITE_BRUSH)); // Release our brush from dc	
	  DeleteObject(current_brush);
  }

  current_brush = CreateSolidBrush(for_color);
  SelectObject(fl_gc, current_brush);
  return current_brush;
};

Fl_Color fl_color_;

void Fl_Win_Display::color(Fl_Color i) {
  if (i & 0xffffff00) {
    unsigned rgb = (unsigned)i;
    fl_color((uchar)(rgb >> 24), (uchar)(rgb >> 16), (uchar)(rgb >> 8));
  } else {
    fl_color_ = i;
#if USE_COLORMAP
    if (fl_palette) {
	    for_color =  PALETTEINDEX(i);
    } else {
#endif
	    unsigned c = fl_cmap[i];
	    for_color = RGB(uchar(c>>24), uchar(c>>16), uchar(c>>8));
#if USE_COLORMAP
    }
#endif
  }
}


void Fl_Win_Display::color(uchar r, uchar g, uchar b) {
  for_color = RGB(r,g,b);
  fl_color_ = fl_rgb_color(r,g,b);
}



void Fl_Win_Display::line_style(int style, int width, char* dashes) {
  static DWORD Cap[4]= {PS_ENDCAP_FLAT, PS_ENDCAP_FLAT, PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE};
  static DWORD Join[4]={PS_JOIN_ROUND, PS_JOIN_MITER, PS_JOIN_ROUND, PS_JOIN_BEVEL};
  if (dashes && dashes[0]) {
    for_style = PS_USERSTYLE | Cap[(style>>8)&3] | Join[(style>>12)&3];
    int n; 
    for (n = 0; n < 16;) if(!(for_dashes[n++] = *dashes++)) break;;
    //dash_pattern_size = n;
    // fix cards that ignore dash pattern for zero width:
    //if (!width) width = 1;
  } else {
    //dash_pattern_size = 0;
    for_dashes[0] = 0;
    // allow any low 8 bits for style
    for_style = style & 0xff | Cap[(style>>8)&3] | Join[(style>>12)&3];
  }
  // for some reason zero width does not work at all:
  if (!width) width = 1;
  for_width = width;
}


HPALETTE fl_palette;


/*




#define FL_N_PEN 32 // number of non-stock pens



  


// Translations to win32 data structures:

Fl_XMap fl_xmap_[256];
Fl_XMap *fl_xmap = fl_xmap_;

Fl_XMap* fl_current_xmap;

HPALETTE fl_palette;

static HBRUSH tmpbrush=0;
*/
static HPEN savepen=0;

static HPEN tmppen=0;

void fl_save_pen(void) {
    if(!tmppen) tmppen = CreatePen(PS_SOLID, 1, 0);
    savepen = (HPEN)SelectObject(fl_gc, tmppen);
}

void fl_restore_pen(void) {
    if (savepen) SelectObject(fl_gc, savepen);
    savepen = 0;
}
/*
static void clear_xmap(Fl_XMap& xmap) {
  if (xmap.pen) {
    if(!tmppen) tmppen = CreatePen(PS_SOLID, 1, 0);
    if(!tmpbrush) tmpbrush = CreateSolidBrush(0);
    HPEN oldpen = (HPEN)SelectObject(fl_gc, tmppen); // Push out the current pen of the gc
    if(oldpen != xmap.pen) SelectObject(fl_gc, oldpen); // Put it back if it is not the one we are about to delete
    SelectObject(fl_gc, tmpbrush); // Push out the old pen of the gc
    //fl_current_xmap = 0;
    DeleteObject((HGDIOBJ)(xmap.pen));
    xmap.pen = 0;
    xmap.brush = -1;
  }
}

static void set_xmap(Fl_XMap& xmap, COLORREF c) {
  xmap.rgb = c;
  xmap.pen = CreatePen(PS_SOLID, 1, xmap.rgb);
  xmap.brush = -1;
}


Fl_Color fl_color_;

void Fl_Win_Display::color(Fl_Color i) {
  if (i & 0xffffff00) {
    unsigned rgb = (unsigned)i;
    fl_color((uchar)(rgb >> 24), (uchar)(rgb >> 16), (uchar)(rgb >> 8));
  } else {
    fl_color_ = i;
    Fl_XMap &xmap = fl_xmap[i];
    if (!xmap.pen) {
#if USE_COLORMAP
      if (fl_palette) {
	      for_color = PALETTEINDEX(i);
      } else {
#endif
	    unsigned c = fl_cmap[i];
	    for_color = RGB(uchar(c>>24), uchar(c>>16), uchar(c>>8));
#if USE_COLORMAP
      }
#endif
    }

  }
}


static Fl_XMap xmap;
void Fl_Win_Display::color(uchar r, uchar g, uchar b) {
 
  COLORREF c = RGB(r,g,b);
  fl_color_ = fl_rgb_color(r, g, b);
  if (!xmap.pen || c != xmap.rgb) {
    clear_xmap(xmap);
    set_xmap(xmap, c);
  }
  fl_current_xmap = &xmap;
  SelectObject(fl_gc, (HGDIOBJ)(xmap.pen));
}

// Wonko: we use some statistics to cache only a limited number
// of brushes:

#define FL_N_BRUSH 16

struct Fl_Brush {
    HBRUSH brush;
    unsigned short usage;
    Fl_XMap* backref;
  };
Fl_Brush brushes_[FL_N_BRUSH];
static Fl_Brush * brushes = brushes_;

HBRUSH fl_brush() {
  Fl_XMap *xmap = fl_current_xmap;



  int i = xmap->brush; // find the associated brush
  if (i != -1) { // if the brush was allready allocated
    if (brushes[i].brush == NULL) goto CREATE_BRUSH;
    if ( (++brushes[i].usage) > 32000 ) { // keep a usage statistic
      for (int j=0; j<FL_N_BRUSH; j++) {
	if (brushes[j].usage>16000)
	  brushes[j].usage -= 16000;
	else 
	  brushes[j].usage = 0;
      }
    }
    return brushes[i].brush;
  } else {
    int umin = 32000, imin = 0;
    for (i=0; i<FL_N_BRUSH; i++) {
      if (brushes[i].brush == NULL) goto CREATE_BRUSH;
      if (brushes[i].usage<umin) {
	umin = brushes[i].usage;
	imin = i;
      }
    }
    i = imin;
    DeleteObject(brushes[i].brush);
    brushes[i].brush = NULL;
    brushes[i].backref->brush = -1;
  }
CREATE_BRUSH:
  brushes[i].brush = CreateSolidBrush(xmap->rgb);
  brushes[i].usage = 0;
  brushes[i].backref = xmap;
  xmap->brush = i;
  return brushes[i].brush;
}
*/
void Fl::free_color(Fl_Color i, int overlay) {
  //if (overlay) return; // do something about GL overlay?
  //clear_xmap(fl_xmap[i]);
  //not doing anything
}

void Fl::set_color(Fl_Color i, unsigned c) {
    fl_cmap[i] = c;
}
#if USE_COLORMAP

// 'fl_select_palette()' - Make a color palette for 8-bit displays if necessary
// Thanks to Michael Sweet @ Easy Software Products for this

HPALETTE
fl_select_palette(void)
{
  static char beenhere;
  if (!beenhere) {
    beenhere = 1;

    //if (GetDeviceCaps(fl_gc, BITSPIXEL) > 8) return NULL;
    int nColors = GetDeviceCaps(fl_gc, SIZEPALETTE);
    if (nColors <= 0 || nColors > 256) return NULL;
    // this will try to work on < 256 color screens, but will probably
    // come out quite badly.

    // I lamely try to get this variable-sized object allocated on stack:
    ulong foo[(sizeof(LOGPALETTE)+256*sizeof(PALETTEENTRY))/sizeof(ulong)+1];
    LOGPALETTE *pPal = (LOGPALETTE*)foo;

    pPal->palVersion    = 0x300;
    pPal->palNumEntries = nColors;

    // Build 256 colors from the standard FLTK colormap...

    for (int i = 0; i < nColors; i ++) {
      pPal->palPalEntry[i].peRed   = (fl_cmap[i] >> 24) & 255;
      pPal->palPalEntry[i].peGreen = (fl_cmap[i] >> 16) & 255;
      pPal->palPalEntry[i].peBlue  = (fl_cmap[i] >>  8) & 255;
      pPal->palPalEntry[i].peFlags = 0;
    };

    // Create the palette:
    fl_palette = CreatePalette(pPal);
  }
  if (fl_palette) {
    SelectPalette(fl_gc, fl_palette, FALSE);
    RealizePalette(fl_gc);
  }
  return fl_palette;
}

#endif




//void _fl_clear_xmap(Fl_XMap& xmap){clear_xmap(xmap);};
/*

static HPEN tmppen_save;
static HBRUSH tmpbrush_save;
static HPEN savepen_save;
static Fl_Brush *brushes_save;
static Fl_XMap *fl_xmap_save;
//static Fl_XMap xmap_save;


*/
static Fl_Region r_save;

FL_EXPORT HDC fl_gc_save = 0;
    
extern Fl_Region fl_clip_region();


static HBRUSH stockbrush_save = 0;
static HPEN stockpen_save = 0;

FL_EXPORT void push_xmaps(){
  if(fl_gc_save) return; //allready pushed
  r_save = fl_clip_region();
  if(current_brush){
	  SelectObject(fl_gc, GetStockObject(WHITE_BRUSH)); // Release our brush from dc	
	  DeleteObject(current_brush);
	  current_brush = 0;
  }
  stockbrush_save = stockbrush;
  stockbrush = 0;
  stockpen_save = stockpen;
  stockpen = 0;
  //tmpbrush_save = tmpbrush;
  //savepen_save = savepen;
  //brushes_save = brushes;
  //fl_xmap_save = fl_xmap;
  fl_gc_save = fl_gc;
  Pen_Cache * pen  = first_pen;
  while(pen){
	  if(pen->pen) DeleteObject(pen->pen);
	  pen = pen->next;
  }
  first_pen = 0;
  no_pens = 0;
};
/*
FL_EXPORT void set_xmaps(HPEN tmppen_, HBRUSH tmpbrush_, HPEN savepen_, Fl_Brush *brushes_, Fl_XMap * fl_xmap_, Fl_XMap xmap_){
  tmppen = tmppen_;
  tmpbrush= tmpbrush_;
  savepen = savepen_;
  brushes = brushes_;
  fl_xmap = fl_xmap_;
};
*/

FL_EXPORT void pop_xmaps(){
  if(!fl_gc_save) return;
  Pen_Cache * pen  = first_pen;
  while(pen){
	  if(pen->pen) DeleteObject(pen->pen);
	  pen = pen->next;
  }
  first_pen = 0;
  no_pens = 0;
  if(current_brush){
	  SelectObject(fl_gc, GetStockObject(WHITE_BRUSH)); // Release our brush from dc	
	  DeleteObject(current_brush);
	  current_brush = 0;
  }
  fl_gc = fl_gc_save;
  fl_gc_save = 0;
  stockbrush = stockbrush_save;
  stockpen = stockpen_save;


  //tmppen = tmppen_save;
  //tmpbrush = tmpbrush_save;
  //savepen = savepen_save;
  //brushes = brushes_save;
  //fl_xmap = fl_xmap_save;
  fl_clip_region(r_save);
}


 


//
// End of "$Id$".
//
