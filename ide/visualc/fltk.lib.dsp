# Microsoft Developer Studio Project File - Name="fltk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fltk - Win32 Debug MinSize
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltk.lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltk.lib.mak" CFG="fltk - Win32 Debug MinSize"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltk - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fltk - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "fltk - Win32 Debug MinSize" (based on "Win32 (x86) Static Library")
!MESSAGE "fltk - Win32 Release MinSize" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fltk - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /GR /GX /Os /Ob2 /I "." /I "..\.." /D "_POSIX_" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\fltk.lib"

!ELSEIF  "$(CFG)" == "fltk - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /GR /GX /ZI /Od /I "." /I "..\.." /I "../visualc" /D "_POSIX_" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\fltkd.lib"

!ELSEIF  "$(CFG)" == "fltk - Win32 Debug MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fltk___Win32_Debug_MinSize"
# PROP BASE Intermediate_Dir "fltk___Win32_Debug_MinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "fltk___Win32_Debug_MinSize"
# PROP Intermediate_Dir "fltk___Win32_Debug_MinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /GX /ZI /Od /I "." /I "..\.." /I "../visualc" /D "_POSIX_" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /FR /YX /FD /c
# ADD CPP /nologo /MDd /GR /GX /ZI /Od /I "." /I "..\.." /I "../visualc" /D "_DEBUG" /D "_MSC_DLL" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\fltkd.lib"
# ADD LIB32 /nologo /out:"..\..\lib\fltkd.lib"

!ELSEIF  "$(CFG)" == "fltk - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "fltk___Win32_Release_MinSize"
# PROP BASE Intermediate_Dir "fltk___Win32_Release_MinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "fltk___Win32_Release_MinSize"
# PROP Intermediate_Dir "fltk___Win32_Release_MinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /GX /Os /Ob2 /I "." /I "..\.." /D "_POSIX_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /FD /c
# ADD CPP /nologo /MD /GR /GX /Os /Ob2 /I "..\.." /I "." /D "NDEBUG" /D "_MSC_DLL" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\fltk.lib"
# ADD LIB32 /nologo /out:"..\..\lib\fltk.lib"

!ENDIF 

# Begin Target

# Name "fltk - Win32 Release"
# Name "fltk - Win32 Debug"
# Name "fltk - Win32 Debug MinSize"
# Name "fltk - Win32 Release MinSize"
# Begin Source File

SOURCE=..\..\src\add_idle.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\addarc.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\addcurve.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Adjuster.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\AlignGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\args.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\array.h
# End Source File
# Begin Source File

SOURCE=..\..\src\BarGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\bmpImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Browser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Button.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CheckButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Choice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\clip.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Clock.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Color.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\color_chooser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\colormap.h
# End Source File
# Begin Source File

SOURCE=..\..\src\ComboBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\compose.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Cursor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CycleButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\default_glyph.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Dial.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\DiamondBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\dnd.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\draw_xpm.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\drawimage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\drawtext.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\dump_compose.c
# End Source File
# Begin Source File

SOURCE=..\..\src\EngravedLabel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\error.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\event_key_state.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fastarrow.h
# End Source File
# Begin Source File

SOURCE=..\..\src\file_chooser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\FileBrowser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\FileChooser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\FileChooser2.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\FileIcon.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\FileIcon2.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\FileInput.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\filename_ext.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\filename_isdir.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\filename_list.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\filename_match.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\filename_normalize.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fillarc.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fillrect.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Fl_Menu_Item.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\FloatInput.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk_theme.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Font.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\gifImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Group.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\HighlightButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Image.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Input.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\InputBrowser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\InvisibleWidget.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Item.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\key_name.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\list_fonts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\load_plugin.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\lock.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\mediumarrow.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Menu.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Menu_add.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Menu_global.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Menu_popup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MenuBar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MenuWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\message.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MultiImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\NumericInput.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Output.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OvalBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\overlay_rect.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\own_colormap.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\PackedGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\path.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\pktdef.h
# End Source File
# Begin Source File

SOURCE=..\..\src\PlasticBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\PopupMenu.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\ProgressBar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\RadioButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\readimage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\RepeatButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\ReturnButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\rgbImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\RoundBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\RoundedBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\run.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\scandir.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Scrollbar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\ScrollGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\scrollrect.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\setcolor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\setdisplay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\setvisual.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\ShadowBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\ShapedWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\SharedImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\ShortcutAssignment.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\show_colormap.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Slider.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\slowarrow.h
# End Source File
# Begin Source File

SOURCE=..\..\src\string.c
# End Source File
# Begin Source File

SOURCE=..\..\src\StringList.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Style.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\StyleSet.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Symbol.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\TabGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\TextBuffer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\TextDisplay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\TextEditor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\ThumbWheel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\TiledGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\TiledImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tooltip.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\UpBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\utf.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Valuator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\ValueInput.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\ValueOutput.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\ValueSlider.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\vsnprintf.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Widget.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Widget_draw.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Window.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Window_fullscreen.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Window_hotspot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Window_iconize.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\win32\WinMain.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\xbmImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\xpmImage.cxx
# End Source File
# End Target
# End Project
