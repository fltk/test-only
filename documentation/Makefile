#
# "$Id: Makefile,v 1.27 2004/08/27 15:07:44 spitzak Exp $"
#
# Documentation makefile for the Fast Light Tool Kit (FLTK).
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

include ../makeinclude

# The only target that probably works now is to run doxygen:
doxygen_output:
	-rm -rf html
	doxygen
	cp tiny.gif fltk_shadow.png doxygen.png html

#
# What media size to use:
#

# "Universal" size - minimum of US Letter and A4
MEDIA		=	--size universal

# Standard US Letter
#MEDIA		=	--size letter

# Standard A4
#MEDIA		=	--size a4

# 7x8.5in (1/2 legal page) with reduced-size fonts
#MEDIA		=	--size 7x8.5in --fontsize 9.0

# These are the HTML "source" files; the files are formatted in the
# order listed... DON'T REORDER THEM!
HTMLFILES	= \
		preface.html \
		intro.html \
		basics.html \
		common.html \
		editor.html \
		fluid.html \
		Fl.html \
		utility.html \
		events.html \
		drawing.html \
		opengl.html \
		widgets.html \
		Fl_Bar.html \
		Fl_Bitmap.html \
		Fl_Box.html \
		Fl_Boxtype.html \
		Fl_Browser.html \
		Fl_Button.html \
		Fl_Chart.html \
		Fl_Check_Button.html \
		Fl_Choice.html \
		Fl_Clock.html \
		Fl_Color_Chooser.html \
		Fl_Color.html \
		Fl_Dial.html \
		Fl_Double_Window.html \
		Fl_End.html \
		Fl_Float_Input.html \
		Fl_Font.html \
		Fl_Free.html \
		Fl_Gl_Window.html \
		Fl_Group.html \
		Fl_Image.html \
		Fl_Input.html \
		Fl_Int_Input.html \
		Fl_Item_Group.html \
		Fl_Item.html \
		Fl_Labeltype.html \
		Fl_Light_Button.html \
		Fl_List.html \
		Fl_Menu_Bar.html \
		Fl_Menu_Button.html \
		Fl_Menu_.html \
		Fl_Menu_Item.html \
		Fl_Menu_Window.html \
		Fl_Multi_Browser.html \
		Fl_Multi_Image.html \
		Fl_Multiline_Input.html \
		Fl_Multiline_Output.html \
		Fl_Named_Style.html \
		Fl_Numeric_Input.html \
		Fl_Output.html \
		Fl_Overlay_Window.html \
		Fl_Pack.html \
		Fl_Pixmap.html \
		Fl_Positioner.html \
		Fl_Repeat_Button.html \
		Fl_Return_Button.html \
		Fl_RGB_Image.html \
		Fl_Roller.html \
		Fl_Round_Button.html \
		Fl_Scrollbar.html \
		Fl_Scroll.html \
		Fl_Secret_Input.html \
		Fl_Slider.html \
		Fl_Style.html \
		Fl_Style_Set.html \
		Fl_Tabs.html \
		Fl_Text_Buffer.html \
		Fl_Text_Display.html \
		Fl_Text_Editor.html \
		Fl_Tile.html \
		Fl_Timer.html \
		Fl_Tooltip.html \
		Fl_Valuator.html \
		Fl_Value_Input.html \
		Fl_Value_Slider.html \
		Fl_Widget.html \
		Fl_Window.html \
		Fl_Wordwrap_Input.html \
		Fl_Wordwrap_Output.html \
		subclassing.html \
		theme.html \
		x.html \
		win32.html \
		forms.html \
		glut.html \
		license.html

MAN1FILES	= \
		fltk-config.man \
		fluid.man

MAN3FILES	= \
		fltk.man


#
# Make everything...
#

all:	fltk.ps fltk.pdf

alldocs:	fltk.ps fltk.pdf fltk.d/index.html


#
# Clean files...
#

clean:
	rm -f fltk.ps
	rm -f fltk.pdf


#
# Update dependencies...
#

depend:


#
# Install all documentation...
#

install:
# 	echo "Installing HTML documentation in $(docdir)"
# 	-$(MKDIR) -p $(docdir)
# 	$(CP) $(HTMLFILES) *.gif *.jpg index.html $(docdir)
# 	$(CHMOD) 644 $(docdir)/*
	echo "Installing man pages in $(mandir)"
	-$(MKDIR) -p $(mandir)/man1
	for file in $(MAN1FILES); do \
		$(CP) $$file $(mandir)/man1/`basename $$file .man`.1; \
	done
	-$(MKDIR) -p $(mandir)/man3
	for file in $(MAN3FILES); do \
		$(CP) $$file $(mandir)/man3/`basename $$file .man`.3; \
	done


#
# Uninstall docos...
#

uninstall:
# 	echo "Removing HTML documentation from $(docdir)"
# 	$(RM) -r $(docdir)
	echo "Removing man pages from $(mandir)"
	for file in $(MAN1FILES); do \
		$(RM) $(mandir)/man1/`basename $$file .man`.1; \
	done
	for file in $(MAN3FILES); do \
		$(RM) $(mandir)/man3/`basename $$file .man`.3; \
	done


#
# The base html files are now the readable ones, so the indexed HTML
# is normally not made.  This target is still used for the web site
# documentation...
#

fltk.d/index.html: $(HTMLFILES)
	echo "Generating HTML documentation..."
	-mkdir fltk.d
	-rm -f fltk.d/*
	$(HTMLDOC) -d fltk.d -t html --verbose --toclevels 2 --bodycolor white --titleimage fltk.gif $(HTMLFILES)

#
# The PostScript manual can be printed...
#

fltk.ps: $(HTMLFILES)
	@echo "Generating PostScript documentation..."
	@$(HTMLDOC) -f fltk.ps $(MEDIA) -t ps2 --duplex --verbose --toclevels 2 --titleimage fltk.gif $(HTMLFILES)

#
# The PDF manual can be printed or viewed...
#

fltk.pdf: $(HTMLFILES)
	@echo "Generating PDF documentation..."
	@rm -f fltk.pdf
	@$(HTMLDOC) -f fltk.pdf $(MEDIA) --jpeg --compression=9 --duplex --verbose --toclevels 2 --titleimage fltk.gif $(HTMLFILES)


#
# End of "$Id: Makefile,v 1.27 2004/08/27 15:07:44 spitzak Exp $".
#
