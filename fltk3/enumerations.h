//
// "$Id: enumerations.h 8623 2011-04-24 17:09:41Z AlbrechtS $"
//
// Enumerations for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

/** \file
 This file contains type definitions and general enumerations.
 */

#ifndef FLTK3_ENUMERATIONS_H
#define FLTK3_ENUMERATIONS_H

#  include "Export.h"
#  include "types.h"

/** \name Version Numbers
 
 FLTK defines some constants to help the programmer to
 find out, for which FLTK version a program is compiled.
 
 The following constants are defined:
 */
/*@{*/

/**
 The major release version of this FLTK library.
 \sa FL_VERSION
 */
#define FL_MAJOR_VERSION	3

/**
 The minor release version for this library.
 
 FLTK remains mostly source-code compatible between minor version changes.
 */
#define FL_MINOR_VERSION	0

/**
 The patch version for this library.
 
 FLTK remains binary compatible between patches.
 */
#define FL_PATCH_VERSION	0

/**
 The FLTK version number as a \em double.
 
 FL_VERSION is a double that describes the major and minor version numbers.
 Version 1.1 is actually stored as 1.01 to allow for more than 9 minor
 releases.
 
 The FL_MAJOR_VERSION, FL_MINOR_VERSION, and FL_PATCH_VERSION constants
 give the integral values for the major, minor, and patch releases
 respectively.
 */
#define FL_VERSION		((double)FL_MAJOR_VERSION + \
(double)FL_MINOR_VERSION * 0.01 + \
(double)FL_PATCH_VERSION * 0.0001)

/*@}*/	// group: Version Numbers

namespace fltk3 {
  
  /**
   Every time a user moves the mouse pointer, clicks a button,
   or presses a key, an event is generated and sent to your
   application. Events can also come from other programs like the
   window manager.
   
   Events are identified by the integer argument passed to the 
   fltk3::Widget::handle() virtual method. Other information about the 
   most recent event is stored in static locations and acquired  by 
   calling the fltk3::event_*() methods. This static information remains 
   valid until the next event is read from the window system, so it 
   is ok to look at it outside of the handle() method.
   
   \sa fltk3::event_text(), fltk3::event_key(), class Fl
   */
  enum Event {	// events
    /** No event. */
    NO_EVENT		= 0,
    
    /** A mouse button has gone down with the mouse pointing at this
     widget. You can find out what button by calling fltk3::event_button(). 
     You find out the mouse position by calling fltk3::event_x() and
     fltk3::event_y().
     
     A widget indicates that it "wants" the mouse click by returning non-zero 
     from its fltk3::Widget::handle() method. It will then become the 
     fltk3::pushed() widget and will get fltk3::DRAG and the matching fltk3::RELEASE events.  
     If fltk3::Widget::handle() returns zero then FLTK will try sending the fltk3::PUSH 
     to another widget. 
     */
    PUSH		= 1,
    
    /** A mouse button has been released. You can find out what button by 
     calling fltk3::event_button().
     
     In order to receive the fltk3::RELEASE event, the widget must return 
     non-zero when handling fltk3::PUSH.
     */
    RELEASE		= 2,
    
    /** The mouse has been moved to point at this widget.  This can
     be used for highlighting feedback.  If a widget wants to
     highlight or otherwise track the mouse, it indicates this by
     returning non-zero from its handle() method. It then
     becomes the fltk3::belowmouse() widget and will receive 
     fltk3::MOVE and fltk3::LEAVE events.
     */
    ENTER		= 3,
    
    /** The mouse has moved out of the widget.
     In order to receive the fltk3::LEAVE event, the widget must 
     return non-zero when handling fltk3::ENTER.
     */
    LEAVE		= 4,
    
    /** The mouse has moved with a button held down. The current button state 
     is in fltk3::event_state(). The mouse position is in fltk3::event_x() and 
     fltk3::event_y().
     
     In order to receive fltk3::DRAG events, the widget must return non-zero 
     when handling fltk3::PUSH.
     */
    DRAG		= 5,
    
    /** This indicates an <I>attempt</I> to give a widget the keyboard focus.
     
     If a widget wants the focus, it should change itself to display the 
     fact that it has the focus, and return non-zero from its handle() method.
     It then becomes the fltk3::focus() widget and gets FL_KEYDOWN, fltk3::KEYUP, 
     and fltk3::UNFOCUS events.
     
     The focus will change either because the window manager changed which 
     window gets the focus, or because the user tried to navigate using tab, 
     arrows, or other keys. You can check fltk3::event_key() to figure out why 
     it moved. For navigation it will be the key pressed and for interaction 
     with the window manager it will be zero.
     */
    FOCUS		= 6,
    
    /** This event is sent to the previous fltk3::focus() widget when another 
     widget gets the focus or the window loses focus.
     */
    UNFOCUS		= 7,
    
