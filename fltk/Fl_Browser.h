//
// "$Id: Fl_Browser.h,v 1.9 2002/10/29 00:37:22 easysw Exp $"
//
// Copyright 1998-2000 by Bill Spitzak and others.
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

#ifndef Fl_Browser_H
#define Fl_Browser_H

#include "Fl_Menu_.h"
#include "Fl_Scrollbar.h"
/**

   The Fl_Browser widget displays a scrolling vertical list of text 
   widgets, possibly with a hierachial arrangement. 

   \image browser.gif

   The items may be created as child widgets (usually the same widgets 
   as are used to create menus: Fl_Item widgets, or Fl_Item_Group 
   widgets to make a hierarchy). Alternatively you can build simple 
   text lists by using the add() method, which will create the child 
   widgets for you (and even create a hierarchy if you put slashes 
   in the text). You can also us an Fl_List which allows you to control 
   the storage by dynamically creating a "fake" widget for the browser 
   to use to draw each item. 

   All the functions used to add, remove, or modify items in the list 
   are defined by the base class Fl_Menu_. See that for much more 
   information. 

   For a simple constant list you can populate the list by calling 
   browser->add("text of item") once for each item. If you give the 
   browser a callback you can find out what item was selected with 
   browser->value();, the first item is zero (this is different from 
   older versions of fltk that started at 1!), and will be negative 
   if no item is selected. You can change the selected item with 
   browser->value(n). 

   The subclass Fl_Multi_Browser lets the user select more than one 
   item at the same time. 

   Callbacks

   You can control when callbacks are done with the when() method. 
   The following values are useful, the default value is FL_WHEN_RELEASE. 
   - FL_WHEN_NEVER          - Callback is never done. changed() can be used 
                              to see if the user has modified the browser.
   - FL_WHEN_CHANGED        - Callback is done whenever the user changes the
                              current item. If they drag the mouse through 
                              items the callback is done for each one, and 
                              the callback is done for each keystroke that 
                              changes the selected item. 
   - FL_WHEN_RELEASE        - Callback is done when the user releases the 
                              mouse and the selected item has changed, and 
                              on any keystroke that changes the item. 
   - FL_WHEN_RELEASE_ALWAYS - Callback is done when the user releases the 
                              mouse even if the current item has not changed, 
                              and on any arrow keystroke even when at the top 
                              or bottom of the browser. You must use this if 
                              you want to detect the user double-clicking an 
                              item. 
   - FL_WHEN_ENTER_KEY      - Callback is only done if the user hits the 
                              Enter key and some item is selected. In the 
                              current version the callback is also done if 
                              the user double-clicks a non-parent item. 


   This is the description of '@' commands, it should be moved to the 
   fl_draw() documentation: 

   A string of formatting codes at the start of each column are 
   stripped off and used to modify how the rest of the line is printed: 

   - @. Print rest of line, don't look for more '@' signs 
   - @@ Print rest of line starting with '@' 
   - @l Use a large (24 point) font 
   - @m Use a medium large (18 point) font 
   - @s Use a small (11 point) font 
   - @b Use a bold font (adds FL_BOLD to font) 
   - @i Use an italic font (adds FL_ITALIC to font) 
   - @f or @t Use a fixed-pitch font (sets font to FL_COURIER) 
   - @c Center the line horizontally 
   - @r Right-justify the text 
   - @B0, @B1, ... @B255 Fill the backgound with fl_color(n) 
   - @C0, @C1, ... @C255 Use fl_color(n) to draw the text 
   - @F0, @F1, ... Use fl_font(n) to draw the text 
   - @S1, @S2, ... Use point size n to draw the text 
   - @u or @_ Underline the text. 
   - @- draw an engraved line through the middle. 
   
   Notice that the @. command can be used to reliably terminate 
   the parsing. To print a random string in a random color, use 
   sprintf("@C%d@.%s", color, string) and it will work even if 
   the string starts with a digit or has the format character in it. 

*/
class FL_API Fl_Browser : public Fl_Menu_ {
  Fl_End endgroup;

public:

  int handle(int);
  void layout();
  void draw();

