//
// "$Id: Fl_Style.h,v 1.10 2002/10/29 00:37:23 easysw Exp $"
//
// Style structure used by Fl_Widgets
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

#ifndef Fl_Style_h
#define Fl_Style_h

#include "Enumerations.h"
//#include "Fl_Color.h"
#include "Fl_Boxtype.h"
#include "Fl_Labeltype.h"
#include "Fl_Font.h"

class FL_API Fl_Widget;

typedef void (*Fl_Glyph)(const Fl_Widget*, int type,
			 int,int,int,int,
			 Fl_Flags);

extern "C" {typedef bool (*Fl_Theme)();}

/**

   Each widget has a pointer to an instance of Fl_Style. Usually many widgets
   share pointers to the same Fl_Style. All the Fl_Styles are linked into a 
   hierarchy of parents & child styles, so that it is possible to change an 
   item in a parent style and propagate all the changes to the children. 

   When a widget looks up a value from a style, it looks at that style and 
   each parent up until it finds a non-zero value to return, or until there
   are no more parents, in which case zero is used. Thus changing a parent 
   style can make global changes as long as widgets do not have local 
   values set. 

   In normal usage "set" methods like Fl_Widget::box(n) will create a "unique"
   style for that widget, which is a child of the original style, and set the 
   box in that style. This "unique" style is reused for any other changes to 
   that widget and it is deleted when the widget is deleted. 

*/
struct FL_API Fl_Style {
  const Fl_Style* parent;
  void  (*revertfunc)(Fl_Style*);

  /**
   The type of box to draw around the outer edge of the widget 
   (for the majority of widgets, some classes ignore this or use 
   it to draw only text fields inside the widget). 
   The default is FL_DOWN_BOX. 
  */
  Fl_Boxtype    box;
  /**
   The type of box to draw buttons internal the widget
   (notice that Fl_Button uses box, however). 
   The default is FL_UP_BOX.
  */
  Fl_Boxtype    button_box;
  /**
   Type of box to draw to indicate focus. This is always called 
   with the FL_INVISIBLE flag as only the border should be drawn. 
   The default is FL_DOTTED_FRAME. 
  */
  Fl_Boxtype    focus_box;
  /**
   Draw part of a widget, called a "glyph". Examples are the up and down 
   arrows on sliders, the slider itself, and check boxes. The base class 
   draws arrow buttons (because they are so commonly used), but most widgets
   that use this define their own function to call. A theme can make these
   functions point at it's own code and thus customize the appearance of 
   widgets quite a bit. 

   The default glyph function can draw these values for glyph: 


   - Zero is reserved to draw an empty box 
   - FL_GLYPH_UP - draw an up arrow 
   - FL_GLYPH_UP_BUTTON - draw an up arrow in a box 
   - FL_GLYPH_DOWN - draw a down arrow 
   - FL_GLYPH_DOWN_BUTTON - draw a down arrow in a box 
   - FL_GLYPH_LEFT - draw a left arrow 
   - FL_GLYPH_LEFT_BUTTON - draw a left arrow in a box 
   - FL_GLYPH_RIGHT - draw a right arrow 
   - FL_GLYPH_RIGHT_BUTTON - draw a right arrow in a box scrollbar
   
   All other values draw an empty box. 
   If you wish to write your own glyph function you should examine the
   ones in the fltk source code and in the sample theme plugins to see
   how the arguments are interpreted.

  */
  Fl_Glyph      glyph;
  /**
   The font used to draw the label. Default is FL_HELVETICA.
  */
  Fl_Font       label_font;
  /**
   Provides a font for text displays. Default is FL_HELVETICA.
  */
  Fl_Font       text_font;
  /**
   How to draw the label. This provides things like inset, 
   shadow, and the symbols. FL_NORMAL_LABEL. 
  */
  Fl_Labeltype  label_type;
  /**
   Color of the widgets. The default is FL_WHITE. This color is inherited by
   text displays and browsers, most of the "gray" you see is due to widgets 
   like Fl_Group that default the color to gray, and the fact that many parts
   of the interface are drawn with the button_color described below. 

   If you want to change the overall color of all the gray parts of the 
   interface you want to call fl_background(color) instead, as this will set 
   the entry for FL_GRAY and also set the "gray ramp" so that the edges of 
   buttons are the same color. 
  */
  Fl_Color      color;
  /**
   Color used when drawing internal buttons. Default is FL_GRAY.
   Check and radio buttons use this to color in the glyph. 
  */
  Fl_Color      button_color;
  /**
   Color used to draw labels and glyphs. Default is FL_BLACK.
  */
  Fl_Color      label_color;
  /**
   Color drawn behind selected text in inputs, or selected
   browser or menu items, or lit light buttons. 
   The default is Windows 95 blue.
  */
  Fl_Color      selection_color;
  /**
   The color to draw text atop the selection_color. The default is FL_WHITE. 
  */
  Fl_Color      selection_text_color;
  /**
   The color to draw the widget when the mouse is over it 
   (for scrollbars and sliders this is used to color the buttons). 
   The default of zero disables highlighting. 
  */
  Fl_Color      highlight_color;
  /**
   Color to draw labels atop the highlight_color. The default
   of zero leaves the label_color unchanged. 
  */
  Fl_Color      highlight_label_color;
  /**
   Color to draw text inside the widget. Default is black.
   Check and radio buttons use this to control the color of the checkmark.
  */
  Fl_Color      text_color;
  /**
   Size of label_font. Default is 12. 
  */
  unsigned      label_size;
  /**
   Size of text_font. Checkboxes and some other widgets use 
   this to control the size of the glyph. Default is 12. 
  */
  unsigned      text_size;
  /**
   Extra spacing added between text lines or other things that 
   are stacked vertically. The default is 0 (menus have a default of 4). 
  */
  unsigned      leading;

