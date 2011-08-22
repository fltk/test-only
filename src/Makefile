#
# "$Id$"
#
# Library makefile for the Fast Light Tool Kit (FLTK).
#
# Copyright 1998-2011 by Bill Spitzak and others.
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
# Please report all bugs and problems on the following page:
#
#      http://www.fltk.org/str.php
#

FLTK3_CPPFILES = \
	core/Fl.cxx \
	core/Fl_Adjuster.cxx \
	core/Fl_Bitmap.cxx \
	core/Fl_Box.cxx \
	core/Fl_Browser.cxx \
	core/Fl_Browser_.cxx \
	core/Fl_Browser_load.cxx \
	core/Fl_Button.cxx \
	core/Fl_Chart.cxx \
	core/Fl_Check_Browser.cxx \
	core/Fl_Check_Button.cxx \
	core/Fl_Choice.cxx \
	core/Fl_Clock.cxx \
	core/Fl_Color_Chooser.cxx \
	core/Fl_Counter.cxx \
	core/Fl_Device.cxx \
	core/Fl_Dial.cxx \
	core/Fl_Double_Window.cxx \
	core/Fl_File_Browser.cxx \
	core/Fl_File_Chooser.cxx \
	core/Fl_File_Chooser2.cxx \
	core/Fl_File_Icon.cxx \
	core/Fl_File_Input.cxx \
	core/Fl_Group.cxx \
	core/Fl_Help_View.cxx \
	core/Fl_Image.cxx \
	core/Fl_Input.cxx \
	core/Fl_Input_.cxx \
	core/Fl_Light_Button.cxx \
	core/Fl_Menu.cxx \
	core/Fl_Menu_.cxx \
	core/Fl_Menu_Bar.cxx \
	core/Fl_Menu_Button.cxx \
	core/Fl_Menu_Window.cxx \
	core/Fl_Menu_add.cxx \
	core/Fl_Menu_global.cxx \
	core/Fl_Multi_Label.cxx \
	core/Fl_Native_File_Chooser.cxx \
	core/Fl_Overlay_Window.cxx \
	core/Fl_Pack.cxx \
	core/Fl_Paged_Device.cxx \
	core/Fl_Pixmap.cxx \
	core/Fl_Positioner.cxx \
	core/Fl_Printer.cxx \
	core/Fl_Preferences.cxx \
	core/Fl_Progress.cxx \
	core/Fl_Repeat_Button.cxx \
	core/Fl_Return_Button.cxx \
	core/Fl_Roller.cxx \
	core/Fl_Round_Button.cxx \
	core/Fl_Scroll.cxx \
	core/Fl_Scrollbar.cxx \
	core/Fl_Shared_Image.cxx \
	core/Fl_Single_Window.cxx \
	core/Fl_Slider.cxx \
	core/Style.cxx \
	core/Fl_Sys_Menu_Bar.cxx \
	core/Fl_Table.cxx \
	core/Fl_Table_Row.cxx \
	core/Fl_Tabs.cxx \
	core/Fl_Text_Buffer.cxx \
	core/Fl_Text_Display.cxx \
	core/Fl_Text_Editor.cxx \
	core/Fl_Tile.cxx \
	core/Fl_Tiled_Image.cxx \
	core/Fl_Tooltip.cxx \
	core/Fl_Tree.cxx \
	core/Fl_Tree_Item.cxx \
	core/Fl_Tree_Item_Array.cxx \
	core/Fl_Tree_Prefs.cxx \
	core/Fl_Valuator.cxx \
	core/Fl_Value_Input.cxx \
	core/Fl_Value_Output.cxx \
	core/Fl_Value_Slider.cxx \
	core/Fl_Widget.cxx \
	core/Fl_Window.cxx \
	core/Fl_Window_fullscreen.cxx \
	core/Fl_Window_hotspot.cxx \
	core/Fl_Window_iconize.cxx \
	core/Fl_Wizard.cxx \
	core/Fl_XBM_Image.cxx \
	core/Fl_XPM_Image.cxx \
	core/Fl_abort.cxx \
	core/Fl_add_idle.cxx \
	core/Fl_arg.cxx \
	core/Fl_compose.cxx \
	core/Fl_display.cxx \
	core/Fl_get_key.cxx \
	core/Fl_get_system_colors.cxx \
	core/Fl_grab.cxx \
	core/Fl_lock.cxx \
	core/Fl_own_colormap.cxx \
	core/Fl_visual.cxx \
	core/Fl_x.cxx \
	core/filename_absolute.cxx \
	core/filename_expand.cxx \
	core/filename_ext.cxx \
	core/filename_isdir.cxx \
	core/filename_list.cxx \
	core/filename_match.cxx \
	core/filename_setext.cxx \
	core/fl_arc.cxx \
	core/fl_arci.cxx \
	core/fl_ask.cxx \
	core/fl_boxtype.cxx \
	core/fl_call_main.cxx \
	core/fl_color.cxx \
	core/fl_cursor.cxx \
	core/fl_curve.cxx \
	core/fl_diamond_box.cxx \
	core/fl_dnd.cxx \
	core/fl_draw.cxx \
	core/fl_draw_image.cxx \
	core/fl_draw_pixmap.cxx \
	core/fl_encoding_latin1.cxx \
	core/fl_encoding_mac_roman.cxx \
	core/fl_engraved_label.cxx \
	core/fl_file_dir.cxx \
	core/fl_font.cxx \
	core/fl_gtk.cxx \
	core/fl_labeltype.cxx \
	core/fl_line_style.cxx \
	core/fl_open_uri.cxx \
	core/fl_oval_box.cxx \
	core/fl_overlay.cxx \
	core/fl_overlay_visual.cxx \
	core/fl_plastic.cxx \
	core/fl_read_image.cxx \
	core/fl_rect.cxx \
	core/fl_round_box.cxx \
	core/fl_rounded_box.cxx \
	core/fl_scroll_area.cxx \
	core/fl_set_font.cxx \
	core/fl_set_fonts.cxx \
	core/fl_shadow_box.cxx \
	core/fl_shortcut.cxx \
	core/fl_show_colormap.cxx \
	core/fl_symbols.cxx \
	core/fl_utf8.cxx \
	core/fl_utf.cxx \
	core/fl_vertex.cxx \
	core/numericsort.cxx \
	core/ps_image.cxx \
	core/scandir.cxx \
	core/screen_xywh.cxx

