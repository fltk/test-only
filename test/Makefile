#
# "$Id$"
#
# Test/example program makefile for the Fast Light Tool Kit (FLTK).
#
# Copyright 1998-2013 by Bill Spitzak and others.
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

include ../makeinclude

CPPFILES = \
	demo.cxx \
	adjuster.cxx \
	arc.cxx \
	ask.cxx \
	bitmap.cxx \
	blocks.cxx \
	boxtype.cxx \
	browser.cxx \
	button.cxx \
	buttons.cxx \
	checkers.cxx \
	clock.cxx \
	colbrowser.cxx \
	color_chooser.cxx \
	connect.cxx \
	cube.cxx \
	CubeView.cxx \
	CubeMain.cxx \
	cursor.cxx \
	curve.cxx \
	device.cxx \
	doublebuffer.cxx \
	editor.cxx \
	file_chooser.cxx \
	fonts.cxx \
	fractals.cxx \
	fracviewer.cxx \
	fullscreen.cxx \
	glpuzzle.cxx \
	gl_overlay.cxx \
	hello.cxx \
	help.cxx \
	iconize.cxx \
	image.cxx \
	input.cxx \
	input_choice.cxx \
	keyboard.cxx \
	label.cxx \
	line_style.cxx \
	list_visuals.cxx \
	mandelbrot.cxx \
	menubar.cxx \
	message.cxx \
	minimum.cxx \
	navigation.cxx \
	native-filechooser.cxx \
	output.cxx \
	overlay.cxx \
	pack.cxx \
	pixmap_browser.cxx \
	pixmap.cxx \
	resizebox.cxx \
	rotated_text.cxx \
	scalebar.cxx \
	scroll.cxx \
	shape.cxx \
	shapedwindow.cxx \
	subwindow.cxx \
	sudoku.cxx \
	symbols.cxx \
	table.cxx \
	threads.cxx \
	tile.cxx \
	tiled_image.cxx \
	unittests.cxx \
	unittest_about.cxx \
	unittest_circles.cxx \
	unittest_images.cxx \
	unittest_lines.cxx \
	unittest_points.cxx \
	unittest_rects.cxx \
	unittest_scrollbarsize.cxx \
	unittest_text.cxx \
	unittest_viewport.cxx \
	utf8.cxx

ALL = \
	demo$(EXEEXT) \
	adjuster$(EXEEXT) \
	arc$(EXEEXT) \
	ask$(EXEEXT) \
	bitmap$(EXEEXT) \
	blocks$(EXEEXT) \
	boxtype$(EXEEXT) \
	browser$(EXEEXT) \
	button$(EXEEXT) \
	buttons$(EXEEXT) \
	checkers$(EXEEXT) \
	clock$(EXEEXT) \
	colbrowser$(EXEEXT) \
	color_chooser$(EXEEXT) \
	connect$(EXEEXT) \
	cube$(EXEEXT) \
	CubeView$(EXEEXT) \
	cursor$(EXEEXT) \
	curve$(EXEEXT) \
	device$(EXEEXT) \
	doublebuffer$(EXEEXT) \
	editor$(EXEEXT) \
	fast_slow$(EXEEXT) \
	file_chooser$(EXEEXT) \
	fonts$(EXEEXT) \
	fractals$(EXEEXT) \
	fullscreen$(EXEEXT) \
	glpuzzle$(EXEEXT) \
	gl_overlay$(EXEEXT) \
	hello$(EXEEXT) \
	help$(EXEEXT) \
	iconize$(EXEEXT) \
	image$(EXEEXT) \
	inactive$(EXEEXT) \
	input$(EXEEXT) \
	input_choice$(EXEEXT) \
	keyboard$(EXEEXT) \
	label$(EXEEXT) \
	line_style$(EXEEXT) \
	list_visuals$(EXEEXT) \
	mandelbrot$(EXEEXT) \
	menubar$(EXEEXT) \
	message$(EXEEXT) \
	minimum$(EXEEXT) \
	navigation$(EXEEXT) \
	native-filechooser$(EXEEXT) \
	output$(EXEEXT) \
	overlay$(EXEEXT) \
	pack$(EXEEXT) \
	pixmap_browser$(EXEEXT) \
	pixmap$(EXEEXT) \
	preferences$(EXEEXT) \
	radio$(EXEEXT) \
	resizebox$(EXEEXT) \
	resize$(EXEEXT) \
	rotated_text$(EXEEXT) \
	scalebar$(EXEEXT) \
	scroll$(EXEEXT) \
	shape$(EXEEXT) \
	shapedwindow$(EXEEXT) \
	subwindow$(EXEEXT) \
	sudoku$(EXEEXT) \
	symbols$(EXEEXT) \
	table$(EXEEXT) \
	tabs$(EXEEXT) \
	threads$(EXEEXT) \
	tile$(EXEEXT) \
	tiled_image$(EXEEXT) \
	tree$(EXEEXT) \
	unittests$(EXEEXT) \
	utf8$(EXEEXT) \
	valuators$(EXEEXT)

