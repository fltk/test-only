#
# "$Id: Makefile,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $"
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

include ../../makeinclude


GDIOBJECTS = $(GDICPPFILES:.cxx=.o)

all:	$(GDILIBNAME)


# Gdi printer  targets

$(GDILIBNAME): $(GDIOBJECTS)
	echo $(LIBCOMMAND) $@ ...
	-$(RM) $@
	$(LIBCOMMAND) $@ $(GDIOBJECTS)
	$(RANLIB) $@

libfltk_gdi.so.1.2: $(GDIOBJECTS) ../libfltk.so.1.2
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(GDIOBJECTS) $(GDILIBS) -L.. -lfltk
	-$(RM) libfltk_gdi.so
	$(LN) libfltk_gdi.so.1.2 libfltk_gdi.so

libfltk_gdi.sl.1.2: $(GDIOBJECTS) ../libfltk.sl.1.2
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(GDIOBJECTS) $(GDILIBS) -L.. -lfltk
	-$(RM) libfltk_gdi.sl
	$(LN) libfltk_gdi.sl.1.2 libfltk_hdi.sl

libfltk_gdi.1.2.dylib: $(GDIOBJECTS) ../libfltk.1.2.dylib
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ \
		-install_name $(libdir)/$@ \
		-current_version 1.2.0 \
		-compatibility_version 1.2.0 \
		$(GDIOBJECTS) $(LDLIBS) $(GDILIBS) -L.. -lfltk
	-$(RM) libfltk_gdi.dylib
	$(LN) libfltk_gdi.1.2.dylib libfltk_gdi.dylib

libfltk_gdi_s.a: $(GDIOBJECTS)
	echo $(DSOCOMMAND) libfltk_gdi_s.o ...
	$(DSOCOMMAND) libfltk_gdi_s.o $(GDIOBJECTS)
	echo $(LIBCOMMAND) libfltk_gdi_s.a libfltk_gdi_s.o
	-$(RM) $@
	$(LIBCOMMAND) libfltk_gdi_s.a libfltk_gdi_s.o
	$(CHMOD) +x libfltk_gdi_s.a


	

clean :
	-@ $(RM) *.o  $GDIDSONAME\
		../../lib/$(GDILIBNAME) \
		libfltk_gdi.so\
		libfltk_gdi.sl \
		libfltk_gdi.dylib\

depend:	$(GDICPPFILES) 
	makedepend -Y -I../../ -f  makedepend $(GDICPPFILES) 

# Automatically generated dependencies...
include makedepend


################################################################

install: $(GDILIBNAME) $(GDIDSONAME)
	echo "Installing fltk gdi libraries in $(libdir)..."
	-$(MKDIR) $(libdir)
	$(RM) $(libdir)/$(GDILIBNAME)
	-$(CP)  $(GDILIBNAME) $(libdir)


	if test x$(GDIDSONAME) = xlibfltk_gdi.so.1.2; then\
		$(RM) $(libdir)/libfltk_gdi.so*;\
		$(CP) libfltk_gdi.so.1.2 $(libdir); \
		$(CHMOD) 755 $(libdir)/libfltk_gdi.so.1.2; \
		$(LN) libfltk_gdi.so.1.2 $(libdir)/libfltk_gdi.so;\
	fi
	if test x$(GDIDSONAME) = xlibfltk_gdi.sl.1.2; then\
		$(RM) $(libdir)/libfltk_gdi.sl*;\
		$(CP) libfltk_gdi.sl.1.2 $(libdir); \
		$(CHMOD) 755 $(libdir)/libfltk_gdi.sl.1.2; \
		$(LN) libfltk_gdi.sl.1.2 $(libdir)/libfltk_gdi.sl;\
	fi
	if test x$(GDIDSONAME) = xlibfltk_gdi.1.2.dylib; then\
		$(RM) $(libdir)/libfltk_gdi.*dylib;\
		$(CP) libfltk_gdi.1.2.dylib $(libdir); \
		$(CHMOD) 755 $(libdir)/libfltk_gdi.1.2.dylib; \
		$(LN) libfltk_gdi.1.2.dylib $(libdir)/libfltk_gdi.dylib;\
	fi
	if test x$(GDIDSONAME) = xlibfltk_gdi_s.a; then\
		$(RM) $(libdir)/libfltk_gdi_s.a;\
		$(CP) libfltk_gdi_s.a $(libdir); \
		$(CHMOD) 755 $(libdir)/libfltk_gdi.a; \
	fi


uninstall:
	echo "Uninstalling GDI printing libraries..."
	$(RM) $(libdir)/$(LIBNAME)

	
	if test x$(GDILIBNAME) != x; then\
		$(RM) $(libdir)/$(GDILIBNAME);\
	fi
	if test x$(GDIDSONAME) = xlibfltk_gdi.so.1.2; then\
		$(RM) $(libdir)/libfltk_gdi.so*;\
	fi
	if test x$(GDIDSONAME) = xlibfltk_gdi.sl.1.2; then\
		$(RM) $(libdir)/libfltk_gdi.sl*;\
	fi
	if test x$(GDIDSONAME) = xlibfltk_gdi.1.2.dylib; then\
		$(RM) $(libdir)/libfltk_gdi.*dylib;\
	fi
	if test x$(GDIDSONAME) = xlibfltk_gdi_s.a; then\
		$(RM) $(libdir)/libfltk_gdi_s.a;\
	fi


#
# End of "$Id: Makefile,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $".
#
