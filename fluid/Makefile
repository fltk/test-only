#
# "$Id: Makefile,v 1.19 1999/09/19 05:01:41 vincent Exp $"
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

PROGRAM = fluid

CPPFILES = \
	Fl_Function_Type.cxx \
	Fl_Menu_Type.cxx \
	Fl_Group_Type.cxx \
	Fl_Widget_Type.cxx \
	Fl_Type.cxx \
	Fl_Window_Type.cxx \
	Fluid_Image.cxx \
	Fluid_Plugins.cxx \
	code.cxx \
	factory.cxx \
	file.cxx \
	fluid.cxx \
	about_panel.cxx \
	widget_panel.cxx \
	alignment_panel.cxx \
	function_panel.cxx

################################################################

OBJECTS = $(CPPFILES:.cxx=.o)
OBJECTSDLL = $(CPPFILES:.cxx=.odll)

include ../makeinclude

.SUFFIXES : .cxx .c .o .odll

.cxx.o :
	$(CXX) -I.. $(CXXFLAGS) -c $<

.cxx.odll :
	$(CXX) -DFL_DLL -I.. $(CXXFLAGS) -c $< -o $@
.c.o :
	$(CC) -I.. $(CFLAGS) -c $<
.cxx :
	$(CXX) -I.. $(CXXFLAGS) -o $@ $< -L../lib -lfltk $(LDLIBS)

$(PROGRAM) : $(OBJECTS) ../lib/$(LIBNAME)
	$(CXX) $(LDFLAGS) -o $(PROGRAM) $(OBJECTS) -L../lib -lfltk $(LDLIBS) $(LD_DLOPEN_FLAGS)

# for mingw32, to build the version of fluid that use the dll
fluid_dll.exe : $(OBJECTSDLL) ../lib/libfltkdll.2.a
	$(CXX) $(LDFLAGS) -o $(PROGRAM) $(OBJECTSDLL) -L../lib -lfltkdll.2 $(GLDLIBS) $(LD_DLOPEN_FLAGS)


clean :
	-@ rm -f *.o $(PROGRAM) $(CLEAN) core *~ makedepend
	@touch makedepend

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) $(CFILES) > makedepend
include makedepend

install: $(PROGRAM)
	@echo "Installing FLUID..."
	@strip $(PROGRAM)
	@-mkdir -p $(bindir)
	@cp $(PROGRAM) $(bindir)/$(PROGRAM)
	@chmod 755 $(bindir)/$(PROGRAM)

uninstall:
	-@ rm -f $(bindir)/$(PROGRAM)

#
# Note: The rebuild target can only be used if you have the original .fl
#       files.  This is normally only used by the FLTK maintainers...
#

rebuild:
	./fluid -c about_panel.fl
	./fluid -c alignment_panel.fl
	./fluid -c function_panel.fl
	./fluid -c widget_panel.fl

#
# End of "$Id: Makefile,v 1.19 1999/09/19 05:01:41 vincent Exp $".
#