    /** A key was pressed (FL_KEYDOWN) or released (fltk3::KEYUP). 
     Fl_KEYBOARD is a synonym for FL_KEYDOWN.
     The key can be found in fltk3::event_key().
     The text that the key should insert can be found with fltk3::event_text() 
     and its length is in fltk3::event_length(). If you use the key handle()
     should return 1. If you return zero then FLTK assumes you ignored the 
     key and will then attempt to send it to a parent widget. If none of 
     them want it, it will change the event into a fltk3::SHORTCUT event.
     
     To receive fltk3::KEYBOARD events you must also respond to the fltk3::FOCUS
     and fltk3::UNFOCUS events.
     
     If you are writing a text-editing widget you may also want to call 
     the fltk3::compose() function to translate individual keystrokes into 
     non-ASCII characters.
     
     fltk3::KEYUP events are sent to the widget that currently has focus. This 
     is not necessarily the same widget that received the corresponding 
     FL_KEYDOWN event because focus may have changed between events.
     */
    KEYDOWN		= 8,
    
    /** Equivalent to FL_KEYDOWN.
     \see FL_KEYDOWN
     */
    KEYBOARD		= 8,
    
    /** Key release event.
     \see FL_KEYDOWN
     */
    KEYUP		= 9,
    
    /** The user clicked the close button of a window.
     This event is used internally only to trigger the callback of
     fltk3::Window derived classed. The default callback closes the 
     window calling fltk3::Window::hide().
     */
    CLOSE		= 10,
    
    /** The mouse has moved without any mouse buttons held down. 
     This event is sent to the fltk3::belowmouse() widget.
     
     In order to receive fltk3::MOVE events, the widget must return 
     non-zero when handling fltk3::ENTER.
     */
    MOVE		= 11,
    
    /** If the fltk3::focus() widget is zero or ignores an fltk3::KEYBOARD
     event then FLTK tries sending this event to every widget it 
     can, until one of them returns non-zero. fltk3::SHORTCUT is first 
     sent to the fltk3::belowmouse() widget, then its parents and siblings, 
     and eventually to every widget in the window, trying to find an 
     object that returns non-zero. FLTK tries really hard to not to ignore 
     any keystrokes!
     
     You can also make "global" shortcuts by using fltk3::add_handler(). A 
     global shortcut will work no matter what windows are displayed or 
     which one has the focus.
     */
    SHORTCUT		= 12,
    
    /** This widget is no longer active, due to fltk3::Widget::deactivate() 
     being called on it or one of its parents. fltk3::Widget::active() may 
     still be true after this, the widget is only active if fltk3::Widget::active()
     is true on it and all its parents (use fltk3::Widget::active_r() to check this).
     */
    DEACTIVATE		= 13,
    
    /** This widget is now active, due to fltk3::Widget::activate() being 
     called on it or one of its parents.
     */
    ACTIVATE		= 14,
    
    /** This widget is no longer visible, due to fltk3::Widget::hide() being 
     called on it or one of its parents, or due to a parent window being 
     minimized.  fltk3::Widget::visible() may still be true after this, but the 
     widget is visible only if visible() is true for it and all its 
     parents (use fltk3::Widget::visible_r() to check this).
     */
    HIDE		= 15,
    
    /** This widget is visible again, due to fltk3::Widget::show() being called on 
     it or one of its parents, or due to a parent window being restored. 
     Child Fl_Windows respond to this by actually creating the window if not 
     done already, so if you subclass a window, be sure to pass fltk3::SHOW 
     to the base class fltk3::Widget::handle() method!
     */
    SHOW		= 16,
    
    /** You should get this event some time after you call fltk3::paste(). 
     The contents of fltk3::event_text() is the text to insert and the number 
     of characters is in fltk3::event_length().
     */
    PASTE		= 17,
    
    /** The fltk3::selection_owner() will get this event before the selection is 
     moved to another widget. This indicates that some other widget or program 
     has claimed the selection. Motif programs used this to clear the selection 
     indication. Most modern programs ignore this.
     */
    SELECTIONCLEAR	= 18,
    
    /** The user has moved the mouse wheel. The fltk3::event_dx() and fltk3::event_dy()
     methods can be used to find the amount to scroll horizontally and vertically.
     */
    MOUSEWHEEL		= 19,
    
    /** The mouse has been moved to point at this widget. A widget that is 
     interested in receiving drag'n'drop data must return 1 to receive 
     fltk3::DND_DRAG, fltk3::DND_LEAVE and fltk3::DND_RELEASE events.
     */
    DND_ENTER		= 20,
    
    /** The mouse has been moved inside a widget while dragging data.  A 
     widget that is interested in receiving drag'n'drop data should 
     indicate the possible drop position.
     */
    DND_DRAG		= 21,
    
    /** The mouse has moved out of the widget.
     */
    DND_LEAVE		= 22,
    
