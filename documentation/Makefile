#
# "$Id: Makefile,v 1.21 2002/12/10 01:46:15 easysw Exp $"
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
# Please report all bugs and problems to "fltk-bugs@easysw.com".
#

# Where to find HTMLDOC...
HTMLDOC		=	htmldoc

# What media size to use:

# "Universal" size - minimum of US Letter and A4
MEDIA		=	--size universal

# Standard US Letter
#MEDIA		=	--size letter

# Standard A4
#MEDIA		=	--size a4

# 7x8.5in (1/2 legal page) with reduced-size fonts
#MEDIA		=	--size 7x8.5in --fontsize 9.0

# These are the HTML "source" files; the files are formatted in the
# order listed...
HTMLFILES	= \
	preface.html \
	intro.html \
	basics.html \
	common.html \
	editor.html \
	fluid.html \
	Fl.html \
	widgets.html \
	Fl_Adjuster.html \
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
	Fl_Color.html \
	Fl_Color_Chooser.html \
	Fl_Counter.html \
	Fl_Dial.html \
	Fl_Double_Window.html \
	Fl_End.html \
	Fl_Float_Input.html \
	Fl_Font.html \
	Fl_Gl_Window.html \
	Fl_Group.html \
	Fl_Image.html \
	Fl_Input.html \
	Fl_Int_Input.html \
	Fl_Item.html \
	Fl_Item_Group.html \
	Fl_Labeltype.html \
	Fl_Light_Button.html \
	Fl_List.html \
	Fl_Menu_.html \
	Fl_Menu_Bar.html \
	Fl_Menu_Button.html \
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
	Fl_RGB_Image.html \
	Fl_Return_Button.html \
	Fl_Roller.html \
	Fl_Round_Button.html \
	Fl_Scroll.html \
	Fl_Scrollbar.html \
	Fl_Secret_Input.html \
	Fl_Single_Window.html \
	Fl_Slider.html \
	Fl_Style.html \
	Fl_Style_Set.html \
	Fl_Tabs.html \
	Fl_Text_Buffer.html \
	Fl_Text_Display.html \
	Fl_Text_Editor.html \
	Fl_Tile.html \
	Fl_Valuator.html \
	Fl_Value_Input.html \
	Fl_Value_Output.html \
	Fl_Value_Slider.html \
	Fl_Widget.html \
	Fl_Window.html \
	Fl_Wordwrap_Input.html \
	utility.html \
	subclassing.html \
	events.html \
	drawing.html \
	opengl.html \
	theme.html \
	x.html \
	win32.html \
	forms.html \
	glut.html \
	license.html

all:	fltk.ps fltk.pdf

alldocs:	fltk.ps fltk.pdf fltk.d/index.html

clean:
	rm -f fltk.ps
	rm -f fltk.pdf

depend:

# According to FHS this should be:
# htmldir = $(prefix)/share/doc/html/en/fltk
# Possibly the "en/" can be removed, it is not clear.
# KDE uses "HTML" instead of "html", which is correct?
htmldir = $(prefix)/share/doc/fltk

# According to FHS this should be:
# mandir = $(prefix)/share/man/en/man
# they say the "en/" is optional
mandir = $(prefix)/share/man

install:
	-mkdir -p $(htmldir)
	cp $(HTMLFILES) *.gif *.jpg index.html $(htmldir)
	chmod 644 $(htmldir)/*
	-mkdir -p $(mandir)1
	cp *.1 $(mandir)1
	-mkdir -p $(mandir)3
	cp *.3 $(mandir)3

# Base html files are now the readable ones, so this target is not make by
# default...
fltk.d/index.html: $(HTMLFILES)
	echo "Generating HTML documentation..."
	-mkdir fltk.d
	-rm -f fltk.d/*
	$(HTMLDOC) -d fltk.d -t html --verbose --toclevels 2 --bodycolor white --titleimage FL.gif $(HTMLFILES)

fltk.ps: $(HTMLFILES)
	@echo "Generating PostScript documentation..."
	@$(HTMLDOC) -f fltk.ps $(MEDIA) -t ps2 --duplex --verbose --toclevels 2 --titleimage FL.gif $(HTMLFILES)

fltk.pdf: $(HTMLFILES)
	@echo "Generating PDF documentation..."
	@rm -f fltk.pdf
	@$(HTMLDOC) -f fltk.pdf $(MEDIA) --jpeg --compression=9 --duplex --verbose --toclevels 2 --titleimage FL.gif $(HTMLFILES)

#
# End of "$Id: Makefile,v 1.21 2002/12/10 01:46:15 easysw Exp $".
#
