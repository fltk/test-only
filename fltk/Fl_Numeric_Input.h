#include <fltk/Fl_Input.h>

class FL_API Fl_Numeric_Input : public Fl_Input {
 protected:
  int handle_arrow(int);
 public:
  Fl_Numeric_Input(int x,int y, int w,int h,const char* l = 0) : Fl_Input(x,y,w,h,l) {when(FL_WHEN_ENTER_KEY|FL_WHEN_RELEASE);}
  void value(double);
  void value(int);
  void value(const char* s) {Fl_Input::value(s);}
  const char* value() const {return Fl_Input::value();}
  int handle(int);
};