    /** The user has released the mouse button dropping data into the widget. 
     If the widget returns 1, it will receive the data in the immediately 
     following fltk3::PASTE event.
     */
    DND_RELEASE	= 23
  };
  
  
  /** \name When Conditions */
  /*@{*/
  /** These constants determine when a callback is performed.
   
   \sa fltk3::Widget::when();
   \todo doxygen comments for values are incomplete and maybe wrong or unclear
   */
  enum When { // fltk3::Widget::when():
    WHEN_NEVER		= 0,	///< Never call the callback
    WHEN_CHANGED	= 1,	///< Do the callback only when the widget value changes
    WHEN_NOT_CHANGED	= 2,	///< Do the callback whenever the user interacts with the widget
    WHEN_RELEASE	= 4,	///< Do the callback when the button or key is released and the value changes
    WHEN_RELEASE_ALWAYS= 6,	///< Do the callback when the button or key is released, even if the value doesn't change
    WHEN_ENTER_KEY	= 8,	///< Do the callback when the user presses the ENTER key and the value changes
    WHEN_ENTER_KEY_ALWAYS=10,	///< Do the callback when the user presses the ENTER key, even if the value doesn't change
    WHEN_ENTER_KEY_CHANGED=11	///< ?
  };
  
  /*@}*/		// group: When Conditions
  
  
  /** \name Mouse and Keyboard Events
   
   This and the following constants define the non-ASCII keys on the
   keyboard for fltk3::KEYBOARD and fltk3::SHORTCUT events.
   
   \todo	fltk3::MouseButton and FL_key... constants could be structured better
   (use an enum or some doxygen grouping ?)
   
   \sa	fltk3::event_key() and fltk3::get_key(int) (use ascii letters for all other keys):
   */
  
  /*@{*/
  
  // FIXME: These codes collide with valid Unicode keys
  
  const unsigned int MouseButton	= 0xfee8;	///< A mouse button; use fltk3::Button + n for mouse button n.
  const unsigned int BackSpaceKey	= 0xff08;	///< The backspace key.
  const unsigned int TabKey	= 0xff09;	///< The tab key.
  const unsigned int EnterKey	= 0xff0d;	///< The enter key. 
  const unsigned int PauseKey	= 0xff13;	///< The pause key.
  const unsigned int ScrollLockKey= 0xff14;	///< The scroll lock key.
  const unsigned int EscapeKey	= 0xff1b;	///< The escape key.
  const unsigned int HomeKey	= 0xff50;	///< The home key.
  const unsigned int LeftKey	= 0xff51;	///< The left arrow key.
  const unsigned int UpKey		= 0xff52;	///< The up arrow key.
  const unsigned int RightKey	= 0xff53;	///< The right arrow key.
  const unsigned int DownKey	= 0xff54;	///< The down arrow key.
  const unsigned int PageUpKey	= 0xff55;	///< The page-up key.
  const unsigned int PageDownKey	= 0xff56;	///< The page-down key.
  const unsigned int EndKey	= 0xff57;	///< The end key.
  const unsigned int PrintKey	= 0xff61;	///< The print (or print-screen) key.
  const unsigned int InsertKey	= 0xff63;	///< The insert key. 
  const unsigned int MenuKey	= 0xff67;	///< The menu key.
  const unsigned int HelpKey	= 0xff68;	///< The 'help' key on Mac keyboards
  const unsigned int NumLockKey	= 0xff7f;	///< The num lock key.
  const unsigned int KPKey		= 0xff80;	///< One of the keypad numbers; use fltk3::KPKey + n for number n.
  const unsigned int KPEnterKey	= 0xff8d;	///< The enter key on the keypad, same as Fl_KP+'\\r'.
  const unsigned int KPLastKey	= 0xffbd;	///< The last keypad key; use to range-check keypad.
  const unsigned int FKey		= 0xffbd;	///< One of the function keys; use fltk3::FKey + n for function key n.
  const unsigned int FLastKey	= 0xffe0;	///< The last function key; use to range-check function keys.
  const unsigned int ShiftLKey	= 0xffe1;	///< The lefthand shift key.
  const unsigned int ShiftRKey	= 0xffe2;	///< The righthand shift key.
  const unsigned int ControlLKey	= 0xffe3;	///< The lefthand control key.
  const unsigned int ControlRKey	= 0xffe4;	///< The righthand control key.
  const unsigned int CapsLockKey	= 0xffe5;	///< The caps lock key.
  const unsigned int MetaLKey	= 0xffe7;	///< The left meta/Windows key.
  const unsigned int MetaRKey	= 0xffe8;	///< The right meta/Windows key.
  const unsigned int AltLKey	= 0xffe9;	///< The left alt key.
  const unsigned int AltRKey	= 0xffea;	///< The right alt key. 
  const unsigned int DeleteKey	= 0xffff;	///< The delete key.
  
