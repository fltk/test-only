//
// "$Id: factory.cxx,v 1.33 2005/01/02 19:24:01 leka Exp $"
//
// Widget factory code for the Fast Light Tool Kit (FLTK).
//
// Type classes for most of the fltk widgets.  Most of the work
// is done by code in WidgetType.C.  Also a factory instance
// of each of these type classes.
//
// This file also contains the "new" menu, which has a pointer
// to a factory instance for every class (both the ones defined
// here and ones in other files)
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <fltk/run.h>
#include <fltk/Group.h>
#include <string.h>
#ifdef _WIN32
# define strcasecmp(a,b) stricmp(a,b)
# define strncasecmp(a,b,c) strnicmp(a,b,c)
#endif
#include <stdio.h>

#include "Fluid_Plugins.h"
#include "FluidType.h"

////////////////////////////////////////////////////////////////

const char *WidgetType::type_name() const {return "fltk::Widget";}
fltk::Widget *WidgetType::widget(int x,int y,int w, int h) {
  return new fltk::Widget(x,y,w,h,"label");
}
WidgetType *WidgetType::_make() {return new WidgetType();}
static WidgetType Widgettype;

////////////////////////////////////////////////////////////////

#include <fltk/InvisibleBox.h>
class InvisibleBoxType : public WidgetType {
public:
  virtual const char *type_name() const {return "fltk::InvisibleBox";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::InvisibleBox(x,y,w,h,0);}
  WidgetType *_make() {return new InvisibleBoxType();}
};
static InvisibleBoxType InvisibleBoxtype;

////////////////////////////////////////////////////////////////

#include <fltk/Button.h>
static const Enumeration buttontype_menu[] = {
  {"Normal", 0,		(void*)fltk::Button::NORMAL},
  {"Toggle", "TOGGLE",	(void*)fltk::Button::TOGGLE},
  {"Radio",  "RADIO",	(void*)fltk::Button::RADIO},
  {0}};
class ButtonType : public WidgetType {
  const Enumeration *subtypes() const {return buttontype_menu;}
public:
  virtual const char *type_name() const {return "fltk::Button";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::Button(x,y,w,h,"button");}
  WidgetType *_make() {return new ButtonType();}
  int is_button() const {return 1;}
};
static ButtonType Buttontype;

////////////////////////////////////////////////////////////////

#include <fltk/ReturnButton.h>
class ReturnButtonType : public ButtonType {
public:
  virtual const char *type_name() const {return "fltk::ReturnButton";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::ReturnButton(x,y,w,h,0);}
  WidgetType *_make() {return new ReturnButtonType();}
};
static ReturnButtonType ReturnButtontype;

////////////////////////////////////////////////////////////////

#include <fltk/RepeatButton.h>
class RepeatButtonType : public WidgetType {
public:
  virtual const char *type_name() const {return "fltk::RepeatButton";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::RepeatButton(x,y,w,h,0);}
  WidgetType *_make() {return new RepeatButtonType();}
};
static RepeatButtonType RepeatButtontype;

////////////////////////////////////////////////////////////////

#include <fltk/LightButton.h>
class LightButtonType : public ButtonType {
public:
  int is_light_button() const {return 1;}
  virtual const char *type_name() const {return "fltk::LightButton";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::LightButton(x,y,w,h,"button");}
  WidgetType *_make() {return new LightButtonType();}
};
static LightButtonType LightButtontype;

////////////////////////////////////////////////////////////////

#include <fltk/CheckButton.h>
class CheckButtonType : public ButtonType {
public:
  int is_light_button() const {return 1;}
  virtual const char *type_name() const {return "fltk::CheckButton";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::CheckButton(x,y,w,h,"button");}
  WidgetType *_make() {return new CheckButtonType();}
};
static CheckButtonType CheckButtontype;

////////////////////////////////////////////////////////////////