FLTK3GL_CPPFILES = \
	core/Fl_Gl_Choice.cxx \
	core/Fl_Gl_Device_Plugin.cxx \
	core/Fl_Gl_Overlay.cxx \
	core/Fl_Gl_Window.cxx \
	core/freeglut_geometry.cxx \
	core/freeglut_stroke_mono_roman.cxx \
	core/freeglut_stroke_roman.cxx \
	core/freeglut_teapot.cxx \
	core/gl_draw.cxx \
	core/glut_compatability.cxx \
	core/glut_font.cxx

FLTK3IMAGES_CPPFILES = \
	core/Fl_BMP_Image.cxx \
	core/Fl_File_Icon2.cxx \
	core/Fl_GIF_Image.cxx \
	core/Fl_Help_Dialog.cxx \
	core/Fl_JPEG_Image.cxx \
	core/Fl_PNG_Image.cxx \
	core/Fl_PNM_Image.cxx \
	core/fl_images_core.cxx

FLTK3_OBJCPPFILES = \
	core/Fl_cocoa.mm \
	core/Fl_Native_File_Chooser_MAC.mm \
	core/Fl_Quartz_Printer.mm

FLTK3_CFILES = \
	core/flstring.c \
	core/vsnprintf.c \
	core/xutf8/case.c \
	core/xutf8/is_right2left.c \
	core/xutf8/is_spacing.c \
	core/xutf8/keysym2Ucs.c \
	core/xutf8/utf8Input.c \
	core/xutf8/utf8Utils.c \
	core/xutf8/utf8Wrap.c

FLTK3PNG_CFILES = \
	png/png.c \
	png/pngerror.c \
	png/pngget.c \
	png/pngmem.c \
	png/pngpread.c \
	png/pngread.c \
	png/pngrio.c \
	png/pngrtran.c \
	png/pngrutil.c \
	png/pngset.c \
	png/pngtrans.c \
	png/pngwio.c \
	png/pngwrite.c \
	png/pngwtran.c \
	png/pngwutil.c

FLTK3JPEG_CFILES = \
	jpeg/jaricom.c \
	jpeg/jcarith.c \
	jpeg/jdarith.c \
	jpeg/jcapimin.c \
	jpeg/jcapistd.c \
	jpeg/jccoefct.c \
	jpeg/jccolor.c \
	jpeg/jcdctmgr.c \
	jpeg/jchuff.c \
	jpeg/jcinit.c \
	jpeg/jcmainct.c \
	jpeg/jcmarker.c \
	jpeg/jcmaster.c \
	jpeg/jcomapi.c \
	jpeg/jcparam.c \
	jpeg/jcprepct.c \
	jpeg/jcsample.c \
	jpeg/jctrans.c \
	jpeg/jdapimin.c \
	jpeg/jdapistd.c \
	jpeg/jdatadst.c \
	jpeg/jdatasrc.c \
	jpeg/jdcoefct.c \
	jpeg/jdcolor.c \
	jpeg/jddctmgr.c \
	jpeg/jdhuff.c \
	jpeg/jdinput.c \
	jpeg/jdmainct.c \
	jpeg/jdmarker.c \
	jpeg/jdmaster.c \
	jpeg/jdmerge.c \
	jpeg/jdpostct.c \
	jpeg/jdsample.c \
	jpeg/jdtrans.c \
	jpeg/jerror.c \
	jpeg/jfdctflt.c \
	jpeg/jfdctfst.c \
	jpeg/jfdctint.c \
	jpeg/jidctflt.c \
	jpeg/jidctfst.c \
	jpeg/jidctint.c \
	jpeg/jmemmgr.c \
	jpeg/jmemnobs.c \
	jpeg/jquant1.c \
	jpeg/jquant2.c \
	jpeg/jutils.c