  /*@}*/	// group: Mouse and Keyboard Events
  
  
  /** \name Mouse Buttons
   
   These constants define the button numbers for fltk3::PUSH and fltk3::RELEASE events.
   
   \sa fltk3::event_button()
   */
  
  /*@{*/
  
  const unsigned int LEFT_MOUSE    = 1;	///< The left mouse button
  const unsigned int MIDDLE_MOUSE  = 2;	///< The middle mouse button
  const unsigned int RIGHT_MOUSE   = 3;	///< The right mouse button
  
  /*@}*/		// group: Mouse Buttons
  
  
  /** \name	Event States
   
   The following constants define bits in the fltk3::event_state() value.
   */
  
  /*@{*/		// group: Event States
  
  // FIXME: it would be nice to have the modifiers in the upper 8 bit so that
  //        a unicode ke (24bit) can be sent as an unsigned with the modifiers.
  
  const unsigned int SHIFT	= 0x00010000;	///< One of the shift keys is down
  const unsigned int CAPS_LOCK	= 0x00020000;	///< The caps lock is on
  const unsigned int CTRL	= 0x00040000;	///< One of the ctrl keys is down
  const unsigned int ALT	= 0x00080000;	///< One of the alt keys is down
  const unsigned int NUM_LOCK	= 0x00100000;	///< The num lock is on
                                                // most X servers do this?
  const unsigned int META	= 0x00400000;	///< One of the meta/Windows keys is down
                                                // correct for XFree86
  const unsigned int SCROLL_LOCK= 0x00800000;	///< The scroll lock is on
                                                // correct for XFree86
  const unsigned int BUTTON1	= 0x01000000;	///< Mouse button 1 is pushed
  const unsigned int BUTTON2	= 0x02000000;	///< Mouse button 2 is pushed
  const unsigned int BUTTON3	= 0x04000000;	///< Mouse button 3 is pushed
  const unsigned int BUTTONS	= 0x7f000000;	///< Any mouse button is pushed
  inline unsigned int BUTTON(unsigned int n) { return (0x00800000<<(n)); } ///< Mouse button n (n > 0) is pushed
  
  const unsigned int KEY_MASK   = 0x0000ffff;		///< All keys are 16 bit for now 
                                                        //   FIXME: Unicode needs 24 bits!
#ifdef __APPLE__
  const unsigned int COMMAND	= META;		///< An alias for fltk3::CTRL on WIN32 and X11, or fltk3::META on MacOS X
  const unsigned int CONTROL 	= CTRL;		///< An alias for fltk3::META on WIN32 and X11, or fltk3::CTRL on MacOS X
#else
  const unsigned int COMMAND	= CTRL;		///< An alias for fltk3::CTRL on WIN32 and X11, or fltk3::META on MacOS X
  const unsigned int CONTROL	= META;		///< An alias for fltk3::META on WIN32 and X11, or fltk3::CTRL on MacOS X
#endif // __APPLE__
  
  /*@}*/		// group: Event States
  
  
  /** \name Box Types
   \brief FLTK standard box types
   
   This enum defines the standard box types included with FLTK.
   
   fltk3::NO_BOX means nothing is drawn at all, so whatever is already 
   on the screen remains. The FL_..._FRAME types only draw their edges, 
   leaving the interior unchanged. The blue color in Figure 1 
   is the area that is not drawn by the frame types.
   
   \image html boxtypes.png "Figure 1: FLTK standard box types"
   \image latex boxtypes.png "FLTK standard box types" width=10cm
   \todo	Description of boxtypes is incomplete.
   See below for the defined enum Boxtype.
   \see src/Fl_get_system_colors.cxx
   */
  /*@{*/
  enum Boxtype { // boxtypes (if you change these you must fix fl_boxtype.C):
    
