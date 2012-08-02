//
// "$Id$"
//
// Image resources code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#include "pixmaps.h"

#include "pixmaps/lock.xpm"
#include "pixmaps/protected.xpm"
#include "pixmaps/flMenuNone.xpm"
#include "pixmaps/flMenuMulti.xpm"
#include "pixmaps/flMenuAll.xpm"

fltk3::Pixmap	lock_pixmap(lock_xpm);
fltk3::Pixmap	protected_pixmap(protected_xpm);
fltk3::Pixmap	menu_none_pixmap(flMenuNone_xpm);
fltk3::Pixmap	menu_multi_pixmap(flMenuMulti_xpm);
fltk3::Pixmap	menu_all_pixmap(flMenuAll_xpm);

#include "pixmaps/flWindow.xpm"
#include "pixmaps/flButton.xpm"
#include "pixmaps/flCheckButton.xpm"
#include "pixmaps/flRoundButton.xpm"
#include "pixmaps/flBox.xpm"
#include "pixmaps/flGroup.xpm"
#include "pixmaps/flFunction.xpm"
#include "pixmaps/flCode.xpm"
#include "pixmaps/flCodeBlock.xpm"
#include "pixmaps/flComment.xpm"
#include "pixmaps/flData.xpm"
#include "pixmaps/flDeclaration.xpm"
#include "pixmaps/flDeclarationBlock.xpm"
#include "pixmaps/flClass.xpm"
#include "pixmaps/flTabs.xpm"
#include "pixmaps/flInput.xpm"
#include "pixmaps/flChoice.xpm"
#include "pixmaps/flMenuitem.xpm"
#include "pixmaps/flMenubar.xpm"
#include "pixmaps/flSubmenu.xpm"
#include "pixmaps/flScroll.xpm"
#include "pixmaps/flTile.xpm"
#include "pixmaps/flWizard.xpm"
#include "pixmaps/flPack.xpm"
#include "pixmaps/flReturnButton.xpm"
#include "pixmaps/flLightButton.xpm"
#include "pixmaps/flRepeatButton.xpm"
#include "pixmaps/flMenuButton.xpm"
#include "pixmaps/flOutput.xpm"
#include "pixmaps/flTextDisplay.xpm"
#include "pixmaps/flTextEdit.xpm"
#include "pixmaps/flFileInput.xpm"
#include "pixmaps/flBrowser.xpm"
#include "pixmaps/flCheckBrowser.xpm"
#include "pixmaps/flFileBrowser.xpm"
#include "pixmaps/flClock.xpm"
#include "pixmaps/flHelp.xpm"
#include "pixmaps/flProgress.xpm"
#include "pixmaps/flSlider.xpm"
#include "pixmaps/flScrollBar.xpm"
#include "pixmaps/flValueSlider.xpm"
#include "pixmaps/flAdjuster.xpm"
#include "pixmaps/flCounter.xpm"
#include "pixmaps/flDial.xpm"
#include "pixmaps/flRoller.xpm"
#include "pixmaps/flValueInput.xpm"
#include "pixmaps/flValueOutput.xpm"
#include "pixmaps/flSpinner.xpm"
#include "pixmaps/flWidgetClass.xpm"
#include "pixmaps/flTree.xpm"
#include "pixmaps/flTable.xpm"
#include "pixmaps/flAppTarget.xpm"
#include "pixmaps/flLibTarget.xpm"
#include "pixmaps/flFile.xpm"
#include "pixmaps/flFolder.xpm"
#include "pixmaps/flFluidFile.xpm"
#include "pixmaps/flCodeFile.xpm"
#include "pixmaps/flWorkspace.xpm"
#include "pixmaps/flDependency.xpm"
#include "pixmaps/flOption.xpm"

