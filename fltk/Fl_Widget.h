//
// "$Id: Fl_Widget.h,v 1.16 2002/10/29 00:37:23 easysw Exp $"
//
// Widget header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Widget_H
#define Fl_Widget_H

#include "Fl_Style.h"

class FL_API Fl_Widget;
class FL_API Fl_Window;
class FL_API Fl_Image;
class FL_API Fl_Group;

typedef void (Fl_Callback )(Fl_Widget*, void*);
typedef Fl_Callback* Fl_Callback_p; // needed for BORLAND
typedef void (Fl_Callback0)(Fl_Widget*);
typedef void (Fl_Callback1)(Fl_Widget*, long);

/**
   Fl_Widget is the base class for all widgets in FLTK. You can't 
   create one of these because the constructor is not public. 
   However you can subclass it.

   Unless otherwise noted, the property setting methods such 
   as color(n) or label(s) do not call redraw(), even if they 
   change the widget's appearance. This is to make the setting 
   of several properties faster and to allow most of them to be 
   small inline functions. It is up to the calling program to call 
   redraw() after changing any visible properties. 
*/
class FL_API Fl_Widget {
  // disable the copy assignment/constructors:
  Fl_Widget & operator=(const Fl_Widget &);
  Fl_Widget(const Fl_Widget &);

public:

  /**
   Draw the widget. The default version draws the box to fill the widget
   and draws any inside label.
  */
  virtual void draw();
  /**
   Handle an event, and return non-zero if the widget understood and used 
   the event. The default version returns true for FL_ENTER, FL_LEAVE and 
   FL_MOVE. Information on how to write your own version is here. If you 
   want to send an event to a widget you probably want to use send() which
   will do some extra processing before and after the event is handled. 
  */
  virtual int handle(int event);
  /**
   Call the handle() method on the widget and return the result, but also 
   does some processing of events: 
   - It makes sure the widget is active and/or visible if the event 
     requres this. 
   - It adjusts Fl::event_x/y to be relative to the widget (It is the 
     caller's responsibility to see if the mouse is pointing at the widget). 
   - If this is not the Fl::belowmouse() widget then it changes FL_MOVE 
     into FL_ENTER and turns FL_DND_DRAG into FL_DND_ENTER. If this is 
     the Fl::belowmouse() widget then the opposite conversion is done. 
   - For move, focus, and push events if handle() returns true it sets the
     Fl::belowmouse() or Fl::focus() or Fl::pushed() widget to reflect this. 
  */
  int	send(int event);
  /**
   Respond to a change in size or position. Calling relayout() or 
   resize() with a different size will cause this to be called later
   (after all pending events are handled and just before draw() is called). 
  */
  virtual void layout();
  /**
   The destructor is virtual. The base class removes itself from the 
   parent widget (if any), and destroys any label made with copy_label(). 
  */
  virtual ~Fl_Widget();

  /**
   Return the Fl_Style structure used by this widget. Normally this structure 
   is shared with many other widgets. 
  */
  const Fl_Style* style() const { return style_; }
  /*@{*/
  /**
   Set the style to this shared style. If this style has never been used 
   before fltk adds it as a child of the current style and returns true 
   (this allows the calling code to initialize it). 
   */
  void	style(const Fl_Style* s) { style_ = s; }
  void	style(const Fl_Style& s) { style_ = &s; }
  /*@}*/
  /**
   Copy the style from another widget. If it is not shared then a copy is made,
   otherwise the pointer is just copied. This is done so that either widget
   may be destroyed without deleting the style used by the other one. 
  */
  bool	copy_style(const Fl_Style* s);
  static Fl_Named_Style* default_style;
  static void default_glyph(const Fl_Widget*,int,int,int,int,int,Fl_Flags);

  /**
   Returns a pointer to the parent widget. Usually this is a Fl_Group 
   or Fl_Window. Returns NULL if none.
   */
  Fl_Group* parent() const	{ return parent_; }
  void	parent(Fl_Group* w)	{ parent_ = w; }
  /**
   Return a pointer to the Fl_Window that this widget is in 
   (it will skip any and all parent widgets between this and the window).
   Returns NULL if none. 

   Note: for an Fl_Window, this returns the parent 
   window (if any), not this window. 
  */
  Fl_Window* window() const	;

