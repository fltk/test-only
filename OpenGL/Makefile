#
# "$Id: Makefile,v 1.10 2001/11/14 09:21:42 spitzak Exp $"
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
	Fl_Gl_Choice.cxx \
	Fl_Gl_Overlay.cxx \
	Fl_Gl_Window.cxx \
	gl_draw.cxx \
	gl_start.cxx


DEMOS = 

################################################################

XTRAFLAGS = -DFL_GL_LIBRARY

include ../makeinclude

.c.o:
	$(CC) -I.. $(XTRAFLAGS) $(CFLAGS) $< -c

.cxx.o:
	$(CXX) -I.. $(XTRAFLAGS) $(CXXFLAGS) $< -c

.cxx:
	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< $(GLLIBS) $(LDLIBS) -o $@

%.shared.o : %.cxx
	$(CXX) -I.. $(XTRAFLAGS) $(DSOCFLAGS) $(CXXFLAGS) -c $< -o $@
%.shared.o : %.c
	$(CC) -I.. $(XTRAFLAGS) $(DSOCFLAGS) $(CFLAGS) -c $< -o $@

LIBRARY = ../lib/$(LIBNAME)_gl
DSO = ../lib/$(DSOLIBglNAME)
DSONAME = $(DSOLIBglNAME)$(DSOEXT)

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.c=.o)

all:	$(BUILDTARGETS)

static:	$(LIBRARY)$(LIBEXT)

shared:	$(DSO)$(DSOEXT)

programs: demos

demos: $(DEMOS)

$(DEMOS): $(FLGLLIB) $(FLLIB)

$(LIBRARY).a: $(OBJECTS)
	@rm -f $@
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

$(DSO).so.2 $(DSO).sl.2: $(OBJECTS:.o=.shared.o)
	@rm -f $@
	$(DSOCOMMAND) $(DSONAME) $(OBJECTS:.o=.shared.o) $(DSOLIBS)
	mv $(DSONAME) $@

DLLIMPORTLIBS = ../lib/$(LIBNAME).2.dll$(LIBEXT)
DLLIMPORTNAME = $(LIBNAME)_gl.2.dll$(LIBEXT)
$(DSO).2.dll: $(OBJECTS:.o=.shared.o)
	@rm -f $@
	$(DSOCOMMAND) $@ $(OBJECTS:.o=.shared.o) $(DSOLIBS) $(DLLIMPORTLIBS)
	mv $@.import ../lib/$(DLLIMPORTNAME)

#.fl.cxx .fl.h:
#	echo Generating $@...
#	../fluid/fluid -c $<

clean :
	@rm -f *.o $(DEMOS) $(LIBRARY)$(LIBEXT) $(DSO)$(DSOEXT) \
	       ../lib/$(DLLIMPORTNAME) core *~ makedepend
	@touch makedepend

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) > makedepend
	sed -e "s'\.o'\.shared\.o'" makedepend > foo
	cat foo >> makedepend
	rm foo

include makedepend

################################################################

install: $(INSTALLTARGETS)

install_static: static
	@echo "Installing static libraries..."
	@mkdir -p $(libdir)
	@rm -f $(libdir)/$(LIBNAME)_gl$(LIBEXT)
	@cp $(LIBRARY)$(LIBEXT) $(libdir)
	@-chmod 644 $(libdir)/$(LIBNAME)_gl$(LIBEXT) 

install_shared: shared
	@echo "Installing shared libraries..."
	@rm -f $(libdir)/$(DLLIMPORTNAME)
	@rm -f $(libdir)/$(DSOLIBglNAME)$(DSOEXT)
	@if [ "$(DSOSHORTEXT)" ]; then rm -f $(libdir)/$(DSOLIBglNAME)$(DSOSHORTEXT); fi
	@cp $(DSO)$(DSOEXT) $(libdir)
	@chmod 755 $(libdir)/$(DSOLIBglNAME)$(DSOEXT)
	@if [ "$(DSOSHORTEXT)" ]; then \
		ln -s $(DSOLIBglNAME)$(DSOEXT) $(libdir)/$(DSOLIBglNAME)$(DSOSHORTEXT); fi
	@if [ -f "../lib/$(DLLIMPORTNAME)" ]; then cp ../lib/$(DLLIMPORTNAME) $(libdir); fi

install_programs:

#
# End of "$Id: Makefile,v 1.10 2001/11/14 09:21:42 spitzak Exp $".
#
