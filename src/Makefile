#
# "$Id: Makefile,v 1.41 1999/10/27 08:41:02 bill Exp $"
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
	Fl_Align_Layout.cxx \
	Fl_Bitmap.cxx \
	fl_bmp.cxx \
	Fl_Browser.cxx \
	Fl_Browser_.cxx \
	Fl_Browser_load.cxx \
	Fl_Box.cxx \
	Fl_Button.cxx \
	Fl_Chart.cxx \
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
	Fl_FileInput.cxx \
	Fl_Fly_Button.cxx \
	fl_gif.cxx \
	Fl_Group.cxx \
	Fl_Image.cxx \
	Fl_Input.cxx \
	Fl_Input_Style.cxx \
	Fl_Input_.cxx \
	Fl_Light_Button.cxx \
	Fl_Menu.cxx \
	Fl_Menu_.cxx \
	Fl_Menu_Bar.cxx \
	Fl_Menu_Button.cxx \
	Fl_Menu_Window.cxx \
	Fl_Menu_add.cxx \
	Fl_Menu_global.cxx \
	Fl_Output.cxx \
	Fl_Overlay_Window.cxx \
	Fl_Pack.cxx \
	Fl_Pixmap.cxx \
	Fl_Positioner.cxx \
	Fl_Repeat_Button.cxx \
	Fl_Return_Button.cxx \
	Fl_RGB_Image.cxx \
	Fl_Roller.cxx \
	Fl_Scroll.cxx \
	Fl_Scrollbar.cxx \
	Fl_Single_Window.cxx \
	Fl_Slider.cxx \
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
	Fl_cutpaste.cxx \
	Fl_display.cxx \
	Fl_get_key.cxx \
	Fl_get_system_colors.cxx \
	Fl_grab.cxx \
	Fl_lock.cxx \
	Fl_own_colormap.cxx \
	Fl_visual.cxx \
	Fl_x.cxx \
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
	fl_load_plugin.cxx \
	fl_oval_box.cxx \
	fl_overlay.cxx \
	fl_overlay_visual.cxx \
	fl_rect.cxx \
	fl_round_box.cxx \
	fl_rounded_box.cxx \
	fl_set_fonts.cxx \
	fl_scroll_area.cxx \
	Fl_Shared_Image.cxx \
	fl_shadow_box.cxx \
	fl_shortcut.cxx \
	fl_show_colormap.cxx \
	fl_symbols.cxx \
	fl_vertex.cxx \
	fl_xpm.cxx \
	forms_compatability.cxx \
	forms_bitmap.cxx \
	forms_free.cxx \
	forms_fselect.cxx \
	forms_pixmap.cxx \
	forms_timer.cxx

CPPglFILES = \
	Fl_Gl_Choice.cxx \
	Fl_Gl_Overlay.cxx \
	Fl_Gl_Window.cxx \
	gl_draw.cxx \
	gl_start.cxx \
	glut_compatability.cxx \
	glut_font.cxx

CPPimagesFILES = \
	Fl_Guess_Image.cxx \
	fl_jpeg.cxx\
	fl_png.cxx

CFILES = \
	scandir.c \
	numericsort.c \
	vsnprintf.c

# files that were deleted:
#	fl_set_font.cxx \
#	Fl_Threads.cxx \
#	Fl_Style_Plugins.cxx \
#	Fl_Style_Util.cxx \

# Carl's style-reading source code:
#	fl_options.cxx \
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

#	conf.c \
#	conf_atol.c \
#	conf_del.c \
#	conf_endtrim.c \
#	conf_error.c \
#	conf_get.c \
#	conf_getboolean.c \
#	conf_getlong.c \
#	conf_keys.c \
#	conf_level_indent.c \
#	conf_list_free.c \
#	conf_sections.c \
#	conf_set.c \
#	conf_setboolean.c \
#	conf_setlong.c \
#	conf_strcasecmp.c \
#	conf_strcnt.c \
#	conf_trim.c

CLEAN =

################################################################

include ../makeinclude

