//
// "$Id$"
//
// Common browser header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/* \file
 fltk3::Browser_ widget . */

#ifndef Fltk3_Browser__H
#define Fltk3_Browser__H

#include "Group.h"
#include "Scrollbar.h"
#include <fltk3/run.h>		// fltk3::scrollbar_size()


namespace fltk3 {
  
  const uchar NORMAL_BROWSER	= 0;	/**< type() of fltk3::Browser */
  const uchar SELECT_BROWSER	= 1;	/**< type() of fltk3::SelectBrowser */
  const uchar HOLD_BROWSER	= 2;	/**< type() of fltk3::HoldBrowser */
  const uchar MULTI_BROWSER	= 3;	/**< type() of fltk3::MultiBrowser */
  
  const uchar SORT_ASCENDING	= 0;	/**< sort browser items in ascending alphabetic order. */
  const uchar SORT_DESCENDING	= 1;	/**< sort in descending order */
  
  /**
   \brief This is the base class for browsers.  
   
   To be useful it must be subclassed and several virtual functions defined. 
   The Forms-compatible browser and the file chooser's browser are subclassed 
   off of this.
   
   This has been designed so that the subclass has complete control
   over the storage of the data, although because next() and 
   prev() functions are used to index, it works best as a linked list
   or as a large block of characters in which the line breaks must be
   searched for.
   
   A great deal of work has been done so that the "height" of a data
   object does not need to be determined until it is drawn.  This is
   useful if actually figuring out the size of an object requires
   accessing image data or doing stat() on a file or doing some
   other slow operation.
   */
  class FLTK3_EXPORT Browser_ : public fltk3::Group {

    int position_;            // where user wants it scrolled to
    int real_position_;       // the current vertical scrolling position
    int hposition_;           // where user wants it panned to
    int real_hposition_;      // the current horizontal scrolling position
    int offset_;              // how far down top_ item the real_position is
    int max_width;            // widest object seen so far
    uchar has_scrollbar_;     // which scrollbars are enabled
    void* top_;               // which item scrolling position is in
    void* selection_;         // which is selected (except for fltk3::MULTI_BROWSER)
    void *redraw1,*redraw2;   // minimal update pointers
    void* max_width_item;     // which item has max_width_
    int scrollbar_size_;      // size of scrollbar trough
    
    // Figure out top() based on position():
    void update_top();
    
  protected:
    
    /**
     \brief Return the first item in the list.
     This method must be provided by the subclass 
     to return the first item in the list.
     \see item_first(), item_next(), item_last(), item_prev()
     \note This method must be implemented by any derived browser class.
     */
    virtual void *item_first() const;
    
    /**
     \brief Return the next item in the list.
     This method must be provided by the subclass
     to return the item in the list after \p item.
     \see item_first(), item_next(), item_last(), item_prev()
     \note This method must be implemented by any derived browser class.
     */
    virtual void *item_next(void *item) const;
    
    /**
     \brief Return the previous item in the list.
     This method must be provided by the subclass
     to return the item in the list before \p item.
     \see item_first(), item_next(), item_last(), item_prev()
     \note This method must be implemented by any derived browser class.
     */
    virtual void *item_prev(void *item) const;
    
    /**
     \brief Return the last item in the list.
     This method must be provided by the subclass
     to return the last item in the list.
     \see item_first(), item_next(), item_last(), item_prev()
     \note This method must be implemented by any derived browser class.
     */
    virtual void *item_last() const;
    
    /** 
     \brief Return the height of an item.
     This method must be provided by the subclass to return 
     the height of \p item in pixels.
     Allow for two additional pixels for the list selection box.
     \param[in] item The item whose height is returned.
     \returns The height of the specified \p item in pixels.
     \see item_height(), item_width(), item_quick_height()
     \note This method must be implemented by any derived browser class.
     */
    virtual int item_height(void *item) const;
    
    /**
     \brief Return the width of an item.
     This method must be provided by the subclass to return the width of the
     \p item in pixels.  Allow for two additional pixels for the list
     selection box.
     \param[in] item The item whose width is returned.
     \returns The width of the item in pixels.
     \note This method must be implemented by any derived browser class.
     */
    virtual int item_width(void *item) const;
    
