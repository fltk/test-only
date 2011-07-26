//
// "$Id$"
//
// Widget factory code for the Fast Light Tool Kit (FLTK).
//
// Type classes for most of the fltk widgets.  Most of the work
// is done by code in Fl_Widget_Type.C.  Also a factory instance
// of each of these type classes.
//
// This file also contains the "new" menu, which has a pointer
// to a factory instance for every class (both the ones defined
// here and ones in other files)
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

#include <fltk3/run.h>
#include <fltk3/Group.h>
#include <fltk3/MenuItem.h>
#include <fltk3/Pixmap.h>
#include <fltk3/Tree.h>
#include <stdio.h>
#include "../src/flstring.h"
#include "undo.h"

#include "Fl_Widget_Type.h"

extern fltk3::Pixmap *pixmap[];

#if !HAVE_STRCASECMP
//
// 'strcasecmp()' - Do a case-insensitive compare...
//

static int
strcasecmp(const char *s, const char *t) {
  while (*s != '\0' && *t != '\0') {
    if (tolower(*s) < tolower(*t))
      return (-1);
    else if (tolower(*s) > tolower(*t))
      return (1);

    s ++;
    t ++;
  }

  if (*s == '\0' && *t == '\0')
    return (0);
  else if (*s != '\0')
    return (1);
  else
    return (-1);
}
#endif // !HAVE_STRCASECMP

////////////////////////////////////////////////////////////////

#include <fltk3/Box.h>
class Fl_Box_Type : public Fl_Widget_Type {
public:
  virtual const char *type_name() {return "fltk3::Box";}
  virtual const char *alt_type_name() {return "Fl_Box";}
  fltk3::Widget *widget(int x,int y,int w, int h) {
    return new fltk3::Box(x,y,w,h,"label");}
  Fl_Widget_Type *_make() {return new Fl_Box_Type();}
  int pixmapID() { return 5; }
};
static Fl_Box_Type Fl_Box_type;

////////////////////////////////////////////////////////////////

#include <fltk3/Button.h>
static fltk3::MenuItem buttontype_menu[] = {
  {"Normal",0,0,(void*)0},
  {"Toggle",0,0,(void*)fltk3::TOGGLE_BUTTON},
  {"Radio",0,0,(void*)fltk3::RADIO_BUTTON},
  {0}};
class Fl_Button_Type : public Fl_Widget_Type {
  fltk3::MenuItem *subtypes() {return buttontype_menu;}
public:
  virtual void ideal_size(int &w, int &h) {
    Fl_Widget_Type::ideal_size(w, h);
    w += 2 * (o->labelsize() - 4);
    h = (h / 5) * 5;
  }
  virtual const char *type_name() {return "fltk3::Button";}
  virtual const char *alt_type_name() {return "Fl_Button";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::Button(x,y,w,h,"button");}
  Fl_Widget_Type *_make() {return new Fl_Button_Type();}
  int is_button() const {return 1;}
  int pixmapID() { return 2; }
};
static Fl_Button_Type Fl_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk3/ReturnButton.h>
class Fl_Return_Button_Type : public Fl_Button_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    Fl_Button_Type::ideal_size(w, h);
    int W = o->h();
    if (o->w()/3 < W) W = o->w()/3;
    w += W + 8 - o->labelsize();
  }
  virtual const char *type_name() {return "fltk3::ReturnButton";}
  virtual const char *alt_type_name() {return "Fl_Return_Button";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::ReturnButton(x,y,w,h,"button");}
  Fl_Widget_Type *_make() {return new Fl_Return_Button_Type();}
  int pixmapID() { return 23; }
};
static Fl_Return_Button_Type Fl_Return_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk3/RepeatButton.h>
class Fl_Repeat_Button_Type : public Fl_Widget_Type {
public:
  virtual const char *type_name() {return "fltk3::RepeatButton";}
  virtual const char *alt_type_name() {return "Fl_Repeat_Button";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::RepeatButton(x,y,w,h,"button");}
  Fl_Widget_Type *_make() {return new Fl_Repeat_Button_Type();}
  int pixmapID() { return 25; }
};
static Fl_Repeat_Button_Type Fl_Repeat_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk3/LightButton.h>
class Fl_Light_Button_Type : public Fl_Button_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    Fl_Button_Type::ideal_size(w, h);
    w += 4;
  }
  virtual const char *type_name() {return "fltk3::LightButton";}
  virtual const char *alt_type_name() {return "Fl_Light_Button";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::LightButton(x,y,w,h,"button");}
  Fl_Widget_Type *_make() {return new Fl_Light_Button_Type();}
  int pixmapID() { return 24; }
};
static Fl_Light_Button_Type Fl_Light_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk3/CheckButton.h>
class Fl_Check_Button_Type : public Fl_Button_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    Fl_Button_Type::ideal_size(w, h);
    w += 4;
  }
  virtual const char *type_name() {return "fltk3::CheckButton";}
  virtual const char *alt_type_name() {return "Fl_Check_Button";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::CheckButton(x,y,w,h,"button");}
  Fl_Widget_Type *_make() {return new Fl_Check_Button_Type();}
  int pixmapID() { return 3; }
};
static Fl_Check_Button_Type Fl_Check_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk3/RoundButton.h>
class Fl_Round_Button_Type : public Fl_Button_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    Fl_Button_Type::ideal_size(w, h);
    w += 4;
  }
  virtual const char *type_name() {return "fltk3::RoundButton";}
  virtual const char *alt_type_name() {return "Fl_Round_Button";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::RoundButton(x,y,w,h,"button");}
  Fl_Widget_Type *_make() {return new Fl_Round_Button_Type();}
  int pixmapID() { return 4; }
};
static Fl_Round_Button_Type Fl_Round_Button_type;

////////////////////////////////////////////////////////////////

extern int compile_only;

#include <fltk3/Browser.h>
#include <fltk3/CheckBrowser.h>
#include <fltk3/FileBrowser.h>