#include <fltk/RadioButton.h>
class RadioButtonType : public ButtonType {
public:
  int is_light_button() const {return 1;}
  virtual const char *type_name() const {return "fltk::RadioButton";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::RadioButton(x,y,w,h,"button");}
  WidgetType *_make() {return new RadioButtonType();}
};
static RadioButtonType RadioButtontype;

////////////////////////////////////////////////////////////////

#include <fltk/Input.h>
static const Enumeration input_type_menu[] = {
  {"Normal",	0,	(void*)fltk::Input::NORMAL},
  {"Numeric",	0,	(void*)6,			"fltk::NumericInput"},
  {"Float",	0,	(void*)1,			"fltk::FloatInput"},
  {"Int",	0,	(void*)2,			"fltk::IntInput"},
  {"Secret",	0,	(void*)fltk::Input::SECRET,	"fltk::SecretInput"},
  {"Multiline",	0,	(void*)fltk::Input::MULTILINE,	"fltk::MultiLineInput"},
  {"Wordwrap",	0,	(void*)fltk::Input::WORDWRAP,	"fltk::WordwrapInput"},
  {0}};
class InputType : public WidgetType {
  int is_input() const {return 1;}
  const Enumeration *subtypes() const {return input_type_menu;}
public:
  virtual const char *type_name() const {return "fltk::Input";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    fltk::Input *o = new fltk::Input(x,y,w,h,"input:");
    o->value("Text Input");
    return o;
  }
  WidgetType *_make() {return new InputType();}
};
static InputType Inputtype;

////////////////////////////////////////////////////////////////

#include <fltk/Clock.h>
class ClockType : public WidgetType {
public:
  virtual const char *type_name() const {return "fltk::Clock";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::Clock(x,y,w,h);}
  WidgetType *_make() {return new ClockType();}
};
static ClockType Clocktype;

////////////////////////////////////////////////////////////////

#include <fltk/Adjuster.h>
class AdjusterType : public WidgetType {
  int is_valuator() const {return 1;}
  int is_adjuster() const {return 1;}
public:
  virtual const char *type_name() const {return "fltk::Adjuster";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::Adjuster(x,y,w,h);}
  WidgetType *_make() {return new AdjusterType();}
};
static AdjusterType Adjustertype;

////////////////////////////////////////////////////////////////

#include <fltk/Dial.h>
static const Enumeration dial_type_menu[] = {
  {"Dot", 	0,	(void*)fltk::Dial::NORMAL},
  {"Line",	"LINE",	(void*)fltk::Dial::LINE},
  {"Fill",	"FILL",	(void*)fltk::Dial::FILL},
  {0}};
class DialType : public WidgetType {
  const Enumeration *subtypes() const {return dial_type_menu;}
  int is_valuator() const {return 1;}
public:
  virtual const char *type_name() const {return "fltk::Dial";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::Dial(x,y,w,h);}
  WidgetType *_make() {return new DialType();}
};
static DialType Dialtype;

////////////////////////////////////////////////////////////////

#include <fltk/ThumbWheel.h>
class ThumbWheelType : public WidgetType {
  int is_valuator() const {return 1;}
public:
  virtual const char *type_name() const {return "fltk::ThumbWheel";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::ThumbWheel(x,y,w,h);
  }
  WidgetType *_make() {return new ThumbWheelType();}
};
static ThumbWheelType ThumbWheeltype;

////////////////////////////////////////////////////////////////

#include <fltk/ProgressBar.h>
class ProgressBarType : public WidgetType {
public:
  virtual const char *type_name() const { return "fltk::ProgressBar"; }
  fltk::Widget *widget(int x, int y, int w, int h) {
    return new fltk::ProgressBar(x, y, w, h);
  }
  WidgetType *_make() { return new ProgressBarType(); }
};
static ProgressBarType ProgressBartype;

////////////////////////////////////////////////////////////////

