#
# "$Id: Makefile,v 1.47 2002/12/22 05:30:22 easysw Exp $"
#
# Fluid makefile for the Fast Light Tool Kit (FLTK).
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
# Files for this directory...
#

CPPFILES = \
	about_panel.cxx \
	alignment_panel.cxx \
	code.cxx \
	coding_style.cxx \
	coding_style_func.cxx \
	factory.cxx \
	file.cxx \
	Fl_Function_Type.cxx \
	Fl_Group_Type.cxx \
	Fl_Menu_Type.cxx \
	Fl_Type.cxx \
	fluid.cxx \
	Fluid_Image.cxx \
	Fluid_Plugins.cxx \
	Fl_Widget_Type.cxx \
	Fl_Window_Type.cxx \
	function_panel.cxx \
	widget_panel.cxx

CFILES	=

OBJECTS = $(CPPFILES:.cxx=.o) $(CFILES:.cxx=.o)
TARGETS	= fluid$(EXEEXT) $(FLUID)


#
# Make everything...
#

all:		$(TARGETS)


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
# Build FLUID...
#

fluid$(EXEEXT): $(OBJECTS) ../lib/$(LIBPREFIX)fltk$(LIBSUFFIX) \
		../lib/$(LIBPREFIX)fltk_images$(LIBSUFFIX)
	echo Linking $@...
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LINKFLTKIMG) $(LDLIBS)


#
# Build shared FLUID...
#

fluid-shared$(EXEEXT): $(OBJECTS) ../src/$(DSOPREFIX)fltk$(DSOSUFFIX) \
		../images/$(DSOPREFIX)fltk_images$(DSOSUFFIX)
	echo Linking $@...
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LINKSHARED) $(LIBS)


#
# Install everything...
#

install:
	echo "Installing FLUID in $(bindir)..."
	-$(MKDIR) $(bindir)
	$(CP) $(FLUID) $(bindir)/fluid$(EXEEXT)
	$(STRIP) $(bindir)/fluid$(EXEEXT)
	$(bindir)/fltk-config --post $(bindir)/fluid$(EXEEXT)
	$(CHMOD) 755 $(bindir)/fluid$(EXEEXT)


uninstall:
	$(RM) $(bindir)/fluid


#
# Note: The rebuild target can only be used if you have the original .fl
#       files.  This is normally only used by the FLTK maintainers...
#

rebuild:
	./fluid$(EXEEXT) -c about_panel.fl
	./fluid$(EXEEXT) -c alignment_panel.fl
	./fluid$(EXEEXT) -c function_panel.fl
	./fluid$(EXEEXT) -c widget_panel.fl
	./fluid$(EXEEXT) -c coding_style.fl


#
# End of "$Id: Makefile,v 1.47 2002/12/22 05:30:22 easysw Exp $".
#
# DO NOT DELETE