  enum { // values for type():
    RESERVED_TYPE	= 0x64,
    GROUP_TYPE		= 0xe0,
    WINDOW_TYPE		= 0xf0
  };
  /*@{*/
  /**
   The property Fl_Widget::type() can return an arbitrary 8-bit identifier, 
   and can be set with the protected method type(uchar t). This value had 
   to be provided for Forms compatibility, but you can use it for any 
   purpose you want (mostly for "bad object oriented programming" where 
   you insert some subclass functionality into the base class). Fl_Widget
   subclasses may store values in the range 0-99 here (larger values are 
   reserved for use by FLTK). 

   For portability FLTK does not use RTTI (Run Time Typing Infomation) 
   internally (you are free to use it, though). If you don't have RTTI you 
   can use the clumsy FLTK mechanisim, by having type() use a unique value. 
   These unique values must be greater than the symbol 
   Fl_Widget::RESERVED_TYPE (which is 100). Look through the header files 
   for Fl_Widget::RESERVED_TYPE to find an unused number. If you make a 
   subclass of Fl_Window you must use Fl_Widget::WINDOW_TYPE + n (n must 
   be in the range 1 to 7), if you make a subclass of Fl_Group you must use 
   Fl_Widget::GROUP_TYPE + n (n must be in the range 1 to 7) (fltk needs to 
   be able to identify Fl_Window and Fl_Group subclasses internally). 
  */
  uchar	type() const		{ return type_; }
  void	type(uchar t)		{ type_ = t; }
  /*@}*/
  /**
   Returns true if this Fl_Widget's class is a subclass of Fl_Group. 
   If so you can cast it to a group with (Fl_Group*)(widget). This is
   done by using type(), but if your compiler supports RTTI you may want
   to safer dynamic_cast<Fl_Group*>(widget). 
  */
  bool	is_group() const	{ return type_ >= GROUP_TYPE; }
  /**
   Returns true if this Fl_Widget's class is a subclass of Fl_Window.
   If so you can cast it to a window with (Fl_Window*)(widget). This
   is done by using type(), but if your compiler supports RTTI you may
   want to safer dynamic_cast<Fl_Window*>(widget). If this is true, 
   is_group() is also true.
  */
  bool	is_window() const	{ return type_ >= WINDOW_TYPE; }

  /*@{*/
  /**
   The position of the upper-left corner of the widget in its enclosing
   Fl_Widget. If this is an outer Fl_Window than this is the position 
   of the upper-left corner of the contents (not the frame) on the screen. 
  */
  int	x() const		{ return x_; }
  void	x(int v)		{ x_ = v; }
  int	y() const		{ return y_; }
  void	y(int v)		{ y_ = v; }
  int	w() const		{ return w_; }
  void	w(int v)		{ w_ = v; }
  int	h() const		{ return h_; }
  void	h(int v)		{ h_ = v; }
  /*@}*/
  /*@{*/
  /**
   Returns w() or h(), but if the current value is zero it calls 
   layout() before returning the value. Using these calls allows
   a widget to delay the calculation of size until it is needed. 
  */
  int	width()			;
  int	height()		;
  /*@}*/
  /**
   Change the size or position of the widget. Nothing is done if
   the passed size and position are the same as before. If there 
   is a change then relayout() is called so that the virtual 
   function layout() is called before the next draw(). 
  */
  bool	resize(int x,int y,int w,int h)	;
  /** Inline equivalent to resize(x,y,w(),h()). */
  bool	position(int X,int Y)	{ return resize(X,Y,w_,h_); }
  /** Inline equivalent to resize(x(),y(),w,h). */
  bool	size(int W,int H)	{ return resize(x_,y_,W,H); }

  /*@{*/
  /**
   The label is printed somewhere on the widget or next to it.
   The string passed to label() is not copied, instead the pointer
   to the string is stored. If copy_label() was called earlier 
   the old string's memory is freed.
  */
  const char* label() const	{ return label_; }
  void	label(const char* a);
  /*@}*/
  /**
   The passed string is copied to private storage and used to set the
   label. The memory will be freed when the widget is destroyed or when
   copy_label() is called again, or label(const char*) is called. 

   Passing NULL will set label() to NULL.
  */
  void	copy_label(const char* a);

  /*@{*/
  /** Fl_Image to draw as part of the label.*/
  Fl_Image* image() const	{ return image_; }
  void	image(Fl_Image* a)	{ image_ = a; }
  void	image(Fl_Image& a)	{ image_ = &a; }
  /*@}*/

  /*@{*/
  /**
   Set the string used as the pop-up tooltip. The pointer to the passed 
   string is stored, it is not copied! Passing null indicates that the 
   tooltip of the parent() should be used (or no tooltip if no parent has one).
   If you want to disable the tooltip but let the parent have one, set this 
   tooltip to "". 

   This is used as the label of the Fl_Tooltip widget that pops up when the 
   user points at the window. You can probably get some interesting formatting
   and symbols in there with the symbol drawing code. 
  */
  const char *tooltip() const	{ return tooltip_; }
  void	tooltip(const char *t)	{ tooltip_ = t; }
  /*@}*/

