#
# "$Id: Makefile,v 1.18 2002/12/13 19:42:57 easysw Exp $"
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
	Fl_Guess_Image.cxx \
	fl_jpeg.cxx\
	fl_png.cxx

CFILES	=

LIBNAME	=	../lib/$(LIBPREFIX)fltk_images$(LIBSUFFIX)
DSONAME	=	../lib/$(DSOPREFIX)fltk_images$(DSOSUFFIX)
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
	makedepend -Y -I.. -f makedepend $(CPPFILES) $(CFILES)


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

../lib/libfltk_images.so.2.0: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	-$(RM) ../lib/libfltk_images.so
	$(LN) libfltk_images.so.2.0 ../lib/libfltk_images.so

../lib/libfltk_images.sl.2.0: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	-$(RM) ../lib/libfltk_images.sl
	$(LN) libfltk_images.sl.2.0 ../lib/libfltk_images.sl

../lib/libfltk_images.2.0.dylib: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	-$(RM) ../lib/libfltk_images.dylib
	$(LN) libfltk_images.2.0.dylib ../lib/libfltk_images.dylib

../lib/libfltk_images_s.a: $(OBJECTS)
	echo $(DSOCOMMAND) ../lib/libfltk_images_s.o ...
	$(DSOCOMMAND) ../lib/libfltk_images_s.o $(OBJECTS) $(IMAGELIBS)
	echo $(LIBCOMMAND) ../lib/libfltk_images_s.a ../lib/libfltk_images_s.o
	-$(RM) $@
	$(LIBCOMMAND) ../lib/libfltk_images_s.a ../lib/libfltk_images_s.o
	$(CHMOD) +x ../lib/libfltk_images_s.a


#
# Install everything...
#

install:

#
# End of "$Id: Makefile,v 1.18 2002/12/13 19:42:57 easysw Exp $".
#