#include <fltk/Scrollbar.h>
static const Enumeration slider_type_menu[] = {
  {"Linear No ticks",	"LINEAR",	(void*)(fltk::Slider::LINEAR)},
  {"Linear Top/left ticks","TICK_ABOVE",(void*)(fltk::Slider::TICK_ABOVE)},
  {"Linear Bottom/right ticks","TICK_BELOW",(void*)(fltk::Slider::TICK_BELOW)},
  {"Linear Both ticks","TICK_BOTH", (void*)(fltk::Slider::TICK_BOTH)},

  {"Log No ticks",	"LOG",	(void*)(fltk::Slider::LOG)},
  {"Log Top/left ticks","LOG|fltk::Slider::TICK_ABOVE",(void*)(fltk::Slider::LOG|fltk::Slider::TICK_ABOVE)},
  {"Log Bottom/right ticks","LOG|fltk::Slider::TICK_BELOW",(void*)(fltk::Slider::LOG|fltk::Slider::TICK_BELOW)},
  {"Log Both ticks","LOG|fltk::Slider::TICK_BOTH", (void*)(fltk::Slider::LOG|fltk::Slider::TICK_BOTH)},

  {0}};
class SliderType : public WidgetType {
  const Enumeration *subtypes() const {return slider_type_menu;}
  int is_valuator() const {return 2;}
  int is_slider() const {return 1;}
public:
  virtual const char *type_name() const {return "fltk::Slider";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::Slider(x,y,w,h);
  }
  WidgetType *_make() {return new SliderType();}
};
static SliderType Slidertype;

class ScrollbarType : public SliderType {
  const Enumeration *subtypes() const {return 0;}
  int is_scrollbar() const {return 1;}
public:
  virtual const char *type_name() const {return "fltk::Scrollbar";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::Scrollbar(x,y,w,h);
  }
  WidgetType *_make() {return new ScrollbarType();}
};
static ScrollbarType Scrollbartype;

////////////////////////////////////////////////////////////////

#include <fltk/Output.h>
static const Enumeration output_type_menu[] = {
  {"Normal",	0,	(void*)fltk::Output::NORMAL},
  {"Multiline",	0,	(void*)fltk::Output::MULTILINE, "fltk::MultilineOutput"},
  {"Wordwrap",	0,	(void*)fltk::Output::WORDWRAP, "fltk::WordwrapOutput"},
  {0}};
class OutputType : public InputType {
  const Enumeration *subtypes() const {return output_type_menu;}
public:
  virtual const char *type_name() const {return "fltk::Output";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    fltk::Output *o = new fltk::Output(x,y,w,h,"output:");
    o->value("Text Output");
    return o;
  }
  WidgetType *_make() {return new OutputType();}
};
static OutputType Outputtype;

////////////////////////////////////////////////////////////////

#include <fltk/ValueInput.h>
class ValueInputType : public WidgetType {
public:
  int is_value_input() const {return 1;}
  virtual const char *type_name() const {return "fltk::ValueInput";}
  int is_valuator() const {return 1;}
  fltk::Widget *widget(int x,int y,int w,int h) {
    fltk::ValueInput *o = new fltk::ValueInput(x,y,w,h,"value:");
    return o;
  }
  WidgetType *_make() {return new ValueInputType();}
};
static ValueInputType ValueInputtype;

////////////////////////////////////////////////////////////////

#include <fltk/ValueOutput.h>
class ValueOutputType : public WidgetType {
public:
  virtual const char *type_name() const {return "fltk::ValueOutput";}
  int is_valuator() const {return 1;}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::ValueOutput(x,y,w,h,"value:");
  }
  WidgetType *_make() {return new ValueOutputType();}
};
static ValueOutputType ValueOutputtype;

////////////////////////////////////////////////////////////////

#include <fltk/ValueSlider.h>
class ValueSliderType : public SliderType {
public:
  int is_value_slider() const {return 1;}
  virtual const char *type_name() const {return "fltk::ValueSlider";}
  fltk::Widget *widget(int x,int y,int w,int h) {
    return new fltk::ValueSlider(x,y,w,h);
  }
  WidgetType *_make() {return new ValueSliderType();}
};
static ValueSliderType ValueSlidertype;

