#
# "$Id: Makefile,v 1.115 2003/01/19 09:32:15 spitzak Exp $"
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
	$(MAKEDEPEND) -fmakedepend -I.. $(CPPFILES) $(CFILES)


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

install: install-lib install-lib$(DSOSUFFIX)
	echo "Installing include files in $(includedir)/fltk..."
	$(RM) -r $(includedir)/fltk
	$(MKDIR) $(includedir)/fltk
	$(CP) ../fltk/*.h  $(includedir)/fltk
	$(CHMOD) 755 $(includedir)/fltk
	$(CHMOD) 644 $(includedir)/fltk/*
	echo "Installing FLTK1.1 emulation include files in $(includedir)/FL..."
	$(RM) -r $(includedir)/FL
	$(MKDIR) $(includedir)/FL
	$(CP) ../FL/*.H ../FL/*.h  $(includedir)/FL
	$(CHMOD) 755 $(includedir)/FL
	$(CHMOD) 644 $(includedir)/FL/*
	echo "Installing fltk-config in $(bindir)..."
	$(MKDIR) $(bindir)
	$(CP) ../fltk-config $(bindir)
	$(CHMOD) 755 $(bindir)/fltk-config


install-lib:
	echo "Installing static core library in $(libdir)"
	$(MKDIR) $(libdir)
	$(RM) $(libdir)/$(LIBPREFIX)fltk$(LIBSUFFIX)
	$(CP) ../lib/$(LIBPREFIX)fltk$(LIBSUFFIX) $(libdir)
	$(RANLIB) $(libdir)/$(LIBPREFIX)fltk$(LIBSUFFIX)

install-lib.so.2:
	echo "Installing shared core library in $(libdir)"
	$(MKDIR) $(libdir)
	$(RM) $(libdir)/libfltk.so.2
	$(CP) ../lib/libfltk.so.2 $(libdir)
	$(RM) $(libdir)/libfltk.so
	$(LN) libfltk.so.2 $(libdir)/libfltk.so

install-lib.sl.2:
	echo "Installing shared core library in $(libdir)"
	$(MKDIR) $(libdir)
	$(RM) $(libdir)/libfltk.sl.2
	$(CP) ../lib/libfltk.sl.2 $(libdir)
	$(RM) $(libdir)/libfltk.sl
	$(LN) libfltk.sl.2 $(libdir)/libfltk.sl

install-lib.2.dylib:
	echo "Installing shared core library in $(libdir)"
	$(MKDIR) $(libdir)
	$(RM) $(libdir)/libfltk.2.dylib
	$(CP) ../lib/libfltk.2.dylib $(libdir)
	$(RM) $(libdir)/libfltk.dylib
	$(LN) libfltk.2.dylib $(libdir)/libfltk.dylib

install-lib_s.a:
	echo "Installing shared core library in $(libdir)"
	$(MKDIR) $(libdir)
	$(RM) $(libdir)/libfltk_s.a
	$(CP) ../lib/libfltk_s.a $(libdir)
	$(CHMOD) 755 $(libdir)/libfltk_s.a


#
# Uninstall the libraries...
#

uninstall:	uninstall-lib uninstall-lib$(DSOSUFFIX)
	echo "Removing FLTK1.1 emulation include files from $(includedir)/FL..."
	$(RM) -r $(includedir)/FL
	echo "Removing include files from $(includedir)/fltk..."
	$(RM) -r $(includedir)/fltk
	echo "Removing fltk-config from $(bindir)..."
	$(RM) $(bindir)/fltk-config

uninstall-lib:
	echo "Removing static core library from $(libdir)"
	$(RM) $(libdir)/$(LIBPREFIX)fltk$(LIBSUFFIX)

uninstall-lib.so.2:
	echo "Removing shared core library from $(libdir)"
	$(RM) $(libdir)/libfltk.so.2
	$(RM) $(libdir)/libfltk.so

uninstall-lib.sl.2:
	echo "Removing shared core library from $(libdir)"
	$(RM) $(libdir)/libfltk.sl.2
	$(RM) $(libdir)/libfltk.sl

uninstall-lib.2.dylib:
	echo "Removing shared core library from $(libdir)"
	$(RM) $(libdir)/libfltk.2.dylib
	$(RM) $(libdir)/libfltk.dylib

uninstall-lib_s.a:
	echo "Removing shared core library from $(libdir)"
	$(RM) $(libdir)/libfltk_s.a


#
# End of "$Id: Makefile,v 1.115 2003/01/19 09:32:15 spitzak Exp $".
#
