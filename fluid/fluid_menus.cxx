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
#include <fltk/DoubleBufferWindow.h>
#include "fluid_menus.h"
#include "WidgetType.h"
#include "FunctionType.h"
#include "fluid_img.h"
#include "undo.h"

using namespace fltk;

DECL_MENUCBV(new_cb);
DECL_MENUCBV(open_cb);
DECL_MENUCBV(save_cb);
DECL_MENUCBV(save_template_cb);
DECL_MENUCBV(revert_cb);
DECL_MENUCBV(write_cb);
DECL_MENUCBV(exit_cb);
DECL_MENUCBV(cut_cb);
DECL_MENUCBV(copy_cb);
DECL_MENUCBV(paste_cb);
DECL_MENUCBV(exit_cb);
DECL_MENUCBV(earlier_cb);
DECL_MENUCBV(later_cb);
DECL_MENUCBV(group_cb);
DECL_MENUCBV(ungroup_cb);
DECL_MENUCBV(sort_cb);
DECL_MENUCBV(show_preferences_cb);
DECL_MENUCBV(show_coding_style_cb);
DECL_MENUCBV(select_all_cb);
DECL_MENUCBV(select_none_cb);
DECL_MENUCBV(openwidget_cb);
DECL_MENUCBV(toggle_widgetbin_cb);
DECL_MENUCBV(open_history_cb);
DECL_MENUCBV(theme_cb);
DECL_MENUCBV(set_images_dir_cb);
DECL_MENUCBV(about_cb);
DECL_MENUCBV(tt_cb);
DECL_MENUCBV(toggle_overlays);
DECL_MENUCBV2(toggle_sourceview_cb,DoubleBufferWindow);
DECL_MENUCBV(toggle_statusbar_cb);
DECL_MENUCBV(show_shell_window);
DECL_MENUCBV2(do_shell_command,ReturnButton);
DECL_MENUCBV(help_cb);
DECL_MENUCBV(manual_cb);
DECL_MENUCBL(align_widget_cb);
DECL_MENUCBL2(default_widget_size_cb, RadioButton);
DECL_MENUCBL(widget_size_cb);

//////////////////////////////////////////////////////////////////////

GroupType Grouptype; // used by group_cb
static PackType Packtype;
static TabsType Tabstype;
static WizardGroupType WizardGrouptype;
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
static StatusBarGroupType StatusBarGrouptype;
static TextDisplayType TextDisplaytype;

//////////////////////////////////////////////////////////////////////
MenuBar* Main_Menu;
Item *history_item[10],*undo_item[2],*iwidget_bin,
    *itooltip,*isource_view,*istatusbar;
ItemGroup* newMenu;