  /** The constructor makes an empty browser. */
  Fl_Browser(int X,int Y,int W,int H,const char*l=0);
  static Fl_Named_Style* default_style;
  /** 
   The destructor deletes all the list items (because they are 
   child Fl_Widgets of an Fl_Group) and destroys the browser. 
  */
  ~Fl_Browser();

  enum { // values for type()
    NORMAL = 0,
    MULTI = 1
  };

  /**
   The width of the longest item in the browser, measured in pixels. 
   If this is changed (by adding or deleting items or opening or 
   closing a parent item) then layout() must be called before 
   this is correct. 
  */
  int width() const {return width_;}
  /**
   The height of all the visible items in the browser, measured in 
   pixels. If this is changed (by adding or deleting items or 
   opening or closing a parent item) then layout() must be called 
   before this is correct. 
   */
  int height() const {return height_;}
  /**
   The width of the display area of the browser in pixels, this is w() 
   minus the edges of the box() minus the width of the vertical scrollbar, 
   if visible. If this is changed (by resizing the widget, adding or 
   deleting items or opening or closing a parent item such that the 
   scrollbar visibility changes) then layout() must be called before this 
   is correct. 
  */
  int box_width() const {return W;}
  /**
   The height of the display area of the browser in pixels, this is 
   h() minus the edges of the box() minus the height of the horizontal 
   scrollbar, if visible. If this is changed (by resizing the widget, 
   adding or deleting items or opening or closing a parent item such 
   that the scrollbar visibility changes) then layout() must be called 
   before this is correct. 
  */
  int box_height() const {return H;}
  /** Current horizontal scrolling pozition. Normally zero. */
  int xposition() const {return xposition_;}
  /** Set the horizontal scrolling to a certain value. */
  void xposition(int);
  /**
   Current vertical scrolling position. This is the location, 
   in pixels, of the top of the visible display, relative to 
   the pixel position of the top of the first item. 
  */
  int yposition() const {return yposition_;}
  /** Set the vertical scrolling to a certain value. */
  void yposition(int);
  /*@{*/
  /**
   Turn this on for space to be reserved for open/close boxes 
   drawn to the left of top-level items. You usually want this 
   for a hierarchial browser. This should be off for a flat browser, 
   or to emulate Windows Explorer where "my computer" does not have 
   an open/close to the left of it. The default value is false. 
  */
  bool indented() const {return indented_;}
  void indented(bool v) {indented_ = v;}
  /*@}*/
  /*@{*/
  /**
   The scrollbars are accessable as instance variables. 
   You can alter their width or other attributes to customize 
   the browser to your liking. 
  */
  Fl_Scrollbar scrollbar;
  Fl_Scrollbar hscrollbar;
  /*@}*/

  /**
  Because of the hierarchial structure it is difficult to identify an 
  item in the browser. Instead of passing an identifier to all the calls that 
  can modify an item, the browser provides several calls to set the "current" 
  item and then calls to modify the current item. 
  
  This call sets the current item to the very first visible item in the 
  browser. It returns the widget for that item, or null if the browser is 
  empty. 

  If you have invisible items in the browser you should use goto_index(0) 
  if you want to go to the first item even if it is invisible. 
  */
  Fl_Widget* goto_top();
  /**
   Set the current item to the "focus" (the item with the dotted square in 
   an Fl_Multi_Browser, and the selected item in a normal Fl_Browser. 
  */
  Fl_Widget* goto_focus() {return goto_mark(FOCUS);}
  /**
   Set the current item to one at or before y pixels 
   from top of the first item. 
  */
  Fl_Widget* goto_position(int y);
  /**
   Go to a nested item. indexes must contain level+1 index numbers. The 
   first number indicates the top-level item number, the second indicates the 
   child number of that parent, and so on. This sets the current item to the 
   given item. If the values are out of range then null is returned, 
   otherwise the widget for that item.
  */
  /*@{*/
  /**
   These are the same as passing the number of arguments minus one as the 
   level and the arguments as the index array to goto_index(). Passing a 
   negative number as the first argument will set it to a special "no item" 
   state where select_only_this() will do deselect() 
  */
  Fl_Widget* goto_index(const int* indexes, int level);
  Fl_Widget* goto_index(int);
  Fl_Widget* goto_index(int,int,int=-1,int=-1,int=-1);
  /*@}*/
  /**
   Move the current item to the next item. If if is a parent it moves to 
   the first child. If not a parent, it moves to the next child of it's 
   parent. If it is the last child it moves to the parent's brother. 
   Repeatedly calling this will visit every child of the browser. This 
   returns the widget. If the current widget is the last one this returns
   null, but the current widget remains on the last one. 
  */
  Fl_Widget* next();
  /**
   Move the current item to the next visible item. Invisible items are 
   either hidden because their parent is closed or because they had 
   hide() called on them. 
  */
  Fl_Widget* next_visible();
  /**
   Move the current item to the previous visible item. Invisible items 
   are either hidden because their parent is closed or because they 
   had hide() called on them. 
  */
  Fl_Widget* previous_visible();
  /**
   Return true if the item would be visible to the user if the 
   browser was scrolled to the correct location. This means that 
   the FL_INVISIBLE flag is not set on it, and all parents of it 
   are open and visible as well. 
  */
  bool item_is_visible() const;
  /**
   Return true if the current item is a parent. Notice that it may 
   have zero children. 
  */
  bool item_is_parent() const;