    /**
     \brief Quickly return some estimated height of the item.
     This method may be provided by the subclass to return the height of the
     \p item, in pixels.
     Allow for two additional pixels for the list selection box.
     This method differs from item_height in that it is only called for
     selection and scrolling operations.
     The default implementation calls item_height.
     \param[in] item The item whose height to return.
     \returns The height, in pixels.
     \note This method must be implemented by any derived browser class.
     */
    virtual int item_quick_height(void *item) const ;
    
    /**
     \brief Draw an item.
     This method must be provided by the subclass to draw the \p item
     in the area indicated by \p X, \p Y, \p W, \p H.
     \note This method must be implemented by any derived browser class.
     */
    virtual void item_draw(void *item,int X,int Y,int W,int H) const;
    
    /**
     \brief Return a label text for sorting.
     This optional method returns a string (label) that may be used for sorting. 
     \param[in] item The item whose label text is returned.
     \returns The item's text label. (Can be NULL if blank)
     \note Implementation of this method is optional.
     */
    virtual const char *item_text(void *item) const;
    
    /**
     \brief Swap to items.
     This optional method should be provided by the subclass 
     to efficiently swap browser items \p a and \p b, such as for sorting.
     \param[in] a,b The two items to be swapped.
     \note Implementation of this method is optional.
     */
    virtual void item_swap(void *a,void *b);
    
    /**
     \brief Return an item at an index.
     This method must be provided by the subclass 
     to return the item for the specified \p index. 
     \param[in] index The \p index of the item to be returned
     \returns The item at the specified \p index.
     \note This method must be implemented by any derived browser class.
     */
    virtual void *item_at(int index) const;
    
    /**
     \brief Current width of all items.
     This method may be provided by the subclass to indicate the full width
     of the item list, in pixels.
     The default implementation computes the full width from the item widths.
     \returns The maximum width of all the items, in pixels.
     \note Implementation of this method is optional, but may speed up calculations.
     */
    virtual int full_width() const ;
    
    /**
     \brief Current height of all items.
     This method may be provided by the subclass to indicate the full height
     of the item list, in pixels.
     The default implementation computes the full height from the item heights.
     Includes the items that are scrolled off screen.
     \returns The height of the entire list, in pixels.
     \note Implementation of this method is optional, but may speed up calculations.
     */
    virtual int full_height() const ;
    
    /**
     \brief average height of an item.
     This method may be provided to return the average height of all items
     to be used for scrolling.
     The default implementation uses the height of the first item.
     \returns The average height of items, in pixels.
     \note Implementation of this method is optional, but may speed up calculations.
     */
    virtual int incr_height() const ;

    /**
     \brief Mark an item selected.
     This method must be implemented by the subclass if it supports
     multiple selections; sets the selection state to \p val for the \p item.
     Sets the selection state for \p item, where optional \p val is 1 (select, the default)
     or 0 (de-select).
     \param[in] item The item to be selected
     \param[in] val The optional selection state; 1=select, 0=de-select.\n
     The default is to select the item (1).
     \note These only need to be implemented by subclass if you want a multi-browser.
     */
    virtual void item_select(void *item,int val=1);

    /**
     \brief Return if an item is selected.
     This method must be implemented by the subclass if it supports
     multiple selections; returns the selection state for \p item.
     The method should return 1 if \p item is selected, or 0 otherwise.
     \param[in] item The item to test.
     \note These only need to be implemented by subclass if you want a multi-browser.
     */
    virtual int item_selected(void *item) const ;
    
    /**
     \brief Returns the item that appears at the top of the list.
     */
    void *top() const { return top_; }
    
    /**
     \brief Returns the item currently selected, or NULL if there is no selection.
     
     For multiple selection browsers this call returns the currently focused item,
     even if it is not selected. To find all selected items, call 
     fltk3::MultiBrowser::selected() for every item in question.
     */
    void *selection() const { return selection_; }
    
    /**
     \brief Quick way to delete and reset everything.
     This method should be called when the list data is completely replaced
     or cleared. It informs the fltk3::Browser_ widget that any cached
     information it has concerning the items is invalid.
     This method does not clear the list, it just handles the follow up
     bookkeeping after the list has been cleared.
     */
    void new_list();
    