  /*@{*/
  /**
   Buttons and menu items use the shortcut to identify a keystroke that will 
   activate them. The value is a bitwise OR of a key and a set of shift flags, 
   for example FL_ALT | 'a' , FL_ALT | (FL_F + 10), or just 'a'. A value of 0 
   disables the shortcut. 

   The key can be any value returned by Fl::event_key(), but will usually be 
   an ASCII letter. Use a lower-case letter unless you require the shift key 
   to be held down. 

   The shift flags can be any set of values accepted by Fl::event_state().
   If the bit is on that shift key must be pushed. Win, Alt, Ctrl, and Shift 
   must be off if they are not in the shift flags (zero for the other bits 
   indicates a "don't care" setting). 

   Shortcuts can also be done in the MS Windows way by putting an '&' in front
   of a letter in the label(). This is equivalent to FL_ALT and the letter. 
  */
  int	shortcut() const	{ return shortcut_; }
  void	shortcut(int s)		{ shortcut_ = s; }
  /*@}*/

  /*@{*/
  /**
   Each widget has a single callback. You can set it or examine 
   it with these methods.
  */
  Fl_Callback_p callback() const { return callback_; }
  void	callback(Fl_Callback* c, void* p) { callback_=c; user_data_=p; }
  void	callback(Fl_Callback* c) { callback_=c; }
  void	callback(Fl_Callback0*c) { callback_=(Fl_Callback*)c; }
  void	callback(Fl_Callback1*c, long p=0) { callback_=(Fl_Callback*)c; user_data_=(void*)p; }
  /*@}*/
  /*@{*/
  /**
   You can also just change the void* second argument to the
   callback with the user_data methods. 
  */
  void*	user_data() const	{ return user_data_; }
  void	user_data(void* v)	{ user_data_ = v; }
  /*@}*/
  /*@{*/
  /**
   For convenience you can also define the callback as taking a long
   integer argument. This is implemented by casting the function to a 
   Fl_Callback and casting the long to a void * and may not be portable 
   to some machines. 
  */
  long	argument() const	{ return (long)user_data_; }
  void	argument(long v)	{ user_data_ = (void*)v; }
  /*@}*/
  /*@{*/
  /**
   Fl_Widget::when() is a set of bitflags used by subclasses of Fl_Widget 
   to decide when to do the callback. This field is in the base class so 
   that you can scan a panel and do_callback() on all the ones that don't 
   do their own callbacks in response to an "OK" button. 

   The following constants can be used, their exact meaning depends on the
   widget's implementation: 

   - FL_WHEN_NEVER   - Never call the callback (0). 
   - FL_WHEN_CHANGED - 
      Do the callback each time the widget's value is changed by the user 
      (many callbacks may be done as the user drags the mouse) 
   - FL_WHEN_RELEASE - 
      Each keystroke that modifies the value, or when the mouse is released 
      and the value has changed, causes the callback (some widgets do not 
      implement this and act like FL_WHEN_CHANGED) 
   - FL_WHEN_RELEASE_ALWAYS - 
      Each recognized keystroke and the mouse being released will cause the 
      callback, even if the value did not change. (some widgets do not 
      implement this and act like FL_WHEN_RELEASE) 
   - FL_WHEN_ENTER_KEY - 
      Do the callback when the user presses the ENTER key and the value has
      changed (used by Fl_Input and Fl_Browser).
   - FL_WHEN_ENTER_KEY_ALWAYS - 
      Do the callback when the user presses the ENTER key, even if the 
      value has not changed. 
   - FL_WHEN_ENTER_KEY_CHANGED - 
      Do the callback when the user presses the ENTER key and each 
      time the value changes. 
  */
  uchar when() const		{ return when_; }
  void	when(uchar i)		{ when_ = i; }
  /*@}*/

  /**
   The default callback does nothing. callback() is initialized to this.
  */
  static void default_callback(Fl_Widget*, void*);

  /*@{*/
  /**
   You can cause a widget to do its callback at any time, 
   and even pass arbitrary arguments.
  */
  void	do_callback()		{ callback_(this,user_data_); }
  void	do_callback(Fl_Widget* o,void* arg=0) { callback_(o,arg); }
  void	do_callback(Fl_Widget* o,long arg) { callback_(o,(void*)arg); }
  /*@}*/

