#
# "$Id: Makefile,v 1.20 2000/06/06 13:44:40 carl Exp $"
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

THEMES=essai.theme motif.theme classic.theme windows.theme Alternative.theme KDE.theme KDE_colors.theme
SCHEMES=Alternative KDE Classic Motif OldMotif Windows Essai

################################################################

INCLUDES = -I..

include ../makeinclude
LIBRARY = ../lib/$(LIBNAME)_themes

CPPFILES = \
	fl_kde.cxx \
	fl_windows.cxx \
	fl_motif.cxx

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.c=.o)

all : $(LIBRARY).$(LIBEXT) $(THEMES)

.cxx.o :
	echo $(CXX) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $<
	$(CXX) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $<
.c.o :
	echo $(CC) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $<
	$(CC) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $<
%.po : ../src/%.cxx
	echo $(CXX) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $< -o $@
	$(CXX) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $< -o $@
%.po : ../images/%.cxx
	echo $(CXX) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $< -o $@
	$(CXX) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CXXFLAGS) $(SUPFLAGS) -c $< -o $@
%.po : ../src/%.c
	echo $(CC) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CFLAGS) $(SUPFLAGS) -c $< -o $@
	$(CC) -DFL_DLL -DFL_INPLUGIN $(INCLUDES) $(CFLAGS) $(SUPFLAGS) -c $< -o $@

$(LIBRARY).a: $(OBJECTS)
	@echo $(LIBCOMMAND) $@ $(OBJECTS)
	@rm -f $@
	@$(LIBCOMMAND) $@ $(OBJECTS)
	@$(RANLIB) $@

essai.theme : essai.o fl_jpeg.po Fl_Shared_Image.po fl_draw_image.po
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< fl_jpeg.po Fl_Shared_Image.po fl_draw_image.po -L../lib -ljpeg
	cp $@ ../config/themes

motif.theme : motif.o fl_motif.o ../lib/$(LIBNAME).$(LIBEXT)
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< fl_motif.o
	cp $@ ../config/themes

classic.theme : classic.o ../lib/$(LIBNAME).$(LIBEXT)
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $<
	cp $@ ../config/themes

windows.theme : windows.o fl_windows.o fl_engraved_label.po ../lib/$(LIBNAME).$(LIBEXT)
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< fl_windows.o fl_engraved_label.po
	cp $@ ../config/themes

Alternative.theme : Alternative.o fl_arci.po ../lib/$(LIBNAME).$(LIBEXT)
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< fl_arci.po
	cp $@ ../config/themes

KDE.theme : KDE.o fl_kde.o fl_list_fonts.po ../lib/$(LIBNAME).$(LIBEXT)
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< fl_kde.o fl_list_fonts.po
	cp $@ ../config/themes

KDE_colors.theme : KDE_colors.o fl_kde.o fl_list_fonts.po ../lib/$(LIBNAME).$(LIBEXT)
	$(CXX)  $(LD_PLUGIN_FLAGS) $(CXXFLAGS) -o $@ $< fl_kde.o fl_list_fonts.po
	cp $@ ../config/themes

# What is this?
win32 : $(OBJECTS) ../lib/$(LIBNAME).$(LIBEXT)
	dllwrap $(CXXFLAGS) -o $(PROGRAM) $(OBJECTS) -L../lib -lfltkdll.2 $(GLDLIBS)
	dllwrap $(CXXFLAGS) -o $(PROGRAM2) $(OBJECTS2) -L../lib -lfltkdll.2 $(GLDLIBS)
	dllwrap $(CXXFLAGS) -o $(PROGRAM3) $(OBJECTS3) -L../lib -lfltkdll.2 $(GLDLIBS)

clean :
	-@ rm -f *.o *.po $(LIBRARY).$(LIBEXT) $(THEMES) ../config/themes/*.theme core *~ makedepend
	@touch makedepend

install : all
	@echo "Installing library..."
	@-mkdir -p $(libdir)
	@rm -f $(libdir)/$(LIBNAME)_themes.$(LIBEXT)
	@-cp $(LIBRARY).$(LIBEXT) $(libdir)
	@-chmod 644 $(libdir)/$(LIBNAME)_themes.$(LIBEXT)
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
# End of "$Id: Makefile,v 1.20 2000/06/06 13:44:40 carl Exp $".
#