    /**
     \brief Call this before deleting an item.
     Tell it that this item is going away, and that this must remove
     all pointers to it.
     
     This method should be used when \p item is being deleted from the list.
     It allows the fltk3::Browser_ to discard any cached data it has on the item.
     This method does not actually delete the item, but handles the follow up
     bookkeeping after the item has just been deleted.
     \param[in] item The item being deleted.
     */
    void deleting(void *item);
    
    /**
     \brief Call this before replacing one item with another.
     This method should be used when item \p a is being replaced by item \p b.
     It allows the fltk3::Browser_ to update its cache data as needed,
     schedules a redraw for the item being changed, and tries to maintain the selection.
     This method does not actually replace the item, but handles the follow up
     bookkeeping after the item has just been replaced.
     \param[in] a Item being replaced
     \param[in] b Item to replace 'a'
     */
    void replacing(void *a,void *b);
    
    /**
     \brief Call this before swapping two items.
     This method should be used when two items \p a and \p b are being swapped.
     It allows the fltk3::Browser_ to update its cache data as needed,
     schedules a redraw for the two items, and tries to maintain the current selection.
     This method does not actually swap items, but handles the follow up
     bookkeeping after items have been swapped.
     \param[in] a,b Items being swapped.
     */
    void swapping(void *a,void *b);
    
    /**
     \brief Call this before inserting an item.
     This method should be used when an item is in the process of
     being inserted into the list.
     It allows the fltk3::Browser_ to update its cache data as needed,
     scheduling a redraw for the affected lines.
     This method does not actually insert items, but handles the
     follow up bookkeeping after items have been inserted.
     \param[in] a The starting item position
     \param[in] b The new item being inserted
     */
    void inserting(void *a,void *b);
    
    /**
     \brief Tell whether item is currently displayed.
     Returns non-zero if \p item has been scrolled to a position where it is being displayed.
     Checks to see if the item's vertical position is within the top and bottom
     edges of the display window. This does NOT take into account the hide()/show()
     status of the widget or item.
     \param[in] item The item to check
     \returns 1 if visible, 0 if not visible.
     \see display(), displayed()
     */
    int displayed(void *item) const ;
    
    /**
     \brief Cause minimal update to redraw the given item.
     This method should be called when the contents of \p item has changed,
     but not its height.
     \param[in] item The item that needs to be redrawn.
     \see redraw_lines(), redraw_line()
     */
    void redraw_line(void *item);
    
    /**
     This method will cause the entire list to be redrawn.
     \see redraw_lines(), redraw_line()
     */
    void redraw_lines() { damage(fltk3::DAMAGE_SCROLL); }
    
    /**
     \brief Returns the drawing area of the widget.
     Returns the bounding box for the interior of the list's display window, 
     inside the scrollbars.
     \param[out] X,Y,W,H The returned bounding box.
                  (The original contents of these parameters are overwritten)
     */
    void bbox(int &X,int &Y,int &W,int &H) const;
    
    /**
     \brief Returns the left edge of the item list.
     This method returns the X position of the left edge of the list area
     after adjusting for the scrollbar and border, if any.
     \returns The X position of the left edge of the list, in pixels.
     \see fltk3::Browser_::bbox()
     */
    int leftedge() const;
    
    /**
     \brief This method returns the item under mouse y position \p ypos.
     NULL is returned if no item is displayed at that position.
     \param[in] ypos The y position (eg. fltk3::event_y()) to find an item under.
     \returns The item, or NULL if not found
     */
    void *find_item(int ypos); // item under mouse
    
    /** \internal
     \brief Draws the list within the normal widget bounding box.
     Draw() and hence redraw() has side effect of updating top and setting 
     scrollbar positions!
     */
    void draw();
    
    /** \internal
     The constructor makes an empty browser.
     \param[in] X,Y,W,H position and size.
     \param[in] L The label string, may be NULL.
     */
    Browser_(int X,int Y,int W,int H,const char *L=0);
    
  public:
    
    /**
     Vertical scrollbar. Public, so that it can be accessed directly.
     */
    fltk3::Scrollbar scrollbar;
    /**
     Horizontal scrollbar. Public, so that it can be accessed directly.
     */
    fltk3::Scrollbar hscrollbar;
    
    /**
     Handles the \p event within the normal widget bounding box.
     \param[in] event The event to process.
     \returns 1 if event was processed, 0 if not.
     */
    int handle(int event);

