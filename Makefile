#
# "$Id: Makefile,v 1.33 2004/08/04 20:49:17 laza2000 Exp $"
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
# Please report all bugs and problems to "fltk-bugs@fltk.org".
#

include makeinclude

DIRS = src $(LOCALIMAGES) images OpenGL fluid test

all: makeinclude
	for dir in $(DIRS); do\
		echo "=== making $$dir ===";\
		(cd $$dir; $(MAKE) $(MFLAGS)) || break;\
	done

install: makeinclude
	for dir in $(DIRS) documentation; do\
		echo "=== installing $$dir ===";\
		(cd $$dir; $(MAKE) $(MFLAGS) install) || break;\
	done

uninstall: makeinclude
	for dir in $(DIRS) documentation; do\
		echo "=== uninstalling $$dir ===";\
		(cd $$dir; $(MAKE) $(MFLAGS) uninstall) || break;\
	done

depend: makeinclude
	for dir in $(DIRS); do\
		echo "=== making dependencies in $$dir ===";\
		(cd $$dir; $(MAKE) $(MFLAGS) depend) || break;\
	done

clean:
	$(RM) core
	$(RM) core.* *.o
	for dir in $(DIRS); do\
		echo "=== cleaning $$dir ===";\
		(cd $$dir; $(MAKE) $(MFLAGS) clean) || break;\
	done

distclean: clean
	$(RM) config.*
	$(RM) fltk-config fltk.list makeinclude
	$(RM) FL/Makefile
	$(RM) documentation/*.$(CAT1EXT)
	$(RM) documentation/*.$(CAT3EXT)
	$(RM) documentation/fltk.pdf
	$(RM) documentation/fltk.ps
	$(RM) -r documentation/fltk.d
	for file in test/*.fl; do\
		$(RM) test/`basename $file .fl`.cxx; \
		$(RM) test/`basename $file .fl`.h; \
	done

makeinclude: configure configh.in makeinclude.in fltk/string.h.in
	if test -f config.status; then \
		./config.status --recheck; \
		./config.status; \
	else \
		./configure; \
	fi

configure: configure.in
	autoconf

portable-dist:
	epm -v fltk

native-dist:
	epm -v -f native fltk


#
# End of "$Id: Makefile,v 1.33 2004/08/04 20:49:17 laza2000 Exp $".
#
