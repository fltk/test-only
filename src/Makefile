#
# "$Id: Makefile,v 1.112 2003/01/05 07:40:29 spitzak Exp $"
#
# Library makefile for the Fast Light Tool Kit (FLTK).
#
# Copyright 1998-2003 by Bill Spitzak and others.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.
#
# Please report all bugs and problems to "fltk-bugs@fltk.org".
#

#
# Include common definitions...
#

include ../makeinclude


#
# Files for this directory...
#

CPPFILES = \
	Fl.cxx \
	Fl_Adjuster.cxx \
	Fl_Align_Group.cxx \
	Fl_Bar.cxx \
	Fl_Bitmap.cxx \
	fl_bmp.cxx \
	Fl_Browser.cxx \
	Fl_Box.cxx \
	Fl_Boxtype.cxx \
	Fl_Button.cxx \
	Fl_Check_Button.cxx \
	Fl_Choice.cxx \
	Fl_Clock.cxx \
	Fl_Color_Chooser.cxx \
	Fl_Dial.cxx \
	Fl_Double_Window.cxx \
	Fl_FileBrowser.cxx \
	Fl_FileChooser.cxx \
	Fl_FileChooser2.cxx \
	Fl_FileIcon.cxx \
	Fl_FileIcon2.cxx \
	Fl_FileInput.cxx \
	Fl_Float_Input.cxx \
	fl_gif.cxx \
	Fl_Group.cxx \
	Fl_Highlight_Button.cxx \
	Fl_Image.cxx \
	Fl_Input.cxx \
	Fl_Input_Browser.cxx \
	Fl_Item.cxx \
	Fl_Light_Button.cxx \
	Fl_Menu.cxx \
	Fl_Menu_.cxx \
	Fl_Menu_add.cxx \
	Fl_Menu_Bar.cxx \
	Fl_Menu_Button.cxx \
	Fl_Menu_Item.cxx \
	Fl_Menu_Window.cxx \
	Fl_Menu_global.cxx \
	Fl_Multi_Image.cxx \
	Fl_Numeric_Input.cxx \
	Fl_Output.cxx \
	Fl_Overlay_Window.cxx \
	Fl_Pack.cxx \
	Fl_Pixmap.cxx \
	Fl_ProgressBar.cxx \
	Fl_Radio_Button.cxx \
	Fl_Repeat_Button.cxx \
	Fl_Return_Button.cxx \
	Fl_RGB_Image.cxx \
	Fl_Roller.cxx \
	Fl_Scroll.cxx \
	Fl_Scrollbar.cxx \
	Fl_Shaped_Window.cxx \
	Fl_Slider.cxx \
	Fl_String_List.cxx \
	Fl_Style.cxx \
	Fl_Style_Set.cxx \
	Fl_Tabs.cxx \
	Fl_Text_Buffer.cxx \
	Fl_Text_Display.cxx \
	Fl_Text_Editor.cxx \
	Fl_Tile.cxx \
	Fl_Tiled_Image.cxx \
	Fl_Tooltip.cxx  \
	Fl_Valuator.cxx \
	Fl_Value_Input.cxx \
	ValueOutput.cxx \
	Fl_Value_Slider.cxx \
	Fl_Widget.cxx \
	Fl_Window.cxx \
	Fl_Window_fullscreen.cxx \
	Fl_Window_hotspot.cxx \
	Fl_Window_iconize.cxx \
	Fl_abort.cxx \
	Fl_add_idle.cxx \
	Fl_arg.cxx \
	Fl_compose.cxx \
	Fl_display.cxx \
	Fl_get_key.cxx \
	Fl_key_name.cxx \
	Fl_lock.cxx \
	Fl_own_colormap.cxx \
	Fl_visual.cxx \
	filename_absolute.cxx \
	filename_expand.cxx \
	filename_ext.cxx \
	filename_isdir.cxx \
	filename_list.cxx \
	filename_match.cxx \
	filename_setext.cxx \
	fl_arc.cxx \
	fl_arci.cxx \
	fl_ask.cxx \
	fl_clip.cxx \
	fl_color.cxx \
	fl_cursor.cxx \
	fl_curve.cxx \
	fl_diamond_box.cxx \
	fl_dnd.cxx \
	fl_draw.cxx \
	fl_draw_image.cxx \
	fl_draw_pixmap.cxx \
	fl_engraved_label.cxx \
	fl_file_chooser.cxx \
	fl_font.cxx \
	fl_glyph.cxx \
	fl_labeltype.cxx \
	fl_list_fonts.cxx \
	fl_load_plugin.cxx \
	fl_oval_box.cxx \
	fl_overlay.cxx \
	fl_overlay_visual.cxx \
	fl_plastic_box.cxx \
	fl_rect.cxx \
	fl_round_box.cxx \
	fl_rounded_box.cxx \
	fl_rgb.cxx \
	fl_scroll_area.cxx \
	Fl_Shared_Image.cxx \
	fl_shadow_box.cxx \
	fl_show_colormap.cxx \
	fl_symbols.cxx \
	fl_vertex.cxx \
	fltk_theme.cxx

CFILES	= \
	scandir.c \
	string.c \
	vsnprintf.c