////////////////////////////////////////////////////////////////

extern class FunctionType Functiontype;
extern class CodeType Codetype;
extern class CodeBlockType CodeBlocktype;
extern class DeclType Decltype;
extern class DeclBlockType DeclBlocktype;
extern class ClassType Classtype;
extern class WindowType Windowtype;
extern class GroupType Grouptype;
extern class PackType Packtype;
extern class TabsType Tabstype;
extern class ScrollType Scrolltype;
extern class TileType Tiletype;
extern class ChoiceType Choicetype;
extern class MenuBarType MenuBartype;
extern class PopupMenuType PopupMenutype;
extern class ItemType Itemtype;
extern class DividerType Dividertype;
extern class SubmenuType Submenutype;
extern class BrowserType Browsertype;
extern class InputBrowserType InputBrowsertype;

extern void select(FluidType *,int);
extern void select_only(FluidType *);

static void cb(fltk::Widget *, void *v) {
  FluidType *t = ((FluidType*)v)->make();
  if (t) {select_only(t); modflag = 1; t->open();}
}

#include <FL/Fl_Menu_Item.H>

Fl_Menu_Item New_Menu[] = {
{"code",0,0,0,FL_SUBMENU},
  {"function/method",0,cb,(void*)&Functiontype},
  {"code",0,cb,(void*)&Codetype},
  {"code block",0,cb,(void*)&CodeBlocktype},
  {"declaration",0,cb,(void*)&Decltype},
  {"declaration block",0,cb,(void*)&DeclBlocktype},
  {"class",0,cb,(void*)&Classtype},
{0},
{"group",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&Windowtype},
  {0,0,cb,(void*)&Grouptype},
  {0,0,cb,(void*)&Packtype},
  {0,0,cb,(void*)&Tabstype},
  {0,0,cb,(void*)&Scrolltype},
  {0,0,cb,(void*)&Tiletype},
{0},
{"buttons",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&Buttontype},
  {0,0,cb,(void*)&ReturnButtontype},
  {0,0,cb,(void*)&LightButtontype},
  {0,0,cb,(void*)&CheckButtontype},
  {0,0,cb,(void*)&RadioButtontype},
  {0,0,cb,(void*)&RepeatButtontype},
{0},
{"valuators",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&Slidertype},
  {0,0,cb,(void*)&ValueSlidertype},
  {0,0,cb,(void*)&ValueInputtype},
  {0,0,cb,(void*)&ValueOutputtype},
  {0,0,cb,(void*)&Scrollbartype},
  {0,0,cb,(void*)&Adjustertype},
  {0,0,cb,(void*)&Dialtype},
  {0,0,cb,(void*)&ThumbWheeltype},
  {0,0,cb,(void*)&ProgressBartype},
{0},
{"text",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&Inputtype},
  {0,0,cb,(void*)&Outputtype},
{0},
{"menus",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&MenuBartype},
  {0,0,cb,(void*)&PopupMenutype},
  {0,0,cb,(void*)&Choicetype},
  {0,0,cb,(void*)&Browsertype},
  {0,0,cb,(void*)&InputBrowsertype},
  {0,0,cb, (void*)&Submenutype},
  {0,0,cb, (void*)&Itemtype},
  {0,0,cb, (void*)&Dividertype},
{0},
{"other",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&Widgettype},
  {0,0,cb,(void*)&InvisibleBoxtype},
  {0,0,cb,(void*)&Clocktype},
{0},
{"plugins",0,0,Plugins_New_Menu,FL_SUBMENU_POINTER},
{0}};

