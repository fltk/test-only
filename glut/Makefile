#
# "$Id: Makefile,v 1.14 2001/02/21 06:15:44 clip Exp $"
#
# Library makefile for the Fast Light Tool Kit (FLTK).
#
# Copyright 1998 by Bill Spitzak and others.
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

CPPFILES = \
	glut_compatability.cxx \
	glut_font.cxx


DEMOS = \
	glpuzzle fractals

################################################################

XTRAFLAGS = -DFL_GLUT_LIBRARY

include ../makeinclude

.c.o:
	$(CC) -I.. $(XTRAFLAGS) $(CFLAGS) $< -c

.cxx.o:
	$(CXX) -I.. $(XTRAFLAGS) $(CXXFLAGS) $< -c

.cxx:
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< $(GLUTLIBS) $(LDLIBS) -o $@

%.shared.o : %.cxx
	$(CXX) -I.. $(XTRAFLAGS) $(DSOCFLAGS) $(CXXFLAGS) -c $< -o $@
%.shared.o : %.c
	$(CC) -I.. $(XTRAFLAGS) $(DSOCFLAGS) $(CFLAGS) -c $< -o $@

LIBRARY = ../lib/$(LIBNAME)_glut
DSO = ../lib/$(DSOLIBglutNAME)
DSONAME = $(DSOLIBglutNAME)$(DSOEXT)

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.c=.o)

all:	static programs

static:	$(LIBRARY)$(LIBEXT)

shared:	$(DSO)$(DSOEXT)

demos: $(DEMOS)

programs: demos

$(DEMOS): $(FLGLUTLIB) $(FLGLLIB) $(FLLIB)

$(LIBRARY).a: $(OBJECTS)
	@rm -f $@
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

$(DSO).so.2 $(DSO).sl.2: $(OBJECTS:.o=.shared.o)
	@rm -f $@
	$(DSOCOMMAND) $(DSONAME) $(OBJECTS:.o=.shared.o) $(DSOLIBS)
	mv $(DSONAME) $@

DLLIMPORTLIBS = ../lib/$(LIBNAME)_gl.2.dll$(LIBEXT) ../lib/$(LIBNAME).2.dll$(LIBEXT)
DLLIMPORTNAME = $(LIBNAME)_glut.2.dll$(LIBEXT)
$(DSO).2.dll: $(OBJECTS:.o=.shared.o)
	@rm -f $@
	$(DSOCOMMAND) $@ $(OBJECTS:.o=.shared.o) $(DSOLIBS) $(DLLIMPORTLIBS)
	mv $@.import ../lib/$(DLLIMPORTNAME)

#.fl.cxx .fl.h:
#	echo Generating $@...
#	../fluid/fluid -c $<

clean :
	@rm -f *.o $(DEMOS) *.exe $(LIBRARY)$(LIBEXT) $(DSO)$(DSOEXT) \
	       ../lib/$(DLLIMPORTNAME) core *~ makedepend
	@touch makedepend

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) > makedepend
include makedepend

################################################################

install: install_static install_shared install_programs

install_static: static
	@echo "Installing static libraries..."
	@mkdir -p $(libdir)
	@rm -f $(libdir)/$(LIBNAME)_glut$(LIBEXT)
	@cp $(LIBRARY)$(LIBEXT) $(libdir)
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
# End of "$Id: Makefile,v 1.14 2001/02/21 06:15:44 clip Exp $".
#
