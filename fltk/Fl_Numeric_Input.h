#include <fltk/Fl_Input.h>


/**

   The Fl_Numeric_Input class is a subclass of Fl_Input that redefines the up 
   and down arrow keys to increment and decrement the digit of the number to 
   the right of the cursor. This makes it very easy to edit numbers. 

   If you change when() to FL_WHEN_ENTER_KEY the callback is only done when 
   the user hits the up/down arrow keys or when the user types the Enter key.
   This may be more useful than the default setting of FL_WHEN_CHANGED which 
   can make the callback happen when partially-edited numbers are in the field. 

   This version lets the user type any text into the field. This is useful
   if you run the text through an expression parser so the user can type 
   in math expressions. However if you want to limit the input to text that
   can be run through strtol() or strtod() you should use the subclasses 
   Fl_Int_Input or Fl_Float_Input. 

   This user interface design is Copyright (C) 2000-2002 Digital Domain.
   Patent Pending! License is ONLY granted to implement this algorithim 
   as GPL or LGPL code (closed source may call an LGPL library to use it,
   however). License to use this technology in closed
   source code may be available:

   Digital Domain 
   Intellectual Property 
   300 Rose Avenue, Venice, CA 90291, USA 
   (310)314-2800 FAX(310)314-2888

*/
class FL_API Fl_Numeric_Input : public Fl_Input {
 protected:
  /**
   Do the work of up-arrow (if direction is greater than zero) or
   down-arrow if direction is less than zero. This protected 
   method is available so subclasses can change what keys do this,
   or invert the direction of the arrows.
  */
  int handle_arrow(int direction);
 public:
  /**
   Creates a new Fl_Numeric_Input widget using the given position, 
   size, and label string. The text starts out blank 
   (you may want to set it to "0").
  */
  Fl_Numeric_Input(int x,int y, int w,int h,const char* l = 0) : Fl_Input(x,y,w,h,l) {when(FL_WHEN_ENTER_KEY|FL_WHEN_RELEASE);}
  /**
   Does a %g sprintf of the value and uses the result to set the 
   string value. Notice that there is no inverse function, you 
   will have to call strtod(widget->value(),0,0) yourself.
  */
  void value(double);
  /**
   Does a %d sprintf of the value and uses the result to set the string value.
   Notice that there is no inverse function, you will have to call 
   strtol(widget->value(),0,0) yourself. 
  */
  void value(int);
  /*@{*/
  /**
   Same as the methods from Fl_Input, these let you set or get the string
   value in the widget. You may also want static_value() if you have a
   string in static memory. 
  */
  void value(const char* s) {Fl_Input::value(s);}
  const char* value() const {return Fl_Input::value();}
  /*@}*/
  /**
   The event handler checks for Fl_Up and Fl_Down keystrokes and
   calls handle_arrow() with them, otherwise it 
   passes the events to the base class.
  */
  int handle(int event);
};