    NO_BOX = 0,		///< nothing is drawn at all, this box is invisible
    FLAT_BOX,			///< a flat box
    UP_BOX,			///< see figure 1
    DOWN_BOX,			///< see figure 1
    UP_FRAME,			///< see figure 1
    DOWN_FRAME,		///< see figure 1
    THIN_UP_BOX,		///< see figure 1
    THIN_DOWN_BOX,		///< see figure 1
    THIN_UP_FRAME,		///< see figure 1
    THIN_DOWN_FRAME,		///< see figure 1
    ENGRAVED_BOX,		///< see figure 1
    FRAME_BOX = ENGRAVED_BOX,
    EMBOSSED_BOX,		///< see figure 1
    ENGRAVED_FRAME,		///< see figure 1
    EMBOSSED_FRAME,		///< see figure 1
    BORDER_BOX,		///< see figure 1
    SHADOW_BOX,		///< see figure 1
    BORDER_FRAME,		///< see figure 1
    SHADOW_FRAME,		///< see figure 1
    ROUNDED_BOX,		///< see figure 1
    RSHADOW_BOX,		///< see figure 1
    ROUNDED_FRAME,		///< see figure 1
    RFLAT_BOX,		///< see figure 1
    ROUND_UP_BOX,		///< see figure 1
    ROUND_DOWN_BOX,		///< see figure 1
    DIAMOND_UP_BOX,		///< see figure 1
    DIAMOND_DOWN_BOX,		///< see figure 1
    OVAL_BOX,			///< see figure 1
    OSHADOW_BOX,		///< see figure 1
    OVAL_FRAME,		///< see figure 1
    OFLAT_BOX,		///< see figure 1
    PLASTIC_UP_BOX,		///< plastic version of fltk3::UP_BOX
    PLASTIC_DOWN_BOX,		///< plastic version of fltk3::DOWN_BOX
    PLASTIC_UP_FRAME,		///< plastic version of fltk3::UP_FRAME
    PLASTIC_DOWN_FRAME,	///< plastic version of fltk3::DOWN_FRAME
    PLASTIC_THIN_UP_BOX,	///< plastic version of fltk3::THIN_UP_BOX
    PLASTIC_THIN_DOWN_BOX,	///< plastic version of fltk3::THIN_DOWN_BOX
    PLASTIC_ROUND_UP_BOX,	///< plastic version of fltk3::ROUND_UP_BOX
    PLASTIC_ROUND_DOWN_BOX,	///< plastic version of fltk3::ROUND_DOWN_BOX
    GTK_UP_BOX,		///< gtk+ version of fltk3::UP_BOX
    GTK_DOWN_BOX,		///< gtk+ version of fltk3::DOWN_BOX
    GTK_UP_FRAME,		///< gtk+ version of fltk3::UP_FRAME
    GTK_DOWN_FRAME,		///< gtk+ version of fltk3::DOWN_RAME
    GTK_THIN_UP_BOX,		///< gtk+ version of fltk3::THIN_UP_BOX
    GTK_THIN_DOWN_BOX,	///< gtk+ version of fltk3::THIN_DOWN_BOX
    GTK_THIN_UP_FRAME,	///< gtk+ version of fltk3::UP_FRAME
    GTK_THIN_DOWN_FRAME,	///< gtk+ version of fltk3::THIN_DOWN_FRAME
    GTK_ROUND_UP_BOX,		///< gtk+ version of fltk3::ROUND_UP_BOX
    GTK_ROUND_DOWN_BOX,	///< gtk+ version of fltk3::ROUND_DOWN_BOX
    FREE_BOXTYPE		///< the first free box type for creation of new box types
  };
  
  // conversions of box types to other boxtypes:
  /**
   Get the filled version of a frame.
   If no filled version of a given frame exists, the behavior of this function
   is undefined and some random box or frame is returned.
   */
  inline Boxtype box(Boxtype b) {
    return (Boxtype)((b<fltk3::UP_BOX||b%4>1)?b:(b-2));
  }
  /**
   Get the "pressed" or "down" version of a box.
   If no "down" version of a given box exists, the behavior of this function
   is undefined and some random box or frame is returned.
   */
  inline Boxtype down(Boxtype b) {
    return (Boxtype)((b<fltk3::UP_BOX)?b:(b|1));
  }
  /**
   Get the unfilled, frame only version of a box.
   If no frame version of a given box exists, the behavior of this function
   is undefined and some random box or frame is returned.
   */
  inline Boxtype frame(Boxtype b) {
    return (Boxtype)((b%4<2)?b:(b+2));
  }
  
  /*@}*/	// group: Box Types
  
