#
# "$Id: Makefile,v 1.7 2000/06/03 14:23:40 mike Exp $"
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
	Fl_Chart.cxx \
	Fl_Positioner.cxx \
	forms_bitmap.cxx \
	forms_compatability.cxx \
	forms_free.cxx \
	forms_fselect.cxx \
	forms_pixmap.cxx \
	forms_timer.cxx \
	fl_load_browser.cxx

DEMOS = \
	forms colbrowser

################################################################

include ../makeinclude

LIBRARY = ../lib/$(LIBNAME)_forms
DSONAME = $(DSOLIBformsNAME)$(DSOEXT)

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.c=.o)

all:	$(LIBRARY).a $(DSONAME) $(DEMOS)

$(LIBRARY).a: $(OBJECTS)
	@echo $(LIBCOMMAND) $@ ...
	@rm -f $@
	@$(LIBCOMMAND) $@ $(OBJECTS)
	@$(RANLIB) $@

$(LIBRARY).so.2 $(LIBRARY).sl.2: $(OBJECTS)
	@echo $(DSOCOMMAND) $@ ...
	@$(DSOCOMMAND) $@ $(OBJECTS)
	@touch $(DSOEXT)

clean :
	-@ rm -f *.o *.do $(LIBRARY).a $(DEMOS) core *~ makedepend
	@touch makedepend

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) > makedepend
include makedepend

################################################################

install: $(LIBRARY).a
	@echo "Installing libraries..."
	@-mkdir -p $(libdir)
	@rm -f $(libdir)/$(LIBRARY).a
	@-cp $(LIBRARY).a $(libdir)
	@-chmod 644 $(libdir)/$(LIBRARY).a
	@if test -n "$(DSOEXT)"; then \
		if test "$(DSONAME)" = $(LIBRARY).so.2; then ext=.so; else ext=.s1; fi; \
		rm -f $(libdir)/$(LIBRARY)$$ext.2;\
		rm -f $(libdir)/$(LIBRARY)$$ext;\
		cp $(DSONAME) $(libdir)/$(LIBRARY)$$ext.2; \
		chmod 755 $(libdir)/$(LIBRARY)$$ext.2; \
		ln -s $(libdir)/$(LIBRARY)$$ext.2 $(libdir)/$(LIBRARY)$$ext;\
	fi

#
# End of "$Id: Makefile,v 1.7 2000/06/03 14:23:40 mike Exp $".
#
