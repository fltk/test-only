//
// "$Id: Fl_Style_Util.H,v 1.2 1999/09/25 22:27:05 vincent Exp $"
//
// Style definition and plugin support
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

#ifndef Fl_Style_Description_h
#define Fl_Style_Description_h

#define STYLE_EXTENSION ".fltk-style-plugin"
#define STYLE_FUNCTION "style_plugin"
#define THEME_EXTENSION ".fltk-theme-plugin"
#define THEME_FUNCTION "theme_plugin"

class Fl_Style;

// Definition of the style associated to a class of widget
struct Fl_Style_D {
  char*     name;
  Fl_Style* style;
};


typedef bool (Fl_Style_Entry_Parser)(Fl_Style& style, char* s);
typedef void (Fl_Style_Plugin_Entry)();

// Call this at any moment to make fltk read all styles and themes plugins
void fl_read_style_plugins();

// Function for plugins
bool fl_add_style_def(char* name, Fl_Style* style);
Fl_Style* fl_search_style(char* name);
void fl_add_style_entry_def(char* name, Fl_Style_Entry_Parser parser);
bool fl_parse_style_entry(char* name, Fl_Style& style, char* s);
char* fl_parse_word(char*&s);
Fl_Color fl_parse_color(char*&s);

#endif

//
// End of "$Id: Fl_Style_Util.H,v 1.2 1999/09/25 22:27:05 vincent Exp $".
//
