#
# "$Id: Makefile,v 1.9 2000/06/10 04:10:27 carl Exp $"
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

.cxx:
	$(CXX) -I.. $(CXXFLAGS) $< $(GLUTLIBS) -o $@

LIBRARY = ../lib/$(LIBNAME)_glut
DSONAME = ../lib/$(DSOLIBglutNAME)
LIBRARIES = $(LIBRARY)$(LIBEXT) $(DSONAME)$(DSOEXT)

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.c=.o)

all:	$(LIBRARIES) $(DEMOS)

$(LIBRARY).a: $(OBJECTS)
	@rm -f $@
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

$(DSONAME).so.2 $(DSONAME).sl.2: $(OBJECTS:.o=.shared.o)
	@rm -f $@
	$(DSOCOMMAND) $@ $(OBJECTS:.o=.shared.o) $(DSOLIBS)

DLLIMPORTLIBS = ../lib/$(LIBNAME)_gl.2.dll$(LIBEXT) ../lib/$(LIBNAME).2.dll$(LIBEXT)
DLLIMPORTNAME = $(LIBNAME)_glut.2.dll$(LIBEXT)
$(DSONAME).2.dll: $(OBJECTS:.o=.shared.o)
	@rm -f $@
	$(DSOCOMMAND) $@ $(OBJECTS:.o=.shared.o) $(DSOLIBS) $(DLLIMPORTLIBS)
	mv $@.import ../lib/$(DLLIMPORTNAME)

$(DEMOS): $(LIBRARIES)

#.fl.cxx .fl.h:
#	echo Generating $@...
#	../fluid/fluid -c $<

clean :
	@rm -f *.o $(DEMOS) $(LIBRARY)$(LIBEXT) ../lib/$(DLLIMPORTNAME) core *~ makedepend
	@if [ -n "$(DSOEXT)" ]; then rm -f $(DSONAME)$(DSOEXT); fi
	@touch makedepend

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) > makedepend
include makedepend

################################################################

install: $(LIBRARIES)
	@echo "Installing libraries..."
	@mkdir -p $(libdir)
	@rm -f $(libdir)/$(LIBNAME)_glut$(LIBEXT)
	@rm -f $(libdir)/$(DLLIMPORTNAME)
	@if [ -n "$(DSOEXT)" ]; then rm -f $(libdir)/$(DSOLIBglutNAME)$(DSOEXT); fi
	@if [ -n "$(DSOSHORTEXT)" ]; then rm -f $(libdir)/$(DSOLIBglutNAME)$(DSOSHORTEXT); fi
	@cp $(LIBRARY)$(LIBEXT) $(libdir)
	@-chmod 644 $(libdir)/$(LIBNAME)_glut$(LIBEXT)
	@if [ -n "$(DSOEXT)" ]; then \
		cp $(DSONAME)$(DSOEXT) $(libdir); \
		chmod 755 $(libdir)/$(DSOLIBglutNAME)$(DSOEXT); \
		if [ -n "$(DSOSHORTEXT)" ]; then \
			ln -s $(DSOLIBglutNAME)$(DSOEXT) $(libdir)/$(DSOLIBglutNAME)$(DSOSHORTEXT); fi; \
	fi
	@if [ -f "../lib/$(DLLIMPORTNAME)" ]; then cp ../lib/$(DLLIMPORTNAME) $(libdir); fi

#
# End of "$Id: Makefile,v 1.9 2000/06/10 04:10:27 carl Exp $".
#