  /**
   The labeltype() method sets the type of the label.
   
   The following standard label types are included:
   */
  enum Labeltype {	// labeltypes:
    NORMAL_LABEL	= 0,	///< draws the text (0)
    SYMBOL_LABEL   = 0,
    NO_LABEL,			///< does nothing
    SHADOW_LABEL,		///< draws a drop shadow under the text
    ENGRAVED_LABEL,		///< draws edges as though the text is engraved
    EMBOSSED_LABEL,		///< draws edges as though the text is raised
    MULTI_LABEL,		///< ?
    ICON_LABEL,		///< draws the icon associated with the text
    IMAGE_LABEL,		///< ?
    FREE_LABELTYPE		///< first free labeltype to use for creating own labeltypes
  };
  
  
  /** \name Alignment Flags 
   Flags to control the label alignment. 
   This controls how the label is displayed next to or inside the widget. 
   The default value is ALIGN_CENTER for most widgets, which centers the label
   inside the widget.
   
   Flags can be or'd to achieve a combination of alignments.
   
   \code
   Outside alignments:
   .            TOP_LEFT        TOP       TOP_RIGHT
   .    LEFT_TOP+---------------------------------+RIGHT_TOP
   .            |                                 |
   .        LEFT|                                 |RIGHT
   .            |                                 |
   . LEFT_BOTTOM+---------------------------------+RIGHT_BOTTOM
   .            BOTTOM_RIGHT   BOTTOM   BOTTOM_LEFT
   
   Inside alignments:
   +---------------------------------+
   |TOP_LEFT       TOP      TOP_RIGHT|
   |                                 |
   |LEFT                        RIGHT|
   |                                 |
   |BOTTOM_RIGHT  BOTTOM  BOTTOM_LEFT|
   +---------------------------------+
   \endcode
   \see #ALIGN_CENTER, etc.
   */
  /*@{*/
  /** FLTK type for alignment control */
  typedef unsigned Align;
  /** Align the label horizontally in the middle. */
  const Align ALIGN_CENTER		= (Align)0;
  /** Align the label at the top of the widget. Inside labels appear below the top,
   outside labels are drawn on top of the widget. */
  const Align ALIGN_TOP		= (Align)1;
  /** Align the label at the bottom of the widget. */
  const Align ALIGN_BOTTOM		= (Align)2;
  /** Align the label at the left of the widget. Inside labels appear left-justified
   starting at the left side of the widget, outside labels are right-justified and
   drawn to the left of the widget. */
  const Align ALIGN_LEFT		= (Align)4;
  /** Align the label to the right of the widget. */
  const Align ALIGN_RIGHT		= (Align)8;
  /** Draw the label inside of the widget. */
  const Align ALIGN_INSIDE		= (Align)16;
  /** If the label contains an image, draw the text on top of the image. */
  const Align ALIGN_TEXT_OVER_IMAGE	= (Align)0x0020;
  /** If the label contains an image, draw the text below the image. */
  const Align ALIGN_IMAGE_OVER_TEXT	= (Align)0x0000;
  /** All parts of the label that are lager than the widget will not be drawn . */
  const Align ALIGN_CLIP		= (Align)64;
  /** Wrap text that does not fit the width of the widget. */
  const Align ALIGN_WRAP		= (Align)128;
  /** If the label contains an image, draw the text to the right of the image. */
  const Align ALIGN_IMAGE_NEXT_TO_TEXT = (Align)0x0100;
  /** If the label contains an image, draw the text to the left of the image. */
  const Align ALIGN_TEXT_NEXT_TO_IMAGE = (Align)0x0120;
  /** If the label contains an image, draw the image or deimage in the background. */
  const Align ALIGN_IMAGE_BACKDROP  = (Align)0x0200;
  const Align ALIGN_TOP_LEFT	= ALIGN_TOP | ALIGN_LEFT;
  const Align ALIGN_TOP_RIGHT	= ALIGN_TOP | ALIGN_RIGHT;
  const Align ALIGN_BOTTOM_LEFT	= ALIGN_BOTTOM | ALIGN_LEFT;
  const Align ALIGN_BOTTOM_RIGHT	= ALIGN_BOTTOM | ALIGN_RIGHT;
  const Align ALIGN_LEFT_TOP	= 0x0007; // magic value
  const Align ALIGN_RIGHT_TOP	= 0x000b; // magic value
  const Align ALIGN_LEFT_BOTTOM	= 0x000d; // magic value
  const Align ALIGN_RIGHT_BOTTOM	= 0x000e; // magic value
  const Align ALIGN_NOWRAP		= (Align)0; // for back compatibility
  const Align ALIGN_POSITION_MASK   = 0x000f; // left, right, top, bottom
  const Align ALIGN_IMAGE_MASK      = 0x0320; // l/r, t/b, backdrop
  /*@}*/
  
  
  /** \name Font Numbers
   The following constants define the standard FLTK fonts:
   */
  /*@{*/
  /** A font number is an index into the internal font table. */
  typedef int Font;
  
  const Font HELVETICA              = 0;	///< Helvetica (or Arial) normal (0)
  const Font HELVETICA_BOLD         = 1;	///< Helvetica (or Arial) bold
  const Font HELVETICA_ITALIC       = 2;	///< Helvetica (or Arial) oblique
  const Font HELVETICA_BOLD_ITALIC  = 3;	///< Helvetica (or Arial) bold-oblique
  const Font COURIER                = 4;	///< Courier normal
  const Font COURIER_BOLD           = 5;	///< Courier bold 
  const Font COURIER_ITALIC         = 6;	///< Courier italic
  const Font COURIER_BOLD_ITALIC    = 7;	///< Courier bold-italic
  const Font TIMES                  = 8;	///< Times roman
  const Font TIMES_BOLD             = 9;	///< Times roman bold
  const Font TIMES_ITALIC           = 10;	///< Times roman italic
  const Font TIMES_BOLD_ITALIC      = 11;	///< Times roman bold-italic
  const Font SYMBOL                 = 12;	///< Standard symbol font
  const Font SCREEN                 = 13;	///< Default monospaced screen font
  const Font SCREEN_BOLD            = 14;	///< Default monospaced bold screen font
  const Font ZAPF_DINGBATS          = 15;	///< Zapf-dingbats font
  
