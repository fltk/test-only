#
# "$Id: Makefile,v 1.65 2003/01/14 06:51:02 spitzak Exp $"
#
# Test/example program makefile for the Fast Light Tool Kit (FLTK).
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

#
# Include common definitions...
#

include ../makeinclude


#
# FLUID file rules...
#

.fl.cxx .fl.h:
	../fluid/fluid$(EXEEXT) -c $<


#
# Files for this directory...
#

CPPFILES = \
	CubeMain.cxx \
	CubeView.cxx \
	adjuster.cxx \
	arc.cxx \
	ask.cxx \
	boxtype.cxx \
	browser.cxx \
	button.cxx \
	buttons.cxx \
	checkers.cxx \
	clock.cxx \
	color_chooser.cxx \
	cube.cxx \
	cursor.cxx \
	curve.cxx \
	demo.cxx \
	doublebuffer.cxx \
	editor.cxx \
	file_chooser.cxx \
	fonts.cxx \
	fullscreen.cxx \
	glpuzzle.cxx \
	gl_overlay.cxx \
	hello.cxx \
	iconize.cxx \
	image.cxx \
	input.cxx \
	keyboard.cxx \
	label.cxx \
	list_visuals.cxx \
	mandelbrot.cxx \
	menu.cxx \
	message.cxx \
	navigation.cxx \
	output.cxx \
	overlay.cxx \
	pixmap_browser.cxx \
	radio.cxx \
	resizable.cxx \
	scroll.cxx \
	shape.cxx \
	subwindow.cxx \
	symbols.cxx \
	tabs.cxx \
	tile.cxx \
	fast_slow.cxx \
	resize.cxx \
	pack.cxx \
	inactive.cxx \
	sizes.cxx \
	image_file.cxx \
	progress.cxx \
	layout.cxx \
	threads.cxx \
	menubar.cxx \
	line_style.cxx \
	valuators.cxx

CFILES	=

# only some of the demos have been changed to use namespaces:
TARGETS = \
	browser$(EXEEXT) \
	checkers$(EXEEXT) \
	boxtype$(EXEEXT) \
	color_chooser$(EXEEXT) \
	cursor$(EXEEXT) \
	demo$(EXEEXT) \
	file_chooser$(EXEEXT) \
	fonts$(EXEEXT) \
	gl_overlay$(EXEEXT) \
	hello$(EXEEXT) \
	image$(EXEEXT) \
	input$(EXEEXT) \
	keyboard$(EXEEXT) \
	label$(EXEEXT) \
	list_visuals$(EXEEXT) \
	menu$(EXEEXT) \
	shape$(EXEEXT) \
	tile$(EXEEXT) \
	inactive$(EXEEXT) \
	sizes$(EXEEXT) \
	pack$(EXEEXT) \
	threads$(EXEEXT) \
	valuators$(EXEEXT)

# ALL =	CubeView adjuster arc ask boxtype browser button buttons checkers \
# 	clock color_chooser cube cursor curve demo doublebuffer \
# 	editor file_chooser fonts fullscreen gl_overlay glpuzzle \
# 	hello iconize image input keyboard label list_visuals \
# 	mandelbrot menu message navigation output overlay \
# 	pixmap_browser radio resizable scroll shape subwindow \
# 	symbols tabs tile fast_slow resize pack inactive sizes progress \
# 	image_file layout threads menubar line_style valuators

#
# Make everything...
#

all:	$(TARGETS)


#
# Clean old files...
#

clean:
	$(RM) *.bck
	$(RM) *.o
	$(RM) core*
	$(RM) $(TARGETS)


#
# Make dependencies, excluding standard include directories...
#

depend:
	$(MAKEDEPEND) -I.. $(CPPFILES) $(CFILES) > makedepend


#
# Include automatically generated dependencies...
#

include makedepend


#
# All demo programs depend on the static library.
#

$(TARGETS):	../lib/$(LIBPREFIX)fltk$(LIBSUFFIX)


#
# Other programs needing special "help"...
#

