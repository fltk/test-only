# Microsoft Developer Studio Project File - Name="fltkdll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fltkdll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll.mak" CFG="fltkdll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltkdll - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltkdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fltkdll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "fltkdll0"
# PROP BASE Intermediate_Dir "fltkdll0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "fltkdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Os /Ob2 /I "." /I ".." /D "NDEBUG" /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdll.pdb" /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fltkdll1"
# PROP BASE Intermediate_Dir "fltkdll1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "fltkdlld"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /GX /Od /I "." /I ".." /I "../visualc" /D "_DEBUG" /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /ZI /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:no /pdb:"fltkdlld.pdb" /debug /machine:I386 /out:"../lib/fltkdlld.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "fltkdll - Win32 Release"
# Name "fltkdll - Win32 Debug"
# Begin Group "Image Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\fl_background.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_BA=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\math.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_bmp.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_BM=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Shared_Image.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_gif.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_GI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Shared_Image.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Image.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_IM=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Image.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_RGB_Image.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_xpm.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_XP=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Shared_Image.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# End Group
# Begin Group "Schemes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\conf.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_atol.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_del.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_D=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_endtrim.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_E=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_error.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_ER=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_get.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_G=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_getboolean.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_GE=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_getlong.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_GET=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_keys.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_K=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_level_indent.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_L=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_list_free.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_LI=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_sections.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_S=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_set.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_SE=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_setboolean.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_SET=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_setlong.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_SETL=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_strcasecmp.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_ST=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_strcnt.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_STR=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\conf_util.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_CONF_U=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_motif.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_MO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Check_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_options.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_OP=\
	"..\FL\conf.h"\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\fl\fl_load_plugin.h"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_windows.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_WI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Check_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# End Group
# Begin Group "Core Source"

# PROP Default_Filter ""
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

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FILEN=\
	"..\FL\filename.H"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\filename_expand.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FILENA=\
	"..\FL\filename.H"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\filename_ext.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FILENAM=\
	"..\FL\filename.H"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\filename_isdir.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FILENAME=\
	"..\FL\filename.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\filename_list.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FILENAME_=\
	"..\FL\filename.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\filename_match.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FILENAME_M=\
	"..\FL\filename.H"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\filename_setext.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FILENAME_S=\
	"..\FL\filename.H"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_CX=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Tooltip.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_abort.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_AB=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_add_idle.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_AD=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Adjuster.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_ADJ=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Adjuster.H"\
	"..\FL\Fl_Bitmap.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\src\fastarrow.h"\
	"..\src\mediumarrow.h"\
	"..\src\slowarrow.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Adjuster.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_arc.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_AR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\math.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_arci.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_ARC=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\math.h"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_arg.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_ARG=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_ask.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_AS=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\fl_ask.H"\
	"..\FL\Fl_Box.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Return_Button.H"\
	"..\FL\Fl_Secret_Input.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\fl_ask.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Bitmap.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_BI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Bitmap.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Bitmap.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Box.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_BO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Box.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Box.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_boxtype.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_BOX=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Boxtype.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_BR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
	"..\FL\Fl_Browser_.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Browser.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser_.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_BRO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
	"..\FL\Fl_Browser_.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Browser_.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser_load.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_BROW=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
	"..\FL\Fl_Browser_.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Button.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_BU=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_call_main.c
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Check_Button.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_CH=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Check_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Check_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Choice.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_CHO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Choice.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Choice.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Clock.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_CL=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Clock.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Clock.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_color.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_CO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_cmap.h"\
	"..\src\fl_color_win32.cxx"\
	"..\src\Fl_XColor.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Color.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Color_Chooser.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_COL=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Box.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Choice.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\Fl_Color_Chooser.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Return_Button.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Value_Input.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\math.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Color_Chooser.H
# End Source File
# Begin Source File