LIBNAME	=	$(LIBPREFIX)fltk$(LIBSUFFIX)
DSONAME	=	$(DSOPREFIX)fltk$(DSOSUFFIX)
OBJECTS =	$(CPPFILES:.cxx=.o) $(CFILES:.c=.o)


#
# Make everything...
#

all:		$(LIBTARGET) $(DSOTARGET)

static:		../lib/$(LIBNAME)

shared:		../lib/$(DSONAME)


#
# Clean old files...
#

clean:
	$(RM) *.bck
	$(RM) *.o
	$(RM) core*
	$(RM) ../lib/$(LIBNAME)
	$(RM) ../lib/$(DSONAME)


#
# Make dependencies, excluding standard include directories...
#

depend:
	$(MAKEDEPEND) -I.. $(CPPFILES) $(CFILES) > makedepend


#
# Include automatically generated dependencies...
#

include makedepend


#
# Make static libraries...
#

../lib/$(LIBNAME): $(OBJECTS)
	$(RM) $@
	echo $(LIBCOMMAND) $@ ...
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

../lib/$(DSONAME): $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	$(RM) ../lib/$(DSOPREFIX)fltk$(DSOSYMLINK)
	$(LN) $(DSONAME) ../lib/$(DSOPREFIX)fltk$(DSOSYMLINK)

libfltk_s.a: $(OBJECTS)
	echo $(DSOCOMMAND) libfltk_s.o ...
	$(DSOCOMMAND) libfltk_s.o $(OBJECTS) $(IMAGELIBS)
	echo $(LIBCOMMAND) libfltk_s.a libfltk_s.o
	$(RM) $@
	$(LIBCOMMAND) libfltk_s.a libfltk_s.o
	$(CHMOD) +x libfltk_s.a


#
# Install everything...
#

install: $(INSTALLTARGETS)