    /**
     \brief Repositions and/or resizes the browser.
     The scrollbars may be moved again by draw(), since each one's size
     depends on whether the other is visible or not.  This skips over
     fltk3::Group::resize since it moves the scrollbars uselessly.
     \param[in] X,Y,W,H The new position and size for the browser, in pixels.
     */
    void resize(int X,int Y,int W,int H);
    
    /**
     \brief Select or deselect an item.
     Sets the selection state of \p item to \p val,
     and returns 1 if the state changed or 0 if it did not.
     
     If \p docallbacks is non-zero, select tries to call
     the callback function for the widget.
     
     \param[in] item The item whose selection state is to be changed
     \param[in] val The new selection state (1=select, 0=de-select)
     \param[in] docallbacks If 1, invokes widget callback if item changed.\n
     If 0, doesn't do callback (default).
     \returns 1 if state was changed, 0 if not.
     */
    int select(void *item,int val=1,int docallbacks=0);

    /**
     \brief Select a single item.
     Selects \p item and returns 1 if the state changed or 0 if it did not.
     Any other items in the list are deselected.
     \param[in] item The \p item to select.
     \param[in] docallbacks If 1, invokes widget callback if item changed.\n
     If 0, doesn't do callback (default).
     */
    int select_only(void *item,int docallbacks=0);
    
    /**
     \brief Deselect all items.
     Deselects all items in the list and returns 1 if the state changed
     or 0 if it did not.
     
     If the optional \p docallbacks parameter is non-zero, deselect tries
     to call the callback function for the widget.
     
     \param[in] docallbacks If 1, invokes widget callback if item changed.\n
     If 0, doesn't do callback (default).
     */
    int deselect(int docallbacks=0);
    
    /**
     Gets the vertical scroll position of the list as a pixel position \p pos.
     The position returned is how many pixels of the list are scrolled off the top edge
     of the screen.  Example: A position of '3' indicates the top 3 pixels of 
     the list are scrolled off the top edge of the screen.
     \see position(), hposition()
     */
    int position() const { return position_; }

    /**
     \brief Sets the vertical scroll position.
     Sets the vertical scroll position of the list to pixel position \p pos.
     The position is how many pixels of the list are scrolled off the top edge
     of the screen. Example: A position of '3' scrolls the top three pixels of
     the list off the top edge of the screen.
     
     Change position(), top() will update when update_top() is called
     (probably by draw() or handle()):
     
     \param[in] pos The vertical position (in pixels) to scroll the browser to.
     \see position(), hposition()
     */
    void position(int pos); // scroll to here
    
    /**
     Gets the horizontal scroll position of the list as a pixel position \p pos.
     The position returned is how many pixels of the list are scrolled off the left edge
     of the screen. Example: A position of '18' indicates the left 18 pixels of
     the list are scrolled off the left edge of the screen.
     \see position(), hposition()
     */
    int hposition() const { return hposition_; }

    /**
     Sets the horizontal scroll position of the list to pixel position \p pos.
     The position is how many pixels of the list are scrolled off the left edge
     of the screen. Example: A position of '18' scrolls the left 18 pixels of the list
     off the left edge of the screen.
     \param[in] pos The horizontal position (in pixels) to scroll the browser to.
     \see position(), hposition()
     */
    void hposition(int pos); // pan to here
    
    /**
     \brief Ensure this item is displayed.
     Displays the \p item, scrolling the list as necessary.
     \param[in] item The item to be displayed.
     \see display(), displayed()
     */
    void display(void *item);
    
    /**
     \brief Values for has_scrollbar().
     Anonymous enum bit flags for has_scrollbar().
     -  bit 0: horizontal
     -  bit 1: vertical
     -  bit 2: 'always' (to be combined with bits 0 and 1)
     -  bit 3-31: reserved for future use
     */
    enum { // values for has_scrollbar()
      HORIZONTAL = 1,		///< Only show horizontal scrollbar.
      VERTICAL = 2,		///< Only show vertical scrollbar.
      BOTH = 3,			///< Show both scrollbars. (default)
      ALWAYS_ON = 4,		///< Specified scrollbar(s) should 'always' be shown (to be used with HORIZONTAL/VERTICAL)
      HORIZONTAL_ALWAYS = 5,	///< Horizontal scrollbar always on.
      VERTICAL_ALWAYS = 6,	///< Vertical scrollbar always on.
      BOTH_ALWAYS = 7		///< Both scrollbars always on.
    };
    
