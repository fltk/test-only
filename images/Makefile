#
# "$Id: Makefile,v 1.21 2002/12/22 05:30:22 easysw Exp $"
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
DSONAME	=	$(DSOPREFIX)fltk_images$(DSOSUFFIX)
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

libfltk_images.so.2.0: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	$(RM) libfltk_images.so
	$(LN) libfltk_images.so.2.0 libfltk_images.so

libfltk_images.sl.2.0: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	$(RM) libfltk_images.sl
	$(LN) libfltk_images.sl.2.0 libfltk_images.sl

libfltk_images.2.0.dylib: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS)
	$(RM) libfltk_images.dylib
	$(LN) libfltk_images.2.0.dylib libfltk_images.dylib

libfltk_images_s.a: $(OBJECTS)
	echo $(DSOCOMMAND) libfltk_images_s.o ...
	$(DSOCOMMAND) libfltk_images_s.o $(OBJECTS) $(IMAGELIBS)
	echo $(LIBCOMMAND) libfltk_images_s.a libfltk_images_s.o
	$(RM) $@
	$(LIBCOMMAND) libfltk_images_s.a libfltk_images_s.o
	$(CHMOD) +x libfltk_images_s.a


#
# Install everything...
#

install:

#
# End of "$Id: Makefile,v 1.21 2002/12/22 05:30:22 easysw Exp $".
#
# DO NOT DELETE

Fl_Guess_Image.o: ../config.h ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h
Fl_Guess_Image.o: ../fltk/FL_API.h ../fltk/SharedImage.h ../fltk/Image.h
Fl_Guess_Image.o: ../fltk/xbmImage.h ../fltk/x.h
fl_jpeg.o: ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h ../fltk/FL_API.h
fl_jpeg.o: ../fltk/x.h ../fltk/SharedImage.h ../fltk/Image.h ../config.h
fl_png.o: ../fltk/draw.h ../fltk/Flags.h ../fltk/Color.h ../fltk/FL_API.h
fl_png.o: ../fltk/x.h ../fltk/SharedImage.h ../fltk/Image.h ../config.h
