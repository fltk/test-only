#
# "$Id: Makefile,v 1.32 2002/02/10 22:57:50 spitzak Exp $"
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

# * essai.theme removed from default list because it requires -ljpeg
#   and breaks the build on some platforms
THEMES = \
	Alternative.theme \
	classic.theme \
	KDE.theme \
	motif.theme \
	none.theme \
	scheme.theme \
	windows.theme

################################################################

include ../makeinclude

all : $(BUILDTARGETS)

static: $(LIBRARY)$(LIBEXT)

shared: $(THEMES)

programs:

$(LIBRARY).a: $(OBJECTS)
	@rm -f $@
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

ESSAI_OBJ = essai.shared.o
essai.theme : $(ESSAI_OBJ)
	$(THEMECOMMAND) $@ $(ESSAI_OBJ) ../lib/libfltk_images.so.2 -ljpeg -lpng $(DSOLIBS)

MOTIF_OBJ = motif.shared.o
motif.theme : $(MOTIF_OBJ)
	$(THEMECOMMAND) $@ $(MOTIF_OBJ) $(DSOLIBS) $(THEMELIBS)

CLASSIC_OBJ = classic.shared.o
classic.theme : $(CLASSIC_OBJ)
	$(THEMECOMMAND) $@ $(CLASSIC_OBJ) $(DSOLIBS) $(THEMELIBS)

WINDOWS_OBJ = windows.shared.o
windows.theme : $(WINDOWS_OBJ)
	$(THEMECOMMAND) $@ $(WINDOWS_OBJ) $(DSOLIBS) $(THEMELIBS)

ALT_OBJ = Alternative.shared.o
Alternative.theme : $(ALT_OBJ)
	$(THEMECOMMAND) $@ $(ALT_OBJ) $(DSOLIBS) $(THEMELIBS)

KDE_OBJ = KDE.shared.o conf.shared.o conf_get.shared.o conf_util.shared.o conf_list_free.shared.o conf_keys.shared.o
KDE.theme : $(KDE_OBJ)
	$(THEMECOMMAND) $@ $(KDE_OBJ) $(DSOLIBS) $(THEMELIBS)

SCHEME_OBJ = scheme.shared.o conf.shared.o conf_get.shared.o conf_util.shared.o conf_list_free.shared.o conf_keys.shared.o conf_sections.shared.o
scheme.theme : $(SCHEME_OBJ)
	$(THEMECOMMAND) $@ $(SCHEME_OBJ) $(DSOLIBS) $(THEMELIBS)

NONE_OBJ = none.shared.o
none.theme : $(NONE_OBJ)
	$(THEMECOMMAND) $@ $(NONE_OBJ) $(DSOLIBS) $(THEMELIBS)

clean :
	-@ rm -f *.shared.o $(LIBRARY)$(LIBEXT) *.theme *.plugin core *~ makedepend
	@touch makedepend

install : $(INSTALLTARGETS)

install_static:
# it would be nice if this built .o versions of the themes

install_shared:
	@echo "Installing themes..."
	@-mkdir -p $(libdir)/fltk
	@cp -f *.scheme $(libdir)/fltk
	@cp -f *.theme $(libdir)/fltk
	@cp -f *.plugin $(libdir)/fltk

install_programs:

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) $(CFILES) > makedepend
	sed -e "s'\.o'\.shared\.o'" makedepend > foo
	cat foo >> makedepend
	rm foo

include makedepend

#
# End of "$Id: Makefile,v 1.32 2002/02/10 22:57:50 spitzak Exp $".
#