  /**
   Tests the value of shortcut() and the value of label() 
   (if FL_NO_SHORTCUT_LABEL is not set) against the current event
   (which must be a FL_SHORTCUT or FL_KEY event). Returns 1 if it 
   matches shortcut(), returns 2 if it matches the label(), and
   returns 0 if there is no match. 
  */
  int	test_shortcut() const	;

  /**
   Returns true if b is a child of this widget, or is equal to this widget. 
   Returns false if b is NULL.
  */
  bool	contains(const Fl_Widget*) const;
  /**
   Returns true if this is a child of a, or is equal to a. 
   Returns false if a is NULL. 
  */
  bool	inside(const Fl_Widget* o) const { return o && o->contains(this); }
  /**
   Returns true if this is equal to Fl::pushed(), meaning it has responded 
   to an FL_PUSH event and the mouse is still held down. Using this function 
   avoids the need to include the <fltk/Fl.h> header file. 
  */
  bool	pushed() const		;
  /**
   Returns true if this is equal to Fl::focus(), meaning it has the keyboard
   focus and FL_KEY events will be sent to this widget. Using this function 
   avoids the need to include the <fltk/Fl.h> header file.
  */
  bool	focused() const		;
  /**
   Returns true if this is equal to Fl::belowmouse(), meaning it has the 
   keyboard focus and FL_MOVE or FL_PUSH events will be sent to this widget. 
   Using this function avoids the need to include the <fltk/Fl.h> header file. 
  */
  bool	belowmouse() const	;

  /*@{*/
  /**
   Each widget, and many of the drawing functions, take a bitmask of flags 
   that indicate the current state and exactly how to draw things. 
   The following flags are defined: 
   - FL_INVISIBLE - !visible() 
   - FL_INACTIVE - !active() 
   - FL_OUTPUT - output() 
   - FL_VALUE - value() 
   - FL_SELECTED - selected() 
   - FL_HIGHLIGHT - draw highlighted 
   - FL_CHANGED - changed() 
   - FL_COPIED_LABEL - indicates copy_label() was called 
   - FL_RAW_LABEL - prevents interpretation of '&' and '@' in labels 
   - FL_ALIGN_* - label alignment, see below. 
   - FL_PACK_VERTICAL - Fl_Pack puts this widget vertical 
   - FL_CLICK_TO_FOCUS - click_to_focus() 
  */
  Fl_Flags flags() const	{ return flags_; }
  Fl_Flags flags(Fl_Flags f)	{ return flags_ = f; }
  Fl_Flags set_flag(int c)	{ return flags_ |= c; }
  Fl_Flags clear_flag(int c)	{ return flags_ &= ~c; }
  Fl_Flags invert_flag(int c)	{ return flags_ ^= c; }
  /*@}*/

