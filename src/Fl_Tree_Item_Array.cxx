//
// "$Id$"
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fltk3/TreeItemArray.h>
#include <fltk3/TreeItem.h>

//////////////////////
// fltk3::TreeItemArray.cxx
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

/// Constructor; creates an empty array.
///
///     The optional 'chunksize' can be specified to optimize
///     memory allocation for potentially large arrays. Default chunksize is 10.
/// 
fltk3::TreeItemArray::TreeItemArray(int new_chunksize) {
  _items     = 0;
  _total     = 0;
  _size      = 0;
  _chunksize = new_chunksize;
}

/// Destructor. Calls each item's destructor, destroys internal _items array.
fltk3::TreeItemArray::~TreeItemArray() {
  clear();
}

/// Copy constructor. Makes new copy of array, with new instances of each item.
fltk3::TreeItemArray::TreeItemArray(const fltk3::TreeItemArray* o) {
  _items = (fltk3::TreeItem**)malloc(o->_size * sizeof(fltk3::TreeItem*));
  _total     = o->_total;
  _size      = o->_size;
  _chunksize = o->_chunksize;
  for ( int t=0; t<o->_total; t++ ) {
    _items[t] = new fltk3::TreeItem(o->_items[t]);
  }
}

/// Clear the entire array.
///
///     Each item will be deleted (destructors will be called),
///     and the array will be cleared. total() will return 0.
///
void fltk3::TreeItemArray::clear() {
  if ( _items ) {
    for ( int t=0; t<_total; t++ ) {
      delete _items[t];
      _items[t] = 0;
    }
    free((void*)_items); _items = 0;
  }
  _total = _size = 0;
}

// Internal: Enlarge the items array.
//
//    Adjusts size/items memory allocation as needed.
//    Does NOT change total.
//
void fltk3::TreeItemArray::enlarge(int count) {
  int newtotal = _total + count;	// new total
  if ( newtotal >= _size ) {		// more than we have allocated?
    // Increase size of array
    int newsize = _size + _chunksize;
    fltk3::TreeItem **newitems = (fltk3::TreeItem**)malloc(newsize * sizeof(fltk3::TreeItem*));
    if ( _items ) { 
      // Copy old array -> new, delete old
      memmove(newitems, _items, _size * sizeof(fltk3::TreeItem*));
      free((void*)_items); _items = 0;
    }
    // Adjust items/sizeitems
    _items = newitems;
    _size = newsize;
  }
}

/// Insert an item at index position \p pos.
///
///     Handles enlarging array if needed, total increased by 1.
///     If \p pos == total(), an empty item is appended to the array.
///
void fltk3::TreeItemArray::insert(int pos, fltk3::TreeItem *new_item) {
  enlarge(1);
  // printf("*** POS=%d TOTAL-1=%d NITEMS=%d\n", pos, _total-1, (_total-pos));
  if ( pos <= (_total - 1) ) {	// need to move memory around?
    int nitems = _total - pos;
    memmove(&_items[pos+1], &_items[pos], sizeof(fltk3::TreeItem*) * nitems);
  } 
  _items[pos] = new_item;
  _total++;
}

/// Add an item* to the end of the array.
///
///     Assumes the item was created with 'new', and will remain
///     allocated.. fltk3::TreeItemArray will handle calling the
///     item's destructor when the array is cleared or the item remove()'ed.
///
void fltk3::TreeItemArray::add(fltk3::TreeItem *val) {
  insert(_total, val);
}

/// Remove the item at \param[in] index from the array.
///
///     The item will be delete'd (if non-NULL), so its destructor will be called.
///
void fltk3::TreeItemArray::remove(int index) {
  if ( _items[index] ) {		// delete if non-zero
    delete _items[index];
  }
  _items[index] = 0;
  for ( _total--; index<_total; index++ ) {
    _items[index] = _items[index+1];
  }
}

/// Remove the item from the array.
///
///     \returns 0 if removed, or -1 if the item was not in the array.
///
int fltk3::TreeItemArray::remove(fltk3::TreeItem *item) {
  for ( int t=0; t<_total; t++ ) {
    if ( item == _items[t] ) {
      remove(t);
      return(0);
    }
  }
  return(-1);
}

//
// End of "$Id$".
//
