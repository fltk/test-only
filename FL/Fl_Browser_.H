//
// "$Id: Fl_Browser_.H,v 1.6.2.4.2.3.2.3 2003/11/11 04:13:26 easysw Exp $"
//
// Common browser header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

// This is the base class for browsers.  To be useful it must
// be subclassed and several virtual functions defined.  The
// Forms-compatable browser and the file chooser's browser are
// subclassed off of this.

// Yes, I know this should be a template...

#ifndef Fl_Browser__H
#define Fl_Browser__H

#ifndef Fl_Group_H
#include "Fl_Group.H"
#endif
#include "Fl_Scrollbar.H"

#define FL_NORMAL_BROWSER	0
#define FL_SELECT_BROWSER	1
#define FL_HOLD_BROWSER		2
#define FL_MULTI_BROWSER	3

/** This is the base class for browsers. To be useful it must be subclassed and several virtual
 * functions defined. The Forms-compatable browser and the file chooser's browser are
 * subclassed off of this.
 *
 * This has been designed so that the subclass has complete control over the storage of the
 * data, although because next() and prev() functions are used to index, it works best as a
 * linked list or as a large block of characters in which the line breaks must be searched for.
 *
 * A great deal of work has been done so that the "height" of a data object does not need to
 * be determined until it is drawn. This is useful if actually figuring out the size of an
 * object requires accessing image data or doing stat() on a file or doing some other slow
 * operation. 
 */
class FL_EXPORT Fl_Browser_ : public Fl_Group {
  int position_;	// where user wants it scrolled to
  int real_position_;	// the current vertical scrolling position
  int hposition_;	// where user wants it panned to
  int real_hposition_;	// the current horizontal scrolling position
  int offset_;		// how far down top_ item the real_position is
  int max_width;	// widest object seen so far
  uchar has_scrollbar_;	// which scrollbars are enabled
  uchar textfont_, textsize_;
  unsigned textcolor_;
  void* top_;		// which item scrolling position is in
  void* selection_;	// which is selected (except for FL_MULTI_BROWSER)
  void *redraw1,*redraw2; // minimal update pointers
  void* max_width_item;	// which item has max_width_

  static int scrollbar_width_;

  void update_top();

protected:

  // All of the following must be supplied by the subclass:
    /** This method must be provided by the subclass to return the first item in the list. */
  virtual void *item_first() const = 0;
    /** This method must be provided by the subclass to return the item in the list after \c p. */
  virtual void *item_next(void *p) const = 0;
    /** This method must be provided by the subclass to return the item in the list before \c p. */
  virtual void *item_prev(void *p) const = 0;
    /** This method must be provided by the subclass to return the height of the item \c p
     * in pixels. Allow for two additional pixels for the list selection box. */
  virtual int item_height(void *p) const = 0;
    /** This method must be provided by the subclass to return the width of the item \c p in 
     * pixels. Allow for two additional pixels for the list selection box. */
  virtual int item_width(void *p) const = 0;
    /** This method may be provided by the subclass to return the height of the item \c p 
     * in pixels. Allow for two additional pixels for the list selection box. This method 
     * differs from item_height in that it is only called for selection and scrolling 
     * operations. The default implementation calls item_height. */
  virtual int item_quick_height(void *) const ;
    /** This method must be provided by the subclass to draw the item p in the area 
     * indicated by x, y, w, and h. */
  virtual void item_draw(void *p,int x,int y,int width,int height) const = 0;
  // you don't have to provide these but it may help speed it up:
    /** This method may be provided by the subclass to indicate the full width of the item 
     * list in pixels. The default implementation computes the full width from the
     * item widths. */
  virtual int full_width() const ;
    /** This method may be provided by the subclass to indicate the full height of the item 
     * list in pixels. The default implementation computes the full height from the 
     * item heights. */
  virtual int full_height() const ;
    /** This method may be provided to return the average height of all items, to be used 
     * for scrolling. The default implementation uses the height of the first item. */
  virtual int incr_height() const ;
  // These only need to be done by subclass if you want a multi-browser:
    /** This method must be implemented by the subclass if it supports multiple selections in 
     * the browser. The s argument specifies the selection state for item p: 0 = off, 1 = on. */
  virtual void item_select(void *,int=1);
    /** This method must be implemented by the subclass if it supports multiple selections in 
     * the browser. The method should return 1 if p is selected and 0 otherwise. */
  virtual int item_selected(void *) const ;