  /*@{*/
  /**
   Forces the values of all the FL_ALIGN_* flags to the passed value. 
   This determines how the label is printed next to or inside the widget.
   The default value is FL_ALIGN_CENTER, which centers the label. 
   The value can be any of these constants or'd together: 
   - FL_ALIGN_CENTER - The label is centered (0). 
   - FL_ALIGN_TOP - The label is top-aligned. 
   - FL_ALIGN_BOTTOM - The label is bottom-aligned. 
   - FL_ALIGN_LEFT - The label is left-aligned. 
   - FL_ALIGN_RIGHT - The label is right-aligned. 
   - FL_ALIGN_CLIP - The label is clipped to the widget. 
   - FL_ALIGN_WRAP - The label text is wrapped as needed. 
   - FL_ALIGN_TOP_LEFT 
   - FL_ALIGN_TOP_RIGHT 
   - FL_ALIGN_BOTTOM_LEFT 
   - FL_ALIGN_BOTTOM_RIGHT 
   - FL_ALIGN_LEFT_TOP 
   - FL_ALIGN_RIGHT_TOP 
   - FL_ALIGN_LEFT_BOTTOM 
   - FL_ALIGN_RIGHT_BOTTOM 
   - FL_ALIGN_INSIDE - 'or' this with other values to put label inside the widget. 

  */
  Fl_Flags align() const	{ return flags_&FL_ALIGN_MASK; }
  void	align(unsigned a)	{ flags_ = (flags_ & (~FL_ALIGN_MASK)) | a; }
  /*@}*/
  /*@{*/
  /**
   An invisible widget never gets redrawn and does not get events. 
   The visible() method returns true if the widget is set to be visible.
   The visible_r() method returns true if the widget and all of its parents 
   are visible. A widget is only visible if visible() is true on it and all 
   of its parents. 
   Changing it will send FL_SHOW or FL_HIDE events to the widget. Do not 
   change it if the parent is not visible, as this will send false FL_SHOW 
   or FL_HIDE events to the widget. redraw() is called if necessary on this
   or the parent. 
  */
  bool	visible() const		{ return !(flags_&FL_INVISIBLE); }
  bool	visible_r() const	;
  void	show()			;
  void	hide()			;
  /*@}*/
  /*@{*/
  /**
   Fast inline versions of Fl_Widget::hide() and Fl_Widget::show(). 
   These do not send the FL_HIDE and FL_SHOW events to the widget.
  */
  void	set_visible()		{ flags_ &= ~FL_INVISIBLE; }
  void	clear_visible()		{ flags_ |= FL_INVISIBLE; }
  /*@}*/
  /*@{*/
  /**
   Fl_Widget::active() returns whether the widget is active. 
   Fl_Widget::active_r() returns whether the widget and all of its 
   parents are active. An inactive widget does not get any events, 
   but it does get redrawn. A widget is only active if active() is 
   true on it and all of its parents. 

   Changing this value will send FL_ACTIVATE or FL_DEACTIVATE to the 
   widget if active_r() is true. 

   Currently you cannot deactivate Fl_Window widgets. 
  */
  bool	active() const		{ return !(flags_&FL_INACTIVE); }
  bool	active_r() const	;
  void	activate()		;
  void	activate(int b)		{ if (b) activate(); else deactivate(); }
  void	deactivate()		;
  /*@}*/
  /*@{*/
  /**
   output() means the same as !active() except it does not change how the
   widget is drawn. The widget will not recieve any events. This is useful
   for making scrollbars or buttons that work as displays rather than 
   input devices. 
  */
  bool	output() const		{ return (flags_&FL_OUTPUT)!=0; }
  void	set_output()		{ flags_ |= FL_OUTPUT; }
  void	clear_output()		{ flags_ &= ~FL_OUTPUT; }
  /*@}*/
  /**
   This is the same as (active() && visible() && !output()) but is faster.
  */
  bool	takesevents() const	{ return !(flags_&(FL_OUTPUT|FL_INVISIBLE|FL_INACTIVE)); }
  /*@{*/
  /**
   Fl_Widget::changed() is a flag that is turned on when the user changes 
   the value stored in the widget. This is only used by subclasses of Fl_Widget
   that store values, but is in the base class so it is easier to scan all the 
   widgets in a panel and do_callback() on the changed ones in response to an "OK" button.
   
   Most widgets turn this flag off when they do the callback, and when the program
   sets the stored value.
  */
  bool	changed() const		{ return (flags_&FL_CHANGED)!=0; }
  void	set_changed()		{ flags_ |= FL_CHANGED; }
  void	clear_changed()		{ flags_ &= ~FL_CHANGED; }
  /*@}*/
  /*@{*/
  /**
   A true/false flag used by Fl_Button to indicate the current state
   and by "parent" items in a hierarchial Fl_Browser to indicate if 
   they are open. Many widgets will draw pushed-in or otherwise indicate 
   that this flag is on. 
  */
  bool	value() const		{ return (flags_&FL_VALUE)!=0; }
  void	set_value()		{ flags_ |= FL_VALUE; }
  void	clear_value()		{ flags_ &= ~FL_VALUE; }
  /*@}*/
  /*@{*/
  /**
   A true/false flag used to mark widgets currently selected in
   Fl_Menu and Fl_Browser widgets. Some widgets will draw with much
   different colors if this is on. 
  */
  bool	selected() const	{ return (flags_&FL_SELECTED)!=0; }
  void	set_selected()		{ flags_ |= FL_SELECTED; }
  void	clear_selected()	{ flags_ &= ~FL_SELECTED; }
  /*@}*/
  /*@{*/
  /**
   If this flag is set then if this widget returns true for an FL_PUSH 
   event then fltk will attempt to give it the focus (by calling take_focus(), 
   so it will work if this widget also returns true for FL_FOCUS events). 
   By default fltk only turns this on on certain widgets such as Fl_Input.
   Turning this on on all widgets will make the user interface match Windows
   more closely. 
  */
  bool	click_to_focus()	{ return (flags_ & FL_CLICK_TO_FOCUS) != 0; }
  void  set_click_to_focus()	{ flags_ |= FL_CLICK_TO_FOCUS; }
  void	clear_click_to_focus()	{ flags_ &= ~FL_CLICK_TO_FOCUS; }
  /*@}*/
  