static fltk3::MenuItem browser_type_menu[] = {
  {"No Select",0,0,(void*)fltk3::NORMAL_BROWSER},
  {"Select",0,0,(void*)fltk3::SELECT_BROWSER},
  {"Hold",0,0,(void*)fltk3::HOLD_BROWSER},
  {"Multi",0,0,(void*)fltk3::MULTI_BROWSER},
  {0}};
class Fl_Browser_Type : public Fl_Widget_Type {
  fltk3::MenuItem *subtypes() {return browser_type_menu;}
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::Browser *myo = (fltk3::Browser *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h -= fltk3::box_dh(o->box());
    w -= fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box());
    h = ((h + fltk3::height() - 1) / fltk3::height()) * fltk3::height() +
        fltk3::box_dh(o->box());
    if (h < 30) h = 30;
    if (w < 50) w = 50;
  }
  virtual const char *type_name() {return "fltk3::Browser";}
  virtual const char *alt_type_name() {return "Fl_Browser";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::Browser* b = new fltk3::Browser(x,y,w,h);
    // fltk3::Browser::add calls fltk3::height(), which requires the X display open.
    // Avoid this when compiling so it works w/o a display:
    if (!compile_only) {
      char buffer[20];
      for (int i = 1; i <= 20; i++) {
	sprintf(buffer,"Browser Line %d",i);
	b->add(buffer);
      }
    }
    return b;
  }
  Fl_Widget_Type *_make() {return new Fl_Browser_Type();}
  int pixmapID() { return 31; }
};
static Fl_Browser_Type Fl_Browser_type;

int Fl_Browser_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::Browser *myo = (fltk3::Browser*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

class Fl_Check_Browser_Type : public Fl_Widget_Type {
  fltk3::MenuItem *subtypes() {return browser_type_menu;}
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::CheckBrowser *myo = (fltk3::CheckBrowser *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h -= fltk3::box_dh(o->box());
    w -= fltk3::box_dw(o->box()) - fltk3::height();
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box());
    h = ((h + fltk3::height() - 1) / fltk3::height()) * fltk3::height() +
        fltk3::box_dh(o->box());
    if (h < 30) h = 30;
    if (w < 50) w = 50;
  }
  virtual const char *type_name() {return "fltk3::CheckBrowser";}
  virtual const char *alt_type_name() {return "Fl_Check_Browser";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::CheckBrowser* b = new fltk3::CheckBrowser(x,y,w,h);
    // fltk3::CheckBrowser::add calls fltk3::height(), which requires the X display open.
    // Avoid this when compiling so it works w/o a display:
    if (!compile_only) {
      char buffer[20];
      for (int i = 1; i <= 20; i++) {
	sprintf(buffer,"Browser Line %d",i);
	b->add(buffer);
      }
    }
    return b;
  }
  Fl_Widget_Type *_make() {return new Fl_Check_Browser_Type();}
  int pixmapID() { return 32; }
};
static Fl_Check_Browser_Type Fl_Check_Browser_type;

int Fl_Check_Browser_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::CheckBrowser *myo = (fltk3::CheckBrowser*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

class Fl_Tree_Type : public Fl_Widget_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    if (h < 60) h = 60;
    if (w < 80) w = 80;
  }
  virtual const char *type_name() {return "fltk3::Tree";}
  virtual const char *alt_type_name() {return "Fl_Tree";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::Tree* b = new fltk3::Tree(x,y,w,h);
    if (!compile_only) {
      b->add("/A1/B1/C1");
      b->add("/A1/B1/C2");
      b->add("/A1/B2/C1");
      b->add("/A1/B2/C2");
      b->add("/A2/B1/C1");
      b->add("/A2/B1/C2");
      b->add("/A2/B2/C1");
      b->add("/A2/B2/C2");
    }
    return b;
  }
  Fl_Widget_Type *_make() {return new Fl_Tree_Type();}
  int pixmapID() { return 50; }
};
static Fl_Tree_Type Fl_Tree_type;

class Fl_File_Browser_Type : public Fl_Widget_Type {
  fltk3::MenuItem *subtypes() {return browser_type_menu;}
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::FileBrowser *myo = (fltk3::FileBrowser *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h -= fltk3::box_dh(o->box());
    w -= fltk3::box_dw(o->box()) + fltk3::height();
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box());
    h = ((h + fltk3::height() - 1) / fltk3::height()) * fltk3::height() +
        fltk3::box_dh(o->box());
    if (h < 30) h = 30;
    if (w < 50) w = 50;
  }
  virtual const char *type_name() {return "fltk3::FileBrowser";}
  virtual const char *alt_type_name() {return "Fl_File_Browser";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::FileBrowser* b = new fltk3::FileBrowser(x,y,w,h);
    // fltk3::FileBrowser::add calls fltk3::height(), which requires the X display open.
    // Avoid this when compiling so it works w/o a display:
    if (!compile_only) {
      b->load(".");
    }
    return b;
  }
  Fl_Widget_Type *_make() {return new Fl_File_Browser_Type();}
  int pixmapID() { return 33; }
};
static Fl_File_Browser_Type Fl_File_Browser_type;

int Fl_File_Browser_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::FileBrowser *myo = (fltk3::FileBrowser*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

////////////////////////////////////////////////////////////////

#include <fltk3/Counter.h>
static fltk3::MenuItem counter_type_menu[] = {
  {"Normal",0,0,(void*)fltk3::NORMAL_COUNTER},
  {"Simple",0,0,(void*)fltk3::SIMPLE_COUNTER},
  {0}};
class Fl_Counter_Type : public Fl_Widget_Type {
  fltk3::MenuItem *subtypes() {return counter_type_menu;}
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
  int is_valuator() const {return 1;}
  int pixmapID() { return 41; }
public:
  virtual const char *type_name() {return "fltk3::Counter";}
  virtual const char *alt_type_name() {return "Fl_Counter";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::Counter(x,y,w,h,"counter:");}
  Fl_Widget_Type *_make() {return new Fl_Counter_Type();}
};
static Fl_Counter_Type Fl_Counter_type;

int Fl_Counter_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::Counter *myo = (fltk3::Counter*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

////////////////////////////////////////////////////////////////

#include <fltk3/Spinner.h>
static fltk3::MenuItem spinner_type_menu[] = {
  {"Integer",0,0,(void*)fltk3::INT_INPUT},
  {"Float",  0,0,(void*)fltk3::FLOAT_INPUT},
  {0}};
class Fl_Spinner_Type : public Fl_Widget_Type {
  fltk3::MenuItem *subtypes() {return spinner_type_menu;}
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
  int pixmapID() { return 47; }
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::Spinner *myo = (fltk3::Spinner *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h = fltk3::height() + myo->textsize() - 6;
    if (h < 15) h = 15;
    w -= fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box()) + h / 2;
    if (w < 40) w = 40	;
  }
  virtual const char *type_name() {return "fltk3::Spinner";}
  virtual const char *alt_type_name() {return "Fl_Spinner";}
  int is_spinner() const { return 1; }
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::Spinner(x,y,w,h,"spinner:");}
  Fl_Widget_Type *_make() {return new Fl_Spinner_Type();}
};
static Fl_Spinner_Type Fl_Spinner_type;