GLALL =

all:	$(ALL) $(GLDEMOS)

gldemos:	$(GLALL)

depend:	$(CPPFILES)
	makedepend -Y -I../include -f makedepend $(CPPFILES)

# Automatically generated dependencies...
include makedepend

clean:
	$(RM) $(ALL) $(GLALL) core
	$(RM) *.o core.* *~ *.bck *.bak
	$(RM) CubeViewUI.cxx CubeViewUI.h
	$(RM) fast_slow.cxx fast_slow.h
	$(RM) inactive.cxx inactive.h
	$(RM) keyboard_ui.cxx keyboard_ui.h
	$(RM) mandelbrot_ui.cxx mandelbrot_ui.h
	$(RM) preferences.cxx preferences.h
	$(RM) radio.cxx radio.h
	$(RM) resize.cxx resize.h
	$(RM) tabs.cxx tabs.h
	$(RM) tree.cxx tree.h
	$(RM) valuators.cxx valuators.h
	$(OSX_ONLY) $(RM) blocks.app/Contents/MacOS/blocks$(EXEEXT)
	$(OSX_ONLY) $(RM) checkers.app/Contents/MacOS/checkers$(EXEEXT)
	$(OSX_ONLY) $(RM) sudoku.app/Contents/MacOS/sudoku$(EXEEXT)