  /**
   Change the focus (the selected item, or in an Fl_Multi_Browser the 
   item that has a dotted box around it, and the current item selected 
   with goto_index(). This calls make_item_visible(). 
  */
  bool set_focus();
  bool set_item_selected(bool value = true, int do_callback = 0);
  bool select_only_this(int do_callback = 0);
  bool deselect(int do_callback = 0);
  enum linepos { NOSCROLL, TOP, MIDDLE, BOTTOM };
  /**
   This makes the current item visible to the user. 

   First it turns off the FL_INVISIBLE flag on the current item, and 
   turns off the FL_INVISIBLE flag and opens (turning on the FL_VALUE 
   flag) all parent items. These flag changes cause flags_changed() 
   to be called on any Fl_List that you have assigned to the browser. 

   The browser is then scrolled by calling yposition() so the item is 
   visible. The optional argument tells how to scroll. If not 
   specified (or the default value of Fl_Browser::NOSCROLL is given) 
   then the browser is scrolled as little as possible to show the item. 
   If it is Fl_Browser::TOP then the item is put at the top of the 
   browser. If it is Fl_Browser::MIDDLE then the item is centered 
   vertically in the browser. If it is Fl_Browser::BOTTOM then the item 
   is put at the bottom of the browser. 

   This does nothing if the current item is null. 
  */
  bool make_item_visible(linepos = NOSCROLL);
  /**
   Tell the browser to redraw the current item. Do this if you know it 
   has changed appearance. This is better than redrawing the entire 
   browser because it will blink a lot less.
  */
  void damage_item() {damage_item(HERE);}
  /**
   If the current item is a parent, set the open state (the FL_VALUE flags) 
   to the given value and redraw the browser correctly. Returns true if 
   the state was actually changed, returns false if it was already in 
   that state. 
  */
  bool set_item_opened(bool);
  /**
   Turn off or on the FL_INVISIBLE flag on the given item and redraw 
   the browser if necessary. Returns true if the state was actually 
   changed, returns false if it was already in that state. 
  */
  bool set_item_visible(bool);
  
  /**
   Return the nesting level of the current item (how many parents it has).
  */
  int current_level() const {return item_level[HERE];}
  /**
   Return an array of current_level()+1 indexes saying which child at each 
   level includes the current item. 
  */
  const int* current_index() const {return item_index[HERE];}
  /**
   Return the y position, in pixels, of the top edge of the current item. 
   You may also want the height, which is in item()->height(). 
  */
  int current_position() const {return item_position[HERE];}