int Fl_Spinner_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::Spinner *myo = (fltk3::Spinner*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = (fltk3::Font)myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

////////////////////////////////////////////////////////////////

#include <fltk3/Input.h>
static fltk3::MenuItem input_type_menu[] = {
  {"Normal",0,0,(void*)fltk3::NORMAL_INPUT},
  {"Multiline",0,0,(void*)fltk3::MULTILINE_INPUT},
  {"Secret",0,0,(void*)fltk3::SECRET_INPUT},
  {"Int",0,0,(void*)fltk3::INT_INPUT},
  {"Float",0,0,(void*)fltk3::FLOAT_INPUT},
  {0}};
class Fl_Input_Type : public Fl_Widget_Type {
  fltk3::MenuItem *subtypes() {return input_type_menu;}
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::Input *myo = (fltk3::Input *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h = fltk3::height() + myo->textsize() - 6;
    w -= fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box());
    if (h < 15) h = 15;
    if (w < 15) w = 15;
  }
  virtual const char *type_name() {return "fltk3::Input";}
  virtual const char *alt_type_name() {return "Fl_Input";}
  int is_input() const {return 1;}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::Input *myo = new fltk3::Input(x,y,w,h,"input:");
    myo->value("Text Input");
    return myo;
  }
  Fl_Widget_Type *_make() {return new Fl_Input_Type();}
  int pixmapID() { return 14; }
  virtual void copy_properties() {
    Fl_Widget_Type::copy_properties();
    fltk3::Input_ *d = (fltk3::Input_*)live_widget, *s = (fltk3::Input_*)o;
    d->textfont(s->textfont());
    d->textsize(s->textsize());
    d->textcolor(s->textcolor());
    d->shortcut(s->shortcut());
  }
};
static Fl_Input_Type Fl_Input_type;

int Fl_Input_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::Input_ *myo = (fltk3::Input_*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

////////////////////////////////////////////////////////////////

#include <fltk3/FileInput.h>
class Fl_File_Input_Type : public Fl_Widget_Type {
  fltk3::MenuItem *subtypes() {return 0;}
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::FileInput *myo = (fltk3::FileInput *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h = fltk3::height() + myo->textsize() + 4;
    w -= fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box());
    if (h < 20) h = 20;
    if (w < 50) w = 50;
  }
  virtual const char *type_name() {return "fltk3::FileInput";}
  virtual const char *alt_type_name() {return "Fl_File_Input";}
  int is_input() const {return 1;}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::FileInput *myo = new fltk3::FileInput(x,y,w,h,"file:");
    myo->value("/now/is/the/time/for/a/filename.ext");
    return myo;
  }
  Fl_Widget_Type *_make() {return new Fl_File_Input_Type();}
  int pixmapID() { return 30; }
};
static Fl_File_Input_Type Fl_File_Input_type;

int Fl_File_Input_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::FileInput *myo = (fltk3::FileInput*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

////////////////////////////////////////////////////////////////

#include <fltk3/TextDisplay.h>
class Fl_Text_Display_Type : public Fl_Widget_Type {
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::TextDisplay *myo = (fltk3::TextDisplay *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h -= fltk3::box_dh(o->box());
    w -= fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box());
    h = ((h + fltk3::height() - 1) / fltk3::height()) * fltk3::height() +
        fltk3::box_dh(o->box());
    if (h < 30) h = 30;
    if (w < 50) w = 50;
  }
  virtual const char *type_name() {return "fltk3::TextDisplay";}
  virtual const char *alt_type_name() {return "Fl_Text_Display";}
  int is_text_display() const {return 1;}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::TextDisplay *myo = new fltk3::TextDisplay(x,y,w,h);
    return myo;
  }
  Fl_Widget_Type *_make() {return new Fl_Text_Display_Type();}
  int pixmapID() { return 28; }
};
static Fl_Text_Display_Type Fl_Text_Display_type;

int Fl_Text_Display_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::TextDisplay *myo = (fltk3::TextDisplay*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

////////////////////////////////////////////////////////////////

#include <fltk3/TextEditor.h>
class Fl_Text_Editor_Type : public Fl_Widget_Type {
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::TextEditor *myo = (fltk3::TextEditor *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h -= fltk3::box_dh(o->box());
    w -= fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box());
    h = ((h + fltk3::height() - 1) / fltk3::height()) * fltk3::height() +
        fltk3::box_dh(o->box());
    if (h < 30) h = 30;
    if (w < 50) w = 50;
  }
  virtual const char *type_name() {return "fltk3::TextEditor";}
  virtual const char *alt_type_name() {return "Fl_Text_Editor";}
  int is_text_display() const {return 1;}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::TextEditor *myo = new fltk3::TextEditor(x,y,w,h);
    return myo;
  }
  Fl_Widget_Type *_make() {return new Fl_Text_Editor_Type();}
  int pixmapID() { return 29; }
};
static Fl_Text_Editor_Type Fl_Text_Editor_type;