  /**
   Tries to make this widget be the keyboard focus widget, by first 
   sending it an FL_FOCUS event, and if it returns non-zero, setting
   Fl::focus() to this widget. You should use this method to assign 
   the focus to an widget. Returns true if the widget accepted the focus. 
  */
  bool	take_focus()		;
  /**
   This function is called by ~Fl_Widget() and by Fl_Widget::deactivate()
   and by Fl_Widget::hide(). It indicates that the widget does not want 
   to receive any more events, and also removes all global variables that 
   point at the widget (not just the Fl::focus(), but the Fl::belowmouse(), 
   Fl::modal(), and some internal pointers). Unlike older versions of fltk, 
   no events (i.e. FL_LEAVE or FL_UNFOCUS) are sent to the widget. 
  */
  void	throw_focus()		;

  /*@{*/
  /**
   Indicates that draw() should be called, and turns on the given bits in
   damage(). At least these bits, and possibly others, will still be 
   on when draw() is called. 
  */
  void	redraw()		;
  void	redraw(uchar bits)		;
  /*@}*/
  /**
   Indicate that an outside label needs to be redrawn. This does 
   nothing if the label is inside the widget or there is no label.
   If the label is outside, the enclosing group is flagged to redraw it. 
  */
  void	redraw_label()		;
  /**
   Indicates that a rectangular region is damaged. draw() will be called 
   later with damage() set to FL_DAMAGE_ALL|FL_DAMAGE_EXPOSE and with FLTK's
   clipping set to at least the given rectangle. Normally this is called 
   more than once and the clip region will be the union of all these calls. 
   In addition damage from the operating system (ie from overlapping windows)
   will increase the clipping region. 

   This can be used to get speed up and improve complex displays of many 
   overlapping and changing objects. Even if you do nothing else about it, 
   it is usually faster to do a drawing operation that is clipped than one 
   that appears, so display will be faster. You can also check to see if 
   anything by testing fl_not_clipped(x,y,w,h) or fl_clip_box(...) and 
   skipping unnecessary drawing calls completely. Also if your normal 
   drawing causes blinking (due to overlapping objects) this can make the 
   display look much better by limiting the blinking to the small area that is
   actually changing. 
  */
  void	redraw(int x,int y,int w,int h);
  /**
   The 'or' of all the calls to redraw() done since the last draw().
   Cleared to zero after draw() is called. 
   When redrawing your widgets you should look at the damage bits to
   see what parts of your widget need redrawing. The handle() method 
   can then set individual damage bits to limit the amount of drawing
   that needs to be done: 

   \code
   MyClass::handle(int event) {
     ...
     if (change_to_part1) damage(1);
     if (change_to_part2) damage(2);
     if (change_to_part3) damage(4);
   }

   MyClass::draw() {
     if (damage() & FL_DAMAGE_ALL) {
       ... draw frame/box and other static stuff ...
     }
     if (damage() & (FL_DAMAGE_ALL | 1)) draw_part1();
     if (damage() & (FL_DAMAGE_ALL | 2)) draw_part2();
     if (damage() & (FL_DAMAGE_ALL | 4)) draw_part3();
   }
   \endcode

   Fltk assigns meaning to the following bits in the damage: 

   - FL_DAMAGE_CHILD - 
      A child of this group widget needs to be redrawn (non-group widgets can
      use this bit for their own purposes). 
   - FL_DAMAGE_CHILD_LABEL - 
      An outside label of this widget needs to be redrawn. This is handled (and 
      this bit is cleared) by the parent group. 
   - FL_DAMAGE_EXPOSE - 
      Damage caused by damage() or by expose events from the operating system.
      If this and FL_DAMAGE_ALL is on the widget should draw every pixel 
      inside it's region. 
   - FL_DAMAGE_ALL - 
      This bit is set by redraw() and indicates that all of the widget (but 
      not "holes" where the background shows through) needs to be redraw. 
   
   To avoid collisions with the these and any other future assigned bit values,
   widgets should limit themselves to these predefined bits for managing their own damage. 
   You can use the names if they are appropriate, or define your own symbols with the same 
   values: 
   - FL_DAMAGE_VALUE 
   - FL_DAMAGE_PUSHED 
   - FL_DAMAGE_SCROLL 
   - FL_DAMAGE_OVERLAY Same value as FL_DAMAGE_SCROLL. 
   - FL_DAMAGE_HIGHLIGHT 
   - FL_DAMAGE_CONTENTS 
      Same as FL_DAMAGE_EXPOSE but if FL_DAMAGE_ALL is off you can use 
      this for your own purposes. 
  */
  uchar	damage() const		{ return damage_; }
  /** Directly change the value returned by damage(). */
  void	set_damage(uchar c)	{ damage_ = c; } // should be called damage(c)

