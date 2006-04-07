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

#
# Include common definitions...
#

include ../makeinclude

# If ZLIBINC is set, we are using local ZLIB
ifneq ( ,$(ZLIBINC))
	CFLAGS += -I./zlib
	CXXFLAGS += -I./zlib
endif

#
# Files for this directory...
#

CPPFILES = \
	Fl_Guess_Image.cxx \
	fl_jpeg.cxx\
	fl_png.cxx \
	pnmImage.cxx \
	xpmFileImage.cxx \
	images_core.cxx \
	FileIcon2.cxx \
	HelpDialog.cxx
CFILES	=

LIBNAME	=	$(LIBPREFIX)fltk_images$(LIBSUFFIX)
DSONAME	=	$(DSOPREFIX)fltk_images$(DSOSUFFIX)
DSOLINK = 	$(DSOPREFIX)fltk_images$(DSOSYMLINK)
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
        ifeq ($(DSONAME), fltk_images.dll)
	$(RM) lib$(DSONAME).a $(DSONAME)
        endif


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

../lib/fltk_images.dll: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS) $(IMAGELIBS) $(LOCAL_IMAGELIBS) $(LINKFLTK) $(LDLIBS)
	cp ../lib/fltk_images.dll ./

#
# Install everything...
#

install:
        ifneq (,$(wildcard ../lib/$(LIBNAME)))
	echo "Installing static images library in $(libdir)"
	$(MKDIR) $(DESTDIR)$(libdir)
	$(RM) $(DESTDIR)$(libdir)/$(LIBNAME)
	$(CP) ../lib/$(LIBNAME) $(DESTDIR)$(libdir)
	$(RM) -f $(DESTDIR)$(libdir)/$(LIBPREFIX)fltk_z$(LIBSUFFIX)
	$(RM) -f $(DESTDIR)$(libdir)/$(LIBPREFIX)fltk_png$(LIBSUFFIX)
	$(RM) -f $(DESTDIR)$(libdir)/$(LIBPREFIX)fltk_jpeg$(LIBSUFFIX)
  ifneq (,$(LOCAL_IMAGELIBS))
	  $(CP) $(LOCAL_IMAGELIBS) $(DESTDIR)$(libdir)
  endif
        endif
        ifneq (,$(wildcard ../lib/$(DSONAME)))
	echo "Installing shared images library in $(libdir)"
	$(MKDIR) $(DESTDIR)$(libdir)
	$(RM) $(DESTDIR)$(libdir)/$(DSONAME)
	$(RM) $(DESTDIR)$(libdir)/$(DSOLINK)
	$(CP) ../lib/$(DSONAME) $(DESTDIR)$(libdir)
	$(LN) -s $(DSONAME) $(DESTDIR)$(libdir)/$(DSOLINK)
        endif
        ifeq ($(DSONAME), fltk_images.dll)
	$(CP) lib$(DSONAME).a $(DESTDIR)$(libdir)
        endif
#
# Uninstall the libraries...
#

uninstall:
	echo "Removing static images library from $(libdir)"
	$(RM) $(libdir)/$(LIBNAME)
	echo "Removing shared images library from $(libdir)"
	$(RM) $(libdir)/$(DSONAME)
	$(RM) $(libdir)/$(DSOLINK)
        ifeq ($(DSONAME), fltk_images.dll)
	$(RM) $(libdir)/lib$(DSONAME).a
        endif

#
# End of "$Id$".
#
