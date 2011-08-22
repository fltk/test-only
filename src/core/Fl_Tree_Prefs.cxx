//
// "$Id$"
//

#include <fltk3/run.h>
#include <fltk3/Pixmap.h>
#include <fltk3/TreePrefs.h>
#include <string.h>

//////////////////////
// fltk3::TreePrefs.cxx
//////////////////////
//
// fltk3::Tree -- This file is part of the fltk3::Tree widget for FLTK
// Copyright (C) 2009-2010 by Greg Ercolano.
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

// INTERNAL: BUILT IN OPEN/STOW XPMS
//    These can be replaced via prefs.openicon()/closeicon()
//
static const char *L_open_xpm[] = {
#ifdef __APPLE__
  "11 11 2 1",
  ".  c None",
  "@  c #000000",
  "...@.......",
  "...@@......",
  "...@@@.....",
  "...@@@@....",
  "...@@@@@...",
  "...@@@@@@..",
  "...@@@@@...",
  "...@@@@....",
  "...@@@.....",
  "...@@......",
  "...@......."
#else
  "11 11 3 1",
  ".	c #fefefe",
  "#	c #444444",
  "@	c #000000",
  "###########",
  "#.........#",
  "#.........#",
  "#....@....#",
  "#....@....#",
  "#..@@@@@..#",
  "#....@....#",
  "#....@....#",
  "#.........#",
  "#.........#",
  "###########"
#endif
};
static fltk3::Pixmap L_openpixmap(L_open_xpm);

static const char *L_close_xpm[] = {
#ifdef __APPLE__
  "11 11 2 1",
  ".  c None",
  "@  c #000000",
  "...........",
  "...........",
  "...........",
  "...........",
  "...........",
  "@@@@@@@@@@@",
  ".@@@@@@@@@.",
  "..@@@@@@@..",
  "...@@@@@...",
  "....@@@....",
  ".....@....."
#else
  "11 11 3 1",
  ".	c #fefefe",
  "#	c #444444",
  "@	c #000000",
  "###########",
  "#.........#",
  "#.........#",
  "#.........#",
  "#.........#",
  "#..@@@@@..#",
  "#.........#",
  "#.........#",
  "#.........#",
  "#.........#",
  "###########"
#endif
};
static fltk3::Pixmap L_closepixmap(L_close_xpm);

/// Sets the default icon to be used as the 'open' icon
/// when items are add()ed to the tree.
/// This overrides the built in default '[+]' icon.
///
/// \param[in] val -- The new image, or zero to use the default [+] icon.
///
void fltk3::TreePrefs::openicon(fltk3::Image *val) {
  _openimage = val ? val : &L_openpixmap;
}

/// Sets the icon to be used as the 'close' icon.
/// This overrides the built in default '[-]' icon.
///
/// \param[in] val -- The new image, or zero to use the default [-] icon.
///
void fltk3::TreePrefs::closeicon(fltk3::Image *val) {
  _closeimage = val ? val : &L_closepixmap;
}

/// fltk3::TreePrefs constructor
fltk3::TreePrefs::TreePrefs() {
  _labelfont              = fltk3::HELVETICA;
  _labelsize              = fltk3::NORMAL_SIZE;
  _marginleft             = 6;
  _margintop              = 3;
  //_marginright          = 3;
  //_marginbottom         = 3;
  _openchild_marginbottom = 0;
  _usericonmarginleft     = 3;
  _labelmarginleft        = 3;
  _linespacing            = 0;
  _labelfgcolor           = fltk3::BLACK;
  _labelbgcolor           = fltk3::WHITE;
  _connectorcolor         = fltk3::Color(43);
#ifdef __APPLE__
  _connectorstyle         = fltk3::TREE_CONNECTOR_NONE;
#else
  _connectorstyle         = fltk3::TREE_CONNECTOR_DOTTED;
#endif
  _openimage              = &L_openpixmap;
  _closeimage             = &L_closepixmap;
  _userimage              = 0;
  _showcollapse           = 1;
  _showroot               = 1;
  _connectorwidth         = 17;
  _sortorder              = fltk3::TREE_SORT_NONE;
  _selectbox              = fltk3::FLAT_BOX;
  _selectmode             = fltk3::TREE_SELECT_SINGLE;
  // Let fltk's current 'scheme' affect defaults
  if ( fltk3::scheme() ) {
    if ( strcmp(fltk3::scheme(), "gtk+") == 0 ) {
      _selectbox = fltk3::GTK_THIN_UP_BOX;
    } else if ( strcmp(fltk3::scheme(), "plastic") == 0 ) {
      _selectbox = fltk3::PLASTIC_THIN_UP_BOX;
    }
  }
}

//
// End of "$Id$".
//
