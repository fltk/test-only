#
# "$Id: Makefile,v 1.3 1999/11/01 02:21:45 carl Exp $"
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

THEMES=essai motif classic windows Demo

all : $(THEMES)

################################################################

INCLUDES = -I..

include ../makeinclude

.SUFFIXES : .cxx .c .o

.cxx.o :
	$(CXX) -DFL_DLL -DFL_INPLUGIN -fPIC $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $<

%.po : ../src/%.cxx
	$(CXX) -DFL_DLL -DFL_INPLUGIN -fPIC $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $< -o $@

essai : essai.o fl_jpeg.po Fl_Shared_Image.po fl_draw_image.po
	$(CXX)  $(LD_DLL_FLAGS) $(CXXFLAGS) -o $@ $< fl_jpeg.po Fl_Shared_Image.po fl_draw_image.po -ljpeg

motif : motif.o ../lib/$(LIBNAME)
	$(CXX)  $(LD_DLL_FLAGS) $(CXXFLAGS) -o $@ $<

classic : classic.o ../lib/$(LIBNAME)
	$(CXX)  $(LD_DLL_FLAGS) $(CXXFLAGS) -o $@ $<

windows : windows.o ../lib/$(LIBNAME)
	$(CXX)  $(LD_DLL_FLAGS) $(CXXFLAGS) -o $@ $<

Demo : Demo.o ../lib/$(LIBNAME)
	$(CXX)  $(LD_DLL_FLAGS) $(CXXFLAGS) -o $@ $<

win32 : $(OBJECTS) $(OBJECTS2) ../lib/$(LIBNAME)
	dllwrap $(CXXFLAGS) -o $(PROGRAM) $(OBJECTS) -L../lib -lfltkdll.2 $(GLDLIBS)
	dllwrap $(CXXFLAGS) -o $(PROGRAM2) $(OBJECTS2) -L../lib -lfltkdll.2 $(GLDLIBS)
	dllwrap $(CXXFLAGS) -o $(PROGRAM3) $(OBJECTS3) -L../lib -lfltkdll.2 $(GLDLIBS)

clean :
	-@ rm -f *.o *.po $(THEMES) core *~ makedepend
	@touch makedepend

#depend:
#	$(MAKEDEPEND) $(INCLUDES) $(CXXFLAGS) $(CPPFILES) $(CFILES) > makedepend
#include makedepend

#
# End of "$Id: Makefile,v 1.3 1999/11/01 02:21:45 carl Exp $".
#