int Fl_Text_Editor_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::TextEditor *myo = (fltk3::TextEditor*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

////////////////////////////////////////////////////////////////

#include <fltk3/Clock.h>
class Fl_Clock_Type : public Fl_Widget_Type {
public:
  virtual const char *type_name() {return "fltk3::Clock";}
  virtual const char *alt_type_name() {return "Fl_Clock";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::Clock(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Clock_Type();}
  int pixmapID() { return 34; }
};
static Fl_Clock_Type Fl_Clock_type;

////////////////////////////////////////////////////////////////

#include <fltk3/HelpView.h>
class Fl_Help_View_Type : public Fl_Widget_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::HelpView *myo = (fltk3::HelpView *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h -= fltk3::box_dh(o->box());
    w -= fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box());
    h = ((h + fltk3::height() - 1) / fltk3::height()) * fltk3::height() +
        fltk3::box_dh(o->box());
    if (h < 30) h = 30;
    if (w < 50) w = 50;
  }
  virtual const char *type_name() {return "fltk3::HelpView";}
  virtual const char *alt_type_name() {return "Fl_Help_View";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::HelpView *myo = new fltk3::HelpView(x,y,w,h);
    if (!compile_only) {
      myo->value("<HTML><BODY><H1>fltk3::HelpView Widget</H1>"
                 "<P>This is a fltk3::HelpView widget.</P></BODY></HTML>");
    }
    return myo;}
  Fl_Widget_Type *_make() {return new Fl_Help_View_Type();}
  int pixmapID() { return 35; }
};
static Fl_Help_View_Type Fl_Help_View_type;

////////////////////////////////////////////////////////////////

#include <fltk3/Progress.h>
class Fl_Progress_Type : public Fl_Widget_Type {
public:
  virtual const char *type_name() {return "fltk3::Progress";}
  virtual const char *alt_type_name() {return "Fl_Progress";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::Progress *myo = new fltk3::Progress(x,y,w,h,"label");
    myo->value(50);
    return myo;}
  Fl_Widget_Type *_make() {return new Fl_Progress_Type();}
  int pixmapID() { return 36; }
};
static Fl_Progress_Type Fl_Progress_type;

////////////////////////////////////////////////////////////////

#include <fltk3/Adjuster.h>
class Fl_Adjuster_Type : public Fl_Widget_Type {
  int is_valuator() const {return 1;}
public:
  virtual const char *type_name() {return "fltk3::Adjuster";}
  virtual const char *alt_type_name() {return "Fl_Adjuster";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::Adjuster(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Adjuster_Type();}
  int pixmapID() { return 40; }
};
static Fl_Adjuster_Type Fl_Adjuster_type;

////////////////////////////////////////////////////////////////

#include <fltk3/Dial.h>
static fltk3::MenuItem dial_type_menu[] = {
  {"Dot",0,0,(void*)0},
  {"Line",0,0,(void*)fltk3::LINE_DIAL},
  {"Fill",0,0,(void*)fltk3::FILL_DIAL},
  {0}};
class Fl_Dial_Type : public Fl_Widget_Type {
  fltk3::MenuItem *subtypes() {return dial_type_menu;}
  int is_valuator() const {return 1;}
public:
  virtual const char *type_name() {return "fltk3::Dial";}
  virtual const char *alt_type_name() {return "Fl_Dial";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::Dial(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Dial_Type();}
  int pixmapID() { return 42; }
};
static Fl_Dial_Type Fl_Dial_type;

////////////////////////////////////////////////////////////////

#include <fltk3/Roller.h>
static fltk3::MenuItem roller_type_menu[] = {
  {"Vertical",0,0,(void*)0},
  {"Horizontal",0,0,(void*)fltk3::HORIZONTAL},
  {0}};
class Fl_Roller_Type : public Fl_Widget_Type {
  fltk3::MenuItem *subtypes() {return roller_type_menu;}
  int is_valuator() const {return 1;}
public:
  virtual const char *type_name() {return "fltk3::Roller";}
  virtual const char *alt_type_name() {return "Fl_Roller";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::Roller(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Roller_Type();}
  int pixmapID() { return 43; }
};
static Fl_Roller_Type Fl_Roller_type;

////////////////////////////////////////////////////////////////

#include <fltk3/Scrollbar.h>
static fltk3::MenuItem slider_type_menu[] = {
  {"Vertical",0,0,(void*)fltk3::VERT_SLIDER},
  {"Horizontal",0,0,(void*)fltk3::HOR_SLIDER},
  {"Vert Fill",0,0,(void*)fltk3::VERT_FILL_SLIDER},
  {"Horz Fill",0,0,(void*)fltk3::HOR_FILL_SLIDER},
  {"Vert Knob",0,0,(void*)fltk3::VERT_NICE_SLIDER},
  {"Horz Knob",0,0,(void*)fltk3::HOR_NICE_SLIDER},
  {0}};
class Fl_Slider_Type : public Fl_Widget_Type {
  fltk3::MenuItem *subtypes() {return slider_type_menu;}
  int is_valuator() const {return 2;}
public:
  virtual const char *type_name() {return "fltk3::Slider";}
  virtual const char *alt_type_name() {return "Fl_Slider";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::Slider(x,y,w,h,"slider:");}
  Fl_Widget_Type *_make() {return new Fl_Slider_Type();}
  int pixmapID() { return 37; }
};
static Fl_Slider_Type Fl_Slider_type;

static fltk3::MenuItem scrollbar_type_menu[] = {
  {"Vertical",0,0,(void*)fltk3::VERT_SLIDER},
  {"Horizontal",0,0,(void*)fltk3::HOR_SLIDER},
  {0}};
class Fl_Scrollbar_Type : public Fl_Slider_Type {
  fltk3::MenuItem *subtypes() {return scrollbar_type_menu;}
  int is_valuator() const {return 3;}
public:
  virtual const char *type_name() {return "fltk3::Scrollbar";}
  virtual const char *alt_type_name() {return "Fl_Scrollbar";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::Scrollbar(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Scrollbar_Type();}
  int pixmapID() { return 38; }
};
static Fl_Scrollbar_Type Fl_Scrollbar_type;

////////////////////////////////////////////////////////////////

#include <fltk3/Output.h>
static fltk3::MenuItem output_type_menu[] = {
  {"Normal",0,0,(void*)fltk3::NORMAL_OUTPUT},
  {"Multiline",0,0,(void*)fltk3::MULTILINE_OUTPUT},
  {0}};
class Fl_Output_Type : public Fl_Input_Type {
  fltk3::MenuItem *subtypes() {return output_type_menu;}
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::Output *myo = (fltk3::Output *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h = fltk3::height() + myo->textsize() - 6;
    w -= fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box());
    if (h < 15) h = 15;
    if (w < 15) w = 15;
  }
  virtual const char *type_name() {return "fltk3::Output";}
  virtual const char *alt_type_name() {return "Fl_Output";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::Output *myo = new fltk3::Output(x,y,w,h,"output:");
    myo->value("Text Output");
    return myo;
  }
  Fl_Widget_Type *_make() {return new Fl_Output_Type();}
  int pixmapID() { return 27; }
};
static Fl_Output_Type Fl_Output_type;

////////////////////////////////////////////////////////////////

#include <fltk3/ValueInput.h>
class Fl_Value_Input_Type : public Fl_Widget_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::ValueInput *myo = (fltk3::ValueInput *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h = fltk3::height() + myo->textsize() - 6;
    w -= fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box());
    if (h < 15) h = 15;
    if (w < 15) w = 15;
  }
  virtual const char *type_name() {return "fltk3::ValueInput";}
  virtual const char *alt_type_name() {return "Fl_Value_Input";}
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
  int is_valuator() const {return 1;}
  int is_value_input() const {return 1;}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::ValueInput *myo = new fltk3::ValueInput(x,y,w,h,"value:");
    return myo;
  }
  Fl_Widget_Type *_make() {return new Fl_Value_Input_Type();}
  int pixmapID() { return 44; }
};
static Fl_Value_Input_Type Fl_Value_Input_type;

