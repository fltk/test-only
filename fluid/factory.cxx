//
// "$Id: factory.cxx,v 1.25 2002/05/06 06:31:26 spitzak Exp $"
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

#include <fltk/Fl.h>
#include <fltk/Fl_Group.h>
#include <string.h>
#include <stdio.h>
#include <config.h> // for strcasecmp

#include "Fluid_Plugins.h"
#include "Fl_Type.h"

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Box.h>
class Fl_Box_Type : public Fl_Widget_Type {
public:
  virtual const char *type_name() const {return "Fl_Box";}
  Fl_Widget *widget(int x,int y,int w, int h) {
    return new Fl_Box(x,y,w,h,"label");}
  Fl_Widget_Type *_make() {return new Fl_Box_Type();}
};
static Fl_Box_Type Fl_Box_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Button.h>
static const Enumeration buttontype_menu[] = {
  {"Normal", 0,		(void*)Fl_Button::NORMAL},
  {"Toggle", "TOGGLE",	(void*)Fl_Button::TOGGLE},
  {"Radio",  "RADIO",	(void*)Fl_Button::RADIO},
  {0}};
class Fl_Button_Type : public Fl_Widget_Type {
  const Enumeration *subtypes() const {return buttontype_menu;}
public:
  virtual const char *type_name() const {return "Fl_Button";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Button(x,y,w,h,"button");}
  Fl_Widget_Type *_make() {return new Fl_Button_Type();}
  int is_button() const {return 1;}
};
static Fl_Button_Type Fl_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Return_Button.h>
class Fl_Return_Button_Type : public Fl_Button_Type {
public:
  virtual const char *type_name() const {return "Fl_Return_Button";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Return_Button(x,y,w,h,0);}
  Fl_Widget_Type *_make() {return new Fl_Return_Button_Type();}
};
static Fl_Return_Button_Type Fl_Return_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Repeat_Button.h>
class Fl_Repeat_Button_Type : public Fl_Widget_Type {
public:
  virtual const char *type_name() const {return "Fl_Repeat_Button";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Repeat_Button(x,y,w,h,0);}
  Fl_Widget_Type *_make() {return new Fl_Repeat_Button_Type();}
};
static Fl_Repeat_Button_Type Fl_Repeat_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Light_Button.h>
class Fl_Light_Button_Type : public Fl_Button_Type {
public:
  int is_light_button() const {return 1;}
  virtual const char *type_name() const {return "Fl_Light_Button";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Light_Button(x,y,w,h,"button");}
  Fl_Widget_Type *_make() {return new Fl_Light_Button_Type();}
};
static Fl_Light_Button_Type Fl_Light_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Check_Button.h>
class Fl_Check_Button_Type : public Fl_Button_Type {
public:
  int is_light_button() const {return 1;}
  virtual const char *type_name() const {return "Fl_Check_Button";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Check_Button(x,y,w,h,"button");}
  Fl_Widget_Type *_make() {return new Fl_Check_Button_Type();}
};
static Fl_Check_Button_Type Fl_Check_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Round_Button.h>
class Fl_Round_Button_Type : public Fl_Button_Type {
public:
  int is_light_button() const {return 1;}
  virtual const char *type_name() const {return "Fl_Round_Button";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Round_Button(x,y,w,h,"button");}
  Fl_Widget_Type *_make() {return new Fl_Round_Button_Type();}
};
static Fl_Round_Button_Type Fl_Round_Button_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Counter.h>
static const Enumeration counter_type_menu[] = {
  {"Normal", "NORMAL", (void*)Fl_Counter::NORMAL},
  {"Simple", "SIMPLE", (void*)Fl_Counter::SIMPLE},
  {0}};
class Fl_Counter_Type : public Fl_Widget_Type {
  const Enumeration *subtypes() const {return counter_type_menu;}
  int is_valuator() const {return 1;}
  int is_counter() const {return 1;}
public:
  virtual const char *type_name() const {return "Fl_Counter";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Counter(x,y,w,h,"counter:");}
  Fl_Widget_Type *_make() {return new Fl_Counter_Type();}
};
static Fl_Counter_Type Fl_Counter_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Input.h>
static const Enumeration input_type_menu[] = {
  {"Normal",	0,	(void*)Fl_Input::NORMAL},
  {"Float",	0,	(void*)1,			"Fl_Float_Input"},
  {"Int",	0,	(void*)2,			"Fl_Int_Input"},
  {"Secret",	0,	(void*)Fl_Input::SECRET,	"Fl_Secret_Input"},
  {"Multiline",	0,	(void*)Fl_Input::MULTILINE,	"Fl_Multiline_Input"},
  {"Wordwrap",	0,	(void*)Fl_Input::WORDWRAP,	"Fl_Wordwrap_Input"},
  {0}};
