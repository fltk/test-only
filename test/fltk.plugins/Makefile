#
# "$Id: Makefile,v 1.6 1999/10/25 21:12:13 mike Exp $"
#
# FLTK-plugin makefile for the Fast Light Tool Kit (FLTK).
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

PROGRAM = essai.fltk-style-plugin
PROGRAM2 = essai.fltk-theme-plugin
PROGRAM3 = buildin.fltk-style-plugin

all	: $(PROGRAM) $(PROGRAM2) $(PROGRAM3)

CPPFILES = \
	style_plugin.cxx

CPPFILES2 = \
	theme_plugin.cxx

CPPFILES3 = \
	buildin_plugin.cxx

################################################################

OBJECTS = $(CPPFILES:.cxx=.o) fl_jpeg.o fl_draw_image.o
OBJECTS2 = $(CPPFILES2:.cxx=.o)
OBJECTS3 = $(CPPFILES3:.cxx=.o) fl_diamond_box.o \
	fl_rounded_box.o fl_round_box.o \
	fl_oval_box.o fl_shadow_box.o \
	fl_vertex.o

INCLUDES = -I../.. -I../../src

include ../../makeinclude

.SUFFIXES : .cxx .c .o

.cxx.o :
	$(CXX) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $<

$(PROGRAM) : $(OBJECTS) ../../lib/$(LIBNAME)
	$(CXX)  $(LD_DLL_FLAGS) $(CXXFLAGS) -o $(PROGRAM) $(OBJECTS) $(IMAGELIBS)

$(PROGRAM2) : $(OBJECTS2) ../../lib/$(LIBNAME)
	$(CXX) $(LD_DLL_FLAGS) $(CXXFLAGS) -o $(PROGRAM2) $(OBJECTS2) 

$(PROGRAM3) : $(OBJECTS3) ../../lib/$(LIBNAME)
	$(CXX) $(LD_DLL_FLAGS) $(CXXFLAGS) -o $(PROGRAM3) $(OBJECTS3)

fl_diamond_box.cxx:	../../src/fl_diamond_box.cxx
	cp ../../src/fl_diamond_box.cxx .

fl_rounded_box.cxx:	../../src/fl_rounded_box.cxx
	cp ../../src/fl_rounded_box.cxx .

fl_round_box.cxx:	../../src/fl_round_box.cxx
	cp ../../src/fl_round_box.cxx .

fl_oval_box.cxx:	../../src/fl_oval_box.cxx
	cp ../../src/fl_oval_box.cxx .

fl_shadow_box.cxx:	../../src/fl_shadow_box.cxx
	cp ../../src/fl_shadow_box.cxx .

fl_vertex.cxx:		../../src/fl_vertex.cxx
	cp ../../src/fl_vertex.cxx .

fl_jpeg.cxx:		../../src/fl_jpeg.cxx
	cp ../../src/fl_jpeg.cxx .

fl_draw_image.cxx:		../../src/fl_draw_image.cxx
	cp ../../src/fl_draw_image.cxx .

win32 : $(OBJECTS) $(OBJECTS2) ../../lib/$(LIBNAME)
	dllwrap $(CXXFLAGS) -o $(PROGRAM) $(OBJECTS) -L../../lib -lfltkdll.2 $(GLDLIBS)
	dllwrap $(CXXFLAGS) -o $(PROGRAM2) $(OBJECTS2) -L../../lib -lfltkdll.2 $(GLDLIBS)
	dllwrap $(CXXFLAGS) -o $(PROGRAM3) $(OBJECTS3) -L../../lib -lfltkdll.2 $(GLDLIBS)

clean :
	-@ rm -f *.o *.po $(PROGRAM) $(PROGRAM2) $(CLEAN) core *~ makedepend
	@touch makedepend

install : $(PROGRAM) $(PROGRAM2) $(PROGRAM3)
	mkdir -p $(libdir)/fltk/plugins
	mkdir -p $(libdir)/fltk/images
	rm -f $(libdir)/fltk/plugins/*
	rm -f $(libdir)/fltk/images/*
	cp *.fltk* $(libdir)/fltk/plugins
	cp ../images/bg*.jpeg $(libdir)/fltk/images
	cp ../fltkrc $(libdir)/fltk

depend:
	$(MAKEDEPEND) $(INCLUDES) $(CXXFLAGS) $(CPPFILES) $(CFILES) > makedepend
include makedepend

#
# End of "$Id: Makefile,v 1.6 1999/10/25 21:12:13 mike Exp $".
#
