//
// "$Id"
//
// Fluid Menus implementation for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2006 by Bill Spitzak and others.
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
#include <fltk/MenuBuild.h>
#include "fluid_menus.h"
#include "WidgetType.h"
#include "FunctionType.h"
#include "fluid_img.h"
#include "undo.h"

using namespace fltk;

#define DECL_MENUCB(n) void n(Widget *, void *)

DECL_MENUCB(new_cb);
DECL_MENUCB(open_cb);
DECL_MENUCB(save_cb);
DECL_MENUCB(write_cb);
DECL_MENUCB(exit_cb);
DECL_MENUCB(cut_cb);
DECL_MENUCB(copy_cb);
DECL_MENUCB(paste_cb);
DECL_MENUCB(exit_cb);
DECL_MENUCB(earlier_cb);
DECL_MENUCB(later_cb);
DECL_MENUCB(group_cb);
DECL_MENUCB(ungroup_cb);
DECL_MENUCB(sort_cb);
DECL_MENUCB(show_alignment_cb);
DECL_MENUCB(show_coding_style_cb);
DECL_MENUCB(select_all_cb);
DECL_MENUCB(openwidget_cb);
DECL_MENUCB(toggle_widgetbin_cb);
DECL_MENUCB(open_history_cb);
DECL_MENUCB(theme_cb);
DECL_MENUCB(set_images_dir_cb);
DECL_MENUCB(about_cb);
DECL_MENUCB(tt_cb);
DECL_MENUCB(toggle_overlays);

//////////////////////////////////////////////////////////////////////
extern char absolute_history[][1024];
extern char relative_history[][1024];

GroupType Grouptype; // used by group_cb
static PackType Packtype;
static TabsType Tabstype;
static ScrollType Scrolltype;
static TileType Tiletype;
static WindowType Windowtype;

static BrowserType Browsertype;
static InputBrowserType InputBrowsertype;
static FileBrowserType FileBrowsertype;

static ItemType Itemtype;
static DividerType Dividertype;
static SubmenuType Submenutype;
static MenuBarType MenuBartype;
static PopupMenuType PopupMenutype;
static ChoiceType Choicetype;

static FunctionType Functiontype;
static CodeType Codetype;
static CodeBlockType CodeBlocktype;
static DeclType Decltype;
static DeclBlockType DeclBlocktype;
static CommentType Commenttype;
static ClassType Classtype;
static NamespaceType Namespacetype;

static WidgetType Widgettype;
static InvisibleBoxType InvisibleBoxtype;
static ButtonType Buttontype;
static ReturnButtonType ReturnButtontype;
static RepeatButtonType RepeatButtontype;
static LightButtonType LightButtontype;
static CheckButtonType CheckButtontype;
static RadioButtonType RadioButtontype;
static InputType Inputtype;
static DialType Dialtype;
static ClockType Clocktype;
static AdjusterType Adjustertype;
static ThumbWheelType ThumbWheeltype;
static TextEditorType TextEditortype;
static FileInputType FileInputtype;
static ProgressBarType ProgressBartype;
static SliderType Slidertype;
static ScrollbarType Scrollbartype;
static OutputType Outputtype;
static ValueInputType ValueInputtype;
static ValueOutputType ValueOutputtype;
static ValueSliderType ValueSlidertype;
static BarGroupType BarGrouptype;
static TextDisplayType TextDisplaytype;

//////////////////////////////////////////////////////////////////////
MenuBar* Main_Menu;
Item *history_item[10],*undo_item[2],*iwidget_bin,*itooltip;
ItemGroup* newMenu;

