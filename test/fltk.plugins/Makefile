#
# "$Id: Makefile,v 1.2 1999/09/27 18:28:11 vincent Exp $"
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

all : essai.fltk-style-plugin essai.fltk-theme-plugin

PROGRAM = essai.fltk-style-plugin
PROGRAM2 = essai.fltk-theme-plugin

CPPFILES = \
	style_plugin.cxx

CPPFILES2 = \
	theme_plugin.cxx

################################################################

OBJECTS = $(CPPFILES:.cxx=.o)
OBJECTS2 = $(CPPFILES2:.cxx=.o)

INCLUDES = -I../..

include ../../makeinclude

.SUFFIXES : .cxx .c .o

.cxx.o :
	$(CXX) -DFL_DLL -DFL_INPLUGIN -fPIC $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $<

$(PROGRAM) : $(OBJECTS) ../../lib/$(LIBNAME)
	$(CXX) $(LD_DLL_FLAGS) $(CXXFLAGS) -o $(PROGRAM) $(OBJECTS)

$(PROGRAM2) : $(OBJECTS2) ../../lib/$(LIBNAME)
	$(CXX) $(LD_DLL_FLAGS) $(CXXFLAGS) -o $(PROGRAM2) $(OBJECTS2)

win32 : $(OBJECTS) $(OBJECTS2) ../../lib/$(LIBNAME)
	dllwrap $(CXXFLAGS) -o $(PROGRAM) $(OBJECTS) -L../../lib -lfltkdll.2 $(GLDLIBS)
	dllwrap $(CXXFLAGS) -o $(PROGRAM2) $(OBJECTS2) -L../../lib -lfltkdll.2 $(GLDLIBS)

clean :
	-@ rm -f *.o $(PROGRAM) $(PROGRAM2) $(CLEAN) core *~ makedepend
	@touch makedepend

depend:
	$(MAKEDEPEND) $(INCLUDES) $(CXXFLAGS) $(CPPFILES) $(CFILES) > makedepend
include makedepend

#
# End of "$Id: Makefile,v 1.2 1999/09/27 18:28:11 vincent Exp $".
#
