//
// "$Id: HelpView.h,v 1.2 2003/04/20 03:17:47 easysw Exp $"
//
// Help Viewer widget definitions.
//
// Copyright 1997-2003 by Easy Software Products.
// Image support donated by Matthias Melcher, Copyright 2000.
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

#ifndef HelpView_H
#  define HelpView_H

//
// Include necessary header files...
//

#  include <stdio.h>
#  include <fltk/Group.h>
#  include <fltk/Scrollbar.h>
#  include <fltk/draw.h>
#  include <fltk/SharedImage.h>

namespace fltk {

//
// HelpFunc type - link callback function for files...
//


typedef const char *(HelpFunc) (Widget *, const char *);


//
// HelpBlock structure...
//

struct HelpBlock {
  const char *start,            // Start of text
   *end;                        // End of text
  uchar border;                 // Draw border?
  Color bgcolor;             // Background color
  int x,                        // Indentation/starting X coordinate
    y,                          // Starting Y coordinate
    w,                          // Width
    h;                          // Height
  int line[32];                 // Left starting position for each line
};

//
// HelpLink structure...
//

struct HelpLink {
  char filename[192],           // Reference filename
    name[32];                   // Link target (blank if none)
  int x,                        // X offset of link text
    y,                          // Y offset of link text
    w,                          // Width of link text
    h;                          // Height of link text
};

//
// HelpTarget structure...
//

struct HelpTarget {
  char name[32];                // Target name
  int y;                        // Y offset of target
};

//
// HelpView class...
//

class FL_API HelpView : public Group    //// Help viewer widget
{
  enum { RIGHT = -1, CENTER, LEFT };    // Alignments

  char title_[1024];            // Title string
  Color defcolor_,           // Default text color
    bgcolor_,                   // Background color
    textcolor_,                 // Text color
    linkcolor_;                 // Link color
  Font *textfont_;              // Default font for text
  int textsize_;                  // Default font size
  const char *value_;           // HTML text value

  int nblocks_,                 // Number of blocks/paragraphs
    ablocks_;                   // Allocated blocks
  HelpBlock *blocks_;       // Blocks

  int nfonts_;                  // Number of fonts in stack
  Font *fonts_[100];         // Font stack
  int fontsizes_[100];

  HelpFunc *link_;          // Link transform function

  int nlinks_,                  // Number of links
    alinks_;                    // Allocated links
  HelpLink *links_;         // Links

  int ntargets_,                // Number of targets
    atargets_;                  // Allocated targets
  HelpTarget *targets_;     // Targets

  char directory_[1024];        // Directory for current file
  char filename_[1024];         // Current filename
  int topline_,                 // Top line in document
    leftline_,                  // Lefthand position
    size_,                      // Total document length
    hsize_;                     // Maximum document width
  Scrollbar *scrollbar_,      // Vertical scrollbar for document
    *hscrollbar_;                // Horizontal scrollbar

  HelpBlock *add_block (const char *s, int xx, int yy, int ww, int hh,
                            uchar border = 0);
  void add_link (const char *n, int xx, int yy, int ww, int hh);
  void add_target (const char *n, int yy);
  static int compare_targets (const HelpTarget * t0,
                              const HelpTarget * t1);
  int do_align (HelpBlock * block, int line, int xx, int a, int &l);
  void draw ();
  void format ();
  void format_table (int *table_width, int *columns, const char *table);
  int get_align (const char *p, int a);
  const char *get_attr (const char *p, const char *n, char *buf, int bufsize);
  Color get_color (const char *n, Color c);
  SharedImage *get_image (const char *name, int W, int H);
  int get_length (const char *l);
  int handle (int);

  void initfont (Font *&f, int &s) {
    nfonts_ = 0;
	f = fonts_[0] = textfont_;
	s = fontsizes_[0] = textsize_;
    setfont (f, (float)s-1);
  } 
  void pushfont (Font *f, int s) {
    if (nfonts_ < 99) nfonts_++;
	fonts_[nfonts_] = f;
	fontsizes_[nfonts_] = s;
    setfont (f, (float)s-1);
  }
  void popfont (Font *&f, int &s) {
    if (nfonts_ > 0) nfonts_--;
    f = fonts_[nfonts_];
	s = fontsizes_[nfonts_];
    setfont(f, (float)s-1);
  }

public:

  HelpView (int xx, int yy, int ww, int hh, const char *l = 0);

  ~HelpView ();
  const char *directory () const {
    if (directory_[0])
      return (directory_);
    else
      return ((const char *) 0);
  } 
  const char *filename () const {
    if (filename_[0])
      return (filename_);
    else
      return ((const char *) 0);
  } void link (HelpFunc * fn) {
    link_ = fn;
  }
  int load (const char *f);
  void resize (int, int, int, int);
  void layout();
  int size () const { return (size_); } 
  void textcolor (Color c) {
    if (textcolor_ == defcolor_)
      textcolor_ = c;
    defcolor_ = c;
  }
  Color textcolor () const { return (defcolor_); } 
  void textfont (Font *f) {
    textfont_ = f;
    format();
  }
  Font *textfont () const { return (textfont_); } 
  void textsize (int s) {
    textsize_ = s;
    format ();
  }
  int textsize () const { return (textsize_); } 
  const char *title () { return (title_); }
  void topline (const char *n);
  void topline (int);
  int topline () const { return (topline_); } 
  void leftline (int);
  int leftline () const { return (leftline_); } 
  void value (const char *v);
  const char *value () const { return (value_); }
};

} // namespace fltk

#endif // !HelpView_H

//
// End of "$Id: HelpView.h,v 1.2 2003/04/20 03:17:47 easysw Exp $".
//