//////////////////////////////////////////////////////////////////////
static void cb(fltk::Widget *, void *v) {
  Undo::checkpoint();
  Undo::suspend();
  FluidType *t = ((FluidType*)v)->make();
  if (t) {select_only(t); modflag = 1; t->open();}
  else {
      Undo::remove_last();
  }
  Undo::resume();
}
//////////////////////////////////////////////////////////////////////
MenuBar* fltk::build_hierarchy(MenuBar* menubar) {
    Main_Menu = menubar;
    menubar->begin();

    {MenuSection g("&File");
	new Item("&New", COMMAND+'n', new_cb, 0);//)->image(new_pixmap);
	new Item("&Open...", COMMAND+'o', open_cb, 0);//)->image(open_pixmap);
	new Item("&Save", COMMAND+'s', save_cb, 0); //)->image(save_pixmap);
	new Item("S&ave As", SHIFT+COMMAND+'S', save_cb, (void*)1);
	new Item("Sa&ve a Copy", ACCELERATOR+COMMAND+'S', save_cb, (void*)2);
	new Item("Save &Template", COMMAND+'T', save_template_cb, (void*)1);
	new Divider();
	new Item("&Revert", COMMAND+'R', revert_cb, (void*)1);
	//new Item("&Print", COMMAND+'P', print_menu_cb, (void*)1); // TODO: add nice printing code
	new Item("&Merge...", COMMAND+'i', open_cb, (void*)1);//)->image(merge_pixmap);
	new Item("&Write code", COMMAND+'w', write_cb, 0);//)->image(write_pixmap);
	new Divider();
	for (int h=0; h<MAX_HISTORY; h++)
	    history_item[h] = new Item(relative_history[h], COMMAND+('0'+h), open_history_cb, absolute_history[h]);
	new Divider();
	new Item("&Quit", COMMAND+'q', exit_cb);
    }
    {MenuSection g("&Edit");
	undo_item[0] = new Item("U&ndo", COMMAND+'z', Undo::undo_cb,0,INACTIVE);
	undo_item[1] = new Item("&Redo", SHIFT+COMMAND+'Z', Undo::redo_cb,0,INACTIVE);
	new Divider();
	new Item("&Cut", COMMAND+'x', cut_cb);
	new Item("C&opy", COMMAND+'c', copy_cb);
	new Item("&Paste", COMMAND+'v', paste_cb);
	new Item("Select &All", COMMAND+'a', select_all_cb);
	new Item("Select &None", SHIFT+COMMAND+'a', select_none_cb);
	new Divider();
	new Item("Ed&it this widget", ReturnKey, openwidget_cb);
	new Item("&Sort these widgets", 0, sort_cb);
	new Divider();
	new Item("Move widget &earlier", F2Key, earlier_cb);
	new Item("Move widget &later", F3Key, later_cb);
	new Divider();
	//new Item("Show", F5Key, show_cb);
	//new Item("Hide", F6Key, hide_cb);
	new Item("&Group", F7Key, group_cb);
	new Item("&Ungroup", F8Key, ungroup_cb,0);
	new Divider();
	//new Item("Deactivate", 0, nyi);
	//new Item("Activate", 0, nyi, 0, FL_MENU_DIVIDER);
	{MenuSection g("&Show / Hide"); 
	    new Item(Item::TOGGLE,"Show Overlays",ACCELERATOR+'o',toggle_overlays);
	    iwidget_bin = new Item(Item::TOGGLE,"Show Widget &Bin",ACCELERATOR+'b',toggle_widgetbin_cb);
	    isource_view = new Item(Item::TOGGLE,"Show Source Code",ACCELERATOR+COMMAND+'s',(Callback*) toggle_sourceview_cb);
	    istatusbar = new Item(Item::TOGGLE,"Show Status bar",ACCELERATOR+COMMAND+'b',(Callback*) toggle_statusbar_cb);
	}
	//new Divider();
	new Item("&Preferences",COMMAND+'p',show_preferences_cb);
	new Item("Coding St&yle", 0, show_coding_style_cb);
	new Item("T&heme", 0, theme_cb);
	new Item("Set i&mages root directory", COMMAND+'d', set_images_dir_cb);
    }    
    {MenuSection g("&New"); fill_in_New_Menu(g.group()); }
    {MenuSection g("&Plugins", 0, 0, (void *)Plugins_Options_Menu);}
    {MenuSection g("&Layout");
      {MenuSection sg("&Align");
	new Item("&Left",0,(Callback *)align_widget_cb,(void*)10);
	new Item("&Center",0,(Callback *)align_widget_cb,(void*)11);
	new Item("&Right",0,(Callback *)align_widget_cb,(void*)12);
	new Item("&Top",0,(Callback *)align_widget_cb,(void*)13);
	new Item("&Middle",0,(Callback *)align_widget_cb,(void*)14);
	new Item("&Bottom",0,(Callback *)align_widget_cb,(void*)15);
      }
      {MenuSection sg("&Space Evenly");
	new Item("&Across",0,(Callback *)align_widget_cb,(void*)20);
	new Item("&Down",0,(Callback *)align_widget_cb,(void*)21);
      }
      {MenuSection sg("&Make Same Size");
	new Item("&Width",0,(Callback *)align_widget_cb,(void*)30);
	new Item("&Height",0,(Callback *)align_widget_cb,(void*)31);
	new Item("&Both",0,(Callback *)align_widget_cb,(void*)32);
      }
      {MenuSection sg("&Center In Group");
	new Item("&Horizontal",0,(Callback *)align_widget_cb,(void*)40);
	new Item("&Vertical",0,(Callback *)align_widget_cb,(void*)41);
      }
      {MenuSection sg("Set &Widget Size");
	new Item("&Tiny",ACCELERATOR+'1',(Callback *)widget_size_cb,(void*)8,0,NORMAL_LABEL,HELVETICA,8);
	new Item("&Small",ACCELERATOR+'2',(Callback *)widget_size_cb,(void*)11,0,NORMAL_LABEL,HELVETICA,11);
	new Item("&Normal",ACCELERATOR+'3',(Callback *)widget_size_cb,(void*)14,0,NORMAL_LABEL,HELVETICA,14);
	new Item("&Medium",ACCELERATOR+'4',(Callback *)widget_size_cb,(void*)18,0,NORMAL_LABEL,HELVETICA,18);
	new Item("&Large",ACCELERATOR+'5',(Callback *)widget_size_cb,(void*)24,0,NORMAL_LABEL,HELVETICA,24);
	new Item("&Huge",ACCELERATOR+'6',(Callback *)widget_size_cb,(void*)32,0,NORMAL_LABEL,HELVETICA,32);
      }
      new Divider();
      new Item("&Grid and Size Settings...",COMMAND+'g',show_preferences_cb,(void*)2);
    }
    {MenuSection g("&Shell");
	new Item("Execute &Command",ACCELERATOR+'x',show_shell_window);
	new Item("Execute &Again",ACCELERATOR+'g', (Callback*)do_shell_command);
    }
    {MenuSection g("&Help");
	new Item("&About fluid",0,about_cb);
	new Divider();
	new Item("&On fluid...",0,help_cb);
	new Item("&FLTK Manual...",0,manual_cb);
	new Divider();
	itooltip= new Item(Item::TOGGLE,"&Tooltips", 0, tt_cb, 0);
        //new Item("Manual",0,nyi);
    }
    menubar->end();

    return menubar;
}