  /*@{*/
  /**
   Cause layout() to be called later (before the next draw() is called). 
   A subclass may want to use this to defer expensive display calculations 
   until the next time the system is idle. 
  */
  void  relayout()		;
  void	relayout(uchar damage)	;
  /*@}*/
  /**
   The 'or' of all the calls to relayout() or resize() done since the last 
   time layout() was called. Cleared to zero by Fl_Widget::layout(). 

   A typical layout function does not care about the widget moving, an easy 
   way to skip it is as follows: 

   \code
   MyClass::layout() {
     if (!(layout_damage() & ~FL_LAYOUT_XY)) return;
     do_expensive_layout();
     redraw();
   }
   \endcode

   The following bit values are defined: 
   - FL_LAYOUT_X - x() changed by resize() 
   - FL_LAYOUT_Y - y() changed by resize() 
   - FL_LAYOUT_XY - same as FL_LAYOUT_X|FL_LAYOUT_Y 
   - FL_LAYOUT_W - w() changed by resize() 
   - FL_LAYOUT_H - h() changed by resize() 
   - FL_LAYOUT_WH - same as FL_LAYOUT_W|FL_LAYOUT_H 
   - FL_LAYOUT_XYWH - same as FL_LAYOUT_XY|FL_LAYOUT_WH 
   - FL_LAYOUT_CHILD - 
      layout() needs to be called on a child of this group widget. 
   - FL_LAYOUT_DAMAGE - relayout() was called. 
  */
  uchar layout_damage() const	{ return layout_damage_; }
  /**
   Directly change the value returned by layout_damage(). 
  */
  void	layout_damage(uchar c)	{ layout_damage_ = c; }

  /**
   Adjust the x and y offsets and current window of the drawing functions 
   to draw into this widget. You only need to call this if you want to 
   incrementally update your widget. When draw() is called this will 
   already have been done. 
  */
  void	make_current() const	;

  /**
   Draws only the edge of the box() (by or'ing FL_INVISIBLE into the 
   flags to it). If !active_r() then it also or's in FL_INACTIVE. 
   This will only work for rectangular boxtypes. 
  */
  void  draw_frame() const	;
  /**
   Fills the entire widget with it's box() and color(). If the box is
   not rectangular and damage() contains FL_DAMAGE_EXPOSE then the 
   parent group's box is drawn to fill in the missing areas. If the 
   widget is inactive or if any bits are turned on in flags() then the 
   box may draw differently to show this.
  */
  void  draw_box() const	;
  /**
   Calls the glyph() function with the given arguments (the caller must 
   figure out inactive and selected, this is best done by using the 
   return value from draw_box()). 
  */
  void  draw_glyph(int t, int x,int y,int w,int h, Fl_Flags f) const
    { glyph()(this,t,x,y,w,h,f); }
  /**
   Draws the label anywhere. This ignores the FL_ALIGN_INSIDE and 
   always draws the label inside the passed bounding box. This is 
   used by Fl_Group and Fl_Tabs to draw the outside labels of 
   their child widgets. 
  */
  void  draw_label(int x,int y,int w,int h, Fl_Flags) const ;
  /*@{*/
  /**
   This is the usual function for a draw() method to call to draw the 
   widget's label. It does not draw the label if it is supposed to be 
   outside the box (on the assumption that the enclosing group will draw 
   those labels). 

   The label consists of the value for label() and the value for image(). 
   The alignment flags control how they are arranged next to each other in 
   the box. The color to draw the label is determined by the settings of 
   the flags, the active_r() value, and the label_color(), 
   selection_text_color(), highlight_label_color() of this widget. 

   The second form uses the passed bounding box instead of the widget's 
   bounding box. This allows the widget to draw the label somewhere other
   than centered on itself, and lets you force FL_VALUE or FL_SELECTED on
   or off (passed alignment flags are 'or'd with the widget's flags). 
  */
  void  draw_inside_label(int x,int y,int w,int h, Fl_Flags) const ;
  void	draw_inside_label() const;
  /*@}*/

  /**
   Return in w and h the size that the widget's label() and image() will take.
  */
  void	measure_label(int&, int&) const ;
  
  /*@{*/
  /** Return the given field out of the style() */
  Fl_Boxtype	box()			const;
  Fl_Boxtype	button_box()		const;
  Fl_Boxtype	focus_box()		const;
  Fl_Glyph	glyph()			const;
  Fl_Font	label_font()		const;
  Fl_Font	text_font()		const;
  Fl_Labeltype	label_type()		const;
  Fl_Color	color()			const;
  Fl_Color	label_color()		const;
  Fl_Color	selection_color()	const;
  Fl_Color	selection_text_color()	const;
  Fl_Color	button_color()		const;
  Fl_Color	highlight_color()	const;
  Fl_Color	highlight_label_color()	const;
  Fl_Color	text_color()		const;
  unsigned	label_size()		const;
  unsigned	text_size()		const;
  unsigned	leading()		const;
  Fl_Flags	scrollbar_align() const { return style()->scrollbar_align; }
  unsigned	scrollbar_width() const { return style()->scrollbar_width; }
  /*@}*/
   