int Fl_Value_Input_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::ValueInput *myo = (fltk3::ValueInput*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

////////////////////////////////////////////////////////////////

#include <fltk3/ValueOutput.h>
class Fl_Value_Output_Type : public Fl_Widget_Type {
public:
  virtual void ideal_size(int &w, int &h) {
    fltk3::ValueOutput *myo = (fltk3::ValueOutput *)o;
    fltk3::font(myo->textfont(), myo->textsize());
    h = fltk3::height() + myo->textsize() - 6;
    w = o->w() - fltk3::box_dw(o->box());
    int ww = (int)fltk3::width('m');
    w = ((w + ww - 1) / ww) * ww + fltk3::box_dw(o->box());
    if (h < 15) h = 15;
    if (w < 15) w = 15;
  }
  virtual const char *type_name() {return "fltk3::ValueOutput";}
  virtual const char *alt_type_name() {return "Fl_Value_Output";}
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
  int is_valuator() const {return 1;}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    fltk3::ValueOutput *myo = new fltk3::ValueOutput(x,y,w,h,"value:");
    return myo;
  }
  Fl_Widget_Type *_make() {return new Fl_Value_Output_Type();}
  int pixmapID() { return 45; }
};
static Fl_Value_Output_Type Fl_Value_Output_type;

int Fl_Value_Output_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::ValueOutput *myo = (fltk3::ValueOutput*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

////////////////////////////////////////////////////////////////

#include <fltk3/ValueSlider.h>
class Fl_Value_Slider_Type : public Fl_Slider_Type {
  int textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c);
public:
  virtual const char *type_name() {return "fltk3::ValueSlider";}
  virtual const char *alt_type_name() {return "Fl_Value_Slider";}
  fltk3::Widget *widget(int x,int y,int w,int h) {
    return new fltk3::ValueSlider(x,y,w,h,"slider:");}
  Fl_Widget_Type *_make() {return new Fl_Value_Slider_Type();}
  int pixmapID() { return 39; }
};
static Fl_Value_Slider_Type Fl_Value_Slider_type;

int Fl_Value_Slider_Type::textstuff(int w, fltk3::Font& f, int& s, fltk3::Color& c) {
  fltk3::ValueSlider *myo = (fltk3::ValueSlider*)(w==4 ? ((Fl_Widget_Type*)factory)->o : o);
  switch (w) {
    case 4:
    case 0: f = myo->textfont(); s = myo->textsize(); c = myo->textcolor(); break;
    case 1: myo->textfont(f); break;
    case 2: myo->textsize(s); break;
    case 3: myo->textcolor(c); break;
  }
  return 1;
}

////////////////////////////////////////////////////////////////

extern class Fl_Function_Type Fl_Function_type;
extern class Fl_Code_Type Fl_Code_type;
extern class Fl_CodeBlock_Type Fl_CodeBlock_type;
extern class Fl_Data_Type Fl_Data_type;
extern class Fl_Decl_Type Fl_Decl_type;
extern class Fl_DeclBlock_Type Fl_DeclBlock_type;
extern class Fl_Comment_Type Fl_Comment_type;
extern class Fl_Class_Type Fl_Class_type;
extern class Fl_Window_Type Fl_Window_type;
extern class Fl_Widget_Class_Type Fl_Widget_Class_type;
extern class Fl_Group_Type Fl_Group_type;
extern class Fl_Pack_Type Fl_Pack_type;
extern class Fl_Tabs_Type Fl_Tabs_type;
extern class Fl_Scroll_Type Fl_Scroll_type;
extern class Fl_Table_Type Fl_Table_type;
extern class Fl_Tile_Type Fl_Tile_type;
extern class Fl_Input_Choice_Type Fl_Input_Choice_type;
extern class Fl_Choice_Type Fl_Choice_type;
extern class Fl_Menu_Bar_Type Fl_Menu_Bar_type;
extern class Fl_Menu_Button_Type Fl_Menu_Button_type;
extern class Fl_Menu_Item_Type Fl_Menu_Item_type;
extern class Fl_Submenu_Type Fl_Submenu_type;
extern class Fl_Wizard_Type Fl_Wizard_type;

extern void select(Fl_Type *,int);
extern void select_only(Fl_Type *);

#include <fltk3/Window.h>