//////////////////////////////////////////////////////////////////////
// All Fluid buildable widgets are accesible from the New menu:
//////////////////////////////////////////////////////////////////////
void fltk::fill_in_New_Menu(ItemGroup* menu) {
  newMenu= menu;
  xpmImage** i = fluid_pixmap;
    newMenu->begin();
	{MenuSection g("code",i[12]);
	  fluidMenuItem(Functiontype,7);
	  fluidMenuItem(Codetype, 8);
	  fluidMenuItem(CodeBlocktype, 9);
	  fluidMenuItem(Decltype, 10);
	  fluidMenuItem(DeclBlocktype, 11);
	  fluidMenuItem(Classtype, 12);
	  fluidMenuItem(Namespacetype, 49);
	  fluidMenuItem(Commenttype, 46);
	}
	{MenuSection g("group",i[1]);
	    fluidMenuItem(Windowtype  , 1);
	    fluidMenuItem(Grouptype  , 6);
	    fluidMenuItem(Packtype  , 22);
	    fluidMenuItem(Tabstype  , 13);
	    fluidMenuItem(Scrolltype  , 19);
	    fluidMenuItem(Tiletype  , 20);
	    fluidMenuItem(WizardGrouptype  , 21);
	    fluidMenuItem(BarGrouptype  , 17);
	    fluidMenuItem(StatusBarGrouptype  , 5);
	}
	{MenuSection g("buttons",i[24]);
	    fluidMenuItem(Buttontype  , 2);
	    fluidMenuItem(ReturnButtontype  , 23);
	    fluidMenuItem(LightButtontype  , 24);
	    fluidMenuItem(CheckButtontype  , 3);
	    fluidMenuItem(RadioButtontype  , 4);
	    fluidMenuItem(RepeatButtontype  , 25);
	}
	{MenuSection g("valuators",i[37]);
	    fluidMenuItem(Slidertype  , 37);
	    fluidMenuItem(ValueSlidertype  , 39);
	    fluidMenuItem(ValueInputtype  , 44);
	    fluidMenuItem(ValueOutputtype  , 45);
	    fluidMenuItem(Scrollbartype  ,38);
	    fluidMenuItem(Adjustertype  , 40);
	    fluidMenuItem(Dialtype  , 42);
	    fluidMenuItem(ThumbWheeltype  , 43);
	    fluidMenuItem(ProgressBartype  , 36);
	}
	{MenuSection g("text",i[27]);
	    fluidMenuItem(Inputtype  , 14);
	    fluidMenuItem(Outputtype  ,  27);
	    fluidMenuItem(TextDisplaytype  , 28);
	    fluidMenuItem(TextEditortype  , 29);
	    fluidMenuItem(FileInputtype  , 30);
	}
	{MenuSection g("menus",i[17]);
	    fluidMenuItem(MenuBartype  , 17);
	    fluidMenuItem(PopupMenutype  , 26);
	    fluidMenuItem(Choicetype  , 15);
	    fluidMenuItem(Browsertype  , 31);
	    fluidMenuItem(InputBrowsertype  , 15);
	    fluidMenuItem(FileBrowsertype  , 33);
	    fluidMenuItem(Submenutype  ,18);
	    fluidMenuItem(Itemtype  , 16);
	    fluidMenuItem(Dividertype  , 16);
	}
	{MenuSection g("other",i[5]);
	    fluidMenuItem(Widgettype  , 5);
	    fluidMenuItem(InvisibleBoxtype  , 5);
	    fluidMenuItem(Clocktype  , 34);
	    //helpview 35
	}
      newMenu->end();
}
//////////////////////////////////////////////////////////////////////
Item * fltk::fluidMenuItem(FluidType& wt,int n) {
  return new Item(wt.type_name(),n>=0 ?fluid_pixmap[n] : (xpmImage*)0,0,cb,(void*)&wt);
}
//////////////////////////////////////////////////////////////////////

//
// End of "$Id"
//
