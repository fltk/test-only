#
# "$Id: Makefile,v 1.8 2000/06/18 11:05:36 vincent Exp $"
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
	Fl_Guess_Image.cxx \
	fl_jpeg.cxx\
	fl_png.cxx

DEMOS = \


################################################################

XTRAFLAGS = -DFL_IMAGES_LIBRARY

include ../makeinclude

.c.o:
	$(CC) -I.. $(XTRAFLAGS) $(CFLAGS) $< -c

.cxx.o:
	$(CXX) -I.. $(XTRAFLAGS) $(CXXFLAGS) $< -c

#.cxx:
#	$(CXX) -I.. $(EXEFLAGS) $(CXXFLAGS) $< $(IMAGESLIBS) -o $@

LIBRARY = ../lib/$(LIBNAME)_images
DSO = ../lib/$(DSOLIBimagesNAME)
DSONAME = $(DSOLIBimagesNAME)$(DSOEXT)
LIBRARIES = $(LIBRARY)$(LIBEXT) $(DSO)$(DSOEXT)

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.c=.o)

all:	$(LIBRARIES) $(DEMOS)

$(LIBRARY).a: $(OBJECTS)
	@rm -f $@
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

$(DSO).so.2 $(DSO).sl.2: $(OBJECTS:.o=.shared.o)
	@rm -f $@
	$(DSOCOMMAND) $(DSONAME) $(OBJECTS:.o=.shared.o) $(DSOLIBS)
	mv $(DSONAME) $@

DLLIMPORTLIBS = ../lib/$(LIBNAME).2.dll$(LIBEXT)
DLLIMPORTNAME = $(LIBNAME)_images.2.dll$(LIBEXT)
$(DSO).2.dll: $(OBJECTS:.o=.shared.o)
	@rm -f $@
	$(DSOCOMMAND) $@ $(OBJECTS:.o=.shared.o) $(DSOLIBS) $(DLLIMPORTLIBS)
	mv $@.import ../lib/$(DLLIMPORTNAME)

$(DEMOS): $(LIBRARIES)

.fl.cxx .fl.h:
	echo Generating $@...
	../fluid/fluid -c $<

clean :
	@rm -f *.o $(DEMOS) $(LIBRARY)$(LIBEXT) ../lib/$(DLLIMPORTNAME) core *~ makedepend
	@if [ -n "$(DSOEXT)" ]; then rm -f $(DSO)$(DSOEXT); fi
	@touch makedepend

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) > makedepend
include makedepend

################################################################

install: $(LIBRARIES)
	@echo "Installing libraries..."
	@mkdir -p $(libdir)
	@rm -f $(libdir)/$(LIBNAME)_images$(LIBEXT)
	@rm -f $(libdir)/$(DLLIMPORTNAME)
	@if [ -n "$(DSOEXT)" ]; then rm -f $(libdir)/$(DSOLIBimagesNAME)$(DSOEXT); fi
	@if [ -n "$(DSOSHORTEXT)" ]; then rm -f $(libdir)/$(DSOLIBimagesNAME)$(DSOSHORTEXT); fi
	@cp $(LIBRARY)$(LIBEXT) $(libdir)
	@-chmod 644 $(libdir)/$(LIBNAME)_images$(LIBEXT)
	@if [ -n "$(DSOEXT)" ]; then \
		cp $(DSO)$(DSOEXT) $(libdir); \
		chmod 755 $(libdir)/$(DSOLIBimagesNAME)$(DSOEXT); \
		if [ -n "$(DSOSHORTEXT)" ]; then \
			ln -s $(DSOLIBimagesNAME)$(DSOEXT) $(libdir)/$(DSOLIBimagesNAME)$(DSOSHORTEXT); fi; \
	fi
	@if [ -f "../lib/$(DLLIMPORTNAME)" ]; then cp ../lib/$(DLLIMPORTNAME) $(libdir); fi

#
# End of "$Id: Makefile,v 1.8 2000/06/18 11:05:36 vincent Exp $".
#