static void cb(fltk3::Widget *, void *v) {
  undo_checkpoint();
  undo_suspend();
  Fl_Type *t = ((Fl_Type*)v)->make();
  if (t) {
    if (t->is_widget() && !t->is_window()) {
      Fl_Widget_Type *wt = (Fl_Widget_Type *)t;

      // Set font sizes...
      wt->o->labelsize(Fl_Widget_Type::default_size);

      fltk3::Font f;
      int s = Fl_Widget_Type::default_size;
      fltk3::Color c;

      wt->textstuff(2, f, s, c);

      // Resize and/or reposition new widget...
      int w = 0, h = 0;
      wt->ideal_size(w, h);

      if (!strcmp(wt->type_name(), "Fl_Menu_Bar")) {
        // Move and resize the menubar across the top of the window...
        wt->o->resize(0, 0, w, h);
      } else {
        // Just resize to the ideal size...
        wt->o->size(w, h);
      }
    }
    select_only(t);
    set_modflag(1);
    t->open();
  } else {
    undo_current --;
    undo_last --;
  }
  undo_resume();
}

fltk3::MenuItem New_Menu[] = {
{"Workspace",0,0,0,fltk3::SUBMENU},
  {"Application",0,cb,(void*)&Fl_App_Target_type},
  {"Library",0,cb,(void*)&Fl_App_Target_type, fltk3::MENU_DIVIDER},
  {"C/C++ File",0,cb,(void*)&Fl_Code_File_type},
  {"Fluid File",0,cb,(void*)&Fl_Fluid_File_type},
  {"Other File",0,cb,(void*)&Fl_File_type, fltk3::MENU_DIVIDER},
  {"Group",0,cb,(void*)&Fl_Folder_type},
{0},
{"Code",0,0,0,fltk3::SUBMENU},
  {"Function/Method",0,cb,(void*)&Fl_Function_type},
  {"Code",0,cb,(void*)&Fl_Code_type},
  {"Code Block",0,cb,(void*)&Fl_CodeBlock_type},
  {"Declaration",0,cb,(void*)&Fl_Decl_type},
  {"Declaration Block",0,cb,(void*)&Fl_DeclBlock_type},
  {"Class",0,cb,(void*)&Fl_Class_type},
  {"Widget Class",0,cb,(void*)&Fl_Widget_Class_type},
  {"Comment",0,cb,(void*)&Fl_Comment_type},
  {"Binary Data",0,cb,(void*)&Fl_Data_type},
{0},
{"Group",0,0,0,fltk3::SUBMENU},
  {0,0,cb,(void*)&Fl_Window_type},
  {0,0,cb,(void*)&Fl_Group_type},
  {0,0,cb,(void*)&Fl_Pack_type},
  {0,0,cb,(void*)&Fl_Tabs_type},
  {0,0,cb,(void*)&Fl_Scroll_type},
  {0,0,cb,(void*)&Fl_Table_type},
  {0,0,cb,(void*)&Fl_Tile_type},
  {0,0,cb,(void*)&Fl_Wizard_type},
{0},
{"Buttons",0,0,0,fltk3::SUBMENU},
  {0,0,cb,(void*)&Fl_Button_type},
  {0,0,cb,(void*)&Fl_Return_Button_type},
  {0,0,cb,(void*)&Fl_Light_Button_type},
  {0,0,cb,(void*)&Fl_Check_Button_type},
  {0,0,cb,(void*)&Fl_Repeat_Button_type},
  {0,0,cb,(void*)&Fl_Round_Button_type},
{0},
{"Valuators",0,0,0,fltk3::SUBMENU},
  {0,0,cb,(void*)&Fl_Slider_type},
  {0,0,cb,(void*)&Fl_Scrollbar_type},
  {0,0,cb,(void*)&Fl_Value_Slider_type},
  {0,0,cb,(void*)&Fl_Adjuster_type},
  {0,0,cb,(void*)&Fl_Counter_type},
  {0,0,cb,(void*)&Fl_Spinner_type},
  {0,0,cb,(void*)&Fl_Dial_type},
  {0,0,cb,(void*)&Fl_Roller_type},
  {0,0,cb,(void*)&Fl_Value_Input_type},
  {0,0,cb,(void*)&Fl_Value_Output_type},
{0},
{"Text",0,0,0,fltk3::SUBMENU},
  {0,0,cb,(void*)&Fl_File_Input_type},
  {0,0,cb,(void*)&Fl_Input_type},
  {0,0,cb,(void*)&Fl_Output_type},
  {0,0,cb,(void*)&Fl_Text_Display_type},
  {0,0,cb,(void*)&Fl_Text_Editor_type},
{0},
{"Menus",0,0,0,fltk3::SUBMENU},
  {0,0,cb,(void*)&Fl_Menu_Bar_type},
  {0,0,cb,(void*)&Fl_Menu_Button_type},
  {0,0,cb,(void*)&Fl_Choice_type},
  {0,0,cb,(void*)&Fl_Input_Choice_type},
  {0,0,cb, (void*)&Fl_Submenu_type},
  {0,0,cb, (void*)&Fl_Menu_Item_type},
{0},
{"Browsers",0,0,0,fltk3::SUBMENU},
  {0,0,cb,(void*)&Fl_Browser_type},
  {0,0,cb,(void*)&Fl_Check_Browser_type},
  {0,0,cb,(void*)&Fl_File_Browser_type},
  {0,0,cb,(void*)&Fl_Tree_type},
{0},
{"Other",0,0,0,fltk3::SUBMENU},
  {0,0,cb,(void*)&Fl_Box_type},
  {0,0,cb,(void*)&Fl_Clock_type},
  {0,0,cb,(void*)&Fl_Help_View_type},
  {0,0,cb,(void*)&Fl_Progress_type},
{0},
{0}};

#include <fltk3/MultiLabel.h>

// modify a menuitem to display an icon in front of the label
static void make_iconlabel( fltk3::MenuItem *mi, fltk3::Image *ic, const char *txt )
{
  if (ic) {
    char *t1 = new char[strlen(txt)+6];
    strcpy( t1, " " );
    strcat(t1, txt);
    strcat(t1, "...");
    mi->image( ic );
    fltk3::MultiLabel *ml = new fltk3::MultiLabel;
    ml->labela = (char*)ic;
    ml->labelb = t1;
    ml->typea = fltk3::IMAGE_LABEL;
    ml->typeb = fltk3::NORMAL_LABEL;
    ml->label( mi );
  }
  else if (txt!=mi->text)
    mi->label(txt);
}