void fill_in_New_Menu(Fl_Menu_Item* menu) {
  int level = 0;
  for (unsigned i = 0; level || menu[i].user_data() || menu[i].text; i++) {
    Fl_Menu_Item *m = menu+i;
    if (m->flags & FL_SUBMENU) level++;
    if (!m->text && !m->user_data()) level--;
    if (m->user_data() && !m->flags && !m->text) {
      const char *n = ((FluidType*)(m->user_data()))->type_name();
      //if (!strncmp(n,"fltk::",3)) n += 6;
      m->text = n;
      m->callback_ = cb;
    }
  }
}

void fill_in_New_Menu() {
  fill_in_New_Menu(New_Menu);
}

int reading_file;

// Recursive function for searching submenus:
static FluidType *FluidType_make(const char *tn, Fl_Menu_Item* menu) {
  int level = 0;
  reading_file = 1; // makes labels be null
  FluidType *r = 0;
  for (unsigned i = 0; level||menu[i].user_data() || menu[i].text; i++) {
    Fl_Menu_Item *m = menu+i;
    if (m->flags & FL_SUBMENU) level++;
    if (!m->text && !m->user_data()) level--;
    if (!m->user_data()) continue;
    if(m->flags & FL_SUBMENU_POINTER) {
      if(r = FluidType_make(tn, (Fl_Menu_Item*) m->user_data()), r) break;
    } else {
      FluidType *t = (FluidType*)(m->user_data());
      if (!strcasecmp(tn,t->type_name())) {r = t->make(); break;}
    }
  }
  reading_file = 0;
  return r;
}

static struct {const char* oldname; const char* newname;} ntable[] = {
  {"submenu",		"fltk::ItemGroup"},
  {"menuitem",		"fltk::Item"},
  {"Fl_Counter",	"fltk::ValueInput"},
  {"Fl_Tabs",		"fltk::TabGroup"},
  {"Fl_Return_Button",	"fltk::ReturnButton"},
  {"fltk::EnterButton",	"fltk::ReturnButton"},
  {"Fl_Box",		"fltk::InvisibleBox"},
  {"Fl_Round_Button",	"fltk::RadioButton"},
  {"Fl_Pack",		"fltk::PackedGroup"},
  {"Fl_Tabs",		"fltk::TabGroup"},
  {"Fl_Scroll",		"fltk::ScrollGroup"},
  {"Fl_Bar",		"fltk::BarGroup"},
  {"Fl_Roller",		"fltk::ThumbWheel"}
};

// Create a new type by name:
FluidType *FluidType_make(const char *tn) {
  const char* p = 0;
  // First try any of our direct translations:
  for (unsigned n = 0; n < sizeof(ntable)/sizeof(*ntable); n++) {
    if (!strcasecmp(ntable[n].oldname, tn)) {p = ntable[n].newname; break;}
  }
  char buffer[128];
  // Now try cooking old names into new ones:
  if (!p && !strncmp(tn, "Fl_", 3)) {
    p = buffer;
    strcpy(buffer, "fltk::");
    char* q = buffer+6;
    for (const char* r = tn+3; *r; r++) if (*r != '_') *q++ = *r;
    *q = 0;
  } else if (!p) {
    p = tn;
  }
  return FluidType_make(p, New_Menu);
}

#include <fltk/Browser.h>

////////////////////////////////////////////////////////////////

// Since I have included all the .h files, do this table here:

// This table is only used for back compatability with XForms fdesign files,
// there should be no need to add to it in future versions of fluid.

struct symbol {const char *name; int value;};

