#
# "$Id: Makefile,v 1.110 2002/12/18 20:13:15 easysw Exp $"
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
	vsnprintf.c

LIBNAME	=	../lib/$(LIBPREFIX)fltk$(LIBSUFFIX)
DSONAME	=	$(DSOPREFIX)fltk$(DSOSUFFIX)
OBJECTS =	$(CPPFILES:.cxx=.o) $(CFILES:.c=.o)


#
# Make everything...
#

all:		$(LIBTARGET) $(DSOTARGET)

static:		$(LIBNAME)

shared:		$(DSONAME)


#
# Clean old files...
#

clean:
	$(RM) *.bck
	$(RM) *.o
	$(RM) core*
	$(RM) $(LIBNAME)
	$(RM) $(DSONAME)


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

$(LIBNAME):	$(OBJECTS)
	$(RM) $@
	echo $(LIBCOMMAND) $@ ...
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

libfltk.so.2.0: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	$(RM) libfltk.so
	$(LN) libfltk.so.2.0 libfltk.so

libfltk.sl.2.0: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	$(RM) libfltk.sl
	$(LN) libfltk.sl.2.0 libfltk.sl

libfltk.2.0.dylib: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	$(RM) libfltk.dylib
	$(LN) libfltk.2.0.dylib libfltk.dylib

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
# End of "$Id: Makefile,v 1.110 2002/12/18 20:13:15 easysw Exp $".
#