LIBRARY = ../lib/$(LIBNAME)
DSONAME = $(DSOLIBNAME)$(DSOEXT)
DSOglNAME = $(DSOLIBNAME)gl$(DSOEXT)
DSOimagesNAME = $(DSOLIBNAME)images$(DSOEXT)

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.c=.o)
OBJECTSgl = $(CPPglFILES:.cxx=.o)
OBJECTSimages = $(CPPimagesFILES:.cxx=.o)

all:	$(LIBRARY) $(DSONAME)

$(LIBRARY): $(OBJECTS) $(OBJECTSgl) $(OBJECTSimages) fl_call_main.o
	@echo $(LIBCOMMAND) $@ ...
	@rm -f $@
	@$(LIBCOMMAND) $@ $(OBJECTS) $(OBJECTSgl) $(OBJECTSimages) fl_call_main.o
	@$(RANLIB) $@

libfltk.so.2 libfltk.sl.2: $(OBJECTS) $(DSOglNAME) $(DSOimagesNAME)
	@echo $(DSOCOMMAND) $@ ...
	@$(DSOCOMMAND) $@ $(OBJECTS)

fltk.2.dll: $(OBJECTS) $(OBJECTSimages) $(OBJECTSgl)
	@echo $(DSOCOMMAND) $@ ...
	@rm -f $@
	@$(DSOCOMMAND) -o $@ \
	    $(OBJECTS) $(OBJECTSimages) $(OBJECTSgl) $(CXXFLAGS) $(GLDLIBS)

libfltkgl.so.2 libfltkgl.sl.2: $(OBJECTSgl)
	@echo $(DSOCOMMAND) $@ ...
	@$(DSOCOMMAND) $@ $(OBJECTSgl)

libfltkimages.so.2 libfltkimages.sl.2: $(OBJECTSimages)
	@echo $(DSOCOMMAND) $@ ...
	@$(DSOCOMMAND) $@ $(OBJECTSimages)

.SUFFIXES:	.cxx .h .o

.cxx.o :
	$(CXX) -I.. $(CXXFLAGS) -c $<
.c.o :
	$(CC) -I.. $(CFLAGS) -c $<

clean :
	-@ rm -f *.o *.do $(DSONAME) $(DSOglNAME) $(DSOimagesNAME) $(LIBRARY) $(CLEAN) core *~ ../include/*~ makedepend cmap
	@touch makedepend

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) $(CPPglFILES) $(CPPimagesFILES) $(CFILES) > makedepend
include makedepend

################################################################

install: $(LIBRARY) $(DSONAME)
	@echo "Installing libraries..."
	@-mkdir -p $(libdir)
	@rm -f $(libdir)/$(LIBNAME)
	@-cp $(LIBRARY) $(libdir)
	@-chmod 644 $(libdir)/$(LIBNAME)
	@if test -n "$(DSONAME)"; then \
		if test "$(DSONAME)" = libfltk.so.2; then ext=.so; else ext=.s1; fi; \
		for lib in libfltk libfltkgl libfltkimages; do \
			rm -f $(libdir)/$$lib$$ext.2;\
			rm -f $(libdir)/$$lib$$ext;\
			cp $$lib$$ext.2 $(libdir); \
			chmod 755 $(libdir)/$$lib$$ext.2; \
			ln -s $(libdir)/$$lib$$ext.2 $(libdir)/$$lib$$ext;\
		done \
	fi
	@echo "Installing include files..."
	@-mkdir -p $(includedir)
	@rm -rf $(includedir)/FL $(includedir)/Fl
	@-cp -r ../FL $(includedir)
	@-chmod 755 $(includedir)/FL
	@-chmod 644 $(includedir)/FL/*
	@for file in $(includedir)/FL/*.H; do\
		newfile="`basename $$file H`h";\
		ln -s $$file $(includedir)/FL/$$newfile;\
	done
	@-ln -s FL $(includedir)/Fl

#
# End of "$Id: Makefile,v 1.41 1999/10/27 08:41:02 bill Exp $".
#
