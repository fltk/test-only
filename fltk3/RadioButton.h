//
// "$Id: RadioButton.h 6614 2009-01-01 16:11:32Z matt $"
//

/* \file
   RadioButton widget . */

#ifndef Fltk3_Radio_Button_H
#define Fltk3_Radio_Button_H

#include "Button.h"

namespace fltk3 {

class RadioButton : public Button {
public:
    RadioButton(int x,int y,int w,int h,const char *l=0)
	: Button(x,y,w,h,l) {type(FL_RADIO_BUTTON);}
};
  
}

#endif

//
// End of "$Id: RadioButton.h 6614 2009-01-01 16:11:32Z matt $".
//
