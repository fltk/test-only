//
// "$Id$"
//

#ifndef _FLtk3_TREE_ITEM_ARRAY_H
#define _FLtk3_TREE_ITEM_ARRAY_H

#include "Export.h"

namespace fltk3 {
  
  class FLTK3_EXPORT TreeItem;	// forward decl must *precede* first doxygen comment block
				// or doxygen will not document our class..
  
  //////////////////////////
  // fltk3/TreeItemArray.h
  //////////////////////////
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
  
  ///
  /// \file
  /// \brief This file defines a class that manages an array of fltk3::TreeItem pointers.
  ///
  
  /// \brief Manages an array of fltk3::TreeItem pointers.
  ///
  /// Because FLTK 1.x.x. has mandated that templates and STL not be used,
  /// we use this class to dynamically manage the arrays.
  ///
  /// None of the methods do range checking on index values; the caller
  /// must be sure that index values are within the range 0<index<total()
  /// (unless otherwise noted).
  ///
  
  class FLTK3_EXPORT TreeItemArray {
    fltk3::TreeItem **_items;	// items array
    int _total;			// #items in array
    int _size;			// #items *allocated* for array
    int _chunksize;		// #items to enlarge mem allocation
    void enlarge(int count);
  public:
    TreeItemArray(int new_chunksize = 10);		// CTOR
    ~TreeItemArray();				// DTOR
    TreeItemArray(const fltk3::TreeItemArray *o);	// COPY CTOR
                                                        /// Return the item and index \p i.
    fltk3::TreeItem *operator[](int i) {
      return(_items[i]);
    }
    /// Const version of operator[](int i)
    const fltk3::TreeItem *operator[](int i) const {
      return(_items[i]);
    }
    /// Return the total items in the array, or 0 if empty.
    int total() const {
      return(_total);
    }
    /// Swap the two items at index positions \p ax and \p bx.
    void swap(int ax, int bx) {
      fltk3::TreeItem *asave = _items[ax];
      _items[ax] = _items[bx];
      _items[bx] = asave;
    }
    void clear();
    void add(fltk3::TreeItem *val);
    void insert(int pos, fltk3::TreeItem *new_item);
    void remove(int index);
    int  remove(fltk3::TreeItem *item);
  };
  
}

#endif /*_FL_TREE_ITEM_ARRAY_H*/

//
// End of "$Id$".
//