SOURCE=..\Fl\fl_config.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Counter.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_COU=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\Fl_Counter.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Counter.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_cursor.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_CU=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_curve.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_CUR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_cutpaste.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_CUT=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_cutpaste_win32.cxx"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Dial.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_DI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\Fl_Dial.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\math.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Dial.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_diamond_box.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_DIA=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_display.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_DIS=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Double_Window.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_DO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\Fl_Double_Window.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Double_Window.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_DR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\fl_draw.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_image.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_DRA=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_draw_image_win32.cxx"\
	"..\src\Fl_XColor.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_pixmap.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_DRAW=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_engraved_label.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_EN=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_file_chooser.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_FI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
	"..\FL\Fl_Browser_.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Choice.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\fl_file_chooser.H"\
	"..\Fl\Fl_FileBrowser.h"\
	"..\Fl\Fl_FileChooser.h"\
	"..\Fl\Fl_FileIcon.h"\
	"..\Fl\Fl_FileInput.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Return_Button.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\fl_file_chooser.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileBrowser.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_FIL=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
	"..\FL\Fl_Browser_.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\Fl\Fl_FileBrowser.h"\
	"..\Fl\Fl_FileIcon.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_FileBrowser.h
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileChooser.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_FILE=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Bitmap.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
	"..\FL\Fl_Browser_.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Choice.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\Fl\Fl_FileBrowser.h"\
	"..\Fl\Fl_FileChooser.h"\
	"..\Fl\Fl_FileIcon.h"\
	"..\Fl\Fl_FileInput.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Return_Button.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_FileChooser.h
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileChooser2.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_FILEC=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\FL\Fl.H"\
	"..\FL\fl_ask.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
	"..\FL\Fl_Browser_.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Choice.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\Fl\Fl_FileBrowser.h"\
	"..\Fl\Fl_FileChooser.h"\
	"..\Fl\Fl_FileIcon.h"\
	"..\Fl\Fl_FileInput.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Return_Button.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileIcon.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_FILEI=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\Fl\Fl_FileIcon.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_FileIcon.h
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileInput.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_FILEIN=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\Fl\Fl_FileInput.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

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

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_FO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_font_win32.cxx"\
	"..\src\fl_fontsize.h"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Font.H
# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Free.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_get_key.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_GE=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_get_key_win32.cxx"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_glyph.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_GL=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_grab.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_GR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Group.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_GRO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Box.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Tabs.H"\
	"..\FL\Fl_Tooltip.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	

!ENDIF 

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

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_IN=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Input.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input_.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_INP=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Input_.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input_Style.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_INPU=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Int_Input.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_labeltype.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_LA=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Labeltype.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Layout.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Light_Button.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_LI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Light_Button.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Light_Button.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Line_Dial.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_load_plugin.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_LO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\fl\fl_load_plugin.h"\
	"..\FL\Fl_Style.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_lock.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_LOC=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_ME=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Menu_Window.H"\
	"..\FL\Fl_Single_Window.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Menu.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_MEN=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Menu_.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_add.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_MENU=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Bar.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_MENU_=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Bar.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Menu_Bar.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Button.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_MENU_B=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Button.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Menu_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_global.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_MENU_G=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Menu_Item.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Window.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_MENU_W=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_Window.H"\
	"..\FL\Fl_Single_Window.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

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

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_OU=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Output.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_oval_box.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_OV=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_OVE=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay_visual.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_OVER=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Overlay_Window.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_OVERL=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\Fl_Double_Window.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Overlay_Window.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Overlay_Window.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_own_colormap.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_OW=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pack.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_PA=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Pack.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Pack.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pixmap.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_PI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Pixmap.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

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

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_RE=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Repeat_Button.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_REP=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Repeat_Button.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Repeat_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Return_Button.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_RET=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Return_Button.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Return_Button.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_rgb.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_RG=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Roller.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_RO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Roller.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Roller.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_round_box.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_ROU=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Round_Button.H
# End Source File
# Begin Source File

SOURCE=..\Fl\Fl_Round_Clock.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_rounded_box.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_ROUN=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scroll.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_SC=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Scroll.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Scroll.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_scroll_area.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_SCR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scrollbar.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_SCRO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

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

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_SH=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Shared_Image.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_SHA=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Bitmap.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Shared_Image.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Shared_Image.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_shortcut.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_SHO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\fl_show_colormap.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_SHOW=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\fl_show_colormap.H"\
	"..\FL\Fl_Single_Window.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

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

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_SI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Single_Window.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Single_Window.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Slider.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_SL=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Slider.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Style.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_ST=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Style.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_symbols.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_SY=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tabs.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_TA=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Tabs.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Tabs.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tile.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_TI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Tile.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	

!ENDIF 

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

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_TO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_Window.H"\
	"..\FL\Fl_Single_Window.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Tooltip.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Tooltip.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Valuator.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_VA=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\math.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Valuator.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Input.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_VAL=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Value_Input.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Value_Input.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Output.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_VALU=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Value_Output.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Value_Output.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Slider.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_VALUE=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input_.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Value_Slider.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Value_Slider.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_vertex.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_VE=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\math.h"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_visual.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_VI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Widget.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_WID=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_WIN=\
	"..\FL\conf.h"\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\Fl_Window.H
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_fullscreen.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_WIND=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_hotspot.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_WINDO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_iconize.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_WINDOW=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Fl_x.cxx

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_FL_X_=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_win32.cxx"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\math.h
# End Source File
# Begin Source File

SOURCE=..\Fl\menubar.h
# End Source File
# Begin Source File

SOURCE=..\src\numericsort.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_NUMER=\
	"..\FL\filename.H"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\scandir.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_SCAND=\
	"..\src\scandir_win32.c"\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\vsnprintf.c

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

DEP_CPP_VSNPR=\
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FL\win32.H
# End Source File
# Begin Source File

SOURCE=..\FL\x.H
# End Source File
# End Group
# End Target
# End Project
