#
# "$Id: Makefile,v 1.67 2003/03/31 07:17:48 spitzak Exp $"
#
# Test/example program makefile for the Fast Light Tool Kit (FLTK).
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
# FLUID file rules...
#

.fl.cxx .fl.h:
	../fluid/fluid$(EXEEXT) -c $<


#
# Files for this directory...
#

CPPFILES = \
	CubeMain.cxx \
	CubeView.cxx \
	adjuster.cxx \
	arc.cxx \
	ask.cxx \
	boxtype.cxx \
	browser.cxx \
	button.cxx \
	buttons.cxx \
	checkers.cxx \
	clock.cxx \
	color_chooser.cxx \
	cube.cxx \
	cursor.cxx \
	curve.cxx \
	demo.cxx \
	doublebuffer.cxx \
	editor.cxx \
	file_chooser.cxx \
	fonts.cxx \
	fullscreen.cxx \
	glpuzzle.cxx \
	gl_overlay.cxx \
	hello.cxx \
	iconize.cxx \
	image.cxx \
	input.cxx \
	keyboard.cxx \
	label.cxx \
	list_visuals.cxx \
	mandelbrot.cxx \
	menu.cxx \
	message.cxx \
	navigation.cxx \
	output.cxx \
	overlay.cxx \
	pixmap_browser.cxx \
	radio.cxx \
	resizable.cxx \
	scroll.cxx \
	shape.cxx \
	subwindow.cxx \
	symbols.cxx \
	tabs.cxx \
	tile.cxx \
	fast_slow.cxx \
	resize.cxx \
	pack.cxx \
	inactive.cxx \
	sizes.cxx \
	image_file.cxx \
	progress.cxx \
	layout.cxx \
	threads.cxx \
	menubar.cxx \
	line_style.cxx \
	valuators.cxx

CFILES	=

# only some of the demos have been changed to use namespaces:
TARGETS = \
	arc$(EXEEXT) \
	browser$(EXEEXT) \
	checkers$(EXEEXT) \
	boxtype$(EXEEXT) \
	color_chooser$(EXEEXT) \
	cursor$(EXEEXT) \
	demo$(EXEEXT) \
	file_chooser$(EXEEXT) \
	fonts$(EXEEXT) \
	gl_overlay$(EXEEXT) \
	hello$(EXEEXT) \
	image$(EXEEXT) \
	input$(EXEEXT) \
	keyboard$(EXEEXT) \
	label$(EXEEXT) \
	list_visuals$(EXEEXT) \
	menu$(EXEEXT) \
	shape$(EXEEXT) \
	tile$(EXEEXT) \
	inactive$(EXEEXT) \
	sizes$(EXEEXT) \
	pack$(EXEEXT) \
	threads$(EXEEXT) \
	valuators$(EXEEXT)

# ALL =	CubeView adjuster arc ask boxtype browser button buttons checkers \
# 	clock color_chooser cube cursor curve demo doublebuffer \
# 	editor file_chooser fonts fullscreen gl_overlay glpuzzle \
# 	hello iconize image input keyboard label list_visuals \
# 	mandelbrot menu message navigation output overlay \
# 	pixmap_browser radio resizable scroll shape subwindow \
# 	symbols tabs tile fast_slow resize pack inactive sizes progress \
# 	image_file layout threads menubar line_style valuators

#
# Make everything...
#

all:	$(TARGETS)


#
# Clean old files...
#

clean:
	$(RM) *.bck
	$(RM) *.o
	$(RM) core*
	$(RM) $(TARGETS)


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
# All demo programs depend on the static library.
#

$(TARGETS):	../lib/$(LIBPREFIX)fltk$(LIBSUFFIX)


#
# Other programs needing special "help"...
#

CubeView$(EXEEXT): CubeMain.o CubeView.o CubeViewUI.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) CubeMain.o CubeView.o CubeViewUI.o \
		$(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

glpuzzle$(EXEEXT): glpuzzle.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

cube$(EXEEXT): cube.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

fullscreen$(EXEEXT): fullscreen.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

gl_overlay$(EXEEXT): gl_overlay.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

keyboard$(EXEEXT): keyboard.o keyboard_ui.o
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTK) $(LDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

mandelbrot$(EXEEXT): mandelbrot.o mandelbrot_ui.o
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTK) $(LDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

shape$(EXEEXT): shape.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

image_file$(EXEEXT): image_file.o ../lib/$(LIBPREFIX)fltk_images$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKIMG) $(LDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

help$(EXEEXT): help.o ../lib/$(LIBPREFIX)fltk_images$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKIMG) $(LDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r


install:
uninstall:


#
# End of "$Id: Makefile,v 1.67 2003/03/31 07:17:48 spitzak Exp $".
#
