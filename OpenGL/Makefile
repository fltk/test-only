#
# "$Id: Makefile,v 1.20 2004/03/08 06:52:45 spitzak Exp $"
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

#
# Include common definitions...
#

include ../makeinclude


#
# Files for this directory...
#

CPPFILES = \
	Fl_Gl_Choice.cxx \
	Fl_Gl_Overlay.cxx \
	Fl_Gl_Window.cxx \
	gl_draw.cxx \
	gl_start.cxx

CFILES	=

LIBNAME	=	$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
DSONAME	=	$(DSOPREFIX)fltk_gl$(DSOSUFFIX)
DSOLINK = 	$(DSOPREFIX)fltk_gl$(DSOSYMLINK)
OBJECTS =	$(CPPFILES:.cxx=.o) $(CFILES:.c=.o)


#
# Make everything...
#

all:		$(LIBTARGET) $(DSOTARGET)

static:		../lib/$(LIBNAME)

shared:		../lib/$(DSONAME)


#
# Clean old files...
#

clean:
	$(RM) *.bck
	$(RM) *.o
	$(RM) core*
	$(RM) ../lib/$(LIBNAME)
	$(RM) ../lib/$(DSONAME)


#
# Make dependencies, excluding standard include directories...
#

depend:
	$(MAKEDEPEND) -fmakedepend -I.. $(CPPFILES) $(CFILES)


#
# Include automatically generated dependencies...
#

include makedepend


#
# Make static libraries...
#

../lib/$(LIBNAME): $(OBJECTS)
	$(RM) $@
	echo $(LIBCOMMAND) $@ ...
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

../lib/$(DSONAME): $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	$(RM) ../lib/$(DSOLINK)
	$(LN) $(DSONAME) ../lib/$(DSOLINK)


#
# Install everything...
#

install:
	echo "Installing static OpenGL library in $(libdir)"
	$(MKDIR) $(libdir)
	$(RM) $(libdir)/$(LIBNAME)
	$(CP) ../lib/$(LIBNAME) $(libdir)
	echo "Installing shared OpenGL library in $(libdir)"
	$(MKDIR) $(libdir)
	$(RM) $(libdir)/$(DSONAME)
	$(RM) $(libdir)/$(DSOLINK)
	$(CP) ../lib/$(DSONAME) $(libdir)
	$(LN) -s $(DSONAME) $(libdir)/$(DSOLINK)

#
# Uninstall the libraries...
#

uninstall:
	echo "Removing static OpenGL library from $(libdir)"
	$(RM) $(libdir)/$(LIBNAME)
	echo "Removing shared OpenGL library from $(libdir)"
	$(RM) $(libdir)/$(DSONAME)
	$(RM) $(libdir)/$(DSOLINK)


#
# End of "$Id: Makefile,v 1.20 2004/03/08 06:52:45 spitzak Exp $".
#