void fill_in_New_Menu() {
  for (unsigned i = 0; i < sizeof(New_Menu)/sizeof(*New_Menu); i++) {
    fltk3::MenuItem *m = New_Menu+i;
    if (m->user_data()) {
      Fl_Type *t = (Fl_Type*)m->user_data();
      if (m->text) {
        make_iconlabel( m, pixmap[t->pixmapID()], m->label() );
      } else {
        const char *n = t->type_name();
        if (!strncmp(n,"fltk3::",7)) n += 7;
        if (!strncmp(n,"Fl_",3)) n += 3;
        make_iconlabel( m, pixmap[t->pixmapID()], n );
      }
    }
  }
}

// use keyword to pick the type, this is used to parse files:
int reading_file;
Fl_Type *Fl_Type_make(const char *tn) {
  reading_file = 1; // makes labels be null
  Fl_Type *r = 0;
  for (unsigned i = 0; i < sizeof(New_Menu)/sizeof(*New_Menu); i++) {
    fltk3::MenuItem *m = New_Menu+i;
    if (!m->user_data()) continue;
    Fl_Type *t = (Fl_Type*)(m->user_data());
    if (!strcasecmp(tn,t->type_name())) {r = t->make(); break;}
    if (!strcasecmp(tn,t->alt_type_name())) {r = t->make(); break;}
  }
  reading_file = 0;
  return r;
}

////////////////////////////////////////////////////////////////

// Since I have included all the .H files, do this table here:
// This table is only used to read fdesign files:

struct symbol {const char *name; int value;};