static fltk3::Pixmap	window_pixmap(flWindow_xpm);
static fltk3::Pixmap	button_pixmap(flButton_xpm);
static fltk3::Pixmap	checkbutton_pixmap(flCheckButton_xpm);
static fltk3::Pixmap	roundbutton_pixmap(flRoundButton_xpm);
static fltk3::Pixmap	box_pixmap(flBox_xpm);
static fltk3::Pixmap	group_pixmap(flGroup_xpm);
static fltk3::Pixmap	function_pixmap(flFunction_xpm);
static fltk3::Pixmap	code_pixmap(flCode_xpm);
static fltk3::Pixmap	codeblock_pixmap(flCodeBlock_xpm);
static fltk3::Pixmap	comment_pixmap(flComment_xpm);
static fltk3::Pixmap	declaration_pixmap(flDeclaration_xpm);
static fltk3::Pixmap	declarationblock_pixmap(flDeclarationBlock_xpm);
static fltk3::Pixmap	class_pixmap(flClass_xpm);
static fltk3::Pixmap	tabs_pixmap(flTabs_xpm);
static fltk3::Pixmap	input_pixmap(flInput_xpm);
static fltk3::Pixmap	choice_pixmap(flChoice_xpm);
static fltk3::Pixmap	menuitem_pixmap(flMenuitem_xpm);
static fltk3::Pixmap	menubar_pixmap(flMenubar_xpm);
static fltk3::Pixmap	submenu_pixmap(flSubmenu_xpm);
static fltk3::Pixmap	scroll_pixmap(flScroll_xpm);
static fltk3::Pixmap	tile_pixmap(flTile_xpm);
static fltk3::Pixmap	wizard_pixmap(flWizard_xpm);
static fltk3::Pixmap	pack_pixmap(flPack_xpm);
static fltk3::Pixmap	returnbutton_pixmap(flReturnButton_xpm);
static fltk3::Pixmap	lightbutton_pixmap(flLightButton_xpm);
static fltk3::Pixmap	repeatbutton_pixmap(flRepeatButton_xpm);
static fltk3::Pixmap	menubutton_pixmap(flMenuButton_xpm);
static fltk3::Pixmap	output_pixmap(flOutput_xpm);
static fltk3::Pixmap	textdisplay_pixmap(flTextDisplay_xpm);
static fltk3::Pixmap	textedit_pixmap(flTextEdit_xpm);
static fltk3::Pixmap	fileinput_pixmap(flFileInput_xpm);
static fltk3::Pixmap	browser_pixmap(flBrowser_xpm);
static fltk3::Pixmap	checkbrowser_pixmap(flCheckBrowser_xpm);
static fltk3::Pixmap	filebrowser_pixmap(flFileBrowser_xpm);
static fltk3::Pixmap	clock_pixmap(flClock_xpm);
static fltk3::Pixmap	help_pixmap(flHelp_xpm);
static fltk3::Pixmap	progress_pixmap(flProgress_xpm);
static fltk3::Pixmap	slider_pixmap(flSlider_xpm);
static fltk3::Pixmap	scrollbar_pixmap(flScrollBar_xpm);
static fltk3::Pixmap	valueslider_pixmap(flValueSlider_xpm);
static fltk3::Pixmap	adjuster_pixmap(flAdjuster_xpm);
static fltk3::Pixmap	counter_pixmap(flCounter_xpm);
static fltk3::Pixmap	dial_pixmap(flDial_xpm);
static fltk3::Pixmap	roller_pixmap(flRoller_xpm);
static fltk3::Pixmap	valueinput_pixmap(flValueInput_xpm);
static fltk3::Pixmap	valueoutput_pixmap(flValueOutput_xpm);
static fltk3::Pixmap	spinner_pixmap(flSpinner_xpm);
static fltk3::Pixmap	widgetclass_pixmap(flWidgetClass_xpm);
static fltk3::Pixmap	data_pixmap(flData_xpm);
static fltk3::Pixmap	tree_pixmap(flTree_xpm);
static fltk3::Pixmap	table_pixmap(flTable_xpm);
static fltk3::Pixmap	app_target_pixmap(flAppTarget_xpm);
static fltk3::Pixmap	lib_target_pixmap(flLibTarget_xpm);
static fltk3::Pixmap	file_pixmap(flFile_xpm);
static fltk3::Pixmap	folder_pixmap(flFolder_xpm);
static fltk3::Pixmap	fluid_file_pixmap(flFluidFile_xpm);
static fltk3::Pixmap	code_file_pixmap(flCodeFile_xpm);
static fltk3::Pixmap	workspace_pixmap(flWorkspace_xpm);
static fltk3::Pixmap	dependency_pixmap(flDependency_xpm);
static fltk3::Pixmap	option_pixmap(flOption_xpm);

fltk3::Pixmap *pixmap[] = { 0, &window_pixmap, &button_pixmap, &checkbutton_pixmap, &roundbutton_pixmap, /* 0..4 */
 &box_pixmap, &group_pixmap, &function_pixmap, &code_pixmap, &codeblock_pixmap, &declaration_pixmap, /* 5..10 */ 
 &declarationblock_pixmap, &class_pixmap, &tabs_pixmap, &input_pixmap, &choice_pixmap,               /* 11..15 */
 &menuitem_pixmap, &menubar_pixmap, &submenu_pixmap, &scroll_pixmap, &tile_pixmap, &wizard_pixmap,   /* 16..21 */
 &pack_pixmap, &returnbutton_pixmap, &lightbutton_pixmap, &repeatbutton_pixmap, &menubutton_pixmap,  /* 22..26 */
 &output_pixmap, &textdisplay_pixmap, &textedit_pixmap, &fileinput_pixmap, &browser_pixmap,          /* 27..32 */
 &checkbrowser_pixmap, &filebrowser_pixmap, &clock_pixmap, &help_pixmap, &progress_pixmap,	     /* 33..36 */
 &slider_pixmap, &scrollbar_pixmap, &valueslider_pixmap, &adjuster_pixmap, &counter_pixmap,          /* 37..41 */
 &dial_pixmap, &roller_pixmap, &valueinput_pixmap, &valueoutput_pixmap, &comment_pixmap,             /* 42..46 */
 &spinner_pixmap, &widgetclass_pixmap, &data_pixmap, &tree_pixmap, &table_pixmap,                    /* 47..51 */
 &app_target_pixmap, &file_pixmap, &folder_pixmap, &fluid_file_pixmap, &code_file_pixmap,            /* 52..56 */
 &lib_target_pixmap, &workspace_pixmap, &dependency_pixmap, &option_pixmap};                         /* 57..60 */


//
// End of "$Id$".
//