FLTK3ZLIB_CFILES = \
	zlib/adler32.c \
	zlib/compress.c \
	zlib/crc32.c \
	zlib/deflate.c \
	zlib/gzclose.c \
	zlib/gzlib.c \
	zlib/gzread.c \
	zlib/gzwrite.c \
	zlib/infback.c \
	zlib/inffast.c \
	zlib/inflate.c \
	zlib/inftrees.c \
	zlib/trees.c \
	zlib/uncompr.c \
	zlib/zutil.c

################################################################
FLTKFLAGS = -DFL_LIBRARY
include ../makeinclude

FLTK3_MMFILES = $(shell if test $(USEMMFILES) = Yes; then echo $(FLTK3_OBJCPPFILES); fi)

FLTK3_OBJECTS = $(FLTK3_CPPFILES:.cxx=.o) $(FLTK3_MMFILES:.mm=.o) $(FLTK3_CFILES:.c=.o)
FLTK3GL_OBJECTS = $(FLTK3GL_CPPFILES:.cxx=.o)
FLTK3IMAGES_OBJECTS = $(FLTK3IMAGES_CPPFILES:.cxx=.o)
FLTK3PNG_OBJECTS = $(FLTK3PNG_CFILES:.c=.o)
FLTK3JPEG_OBJECTS = $(FLTK3JPEG_CFILES:.c=.o)
FLTK3ZLIB_OBJECTS = $(FLTK3ZLIB_CFILES:.c=.o)

all: \
	$(FLTK3_LIBNAME) $(FLTK3_DSONAME) \
	$(FLTK3GL_LIBNAME) $(FLTK3GL_DSONAME) \
	$(FLTK3IMAGES_LIBNAME) $(FLTK3IMAGES_DSONAME) \
	$(FLTK3PNG_LIBNAME) $(FLTK3PNG_DSONAME) \
	$(FLTK3JPEG_LIBNAME) $(FLTK3JPEG_DSONAME) \
	$(FLTK3ZLIB_LIBNAME) $(FLTK3ZLIB_DSONAME)

$(FLTK3_LIBNAME): $(FLTK3_OBJECTS)
	echo $(LIBCOMMAND) $@ ...
	$(RM) $@
	$(LIBCOMMAND) $@ $(FLTK3_OBJECTS)
	$(RANLIB) $@

$(FLTK3JPEG_LIBNAME): $(FLTK3JPEG_OBJECTS)
	echo $(LIBCOMMAND) $@ ...
	$(RM) $@
	$(LIBCOMMAND) $@ $(FLTK3JPEG_OBJECTS)
	$(RANLIB) $@

$(FLTK3PNG_LIBNAME): $(FLTK3PNG_OBJECTS)
	echo $(LIBCOMMAND) $@ ...
	$(RM) $@
	$(LIBCOMMAND) $@ $(FLTK3PNG_OBJECTS)
	$(RANLIB) $@

$(FLTK3ZLIB_LIBNAME): $(FLTK3ZLIB_OBJECTS)
	echo $(LIBCOMMAND) $@ ...
	$(RM) $@
	$(LIBCOMMAND) $@ $(FLTK3ZLIB_OBJECTS)
	$(RANLIB) $@