//////////////////////////////////////////////////////////////////////
static void cb(fltk::Widget *, void *v) {
  Undo::checkpoint();
  Undo::suspend();
  FluidType *t = ((FluidType*)v)->make();
  if (t) {select_only(t); modflag = 1; t->open();}
  else {
      Undo::decr();
  }
  Undo::resume();
}
//////////////////////////////////////////////////////////////////////
MenuBar* fltk::build_hierarchy(MenuBar* menubar) {
    ItemGroup* g;
    
    Main_Menu = menubar;

    menubar->begin();
    g=new ItemGroup("&File",0,0);
    new Item("New", 0, new_cb, 0);
    new Item("Open...", CTRL+'o', open_cb, 0);
    new Item("Save", CTRL+'s', save_cb, 0);
    new Item("Save As...", CTRL+'S', save_cb, (void*)1);
    new Item("Merge...", CTRL+'i', open_cb, (void*)1);
    new Divider();
    new Item("Write code", CTRL+'w', write_cb, 0);
    history_item[0] = new Item(relative_history[0], CTRL+'0', open_history_cb, absolute_history[0]);
    history_item[1] = new Item(relative_history[1], CTRL+'1', open_history_cb, absolute_history[1]);
    history_item[2] = new Item(relative_history[2], CTRL+'2', open_history_cb, absolute_history[2]);
    history_item[3] = new Item(relative_history[3], CTRL+'3', open_history_cb, absolute_history[3]);
    history_item[4] = new Item(relative_history[4], CTRL+'4', open_history_cb, absolute_history[4]);
    history_item[5] = new Item(relative_history[5], CTRL+'5', open_history_cb, absolute_history[5]);
    history_item[6] = new Item(relative_history[6], CTRL+'6', open_history_cb, absolute_history[6]);
    history_item[7] = new Item(relative_history[7], CTRL+'7', open_history_cb, absolute_history[7]);
    history_item[8] = new Item(relative_history[8], CTRL+'8', open_history_cb, absolute_history[8]);
    history_item[9] = new Item(relative_history[9], CTRL+'9', open_history_cb, absolute_history[9]);
    new Divider();
    new Item("Quit", CTRL+'q', exit_cb);
    g->end();
    
    g=new ItemGroup("&Edit",0,0);
    undo_item[0] = new Item("Undo", CTRL+'z', Undo::undo_cb,0,INACTIVE);
    undo_item[1] = new Item("Redo", CTRL+'y', Undo::redo_cb,0,INACTIVE);
    new Divider();
    new Item("Cut", CTRL+'x', cut_cb);
    new Item("Copy", CTRL+'c', copy_cb);
    new Item("Paste", CTRL+'v', paste_cb);
    new Item("Select All", CTRL+'a', select_all_cb);
    new Divider();
    new Item("Edit this widget", ReturnKey, openwidget_cb);
    new Item("Sort these widgets", 0, sort_cb);
    new Divider();
    new Item("Move widget earlier", F2Key, earlier_cb);
    new Item("Move widget later", F3Key, later_cb);
    new Divider();
    //new Item("Show", F5Key, show_cb);
    //new Item("Hide", F6Key, hide_cb);
    new Item("Group", F7Key, group_cb);
    new Item("Ungroup", F8Key, ungroup_cb,0);
    new Divider();
    //new Item("Deactivate", 0, nyi);
    //new Item("Activate", 0, nyi, 0, FL_MENU_DIVIDER);
    new Item(Item::TOGGLE,"Show Overlays",ALT+'o',toggle_overlays);
    iwidget_bin = new Item(Item::TOGGLE,"Show Widget &Bin",ALT+'b',toggle_widgetbin_cb);
    new Divider();
    new Item("Preferences",CTRL+'p',show_alignment_cb);
    new Item("Coding Style", 0, show_coding_style_cb);
    new Item("Theme", 0, theme_cb);
    new Item("Set images root directory", CTRL+'d', set_images_dir_cb);
    g->end();
    
    g=new ItemGroup("&New", 0, 0);
    fill_in_New_Menu(g);
    g->end();
    
    g=new ItemGroup("&Plugins", 0, 0, (void *)Plugins_Options_Menu);
    g->end();
    
    g=new ItemGroup("&Help",0,0);
    new Item("About fluid",0,about_cb);
    itooltip= new Item(Item::TOGGLE,"Tooltips", 0, tt_cb, 0);
    //new Item("Manual",0,nyi);
    g->end();
    menubar->end();

    return menubar;
}

