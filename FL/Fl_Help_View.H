/
// "$Id$
/
// Help Viewer widget definitions
/
// Copyright 1997-2004 by Easy Software Products
// Image support donated by Matthias Melcher, Copyright 2000
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#ifndef Fl_Help_View_
#  define Fl_Help_View_

/
// Include necessary header files..
/

#  include <stdio.h
#  include "Fl.H
#  include "Fl_Group.H
#  include "Fl_Scrollbar.H
#  include "fl_draw.H
#  include "Fl_Shared_Image.H


/
// Fl_Help_Func type - link callback function for files..
/


typedef const char *(Fl_Help_Func)(Fl_Widget *, const char *)


/
// Fl_Help_Block structure..
/

struct Fl_Help_Bloc

  const char	*start,		// Start of tex
		*end;		// End of tex
  uchar		border;		// Draw border
  Fl_Color	bgcolor;	// Background colo
  int		x,		// Indentation/starting X coordinat
		y,		// Starting Y coordinat
		w,		// Widt
		h;		// Heigh
  int		line[32];	// Left starting position for each lin
}

/
// Fl_Help_Link structure..
/

struct Fl_Help_Lin

  char		filename[192],	// Reference filenam
		name[32];	// Link target (blank if none
  int		x,		// X offset of link tex
		y,		// Y offset of link tex
		w,		// Width of link tex
		h;		// Height of link tex
}

/
// Fl_Help_Target structure..
/

struct Fl_Help_Targe

  char		name[32];	// Target nam
  int		y;		// Y offset of targe
}

/
// Fl_Help_View class..
/