  // global settings:
  static bool   draw_boxes_inactive;
  static unsigned scrollbar_width;
  static Fl_Flags scrollbar_align;
  static int	wheel_scroll_lines;

  Fl_Style(); // creates a dynamic() style
  bool dynamic() const {return !revertfunc;}

  /**

   This is a static method on Fl_Style. It returns the first style found that
   matches the name. Case is ignored and spaces and underscores are considered
   equivalent. If nothing matches NULL is returned. 

   Theme-setting code uses this to locate the styles it has to modify. Using a
   string to locate the styles serves several purposes: 

   It allows a theme plugin to locate and modify styles that are in your 
   program as well as in the fltk library. 

   It allows a theme plugin to read a text database and not have to implement
   a translation from names to styles. 

   If you static-link your fltk program and include a theme function, it won't
   link in all the classes that you are not using but it tries to modify. 
   On machines that allow a plugin to be loaded into a static-linked program
   (some Unix systems, Linux if you use -shared) it means the plugin can work
   even if it wants to modify some fltk classes that were not linked in. 

  */
  static Fl_Style* find(const char* name);
  static void revert();
  static Fl_Theme theme() {return theme_;}
  static void theme(Fl_Theme f) {theme_ = f;}
  static Fl_Theme load_theme(const char*);
  static void load_theme();
  static void reload_theme();
  static const char* scheme() {return scheme_;}
  static void scheme(const char* f) {scheme_ = f;}

private:
  static Fl_Theme theme_;
  static const char* scheme_;
};

/**

   This subclass of Fl_Style creates a style that can be returned
   by Fl_Style::find(). It also provides a "revert" method that 
   is used to restore the initial state so that themes can be 
   switched. 

   name is the name you want themes to use to look up the style. 
   If you don't care if themes can modify your class you can 
   just use NULL for the name. 

   revert is a function that initializes the style to it's 
   default value. The style is cleared to zero (meaning inherit 
   from the default style) before revert is called. If revert is
   NULL then all-zeros is the default. 

   pointer is a back-pointer to the pointer to the style. 
   This allows multiple themes to exist at once, if an application
   wants it (for instance a theme-editor application would like this).
   The application can change this pointer to point at different style
   structures and then all widgets that are constructed after that get
   the new style. If you don't plan to use your widget in such a program
   you can make a static instance of your style (rather than using new) 
   and pass null as this pointer. 

   Here is an example of a class that wants the 
   box to default to FL_ROUND_BOX: 

   \code

   static void revert(Fl_Style* s) {
     s->box = FL_ROUND_BOX;
   }
   static Fl_Named_Style* style = new Fl_Named_Style("MyWidget", revert, &style);

   MyWidget::MyWidget(int x, int y, int w, int h, const char *l)
     : Fl_Widget(x,y,w,h,l)
   {
     style(::style);
   }

  \endcode

*/
struct FL_API Fl_Named_Style : public Fl_Style {
  const char* name;
  Fl_Named_Style* next;
  static Fl_Named_Style* first;
  Fl_Named_Style** back_pointer; // used by Fl_Style_Set
  Fl_Named_Style(const char* name, void (*revert)(Fl_Style*), Fl_Named_Style**);
};

enum Fl_Glyphtype {	// glyph types understood by Fl_Widget::default_glyph()
  // the numbers 0-99 are reserved for private glyph functions
  FL_GLYPH_BOX = 100,
  FL_GLYPH_UP,
  FL_GLYPH_DOWN,
  FL_GLYPH_LEFT,
  FL_GLYPH_RIGHT,
  FL_GLYPH_UP_BUTTON,
  FL_GLYPH_DOWN_BUTTON,
  FL_GLYPH_LEFT_BUTTON,
  FL_GLYPH_RIGHT_BUTTON
};

extern "C" FL_API bool fltk_theme();
FL_API bool fl_get_system_colors();
FL_API const char* fl_find_config_file(char* out, int size, const char* name);

#endif

//
// End of "$Id: Fl_Style.h,v 1.10 2002/10/29 00:37:23 easysw Exp $".
//