  const Font FREE_FONT              = 16;	///< first one to allocate
  const Font BOLD                   = 1;	///< add this to helvetica, courier, or times
  const Font ITALIC                 = 2;	///< add this to helvetica, courier, or times
  const Font BOLD_ITALIC            = 3;	///< add this to helvetica, courier, or times
  
  /*@}*/
  
  
  /** Size of a font in pixels.
   This is the approximate height of a font in pixels.
   */
  typedef int Fontsize;
  
  extern FLTK3_EXPORT Fontsize NORMAL_SIZE;	///< normal font size
  
  
  /** \name Colors 
   The Color type holds an FLTK color value.
   
   Colors are either 8-bit indexes into a virtual colormap
   or 24-bit RGB color values.
   
   Color indices occupy the lower 8 bits of the value, while
   RGB colors occupy the upper 24 bits, for a byte organization of RGBI.
   
   <pre>
   Color => 0xrrggbbii
   | | | |
   | | | +--- index between 0 and 255
   | | +----- blue color component (8 bit)
   | +------- green component (8 bit)
   +--------- red component (8 bit)
   </pre>
   
   A color can have either an index or an rgb value. Colors with rgb set 
   and an index >0 are reserved for special use.
   
   */
  /*@{*/
  /** an FLTK color value */
  typedef unsigned int Color;
  
  // Standard colors. These are used as default colors in widgets and altered as necessary
  const Color FOREGROUND_COLOR  = 0;	///< the default foreground color (0) used for labels and text
  const Color BACKGROUND2_COLOR = 7;	///< the default background color for text, list, and valuator widgets
  const Color INACTIVE_COLOR    = 8;	///< the inactive foreground color
  const Color SELECTION_COLOR   = 15;	///< the default selection/highlight color
  
  // boxtypes generally limit themselves to these colors so
  // the whole ramp is not allocated:
  
  const Color GRAY0   = 32;			// 'A'
  const Color DARK3   = 39;			// 'H'
  const Color DARK2   = 45;			// 'N'
  const Color DARK1   = 47;			// 'P'
  const Color BACKGROUND_COLOR  = 49;	// 'R' default background color
  const Color LIGHT1  = 50;			// 'S'
  const Color LIGHT2  = 52;			// 'U'
  const Color LIGHT3  = 54;			// 'W'
  
  // FLTK provides a 5x8x5 color cube that is used with colormap visuals
  
  const Color BLACK   = 56;
  const Color RED     = 88;
  const Color GREEN   = 63;
  const Color YELLOW  = 95;
  const Color BLUE    = 216;
  const Color MAGENTA = 248;
  const Color CYAN    = 223;
  const Color DARK_RED = 72;
  
  const Color DARK_GREEN    = 60;
  const Color DARK_YELLOW   = 76;
  const Color DARK_BLUE     = 136;
  const Color DARK_MAGENTA  = 152;
  const Color DARK_CYAN     = 140;
  
  const Color WHITE         = 255;
  
  
  const Color FREE_COLOR    = 16;
  const unsigned int NUM_FREE_COLOR = 16;
  const Color GRAY_RAMP      = 32;
  const unsigned int NUM_GRAY       = 24;
  const Color GRAY           = BACKGROUND_COLOR;
  const Color COLOR_CUBE     = 56;
  const unsigned int NUM_RED        = 5;
  const unsigned int NUM_GREEN      = 8;
  const unsigned int NUM_BLUE       = 5;
  
  FLTK3_EXPORT Color inactive(Color c);
  
  FLTK3_EXPORT Color contrast(Color fg, Color bg);
  
  FLTK3_EXPORT Color color_average(Color c1, Color c2, float weight);
  
  /** Returns a lighter version of the specified color. */
  inline Color lighter(Color c) { return color_average(c, WHITE, .67f); }
  
  /** Returns a darker version of the specified color. */
  inline Color darker(Color c) { return color_average(c, BLACK, .67f); }
  
  /** Returns the 24-bit color value closest to \p r, \p g, \p b. */
  inline Color rgb_color(uchar r, uchar g, uchar b) {
    if (!r && !g && !b) return BLACK;
    else return (Color)(((((r << 8) | g) << 8) | b) << 8);
  }
  
  /** Returns the 24-bit color value closest to \p g (grayscale). */
  inline Color rgb_color(uchar g) {
    if (!g) return BLACK;
    else return (Color)(((((g << 8) | g) << 8) | g) << 8);
  }
  
