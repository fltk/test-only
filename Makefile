# "$Id: Makefile,v 1.28 2002/12/10 01:46:14 easysw Exp $"
#
# Top-level makefile for the Fast Light Tool Kit (FLTK).
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
# Please report all bugs and problems to "fltk-bugs@easysw.com".
#

SHELL=/bin/sh

DIRS	= src images fluid OpenGL themes test
# forms and glut are removed for now

GENERATED = makeinclude fltk-config

all: $(GENERATED)
	@for dir in $(DIRS); do\
		if test ! -f $$dir/makedepend; then\
			touch $$dir/makedepend;\
		fi;\
		(cd $$dir; $(MAKE) $(MFLAGS)) || break;\
	done

static: $(GENERATED)
	@for dir in $(DIRS); do\
		if test ! -f $$dir/makedepend; then\
			touch $$dir/makedepend;\
		fi;\
		(cd $$dir; $(MAKE) $(MFLAGS) static) || break;\
	done

shared: $(GENERATED)
	@for dir in $(DIRS); do\
		if test ! -f $$dir/makedepend; then\
			touch $$dir/makedepend;\
		fi;\
		(cd $$dir; $(MAKE) $(MFLAGS) shared) || break;\
	done

install: $(GENERATED)
	@for dir in $(DIRS); do\
		if test ! -f $$dir/makedepend; then\
			touch $$dir/makedepend;\
		fi;\
		(cd $$dir; $(MAKE) $(MFLAGS) install) || break;\
	done

install_static: $(GENERATED)
	@for dir in $(DIRS); do\
		if test ! -f $$dir/makedepend; then\
			touch $$dir/makedepend;\
		fi;\
		(cd $$dir; $(MAKE) $(MFLAGS) install_static) || break;\
	done

install_shared: $(GENERATED)
	@for dir in $(DIRS); do\
		if test ! -f $$dir/makedepend; then\
			touch $$dir/makedepend;\
		fi;\
		(cd $$dir; $(MAKE) $(MFLAGS) install_shared) || break;\
	done

install_programs: $(GENERATED)
	@for dir in $(DIRS); do\
		if test ! -f $$dir/makedepend; then\
			touch $$dir/makedepend;\
		fi;\
		(cd $$dir; $(MAKE) $(MFLAGS) install_programs) || break;\
	done

depend: $(GENERATED)
	@for dir in $(DIRS); do\
		if test ! -f $$dir/makedepend; then\
			touch $$dir/makedepend;\
		fi;\
		(cd $$dir;$(MAKE) $(MFLAGS) depend) || break;\
	done

clean:
	-@ rm -f core *~ *.o *.bck
	@for dir in $(DIRS); do\
		(cd $$dir;$(MAKE) $(MFLAGS) clean) || break;\
	done

distclean: clean
	rm -f config.* makeinclude fltk-config

$(GENERATED) : configure configh.in makeinclude.in fltk-config.in
	./configure

configure: configure.in
	autoconf

portable-dist:
	epm -v fltk

native-dist:
	epm -v -f native fltk

#
# End of "$Id: Makefile,v 1.28 2002/12/10 01:46:14 easysw Exp $".
#
