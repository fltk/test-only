#
# "$Id: Makefile,v 1.10 1999/11/21 04:23:09 vincent Exp $"
#
# FLTK-plugin makefile for the Fast Light Tool Kit (FLTK).
#
# Copyright 1998-1999 by Bill Spitzak and others.
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

THEMES=essai.fltp motif.fltp classic.fltp windows.fltp Alternative.fltp KDE1.fltp KDE1_colors.fltp
SCHEMES=Alternative KDE1 Classic Motif OldMotif Windows Essai

all : $(THEMES)

################################################################

INCLUDES = -I..

include ../makeinclude

.SUFFIXES : .cxx .c .o

.cxx.o :
	$(CXX) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $<

%.po : ../src/%.cxx
	$(CXX) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $< -o $@
%.po : ../images/%.cxx
	$(CXX) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $< -o $@

essai.fltp : essai.o fl_jpeg.po Fl_Shared_Image.po fl_draw_image.po
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< fl_jpeg.po Fl_Shared_Image.po fl_draw_image.po -ljpeg

motif.fltp : motif.o fl_motif.po ../lib/$(LIBNAME).a
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< fl_motif.po

classic.fltp : classic.o ../lib/$(LIBNAME).a
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $<

windows.fltp : windows.o fl_windows.po fl_engraved_label.po ../lib/$(LIBNAME).a
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< fl_windows.po fl_engraved_label.po

Alternative.fltp : Alternative.o ../lib/$(LIBNAME).a
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< 

KDE1.fltp : KDE1.o fl_kde1.po fl_kde1_colors.po fl_set_fonts.po fl_motif.po fl_windows.po fl_engraved_label.po ../lib/$(LIBNAME).a
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< fl_kde1.po fl_kde1_colors.po fl_set_fonts.po fl_motif.po fl_windows.po fl_engraved_label.po

KDE1_colors.fltp : KDE1_colors.o fl_kde1_colors.po fl_set_fonts.po ../lib/$(LIBNAME).a
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< fl_kde1_colors.po fl_set_fonts.po

win32 : $(OBJECTS) $(OBJECTS2) ../lib/$(LIBNAME).a
	dllwrap $(CXXFLAGS) -o $(PROGRAM) $(OBJECTS) -L../lib -lfltkdll.2 $(GLDLIBS)
	dllwrap $(CXXFLAGS) -o $(PROGRAM2) $(OBJECTS2) -L../lib -lfltkdll.2 $(GLDLIBS)
	dllwrap $(CXXFLAGS) -o $(PROGRAM3) $(OBJECTS3) -L../lib -lfltkdll.2 $(GLDLIBS)

clean :
	-@ rm -f *.o *.po $(THEMES) core *~ makedepend
	@touch makedepend

install :
	@echo "Installing themes..."
	@-mkdir -p $(libdir)/fltk
	@-mkdir -p $(libdir)/fltk/schemes
	@-mkdir -p $(libdir)/fltk/themes
	(cd ../config/schemes; cp -f $(SCHEMES) $(libdir)/fltk/schemes)
	cp -f $(THEMES) $(libdir)/fltk/themes

#depend:
#	$(MAKEDEPEND) $(INCLUDES) $(CXXFLAGS) $(CPPFILES) $(CFILES) > makedepend
#include makedepend

#
# End of "$Id: Makefile,v 1.10 1999/11/21 04:23:09 vincent Exp $".
#