  /** Returns a gray color value from black (i == 0) to white (i == fltk3::NUM_GRAY - 1). 
   fltk3::NUM_GRAY is defined to be 24 in the current FLTK release. 
   To get the closest FLTK gray value to an 8-bit grayscale color 'I' use:
   
   \code 
   fltk3::gray_ramp(I * (fltk3::NUM_GRAY - 1) / 255)
   \endcode
   */ 
  inline Color gray_ramp(int i) {return (Color)(i+GRAY_RAMP);}
  
  /** Returns a color out of the color cube.
   
   \p r must be in the range 0 to fltk3::NUM_RED (5) minus 1, 
   \p g must be in the range 0 to fltk3::NUM_GREEN (8) minus 1,
   \p b must be in the range 0 to fltk3::NUM_BLUE (5) minus 1.
   
   To get the closest color to a 8-bit set of R,G,B values use:
   
   \code
   fltk3::color_cube(R * (fltk3::NUM_RED - 1) / 255,
   G * (fltk3::NUM_GREEN - 1) / 255,
   B * (fltk3::NUM_BLUE - 1) / 255);
   \endcode
   */
  inline Color color_cube(int r, int g, int b) {
    return (Color)((b*NUM_RED + r) * NUM_GREEN + g + COLOR_CUBE);}
  
  /*@}*/		// group: Colors  
  
  
  /** \name Cursors */
  /*@{*/
  
  /** The following constants define the mouse cursors that are available in FLTK.
   
   The double-headed arrows are bitmaps provided by FLTK on X, the others
   are provided by system-defined cursors.
   
   \todo enum fltk3::Cursor needs maybe an image.
   */
  enum Cursor {
    CURSOR_DEFAULT	=  0, /**< the default cursor, usually an arrow. */
    CURSOR_ARROW	= 35, /**< an arrow pointer. */
    CURSOR_CROSS	= 66, /**< crosshair. */
    CURSOR_WAIT	= 76, /**< watch or hourglass. */
    CURSOR_INSERT	= 77, /**< I-beam. */
    CURSOR_HAND	= 31, /**< hand (uparrow on MSWindows). */
    CURSOR_HELP	= 47, /**< question mark. */
    CURSOR_MOVE	= 27, /**< 4-pointed arrow. */
    // fltk provides bitmaps for these:
    CURSOR_NS		= 78, /**< up/down arrow. */
    CURSOR_WE		= 79, /**< left/right arrow. */
    CURSOR_NWSE	= 80, /**< diagonal arrow. */
    CURSOR_NESW	= 81, /**< diagonal arrow. */
    CURSOR_NONE	=255, /**< invisible. */
    // for back compatibility (non MSWindows ones):
    CURSOR_N		= 70, /**< for back compatibility. */
    CURSOR_NE		= 69, /**< for back compatibility. */
    CURSOR_E		= 49, /**< for back compatibility. */
    CURSOR_SE		=  8, /**< for back compatibility. */
    CURSOR_S		=  9, /**< for back compatibility. */
    CURSOR_SW		=  7, /**< for back compatibility. */
    CURSOR_W		= 36, /**< for back compatibility. */
    CURSOR_NW		= 68 /**< for back compatibility. */
  };
  /*@}*/		// group: Cursors  
  
  
  /** FD "when" conditions */
  enum { // values for "when" passed to fltk3::add_fd()
    READ   = 1, /**< Call the callback when there is data to be read. */
    WRITE  = 4, /**< Call the callback when data can be written without blocking. */
    EXCEPT = 8  /**< Call the callback if an exception occurs on the file. */
  };
  
  
  /** visual types and Fl_Gl_Window::mode() (values match Glut) */
  enum Mode { 
    RGB	= 0,
    INDEX	= 1,
    SINGLE	= 0,
    DOUBLE	= 2,
    ACCUM	= 4,
    ALPHA	= 8,
    DEPTH	= 16,
    STENCIL	= 32,
    RGB8	= 64,
    MULTISAMPLE= 128,
    STEREO     = 256,
    FAKE_SINGLE = 512	// Fake single buffered windows using double-buffer
  };
  
  // image alpha blending
  
  const unsigned int IMAGE_WITH_ALPHA = 0x40000000;
  
  /** Damage masks */
  enum Damage {
    DAMAGE_CHILD    = 0x01, /**< A child needs to be redrawn. */
    DAMAGE_EXPOSE   = 0x02, /**< The window was exposed. */
    DAMAGE_SCROLL   = 0x04, /**< The fltk3::ScrollGroup widget was scrolled. */
    DAMAGE_OVERLAY  = 0x08, /**< The overlay planes need to be redrawn. */
    DAMAGE_USER1    = 0x10, /**< First user-defined damage bit. */
    DAMAGE_USER2    = 0x20, /**< Second user-defined damage bit. */
    DAMAGE_ALL      = 0x80  /**< Everything needs to be redrawn. */
  };
  
} // namespace


#endif

//
// End of "$Id: enumerations.h 8623 2011-04-24 17:09:41Z AlbrechtS $".
//
