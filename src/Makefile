#
# "$Id: Makefile,v 1.91 2001/02/28 21:19:50 clip Exp $"
#
# Library makefile for the Fast Light Tool Kit (FLTK).
#
# Copyright 1998 by Bill Spitzak and others.
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
# Please report all bugs and problems to "fltk-bugs@easysw.com".
#

CPPFILES = \
	Fl.cxx \
	Fl_Adjuster.cxx \
	Fl_Align_Group.cxx \
	Fl_Bitmap.cxx \
	fl_bmp.cxx \
	Fl_Browser.cxx \
	Fl_Box.cxx \
	Fl_Button.cxx \
	Fl_Check_Button.cxx \
	Fl_Choice.cxx \
	Fl_Clock.cxx \
	Fl_Color_Chooser.cxx \
	Fl_Counter.cxx \
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
	Fl_Output.cxx \
	Fl_Overlay_Window.cxx \
	Fl_Pack.cxx \
	Fl_Pixmap.cxx \
	Fl_Repeat_Button.cxx \
	Fl_Return_Button.cxx \
	Fl_RGB_Image.cxx \
	Fl_Roller.cxx \
	Fl_Scroll.cxx \
	Fl_Scrollbar.cxx \
	Fl_Shaped_Window.cxx \
	Fl_Single_Window.cxx \
	Fl_Slider.cxx \
	Fl_String_List.cxx \
	Fl_Style.cxx \
	Fl_Style_start.cxx \
	Fl_Tabs.cxx \
	Fl_Text_Buffer.cxx \
	Fl_Text_Display.cxx \
	Fl_Text_Editor.cxx \
	Fl_Tile.cxx \
	Fl_Tooltip.cxx  \
	Fl_Valuator.cxx \
	Fl_Value_Input.cxx \
	Fl_Value_Output.cxx \
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
	fl_boxtype.cxx \
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
	fl_xpm.cxx \
	fl_options.cxx

CFILES = \
	scandir.c \
	numericsort.c \
	vsnprintf.c \
	conf.c \
	conf_endtrim.c \
	conf_get.c \
	conf_keys.c \
	conf_strcasecmp.c \
	conf_strcnt.c \
	conf_list_free.c \
	conf_util.c \
	conf_del.c \
	conf_error.c \
	conf_getboolean.c \
	conf_getlong.c \
	conf_sections.c \
	conf_set.c \
	conf_setboolean.c \
	conf_setlong.c

# files that were deleted:

#	Fl_theme.cxx \
#	fl_background.cxx \
#	fl_set_font.cxx \
#	fl_set_fonts.cxx \
#	Fl_Threads.cxx \
#	Fl_Style_Plugins.cxx \
#	Fl_Style_Util.cxx \
#	Fl_Adjuster_SW.cxx \
#	Fl_Browser_SW.cxx \
#	Fl_Button_SW.cxx \
#	Fl_Chart_SW.cxx \
#	Fl_Choice_SW.cxx \
#	Fl_Counter_SW.cxx \
#	Fl_Input_SW.cxx \
#	Fl_Menu_Item_SW.cxx \
#	Fl_Menu_Button_SW.cxx \
#	Fl_Menu_SW.cxx \
#	Fl_Slider_SW.cxx \
#	Fl_Widget_SW.cxx \

################################################################

XTRAFLAGS = -DFL_LIBRARY

include ../makeinclude

.c.o:
	$(CC) -I.. $(XTRAFLAGS) $(CFLAGS) $< -c

.cxx.o:
	$(CXX) -I.. $(XTRAFLAGS) $(CXXFLAGS) $< -c

%.shared.o : %.cxx
	$(CXX) -I.. $(XTRAFLAGS) $(DSOCFLAGS) $(CXXFLAGS) -c $< -o $@
%.shared.o : %.c
	$(CC) -I.. $(XTRAFLAGS) $(DSOCFLAGS) $(CFLAGS) -c $< -o $@

LIBRARY = ../lib/$(LIBNAME)
DSO = ../lib/$(DSOLIBNAME)
DSONAME = $(DSOLIBNAME)$(DSOEXT)

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.c=.o)

all: static programs

static: $(LIBRARY)$(LIBEXT)

shared: $(DSO)$(DSOEXT)

programs:

$(LIBRARY).a: $(OBJECTS) fl_call_main.o
	@rm -f $@
	$(LIBCOMMAND) $@ $(OBJECTS) fl_call_main.o
	$(RANLIB) $@

$(DSO).so.2 $(DSO).sl.2: $(OBJECTS:.o=.shared.o)
	rm -f $@
	$(DSOCOMMAND) $(DSONAME) $(OBJECTS:.o=.shared.o) $(DSOLIBS)
	@mv $(DSONAME) $@

DLLIMPORTNAME = $(LIBNAME).2.dll$(LIBEXT)
$(DSO).2.dll: $(OBJECTS:.o=.shared.o)
	@-rm -f $@
	$(DSOCOMMAND) $@ $(OBJECTS:.o=.shared.o) $(DSOLIBS)
	-mv $@.import ../lib/$(DLLIMPORTNAME)

clean :
	@rm -f *.o $(LIBRARY)$(LIBEXT) $(DSO)$(DSOEXT) ../lib/$(DLLIMPORTNAME) \
	       core *~ ../include/*~ makedepend cmap
	@touch makedepend

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) $(CFILES) > makedepend
include makedepend

################################################################

install: install_static install_shared

install_headers:
	@echo "Installing include files..."
	@mkdir -p $(includedir)
	@rm -rf $(includedir)/FL $(includedir)/Fl
	@mkdir -p $(includedir)/FL
	@cp ../FL/*.[Hh]  $(includedir)/FL
	@-chmod 755 $(includedir)/FL
	@-chmod 644 $(includedir)/FL/*
	@if [ ! "$(SYSTYPE)" = "WIN32" ]; then for file in $(includedir)/FL/*.H; do\
		newfile="`basename $$file H`h";\
		ln -s $$file $(includedir)/FL/$$newfile;\
	done; fi
	@if [ ! "$(SYSTYPE)" = "WIN32" ]; then ln -sf FL $(includedir)/Fl; fi

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
# End of "$Id: Makefile,v 1.91 2001/02/28 21:19:50 clip Exp $".
#