  /**
   Return the nesting level of the focus (how many parents it has). 
   The focus is the selected item the user sees. 
  */
  int focus_level() const {return item_level[FOCUS];}
  /**
   Return an array of focus_level()+1 indexes saying which child at each 
   level includes the focus. 
  */
  const int* focus_index() const {return item_index[FOCUS];}
  /**
   Return the y position, in pixels, of the top edge of the focus item. 
  */
  int focus_position() const {return item_position[FOCUS];}
  /**
   Same as goto_index(v); set_focus();, to change the current item in 
   a non-hierarchial browser. 
  */
  void value(int v) {goto_index(v); set_focus();}
  /**
   Returns focus_index(v)[0], to get the current item in a 
   non-hierarchial browser
  */
  int value() const {return focus_index()[0];}

  // Maybe these should be moved to base Fl_Menu_ class:
  /*@{*/
  /**
   Sent to the fl_draw() function to control the interpretation of 
   leading symbol characters. This defaults to '@'. 
  */
  char format_char() const {return format_char_;}
  void format_char(char c) {format_char_ = c;}
  /*@}*/
  /*@{*/
  /**
   Sets the horizontal locations that each '\t' character in an item 
   should start printing text at. These are measured from the left edge 
   of the browser, including any area for the open/close + glyphs. 
  */
  const int *column_widths() const { return column_widths_; }
  void column_widths(const int *pWidths)  { column_widths_ = pWidths; }
  /*@}*/

  // Convienence functions for flat browsers:
  bool select(int line, bool value = true);
  bool selected(int line);
  /**
   Return the index of the top-level item that is at the top of the browser. 
  */
  int topline() const {return item_index[FIRST_VISIBLE][0];}
  /**
   Same as goto_index(line); make_item_visible(TOP);. 
  */
  void topline(int line) {goto_index(line); make_item_visible(TOP);}
  void bottomline(int line) {goto_index(line); make_item_visible(BOTTOM);}
  void middleline(int line) {goto_index(line); make_item_visible();}
  bool displayed(int line);
  bool display(int line, bool value = true);

private:

  bool indented_;
  char format_char_;
  const int *column_widths_;
  int xposition_, yposition_;
  int width_, height_;
  int scrolldx, scrolldy;
  static void hscrollbar_cb(Fl_Widget*, void*);
  static void scrollbar_cb(Fl_Widget*, void*);
  void draw_item();
  void draw_clip(int,int,int,int);
  static void draw_clip_cb(void*,int,int,int,int);
  int X,Y,W,H; // bounding box area

  int multi() const {return type()&MULTI;}

  // Marks serve as "indexes" into the hierarchial browser. We probably
  // need to make some sort of public interface but I have not figured
  // it out completely.
  enum { // predefined marks
    HERE = 0, // current item, the one moved by all the calls
    FOCUS,
    FIRST_VISIBLE,
    REDRAW_0,
    REDRAW_1,
    TEMP,
    NUMMARKS
  };
  Fl_Widget* goto_mark(int mark); // set HERE to mark
  Fl_Widget* goto_visible_focus(); // set HERE to focus if visible
  void set_mark(int dest, int mark); // set dest to mark
  int compare_marks(int mark1, int mark2); // returns >0 if mark1 after mark2
  bool at_mark(int mark) { return !compare_marks(HERE,mark); }
  void unset_mark(int mark);  // makes mark have illegal value
  bool is_set(int mark);  // false if unset_mark was called
  void damage_item(int mark); // make this item redraw
  int siblings; // # of children of parent of HERE item

  // For each mark:
  unsigned char item_level[NUMMARKS]; // depth in hierarchy of the item
  unsigned char open_level[NUMMARKS]; // depth of highest closed parent
  int item_position[NUMMARKS]; // distance in pixels from top of browser
  int* item_index[NUMMARKS]; // indexes at each level of hierarchy
  int levels; // maximum depth of all items encountered so far
  void set_level(int); // increases levels by reallocating the arrays

};

#ifndef FLTK_2
#define FL_NORMAL_BROWSER Fl_Browser::NORMAL
#define FL_MULTI_BROWSER  Fl_Browser::MULTI
#define FL_SELECT_BROWSER Fl_Browser::NORMAL
#define FL_HOLD_BROWSER   Fl_Browser::NORMAL
#endif

#endif

//
// End of "$Id: Fl_Browser.h,v 1.9 2002/10/29 00:37:22 easysw Exp $".
//