//////////////////////////////////////////////////////////////////////
// All Fluid buildable widgets are accesible from the New menu:
//////////////////////////////////////////////////////////////////////
void fltk::fill_in_New_Menu(ItemGroup* menu) {
  fltk::ItemGroup* submenu;
  newMenu= menu;

    newMenu->begin();
	submenu=fluidMenuGroup("code",12);
	  fluidMenuItem(Functiontype,7);
	  //new fltk::Item((*(WidgetType*)&Functiontype).type_name(),0,cb,(void*)&Functiontype);
	  fluidMenuItem(Codetype, 8);
	  fluidMenuItem(CodeBlocktype, 9);
	  fluidMenuItem(Decltype, 10);
	  fluidMenuItem(DeclBlocktype, 11);
	  fluidMenuItem(Classtype, 12);
	  fluidMenuItem(Namespacetype, 49);
	  fluidMenuItem(Commenttype, 46);
	submenu->end();
	submenu=fluidMenuGroup("group",1);
	    fluidMenuItem(Windowtype  , 1);
	    fluidMenuItem(Grouptype  , 6);
	    fluidMenuItem(Packtype  , 22);
	    fluidMenuItem(Tabstype  , 13);
	    fluidMenuItem(Scrolltype  , 19);
	    fluidMenuItem(Tiletype  , 20);
	    // WizardType 21
	    fluidMenuItem(BarGrouptype  , 17);
	submenu->end();
	submenu=fluidMenuGroup("buttons",24);
	    fluidMenuItem(Buttontype  , 2);
	    fluidMenuItem(ReturnButtontype  , 23);
	    fluidMenuItem(LightButtontype  , 24);
	    fluidMenuItem(CheckButtontype  , 3);
	    fluidMenuItem(RadioButtontype  , 4);
	    fluidMenuItem(RepeatButtontype  , 25);
	submenu->end();
	submenu=fluidMenuGroup("valuators",37);
	    fluidMenuItem(Slidertype  , 37);
	    fluidMenuItem(ValueSlidertype  , 39);
	    fluidMenuItem(ValueInputtype  , 44);
	    fluidMenuItem(ValueOutputtype  , 45);
	    fluidMenuItem(Scrollbartype  ,38);
	    fluidMenuItem(Adjustertype  , 40);
	    fluidMenuItem(Dialtype  , 42);
	    fluidMenuItem(ThumbWheeltype  , 43);
	    fluidMenuItem(ProgressBartype  , 36);
	submenu->end();
	submenu=fluidMenuGroup("text",27);
	    fluidMenuItem(Inputtype  , 14);
	    fluidMenuItem(Outputtype  ,  27);
	    fluidMenuItem(TextDisplaytype  , 28);
	    fluidMenuItem(TextEditortype  , 29);
	    fluidMenuItem(FileInputtype  , 30);
	submenu->end();
	submenu=fluidMenuGroup("menus",17);
	    fluidMenuItem(MenuBartype  , 17);
	    fluidMenuItem(PopupMenutype  , 26);
	    fluidMenuItem(Choicetype  , 15);
	    fluidMenuItem(Browsertype  , 31);
	    fluidMenuItem(InputBrowsertype  , 15);
	    fluidMenuItem(FileBrowsertype  , 33);
	    fluidMenuItem(Submenutype  ,18);
	    fluidMenuItem(Itemtype  , 16);
	    fluidMenuItem(Dividertype  , 16);
	submenu->end();
	submenu=fluidMenuGroup("other",5);
	    fluidMenuItem(Widgettype  , 5);
	    fluidMenuItem(InvisibleBoxtype  , 5);
	    fluidMenuItem(Clocktype  , 34);
	    //helpview 35
	submenu->end();
      newMenu->end();
}
//////////////////////////////////////////////////////////////////////
Item * fltk::fluidMenuItem(FluidType& wt,int n) {
    fltk::Item * i = new fltk::Item(wt.type_name(),0,cb,(void*)&wt);
    if (n>=0) {	i->image(fluid_pixmap[n]);}
    return i;
}
//////////////////////////////////////////////////////////////////////
ItemGroup * fltk::fluidMenuGroup(const char * menu_name,int n) {
    fltk::ItemGroup * i = new fltk::ItemGroup(menu_name,0,0);
    if (n>=0) {	i->image(fluid_pixmap[n]); }
    return i;
}

//
// End of "$Id"
//