static symbol table[] = {
  {"BLACK",	fltk3::BLACK},
  {"RED",	fltk3::RED},
  {"GREEN",	fltk3::GREEN},
  {"YELLOW",	fltk3::YELLOW},
  {"BLUE",	fltk3::BLUE},
  {"MAGENTA",	fltk3::MAGENTA},
  {"CYAN",	fltk3::CYAN},
  {"WHITE",	fltk3::WHITE},

  {"LCOL",		 fltk3::BLACK},
  {"COL1",		 fltk3::GRAY},
  {"MCOL",		 fltk3::LIGHT1},
  {"LEFT_BCOL",		 fltk3::LIGHT3},
  {"TOP_BCOL",		 fltk3::LIGHT2},
  {"BOTTOM_BCOL",	 fltk3::DARK2},
  {"RIGHT_BCOL",		 fltk3::DARK3},
  {"INACTIVE",		 fltk3::INACTIVE_COLOR},
  {"INACTIVE_COL",	 fltk3::INACTIVE_COLOR},
  {"FREE_COL1",		 fltk3::FREE_COLOR},
  {"FREE_COL2",		 fltk3::FREE_COLOR+1},
  {"FREE_COL3",		 fltk3::FREE_COLOR+2},
  {"FREE_COL4",		 fltk3::FREE_COLOR+3},
  {"FREE_COL5",		 fltk3::FREE_COLOR+4},
  {"FREE_COL6",		 fltk3::FREE_COLOR+5},
  {"FREE_COL7",		 fltk3::FREE_COLOR+6},
  {"FREE_COL8",		 fltk3::FREE_COLOR+7},
  {"FREE_COL9",		 fltk3::FREE_COLOR+8},
  {"FREE_COL10",		 fltk3::FREE_COLOR+9},
  {"FREE_COL11",		 fltk3::FREE_COLOR+10},
  {"FREE_COL12",		 fltk3::FREE_COLOR+11},
  {"FREE_COL13",		 fltk3::FREE_COLOR+12},
  {"FREE_COL14",		 fltk3::FREE_COLOR+13},
  {"FREE_COL15",		 fltk3::FREE_COLOR+14},
  {"FREE_COL16",		 fltk3::FREE_COLOR+15},
  {"TOMATO",		 131},
  {"INDIANRED",		 164},
  {"SLATEBLUE",		 195},
  {"DARKGOLD",		 84},
  {"PALEGREEN",		 157},
  {"ORCHID",		 203},
  {"DARKCYAN",		 189},
  {"DARKTOMATO",		 113},
  {"WHEAT",		 174},
  {"ALIGN_CENTER",	fltk3::ALIGN_CENTER},
  {"ALIGN_TOP",		fltk3::ALIGN_TOP},
  {"ALIGN_BOTTOM",	fltk3::ALIGN_BOTTOM},
  {"ALIGN_LEFT",	fltk3::ALIGN_LEFT},
  {"ALIGN_RIGHT",	fltk3::ALIGN_RIGHT},
  {"ALIGN_INSIDE",	fltk3::ALIGN_INSIDE},
  {"ALIGN_TOP_LEFT",	 fltk3::ALIGN_TOP | fltk3::ALIGN_LEFT},
  {"ALIGN_TOP_RIGHT",	 fltk3::ALIGN_TOP | fltk3::ALIGN_RIGHT},
  {"ALIGN_BOTTOM_LEFT",	 fltk3::ALIGN_BOTTOM | fltk3::ALIGN_LEFT},
  {"ALIGN_BOTTOM_RIGHT", fltk3::ALIGN_BOTTOM | fltk3::ALIGN_RIGHT},
  {"ALIGN_CENTER|fltk3::ALIGN_INSIDE",	fltk3::ALIGN_CENTER|fltk3::ALIGN_INSIDE},
  {"ALIGN_TOP|fltk3::ALIGN_INSIDE",		fltk3::ALIGN_TOP|fltk3::ALIGN_INSIDE},
  {"ALIGN_BOTTOM|fltk3::ALIGN_INSIDE",	fltk3::ALIGN_BOTTOM|fltk3::ALIGN_INSIDE},
  {"ALIGN_LEFT|fltk3::ALIGN_INSIDE",	fltk3::ALIGN_LEFT|fltk3::ALIGN_INSIDE},
  {"ALIGN_RIGHT|fltk3::ALIGN_INSIDE",	fltk3::ALIGN_RIGHT|fltk3::ALIGN_INSIDE},
  {"ALIGN_INSIDE|fltk3::ALIGN_INSIDE",	fltk3::ALIGN_INSIDE|fltk3::ALIGN_INSIDE},
  {"ALIGN_TOP_LEFT|fltk3::ALIGN_INSIDE",	fltk3::ALIGN_TOP|fltk3::ALIGN_LEFT|fltk3::ALIGN_INSIDE},
  {"ALIGN_TOP_RIGHT|fltk3::ALIGN_INSIDE",	fltk3::ALIGN_TOP|fltk3::ALIGN_RIGHT|fltk3::ALIGN_INSIDE},
  {"ALIGN_BOTTOM_LEFT|fltk3::ALIGN_INSIDE",	fltk3::ALIGN_BOTTOM|fltk3::ALIGN_LEFT|fltk3::ALIGN_INSIDE},
  {"ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE",fltk3::ALIGN_BOTTOM|fltk3::ALIGN_RIGHT|fltk3::ALIGN_INSIDE},

  {"ALIGN_LEFT_TOP",	 fltk3::ALIGN_TOP | fltk3::ALIGN_LEFT},
  {"ALIGN_RIGHT_TOP",	 fltk3::ALIGN_TOP | fltk3::ALIGN_RIGHT},
  {"ALIGN_LEFT_BOTTOM",	 fltk3::ALIGN_BOTTOM | fltk3::ALIGN_LEFT},
  {"ALIGN_RIGHT_BOTTOM", fltk3::ALIGN_BOTTOM | fltk3::ALIGN_RIGHT},
  {"INVALID_STYLE",	 255},
  {"NORMAL_STYLE",	 fltk3::HELVETICA},
  {"BOLD_STYLE",		 fltk3::HELVETICA|fltk3::BOLD},
  {"ITALIC_STYLE",	 fltk3::HELVETICA|fltk3::ITALIC},
  {"BOLDITALIC_STYLE",	 fltk3::HELVETICA|fltk3::BOLD|fltk3::ITALIC},
  {"FIXED_STYLE",	 fltk3::COURIER},
  {"FIXEDBOLD_STYLE",	 fltk3::COURIER|fltk3::BOLD},
  {"FIXEDITALIC_STYLE",	 fltk3::COURIER|fltk3::ITALIC},
  {"FIXEDBOLDITALIC_STYLE",  fltk3::COURIER|fltk3::BOLD|fltk3::ITALIC},
  {"TIMES_STYLE",	 fltk3::TIMES},
  {"TIMESBOLD_STYLE",	 fltk3::TIMES|fltk3::BOLD},
  {"TIMESITALIC_STYLE",	 fltk3::TIMES|fltk3::ITALIC},
  {"TIMESBOLDITALIC_STYLE",  fltk3::TIMES|fltk3::BOLD|fltk3::ITALIC},
  {"SHADOW_STYLE",	(fltk3::SHADOW_LABEL<<8)},
  {"ENGRAVED_STYLE",	(fltk3::ENGRAVED_LABEL<<8)},
  {"EMBOSSED_STYLE",	(fltk3::EMBOSSED_LABEL<<0)},
  {"TINY_SIZE",		 8},
  {"SMALL_SIZE",		 11},
  {"NORMAL_SIZE",	 fltk3::NORMAL_SIZE},
  {"MEDIUM_SIZE",	 18},
  {"LARGE_SIZE",		 24},
  {"HUGE_SIZE",		 32},
  {"DEFAULT_SIZE",	 fltk3::NORMAL_SIZE},
  {"TINY_FONT",		 8},
  {"SMALL_FONT",		 11},
  {"NORMAL_FONT",	 fltk3::NORMAL_SIZE},
  {"MEDIUM_FONT",	 18},
  {"LARGE_FONT",		 24},
  {"HUGE_FONT",		 32},
  {"NORMAL_FONT1",	 11},
  {"NORMAL_FONT2",	 fltk3::NORMAL_SIZE},
  {"DEFAULT_FONT",	 11},
  {"RETURN_END_CHANGED",  0},
  {"RETURN_CHANGED",	 1},
  {"RETURN_END",		 2},
  {"RETURN_ALWAYS",	 3},
  {"PUSH_BUTTON",	fltk3::TOGGLE_BUTTON},
  {"RADIO_BUTTON",	fltk3::RADIO_BUTTON},
  {"HIDDEN_BUTTON",	fltk3::HIDDEN_BUTTON},
  {"SELECT_BROWSER",	fltk3::SELECT_BROWSER},
  {"HOLD_BROWSER",	fltk3::HOLD_BROWSER},
  {"MULTI_BROWSER",	fltk3::MULTI_BROWSER},
  {"SIMPLE_COUNTER",	fltk3::SIMPLE_COUNTER},
  {"LINE_DIAL",		fltk3::LINE_DIAL},
  {"FILL_DIAL",		fltk3::FILL_DIAL},
  {"VERT_SLIDER",	fltk3::VERT_SLIDER},
  {"HOR_SLIDER",	fltk3::HOR_SLIDER},
  {"VERT_FILL_SLIDER",	fltk3::VERT_FILL_SLIDER},
  {"HOR_FILL_SLIDER",	fltk3::HOR_FILL_SLIDER},
  {"VERT_NICE_SLIDER",	fltk3::VERT_NICE_SLIDER},
  {"HOR_NICE_SLIDER",	fltk3::HOR_NICE_SLIDER},
};

#include <stdlib.h>

int lookup_symbol(const char *name, int &v, int numberok) {
  if (name[0]=='F' && name[1]=='L' && name[2]=='_') name += 3;
  for (int i=0; i < int(sizeof(table)/sizeof(*table)); i++)
    if (!strcasecmp(name,table[i].name)) {v = table[i].value; return 1;}
  if (numberok && ((v = atoi(name)) || !strcmp(name,"0"))) return 1;
  return 0;
}

//
// End of "$Id$".
//