class Fl_Input_Type : public Fl_Widget_Type {
  int is_input() const {return 1;}
  const Enumeration *subtypes() const {return input_type_menu;}
public:
  virtual const char *type_name() const {return "Fl_Input";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    Fl_Input *o = new Fl_Input(x,y,w,h,"input:");
    o->value("Text Input");
    return o;
  }
  Fl_Widget_Type *_make() {return new Fl_Input_Type();}
};
static Fl_Input_Type Fl_Input_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Clock.h>
class Fl_Clock_Type : public Fl_Widget_Type {
public:
  virtual const char *type_name() const {return "Fl_Clock";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Clock(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Clock_Type();}
};
static Fl_Clock_Type Fl_Clock_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Adjuster.h>
class Fl_Adjuster_Type : public Fl_Widget_Type {
  int is_valuator() const {return 1;}
  int is_adjuster() const {return 1;}
public:
  virtual const char *type_name() const {return "Fl_Adjuster";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Adjuster(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Adjuster_Type();}
};
static Fl_Adjuster_Type Fl_Adjuster_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Dial.h>
static const Enumeration dial_type_menu[] = {
  {"Dot", 	0,	(void*)Fl_Dial::NORMAL},
  {"Line",	"LINE",	(void*)Fl_Dial::LINE},
  {"Fill",	"FILL",	(void*)Fl_Dial::FILL},
  {0}};
class Fl_Dial_Type : public Fl_Widget_Type {
  const Enumeration *subtypes() const {return dial_type_menu;}
  int is_valuator() const {return 1;}
public:
  virtual const char *type_name() const {return "Fl_Dial";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Dial(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Dial_Type();}
};
static Fl_Dial_Type Fl_Dial_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Roller.h>
static const Enumeration roller_type_menu[] = {
  {"Vertical",   "VERTICAL",   (void*)Fl_Roller::VERTICAL},
  {"Horizontal", "HORIZONTAL", (void*)Fl_Roller::HORIZONTAL},
  {0}};
class Fl_Roller_Type : public Fl_Widget_Type {
  const Enumeration *subtypes() const {return roller_type_menu;}
  int is_valuator() const {return 1;}
public:
  virtual const char *type_name() const {return "Fl_Roller";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Roller(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Roller_Type();}
};
static Fl_Roller_Type Fl_Roller_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Scrollbar.h>
static const Enumeration slider_type_menu[] = {
  {"Vertical Lin No ticks","VERTICAL",	(void*)Fl_Slider::VERTICAL},
  {"Vertical Lin Left","VERTICAL|Fl_Slider::TICK_ABOVE",(void*)(Fl_Slider::VERTICAL|Fl_Slider::TICK_ABOVE)},
  {"Vertical Lin Right","VERTICAL|Fl_Slider::TICK_BELOW",(void*)(Fl_Slider::VERTICAL|Fl_Slider::TICK_BELOW)},
  {"Vertical Lin Both","VERTICAL|Fl_Slider::TICK_BOTH",(void*)(Fl_Slider::VERTICAL|Fl_Slider::TICK_BOTH)},

  {"Vertical Log No ticks","VERTICAL|Fl_Slider::LOG",	(void*)(Fl_Slider::VERTICAL|Fl_Slider::LOG)},
  {"Vertical Log Left","VERTICAL|Fl_Slider::LOG|Fl_Slider::TICK_ABOVE",(void*)(Fl_Slider::VERTICAL|Fl_Slider::LOG|Fl_Slider::TICK_ABOVE)},
  {"Vertical Log Right","VERTICAL|Fl_Slider::LOG|Fl_Slider::TICK_BELOW",(void*)(Fl_Slider::VERTICAL|Fl_Slider::LOG|Fl_Slider::TICK_BELOW)},
  {"Vertical Log Both","VERTICAL|Fl_Slider::LOG|Fl_Slider::TICK_BOTH",(void*)(Fl_Slider::VERTICAL|Fl_Slider::LOG|Fl_Slider::TICK_BOTH)},

  {"Horizontal Lin No ticks","HORIZONTAL",	(void*)Fl_Slider::HORIZONTAL},
  {"Horizontal Lin Above","HORIZONTAL|Fl_Slider::TICK_ABOVE",(void*)(Fl_Slider::HORIZONTAL|Fl_Slider::TICK_ABOVE)},
  {"Horizontal Lin Below","HORIZONTAL|Fl_Slider::TICK_BELOW",(void*)(Fl_Slider::HORIZONTAL|Fl_Slider::TICK_BELOW)},
  {"Horizontal Lin Both","HORIZONTAL|Fl_Slider::TICK_BOTH",(void*)(Fl_Slider::HORIZONTAL|Fl_Slider::TICK_BOTH)},

  {"Horizontal Log No ticks","HORIZONTAL|Fl_Slider::LOG",	(void*)(Fl_Slider::HORIZONTAL|Fl_Slider::LOG)},
  {"Horizontal Log Above","HORIZONTAL|Fl_Slider::LOG|Fl_Slider::TICK_ABOVE",(void*)(Fl_Slider::HORIZONTAL|Fl_Slider::LOG|Fl_Slider::TICK_ABOVE)},
  {"Horizontal Log Below","HORIZONTAL|Fl_Slider::LOG|Fl_Slider::TICK_BELOW",(void*)(Fl_Slider::HORIZONTAL|Fl_Slider::LOG|Fl_Slider::TICK_BELOW)},
  {"Horizontal Log Both","HORIZONTAL|Fl_Slider::LOG|Fl_Slider::TICK_BOTH",(void*)(Fl_Slider::HORIZONTAL|Fl_Slider::LOG|Fl_Slider::TICK_BOTH)},

  {0}};
class Fl_Slider_Type : public Fl_Widget_Type {
  const Enumeration *subtypes() const {return slider_type_menu;}
  int is_valuator() const {return 2;}
  int is_slider() const {return 1;}
public:
  virtual const char *type_name() const {return "Fl_Slider";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Slider(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Slider_Type();}
};
static Fl_Slider_Type Fl_Slider_type;

static const Enumeration scrollbar_type_menu[] = {
  {"Vertical",	"VERTICAL",	(void*)Fl_Scrollbar::VERTICAL},
  {"Horizontal","HORIZONTAL",	(void*)Fl_Scrollbar::HORIZONTAL},
  {0}};
class Fl_Scrollbar_Type : public Fl_Slider_Type {
  const Enumeration *subtypes() const {return scrollbar_type_menu;}
  int is_scrollbar() const {return 1;}
public:
  virtual const char *type_name() const {return "Fl_Scrollbar";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Scrollbar(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Scrollbar_Type();}
};
static Fl_Scrollbar_Type Fl_Scrollbar_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Output.h>
static const Enumeration output_type_menu[] = {
  {"Normal",	0,	(void*)Fl_Output::NORMAL},
  {"Multiline",	0,	(void*)Fl_Output::MULTILINE, "Fl_Multiline_Output"},
  {"Wordwrap",	0,	(void*)Fl_Output::WORDWRAP, "Fl_Wordwrap_Output"},
  {0}};
class Fl_Output_Type : public Fl_Input_Type {
  const Enumeration *subtypes() const {return output_type_menu;}
public:
  virtual const char *type_name() const {return "Fl_Output";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    Fl_Output *o = new Fl_Output(x,y,w,h,"output:");
    o->value("Text Output");
    return o;
  }
  Fl_Widget_Type *_make() {return new Fl_Output_Type();}
};
static Fl_Output_Type Fl_Output_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Value_Input.h>
class Fl_Value_Input_Type : public Fl_Widget_Type {
public:
  int is_value_input() const {return 1;}
  virtual const char *type_name() const {return "Fl_Value_Input";}
  int is_valuator() const {return 1;}
  Fl_Widget *widget(int x,int y,int w,int h) {
    Fl_Value_Input *o = new Fl_Value_Input(x,y,w,h,"value:");
    return o;
  }
  Fl_Widget_Type *_make() {return new Fl_Value_Input_Type();}
};
static Fl_Value_Input_Type Fl_Value_Input_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Value_Output.h>
class Fl_Value_Output_Type : public Fl_Widget_Type {
public:
  int is_value_output() const {return 1;}
  virtual const char *type_name() const {return "Fl_Value_Output";}
  int is_valuator() const {return 1;}
  Fl_Widget *widget(int x,int y,int w,int h) {
    Fl_Value_Output *o = new Fl_Value_Output(x,y,w,h,"value:");
    return o;
  }
  Fl_Widget_Type *_make() {return new Fl_Value_Output_Type();}
};
static Fl_Value_Output_Type Fl_Value_Output_type;

////////////////////////////////////////////////////////////////

#include <fltk/Fl_Value_Slider.h>
class Fl_Value_Slider_Type : public Fl_Slider_Type {
public:
  int is_value_slider() const {return 1;}
  virtual const char *type_name() const {return "Fl_Value_Slider";}
  Fl_Widget *widget(int x,int y,int w,int h) {
    return new Fl_Value_Slider(x,y,w,h);}
  Fl_Widget_Type *_make() {return new Fl_Value_Slider_Type();}
};
static Fl_Value_Slider_Type Fl_Value_Slider_type;

////////////////////////////////////////////////////////////////

extern class Fl_Function_Type Fl_Function_type;
extern class Fl_Code_Type Fl_Code_type;
extern class Fl_CodeBlock_Type Fl_CodeBlock_type;
extern class Fl_Decl_Type Fl_Decl_type;
extern class Fl_DeclBlock_Type Fl_DeclBlock_type;
extern class Fl_Class_Type Fl_Class_type;
extern class Fl_Window_Type Fl_Window_type;
extern class Fl_Group_Type Fl_Group_type;
extern class Fl_Pack_Type Fl_Pack_type;
extern class Fl_Tabs_Type Fl_Tabs_type;
extern class Fl_Scroll_Type Fl_Scroll_type;
extern class Fl_Tile_Type Fl_Tile_type;
extern class Fl_Choice_Type Fl_Choice_type;
extern class Fl_Menu_Bar_Type Fl_Menu_Bar_type;
extern class Fl_Menu_Button_Type Fl_Menu_Button_type;
extern class Fl_Menu_Item_Type Fl_Menu_Item_type;
extern class Fl_Menu_Divider_Type Fl_Menu_Divider_type;
extern class Fl_Submenu_Type Fl_Submenu_type;
extern class Fl_Browser_Type Fl_Browser_type;
extern class Fl_Input_Browser_Type Fl_Input_Browser_type;

extern void select(Fl_Type *,int);
extern void select_only(Fl_Type *);

static void cb(Fl_Widget *, void *v) {
  Fl_Type *t = ((Fl_Type*)v)->make();
  if (t) {select_only(t); modflag = 1; t->open();}
}

#include <fltk/Fl_Menu_Item.h>

Fl_Menu_Item New_Menu[] = {
{"code",0,0,0,FL_SUBMENU},
  {"function/method",0,cb,(void*)&Fl_Function_type},
  {"code",0,cb,(void*)&Fl_Code_type},
  {"code block",0,cb,(void*)&Fl_CodeBlock_type},
  {"declaration",0,cb,(void*)&Fl_Decl_type},
  {"declaration block",0,cb,(void*)&Fl_DeclBlock_type},
  {"class",0,cb,(void*)&Fl_Class_type},
{0},
{"group",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&Fl_Window_type},
  {0,0,cb,(void*)&Fl_Group_type},
  {0,0,cb,(void*)&Fl_Pack_type},
  {0,0,cb,(void*)&Fl_Tabs_type},
  {0,0,cb,(void*)&Fl_Scroll_type},
  {0,0,cb,(void*)&Fl_Tile_type},
{0},
{"buttons",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&Fl_Button_type},
  {0,0,cb,(void*)&Fl_Return_Button_type},
  {0,0,cb,(void*)&Fl_Light_Button_type},
  {0,0,cb,(void*)&Fl_Check_Button_type},
  {0,0,cb,(void*)&Fl_Round_Button_type},
  {0,0,cb,(void*)&Fl_Repeat_Button_type},
{0},
{"valuators",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&Fl_Slider_type},
  {0,0,cb,(void*)&Fl_Scrollbar_type},
  {0,0,cb,(void*)&Fl_Value_Slider_type},
  {0,0,cb,(void*)&Fl_Adjuster_type},
  {0,0,cb,(void*)&Fl_Counter_type},
  {0,0,cb,(void*)&Fl_Dial_type},
  {0,0,cb,(void*)&Fl_Roller_type},
  {0,0,cb,(void*)&Fl_Value_Input_type},
  {0,0,cb,(void*)&Fl_Value_Output_type},
{0},
{"text",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&Fl_Input_type},
  {0,0,cb,(void*)&Fl_Output_type},
{0},
{"menus",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&Fl_Menu_Bar_type},
  {0,0,cb,(void*)&Fl_Menu_Button_type},
  {0,0,cb,(void*)&Fl_Choice_type},
  {0,0,cb,(void*)&Fl_Browser_type},
  {0,0,cb,(void*)&Fl_Input_Browser_type},
  {0,0,cb, (void*)&Fl_Submenu_type},
  {0,0,cb, (void*)&Fl_Menu_Item_type},
  {0,0,cb, (void*)&Fl_Menu_Divider_type},
{0},
{"other",0,0,0,FL_SUBMENU},
  {0,0,cb,(void*)&Fl_Box_type},
  {0,0,cb,(void*)&Fl_Clock_type},
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
      const char *n = ((Fl_Type*)(m->user_data()))->type_name();
      if (!strncmp(n,"Fl_",3)) n += 3;
      m->text = n;
      m->callback_ = cb;
    }
  }
}

void fill_in_New_Menu() {
  fill_in_New_Menu(New_Menu);
}

// use keyword to pick the type, this is used to parse files:
int reading_file;
Fl_Type *Fl_Type_make(const char *tn, Fl_Menu_Item* menu) {
  int level = 0;
  reading_file = 1; // makes labels be null
  Fl_Type *r = 0;
  if (!strcasecmp(tn, "submenu")) tn = "Fl_Item_Group";
  else if (!strcasecmp(tn, "menuitem")) tn = "Fl_Item";
  for (unsigned i = 0; level||menu[i].user_data() || menu[i].text; i++) {
    Fl_Menu_Item *m = menu+i;
    if (m->flags & FL_SUBMENU) level++;
    if (!m->text && !m->user_data()) level--;
    if (!m->user_data()) continue;
    if(m->flags & FL_SUBMENU_POINTER) {
      if(r = Fl_Type_make(tn, (Fl_Menu_Item*) m->user_data()), r) break;
    } else {
      Fl_Type *t = (Fl_Type*)(m->user_data());
      if (!strcasecmp(tn,t->type_name())) {r = t->make(); break;}
    }
  }
  reading_file = 0;
  return r;
}

Fl_Type *Fl_Type_make(const char *tn) {
  return Fl_Type_make(tn, New_Menu);
}

#include <fltk/Fl_Browser.h>

////////////////////////////////////////////////////////////////

// Since I have included all the .h files, do this table here:

// This table is only used for back compatability with XForms fdesign files,
// there should be no need to add to it in future versions of fluid.

struct symbol {const char *name; int value;};

static symbol table[] = {
  {"BLACK",	FL_BLACK},
  {"RED",	FL_RED},
  {"GREEN",	FL_GREEN},
  {"YELLOW",	FL_YELLOW},
  {"BLUE",	FL_BLUE},
  {"MAGENTA",	FL_MAGENTA},
  {"CYAN",	FL_CYAN},
  {"WHITE",	FL_WHITE},

  {"LCOL",		FL_BLACK},
  {"COL1",		FL_GRAY},
  {"MCOL",		51},
  {"LEFT_BCOL",		55},
  {"TOP_BCOL",		53},
  {"BOTTOM_BCOL",	45},
  {"RIGHT_BCOL",	39},
  {"INACTIVE",		FL_INACTIVE_COLOR},
  {"INACTIVE_COL",	FL_INACTIVE_COLOR},
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

  {"ALIGN_CENTER",	FL_ALIGN_CENTER},
  {"ALIGN_TOP",		FL_ALIGN_TOP},
  {"ALIGN_BOTTOM",	FL_ALIGN_BOTTOM},
  {"ALIGN_LEFT",	FL_ALIGN_LEFT},
  {"ALIGN_RIGHT",	FL_ALIGN_RIGHT},
  {"ALIGN_INSIDE",	FL_ALIGN_INSIDE},
  {"ALIGN_TOP_LEFT",	 FL_ALIGN_TOP | FL_ALIGN_LEFT},
  {"ALIGN_TOP_RIGHT",	 FL_ALIGN_TOP | FL_ALIGN_RIGHT},
  {"ALIGN_BOTTOM_LEFT",	 FL_ALIGN_BOTTOM | FL_ALIGN_LEFT},
  {"ALIGN_BOTTOM_RIGHT", FL_ALIGN_BOTTOM | FL_ALIGN_RIGHT},
  {"ALIGN_CENTER|FL_ALIGN_INSIDE",	FL_ALIGN_CENTER|FL_ALIGN_INSIDE},
  {"ALIGN_TOP|FL_ALIGN_INSIDE",		FL_ALIGN_TOP|FL_ALIGN_INSIDE},
  {"ALIGN_BOTTOM|FL_ALIGN_INSIDE",	FL_ALIGN_BOTTOM|FL_ALIGN_INSIDE},
  {"ALIGN_LEFT|FL_ALIGN_INSIDE",	FL_ALIGN_LEFT|FL_ALIGN_INSIDE},
  {"ALIGN_RIGHT|FL_ALIGN_INSIDE",	FL_ALIGN_RIGHT|FL_ALIGN_INSIDE},
  {"ALIGN_INSIDE|FL_ALIGN_INSIDE",	FL_ALIGN_INSIDE|FL_ALIGN_INSIDE},
  {"ALIGN_TOP_LEFT|FL_ALIGN_INSIDE",	FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE},
  {"ALIGN_TOP_RIGHT|FL_ALIGN_INSIDE",	FL_ALIGN_TOP|FL_ALIGN_RIGHT|FL_ALIGN_INSIDE},
  {"ALIGN_BOTTOM_LEFT|FL_ALIGN_INSIDE",	FL_ALIGN_BOTTOM|FL_ALIGN_LEFT|FL_ALIGN_INSIDE},
  {"ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE",FL_ALIGN_BOTTOM|FL_ALIGN_RIGHT|FL_ALIGN_INSIDE},

  {"ALIGN_LEFT_TOP",	 FL_ALIGN_TOP | FL_ALIGN_LEFT},
  {"ALIGN_RIGHT_TOP",	 FL_ALIGN_TOP | FL_ALIGN_RIGHT},
  {"ALIGN_LEFT_BOTTOM",	 FL_ALIGN_BOTTOM | FL_ALIGN_LEFT},
  {"ALIGN_RIGHT_BOTTOM", FL_ALIGN_BOTTOM | FL_ALIGN_RIGHT},
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
  {"PUSH_BUTTON",	Fl_Button::TOGGLE},
  {"RADIO_BUTTON",	Fl_Button::RADIO},
  {"HIDDEN_BUTTON",	Fl_Button::HIDDEN},
  {"SELECT_BROWSER",	Fl_Browser::NORMAL},
  {"HOLD_BROWSER",	Fl_Browser::NORMAL},
  {"MULTI_BROWSER",	Fl_Browser::MULTI},
  {"SIMPLE_COUNTER",	Fl_Counter::SIMPLE},
  {"LINE_DIAL",		Fl_Dial::LINE},
  {"FILL_DIAL",		Fl_Dial::FILL},
  {"VERT_SLIDER",	Fl_Slider::VERTICAL},
  {"HOR_SLIDER",	Fl_Slider::HORIZONTAL},
  {"VERT_FILL_SLIDER",	Fl_Slider::VERTICAL},
  {"HOR_FILL_SLIDER",	Fl_Slider::HORIZONTAL},
  {"VERT_NICE_SLIDER",	Fl_Slider::VERTICAL},
  {"HOR_NICE_SLIDER",	Fl_Slider::HORIZONTAL},
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
// End of "$Id: factory.cxx,v 1.25 2002/05/06 06:31:26 spitzak Exp $".
//
