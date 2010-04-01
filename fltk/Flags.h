//
// "$Id: Flags.h 6233 2008-09-14 07:54:06Z spitzak $"
//

#ifndef fltk2_Flags_h
#define fltk2_Flags_h

namespace fltk {

typedef int Flags;
typedef Flags Align; // for back compatability
  
enum {
  NO_FLAGS		= 0x00000000,

  ALIGN_TOP		  = fltk3::ALIGN_TOP,
  ALIGN_BOTTOM		  = fltk3::ALIGN_BOTTOM,
  ALIGN_LEFT		  = fltk3::ALIGN_LEFT,
  ALIGN_RIGHT		  = fltk3::ALIGN_RIGHT,
  ALIGN_CENTER		  = fltk3::ALIGN_CENTER,
  ALIGN_INSIDE		  = fltk3::ALIGN_INSIDE,
  ALIGN_CLIP		  = fltk3::ALIGN_CLIP,
  ALIGN_WRAP		  = fltk3::ALIGN_WRAP,
  ALIGN_MASK		  = 0x000000FF,
  ALIGN_POSITIONMASK      = 0x0000000F,

  ALIGN_TOPLEFT		  = (ALIGN_TOP|ALIGN_LEFT),	    //!< Label is left-justified above widget
  ALIGN_BOTTOMLEFT	  = (ALIGN_BOTTOM|ALIGN_LEFT),	    //!< Label is left-justified below widget
  ALIGN_TOPRIGHT	  = (ALIGN_TOP|ALIGN_RIGHT),	    //!< Label is right-justified above widget
  ALIGN_BOTTOMRIGHT	  = (ALIGN_BOTTOM|ALIGN_RIGHT),	    //!< Label is right-justified below widget
  ALIGN_CENTERLEFT	  = (ALIGN_CENTER|ALIGN_LEFT),	    //!< Label is centered in space left of widget
  ALIGN_CENTERRIGHT	  = (ALIGN_CENTER|ALIGN_RIGHT),	    //!< Label is centered in space left of widget
  ALIGN_INSIDE_TOP	  = (ALIGN_INSIDE|ALIGN_TOP),	    //!< Label is inside widget at top
  ALIGN_INSIDE_BOTTOM	  = (ALIGN_INSIDE|ALIGN_BOTTOM),    //!< Label is inside widget at bottom
  ALIGN_INSIDE_LEFT	  = (ALIGN_INSIDE|ALIGN_LEFT),	    //!< Label is inside widget at left
  ALIGN_INSIDE_TOPLEFT	  = (ALIGN_INSIDE|ALIGN_TOPLEFT),   //!< Label is inside widget at top left
  ALIGN_INSIDE_BOTTOMLEFT = (ALIGN_INSIDE|ALIGN_BOTTOMLEFT),//!< Label is inside widget at bottom left
  ALIGN_INSIDE_RIGHT	  = (ALIGN_INSIDE|ALIGN_RIGHT),	    //!< Label is inside widget at right
  ALIGN_INSIDE_TOPRIGHT	  = (ALIGN_INSIDE|ALIGN_TOPRIGHT),  //!< Label is inside widget at top right
  ALIGN_INSIDE_BOTTOMRIGHT= (ALIGN_INSIDE|ALIGN_BOTTOMRIGHT),//!< Label is inside widget bottom right
  ALIGN_MENU		  = (ALIGN_INSIDE_LEFT|ALIGN_CLIP), //!< Label is inside widget bottom right
  ALIGN_BROWSER		  = ALIGN_MENU,			    //!< Label is inside widget bottom right

  /*
  INACTIVE		  = 0x00000100, //!< !active()
  OUTPUT		  = 0x00000200, //!< does not get events, draw text colors
  STATE			  = 0x00000400, //!< state(), value() for buttons 
  SELECTED		  = 0x00000800, //!< chosen in browser/menu, draw selected colors
  INVISIBLE		  = 0x00001000, //!< !visible(), draw_frame()
  HIGHLIGHT		  = 0x00002000, //!< draw highlighted
  CHANGED		  = 0x00004000, //!< value changed since last callback
  COPIED_LABEL		  = 0x00008000, //!< copy_label() was called
  RAW_LABEL		  = 0x00010000, //!< don't interpret & or @ in label
  LAYOUT_VERTICAL	  = 0x00020000, //!< fltk::Pack puts this widget vertical
  TAB_TO_FOCUS		  = 0x00040000, //!< Widget::tab_to_focus();
  CLICK_TO_FOCUS	  = 0x00080000, //!< Widget::click_to_focus()
  INACTIVE_R		  = 0x00100000, //!< draw it grayed-out
  FOCUSED		  = 0x00200000, //!< draw with keyboard focus
  PUSHED		  = 0x00400000, //!< draw pushed-in 
  RESIZE_NONE		  = 0,	//!< default behavior
  RESIZE_FIT		  = 0x01000000, //!< proportionnaly resize img in widget
  RESIZE_FILL		  = 0x00800000, //!< resize img to fill the widget
  OPENED		  = STATE	//!< opened browser hierarchy parent
   */
};

//@}

}

#endif

//
// End of "$Id: Flags.h 6233 2008-09-14 07:54:06Z spitzak $".
//