install_headers:
	@echo "Installing include files..."
	@mkdir -p $(includedir)
	@rm -rf $(includedir)/fltk $(includedir)/fltk
	@mkdir -p $(includedir)/fltk
	@cp ../fltk/*.h  $(includedir)/fltk
	@-chmod 755 $(includedir)/fltk
	@-chmod 644 $(includedir)/fltk/*

install_fltk_config:
	@echo "Installing fltk-config..."
	@mkdir -p $(bindir)
	@cp ../fltk-config $(bindir)

install_static: static install_headers install_fltk_config
	@echo "Installing static libraries..."
	@mkdir -p $(libdir)
	@rm -f $(libdir)/$(LIBNAME)$(LIBEXT)
	@cp $(LIBRARY)$(LIBEXT) $(libdir)
	@-chmod 644 $(libdir)/$(LIBNAME)$(LIBEXT)

install_shared: shared
	@echo "Installing shared libraries..."
	@rm -f $(libdir)/$(DLLIMPORTNAME)
	@rm -f $(libdir)/$(DSOLIBNAME)$(DSOEXT)
	@if [ "$(DSOSHORTEXT)" ]; then rm -f $(libdir)/$(DSOLIBNAME)$(DSOSHORTEXT); fi
	@cp $(DSO)$(DSOEXT) $(libdir)
	@-chmod 755 $(libdir)/$(DSOLIBNAME)$(DSOEXT)
	@if [ "$(DSOSHORTEXT)" ]; then \
		ln -s $(DSOLIBNAME)$(DSOEXT) $(libdir)/$(DSOLIBNAME)$(DSOSHORTEXT); fi
	@if [ -f "../lib/$(DLLIMPORTNAME)" ]; then cp ../lib/$(DLLIMPORTNAME) $(libdir); fi

install_programs:


#
# End of "$Id: Makefile,v 1.112 2003/01/05 07:40:29 spitzak Exp $".
#
# DO NOT DELETE

Fl.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/events.h ../fltk/error.h
Fl.o: ../fltk/damage.h ../fltk/layout.h ../fltk/Window.h ../fltk/Group.h
Fl.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
Fl.o: ../fltk/Style.h ../fltk/Tooltip.h ../fltk/x.h ../config.h
Fl.o: ../fltk/FL_VERSION.h ../fltk/ScreenInfo.h Fl_x.cxx
Fl_Adjuster.o: ../fltk/Adjuster.h ../fltk/Valuator.h ../fltk/Widget.h
Fl_Adjuster.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Adjuster.o: ../fltk/Flags.h ../fltk/Box.h ../fltk/events.h
Fl_Adjuster.o: ../fltk/damage.h ../fltk/draw.h ../fltk/xbmImage.h
Fl_Adjuster.o: ../fltk/Image.h fastarrow.h mediumarrow.h slowarrow.h
Fl_Align_Group.o: ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h
Fl_Align_Group.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/AlignGroup.h
Fl_Align_Group.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
Fl_Bar.o: ../fltk/BarGroup.h ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
Fl_Bar.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Box.h
Fl_Bar.o: ../fltk/events.h ../fltk/damage.h
Fl_Bitmap.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/x.h ../fltk/Color.h
Fl_Bitmap.o: ../fltk/Flags.h ../fltk/draw.h ../fltk/xbmImage.h
Fl_Bitmap.o: ../fltk/Image.h
fl_bmp.o: ../fltk/SharedImage.h ../fltk/Image.h ../fltk/FL_API.h
fl_bmp.o: ../fltk/Flags.h ../fltk/events.h ../fltk/error.h ../fltk/draw.h
fl_bmp.o: ../fltk/Color.h ../fltk/x.h
Fl_Browser.o: ../fltk/Browser.h ../fltk/Menu.h ../fltk/Group.h
Fl_Browser.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_Browser.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Scrollbar.h
Fl_Browser.o: ../fltk/Slider.h ../fltk/Valuator.h ../fltk/events.h
Fl_Browser.o: ../fltk/damage.h ../fltk/layout.h ../fltk/Box.h ../fltk/draw.h
Fl_Browser.o: ../fltk/error.h ../fltk/xbmImage.h ../fltk/Image.h
Fl_Browser.o: browser_plus.xbm browser_minus.xbm
Fl_Box.o: ../fltk/InvisibleBox.h ../fltk/Widget.h ../fltk/Style.h
Fl_Box.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Boxtype.o: ../fltk/Box.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Boxtype.o: ../fltk/Style.h ../fltk/Widget.h ../fltk/Style.h ../fltk/draw.h
Fl_Boxtype.o: ../fltk/x.h ../config.h
Fl_Button.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/damage.h
Fl_Button.o: ../fltk/Button.h ../fltk/Widget.h ../fltk/Style.h
Fl_Button.o: ../fltk/Color.h ../fltk/Flags.h ../fltk/Group.h ../fltk/Box.h
Fl_Button.o: ../fltk/draw.h
Fl_Check_Button.o: ../fltk/CheckButton.h ../fltk/Button.h ../fltk/Widget.h
Fl_Check_Button.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Check_Button.o: ../fltk/Flags.h ../fltk/Group.h ../fltk/Box.h
Fl_Check_Button.o: ../fltk/draw.h
Fl_Choice.o: ../fltk/Choice.h ../fltk/Menu.h ../fltk/Group.h ../fltk/Widget.h
Fl_Choice.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Choice.o: ../fltk/events.h ../fltk/damage.h ../fltk/Box.h ../fltk/draw.h
Fl_Clock.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/damage.h
Fl_Clock.o: ../fltk/Clock.h ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_Clock.o: ../fltk/Flags.h ../fltk/draw.h
Fl_Color_Chooser.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/damage.h
Fl_Color_Chooser.o: ../fltk/ColorChooser.h ../fltk/Group.h ../fltk/Widget.h
Fl_Color_Chooser.o: ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
Fl_Color_Chooser.o: ../fltk/Choice.h ../fltk/Menu.h ../fltk/Group.h
Fl_Color_Chooser.o: ../fltk/ValueInput.h ../fltk/Valuator.h
Fl_Color_Chooser.o: ../fltk/FloatInput.h ../fltk/NumericInput.h
Fl_Color_Chooser.o: ../fltk/Input.h ../fltk/Box.h ../fltk/draw.h
Fl_Color_Chooser.o: ../fltk/Item.h ../fltk/math.h ../fltk/Window.h
Fl_Color_Chooser.o: ../fltk/ReturnButton.h ../fltk/Button.h ../fltk/ask.h
Fl_Dial.o: ../fltk/Dial.h ../fltk/Valuator.h ../fltk/Widget.h ../fltk/Style.h
Fl_Dial.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h ../fltk/events.h
Fl_Dial.o: ../fltk/damage.h ../fltk/Box.h ../fltk/draw.h ../fltk/math.h
Fl_Double_Window.o: ../config.h ../fltk/damage.h ../fltk/layout.h
Fl_Double_Window.o: ../fltk/DoubleBufferWindow.h ../fltk/Window.h
Fl_Double_Window.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
Fl_Double_Window.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Double_Window.o: ../fltk/x.h ../fltk/draw.h
Fl_FileBrowser.o: ../fltk/FileBrowser.h ../fltk/Browser.h ../fltk/Menu.h
Fl_FileBrowser.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
Fl_FileBrowser.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_FileBrowser.o: ../fltk/Scrollbar.h ../fltk/Slider.h ../fltk/Valuator.h
Fl_FileBrowser.o: ../fltk/FileIcon.h ../fltk/draw.h ../fltk/filename.h
Fl_FileBrowser.o: ../fltk/string.h ../config.h
Fl_FileChooser.o: ../fltk/FileChooser.h ../fltk/Window.h ../fltk/Group.h
Fl_FileChooser.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_FileChooser.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Choice.h
Fl_FileChooser.o: ../fltk/Menu.h ../fltk/Button.h ../fltk/filename.h
Fl_FileChooser.o: ../fltk/FileBrowser.h ../fltk/Browser.h ../fltk/Scrollbar.h
Fl_FileChooser.o: ../fltk/Slider.h ../fltk/Valuator.h ../fltk/FileIcon.h
Fl_FileChooser.o: ../fltk/FileInput.h ../fltk/Input.h ../fltk/ReturnButton.h
Fl_FileChooser.o: ../fltk/Button.h ../fltk/string.h ../fltk/xbmImage.h
Fl_FileChooser.o: ../fltk/Image.h
Fl_FileChooser2.o: ../fltk/FileChooser.h ../fltk/Window.h ../fltk/Group.h
Fl_FileChooser2.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_FileChooser2.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Choice.h
Fl_FileChooser2.o: ../fltk/Menu.h ../fltk/Button.h ../fltk/filename.h
Fl_FileChooser2.o: ../fltk/FileBrowser.h ../fltk/Browser.h
Fl_FileChooser2.o: ../fltk/Scrollbar.h ../fltk/Slider.h ../fltk/Valuator.h
Fl_FileChooser2.o: ../fltk/FileIcon.h ../fltk/FileInput.h ../fltk/Input.h
Fl_FileChooser2.o: ../fltk/ReturnButton.h ../fltk/Button.h ../fltk/filename.h
Fl_FileChooser2.o: ../fltk/ask.h ../fltk/string.h ../fltk/events.h
Fl_FileChooser2.o: ../config.h
Fl_FileIcon.o: ../fltk/FileIcon.h ../fltk/FL_API.h ../fltk/Color.h
Fl_FileIcon.o: ../fltk/Flags.h ../fltk/Widget.h ../fltk/Style.h
Fl_FileIcon.o: ../fltk/draw.h ../fltk/filename.h
Fl_FileIcon2.o: ../fltk/FileIcon.h ../fltk/FL_API.h ../fltk/Color.h
Fl_FileIcon2.o: ../fltk/Flags.h ../fltk/filename.h ../fltk/draw.h ../config.h
Fl_FileInput.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/FileInput.h
Fl_FileInput.o: ../fltk/Input.h
Fl_Float_Input.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/Widget.h
Fl_Float_Input.o: ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
Fl_Float_Input.o: ../fltk/FloatInput.h ../fltk/NumericInput.h ../fltk/Input.h
fl_gif.o: ../fltk/SharedImage.h ../fltk/Image.h ../fltk/FL_API.h
fl_gif.o: ../fltk/Flags.h ../fltk/draw.h ../fltk/Color.h ../fltk/x.h
Fl_Group.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_Group.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Box.h ../fltk/draw.h
Fl_Group.o: ../fltk/events.h ../fltk/layout.h ../fltk/damage.h
Fl_Highlight_Button.o: ../fltk/events.h ../fltk/FL_API.h
Fl_Highlight_Button.o: ../fltk/HighlightButton.h ../fltk/Button.h
Fl_Highlight_Button.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_Highlight_Button.o: ../fltk/Flags.h
Fl_Image.o: ../fltk/Image.h ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Widget.h
Fl_Image.o: ../fltk/Style.h ../fltk/Color.h ../fltk/events.h ../fltk/draw.h
Fl_Image.o: ../fltk/x.h
Fl_Input.o: ../fltk/Input.h ../fltk/events.h ../fltk/FL_API.h
Fl_Input.o: ../fltk/damage.h ../fltk/Box.h ../fltk/Color.h ../fltk/Flags.h
Fl_Input.o: ../fltk/draw.h ../fltk/math.h
Fl_Input_Browser.o: ../fltk/InputBrowser.h ../fltk/Menu.h ../fltk/Group.h
Fl_Input_Browser.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_Input_Browser.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Input.h
Fl_Input_Browser.o: ../fltk/MenuWindow.h ../fltk/Window.h ../fltk/Browser.h
Fl_Input_Browser.o: ../fltk/Menu.h ../fltk/Scrollbar.h ../fltk/Slider.h
Fl_Input_Browser.o: ../fltk/Valuator.h ../fltk/ScreenInfo.h ../fltk/events.h
Fl_Input_Browser.o: ../fltk/damage.h ../fltk/Box.h ../fltk/draw.h
Fl_Item.o: ../fltk/Item.h ../fltk/Image.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Item.o: ../fltk/CheckButton.h ../fltk/Button.h ../fltk/Widget.h
Fl_Item.o: ../fltk/Style.h ../fltk/Color.h ../fltk/Box.h ../fltk/draw.h
Fl_Item.o: ../fltk/ItemGroup.h ../fltk/Menu.h ../fltk/Group.h
Fl_Item.o: ../fltk/Divider.h
Fl_Light_Button.o: ../fltk/LightButton.h ../fltk/CheckButton.h
Fl_Light_Button.o: ../fltk/Button.h ../fltk/Widget.h ../fltk/Style.h
Fl_Light_Button.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Light_Button.o: ../fltk/Box.h
Fl_Menu.o: ../fltk/MenuWindow.h ../fltk/Window.h ../fltk/Group.h
Fl_Menu.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Menu.o: ../fltk/Flags.h ../fltk/Menu.h ../fltk/Box.h ../fltk/LabelType.h
Fl_Menu.o: ../fltk/draw.h ../fltk/events.h ../fltk/damage.h ../fltk/run.h
Fl_Menu.o: ../fltk/ScreenInfo.h ../fltk/Item.h
Fl_Menu_.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/Menu.h ../fltk/Group.h
Fl_Menu_.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
Fl_Menu_.o: ../fltk/Item.h
Fl_Menu_add.o: ../fltk/Menu.h ../fltk/Group.h ../fltk/Widget.h
Fl_Menu_add.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Menu_add.o: ../fltk/Flags.h ../fltk/Item.h ../fltk/ItemGroup.h
Fl_Menu_add.o: ../fltk/Menu.h ../fltk/Divider.h ARRAY.h
Fl_Menu_Bar.o: ../fltk/MenuBar.h ../fltk/Menu.h ../fltk/Group.h
Fl_Menu_Bar.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_Menu_Bar.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/events.h
Fl_Menu_Bar.o: ../fltk/damage.h ../fltk/Box.h ../fltk/draw.h
Fl_Menu_Bar.o: ../fltk/Tooltip.h ../config.h ../fltk/Item.h
Fl_Menu_Button.o: ../fltk/PopupMenu.h ../fltk/Menu.h ../fltk/Group.h
Fl_Menu_Button.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_Menu_Button.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/events.h
Fl_Menu_Button.o: ../fltk/damage.h ../fltk/Box.h ../fltk/draw.h
Fl_Menu_Button.o: ../fltk/Item.h
Fl_Menu_Item.o: ../FL/Fl_Menu_Item.h ../fltk/Widget.h ../fltk/Style.h
Fl_Menu_Item.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Menu_Item.o: ../fltk/Menu.h ../fltk/Group.h ../fltk/Widget.h
Fl_Menu_Item.o: ../fltk/Item.h ../fltk/ItemGroup.h ../fltk/Menu.h
Fl_Menu_Item.o: ../fltk/Divider.h ../fltk/PopupMenu.h ../fltk/events.h
Fl_Menu_Window.o: ../fltk/damage.h ../fltk/MenuWindow.h ../fltk/Window.h
Fl_Menu_Window.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
Fl_Menu_Window.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Menu_Window.o: ../fltk/x.h ../fltk/draw.h ../config.h
Fl_Menu_global.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/Menu.h
Fl_Menu_global.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
Fl_Menu_global.o: ../fltk/Color.h ../fltk/Flags.h ../fltk/Window.h
Fl_Multi_Image.o: ../fltk/MultiImage.h ../fltk/Image.h ../fltk/FL_API.h
Fl_Multi_Image.o: ../fltk/Flags.h
Fl_Numeric_Input.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/NumericInput.h
Fl_Numeric_Input.o: ../fltk/Input.h
Fl_Output.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/Output.h
Fl_Output.o: ../fltk/Input.h ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h
Fl_Overlay_Window.o: ../fltk/OverlayWindow.h ../fltk/DoubleBufferWindow.h
Fl_Overlay_Window.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
Fl_Overlay_Window.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Overlay_Window.o: ../fltk/Flags.h ../fltk/damage.h ../fltk/draw.h
Fl_Overlay_Window.o: ../fltk/x.h ../config.h
Fl_Pack.o: ../fltk/PackedGroup.h ../fltk/Group.h ../fltk/Widget.h
Fl_Pack.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Pack.o: ../fltk/layout.h ../fltk/Box.h
Fl_Pixmap.o: ../fltk/xpmImage.h ../fltk/Image.h ../fltk/FL_API.h
Fl_Pixmap.o: ../fltk/Flags.h ../fltk/draw.h ../fltk/Color.h ../fltk/x.h
Fl_ProgressBar.o: ../fltk/ProgressBar.h ../fltk/Widget.h ../fltk/Style.h
Fl_ProgressBar.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_ProgressBar.o: ../fltk/damage.h ../fltk/Box.h ../fltk/draw.h
Fl_Radio_Button.o: ../fltk/RadioButton.h ../fltk/CheckButton.h
Fl_Radio_Button.o: ../fltk/Button.h ../fltk/Widget.h ../fltk/Style.h
Fl_Radio_Button.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Radio_Button.o: ../fltk/draw.h ../fltk/Box.h
Fl_Repeat_Button.o: ../fltk/RepeatButton.h ../fltk/Button.h ../fltk/Widget.h
Fl_Repeat_Button.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Repeat_Button.o: ../fltk/Flags.h ../fltk/events.h
Fl_Return_Button.o: ../fltk/ReturnButton.h ../fltk/Button.h ../fltk/Widget.h
Fl_Return_Button.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Return_Button.o: ../fltk/Flags.h ../fltk/events.h ../fltk/draw.h
Fl_RGB_Image.o: ../fltk/rgbImage.h ../fltk/Image.h ../fltk/FL_API.h
Fl_RGB_Image.o: ../fltk/Flags.h ../fltk/events.h ../fltk/draw.h
Fl_RGB_Image.o: ../fltk/Color.h ../fltk/x.h
Fl_Roller.o: ../fltk/ThumbWheel.h ../fltk/Valuator.h ../fltk/Widget.h
Fl_Roller.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Roller.o: ../fltk/events.h ../fltk/damage.h ../fltk/Box.h ../fltk/draw.h
Fl_Roller.o: ../fltk/math.h
Fl_Scroll.o: ../fltk/ScrollGroup.h ../fltk/Group.h ../fltk/Widget.h
Fl_Scroll.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Scroll.o: ../fltk/Scrollbar.h ../fltk/Slider.h ../fltk/Valuator.h
Fl_Scroll.o: ../fltk/events.h ../fltk/layout.h ../fltk/damage.h ../fltk/Box.h
Fl_Scroll.o: ../fltk/draw.h
Fl_Scrollbar.o: ../fltk/Scrollbar.h ../fltk/Slider.h ../fltk/Valuator.h
Fl_Scrollbar.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_Scrollbar.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/events.h
Fl_Scrollbar.o: ../fltk/damage.h ../fltk/Box.h ../fltk/draw.h
Fl_Shaped_Window.o: ../fltk/ShapedWindow.h ../fltk/DoubleBufferWindow.h
Fl_Shaped_Window.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
Fl_Shaped_Window.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Shaped_Window.o: ../fltk/Flags.h ../fltk/xbmImage.h ../fltk/Image.h
Fl_Shaped_Window.o: ../fltk/x.h
Fl_Slider.o: ../fltk/Slider.h ../fltk/Valuator.h ../fltk/Widget.h
Fl_Slider.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Slider.o: ../fltk/events.h ../fltk/damage.h ../fltk/Box.h ../fltk/draw.h
Fl_Slider.o: ../fltk/math.h ../config.h
Fl_String_List.o: ../fltk/StringList.h ../fltk/Menu.h ../fltk/Group.h
Fl_String_List.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_String_List.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Item.h
Fl_Style.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Style.o: ../fltk/Flags.h ../fltk/load_plugin.h ../fltk/string.h
Fl_Style.o: ../config.h ../fltk/math.h
Fl_Style_Set.o: ../fltk/StyleSet.h ../fltk/Style.h ../fltk/Color.h
Fl_Style_Set.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Widget.h
Fl_Tabs.o: ../fltk/TabGroup.h ../fltk/Group.h ../fltk/Widget.h
Fl_Tabs.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Tabs.o: ../fltk/events.h ../fltk/damage.h ../fltk/layout.h ../fltk/Box.h
Fl_Tabs.o: ../fltk/draw.h ../fltk/Tooltip.h
Fl_Text_Buffer.o: ../fltk/TextBuffer.h ../fltk/FL_API.h
Fl_Text_Display.o: ../fltk/TextBuffer.h ../fltk/FL_API.h
Fl_Text_Display.o: ../fltk/TextDisplay.h ../fltk/Group.h ../fltk/Widget.h
Fl_Text_Display.o: ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
Fl_Text_Display.o: ../fltk/Scrollbar.h ../fltk/Slider.h ../fltk/Valuator.h
Fl_Text_Display.o: ../fltk/TextBuffer.h ../fltk/events.h ../fltk/damage.h
Fl_Text_Display.o: ../fltk/Box.h ../fltk/draw.h
Fl_Text_Editor.o: ../fltk/TextEditor.h ../fltk/TextDisplay.h ../fltk/Group.h
Fl_Text_Editor.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_Text_Editor.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Scrollbar.h
Fl_Text_Editor.o: ../fltk/Slider.h ../fltk/Valuator.h ../fltk/TextBuffer.h
Fl_Text_Editor.o: ../fltk/Style.h ../fltk/events.h
Fl_Tile.o: ../fltk/TiledGroup.h ../fltk/Group.h ../fltk/Widget.h
Fl_Tile.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Tile.o: ../fltk/Window.h ../fltk/events.h ../fltk/layout.h
Fl_Tile.o: ../fltk/Cursor.h
Fl_Tiled_Image.o: ../fltk/TiledImage.h ../fltk/Image.h ../fltk/FL_API.h
Fl_Tiled_Image.o: ../fltk/Flags.h ../fltk/draw.h ../fltk/Color.h
Fl_Tooltip.o: ../fltk/Tooltip.h ../fltk/Widget.h ../fltk/Style.h
Fl_Tooltip.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Tooltip.o: ../fltk/MenuWindow.h ../fltk/Window.h ../fltk/Group.h
Fl_Tooltip.o: ../fltk/ScreenInfo.h ../fltk/events.h ../fltk/run.h
Fl_Tooltip.o: ../fltk/draw.h
Fl_Valuator.o: ../fltk/Valuator.h ../fltk/Widget.h ../fltk/Style.h
Fl_Valuator.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Valuator.o: ../fltk/events.h ../fltk/damage.h ../fltk/math.h
Fl_Value_Input.o: ../fltk/ValueInput.h ../fltk/Valuator.h ../fltk/Widget.h
Fl_Value_Input.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Value_Input.o: ../fltk/Flags.h ../fltk/FloatInput.h ../fltk/NumericInput.h
Fl_Value_Input.o: ../fltk/Input.h ../fltk/Group.h ../fltk/events.h
Fl_Value_Input.o: ../fltk/damage.h ../fltk/Box.h
ValueOutput.o: ../fltk/ValueOutput.h ../fltk/Valuator.h ../fltk/Widget.h
ValueOutput.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
ValueOutput.o: ../fltk/Flags.h ../fltk/events.h ../fltk/damage.h
ValueOutput.o: ../fltk/draw.h ../fltk/Box.h ../fltk/run.h
Fl_Value_Slider.o: ../fltk/ValueSlider.h ../fltk/Slider.h ../fltk/Valuator.h
Fl_Value_Slider.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_Value_Slider.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/damage.h
Fl_Value_Slider.o: ../fltk/draw.h ../fltk/Box.h
Fl_Widget.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
Fl_Widget.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/events.h
Fl_Widget.o: ../fltk/damage.h ../fltk/layout.h ../fltk/Group.h
Fl_Widget.o: ../fltk/Widget.h ../fltk/run.h ../config.h
Fl_Window.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
Fl_Window.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
Fl_Window.o: ../fltk/events.h ../fltk/damage.h ../fltk/layout.h ../fltk/run.h
Fl_Window.o: ../fltk/x.h ../config.h
Fl_Window_fullscreen.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
Fl_Window_fullscreen.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Window_fullscreen.o: ../fltk/Flags.h ../fltk/ScreenInfo.h ../fltk/events.h
Fl_Window_fullscreen.o: ../fltk/x.h
Fl_Window_hotspot.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/Window.h
Fl_Window_hotspot.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
Fl_Window_hotspot.o: ../fltk/Color.h ../fltk/Flags.h ../fltk/ScreenInfo.h
Fl_Window_iconize.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
Fl_Window_iconize.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
Fl_Window_iconize.o: ../fltk/Flags.h ../fltk/x.h
Fl_abort.o: ../fltk/error.h ../fltk/FL_API.h ../fltk/string.h
Fl_add_idle.o: ../fltk/run.h ../fltk/FL_API.h
Fl_arg.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Window.h ../fltk/Group.h
Fl_arg.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
Fl_arg.o: ../fltk/filename.h ../fltk/error.h ../fltk/ScreenInfo.h ../fltk/x.h
Fl_compose.o: ../fltk/events.h ../fltk/FL_API.h
Fl_display.o: ../fltk/run.h ../fltk/FL_API.h
Fl_get_key.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/x.h ../fltk/Color.h
Fl_get_key.o: ../fltk/Flags.h
Fl_key_name.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/Widget.h
Fl_key_name.o: ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h ../fltk/draw.h
Fl_key_name.o: ../fltk/x.h
Fl_lock.o: ../fltk/run.h ../fltk/FL_API.h ../config.h
Fl_own_colormap.o: ../fltk/visual.h ../fltk/FL_API.h ../fltk/x.h
Fl_own_colormap.o: ../fltk/Color.h ../fltk/Flags.h ../config.h
Fl_visual.o: ../fltk/visual.h ../fltk/FL_API.h ../fltk/x.h ../fltk/Color.h
Fl_visual.o: ../fltk/Flags.h ../config.h
filename_absolute.o: ../fltk/filename.h ../fltk/FL_API.h
filename_expand.o: ../fltk/filename.h ../fltk/FL_API.h
filename_ext.o: ../fltk/filename.h ../fltk/FL_API.h
filename_isdir.o: ../config.h ../fltk/filename.h ../fltk/FL_API.h
filename_list.o: ../config.h ../fltk/filename.h ../fltk/FL_API.h
filename_match.o: ../fltk/filename.h ../fltk/FL_API.h
filename_setext.o: ../fltk/filename.h ../fltk/FL_API.h
fl_arc.o: ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h ../fltk/FL_API.h
fl_arc.o: ../fltk/math.h
fl_arci.o: ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h ../fltk/FL_API.h
fl_arci.o: ../fltk/math.h ../fltk/x.h
fl_ask.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/draw.h ../fltk/Flags.h
fl_ask.o: ../fltk/Color.h ../fltk/ask.h ../fltk/Style.h ../fltk/Button.h
fl_ask.o: ../fltk/Widget.h ../fltk/ReturnButton.h ../fltk/Button.h
fl_ask.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Input.h
fl_ask.o: ../fltk/SecretInput.h ../fltk/Input.h ../fltk/string.h
fl_clip.o: ../fltk/x.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
fl_clip.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
fl_clip.o: ../fltk/draw.h
fl_color.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h ../fltk/draw.h
fl_color.o: ../fltk/Color.h ../config.h ../fltk/x.h fl_cmap.h fl_color_x.cxx
fl_color.o: XColorMap.h
fl_cursor.o: ../fltk/Cursor.h ../fltk/FL_API.h ../fltk/Window.h
fl_cursor.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
fl_cursor.o: ../fltk/Flags.h ../fltk/x.h ../fltk/draw.h ../fltk/Color.h
fl_curve.o: ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h ../fltk/FL_API.h
fl_curve.o: ../fltk/math.h
fl_diamond_box.o: ../fltk/Box.h ../fltk/Color.h ../fltk/FL_API.h
fl_diamond_box.o: ../fltk/Flags.h ../fltk/Style.h ../fltk/Widget.h
fl_diamond_box.o: ../fltk/Style.h ../fltk/draw.h
fl_dnd.o: fl_dnd_x.cxx ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
fl_dnd.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
fl_dnd.o: ../fltk/run.h ../fltk/events.h ../fltk/Cursor.h ../fltk/x.h
fl_draw.o: ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h ../fltk/FL_API.h
fl_draw.o: ../fltk/math.h ../fltk/Widget.h ../fltk/Style.h ../fltk/Style.h
fl_draw_image.o: ../config.h ../fltk/events.h ../fltk/FL_API.h ../fltk/draw.h
fl_draw_image.o: ../fltk/Flags.h ../fltk/Color.h ../fltk/x.h
fl_draw_image.o: fl_draw_image_x.cxx ../fltk/error.h XColorMap.h
fl_draw_pixmap.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
fl_draw_pixmap.o: ../fltk/draw.h ../fltk/Color.h ../config.h
fl_engraved_label.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
fl_engraved_label.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/LabelType.h
fl_engraved_label.o: ../fltk/draw.h
fl_file_chooser.o: ../config.h ../fltk/file_chooser.h ../fltk/FL_API.h
fl_file_chooser.o: ../fltk/FileChooser.h ../fltk/Window.h ../fltk/Group.h
fl_file_chooser.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
fl_file_chooser.o: ../fltk/Flags.h ../fltk/Choice.h ../fltk/Menu.h
fl_file_chooser.o: ../fltk/Button.h ../fltk/filename.h ../fltk/FileBrowser.h
fl_file_chooser.o: ../fltk/Browser.h ../fltk/Scrollbar.h ../fltk/Slider.h
fl_file_chooser.o: ../fltk/Valuator.h ../fltk/FileIcon.h ../fltk/FileInput.h
fl_file_chooser.o: ../fltk/Input.h ../fltk/ReturnButton.h ../fltk/Button.h
fl_font.o: ../config.h ../fltk/Font.h ../fltk/Style.h ../fltk/Color.h
fl_font.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/draw.h fl_font_x.cxx
fl_font.o: ../fltk/error.h ../fltk/x.h ../fltk/math.h IFont.h
fl_glyph.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
fl_glyph.o: ../fltk/Flags.h ../fltk/Box.h ../fltk/draw.h
fl_labeltype.o: ../fltk/LabelType.h ../fltk/Color.h ../fltk/FL_API.h
fl_labeltype.o: ../fltk/Flags.h ../fltk/Box.h ../fltk/Image.h
fl_labeltype.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Window.h
fl_labeltype.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/damage.h
fl_labeltype.o: ../fltk/draw.h ../config.h
fl_list_fonts.o: ../config.h ../fltk/Font.h ../fltk/Style.h ../fltk/Color.h
fl_list_fonts.o: ../fltk/FL_API.h ../fltk/Flags.h fl_list_fonts_x.cxx
fl_list_fonts.o: ../fltk/x.h IFont.h
fl_load_plugin.o: ../fltk/load_plugin.h ../fltk/FL_API.h ../config.h
fl_oval_box.o: ../fltk/Box.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
fl_oval_box.o: ../fltk/Style.h ../fltk/draw.h
fl_overlay.o: ../fltk/x.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
fl_overlay.o: ../fltk/draw.h
fl_overlay_visual.o: ../config.h
fl_plastic_box.o: ../fltk/Box.h ../fltk/Color.h ../fltk/FL_API.h
fl_plastic_box.o: ../fltk/Flags.h ../fltk/Style.h ../fltk/draw.h
fl_rect.o: ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h ../fltk/FL_API.h
fl_rect.o: ../fltk/x.h
fl_round_box.o: ../fltk/Box.h ../fltk/Color.h ../fltk/FL_API.h
fl_round_box.o: ../fltk/Flags.h ../fltk/Style.h ../fltk/draw.h
fl_rounded_box.o: ../fltk/Box.h ../fltk/Color.h ../fltk/FL_API.h
fl_rounded_box.o: ../fltk/Flags.h ../fltk/Style.h ../fltk/draw.h
fl_rgb.o: ../fltk/x.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
fl_rgb.o: ../fltk/Color.h
fl_scroll_area.o: ../fltk/x.h ../fltk/Color.h ../fltk/FL_API.h
fl_scroll_area.o: ../fltk/Flags.h ../fltk/draw.h
Fl_Shared_Image.o: ../config.h ../fltk/events.h ../fltk/FL_API.h
Fl_Shared_Image.o: ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h
Fl_Shared_Image.o: ../fltk/SharedImage.h ../fltk/Image.h ../fltk/xbmImage.h
Fl_Shared_Image.o: ../fltk/x.h
fl_shadow_box.o: ../fltk/Box.h ../fltk/Color.h ../fltk/FL_API.h
fl_shadow_box.o: ../fltk/Flags.h ../fltk/Style.h ../fltk/draw.h
fl_show_colormap.o: ../fltk/show_colormap.h ../fltk/FL_API.h ../fltk/Color.h
fl_show_colormap.o: ../fltk/Flags.h ../fltk/MenuWindow.h ../fltk/Window.h
fl_show_colormap.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
fl_show_colormap.o: ../fltk/ScreenInfo.h ../fltk/Box.h ../fltk/draw.h
fl_show_colormap.o: ../fltk/events.h ../fltk/damage.h
fl_symbols.o: ../fltk/Style.h ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h
fl_symbols.o: ../fltk/FL_API.h ../fltk/events.h
fl_vertex.o: ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h ../fltk/FL_API.h
fl_vertex.o: ../fltk/math.h ../fltk/x.h
fltk_theme.o: ../fltk/Style.h
scandir.o: ../config.h
string.o: ../fltk/string.h ../fltk/FL_API.h
vsnprintf.o: ../config.h ../fltk/string.h ../fltk/FL_API.h
