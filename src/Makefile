#
# "$Id: Makefile,v 1.76 2000/06/03 05:32:19 carl Exp $"
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
	fl_gif.cxx \
	Fl_Group.cxx \
	Fl_Highlight_Button.cxx \
	Fl_Image.cxx \
	Fl_Input.cxx \
	Fl_Input_.cxx \
	Fl_Input_Style.cxx \
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
	Fl_cutpaste.cxx \
	Fl_display.cxx \
	Fl_get_key.cxx \
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
	fl_draw.cxx \
	fl_draw_image.cxx \
	fl_draw_pixmap.cxx \
	fl_engraved_label.cxx \
	fl_file_chooser.cxx \
	fl_font.cxx \
	fl_glyph.cxx \
	fl_labeltype.cxx \
	fl_line_style.cxx \
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
	fl_shortcut.cxx \
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
	conf_atol.c \
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

CLEAN =

################################################################

include ../makeinclude

LIBRARY = ../lib/$(LIBNAME)
DSONAME = $(DSOLIBNAME)$(DSOEXT)

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.c=.o)

all:	$(LIBRARY).a $(DSONAME)

$(LIBRARY).a: $(OBJECTS) fl_call_main.o
	@echo $(LIBCOMMAND) $@ ...
	@rm -f $@
	@$(LIBCOMMAND) $@ $(OBJECTS) fl_call_main.o
	@$(RANLIB) $@

libfltk.so.2 libfltk.sl.2: $(OBJECTS)
	@echo $(DSOCOMMAND) $@ ...
	@$(DSOCOMMAND) $@ $(OBJECTS)

fltk.2.dll: $(OBJECTS:.o=.dll.o)
	@echo $(DSOCOMMAND) $@ ...
	@rm -f $@
	@$(DSOCOMMAND) -o $@ \
	    $(OBJECTS:.o=.dll.o) $(CXXFLAGS) $(GLDLIBS)

.SUFFIXES:	.cxx .c .h .o

.cxx.o :
	$(CXX) -I.. $(CXXFLAGS) -c $<
.c.o :
	$(CC) -I.. $(CFLAGS) -c $<

%.dll.o : %.cxx
	$(CXX) -I.. $(CXXFLAGS) $(DLLCFLAGS) -c $< -o $@
%.dll.o : %.x
	$(CC) -I.. $(CFLAGS) $(DLLCFLAGS) -c $< -o $@

clean :
	-@ rm -f *.o *.do $(DSONAME) $(LIBRARY).a $(CLEAN) core *~ ../include/*~ makedepend cmap
	@touch makedepend

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) $(CFILES) > makedepend
include makedepend

################################################################

install: $(LIBRARY).a $(DSONAME)
	@echo "Installing libraries..."
	@-mkdir -p $(libdir)
	@rm -f $(libdir)/$(LIBNAME)
	@-cp $(LIBRARY).a $(libdir)
	@-chmod 644 $(libdir)/$(LIBNAME).a
	@if test -n "$(DSOEXT)"; then \
		if test $(DSONAME) = $(LIBNAME).so.2; then ext=.so; else ext=.s1; fi; \
		rm -f $(libdir)/$(LIBNAME)$$ext.2;\
		rm -f $(libdir)/$(LIBNAME)$$ext;\
		cp $(LIBNAME)$$ext.2 $(libdir); \
		chmod 755 $(libdir)/$(LIBNAME)$$ext.2; \
		ln -s $(libdir)/$(LIBNAME)$$ext.2 $(libdir)/$(LIBNAME)$$ext;\
	fi
	@echo "Installing include files..."
	@-mkdir -p $(includedir)
	@rm -rf $(includedir)/FL $(includedir)/Fl
	@-mkdir -p $(includedir)/FL
	@-cp ../FL/*.[Hh]  $(includedir)/FL
	@-chmod 755 $(includedir)/FL
	@-chmod 644 $(includedir)/FL/*
	@if [ ! "$(OSTYPE)" = "cygwin32" ]; then for file in $(includedir)/FL/*.H; do\
		newfile="`basename $$file H`h";\
		ln -s $$file $(includedir)/FL/$$newfile;\
	done; fi
	@if [ ! "$(OSTYPE)" = "cygwin32" ]; then ln -sf FL $(includedir)/Fl; fi
	@echo "Installing fltk-config..."
	@-mkdir -p $(bindir)
	@cp ../fltk-config $(bindir)

#
# End of "$Id: Makefile,v 1.76 2000/06/03 05:32:19 carl Exp $".
#