CubeView$(EXEEXT): CubeMain.o CubeView.o CubeViewUI.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) CubeMain.o CubeView.o CubeViewUI.o \
		$(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

glpuzzle$(EXEEXT): glpuzzle.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

cube$(EXEEXT): cube.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

fullscreen$(EXEEXT): fullscreen.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

gl_overlay$(EXEEXT): gl_overlay.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

keyboard$(EXEEXT): keyboard.o keyboard_ui.o
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTK) $(LDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

mandelbrot$(EXEEXT): mandelbrot.o mandelbrot_ui.o
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTK) $(LDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

shape$(EXEEXT): shape.o ../lib/$(LIBPREFIX)fltk_gl$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKGL) $(GLDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

image_file$(EXEEXT): image_file.o ../lib/$(LIBPREFIX)fltk_images$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKIMG) $(LDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r

help$(EXEEXT): help.o ../lib/$(LIBPREFIX)fltk_images$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) -I.. $(CXXFLAGS) $< $(LINKFLTKIMG) $(LDLIBS) -o $@
	$(POSTBUILD) $@ ../FL/mac.r


install:


#
# End of "$Id: Makefile,v 1.65 2003/01/14 06:51:02 spitzak Exp $".
#
# DO NOT DELETE

CubeMain.o: ../config.h CubeViewUI.h CubeView.h ../fltk/gl.h ../fltk/Color.h
CubeMain.o: ../fltk/FL_API.h ../fltk/Flags.h
CubeView.o: CubeView.h ../config.h ../fltk/gl.h ../fltk/Color.h
CubeView.o: ../fltk/FL_API.h ../fltk/Flags.h
boxtype.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Window.h ../fltk/Group.h
boxtype.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
browser.o: ../fltk/Browser.h ../fltk/Menu.h ../fltk/Group.h ../fltk/Widget.h
browser.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
browser.o: ../fltk/Scrollbar.h ../fltk/Slider.h ../fltk/Valuator.h
browser.o: ../fltk/Window.h ../fltk/Button.h ../fltk/run.h ../fltk/events.h
browser.o: ../fltk/xpmImage.h ../fltk/Image.h ../fltk/Item.h
browser.o: ../fltk/ItemGroup.h ../fltk/CheckButton.h ../fltk/Button.h
browser.o: folder_small.xpm file_small.xpm
checkers.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/events.h
checkers.o: ../fltk/DoubleBufferWindow.h ../fltk/Window.h ../fltk/Group.h
checkers.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
checkers.o: ../fltk/xbmImage.h ../fltk/Image.h ../fltk/draw.h
checkers.o: ../FL/Fl_Menu_Item.h ../fltk/Widget.h ../fltk/ask.h
checkers.o: ../fltk/Cursor.h black_1.xbm black_2.xbm black_3.xbm black_4.xbm
checkers.o: white_1.xbm white_2.xbm white_3.xbm white_4.xbm blackking_1.xbm
checkers.o: blackking_2.xbm blackking_3.xbm blackking_4.xbm whiteking_1.xbm
checkers.o: whiteking_2.xbm whiteking_3.xbm whiteking_4.xbm ../fltk/visual.h
checkers.o: ../fltk/HorizontalSlider.h ../fltk/Slider.h ../fltk/Valuator.h
checkers.o: ../fltk/ValueInput.h ../fltk/FloatInput.h ../fltk/NumericInput.h
checkers.o: ../fltk/Input.h
clock.o: circle.xbm
color_chooser.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Window.h
color_chooser.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
color_chooser.o: ../fltk/Color.h ../fltk/Flags.h ../fltk/visual.h
color_chooser.o: ../fltk/Button.h ../fltk/show_colormap.h
color_chooser.o: ../fltk/ColorChooser.h ../fltk/Group.h ../fltk/Choice.h
color_chooser.o: ../fltk/Menu.h ../fltk/ValueInput.h ../fltk/Valuator.h
color_chooser.o: ../fltk/FloatInput.h ../fltk/NumericInput.h ../fltk/Input.h
color_chooser.o: ../fltk/rgbImage.h ../fltk/Image.h ../fltk/x.h
color_chooser.o: ../fltk/error.h ../fltk/draw.h list_visuals.cxx ../config.h
cube.o: ../config.h ../fltk/gl.h ../fltk/Color.h ../fltk/FL_API.h
cube.o: ../fltk/Flags.h
cursor.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/events.h ../fltk/Window.h
cursor.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
cursor.o: ../fltk/Flags.h ../fltk/Cursor.h
demo.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Window.h ../fltk/Group.h
demo.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
demo.o: ../fltk/Box.h ../fltk/Button.h ../fltk/filename.h ../fltk/error.h
doublebuffer.o: ../fltk/math.h
file_chooser.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Button.h
file_chooser.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
file_chooser.o: ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
file_chooser.o: ../fltk/Input.h ../fltk/file_chooser.h
fonts.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/DoubleBufferWindow.h
fonts.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
fonts.o: ../fltk/Color.h ../fltk/Flags.h ../fltk/Browser.h ../fltk/Menu.h
fonts.o: ../fltk/Scrollbar.h ../fltk/Slider.h ../fltk/Valuator.h
fonts.o: ../fltk/CheckButton.h ../fltk/Button.h ../fltk/draw.h ../fltk/Font.h
fonts.o: ../fltk/ask.h
fullscreen.o: ../config.h ../fltk/math.h ../fltk/gl.h ../fltk/Color.h
fullscreen.o: ../fltk/FL_API.h ../fltk/Flags.h
glpuzzle.o: ../fltk/gl.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
gl_overlay.o: ../config.h ../fltk/run.h ../fltk/FL_API.h ../fltk/Window.h
gl_overlay.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
gl_overlay.o: ../fltk/Color.h ../fltk/Flags.h ../fltk/HorizontalSlider.h
gl_overlay.o: ../fltk/Slider.h ../fltk/Valuator.h ../fltk/ToggleButton.h
gl_overlay.o: ../fltk/Button.h ../fltk/math.h ../fltk/gl.h ../fltk/GlWindow.h
gl_overlay.o: ../fltk/Window.h
hello.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
hello.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Widget.h
hello.o: ../fltk/run.h
image.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Window.h ../fltk/Group.h
image.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
image.o: ../fltk/Button.h list_visuals.cxx ../config.h ../fltk/x.h
image.o: ../fltk/visual.h ../fltk/xbmImage.h ../fltk/Image.h escherknot.xbm
image.o: ../fltk/xpmImage.h porsche.xpm ../fltk/rgbImage.h
image.o: ../fltk/ToggleButton.h ../fltk/Button.h ../fltk/CheckButton.h
image.o: ../fltk/Choice.h ../fltk/Menu.h ../fltk/Item.h
input.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Window.h ../fltk/Group.h
input.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
input.o: ../fltk/Input.h ../fltk/FloatInput.h ../fltk/NumericInput.h
input.o: ../fltk/Input.h ../fltk/IntInput.h ../fltk/FloatInput.h
input.o: ../fltk/SecretInput.h ../fltk/WordwrapInput.h ../fltk/Button.h
input.o: ../fltk/ToggleButton.h ../fltk/Button.h ../fltk/show_colormap.h
keyboard.o: keyboard_ui.cxx keyboard_ui.h ../fltk/Window.h ../fltk/Group.h
keyboard.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
keyboard.o: ../fltk/Flags.h ../fltk/Output.h ../fltk/Input.h ../fltk/Button.h
keyboard.o: ../fltk/Widget.h ../fltk/events.h ../fltk/run.h
label.o: ../FL/Fl_Menu_Item.h ../fltk/Widget.h ../fltk/run.h ../fltk/FL_API.h
label.o: ../fltk/DoubleBufferWindow.h ../fltk/Window.h ../fltk/Group.h
label.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
label.o: ../fltk/Box.h ../fltk/HorizontalValueSlider.h ../fltk/ValueSlider.h
label.o: ../fltk/Slider.h ../fltk/Valuator.h ../fltk/ToggleButton.h
label.o: ../fltk/Button.h ../fltk/Input.h ../fltk/Choice.h ../fltk/Menu.h
label.o: ../fltk/draw.h
list_visuals.o: ../config.h ../fltk/x.h ../fltk/Color.h ../fltk/FL_API.h
list_visuals.o: ../fltk/Flags.h ../fltk/run.h
mandelbrot.o: mandelbrot_ui.cxx mandelbrot_ui.h mandelbrot.h
menu.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Item.h ../fltk/ItemGroup.h
menu.o: ../fltk/Menu.h ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
menu.o: ../fltk/Color.h ../fltk/Flags.h ../fltk/Divider.h ../fltk/Window.h
menu.o: ../fltk/Box.h ../fltk/MenuBar.h ../fltk/PopupMenu.h
menu.o: ../fltk/InputBrowser.h ../fltk/Menu.h ../fltk/Input.h
menu.o: ../fltk/Choice.h ../fltk/Button.h ../fltk/draw.h
menu.o: ../fltk/HorizontalSlider.h ../fltk/Slider.h ../fltk/Valuator.h
menu.o: ../fltk/StringList.h ../fltk/ScreenInfo.h ../fltk/events.h
radio.o: radio.h
resizable.o: resizable.h
scroll.o: ../fltk/math.h
shape.o: ../config.h ../fltk/run.h ../fltk/FL_API.h ../fltk/Window.h
shape.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
shape.o: ../fltk/Flags.h ../fltk/HorizontalSlider.h ../fltk/Slider.h
shape.o: ../fltk/Valuator.h ../fltk/math.h ../fltk/gl.h ../fltk/GlWindow.h
shape.o: ../fltk/Window.h
tabs.o: tabs.h ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
tabs.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
tabs.o: ../fltk/TabGroup.h ../fltk/Group.h ../fltk/Input.h
tabs.o: ../fltk/WordwrapInput.h ../fltk/Input.h ../fltk/Button.h
tabs.o: ../fltk/Clock.h ../fltk/Widget.h ../fltk/ReturnButton.h
tabs.o: ../fltk/Button.h ../fltk/run.h
tile.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/DoubleBufferWindow.h
tile.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
tile.o: ../fltk/Color.h ../fltk/Flags.h ../fltk/TiledGroup.h ../fltk/Button.h
fast_slow.o: fast_slow.h
resize.o: resize.h
pack.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Window.h ../fltk/Group.h
pack.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
pack.o: ../fltk/Group.h ../fltk/Button.h ../fltk/PackedGroup.h
pack.o: ../fltk/BarGroup.h
inactive.o: inactive.h ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
inactive.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
inactive.o: ../fltk/Group.h ../fltk/Button.h ../fltk/CheckButton.h
inactive.o: ../fltk/Button.h ../fltk/Slider.h ../fltk/Valuator.h
inactive.o: ../fltk/Input.h ../fltk/InputBrowser.h ../fltk/Menu.h
inactive.o: ../fltk/Item.h ../fltk/Widget.h ../fltk/ValueInput.h
inactive.o: ../fltk/FloatInput.h ../fltk/NumericInput.h ../fltk/Input.h
inactive.o: ../fltk/Scrollbar.h ../fltk/Slider.h ../fltk/run.h
sizes.o: ../fltk/Widget.h ../fltk/Browser.h ../fltk/Menu.h ../fltk/Group.h
sizes.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
sizes.o: ../fltk/Flags.h ../fltk/Scrollbar.h ../fltk/Slider.h
sizes.o: ../fltk/Valuator.h ../fltk/Button.h ../fltk/CheckButton.h
sizes.o: ../fltk/Button.h ../fltk/LightButton.h ../fltk/CheckButton.h
sizes.o: ../fltk/ReturnButton.h ../fltk/Menu.h ../fltk/MenuBar.h
sizes.o: ../fltk/PopupMenu.h ../fltk/Choice.h ../fltk/Adjuster.h
sizes.o: ../fltk/Clock.h ../fltk/Group.h ../fltk/ColorChooser.h
sizes.o: ../fltk/ValueInput.h ../fltk/FloatInput.h ../fltk/NumericInput.h
sizes.o: ../fltk/Input.h ../fltk/Dial.h ../fltk/Valuator.h ../fltk/Window.h
sizes.o: ../fltk/Input.h ../fltk/Output.h ../fltk/DoubleBufferWindow.h
sizes.o: ../fltk/Window.h ../fltk/ThumbWheel.h ../fltk/ScrollGroup.h
sizes.o: ../fltk/Slider.h ../fltk/Scrollbar.h ../fltk/TabGroup.h
sizes.o: ../fltk/Style.h ../fltk/TextBuffer.h ../fltk/TextDisplay.h
sizes.o: ../fltk/TextBuffer.h ../fltk/TextEditor.h ../fltk/TextDisplay.h
sizes.o: ../fltk/Box.h
image_file.o: image_file.h
threads.o: ../config.h ../fltk/ask.h ../fltk/FL_API.h ../fltk/Style.h
threads.o: ../fltk/Color.h ../fltk/Flags.h
valuators.o: valuators.h ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
valuators.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
valuators.o: ../fltk/Widget.h ../fltk/Slider.h ../fltk/Valuator.h
valuators.o: ../fltk/ValueInput.h ../fltk/FloatInput.h ../fltk/NumericInput.h
valuators.o: ../fltk/Input.h ../fltk/Scrollbar.h ../fltk/Slider.h
valuators.o: ../fltk/ValueSlider.h ../fltk/ThumbWheel.h ../fltk/Group.h
valuators.o: ../fltk/Dial.h ../fltk/run.h