    /**
     Returns the current scrollbar mode, see fltk3::Browser_::has_scrollbar(uchar)
     */
    uchar has_scrollbar() const { return has_scrollbar_; }
    
    /**
     Sets whether the widget should have scrollbars or not (default fltk3::Browser_::BOTH).
     By default you can scroll in both directions, and the scrollbars
     disappear if the data will fit in the widget.  
     has_scrollbar() changes this based on the value of \p mode:
     
     - 0 - No scrollbars.
     
     - fltk3::Browser_::HORIZONTAL - Only a horizontal scrollbar.
     
     - fltk3::Browser_::VERTICAL - Only a vertical scrollbar.
     
     - fltk3::Browser_::BOTH - The default is both scrollbars.
     
     - fltk3::Browser_::HORIZONTAL_ALWAYS - Horizontal scrollbar always on,
     vertical always off.
     
     - fltk3::Browser_::VERTICAL_ALWAYS - Vertical scrollbar always on,
     horizontal always off.
     
     - fltk3::Browser_::BOTH_ALWAYS - Both always on.
     */
    void has_scrollbar(uchar mode) { has_scrollbar_ = mode; }
    
    /**
     Gets the current size of the scrollbars' troughs, in pixels.
     
     If this value is zero (default), this widget will use the 
     fltk3::scrollbar_size() value as the scrollbar's width.
     
     \returns Scrollbar size in pixels, or 0 if the global fltk3::scrollsize() is being used.
     \see fltk3::scrollbar_size(int)
     */
    int scrollbar_size() const {
      return(scrollbar_size_);
    }
    
    /**
     Sets the pixel size of the scrollbars' troughs to the \p size, in pixels.
     
     Normally you should not need this method, and should use
     fltk3::scrollbar_size(int) instead to manage the size of ALL 
     your widgets' scrollbars. This ensures your application 
     has a consistent UI, is the default behavior, and is normally
     what you want.
     
     Only use THIS method if you really need to override the global
     scrollbar size. The need for this should be rare.
     
     Setting \p size to the special value of 0 causes the widget to
     track the global fltk3::scrollbar_size(), which is the default.
     
     \param[in] size Sets the scrollbar size in pixels.\n
     If 0 (default), scrollbar size tracks the global fltk3::scrollbar_size()
     \see fltk3::scrollbar_size()
     */
    void scrollbar_size(int size) {
      scrollbar_size_ = size;
    }
    
    /**
     This method has been deprecated, existing for backwards compatibility only.
     Use scrollbar_size() instead.
     This method always returns the global value fltk3::scrollbar_size().
     \returns Always returns the global value fltk3::scrollbar_size().
     \todo This method should eventually be removed in 1.4+
     */
    int scrollbar_width() const {
      return(fltk3::scrollbar_size());
    }
    
    /**
     This method has been deprecated, existing for backwards compatibility only.
     Use scrollbar_size(int) instead.
     This method sets the global fltk3::scrollbar_size(), and forces this
     instance of the widget to use it.
     \todo This method should eventually be removed in 1.4+
     */
    void scrollbar_width(int width) {
      fltk3::scrollbar_size(width);
      scrollbar_size_ = 0;
    }
    
    /**
     Moves the vertical scrollbar to the righthand side of the list.
     For back compatibility.
     */
    void scrollbar_right() { scrollbar.align(fltk3::ALIGN_RIGHT); }
    
    /**
     Moves the vertical scrollbar to the lefthand side of the list.
     For back compatibility.
     */
    void scrollbar_left() { scrollbar.align(fltk3::ALIGN_LEFT); }
    
    /**
     Sort the items in the browser based on \p flags.
     item_swap(void*, void*) and item_text(void*) must be implemented for this call.
     \param[in] flags fltk3::SORT_ASCENDING -- sort in ascending order\n
     fltk3::SORT_DESCENDING -- sort in descending order\n
     Values other than the above will cause undefined behavior\n
     Other flags may appear in the future.
     \todo Add a flag to ignore case
     */
    void sort(int flags=0);
  };
  
}

#endif

//
// End of "$Id$".
//