  // things the subclass may want to call:
    /** Returns the item the appears at the top of the list. */
  void *top() const {return top_;}
    /** Returns the item currently selected, or \c NULL if there is no selection. For
     * multiple selection browsers this call returns the last item that was selected. */
  void *selection() const {return selection_;}
    /** This method should be called when the list data is completely replaced or cleared. It
     * informs the Fl_Browser_ widget that any cached information it has concerning the items 
     * is invalid. */
  void new_list(); 
    /** This method should be used when an item is deleted from the list. It allows the 
     * Fl_Browser_ to discard any cached data it has on the item. */
  void deleting(void *a); 
    /** This method should be used when an item is replaced in the list. It allows the 
     * Fl_Browser_ to update its cache data as needed. */
  void replacing(void *a,void *b); 
    /** This method should be used when an item is added to the list. It allows the 
     * Fl_Browser_ to update its cache data as needed. */
  void inserting(void *a,void *b); 
    /** This method returns non-zero if item p is currently visible in the list. */
  int displayed(void *) const ;
    /** This method should be called when the contents of an item have changed but 
     * not changed the height of the item. */
  void redraw_line(void *); 
    /** This method will cause the entire list to be redrawn. */
  void redraw_lines() {damage(FL_DAMAGE_SCROLL);} 
    /** This method returns the bounding box for the interior of the list, 
     * inside the scrollbars. */
  void bbox(int&,int&,int&,int&) const;
    /** This method returns the X position of the left edge of the list area after adjusting
     * for the scrollbar and border, if any. */
  int leftedge() const;	
    /** This method returns the item under mouse at my. If no item is displayed at that 
     * position then \c NULL is returned. */
  void *find_item(int my); 
    /** Draws the contents of the browser within the specified bounding box. */
  void draw(int x,int y,int width,int height);
    /** Handles an event within the specified bounding box. */
  int handle(int event,int x,int y,int width,int height);

    /** Draws the list within the normal widget bounding box. */
  void draw();
    /** The constructor makes an empty browser. */
  Fl_Browser_(int x,int y,int width,int height,const char *label = 0);

public:

  Fl_Scrollbar scrollbar;		// Vertical scrollbar
  Fl_Scrollbar hscrollbar;		// Horizontal scrollbar

    /** Handles an event within the normal widget bounding box. */
  int handle(int);
    /** Repositions and/or resizes the browser. */
  void resize(int x,int y,int width,int height);

    /** Sets the selection state of item \c p to \c s and returns 1 if the state changed or 0
     * if it did not. If docb is non-zero, select tries to call the callback function 
     * for the widget. */
  int select(void *p,int s=1,int docallbacks=0);
    /** Selects item \c p and returns 1 if the state changed or 0 if it did not. Any other 
     * items in the list are deselected. If \c docb is non-zero, select_only tries to call
     * the callback function for the widget. */
  int select_only(void *p,int docallbacks=0);
    /** Deselects all items in the list and returns 1 if the state changed or 0 if it did not.
     * If docb is non-zero, deselect tries to call the callback function for the widget. */
  int deselect(int docallbacks=0);
    /** Gets the vertical scrolling position of the list. */
  int position() const {return position_;}
    /** Gets the horizontal scrolling position of the list. */
  int hposition() const {return hposition_;}
    /** Sets the vertical scrolling position of the list. */
  void position(int); // scroll to here
    /** Sets the horizontal scrolling position of the list. */
  void hposition(int); // pan to here
    /** Displays item p, scrolling the list as necessary. */
  void display(void*); // scroll so this item is shown

    /** By default you can scroll in both directions, and the scrollbars disappear if the 
     * data will fit in the widget. has_scrollbar() changes this based on the value of h:
     * \arg 0 - No scrollbars.
     * \arg \c Fl_Browser_::HORIZONTAL - Only a horizontal scrollbar.
     * \arg \c Fl_Browser_::VERTICAL - Only a vertical scrollbar.
     * \arg \c Fl_Browser_::BOTH - The default is both scrollbars.
     * \arg \c Fl_Browser_::HORIZONTAL_ALWAYS - Horizontal scrollbar always on, vertical 
     *      always off.
     * \arg \c Fl_Browser_::VERTICAL_ALWAYS - Vertical scrollbar always on, horizontal 
     *      always off.
     * \arg \c Fl_Browser_::BOTH_ALWAYS - Both always on. */
  uchar has_scrollbar() const {return has_scrollbar_;}
  void has_scrollbar(uchar i) {has_scrollbar_ = i;}
  enum { // values for has_scrollbar()
    HORIZONTAL = 1,
    VERTICAL = 2,
    BOTH = 3,
    ALWAYS_ON = 4,
    HORIZONTAL_ALWAYS = 5,
    VERTICAL_ALWAYS = 6,
    BOTH_ALWAYS = 7
  };

    /** Gets the default text font for the lines in the browser. */
  Fl_Font textfont() const {return (Fl_Font)textfont_;}
    /** Sets the default text font to \a font */
  void textfont(uchar font) {textfont_ = font;}
    /** Gets the default text size for the lines in the browser. */
  uchar textsize() const {return textsize_;}
    /** Sets the default text size to \a size */
  void textsize(uchar size) {textsize_ = size;}
    /** Gets the default text color for the lines in the browser. */
  Fl_Color textcolor() const {return (Fl_Color)textcolor_;}
    /** Sets the default text color to \c color */
  void textcolor(unsigned color) {textcolor_ = color;}

  static void scrollbar_width(int b) {scrollbar_width_ = b;}
  static int scrollbar_width() {return scrollbar_width_;}

    /** \deprectaed This method moves the vertical scrollbar to the righthand side of the list. */
  void scrollbar_right() {scrollbar.align(FL_ALIGN_RIGHT);}
    /** \deprecated This method moves the vertical scrollbar to the lefthand side of the list. */
  void scrollbar_left() {scrollbar.align(FL_ALIGN_LEFT);}

};

#endif

//
// End of "$Id: Fl_Browser_.H,v 1.6.2.4.2.3.2.3 2003/11/11 04:13:26 easysw Exp $".
//
