# Microsoft Developer Studio Project File - Name="fltk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fltk - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltk.lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltk.lib.mak" CFG="fltk - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltk - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fltk - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

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
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /GX /Os /Ob2 /I "." /I ".." /D "_POSIX_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\fltk.lib"

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
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /GX /Od /I "." /I ".." /I "../visualc" /D "_POSIX_" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /FR /YX /FD /ZI /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\fltkd.lib"

!ENDIF 

# Begin Target

# Name "fltk - Win32 Release"
# Name "fltk - Win32 Debug"
# Begin Group "Core Source"

# PROP Default_Filter ".c,.cxx"
# Begin Source File

SOURCE=..\FL\conf.h
# End Source File
# Begin Source File

SOURCE=..\FL\dirent.h
# End Source File
# Begin Source File

SOURCE=..\FL\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\FL\filename.H
# End Source File
# Begin Source File

SOURCE=..\src\filename_absolute.cxx
# End Source File
# Begin Source File

SOURCE=..\src\filename_expand.cxx
# End Source File
# Begin Source File

SOURCE=..\src\filename_ext.cxx
# End Source File
# Begin Source File

SOURCE=..\src\filename_isdir.cxx
# End Source File
# Begin Source File

SOURCE=..\src\filename_list.cxx
# End Source File
# Begin Source File

SOURCE=..\src\filename_match.cxx
# End Source File
# Begin Source File

SOURCE=..\src\filename_setext.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_abort.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_add_idle.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Adjuster.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Adjuster.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_arc.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_arci.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_arg.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_ask.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\fl_ask.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_background.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Bitmap.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Bitmap.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Box.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Box.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_boxtype.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Boxtype.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Browser.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser_.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Browser_.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser_load.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Button.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_call_main.c
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Check_Button.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Check_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Choice.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Choice.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Clock.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Clock.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_color.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Color.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Color_Chooser.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Color_Chooser.H
# End Source File
# Begin Source File

SOURCE=..\Fl\fl_config.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Counter.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Counter.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_cursor.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_curve.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_cutpaste.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Dial.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Dial.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_diamond_box.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_display.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Double_Window.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Double_Window.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\fl_draw.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_image.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_pixmap.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_engraved_label.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_file_chooser.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\fl_file_chooser.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileBrowser.cxx
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_FileBrowser.h
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileChooser.cxx
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_FileChooser.h
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileChooser2.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileIcon.cxx
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_FileIcon.h
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileInput.cxx
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_FileInput.h
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Fill_Dial.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Fill_Slider.H
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Flags.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Float_Input.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_font.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Font.H
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Free.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_get_key.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_glyph.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_grab.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Group.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Header_End.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Header_Start.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Hold_Browser.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Hor_Fill_Slider.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Hor_Nice_Slider.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Hor_Slider.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Hor_Value_Slider.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Input.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input_.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Input_.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input_Style.cxx
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Int_Input.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_labeltype.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Labeltype.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Layout.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Light_Button.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Light_Button.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Line_Dial.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_load_plugin.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_lock.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu.cxx
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Menu.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Menu_.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_add.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Bar.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Menu_Bar.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Button.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Menu_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_global.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Menu_Item.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Window.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Menu_Window.H
# End Source File
# Begin Source File

SOURCE=..\Fl\fl_message.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Multi_Browser.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Multi_Label.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Multiline_Input.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Multiline_Output.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Nice_Slider.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Object.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Output.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Output.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_oval_box.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay_visual.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Overlay_Window.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Overlay_Window.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_own_colormap.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pack.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Pack.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pixmap.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Pixmap.H
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Plugins.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Radio_Button.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Radio_Light_Button.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Radio_Round_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_rect.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Repeat_Button.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Repeat_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Return_Button.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Return_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_rgb.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Roller.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Roller.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_round_box.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Round_Button.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Round_Clock.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_rounded_box.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scroll.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Scroll.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_scroll_area.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scrollbar.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Scrollbar.H
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Secret_Input.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Select_Browser.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_shadow_box.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Shared_Image.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Shared_Image.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_shortcut.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_show_colormap.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\fl_show_colormap.H
# End Source File
# Begin Source File

SOURCE=..\Fl\fl_show_input.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Simple_Counter.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Single_Window.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Single_Window.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Slider.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Slider.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Style.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Style.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_symbols.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tabs.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Tabs.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tile.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Tile.H
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Timer.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Toggle_Button.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Toggle_Light_Button.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Toggle_Round_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tooltip.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Tooltip.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Valuator.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Valuator.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Input.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Value_Input.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Output.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Value_Output.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Slider.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Value_Slider.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_vertex.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_visual.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Widget.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Window.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_fullscreen.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_hotspot.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_iconize.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_x.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\math.h
# End Source File
# Begin Source File

SOURCE=..\Fl\menubar.h
# End Source File
# Begin Source File

SOURCE=..\src\numericsort.c
# End Source File
# Begin Source File

SOURCE=..\src\scandir.c
# End Source File
# Begin Source File

SOURCE=..\src\vsnprintf.c
# End Source File
# Begin Source File

SOURCE=..\FL\win32.H
# End Source File
# Begin Source File

SOURCE=..\FL\x.H
# End Source File
# End Group
# Begin Group "Image Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\fl_bmp.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_gif.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Image.cxx
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Image.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_RGB_Image.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_xpm.cxx
# End Source File
# End Group
# Begin Group "Schemes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\conf.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_atol.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_del.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_endtrim.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_error.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_get.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_getboolean.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_getlong.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_keys.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_level_indent.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_list_free.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_sections.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_set.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_setboolean.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_setlong.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_strcasecmp.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_strcnt.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_util.c
# End Source File
# Begin Source File

SOURCE=..\src\fl_motif.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_options.cxx
# End Source File
# Begin Source File

SOURCE=..\src\fl_windows.cxx
# End Source File
# End Group
# End Target
# End Project