libfltk.so.1.3: $(FLTK_OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(FLTK_OBJECTS)
	$(RM) libfltk.so
	$(LN) libfltk.so.1.3 libfltk.so

libfltk.sl.1.3: $(FLTK_OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(FLTK_OBJECTS)
	$(RM) libfltk.sl
	$(LN) libfltk.sl.1.3 libfltk.sl

libfltk.1.3.dylib: $(FLTK_OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ \
		-install_name $(libdir)/$@ \
		-current_version 1.3.0 \
		-compatibility_version 1.3.0 \
		$(FLTK_OBJECTS) $(LDLIBS)
	$(RM) libfltk.dylib
	$(LN) libfltk.1.3.dylib libfltk.dylib

libfltk_s.a: $(FLTK_OBJECTS)
	echo $(DSOCOMMAND) libfltk_s.o ...
	$(DSOCOMMAND) libfltk_s.o $(FLTK_OBJECTS) $(IMAGELIBS)
	echo $(LIBCOMMAND) libfltk_s.a libfltk_s.o
	$(RM) $@
	$(LIBCOMMAND) libfltk_s.a libfltk_s.o
	$(CHMOD) +x libfltk_s.a

$(FLTK3GL_LIBNAME): $(FLTK3GL_OBJECTS)
	echo $(LIBCOMMAND) $@ ...
	$(RM) $@
	$(LIBCOMMAND) $@ $(FLTK3GL_OBJECTS)
	$(RANLIB) $@

libfltk_gl.so.1.3: $(FLTK_GL_OBJECTS) libfltk.so.1.3
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(FLTK_GL_OBJECTS) -L. -lfltk
	$(RM) libfltk_gl.so
	$(LN) libfltk_gl.so.1.3 libfltk_gl.so

libfltk_gl.sl.1.3: $(FLTK_GL_OBJECTS) libfltk.sl.1.3
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(FLTK_GL_OBJECTS) -L. -lfltk
	$(RM) libfltk_gl.sl
	$(LN) libfltk_gl.sl.1.3 libfltk_gl.sl

libfltk_gl.1.3.dylib: $(FLTK_GL_OBJECTS) libfltk.1.3.dylib
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ \
		-install_name $(libdir)/$@ \
		-current_version 1.3.0 \
		-compatibility_version 1.3.0 \
		$(FLTK_GL_OBJECTS) -L. $(GLDLIBS) -lfltk
	$(RM) libfltk_gl.dylib
	$(LN) libfltk_gl.1.3.dylib libfltk_gl.dylib

libfltk_gl_s.a: $(FLTK_GL_OBJECTS)
	echo $(DSOCOMMAND) libfltk_gl_s.o ...
	$(DSOCOMMAND) libfltk_gl_s.o $(FLTK_GL_OBJECTS)
	echo $(LIBCOMMAND) libfltk_gl_s.a libfltk_gl_s.o
	$(RM) $@
	$(LIBCOMMAND) libfltk_gl_s.a libfltk_gl_s.o
	$(CHMOD) +x libfltk_gl_s.a

$(FLTK3IMAGES_LIBNAME): $(FLTK3IMAGES_OBJECTS)
	echo $(LIBCOMMAND) $@ ...
	$(RM) $@
	$(LIBCOMMAND) $@ $(FLTK3IMAGES_OBJECTS)
	$(RANLIB) $@

libfltk_images.so.1.3: $(FLTK_IMAGES_OBJECTS) libfltk.so.1.3
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(FLTK_IMAGES_OBJECTS) -L. $(IMAGELIBS) -lfltk
	$(RM) libfltk_images.so
	$(LN) libfltk_images.so.1.3 libfltk_images.so

libfltk_images.sl.1.3: $(FLTK_IMAGES_OBJECTS) libfltk.sl.1.3
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(FLTK_IMAGES_OBJECTS) -L. $(IMAGELIBS) -lfltk
	$(RM) libfltk_images.sl
	$(LN) libfltk_images.sl.1.3 libfltk_images.sl

libfltk_images.1.3.dylib: $(FLTK_IMAGES_OBJECTS) libfltk.1.3.dylib
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ \
		-install_name $(libdir)/$@ \
		-current_version 1.3.0 \
		-compatibility_version 1.3.0 \
		$(FLTK_IMAGES_OBJECTS)  -L. $(LDLIBS) $(IMAGELIBS) -lfltk
	$(RM) libfltk_images.dylib
	$(LN) libfltk_images.1.3.dylib libfltk_images.dylib

libfltk_images_s.a: $(FLTK_IMAGES_OBJECTS)
	echo $(DSOCOMMAND) libfltk_images_s.o ...
	$(DSOCOMMAND) libfltk_images_s.o $(FLTK_IMAGES_OBJECTS)
	echo $(LIBCOMMAND) libfltk_images_s.a libfltk_images_s.o
	$(RM) $@
	$(LIBCOMMAND) libfltk_images_s.a libfltk_images_s.o
	$(CHMOD) +x libfltk_images_s.a

#-----------------------------------------------------------------
# - the import libraries libfltk*.dll.a and the .dll files
#   are created from the libfltk*.a files. They are built
#   into the src dir.
# - The _images, _gl and dlls must be linked
#   against the import libraries in the src dir.
#-----------------------------------------------------------------

#-----------------------------------------------------------------
# cygwin GDI shared libraries
#-----------------------------------------------------------------

cygfltknox-1.3.dll: $(FLTK_LIBNAME)
	echo $(DSOCOMMAND) $(FLTK_LIBNAME) ...
	$(DSOCOMMAND) $(FLTK_LIBNAME) -Wl,--no-whole-archive \
		-Wl,--out-implib=libfltk.dll.a $(LDLIBS)

cygfltknox_gl-1.3.dll: $(FLTK_GL_LIBNAME) cygfltknox-1.3.dll
	echo $(DSOCOMMAND) $(FLTK_GL_LIBNAME) ...
	$(DSOCOMMAND) $(FLTK_GL_LIBNAME) -Wl,--no-whole-archive \
		-Wl,--out-implib=libfltk_gl.dll.a \
		-L. -lfltk $(GLDLIBS)

cygfltknox_images-1.3.dll: $(FLTK_IMAGES_LIBNAME) cygfltknox-1.3.dll
	echo $(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) ...
	$(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) -Wl,--no-whole-archive \
		-Wl,--out-implib=libfltk_images.dll.a \
		-L. -lfltk -Wl,--exclude-libs -Wl,libfltk_png.a \
			$(IMAGELIBS) $(LDLIBS)

#-----------------------------------------------------------------
# cygwin X11 shared libraries
#-----------------------------------------------------------------

cygfltk-1.3.dll: $(FLTK_LIBNAME)
	echo $(DSOCOMMAND) $(FLTK_LIBNAME) ...
	$(DSOCOMMAND) $(FLTK_LIBNAME) -Wl,--no-whole-archive \
		-Wl,--out-implib=libfltk.dll.a $(LDLIBS)

cygfltk_gl-1.3.dll: $(FLTK_GL_LIBNAME) cygfltk-1.3.dll
	echo $(DSOCOMMAND) $(FLTK_GL_LIBNAME) ...
	$(DSOCOMMAND) $(FLTK_GL_LIBNAME) -Wl,--no-whole-archive \
		-Wl,--out-implib=libfltk_gl.dll.a \
		-L. -lfltk $(GLDLIBS)

cygfltk_images-1.3.dll: $(FLTK_IMAGES_LIBNAME) cygfltk-1.3.dll
	echo $(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) ...
	$(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) -Wl,--no-whole-archive \
		-Wl,--out-implib=libfltk_images.dll.a \
		-L. -lfltk -Wl,--exclude-libs -Wl,libfltk_png.a \
			$(IMAGELIBS) $(LDLIBS)

mgwfltknox-1.3.dll: $(FLTK_LIBNAME)
	echo $(DSOCOMMAND) $(FLTK_LIBNAME) ...
	$(DSOCOMMAND) $(FLTK_LIBNAME) -Wl,--no-whole-archive \
		-Wl,--out-implib=libfltk.dll.a $(LDLIBS)

mgwfltknox_gl-1.3.dll: $(FLTK_GL_LIBNAME) mgwfltknox-1.3.dll
	echo $(DSOCOMMAND) $(FLTK_GL_LIBNAME) ...
	$(DSOCOMMAND) $(FLTK_GL_LIBNAME) -Wl,--no-whole-archive \
		-Wl,--out-implib=libfltk_gl.dll.a \
		-L. -lfltk $(GLDLIBS)

#-----------------------------------------------------
# See STR #1585 for --exclude-libs
#-----------------------------------------------------

mgwfltknox_images-1.3.dll: $(FLTK_IMAGES_LIBNAME) mgwfltknox-1.3.dll
	echo $(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) ...
	$(DSOCOMMAND) $(FLTK_IMAGES_LIBNAME) -Wl,--no-whole-archive \
		-Wl,--out-implib=libfltk_images.dll.a \
		-L. -lfltk -Wl,--exclude-libs -Wl,libfltk_png.a \
			$(IMAGELIBS) $(LDLIBS)

clean:
	-$(RM) *.o xutf8/*.o *.dll.a core.* *~ *.bak *.bck
	-$(RM) $(FLTK_DSONAME) $(FLTK_GL_DSONAME) $(FLTK_IMAGES_DSONAME) \
		$(FLTK_LIBNAME) $(FLTK_GL_LIBNAME) \
		$(FLTK_IMAGES_LIBNAME) \
		libfltk.so libfltk_gl.so libfltk_images.so \
		libfltk.sl libfltk_gl.sl libfltk_images.sl \
		libfltk.dylib \
		libfltk_gl.dylib libfltk_images.dylib \
		cmap

depend:	$(FLTK_CPPFILES) $(FLTK_MMFILES) $(FLTK_GL_CPPFILES) $(FLTK_IMAGES_CPPFILES) $(FLTK_CFILES)
	makedepend -Y -I.. -f makedepend $(FLTK_CPPFILES) $(FLTK_MMFILES) \
		$(FLTK_GL_CPPFILES) $(FLTK_IMAGES_CPPFILES) $(FLTK_CFILES)

# Automatically generated dependencies... generated on a Linux/Unix host !
include makedepend

# These dependencies aren't part of the makedepend file since
# they are part of the WIN32 and MacOS code base...
# Please add only non-Linux/Unix files or such that are optional
# (like "*xft*") here:
Fl_get_key.o:	Fl_get_key_mac.cxx Fl_get_key_win32.cxx
Fl_Native_File_Chooser.o : Fl_Native_File_Chooser_WIN32.cxx
Fl_Native_File_Chooser_MAC.o:	Fl_Native_File_Chooser_MAC.mm
Fl.o:		Fl_win32.cxx
Fl_cocoa.o:	Fl_cocoa.mm
fl_color.o:	fl_color_mac.cxx fl_color_win32.cxx
fl_dnd.o:	fl_dnd_win32.cxx fl_dnd_x.cxx
fl_draw_image.o: fl_draw_image_mac.cxx fl_draw_image_win32.cxx
fl_font.o:	fl_font_mac.cxx fl_font_x.cxx fl_font_xft.cxx fl_font_win32.cxx
fl_read_image.o: fl_read_image_mac.cxx fl_read_image_win32.cxx
fl_set_fonts.o:	fl_set_fonts_mac.cxx fl_set_fonts_x.cxx \
		fl_set_fonts_xft.cxx fl_set_fonts_win32.cxx
Fl_Printer.o: Fl_GDI_Printer.cxx Fl_PostScript.cxx
Fl_Quartz_Printer.o:	Fl_Quartz_Printer.mm
fl_arci.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_arg.o:	../fltk3/osx.h ../fltk3/win32.h
fl_ask.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Bitmap.o:	../fltk3/osx.h ../fltk3/win32.h
fl_color.o:	../fltk3/osx.h ../fltk3/win32.h
fl_cursor.o:	../fltk3/osx.h ../fltk3/win32.h
fl_dnd.o:	../fltk3/osx.h ../fltk3/win32.h
Fl.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Double_Window.o:	../fltk3/osx.h ../fltk3/win32.h
fl_draw_image.o:	../fltk3/osx.h ../fltk3/win32.h
fl_draw_image_mac.o:	../fltk3/osx.h ../fltk3/win32.h
fl_draw_image_win32.o:	../fltk3/osx.h ../fltk3/win32.h
fl_draw_pixmap.o:	../fltk3/osx.h ../fltk3/win32.h
fl_encoding_latin1.o:	../fltk3/osx.h ../fltk3/win32.h
fl_encoding_mac_roman.o:   ../fltk3/osx.h ../fltk3/win32.h
Fl_File_Chooser2.o:	../fltk3/osx.h ../fltk3/win32.h
fl_font.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_get_key.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_get_system_colors.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Gl_Choice.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Gl_Overlay.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Gl_Window.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_grab.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Image.o:	../fltk3/osx.h ../fltk3/win32.h
fl_line_style.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Menu_Window.o:	../fltk3/osx.h ../fltk3/win32.h
fl_overlay.o:	../fltk3/osx.h ../fltk3/win32.h
fl_overlay_visual.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Overlay_Window.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_own_colormap.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Pixmap.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Printer.o:  ../fltk3/osx.h ../fltk3/win32.h
fl_read_image.o:	../fltk3/osx.h ../fltk3/win32.h
fl_read_image_mac.o:	../fltk3/osx.h ../fltk3/win32.h
fl_read_image_win32.o:	../fltk3/osx.h ../fltk3/win32.h
fl_rect.o:	../fltk3/osx.h ../fltk3/win32.h
fl_scroll_area.o:	../fltk3/osx.h ../fltk3/win32.h
fl_set_font.o:	../fltk3/osx.h ../fltk3/win32.h
fl_set_fonts.o:	../fltk3/osx.h ../fltk3/win32.h
fl_shortcut.o:	../fltk3/osx.h ../fltk3/win32.h
fl_vertex.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_visual.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Window_fullscreen.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_Window_iconize.o:	../fltk3/osx.h ../fltk3/win32.h
Fl_x.o:	../fltk3/osx.h ../fltk3/win32.h
gl_draw.o:	../fltk3/osx.h ../fltk3/win32.h
gl_start.o:	../fltk3/osx.h ../fltk3/win32.h

################################################################

install: $(FLTK_LIBNAME) $(FLTK_DSONAME) \
	$(FLTK_GL_LIBNAME) $(FLTK_GL_DSONAME) \
	$(FLTK_IMAGES_LIBNAME) $(FLTK_IMAGES_DSONAME) 
	echo "Installing libraries in $(DESTDIR)$(libdir)..."
	-$(INSTALL_DIR) $(DESTDIR)$(libdir)
	-$(INSTALL_DIR) $(DESTDIR)$(bindir)
	$(RM) $(DESTDIR)$(libdir)/$(FLTK_LIBBASENAME)
	$(INSTALL_LIB) $(FLTK_LIBNAME) $(DESTDIR)$(libdir)
	$(INSTALL_LIB) $(FLTK_IMAGES_LIBNAME) $(DESTDIR)$(libdir)
	$(RANLIB) $(DESTDIR)$(libdir)/$(FLTK_LIBBASENAME)
	if test x$(FLTK_GL_LIBNAME) != x; then \
		$(INSTALL_LIB) $(FLTK_GL_LIBNAME) $(DESTDIR)$(libdir); \
		$(RANLIB) $(DESTDIR)$(libdir)/$(FLTK_GL_LIBBASENAME); \
	fi
	$(RANLIB) $(DESTDIR)$(libdir)/$(FLTK_IMAGES_LIBBASENAME)
	if test x$(FLTK_DSONAME) = xlibfltk.so.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk.so*;\
		$(INSTALL_LIB) libfltk.so.1.3 $(DESTDIR)$(libdir); \
		$(LN) libfltk.so.1.3 $(DESTDIR)$(libdir)/libfltk.so;\
	fi
	if test x$(FLTK_DSONAME) = xlibfltk.sl.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk.sl*;\
		$(INSTALL_LIB) libfltk.sl.1.3 $(DESTDIR)$(libdir); \
		$(LN) libfltk.sl.1.3 $(DESTDIR)$(libdir)/libfltk.sl;\
	fi
	if test x$(FLTK_DSONAME) = xlibfltk.1.3.dylib; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk.*dylib;\
		$(INSTALL_LIB) libfltk.1.3.dylib $(DESTDIR)$(libdir); \
		$(LN) libfltk.1.3.dylib $(DESTDIR)$(libdir)/libfltk.dylib;\
	fi
	if test x$(FLTK_DSONAME) = xlibfltk_s.a; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_s.a;\
		$(INSTALL_LIB) libfltk_s.a $(DESTDIR)$(libdir); \
	fi
	if test x$(FLTK_DSONAME) = xcygfltknox-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_DSONAME);\
		$(INSTALL_LIB) $(FLTK_DSONAME) $(DESTDIR)$(bindir); \
		$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\
		$(INSTALL_LIB) libfltk.dll.a $(DESTDIR)$(libdir); \
	fi
	if test x$(FLTK_DSONAME) = xmgwfltknox-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_DSONAME);\
		$(INSTALL_LIB) $(FLTK_DSONAME) $(DESTDIR)$(bindir); \
		$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\
		$(INSTALL_LIB) libfltk.dll.a $(DESTDIR)$(libdir); \
	fi
	if test x$(FLTK_GL_DSONAME) = xlibfltk_gl.so.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl.so*;\
		$(INSTALL_LIB) libfltk_gl.so.1.3 $(DESTDIR)$(libdir); \
		$(LN) libfltk_gl.so.1.3 $(DESTDIR)$(libdir)/libfltk_gl.so;\
	fi
	if test x$(FLTK_GL_DSONAME) = xlibfltk_gl.sl.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl.sl*;\
		$(INSTALL_LIB) libfltk_gl.sl.1.3 $(DESTDIR)$(libdir); \
		$(LN) libfltk_gl.sl.1.3 $(DESTDIR)$(libdir)/libfltk_gl.sl;\
	fi
	if test x$(FLTK_GL_DSONAME) = xlibfltk_gl.1.3.dylib; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl.*dylib;\
		$(INSTALL_LIB) libfltk_gl.1.3.dylib $(DESTDIR)$(libdir); \
		$(LN) libfltk_gl.1.3.dylib $(DESTDIR)$(libdir)/libfltk_gl.dylib;\
	fi
	if test x$(FLTK_GL_DSONAME) = xlibfltk_gl_s.a; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl_s.a;\
		$(INSTALL_LIB) libfltk_gl_s.a $(DESTDIR)$(libdir); \
	fi
	if test x$(FLTK_GL_DSONAME) = xcygfltknox_gl-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_GL_DSONAME);\
		$(INSTALL_LIB) $(FLTK_GL_DSONAME) $(DESTDIR)$(bindir); \
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\
		$(INSTALL_LIB) libfltk_gl.dll.a $(DESTDIR)$(libdir); \
	fi
	if test x$(FLTK_GL_DSONAME) = xmgwfltknox_gl-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_GL_DSONAME);\
		$(INSTALL_LIB) $(FLTK_GL_DSONAME) $(DESTDIR)$(bindir); \
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\
		$(INSTALL_LIB) libfltk_gl.dll.a $(DESTDIR)$(libdir); \
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.so.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_images.so*;\
		$(INSTALL_LIB) libfltk_images.so.1.3 $(DESTDIR)$(libdir); \
		$(LN) libfltk_images.so.1.3 $(DESTDIR)$(libdir)/libfltk_images.so;\
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.sl.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_images.sl*;\
		$(INSTALL_LIB) libfltk_images.sl.1.3 $(DESTDIR)$(libdir); \
		$(LN) libfltk_images.sl.1.3 $(DESTDIR)$(libdir)/libfltk_images.sl;\
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.1.3.dylib; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_images.*dylib;\
		$(INSTALL_LIB) libfltk_images.1.3.dylib $(DESTDIR)$(libdir); \
		$(LN) libfltk_images.1.3.dylib $(DESTDIR)$(libdir)/libfltk_images.dylib;\
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images_s.a; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_images_s.a;\
		$(INSTALL_LIB) libfltk_images_s.a $(DESTDIR)$(libdir); \
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xcygfltknox_images-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_IMAGES_DSONAME); \
		$(INSTALL_LIB) $(FLTK_IMAGES_DSONAME) $(DESTDIR)$(bindir); \
		$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\
		$(INSTALL_LIB) libfltk_images.dll.a $(DESTDIR)$(libdir); \
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xmgwfltknox_images-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_IMAGES_DSONAME); \
		$(INSTALL_LIB) $(FLTK_IMAGES_DSONAME) $(DESTDIR)$(bindir); \
		$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\
		$(INSTALL_LIB) libfltk_images.dll.a $(DESTDIR)$(libdir); \
	fi


uninstall:
	echo "Uninstalling libraries..."
	$(RM) $(DESTDIR)$(libdir)/$(FLTK_LIBBASENAME)
	if test x$(FLTK_DSONAME) = xlibfltk.so.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk.so*;\
	fi
	if test x$(FLTK_DSONAME) = xlibfltk.sl.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk.sl*;\
	fi
	if test x$(FLTK_DSONAME) = xlibfltk.1.3.dylib; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk.*dylib;\
	fi
	if test x$(FLTK_DSONAME) = xlibfltk_s.a; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_s.a;\
	fi
	if test x$(FLTK_DSONAME) = xcygfltknox-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_DSONAME); \
		$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\
	fi
	if test x$(FLTK_DSONAME) = xcygfltk-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_DSONAME); \
		$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\
	fi
	if test x$(FLTK_DSONAME) = xmgwfltknox-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_DSONAME); \
		$(RM) $(DESTDIR)$(libdir)/libfltk.dll.a;\
	fi
	if test x$(FLTK_GL_LIBNAME) != x; then\
		$(RM) $(DESTDIR)$(libdir)/$(FLTK_GL_LIBBASENAME);\
	fi
	if test x$(FLTK_GL_DSONAME) = xlibfltk_gl.so.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl.so*;\
	fi
	if test x$(FLTK_GL_DSONAME) = xlibfltk_gl.sl.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl.sl*;\
	fi
	if test x$(FLTK_GL_DSONAME) = xlibfltk_gl.1.3.dylib; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl.*dylib;\
	fi
	if test x$(FLTK_GL_DSONAME) = xlibfltk_gl_s.a; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl_s.a;\
	fi
	if test x$(FLTK_GL_DSONAME) = xcygfltknox_gl-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_GL_DSONAME); \
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\
	fi
	if test x$(FLTK_GL_DSONAME) = xcygfltk_gl-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_GL_DSONAME); \
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\
	fi
	if test x$(FLTK_GL_DSONAME) = xmgwfltknox_gl-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_GL_DSONAME); \
		$(RM) $(DESTDIR)$(libdir)/libfltk_gl.dll.a;\
	fi
	if test x$(FLTK_IMAGES_LIBNAME) != x; then\
		$(RM) $(DESTDIR)$(libdir)/$(FLTK_IMAGES_LIBBASENAME);\
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.so.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_images.so*;\
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.sl.1.3; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_images.sl*;\
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images.1.3.dylib; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_images.*dylib;\
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xlibfltk_images_s.a; then\
		$(RM) $(DESTDIR)$(libdir)/libfltk_images_s.a;\
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xcygfltknox_images-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_IMAGES_DSONAME); \
		$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xcygfltk_images-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_IMAGES_DSONAME); \
		$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\
	fi
	if test x$(FLTK_IMAGES_DSONAME) = xmgwfltknox_images-1.3.dll; then\
		$(RM) $(DESTDIR)$(bindir)/$(FLTK_IMAGES_DSONAME); \
		$(RM) $(DESTDIR)$(libdir)/libfltk_images.dll.a;\
	fi

#
# End of "$Id$".
#
# DO NOT DELETE
