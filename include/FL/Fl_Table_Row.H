//
// "$Id$"
//

#ifndef _FL_TABLE_ROW_H
#define _FL_TABLE_ROW_H

//
// Fl_Table_Row -- A row oriented table widget
//
//    A class specializing in a table of rows.
//    Handles row-specific selection behavior.
//
// Copyright 2002 by Greg Ercolano.
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
// Please report all bugs and problems to "erco at seriss dot com".
//
//

#include <fltk3/TableRow.h>
#include "Fl_Table.H"


FLTK3_WRAPPER_INTERFACE_BEGIN(Fl_Table_Row, TableRow)
FLTK3_WRAPPER_INTERFACE_WIDGET(Fl_Table_Row, TableRow)
FLTK3_WRAPPER_INTERFACE_END()


class FL_EXPORT Fl_Table_Row : public Fl_Table {

public:

  enum TableRowSelectMode {
    SELECT_NONE,		// no selection allowed
    SELECT_SINGLE,		// single row selection
    SELECT_MULTI		// multiple row selection (default)
  }; 

#if 0 // FIXME: 123
protected:
  int handle(int event);
  int find_cell(TableContext context,		// find cell's x/y/w/h given r/c
                int R, int C, int &X, int &Y, int &W, int &H) {
    return(Fl_Table::find_cell(context, R, C, X, Y, W, H));
  }
#endif
 
public:

  FLTK3_WIDGET_VCALLS(Fl_Table_Row, TableRow)

  Fl_Table_Row(int X, int Y, int W, int H, const char *l=0)  {
    _p = new fltk3::TableRow_I(_ctor_dx(X), _ctor_dy(Y), W, H, l);
    _p->wrapper(this);
  }
  
#if 0 // FIXME: 123
  /**
   The destructor for the Fl_Table_Row.
   Destroys the table and its associated widgets.
   */
  ~Fl_Table_Row() { }
  
  void rows(int val);			// set number of rows
  int rows() {				// get number of rows
    return(Fl_Table::rows());
  }
  
  /**
   Sets the table selection mode.
   
   - \p Fl_Table_Row::SELECT_NONE - No selection allowed
   - \p Fl_Table_Row::SELECT_SINGLE - Only single rows can be selected
   - \p Fl_Table_Row::SELECT_MULTI - Multiple rows can be selected
   */
  void type(TableRowSelectMode val);	// set selection mode
  
  TableRowSelectMode type() const {	// get selection mode
    return(_selectmode);
  }
  
#endif

  int row_selected(int row) {
    return ((fltk3::TableRow*)_p)->row_selected(row);
  }
  
#if 0 // FIXME: 123
  /**
   Changes the selection state for 'row', depending on the value
   of 'flag'.  0=deselected, 1=select, 2=toggle existing state.
   */
  int select_row(int row, int flag=1);	// select state for row: flag:0=off, 1=on, 2=toggle
  // returns: 0=no change, 1=changed, -1=range err
  
  /**
   This convenience function changes the selection state 
   for \em all rows based on 'flag'. 0=deselect, 1=select, 2=toggle existing state.
   */
  void select_all_rows(int flag=1);	// all rows to a known state
  
  void clear() {
    rows(0);		// implies clearing selection
    cols(0);
    Fl_Table::clear();	// clear the table
  }
#endif

};

#endif /*_FL_TABLE_ROW_H*/

//
// End of "$Id$".
//
