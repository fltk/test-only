#
# "$Id: Makefile,v 1.3 2000/06/06 13:44:30 carl Exp $"
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

include ../makeinclude

LIBRARY = ../lib/$(LIBNAME)_gl
DSONAME = ../lib/$(DSOLIBglNAME)
LIBRARIES = $(LIBRARY).$(LIBEXT) $(DSONAME).$(DSOEXT)

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.c=.o)

all:	$(LIBRARY).$(LIBEXT) $(DSONAME).$(DSOEXT) $(DEMOS)

$(LIBRARY).a: $(OBJECTS)
#	@echo $(LIBCOMMAND) $@ $(OBJECTS)
	@rm -f $@
	-@$(LIBCOMMAND) $@ $(OBJECTS)
	-@$(RANLIB) $@

$(DSONAME).so.2 $(DSONAME).sl.2: $(OBJECTS:.o=.shared.o)
	@echo $(DSOCOMMAND) $@ $(OBJECTS:.o=.shared.o)
	@$(DSOCOMMAND) $@ $(OBJECTS:.o=.shared.o)

$(DSONAME).2.dll: $(OBJECTS:.o=.dll.o)
	@echo $(DSOCOMMAND) $@ $(OBJECTS:.o=.dll.o) $(CXXFLAGS) $(GLDLIBS)
	@rm -f $@
	@$(DSOCOMMAND) -o $@ $(OBJECTS:.o=.dll.o) $(CXXFLAGS) $(GLDLIBS)

$(DEMOS): $(LIBRARIES)

#.fl.cxx .fl.h:
#	echo Generating $@...
#	../fluid/fluid -c $<

clean :
	@ rm -f *.o $(DEMOS) $(LIBRARY).$(LIBEXT) core *~ makedepend
	-@test -n "$(DSOEXT)" && rm -f $(DSONAME).$(DSOEXT)
	@touch makedepend

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) > makedepend

include makedepend

################################################################

install: $(LIBRARIES)
	@echo "Installing libraries..."
	@-mkdir -p $(libdir)
	@-rm -f $(libdir)/$(LIBNAME)_gl.$(LIBEXT)
	@-test -n "$(DSOEXT)" && rm -f $(libdir)/$(DSOLIBglNAME).$(DSOEXT)
	@-test -n "$(DSOSHORTEXT)" && rm -f $(libdir)/$(DSOLIBglNAME).$(DSOSHORTEXT)
	@-cp $(LIBRARY).$(LIBEXT) $(libdir)
	@-chmod 644 $(libdir)/$(LIBNAME)_gl.$(LIBEXT) 
	@if test -n "$(DSOEXT)"; then \
		cp $(DSONAME).$(DSOEXT) $(libdir); \
		chmod 755 $(libdir)/$(DSOLIBglNAME).$(DSOEXT); \
		test -n "$(DSOSHORTEXT)" && \
			ln -s $(DSOLIBglNAME).$(DSOEXT) $(libdir)/$(DSOLIBglNAME).$(DSOSHORTEXT); \
	fi

#
# End of "$Id: Makefile,v 1.3 2000/06/06 13:44:30 carl Exp $".
#
