#
# "$Id$"
#
# Gdi printing library makefile for the Fast Light Tool Kit (FLTK).
#
# Copyright 1998-2004 by Bill Spitzak and others.
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

	
GDICPPFILES = \
	Fl_GDI_Printer.cxx \
	arci.cxx \
	color.cxx \
	draw.cxx \
	font.cxx \
	image.cxx \
	line_style.cxx \
	rect.cxx \
	vertex.cxx 



CLEAN =	core* cmap

################################################################

UPPATH =  ../

include ../../makeinclude


GDIOBJECTS = $(GDICPPFILES:.cxx=.o)

all:	../../lib/$(GDILIBNAME)


# Gdi printer  targets

../../lib/$(GDILIBNAME): $(GDIOBJECTS)
	echo $(LIBCOMMAND) $@ ...
	-$(RM) $@
	$(LIBCOMMAND) $@ $(GDIOBJECTS)
	$(RANLIB) $@


	

clean :
	-@ $(RM) *.o \
		../../lib/$(GDILIBNAME)
		


depend:	$(GDICPPFILES) 
	makedepend -Y -I../../ -f  makedepend $(GDICPPFILES) 

# Automatically generated dependencies...
include makedepend


################################################################

install: ../../lib/$(GDILIBNAME)
	echo "Installing fltk gdi libraries in $(libdir)..."
	-$(MKDIR) $(libdir)
	$(RM) $(libdir)/$(GDILIBNAME)
	-$(CP)  ../../lib/$(GDILIBNAME) $(libdir)




uninstall:
	echo "Uninstalling GDI printing libraries..."
	$(RM) $(libdir)/$(LIBNAME)

	
	if test x$(libdir)/$(GDILIBNAME) != x; then\
		$(RM) $(libdir)/$(GDILIBNAME);\
	fi


#
# End of "$Id$".
#
