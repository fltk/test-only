# Microsoft Developer Studio Project File - Name="fltk3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fltk3 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltk3.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltk3.mak" CFG="fltk3 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltk3 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fltk3 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fltk3 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release\fltk3"
# PROP BASE Intermediate_Dir "Release\fltk3"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\fltk3"
# PROP Intermediate_Dir "Release\fltk3"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "FL_LIBRARY" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /GX /Ot /Op /Ob2 /I "." /I "..\..\include" /I "..\..\include\fltk3zlib" /I "..\..\include\fltk3png" /I "..\..\include\fltk3jpeg" /D "WIN32" /D "FL_LIBRARY" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_NONSTDC_NO_DEPRECATE" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /FD /c
# SUBTRACT CPP /Os
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\fltk3.lib"

!ELSEIF  "$(CFG)" == "fltk3 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug\fltk3"
# PROP BASE Intermediate_Dir "Debug\fltk3"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\fltk3"
# PROP Intermediate_Dir "Debug\fltk3"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "FL_LIBRARY" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /GX /Z7 /Od /I "." /I "..\..\include" /I "..\..\include\fltk3zlib" /I "..\..\include\fltk3png" /I "..\..\include\fltk3jpeg" /D "WIN32" /D "FL_LIBRARY" /D "_DEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_NONSTDC_NO_DEPRECATE" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\fltk3d.lib"

!ENDIF 

# Begin Target

# Name "fltk3 - Win32 Release"
# Name "fltk3 - Win32 Debug"
# Begin Source File

SOURCE=..\..\src\fltk3\Adjuster.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Bitmap.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Box.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Browser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Browser_.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Browser_load.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Button.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Chart.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\CheckBrowser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\CheckButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Choice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Clock.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\ColorChooser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Counter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Device.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Dial.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\DoubleWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\FileBrowser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\FileChooser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\FileChooser2.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\FileIcon.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\FileInput.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Group.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\HelpView.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Image.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Input.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Input_.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\LightButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Menu.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Menu_.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\MenuBar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\MenuButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\MenuWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Menu_add.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Menu_global.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\MultiLabel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\NativeFileChooser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\OverlayWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\PackedGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\PagedDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Pixmap.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Positioner.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Printer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Preferences.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Progress.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\RepeatButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\ReturnButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Roller.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\RoundButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\ScrollGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Scrollbar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\ShapedWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\SharedImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\SingleWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Slider.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Style.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Table.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\TableRow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\TabGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\TextBuffer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\TextDisplay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\TextEditor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\TiledGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\TiledImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Tooltip.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Tree.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\TreeItem.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\TreeItemArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\TreePrefs.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Valuator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\ValueInput.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\ValueOutput.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\ValueSlider.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Widget.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Window.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Window_fullscreen.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Window_hotspot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\Window_iconize.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\WizardGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\XBMImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\XPMImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\arc.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\arci.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\ask.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\abort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\add_idle.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\arg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\boxtype.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\call_main.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\xutf8\case.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\color.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\compatibility1.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\compatibility2.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\compose.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\cursor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\curve.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\diamond_box.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\display.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\dnd.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\draw.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\draw_image.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\draw_pixmap.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\encoding_latin1.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\encoding_mac_roman.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\engraved_label.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\file_dir.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\filename_absolute.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\filename_expand.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\filename_ext.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\filename_isdir.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\filename_list.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\filename_match.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\filename_setext.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\flstring.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\font.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\gtk.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\get_key.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\get_system_colors.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\grab.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\xutf8\is_right2left.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\xutf8\is_spacing.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\labeltype.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\line_style.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\lock.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\numericsort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\open_uri.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\oval_box.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\overlay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\overlay_visual.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\own_colormap.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\plastic.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\ps_image.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\read_image.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\rect.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\round_box.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\rounded_box.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\run.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\scandir.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\screen_xywh.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\scroll_area.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\set_font.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\set_fonts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\shadow_box.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\shortcut.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\show_colormap.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\symbols.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\utf.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\utf8.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\vertex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\visual.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\x11.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3\vsnprintf.c
# End Source File
# End Target
# End Project