/** The Fl_Help_View widget displays HTML text. Most HTML 2.0 elements ar
 * supported, as well as a primitive implementation of tables. GIF, JPEG
 * and PNG images are displayed inline. *
class FL_EXPORT Fl_Help_View : public Fl_Group	//// Help viewer widge

  enum { RIGHT = -1, CENTER, LEFT };	// Alignment

  char		title_[1024];		// Title strin
  Fl_Color	defcolor_,		// Default text colo
		bgcolor_,		// Background colo
		textcolor_,		// Text colo
		linkcolor_;		// Link colo
  uchar		textfont_,		// Default font for tex
		textsize_;		// Default font siz
  const char	*value_;		// HTML text valu

  int		nblocks_,		// Number of blocks/paragraph
		ablocks_;		// Allocated block
  Fl_Help_Block	*blocks_;		// Block

  int		nfonts_;		// Number of fonts in stac
  uchar		fonts_[100][2];		// Font stac

  Fl_Help_Func	*link_;			// Link transform functio

  int		nlinks_,		// Number of link
		alinks_;		// Allocated link
  Fl_Help_Link	*links_;		// Link

  int		ntargets_,		// Number of target
		atargets_;		// Allocated target
  Fl_Help_Target *targets_;		// Target

  char		directory_[1024];	// Directory for current fil
  char		filename_[1024];	// Current filenam
  int		topline_,		// Top line in documen
		leftline_,		// Lefthand positio
		size_,			// Total document lengt
		hsize_;			// Maximum document widt
  Fl_Scrollbar	scrollbar_,		// Vertical scrollbar for documen
		hscrollbar_;		// Horizontal scrollba

  Fl_Help_Block	*add_block(const char *s, int xx, int yy, int ww, int hh, uchar border = 0)
  void		add_link(const char *n, int xx, int yy, int ww, int hh)
  void		add_target(const char *n, int yy)
  static int	compare_targets(const Fl_Help_Target *t0, const Fl_Help_Target *t1)
  int		do_align(Fl_Help_Block *block, int line, int xx, int a, int &l)
  void		draw()
  void		format()
  void		format_table(int *table_width, int *columns, const char *table)
  int		get_align(const char *p, int a)
  const char	*get_attr(const char *p, const char *n, char *buf, int bufsize)
  Fl_Color	get_color(const char *n, Fl_Color c)
  Fl_Shared_Image *get_image(const char *name, int W, int H)
  int		get_length(const char *l)
  int		handle(int)

  void		initfont(uchar &f, uchar &s) { nfonts_ = 0
			fl_font(f = fonts_[0][0] = textfont_
			        s = fonts_[0][1] = textsize_); 
  void		pushfont(uchar f, uchar s) { if (nfonts_ < 99) nfonts_ ++
			fl_font(fonts_[nfonts_][0] = f
			        fonts_[nfonts_][1] = s); 
  void		popfont(uchar &f, uchar &s) { if (nfonts_ > 0) nfonts_ --
			fl_font(f = fonts_[nfonts_][0]
			        s = fonts_[nfonts_][1]); 

  public

    /** The constructor creates the Fl_Help_View widget at the specifie
     * position and size. *
  Fl_Help_View(int xx, int yy, int ww, int hh, const char *l = 0)
    /** The destructor destroys the widget and frees all memory that ha
     * been allocated for the current file. *
  ~Fl_Help_View()
    /** This method returns the current directory (base) path for the file in the buffer. *
  const char	*directory() const { if (directory_[0]) return (directory_)
  					else return ((const char *)0); 
    /** This method returns the current filename for the text in the buffer. *
  const char	*filename() const { if (filename_[0]) return (filename_)
  					else return ((const char *)0); 
  int		find(const char *s, int p = 0)
    /** This method assigns a callback function to use when a link is followe
     * or a file is loaded (via Fl_Help_View::load()) that requires a differen
     * file or path. The callback function receives a pointer to the Fl_Help_Vie
     * widget and the URI or full pathname for the file in question. It mus
     * return a pathname that can be opened as a local file or \c NULL
     *
     * <tt>const char *fn(Fl_Widget *w, const char *uri);</tt
     
     * The link function can be used to retrieve remote or virtual documents
     * returning a temporary file that contains the actual data. If the lin
     * function returns \c NULL, the value of the Fl_Help_View widget wil
     * remain unchanged
     
     * If the link callback cannot handle the URI scheme, it should retur
     * the uri value unchanged or set the value() of the widget befor
     * returning \c NULL. *
  void		link(Fl_Help_Func *fn) { link_ = fn; 
    /** This method loads the specified file or URL. *
  int		load(const char *f)
  void		resize(int,int,int,int)
    /** This method returns the length of the buffer text in pixels. *
  int		size() const { return (size_); 
  void		size(int W, int H) { Fl_Widget::size(W, H); 
    /** Sets the default text color. *
  void		textcolor(Fl_Color c) { if (textcolor_ == defcolor_) textcolor_ = c; defcolor_ = c; 
    /** Returns the current default text color. *
  Fl_Color	textcolor() const { return (defcolor_); 
    /** Sets the default text font. *
  void		textfont(uchar f) { textfont_ = f; format(); 
    /** Returns the current default text font. *
  uchar		textfont() const { return (textfont_); 
    /** Sets the default text size. *
  void		textsize(uchar s) { textsize_ = s; format(); 
    /** Returns the current default text size. *
  uchar		textsize() const { return (textsize_); 
    /** This method returns the current document title, or \c NULL if ther
     * is no title. *
  const char	*title() { return (title_); 
    /** Scroll the text to the indicated position with a named destination. *
  void		topline(const char *n)
    /** Scroll the text to the indicated position by pixel line. *
  void		topline(int)
    /** Returns the current top line in pixels. *
  int		topline() const { return (topline_); 
  void		leftline(int)
  int		leftline() const { return (leftline_); 
    /** Sets the current buffer to the string provided and reformats the text. *
  void		value(const char *v)
    /** Returns the current buffer contents.*
  const char	*value() const { return (value_); 
}

#endif // !Fl_Help_View_

/
// End of "$Id$"
/