about_panel.o: about_panel.h ../fltk/DoubleBufferWindow.h ../fltk/Window.h
about_panel.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
about_panel.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
about_panel.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Button.h
about_panel.o: ../fltk/ReturnButton.h ../fltk/Button.h
alignment_panel.o: alignment_panel.h ../fltk/Window.h ../fltk/Group.h
alignment_panel.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
alignment_panel.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/Group.h
alignment_panel.o: ../fltk/Input.h ../fltk/Widget.h ../fltk/CheckButton.h
alignment_panel.o: ../fltk/Button.h ../fltk/Button.h
code.o: ../fltk/run.h ../fltk/FL_API.h FluidType.h ../fltk/Widget.h
code.o: Fluid_Plugins.h Fluid_Image.h ../fltk/SharedImage.h ../fltk/Image.h
code.o: ../fltk/Flags.h alignment_panel.h ../fltk/Window.h ../fltk/Group.h
code.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h ../fltk/Group.h
code.o: ../fltk/Input.h ../fltk/CheckButton.h ../fltk/Button.h
code.o: ../fltk/Button.h coding_style.h ../fltk/TabGroup.h
code.o: ../fltk/RadioButton.h ../fltk/CheckButton.h ../fltk/ValueInput.h
code.o: ../fltk/Valuator.h ../fltk/FloatInput.h ../fltk/NumericInput.h
code.o: ../fltk/Input.h ../fltk/filename.h ../fltk/FL_VERSION.h
coding_style.o: coding_style.h ../fltk/Widget.h ../fltk/Window.h
coding_style.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
coding_style.o: ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
coding_style.o: ../fltk/TabGroup.h ../fltk/Group.h ../fltk/RadioButton.h
coding_style.o: ../fltk/CheckButton.h ../fltk/CheckButton.h ../fltk/Button.h
coding_style.o: ../fltk/ValueInput.h ../fltk/Valuator.h ../fltk/FloatInput.h
coding_style.o: ../fltk/NumericInput.h ../fltk/Input.h ../fltk/Button.h
coding_style_func.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
coding_style_func.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h
coding_style_func.o: ../fltk/Flags.h FluidType.h ../fltk/Widget.h
coding_style_func.o: Fluid_Plugins.h Fluid_Image.h ../fltk/SharedImage.h
coding_style_func.o: ../fltk/Image.h
factory.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Group.h Fluid_Plugins.h
factory.o: FluidType.h ../fltk/Widget.h Fluid_Image.h ../fltk/SharedImage.h
factory.o: ../fltk/Image.h ../fltk/Flags.h ../fltk/InvisibleBox.h
factory.o: ../fltk/Button.h ../fltk/ReturnButton.h ../fltk/Button.h
factory.o: ../fltk/RepeatButton.h ../fltk/LightButton.h ../fltk/CheckButton.h
factory.o: ../fltk/CheckButton.h ../fltk/RadioButton.h ../fltk/Input.h
factory.o: ../fltk/Clock.h ../fltk/Adjuster.h ../fltk/Valuator.h
factory.o: ../fltk/Dial.h ../fltk/ThumbWheel.h ../fltk/Scrollbar.h
factory.o: ../fltk/Slider.h ../fltk/Output.h ../fltk/Input.h
factory.o: ../fltk/ValueInput.h ../fltk/FloatInput.h ../fltk/NumericInput.h
factory.o: ../fltk/ValueOutput.h ../fltk/Valuator.h ../fltk/ValueSlider.h
factory.o: ../FL/Fl_Menu_Item.h ../fltk/Browser.h ../fltk/Menu.h
factory.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
factory.o: ../fltk/Scrollbar.h
file.o: alignment_panel.h ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
file.o: ../fltk/Style.h ../fltk/Color.h ../fltk/FL_API.h ../fltk/Flags.h
file.o: ../fltk/Group.h ../fltk/Input.h ../fltk/Widget.h
file.o: ../fltk/CheckButton.h ../fltk/Button.h ../fltk/Button.h Fluid_Image.h
file.o: Fluid_Plugins.h ../fltk/SharedImage.h ../fltk/Image.h FluidType.h
file.o: ../fltk/FL_VERSION.h ../fltk/ask.h
Fl_Function_Type.o: ../fltk/run.h ../fltk/FL_API.h FluidType.h
Fl_Function_Type.o: ../fltk/Widget.h Fluid_Plugins.h Fluid_Image.h
Fl_Function_Type.o: ../fltk/SharedImage.h ../fltk/Image.h ../fltk/Flags.h
Fl_Function_Type.o: ../fltk/ask.h ../fltk/Style.h ../fltk/Color.h
Fl_Function_Type.o: coding_style.h ../fltk/Window.h ../fltk/Group.h
Fl_Function_Type.o: ../fltk/Widget.h ../fltk/TabGroup.h ../fltk/Group.h
Fl_Function_Type.o: ../fltk/RadioButton.h ../fltk/CheckButton.h
Fl_Function_Type.o: ../fltk/CheckButton.h ../fltk/Button.h
Fl_Function_Type.o: ../fltk/ValueInput.h ../fltk/Valuator.h
Fl_Function_Type.o: ../fltk/FloatInput.h ../fltk/NumericInput.h
Fl_Function_Type.o: ../fltk/Input.h ../fltk/Button.h function_panel.h
Fl_Function_Type.o: ../fltk/Input.h ../fltk/ReturnButton.h
Fl_Function_Type.o: ../fltk/MultiLineInput.h
Fl_Group_Type.o: FluidType.h ../fltk/Widget.h Fluid_Plugins.h Fluid_Image.h
Fl_Group_Type.o: ../fltk/SharedImage.h ../fltk/Image.h ../fltk/FL_API.h
Fl_Group_Type.o: ../fltk/Flags.h ../fltk/events.h ../fltk/run.h
Fl_Group_Type.o: ../fltk/Group.h ../fltk/ask.h ../fltk/Style.h
Fl_Group_Type.o: ../fltk/Color.h ../fltk/PackedGroup.h ../fltk/Group.h
Fl_Group_Type.o: ../fltk/Widget.h ../fltk/TabGroup.h ../fltk/ScrollGroup.h
Fl_Group_Type.o: ../fltk/Scrollbar.h
Fl_Menu_Type.o: ../fltk/events.h ../fltk/FL_API.h FluidType.h
Fl_Menu_Type.o: ../fltk/Widget.h Fluid_Plugins.h Fluid_Image.h
Fl_Menu_Type.o: ../fltk/SharedImage.h ../fltk/Image.h ../fltk/Flags.h
Fl_Menu_Type.o: ../fltk/ask.h ../fltk/Style.h ../fltk/Color.h ../fltk/Menu.h
Fl_Menu_Type.o: ../fltk/Group.h ../fltk/Widget.h ../fltk/Item.h
Fl_Menu_Type.o: ../fltk/ItemGroup.h ../fltk/Menu.h ../fltk/Divider.h
Fl_Menu_Type.o: ../fltk/Button.h ../fltk/PopupMenu.h ../fltk/Choice.h
Fl_Menu_Type.o: ../fltk/MenuBar.h ../fltk/InputBrowser.h ../fltk/Input.h
Fl_Menu_Type.o: ../fltk/Browser.h ../fltk/Scrollbar.h ../fltk/Output.h
Fl_Menu_Type.o: ../fltk/Input.h Shortcut_Button.h ../fltk/draw.h
Fl_Type.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/MultiBrowser.h
Fl_Type.o: ../fltk/Browser.h ../fltk/Menu.h ../fltk/Group.h ../fltk/Widget.h
Fl_Type.o: ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
Fl_Type.o: ../fltk/Scrollbar.h ../fltk/Item.h ../fltk/draw.h FluidType.h
Fl_Type.o: ../fltk/Widget.h Fluid_Plugins.h Fluid_Image.h
Fl_Type.o: ../fltk/SharedImage.h ../fltk/Image.h
fluid.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/visual.h ../fltk/events.h
fluid.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h ../fltk/Style.h
fluid.o: ../fltk/Color.h ../fltk/Flags.h ../fltk/Box.h ../fltk/Button.h
fluid.o: ../fltk/Browser.h ../fltk/Menu.h ../fltk/Scrollbar.h
fluid.o: ../fltk/MenuBar.h ../fltk/Input.h ../fltk/Tooltip.h ../fltk/ask.h
fluid.o: ../fltk/draw.h ../fltk/file_chooser.h ../fltk/filename.h
fluid.o: ../fltk/FileIcon.h about_panel.h ../fltk/DoubleBufferWindow.h
fluid.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
fluid.o: ../fltk/ReturnButton.h ../fltk/Button.h Fluid_Plugins.h FluidType.h
fluid.o: Fluid_Image.h ../fltk/SharedImage.h ../fltk/Image.h coding_style.h
fluid.o: ../fltk/TabGroup.h ../fltk/RadioButton.h ../fltk/CheckButton.h
fluid.o: ../fltk/CheckButton.h ../fltk/ValueInput.h ../fltk/Valuator.h
fluid.o: ../fltk/FloatInput.h ../fltk/NumericInput.h ../fltk/Input.h
fluid.o: ../fltk/StyleSet.h ../FL/Fl_Menu_Item.h
Fluid_Image.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Widget.h FluidType.h
Fluid_Image.o: Fluid_Plugins.h Fluid_Image.h ../fltk/SharedImage.h
Fluid_Image.o: ../fltk/Image.h ../fltk/Flags.h ../fltk/filename.h
Fluid_Image.o: ../fltk/xbmImage.h ../fltk/file_chooser.h image_file_panel.h
Fluid_Image.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
Fluid_Image.o: ../fltk/Style.h ../fltk/Color.h ../fltk/Input.h
Fluid_Image.o: ../fltk/Button.h ../fltk/ReturnButton.h ../fltk/Button.h
Fluid_Image.o: image_file_panel.cxx
Fluid_Plugins.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Group.h
Fluid_Plugins.o: Fluid_Plugins.h ../FL/Fl_Menu_Item.h ../fltk/Widget.h
Fluid_Plugins.o: ../fltk/load_plugin.h
Fl_Widget_Type.o: ../fltk/run.h ../fltk/FL_API.h ../fltk/Group.h
Fl_Widget_Type.o: Fluid_Plugins.h FluidType.h ../fltk/Widget.h Fluid_Image.h
Fl_Widget_Type.o: ../fltk/SharedImage.h ../fltk/Image.h ../fltk/Flags.h
Fl_Widget_Type.o: ../fltk/ask.h ../fltk/Style.h ../fltk/Color.h
Fl_Widget_Type.o: ../fltk/Slider.h ../fltk/Valuator.h ../fltk/Window.h
Fl_Widget_Type.o: ../fltk/Group.h ../fltk/Widget.h coding_style.h
Fl_Widget_Type.o: ../fltk/TabGroup.h ../fltk/RadioButton.h
Fl_Widget_Type.o: ../fltk/CheckButton.h ../fltk/CheckButton.h
Fl_Widget_Type.o: ../fltk/Button.h ../fltk/ValueInput.h ../fltk/FloatInput.h
Fl_Widget_Type.o: ../fltk/NumericInput.h ../fltk/Input.h ../fltk/Button.h
Fl_Widget_Type.o: ../fltk/StyleSet.h widget_panel.h ../fltk/Input.h
Fl_Widget_Type.o: ../fltk/Choice.h ../fltk/Menu.h Shortcut_Button.h
Fl_Widget_Type.o: ../fltk/WordwrapInput.h ../fltk/LightButton.h
Fl_Widget_Type.o: ../fltk/MultiLineInput.h ../fltk/ReturnButton.h
Fl_Widget_Type.o: ../fltk/ColorChooser.h
Fl_Window_Type.o: ../fltk/events.h ../fltk/FL_API.h ../fltk/damage.h
Fl_Window_Type.o: ../fltk/OverlayWindow.h ../fltk/DoubleBufferWindow.h
Fl_Window_Type.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Widget.h
Fl_Window_Type.o: ../fltk/Style.h ../fltk/Color.h ../fltk/Flags.h
Fl_Window_Type.o: ../fltk/ask.h ../fltk/draw.h ../fltk/Box.h FluidType.h
Fl_Window_Type.o: ../fltk/Widget.h Fluid_Plugins.h Fluid_Image.h
Fl_Window_Type.o: ../fltk/SharedImage.h ../fltk/Image.h alignment_panel.h
Fl_Window_Type.o: ../fltk/Window.h ../fltk/Group.h ../fltk/Input.h
Fl_Window_Type.o: ../fltk/CheckButton.h ../fltk/Button.h ../fltk/Button.h
Fl_Window_Type.o: ../fltk/StyleSet.h widget_panel.h ../fltk/TabGroup.h
Fl_Window_Type.o: ../fltk/Choice.h ../fltk/Menu.h ../fltk/ValueInput.h
Fl_Window_Type.o: ../fltk/Valuator.h ../fltk/FloatInput.h
Fl_Window_Type.o: ../fltk/NumericInput.h ../fltk/Input.h Shortcut_Button.h
Fl_Window_Type.o: ../fltk/WordwrapInput.h ../fltk/LightButton.h
Fl_Window_Type.o: ../fltk/CheckButton.h ../fltk/MultiLineInput.h
Fl_Window_Type.o: ../fltk/ReturnButton.h ../fltk/MenuBar.h
Fl_Window_Type.o: ../FL/Fl_Menu_Item.h
function_panel.o: function_panel.h ../fltk/Window.h ../fltk/Group.h
function_panel.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
function_panel.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/CheckButton.h
function_panel.o: ../fltk/Button.h ../fltk/Input.h ../fltk/ReturnButton.h
function_panel.o: ../fltk/Button.h ../fltk/MultiLineInput.h ../fltk/Input.h
function_panel.o: ../fltk/Widget.h
widget_panel.o: widget_panel.h ../fltk/Window.h ../fltk/Group.h
widget_panel.o: ../fltk/Widget.h ../fltk/Style.h ../fltk/Color.h
widget_panel.o: ../fltk/FL_API.h ../fltk/Flags.h ../fltk/TabGroup.h
widget_panel.o: ../fltk/Group.h ../fltk/Input.h ../fltk/Choice.h
widget_panel.o: ../fltk/Menu.h ../fltk/Widget.h ../fltk/Button.h
widget_panel.o: ../fltk/CheckButton.h ../fltk/Button.h ../fltk/ValueInput.h
widget_panel.o: ../fltk/Valuator.h ../fltk/FloatInput.h
widget_panel.o: ../fltk/NumericInput.h ../fltk/Input.h Shortcut_Button.h
widget_panel.o: ../fltk/WordwrapInput.h ../fltk/LightButton.h
widget_panel.o: ../fltk/CheckButton.h ../fltk/MultiLineInput.h
widget_panel.o: ../fltk/ReturnButton.h
