//
// "$Id: Fl_Font.h,v 1.2 2002/10/29 00:37:23 easysw Exp $"
//
// Fonts used by FLTK styles and drawing code
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

#ifndef Fl_Font_h
#define Fl_Font_h

#include <fltk/Enumerations.h> // for FL_API

class FL_API Fl_FontSize;

//class FL_API Fl_Font_;
struct FL_API Fl_Font_;

/**
   The type passed to Fl_Widget::label_font() and other parts of fltk is a 
   typedef defined like this, pointing at a structure Fl_Font_: 
   \code
   typedef Fl_Font_* Fl_Font;
   \endcode

   To draw text in a font you use fl_font(Fl_Font,size) to select a font and
   scale, and then fl_draw(const char*) to draw letters. See the documentaion
   on drawing functions. 

   The following Fl_Font values are predefined. These names are historical,
   actually FL_HELVETICA will give the system's sans-serif font, FL_TIMES 
   will give the system's serif font, and FL_COURIER will give the system's
   fixed-pitch font. FL_SCREEN will give a fixed-pitch font designed for 
   computer i/o, it may match FL_COURIER. Avoid FL_SYMBOL and FL_ZAPF_DINGBATS
   for compatability with future versions of fltk. 

   - FL_HELVETICA (Ariel on Win32) 
   - FL_HELVETICA_BOLD 
   - FL_HELVETICA_ITALIC 
   - FL_HELVETICA_BOLD_ITALIC 
   - FL_COURIER 
   - FL_COURIER_BOLD 
   - FL_COURIER_ITALIC 
   - FL_COURIER_BOLD_ITALIC 
   - FL_TIMES 
   - FL_TIMES_BOLD 
   - FL_TIMES_ITALIC 
   - FL_TIMES_BOLD_ITALIC 
   - FL_SCREEN (Terminal style font) 
   - FL_SCREEN_BOLD 
   - FL_SYMBOL 
   - FL_ZAPF_DINGBATS (Wingdings font on Win32!) 
*/
typedef const Fl_Font_* Fl_Font;

// This is a struct so I can init a table with constants:
struct FL_API Fl_Font_ {
  const char* name_;
  Fl_Font_* bold_;
  Fl_Font_* italic_;
// list of sizes (used by X and Win32 GDI):
  Fl_FontSize* first;
// crap needed by X:
  char **xlist;
  int n;
//public:
  // return the system-specific name, different for each font:
  /**
   Returns a string that identifies the font in a system-specific manner. 
   About all that can be said about it is that it is different for every 
   font (two different fonts may return the same name()). This string is not
   portable, even between different machines running the 
   same operating system. 
  */
  const char* system_name() const {return name_;}
  // return the "nice" name, and the attributes:
  /**
   Returns the name of the font. The return value points to a static
   buffer that is overwritten each call (so copy the string if you 
   want to keep it). 

   The integer pointed to by attributes is set to zero, FL_BOLD or 
   FL_ITALIC or FL_BOLD|FL_ITALIC. If this pointer is null then the attributes
   are indicated by adding a space and "bold" or "italic" to the name.
  */
  const char* name(int* = 0) const;
  // return array of sizes:
  /**
   Sets array to point at a list of sizes. The return value is the length of 
   this array. The sizes are sorted from smallest to largest and indicate what
   sizes can be given to fl_font() that will be matched exactly 
   (fl_font() will pick the closest size for other sizes). A zero in the 
   first location of the array indicates a scalable font, where any size 
   works, although the array may list sizes that work "better" than others.
   The returned array points at a static buffer that is overwritten each call,
   so you want to copy it if you plan to keep it. 
  */
  int sizes(int*&) const;
  // return array of encodings:
  /**
   Sets array to point at a list of encoding names. The return value is the 
   length of this array. Each string identifies an "encoding" that is 
   supported by this font. These strings may be passed to the fl_encoding() 
   to select what characters the first 256 codes print. A zero length 
   array may be returned, this indicates that the font will print the same
   no matter what encoding is set. 
  */
  int encodings(const char**&) const;
  /*@{*/
  /**
   Pointers to the bold and italic versions of this font. 
   FL_HELVETICA->bold() is the same as FL_HELVETICA_BOLD, 
   FL_TIMES->bold()->italic() is the same as FL_TIMES_BOLD_ITALIC. 

   These are never null. If this font has no bold or italic version 
   then these are circular pointers. Thus FL_TIMES_BOLD->bold() is the
   same as FL_TIMES_BOLD and FL_SYMBOL->bold() is the same as FL_SYMBOL. 
  */
  Fl_Font_* bold() const { return bold_; }
  Fl_Font_* italic() const { return italic_; }
  /*@}*/
  // "measurement" is considered a drawing function, see fl_draw.h

};

// values for attributes:
enum {
  FL_BOLD = 1,
  FL_ITALIC = 2,
  FL_BOLD_ITALIC = 3
};

// Built-in fonts:
extern FL_API Fl_Font_ fl_fonts[16];
#define FL_HELVETICA		(fl_fonts+0)
#define FL_HELVETICA_BOLD	(fl_fonts+1)
#define FL_HELVETICA_ITALIC	(fl_fonts+2)
#define FL_HELVETICA_BOLD_ITALIC (fl_fonts+3)
#define FL_COURIER		(fl_fonts+4)
#define FL_COURIER_BOLD		(fl_fonts+5)
#define FL_COURIER_ITALIC	(fl_fonts+6)
#define FL_COURIER_BOLD_ITALIC	(fl_fonts+7)
#define FL_TIMES		(fl_fonts+8)
#define FL_TIMES_BOLD		(fl_fonts+9)
#define FL_TIMES_ITALIC		(fl_fonts+10)
#define FL_TIMES_BOLD_ITALIC	(fl_fonts+11)
#define FL_SYMBOL		(fl_fonts+12)
#define FL_SCREEN		(fl_fonts+13)
#define FL_SCREEN_BOLD		(fl_fonts+14)
#define FL_ZAPF_DINGBATS	(fl_fonts+15)

// Find a font by name + attributes:
/**
   Find a font with the given "nice" name. You can get bold and italic by 
   adding a space and "bold" or "italic" (or both) to the name, or by passing
   them as the attributes. Case is ignored and fltk will accept some 
   variations in the font name. 

   The current implementation calls fl_list_fonts() and then does a binary
   search for the font in it. This can make the first call pretty slow, 
   especially on X. 
*/
FL_API Fl_Font fl_find_font(const char* name, int attrib = 0);

// Find and return every font on the system.
/**
   This allocates and fills in an array containing every font on the server. 
   The location arrayp is set to a pointer to this array, and the length of 
   the array is the return value. Each entry is a "base" font, there may be
   bold, italic, and bold+italic version of each font pointed to by bold() 
   or italic(). 

   Subsequent calls to this function returns the same array again. Currently
   there is no way to update the list from any changes to the set of fonts
   on the server.  
*/
FL_API int fl_list_fonts(Fl_Font*& arrayp);

#ifndef FLTK_2
#define FL_NORMAL_SIZE	12
#endif

#endif