  /*@{*/
  /** 
   These all create a writable Fl_Style with wstyle() and set 
   the given field in it. 
  */
  void box(Fl_Boxtype)		;
  void button_box(Fl_Boxtype)	;
  void focus_box(Fl_Boxtype)	;
  void glyph(Fl_Glyph)		;
  void label_font(Fl_Font)	;
  void text_font(Fl_Font)	;
  void label_type(Fl_Labeltype)	;
  void color(Fl_Color)		;
  void label_color(Fl_Color)	;
  void selection_color(Fl_Color);
  void selection_text_color(Fl_Color);
  void button_color(Fl_Color)	;
  void highlight_color(Fl_Color);
  void highlight_label_color(Fl_Color);
  void text_color(Fl_Color a)	;
  void label_size(unsigned a)	;
  void text_size(unsigned a)	;
  void leading(unsigned a)	;
  /*@}*/

#ifndef FLTK_2  // back-compatability section:

  Fl_Boxtype	down_box()		const { return box(); }
  Fl_Boxtype	slider()		const { return button_box(); }
  Fl_Boxtype	box2()			const { return box(); }
  Fl_Boxtype	fly_box()		const { return box(); }
  Fl_Color	color2()		const { return selection_color(); }
  Fl_Color	color3()		const { return button_color(); }
  Fl_Labeltype	labeltype()		const { return label_type(); }
  Fl_Color	labelcolor()		const { return label_color(); }
  Fl_Color	down_labelcolor()	const { return selection_text_color(); }
  Fl_Font	labelfont()		const { return label_font(); }
  unsigned	labelsize()		const { return label_size(); }
  Fl_Color	fly_color()		const { return highlight_color(); }
  Fl_Font	textfont()		const { return text_font(); }
  unsigned	textsize()		const { return text_size(); }
  Fl_Color	textcolor()		const { return text_color(); }
  Fl_Color	selected_textcolor()	const { return selection_text_color(); }
  Fl_Color	cursor_color()		const { return selection_color(); }

  void down_box(Fl_Boxtype a)           { box(a); }
  void slider(Fl_Boxtype a)             { button_box(a); }
  void fly_box(Fl_Boxtype)              { }
  void color(Fl_Color a, Fl_Color b)    { color(a); selection_color(b); }
  void color2(Fl_Color a)               { selection_color(a); }
  void color3(Fl_Color a)               { button_color(a); }
  void labeltype(Fl_Labeltype a)        { label_type(a); }
  void labelcolor(Fl_Color a)           { label_color(a); }
  void down_labelcolor(Fl_Color a)      { selection_text_color(a); }
  void labelfont(Fl_Font a)             { label_font(a); }
  void labelfont(unsigned a)            { label_font(fl_fonts+a); }
  void labelsize(unsigned a)            { label_size(a); }
  void fly_color(Fl_Color a)            { highlight_color(a); }
  void textfont(Fl_Font a)              { text_font(a); }
  void textfont(unsigned a)             { text_font(fl_fonts+a); }
  void textsize(unsigned a)             { text_size(a); }
  void textcolor(Fl_Color a)            { text_color(a); }
  void selected_textcolor(Fl_Color a)   { selection_text_color(a); }
  void cursor_color(Fl_Color a)         { selection_color(a); }

#endif

protected:

  /**
   This is the protected constructor for an Fl_Widget, 
   but all derived widgets have a matching public constructor.
   It takes a value for x(), y(), w(), h(), and an optional value for label(). 
  */
  Fl_Widget(int x,int y,int w,int h,const char* label=0);

private:

  const char*		label_;
  Fl_Image*		image_;
  int			shortcut_; // encode in the label?
  unsigned		flags_;
  const Fl_Style*	style_;
  Fl_Callback*		callback_;
  void*			user_data_;
  const char*		tooltip_; // make this into another widget?
  Fl_Group*		parent_;
  int			x_,y_,w_,h_;
  uchar			type_;
  uchar			damage_;
  uchar			layout_damage_;
  uchar			when_;

};

#endif

//
// End of "$Id: Fl_Widget.h,v 1.16 2002/10/29 00:37:23 easysw Exp $".
//
