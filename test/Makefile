#
# "$Id: Makefile,v 1.51 2001/04/09 21:16:50 robertk Exp $"
#
# Test/example program makefile for the Fast Light Tool Kit (FLTK).
#
# Copyright 1998-1999 by Bill Spitzak and others.
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

CPPFILES =\
	CubeMain.cxx CubeView.cxx \
	adjuster.cxx arc.cxx ask.cxx bitmap.cxx boxtype.cxx browser.cxx button.cxx \
	buttons.cxx checkers.cxx clock.cxx color_chooser.cxx \
	cube.cxx cursor.cxx curve.cxx demo.cxx doublebuffer.cxx editor.cxx \
	file_chooser.cxx fonts.cxx fullscreen.cxx glpuzzle.cxx \
	gl_overlay.cxx hello.cxx iconize.cxx image.cxx input.cxx \
	keyboard.cxx label.cxx list_visuals.cxx mandelbrot.cxx menu.cxx \
	message.cxx navigation.cxx output.cxx overlay.cxx pixmap.cxx \
	pixmap_browser.cxx radio.cxx resizable.cxx scroll.cxx shape.cxx \
	subwindow.cxx symbols.cxx tabs.cxx tile.cxx fast_slow.cxx \
	resize.cxx pack.cxx inactive.cxx sizes.cxx image_file.cxx progress.cxx \
	layout.cxx threads.cxx fltk_scheme.cxx menubar.cxx line_style.cxx valuators.cxx

ALL =	CubeView adjuster arc ask bitmap boxtype browser button buttons checkers \
	clock color_chooser cube cursor curve demo doublebuffer \
	editor file_chooser fonts fullscreen gl_overlay glpuzzle \
	hello iconize image input keyboard label list_visuals \
	mandelbrot menu message navigation output overlay pixmap \
	pixmap_browser radio resizable scroll shape subwindow \
	symbols tabs tile fast_slow resize pack inactive sizes progress \
	image_file layout threads fltk_scheme menubar line_style valuators

all:	demos

static:

shared:

demos:	$(ALL)

programs: demos

include ../makeinclude

#.cxx:
#	$(CXX) $< `../fltk-config --cxxflags --ldflags` $(DEBUGFLAGS) -o $@
#
#.cxx.o:
#	$(CXX) `../fltk-config --cxxflags` $(DEBUGFLAGS) $< -c

.c.o:
	$(CC) -I.. $(EXEFLAGS) $(CFLAGS) $< -c

.cxx.o:
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< -c

.cxx:
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< $(LDLIBS) -o $@

$(ALL): $(FLLIB)

# FLUID file rules
.fl.cxx .fl.h:
	../fluid/fluid -c $<

# Other programs needing special "help"...
CubeView: CubeMain.o CubeView.o CubeViewUI.o $(FLGLLIB) $(FLLIB)
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) CubeMain.o CubeView.o CubeViewUI.o $(GLDLIBS) $(LDLIBS) -o $@
CubeMain.o: CubeViewUI.h CubeView.h
CubeView.o: CubeView.h

glpuzzle: glpuzzle.cxx $(FLGLLIB) $(FLLIB)
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< $(GLDLIBS) $(LDLIBS) -o $@
cube: cube.cxx $(FLGLLIB) $(FLLIB)
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< $(GLDLIBS) $(LDLIBS) -o $@
fullscreen: fullscreen.cxx $(FLGLLIB) $(FLLIB)
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< $(GLDLIBS) $(LDLIBS) -o $@
gl_overlay: gl_overlay.cxx $(FLGLLIB) $(FLLIB)
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< $(GLDLIBS) $(LDLIBS) -o $@
keyboard: keyboard.cxx keyboard_ui.cxx
mandelbrot: mandelbrot.cxx mandelbrot_ui.cxx
shape: shape.cxx $(FLGLLIB) $(FLLIB)
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< $(GLDLIBS) $(LDLIBS) -o $@
image_file: image_file.cxx $(FLIMAGESLIB) $(FLLIB)
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< $(IMAGELIBS) $(PNGLIBS) $(JPEGLIBS) $(LDLIBS) -o $@
threads: threads.cxx $(FLLIB)
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< $(THREADSLIBS) $(LDLIBS) -o $@

# If you have libjpeg installed, you might want to try this test program:

jpeg_image: jpeg_image.cxx
	$(CXX) -I.. $(CXXFLAGS) -I../../../local/jpeg-6b -L../../../local/jpeg-6b jpeg_image.cxx -L../lib -lfltk $(LDLIBS) $(IMAGELIBS) -lXext -o $@

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) > makedepend

clean:
	-@ rm -f $(ALL) *.exe jpeg_image *.o core *~

install:

install_static:

install_shared:

install_programs:

#
# End of "$Id: Makefile,v 1.51 2001/04/09 21:16:50 robertk Exp $".
#
