#
# "$Id: Makefile,v 1.17 2002/12/18 20:13:15 easysw Exp $"
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

LIBNAME	=	../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
DSONAME	=	$(DSOPREFIX)fltk_gl$(DSOSUFFIX)
OBJECTS =	$(CPPFILES:.cxx=.o) $(CFILES:.c=.o)


#
# Make everything...
#

all:		$(LIBTARGET) $(DSOTARGET)

static:		$(LIBNAME)

shared:		$(DSONAME)


#
# Clean old files...
#

clean:
	$(RM) *.bck
	$(RM) *.o
	$(RM) core*
	$(RM) $(LIBNAME)
	$(RM) $(DSONAME)


#
# Make dependencies, excluding standard include directories...
#

depend:
	$(MAKEDEPEND) -I.. $(CPPFILES) $(CFILES) > makedepend


#
# Include automatically generated dependencies...
#

include makedepend


#
# Make static libraries...
#

$(LIBNAME):	$(OBJECTS)
	$(RM) $@
	echo $(LIBCOMMAND) $@ ...
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

libfltk_gl.so.2.0: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	$(RM) libfltk_gl.so
	$(LN) libfltk_gl.so.2.0 libfltk_gl.so

libfltk_gl.sl.2.0: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	$(RM) libfltk_gl.sl
	$(LN) libfltk_gl.sl.2.0 libfltk_gl.sl

libfltk_gl.2.0.dylib: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	$(RM) libfltk_gl.dylib
	$(LN) libfltk_gl.2.0.dylib libfltk_gl.dylib

libfltk_gl_s.a: $(OBJECTS)
	echo $(DSOCOMMAND) libfltk_gl_s.o ...
	$(DSOCOMMAND) libfltk_gl_s.o $(OBJECTS) $(IMAGELIBS)
	echo $(LIBCOMMAND) libfltk_gl_s.a libfltk_gl_s.o
	$(RM) $@
	$(LIBCOMMAND) libfltk_gl_s.a libfltk_gl_s.o
	$(CHMOD) +x libfltk_gl_s.a


#
# Install everything...
#

install: $(INSTALLTARGETS)


#
# End of "$Id: Makefile,v 1.17 2002/12/18 20:13:15 easysw Exp $".
#
