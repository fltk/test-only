#
# "$Id: Makefile,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $"
#
# Postscript library makefile for the Fast Light Tool Kit (FLTK).
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

	
PSCPPFILES = \
	Fl_PS_Printer.cxx \
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


PSOBJECTS = $(PSCPPFILES:.cxx=.o)

all:	$(PSLIBNAME) $(PSDSONAME) 


# Posrscript targets

$(PSLIBNAME): $(PSOBJECTS)
	echo $(LIBCOMMAND) $@ ...
	-$(RM) $@
	$(LIBCOMMAND) $@ $(PSOBJECTS)
	$(RANLIB) $@

libfltk_ps.so.1.2: $(PSOBJECTS) ../libfltk.so.1.2
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(PSOBJECTS) $(PSLIBS) -L.. -lfltk
	-$(RM) libfltk_ps.so
	$(LN) libfltk_ps.so.1.2 libfltk_ps.so

libfltk_ps.sl.1.2: $(PSOBJECTS) ../libfltk.sl.1.2
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(PSOBJECTS) $(PSLIBS) -L.. -lfltk
	-$(RM) libfltk_ps.sl
	$(LN) libfltk_ps.sl.1.2 libfltk_ps.sl

libfltk_ps.1.2.dylib: $(PSOBJECTS) ../libfltk.1.2.dylib
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ \
		-install_name $(libdir)/$@ \
		-current_version 1.2.0 \
		-compatibility_version 1.2.0 \
		$(PSOBJECTS) $(LDLIBS) $(PSLIBS) -L.. -lfltk
	-$(RM) libfltk_ps.dylib
	$(LN) libfltk_ps.1.2.dylib libfltk_ps.dylib

libfltk_ps_s.a: $(PSOBJECTS)
	echo $(DSOCOMMAND) libfltk_ps_s.o ...
	$(DSOCOMMAND) libfltk_ps_s.o $(PSOBJECTS)
	echo $(LIBCOMMAND) libfltk_ps_s.a libfltk_ps_s.o
	-$(RM) $@
	$(LIBCOMMAND) libfltk_ps_s.a libfltk_ps_s.o
	$(CHMOD) +x libfltk_ps_s.a


	

clean :
	-@ $(RM) *.o  $PSDSONAME\
		../../lib/$(PSLIBNAME) \
		libfltk_ps.so\
		libfltk_ps.sl \
		libfltk_ps.dylib\

depend:	$(PSCPPFILES) 
	makedepend -Y -I../../ -f  makedepend $(PSCPPFILES) 

# Automatically generated dependencies...
include makedepend


################################################################

install: $(PSLIBNAME) $(PSDSONAME)
	echo "Installing fltk postscript libraries in $(libdir)..."
	-$(MKDIR) $(libdir)
	$(RM) $(libdir)/$(PSLIBNAME)
	-$(CP)  $(PSLIBNAME) $(libdir)


	if test x$(PSDSONAME) = xlibfltk_ps.so.1.2; then\
		$(RM) $(libdir)/libfltk_ps.so*;\
		$(CP) libfltk_ps.so.1.2 $(libdir); \
		$(CHMOD) 755 $(libdir)/libfltk_ps.so.1.2; \
		$(LN) libfltk_ps.so.1.2 $(libdir)/libfltk_ps.so;\
	fi
	if test x$(PSDSONAME) = xlibfltk_ps.sl.1.2; then\
		$(RM) $(libdir)/libfltk_ps.sl*;\
		$(CP) libfltk_ps.sl.1.2 $(libdir); \
		$(CHMOD) 755 $(libdir)/libfltk_ps.sl.1.2; \
		$(LN) libfltk_ps.sl.1.2 $(libdir)/libfltk_ps.sl;\
	fi
	if test x$(PSDSONAME) = xlibfltk_ps.1.2.dylib; then\
		$(RM) $(libdir)/libfltk_ps.*dylib;\
		$(CP) libfltk_ps.1.2.dylib $(libdir); \
		$(CHMOD) 755 $(libdir)/libfltk_ps.1.2.dylib; \
		$(LN) libfltk_ps.1.2.dylib $(libdir)/libfltk_ps.dylib;\
	fi
	if test x$(PSDSONAME) = xlibfltk_ps_s.a; then\
		$(RM) $(libdir)/libfltk_ps_s.a;\
		$(CP) libfltk_ps_s.a $(libdir); \
		$(CHMOD) 755 $(libdir)/libfltk_ps.a; \
	fi


uninstall:
	echo "Uninstalling postscript libraries..."
	$(RM) $(libdir)/$(LIBNAME)

	
	if test x$(PSLIBNAME) != x; then\
		$(RM) $(libdir)/$(PSLIBNAME);\
	fi
	if test x$(PSDSONAME) = xlibfltk_ps.so.1.2; then\
		$(RM) $(libdir)/libfltk_ps.so*;\
	fi
	if test x$(PSDSONAME) = xlibfltk_ps.sl.1.2; then\
		$(RM) $(libdir)/libfltk_ps.sl*;\
	fi
	if test x$(PSDSONAME) = xlibfltk_ps.1.2.dylib; then\
		$(RM) $(libdir)/libfltk_ps.*dylib;\
	fi
	if test x$(PSDSONAME) = xlibfltk_ps_s.a; then\
		$(RM) $(libdir)/libfltk_ps_s.a;\
	fi


#
# End of "$Id: Makefile,v 1.1.2.1 2004/03/28 10:30:31 rokan Exp $".
#
