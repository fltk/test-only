#
# "$Id: Makefile,v 1.8 1999/09/24 16:03:43 vincent Exp $"
#
# Fluid makefile for the Fast Light Tool Kit (FLTK).
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

PROGRAM = essai.fluidplugin

CPPFILES = \
	essai_plugin.cxx

################################################################

OBJECTS = $(CPPFILES:.cxx=.o)

INCLUDES = -I../../fluid -I../..

include ../../makeinclude

.SUFFIXES : .cxx .c .o

.cxx.o :
	$(CXX) -DFL_DLL -DFLUID_PLUGIN -fPIC $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $<

$(PROGRAM) : $(OBJECTS) ../../lib/$(LIBNAME)
	$(CXX) $(LD_DLL_FLAGS) $(CXXFLAGS) -o $(PROGRAM) $(OBJECTS)

win32 : $(OBJECTS) ../../lib/$(LIBNAME)
	dllwrap $(CXXFLAGS) -o $(PROGRAM) $(OBJECTS) -L../../lib -lfltkdll.2 -lfluid $(GLDLIBS) --output-def essai_plugin.def

clean :
	-@ rm -f *.o $(PROGRAM) $(CLEAN) core *~ makedepend
	@touch makedepend

depend:
	$(MAKEDEPEND) $(INCLUDES) $(CXXFLAGS) $(CPPFILES) $(CFILES) > makedepend
include makedepend

#
# End of "$Id: Makefile,v 1.8 1999/09/24 16:03:43 vincent Exp $".
#
