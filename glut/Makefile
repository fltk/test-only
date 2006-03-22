#
# "$Id$"
#
# Library makefile for the Fast Light Tool Kit (FLTK).
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

CPPFILES = \
	glut_compatability.cxx \
	glut_font.cxx


DEMOS = \
	glpuzzle fractals

################################################################


include ../makeinclude
DSOCFLAGS += -DFL_GLUT_LIBRARY
DSOCXXFLAGS += -DFL_GLUT_LIBRARY
LDLIBS := $(GLUTLIBS) $(LDLIBS)

LIBNAME	= ../lib/$(LIBPREFIX)fltk_glut$(LIBSUFFIX)

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.c=.o)

all:	static

static:	$(LIBNAME)

shared:	$(DSO)$(DSOEXT)

demos: $(DEMOS)

programs: demos

$(DEMOS): $(FLGLUTLIB) $(FLGLLIB) $(FLLIB)

$(LIBNAME): $(OBJECTS)
	@rm -f $@
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

$(DSO).so.2 $(DSO).sl.2: $(OBJECTS)
	@rm -f $@
	$(DSOCOMMAND) $(DSONAME) $(OBJECTS) $(DSOLIBS)
	mv $(DSONAME) $@

DLLIMPORTLIBS = ../lib/$(LIBNAME)_gl.2.dll$(LIBEXT) ../lib/$(LIBNAME).2.dll$(LIBEXT)
DLLIMPORTNAME = $(LIBNAME)_glut.2.dll$(LIBEXT)
$(DSO).2.dll: $(OBJECTS)
	@rm -f $@
	$(DSOCOMMAND) $@ $(OBJECTS) $(DSOLIBS) $(DLLIMPORTLIBS)
	mv $@.import ../lib/$(DLLIMPORTNAME)

#.fl.cxx .fl.h:
#	echo Generating $@...
#	../fluid/fluid -c $<

clean :
	@rm -f *.o $(DEMOS) *.exe $(LIBNAME) $(DSO)$(DSOEXT) \
	       ../lib/$(DLLIMPORTNAME) core *~ makedepend
	@touch makedepend

depend:	$(CPPFILES)
	$(MAKEDEPEND) -I.. $(CPPFILES) > makedepend

# Automatically generated dependencies...
include makedepend


################################################################

install: $(INSTALLTARGETS)

install_static: static
	@echo "Installing static libraries..."
	@mkdir -p $(libdir)
	@rm -f $(libdir)/$(LIBNAME)_glut$(LIBEXT)
	@cp $(LIBNAME) $(libdir)
	@-chmod 644 $(libdir)/$(LIBNAME)_glut$(LIBEXT) 

install_shared: shared
	@echo "Installing shared libraries..."
	@rm -f $(libdir)/$(DLLIMPORTNAME)
	@rm -f $(libdir)/$(DSOLIBglutNAME)$(DSOEXT)
	@if [ "$(DSOSHORTEXT)" ]; then rm -f $(libdir)/$(DSOLIBglutNAME)$(DSOSHORTEXT); fi
	@cp $(DSO)$(DSOEXT) $(libdir)
	@chmod 755 $(libdir)/$(DSOLIBglutNAME)$(DSOEXT)
	@if [ "$(DSOSHORTEXT)" ]; then \
		ln -s $(DSOLIBglutNAME)$(DSOEXT) $(libdir)/$(DSOLIBglutNAME)$(DSOSHORTEXT); fi
	@if [ -f "../lib/$(DLLIMPORTNAME)" ]; then cp ../lib/$(DLLIMPORTNAME) $(libdir); fi

install_programs:

#
# End of "$Id$".
#
# DO NOT DELETE

glut_compatability.o: ../config.h ../fltk/glut.h ../fltk/gl.h ../fltk/Color.h
glut_compatability.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/run.h
glut_compatability.o: ../fltk/events.h ../fltk/GlWindow.h ../fltk/Window.h
glut_compatability.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
glut_compatability.o: ../fltk/Rectangle.h ../fltk/Cursor.h ../fltk/visual.h
glut_compatability.o: ../fltk/Menu.h ../fltk/Item.h ../fltk/ItemGroup.h
glut_compatability.o: ../fltk/Menu.h ../fltk/Monitor.h
glut_font.o: ../config.h ../fltk/glut.h ../fltk/gl.h ../fltk/Color.h
glut_font.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/run.h ../fltk/events.h
glut_font.o: ../fltk/GlWindow.h ../fltk/Window.h ../fltk/Group.h
glut_font.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Rectangle.h
glut_font.o: ../fltk/Cursor.h ../fltk/visual.h ../fltk/gl.h
