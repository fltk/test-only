#
# "$Id: Makefile,v 1.25 2000/09/27 16:25:52 spitzak Exp $"
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

# essai.theme removed from default list because it requires -ljpeg
# and breaks the build on some platforms
THEMES = motif.theme classic.theme windows.theme Alternative.theme \
         KDE.theme KDE_colors.theme
SCHEMES = Alternative KDE Classic Motif OldMotif Windows

################################################################

XTRAFLAGS =

include ../makeinclude

%.shared.o : %.cxx
	$(CXX) -I.. $(XTRAFLAGS) $(DSOCFLAGS) $(CXXFLAGS) -c $< -o $@
%.shared.o : %.c
	$(CC) -I.. $(XTRAFLAGS) $(DSOCFLAGS) $(CFLAGS) -c $< -o $@

LIBRARY = ../lib/$(LIBNAME)_themes

CPPFILES = \
	fl_kde.cxx \
	fl_windows.cxx \
	fl_motif.cxx

OBJECTS = $(CPPFILES:.cxx=.shared.o) $(CFILES:.c=.shared.o)

all : static shared programs

static: $(LIBRARY)$(LIBEXT)

shared: $(THEMES)

demos:

programs: demos

$(LIBRARY).a: $(OBJECTS)
	@rm -f $@
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

ESSAI_OBJ = essai.shared.o
essai.theme : $(ESSAI_OBJ)
	$(THEMECOMMAND) $@ $(ESSAI_OBJ) -ljpeg $(DSOLIBS)
	cp $@ ../config/themes
	cp bg*.jpeg ../config/themes


MOTIF_OBJ = motif.shared.o fl_motif.shared.o
motif.theme : $(MOTIF_OBJ)
	$(THEMECOMMAND) $@ $(MOTIF_OBJ) $(DSOLIBS) $(THEMELIBS)
	cp $@ ../config/themes

CLASSIC_OBJ = classic.shared.o
classic.theme : $(CLASSIC_OBJ)
	$(THEMECOMMAND) $@ $(CLASSIC_OBJ) $(DSOLIBS) $(THEMELIBS)
	cp $@ ../config/themes

WINDOWS_OBJ = windows.shared.o fl_windows.shared.o
windows.theme : $(WINDOWS_OBJ)
	$(THEMECOMMAND) $@ $(WINDOWS_OBJ) $(DSOLIBS) $(THEMELIBS)
	cp $@ ../config/themes

ALT_OBJ = Alternative.shared.o
Alternative.theme : $(ALT_OBJ)
	$(THEMECOMMAND) $@ $(ALT_OBJ) $(DSOLIBS) $(THEMELIBS)
	cp $@ ../config/themes

KDE_OBJ = KDE.shared.o fl_kde.shared.o
KDE.theme : $(KDE_OBJ)
	$(THEMECOMMAND) $@ $(KDE_OBJ) $(DSOLIBS) $(THEMELIBS)
	cp $@ ../config/themes

KDEC_OBJ = KDE_colors.shared.o fl_kde.shared.o
KDE_colors.theme : $(KDEC_OBJ)
	$(THEMECOMMAND) $@ $(KDEC_OBJ) $(DSOLIBS) $(THEMELIBS)
	cp $@ ../config/themes

clean :
	-@ rm -f *.shared.o $(LIBRARY)$(LIBEXT) $(THEMES) ../config/themes/*.theme core *~ makedepend
	@touch makedepend

install : install_static install_shared install_programs

install_static:
	@echo "Installing static library..."
	@-mkdir -p $(libdir)
	@rm -f $(libdir)/$(LIBNAME)_themes$(LIBEXT)
	@-cp $(LIBRARY)$(LIBEXT) $(libdir)
	@-chmod 644 $(libdir)/$(LIBNAME)_themes$(LIBEXT)

install_shared:
	@echo "Installing themes..."
	@-mkdir -p $(libdir)/fltk/schemes
	@-mkdir -p $(libdir)/fltk/themes
	@(cd ../config/schemes; cp -f $(SCHEMES) $(libdir)/fltk/schemes)
	@cp -f $(THEMES) $(libdir)/fltk/themes

install_programs:

depend:
	$(MAKEDEPEND) -I.. $(XTRAFLAGS) $(CXXFLAGS) $(CPPFILES) $(CFILES) > makedepend
include makedepend

#
# End of "$Id: Makefile,v 1.25 2000/09/27 16:25:52 spitzak Exp $".
#