install:	all
	echo "Installing example programs to $(DESTDIR)$(docdir)/examples..."
	-$(INSTALL_DIR) $(DESTDIR)$(docdir)/examples
	for file in *.h *.cxx *.fl demo.menu; do \
		$(INSTALL_DATA) $$file $(DESTDIR)$(docdir)/examples; \
	done
	-$(INSTALL_DIR) $(DESTDIR)$(docdir)/examples/pixmaps
	for file in pixmaps/*.xbm pixmaps/*.xpm; do \
		$(INSTALL_DATA) $$file $(DESTDIR)$(docdir)/examples/pixmaps; \
	done

install-linux:
	echo Installing games to $(DESTDIR)$(bindir)...
	-$(INSTALL_DIR) $(DESTDIR)$(bindir)
	-$(INSTALL_DIR) $(DESTDIR)/usr/share/applications
	-$(INSTALL_DIR) $(DESTDIR)/usr/share/icons/hicolor/32x32/apps
	-$(INSTALL_DIR) $(DESTDIR)/usr/share/icons/hicolor/128x128/apps
	for game in blocks checkers sudoku; do \
		$(INSTALL_BIN) $$game $(DESTDIR)$(bindir); \
		$(INSTALL_DATA) desktop/$$game.desktop  $(DESTDIR)/usr/share/applications; \
		$(INSTALL_DATA) desktop/$$game-32.png $(DESTDIR)/usr/share/icons/hicolor/32x32/apps/$$game.png; \
		$(INSTALL_DATA) desktop/$$game-128.png $(DESTDIR)/usr/share/icons/hicolor/128x128/apps/$$game.png; \
	done

install-osx:
	echo Installing games in $(DESTDIR)/Applications...
	for game in blocks checkers sudoku; do \
		if test ! -d $(DESTDIR)/Applications/$$game.app; then \
			$(INSTALL_DIR) $(DESTDIR)/Applications/$$game.app; \
			$(INSTALL_DIR) $(DESTDIR)/Applications/$$game.app/Contents; \
			$(INSTALL_DIR) $(DESTDIR)/Applications/$$game.app/Contents/MacOS; \
			$(INSTALL_DIR) $(DESTDIR)/Applications/$$game.app/Contents/Resources; \
		fi; \
		$(INSTALL_DATA) $$game.app/Contents/Info.plist $(DESTDIR)/Applications/$$game.app/Contents; \
		$(INSTALL_DATA) $$game.app/Contents/PkgInfo $(DESTDIR)/Applications/$$game.app/Contents; \
		$(INSTALL_BIN) $$game.app/Contents/MacOS/$$game $(DESTDIR)/Applications/$$game.app/Contents/MacOS; \
		$(INSTALL_DATA) $$game.app/Contents/Resources/$$game.icns $(DESTDIR)/Applications/$$game.app/Contents/Resources; \
	done

uninstall:
	echo "Removing examples programs from $(DESTDIR)$(docdir)/examples..."
	-$(RMDIR) $(DESTDIR)$(docdir)/examples

uninstall-linux:
	echo Removing games from $(DESTDIR)$(bindir)...
	for game in blocks checkers sudoku; do \
		$(RM) $(DESTDIR)$(bindir)/$$game; \
		$(RM) $(DESTDIR)/usr/share/applications/$$game.desktop; \
		$(RM) $(DESTDIR)/usr/share/icons/hicolor/32x32/apps/$$game.png; \
		$(RM) $(DESTDIR)/usr/share/icons/hicolor/128x128/apps/$$game.png; \
	done

uninstall-osx:
	echo Removing games from $(DESTDIR)/Applications...
	$(RMDIR) $(DESTDIR)/Applications/blocks.app
	$(RMDIR) $(DESTDIR)/Applications/checkers.app
	$(RMDIR) $(DESTDIR)/Applications/sudoku.app

# FLUID file rules
.fl.cxx .fl.h:	../fluid/fluid$(EXEEXT)
	echo Generating $@ and header from $<...
	../fluid/fluid$(EXEEXT) -c $<

# All demos depend on the FLTK library...
$(ALL): $(FLTK3_LIBNAME)

demo$(EXEEXT): demo.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ demo.o $(LINK_FLTK3) $(LDLIBS)

adjuster$(EXEEXT): adjuster.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ adjuster.o $(LINK_FLTK3) $(LDLIBS)

arc$(EXEEXT): arc.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ arc.o $(LINK_FLTK3) $(LDLIBS)

ask$(EXEEXT): ask.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ ask.o $(LINK_FLTK3) $(LDLIBS)

bitmap$(EXEEXT): bitmap.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ bitmap.o $(LINK_FLTK3) $(LDLIBS)

blocks$(EXEEXT): blocks.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ blocks.o $(LINK_FLTK3) $(AUDIOLIBS) $(LDLIBS)

boxtype$(EXEEXT): boxtype.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ boxtype.o $(LINK_FLTK3) $(LDLIBS)

browser$(EXEEXT): browser.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ browser.o $(LINK_FLTK3) $(LDLIBS)

button$(EXEEXT): button.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ button.o $(LINK_FLTK3) $(LDLIBS)

buttons$(EXEEXT): buttons.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ buttons.o $(LINK_FLTK3) $(LDLIBS)

checkers$(EXEEXT): checkers.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ checkers.o $(LINK_FLTK3) $(LDLIBS)

clock$(EXEEXT): clock.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ clock.o $(LINK_FLTK3) $(LDLIBS)

colbrowser$(EXEEXT): colbrowser.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ colbrowser.o $(LINK_FLTK3) $(LDLIBS)

color_chooser$(EXEEXT): color_chooser.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ color_chooser.o $(LINK_FLTK3) $(LDLIBS)

connect$(EXEEXT): connect.o $(FLTK3CONNECT_LIB_NAME) $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ connect.o $(LINK_FLTK3CONNECT) $(LINK_FLTK3) $(CONNECTLIB) $(LDLIBS)

cube$(EXEEXT): cube.o $(FLTK3_LIB_NAME) $(FLTK3GL_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ cube.o $(LINK_FLTK3) $(LINK_FLTK3GL) $(GLLIB) $(LDLIBS)

CubeView$(EXEEXT): CubeView.o CubeMain.o CubeViewUI.o $(FLTK3_LIB_NAME) $(FLTK3GL_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ CubeView.o CubeMain.o CubeViewUI.o $(LINK_FLTK3) $(LINK_FLTK3GL) $(GLLIB) $(LDLIBS)
CubeViewUI.o: CubeViewUI.h
CubeViewUI.cxx: CubeViewUI.fl ../fluid/fluid$(EXEEXT)

cursor$(EXEEXT): cursor.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ cursor.o $(LINK_FLTK3) $(LDLIBS)

curve$(EXEEXT): curve.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ curve.o $(LINK_FLTK3) $(LDLIBS)

device$(EXEEXT): device.o $(FLTK3IMAGES_LIB_NAME) $(FLTK3_LIB_NAME) $(FLTK3PNG_LIB_NAME) $(FLTK3JPEG_LIB_NAME) $(FLTK3ZLIB_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ device.o $(LINK_FLTK3IMAGES) $(LINK_FLTK3) $(LINK_FLTK3PNG) $(LINK_FLTK3JPEG) $(LINK_FLTK3ZLIB) $(LDLIBS)

doublebuffer$(EXEEXT): doublebuffer.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ doublebuffer.o $(LINK_FLTK3) $(LDLIBS)

editor$(EXEEXT): editor.o $(FLTK3IMAGES_LIB_NAME) $(FLTK3_LIB_NAME) $(FLTK3PNG_LIB_NAME) $(FLTK3JPEG_LIB_NAME) $(FLTK3ZLIB_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ editor.o $(LINK_FLTK3IMAGES) $(LINK_FLTK3) $(LINK_FLTK3PNG) $(LINK_FLTK3JPEG) $(LINK_FLTK3ZLIB) $(LDLIBS)

fast_slow$(EXEEXT): fast_slow.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ fast_slow.o $(LINK_FLTK3) $(LDLIBS)
fast_slow.o: fast_slow.h
fast_slow.cxx: fast_slow.fl ../fluid/fluid$(EXEEXT)

file_chooser$(EXEEXT): file_chooser.o $(FLTK3IMAGES_LIB_NAME) $(FLTK3_LIB_NAME) $(FLTK3PNG_LIB_NAME) $(FLTK3JPEG_LIB_NAME) $(FLTK3ZLIB_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ file_chooser.o $(LINK_FLTK3IMAGES) $(LINK_FLTK3) $(LINK_FLTK3PNG) $(LINK_FLTK3JPEG) $(LINK_FLTK3ZLIB) $(LDLIBS)

fonts$(EXEEXT): fonts.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ fonts.o $(LINK_FLTK3) $(LDLIBS)

fractals$(EXEEXT): fractals.o fracviewer.o $(FLTK3GL_LIB_NAME) $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ fractals.o fracviewer.o $(LINK_FLTK3GL) $(LINK_FLTK3) $(GLLIB) $(LDLIBS)

fullscreen$(EXEEXT): fullscreen.o $(FLTK3_LIB_NAME) $(FLTK3GL_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ fullscreen.o $(LINK_FLTK3) $(LINK_FLTK3GL) $(GLLIB) $(LDLIBS)

glpuzzle$(EXEEXT): glpuzzle.o $(FLTK3GL_LIB_NAME) $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ glpuzzle.o $(LINK_FLTK3GL) $(LINK_FLTK3) $(GLLIB) $(LDLIBS)

gl_overlay$(EXEEXT): gl_overlay.o $(FLTK3_LIB_NAME) $(FLTK3GL_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ gl_overlay.o $(LINK_FLTK3) $(LINK_FLTK3GL) $(GLLIB) $(LDLIBS)

hello$(EXEEXT): hello.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ hello.o $(LINK_FLTK3) $(LDLIBS)

help$(EXEEXT): help.o $(FLTK3IMAGES_LIB_NAME) $(FLTK3_LIB_NAME) $(FLTK3PNG_LIB_NAME) $(FLTK3JPEG_LIB_NAME) $(FLTK3ZLIB_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ help.o $(LINK_FLTK3IMAGES) $(LINK_FLTK3) $(LINK_FLTK3PNG) $(LINK_FLTK3JPEG) $(LINK_FLTK3ZLIB) $(LDLIBS)

iconize$(EXEEXT): iconize.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ iconize.o $(LINK_FLTK3) $(LDLIBS)

image$(EXEEXT): image.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ image.o $(LINK_FLTK3) $(LDLIBS)

inactive$(EXEEXT): inactive.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ inactive.o $(LINK_FLTK3) $(LDLIBS)
inactive.o: inactive.h
inactive.cxx: inactive.fl ../fluid/fluid$(EXEEXT)

input$(EXEEXT): input.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ input.o $(LINK_FLTK3) $(LDLIBS)

input_choice$(EXEEXT): input_choice.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ input_choice.o $(LINK_FLTK3) $(LDLIBS)

keyboard$(EXEEXT): keyboard.o keyboard_ui.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ keyboard.o keyboard_ui.o $(LINK_FLTK3) $(LDLIBS)
keyboard_ui.o: keyboard_ui.h
keyboard_ui.cxx: keyboard_ui.fl ../fluid/fluid$(EXEEXT)

label$(EXEEXT): label.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ label.o $(LINK_FLTK3) $(LDLIBS)

line_style$(EXEEXT): line_style.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ line_style.o $(LINK_FLTK3) $(LDLIBS)

list_visuals$(EXEEXT): list_visuals.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ list_visuals.o $(LINK_FLTK3) $(LDLIBS)

mandelbrot$(EXEEXT): mandelbrot.o mandelbrot_ui.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ mandelbrot.o mandelbrot_ui.o $(LINK_FLTK3) $(LDLIBS)
mandelbrot_ui.o: mandelbrot_ui.h
mandelbrot_ui.cxx: mandelbrot_ui.fl ../fluid/fluid$(EXEEXT)

menubar$(EXEEXT): menubar.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ menubar.o $(LINK_FLTK3) $(LDLIBS)

message$(EXEEXT): message.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ message.o $(LINK_FLTK3) $(LDLIBS)

minimum$(EXEEXT): minimum.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ minimum.o $(LINK_FLTK3) $(LDLIBS)

navigation$(EXEEXT): navigation.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ navigation.o $(LINK_FLTK3) $(LDLIBS)

native-filechooser$(EXEEXT): native-filechooser.o $(FLTK3IMAGES_LIB_NAME) $(FLTK3_LIB_NAME) $(FLTK3PNG_LIB_NAME) $(FLTK3JPEG_LIB_NAME) $(FLTK3ZLIB_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ native-filechooser.o $(LINK_FLTK3IMAGES) $(LINK_FLTK3) $(LINK_FLTK3PNG) $(LINK_FLTK3JPEG) $(LINK_FLTK3ZLIB) $(LDLIBS)

output$(EXEEXT): output.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ output.o $(LINK_FLTK3) $(LDLIBS)

overlay$(EXEEXT): overlay.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ overlay.o $(LINK_FLTK3) $(LDLIBS)

pack$(EXEEXT): pack.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ pack.o $(LINK_FLTK3) $(LDLIBS)

pixmap_browser$(EXEEXT): pixmap_browser.o $(FLTK3_LIB_NAME) $(FLTK3IMAGES_LIB_NAME) $(FLTK3PNG_LIB_NAME) $(FLTK3JPEG_LIB_NAME) $(FLTK3ZLIB_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ pixmap_browser.o $(LINK_FLTK3) $(LINK_FLTK3IMAGES) $(LINK_FLTK3PNG) $(LINK_FLTK3JPEG) $(LINK_FLTK3ZLIB) $(LDLIBS)

pixmap$(EXEEXT): pixmap.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ pixmap.o $(LINK_FLTK3) $(LDLIBS)

preferences$(EXEEXT): preferences.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ preferences.o $(LINK_FLTK3) $(LDLIBS)
preferences.o: preferences.h
preferences.cxx: preferences.fl ../fluid/fluid$(EXEEXT)

radio$(EXEEXT): radio.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ radio.o $(LINK_FLTK3) $(LDLIBS)
radio.o: radio.h
radio.cxx: radio.fl ../fluid/fluid$(EXEEXT)

resizebox$(EXEEXT): resizebox.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ resizebox.o $(LINK_FLTK3) $(LDLIBS)

resize$(EXEEXT): resize.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ resize.o $(LINK_FLTK3) $(LDLIBS)
resize.o: resize.h
resize.cxx: resize.fl ../fluid/fluid$(EXEEXT)

rotated_text$(EXEEXT): rotated_text.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ rotated_text.o $(LINK_FLTK3) $(LDLIBS)

scalebar$(EXEEXT): scalebar.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ scalebar.o $(LINK_FLTK3) $(LDLIBS)

scroll$(EXEEXT): scroll.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ scroll.o $(LINK_FLTK3) $(LDLIBS)

shape$(EXEEXT): shape.o $(FLTK3_LIB_NAME) $(FLTK3GL_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ shape.o $(LINK_FLTK3) $(LINK_FLTK3GL) $(GLLIB) $(LDLIBS)

shapedwindow$(EXEEXT): shapedwindow.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ shapedwindow.o $(LINK_FLTK3) $(LDLIBS)

subwindow$(EXEEXT): subwindow.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ subwindow.o $(LINK_FLTK3) $(LDLIBS)

sudoku$(EXEEXT): sudoku.o $(FLTK3IMAGES_LIB_NAME) $(FLTK3_LIB_NAME) $(FLTK3PNG_LIB_NAME) $(FLTK3JPEG_LIB_NAME) $(FLTK3ZLIB_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ sudoku.o $(LINK_FLTK3IMAGES) $(LINK_FLTK3) $(LINK_FLTK3PNG) $(LINK_FLTK3JPEG) $(LINK_FLTK3ZLIB) $(AUDIOLIBS) $(LDLIBS)

symbols$(EXEEXT): symbols.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ symbols.o $(LINK_FLTK3) $(LDLIBS)

table$(EXEEXT): table.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ table.o $(LINK_FLTK3) $(LDLIBS)

tabs$(EXEEXT): tabs.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ tabs.o $(LINK_FLTK3) $(LDLIBS)
tabs.o: tabs.h
tabs.cxx: tabs.fl ../fluid/fluid$(EXEEXT)

threads$(EXEEXT): threads.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ threads.o $(LINK_FLTK3) $(LDLIBS)

tile$(EXEEXT): tile.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ tile.o $(LINK_FLTK3) $(LDLIBS)

tiled_image$(EXEEXT): tiled_image.o $(FLTK3_LIB_NAME) $(FLTK3IMAGES_LIB_NAME) $(FLTK3PNG_LIB_NAME) $(FLTK3JPEG_LIB_NAME) $(FLTK3ZLIB_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ tiled_image.o $(LINK_FLTK3) $(LINK_FLTK3IMAGES) $(LINK_FLTK3PNG) $(LINK_FLTK3JPEG) $(LINK_FLTK3ZLIB) $(LDLIBS)

tree$(EXEEXT): tree.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ tree.o $(LINK_FLTK3) $(LDLIBS)
tree.o: tree.h
tree.cxx: tree.fl ../fluid/fluid$(EXEEXT)

unittests$(EXEEXT): unittests.o unittest_about.o unittest_circles.o unittest_images.o unittest_lines.o unittest_points.o unittest_rects.o unittest_scrollbarsize.o unittest_text.o unittest_viewport.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ unittests.o unittest_about.o unittest_circles.o unittest_images.o unittest_lines.o unittest_points.o unittest_rects.o unittest_scrollbarsize.o unittest_text.o unittest_viewport.o $(LINK_FLTK3) $(LDLIBS)

utf8$(EXEEXT): utf8.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ utf8.o $(LINK_FLTK3) $(LDLIBS)

valuators$(EXEEXT): valuators.o $(FLTK3_LIB_NAME)
	echo Linking $@...
	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ valuators.o $(LINK_FLTK3) $(LDLIBS)
valuators.o: valuators.h
valuators.cxx: valuators.fl ../fluid/fluid$(EXEEXT)


#
# End of "$Id$".
#