static symbol table[] = {
  {"BLACK",	fltk::BLACK},
  {"RED",	fltk::RED},
  {"GREEN",	fltk::GREEN},
  {"YELLOW",	fltk::YELLOW},
  {"BLUE",	fltk::BLUE},
  {"MAGENTA",	fltk::MAGENTA},
  {"CYAN",	fltk::CYAN},
  {"WHITE",	fltk::WHITE},

  {"LCOL",		fltk::BLACK},
  {"COL1",		fltk::GRAY75},
  {"MCOL",		51},
  {"LEFT_BCOL",		55},
  {"TOP_BCOL",		53},
  {"BOTTOM_BCOL",	45},
  {"RIGHT_BCOL",	39},
  {"INACTIVE",		fltk::GRAY66},
  {"INACTIVE_COL",	fltk::GRAY66},
  {"FREE_COL1",		16},
  {"FREE_COL2",		17},
  {"FREE_COL3",		18},
  {"FREE_COL4",		19},
  {"FREE_COL5",		20},
  {"FREE_COL6",		21},
  {"FREE_COL7",		22},
  {"FREE_COL8",		23},
  {"FREE_COL9",		24},
  {"FREE_COL10",	25},
  {"FREE_COL11",	26},
  {"FREE_COL12",	27},
  {"FREE_COL13",	28},
  {"FREE_COL14",	29},
  {"FREE_COL15",	30},
  {"FREE_COL16",	31},
  {"TOMATO",		131},
  {"INDIANRED",		164},
  {"SLATEBLUE",		195},
  {"DARKGOLD",		84},
  {"PALEGREEN",		157},
  {"ORCHID",		203},
  {"DARKCYAN",		189},
  {"DARKTOMATO",	113},
  {"WHEAT",		174},

  {"ALIGN_CENTER",	fltk::ALIGN_CENTER},
  {"ALIGN_TOP",		fltk::ALIGN_TOP},
  {"ALIGN_BOTTOM",	fltk::ALIGN_BOTTOM},
  {"ALIGN_LEFT",	fltk::ALIGN_LEFT},
  {"ALIGN_RIGHT",	fltk::ALIGN_RIGHT},
  {"ALIGN_INSIDE",	fltk::ALIGN_INSIDE},
  {"ALIGN_TOP_LEFT",	 fltk::ALIGN_TOP | fltk::ALIGN_LEFT},
  {"ALIGN_TOP_RIGHT",	 fltk::ALIGN_TOP | fltk::ALIGN_RIGHT},
  {"ALIGN_BOTTOM_LEFT",	 fltk::ALIGN_BOTTOM | fltk::ALIGN_LEFT},
  {"ALIGN_BOTTOM_RIGHT", fltk::ALIGN_BOTTOM | fltk::ALIGN_RIGHT},
  {"ALIGN_CENTER|FL_ALIGN_INSIDE",	fltk::ALIGN_CENTER|fltk::ALIGN_INSIDE},
  {"ALIGN_TOP|FL_ALIGN_INSIDE",		fltk::ALIGN_TOP|fltk::ALIGN_INSIDE},
  {"ALIGN_BOTTOM|FL_ALIGN_INSIDE",	fltk::ALIGN_BOTTOM|fltk::ALIGN_INSIDE},
  {"ALIGN_LEFT|FL_ALIGN_INSIDE",	fltk::ALIGN_LEFT|fltk::ALIGN_INSIDE},
  {"ALIGN_RIGHT|FL_ALIGN_INSIDE",	fltk::ALIGN_RIGHT|fltk::ALIGN_INSIDE},
  {"ALIGN_INSIDE|FL_ALIGN_INSIDE",	fltk::ALIGN_INSIDE|fltk::ALIGN_INSIDE},
  {"ALIGN_TOP_LEFT|FL_ALIGN_INSIDE",	fltk::ALIGN_TOP|fltk::ALIGN_LEFT|fltk::ALIGN_INSIDE},
  {"ALIGN_TOP_RIGHT|FL_ALIGN_INSIDE",	fltk::ALIGN_TOP|fltk::ALIGN_RIGHT|fltk::ALIGN_INSIDE},
  {"ALIGN_BOTTOM_LEFT|FL_ALIGN_INSIDE",	fltk::ALIGN_BOTTOM|fltk::ALIGN_LEFT|fltk::ALIGN_INSIDE},
  {"ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE",fltk::ALIGN_BOTTOM|fltk::ALIGN_RIGHT|fltk::ALIGN_INSIDE},

  {"ALIGN_LEFT_TOP",	 fltk::ALIGN_TOP | fltk::ALIGN_LEFT},
  {"ALIGN_RIGHT_TOP",	 fltk::ALIGN_TOP | fltk::ALIGN_RIGHT},
  {"ALIGN_LEFT_BOTTOM",	 fltk::ALIGN_BOTTOM | fltk::ALIGN_LEFT},
  {"ALIGN_RIGHT_BOTTOM", fltk::ALIGN_BOTTOM | fltk::ALIGN_RIGHT},
  {"INVALID_STYLE",	 255},
  {"NORMAL_STYLE",	 0},
  {"BOLD_STYLE",	 1},
  {"ITALIC_STYLE",	 2},
  {"BOLDITALIC_STYLE",	 3},
  {"FIXED_STYLE",	 4},
  {"FIXEDBOLD_STYLE",	 5},
  {"FIXEDITALIC_STYLE",	 6},
  {"FIXEDBOLDITALIC_STYLE",  7},
  {"TIMES_STYLE",	 8},
  {"TIMESBOLD_STYLE",	 9},
  {"TIMESITALIC_STYLE",	 10},
  {"TIMESBOLDITALIC_STYLE",  11},
//   {"SHADOW_STYLE",	0x100
//   {"ENGRAVED_STYLE",	0x200
//   {"EMBOSSED_STYLE",	0x300
  {"TINY_SIZE",		8},
  {"SMALL_SIZE",	11},
  {"NORMAL_SIZE",	12},
  {"MEDIUM_SIZE",	18},
  {"LARGE_SIZE",	24},
  {"HUGE_SIZE",		32},
  {"DEFAULT_SIZE",	12},
  {"TINY_FONT",		8},
  {"SMALL_FONT",	11},
  {"NORMAL_FONT",	12},
  {"MEDIUM_FONT",	18},
  {"LARGE_FONT",	24},
  {"HUGE_FONT",		32},
  {"NORMAL_FONT1",	11},
  {"NORMAL_FONT2",	12},
  {"DEFAULT_FONT",	11},
  {"RETURN_END_CHANGED",0},
  {"RETURN_CHANGED",	1},
  {"RETURN_END",	2},
  {"RETURN_ALWAYS",	3},
  {"PUSH_BUTTON",	fltk::Button::TOGGLE},
  {"RADIO_BUTTON",	fltk::Button::RADIO},
  {"HIDDEN_BUTTON",	fltk::Button::HIDDEN},
  {"SELECT_BROWSER",	fltk::Browser::NORMAL},
  {"HOLD_BROWSER",	fltk::Browser::NORMAL},
  {"MULTI_BROWSER",	fltk::Browser::MULTI},
  //  {"SIMPLE_COUNTER",	fltk::Counter::SIMPLE},
  {"LINE_DIAL",		fltk::Dial::LINE},
  {"FILL_DIAL",		fltk::Dial::FILL},
  {"VERT_SLIDER",	fltk::Slider::LINEAR},
  {"HOR_SLIDER",	fltk::Slider::LINEAR},
  {"VERT_FILL_SLIDER",	fltk::Slider::LINEAR},
  {"HOR_FILL_SLIDER",	fltk::Slider::LINEAR},
  {"VERT_NICE_SLIDER",	fltk::Slider::LINEAR},
  {"HOR_NICE_SLIDER",	fltk::Slider::LINEAR},
};

#include <stdlib.h>

int lookup_symbol(const char *name, int &v, int numberok) {
  if (name[0]=='F' && name[1]=='L' && name[2]=='_') name += 3;
  for (int i=0; i < int(sizeof(table)/sizeof(*table)); i++)
    if (!strcasecmp(name,table[i].name)) {v = table[i].value; return 1;}
  if (numberok) {
    char* p;
    v = strtol(name, &p, 0);
    if (!*p && p > name) return 1;
  }
  return 0;
}

//
// End of "$Id: factory.cxx,v 1.33 2005/01/02 19:24:01 leka Exp $".
//
