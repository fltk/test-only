//
// "$Id$"
//
// Hash table implementation used in various places in fltk.
// It is used for Symbools (Boxtypes) and other fltk obljects
//
// Copyright 1998-2005 by Bill Spitzak and others.
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
// Implementation temporartly in fl_symbols.cxx

struct Fl_Hash_Item{
  const char * name;
  void * data;
};


class Fl_Hash_Table{
  int size;
  int no_items;
  Fl_Hash_Item * table;
  unsigned index(const char *name, unsigned n, bool delok);
  void double_table();
public:
  void * set(const char * name, void * data);
  void * remove(const char* name);
  void * find(const char *name);
  Fl_Hash_Table(int size = 113);
};


//
// End of "$Id$".
//
