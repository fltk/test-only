# Microsoft Developer Studio Project File - Name="fltkdll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fltkdll - Win32 Debug MinSize
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll.mak" CFG="fltkdll - Win32 Debug MinSize"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltkdll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltkdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltkdll - Win32 Debug MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltkdll - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
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
# ADD CPP /nologo /MT /W3 /GX /Os /Ob2 /I "." /I ".." /D "NDEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /YX /c
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
# ADD CPP /nologo /MTd /GX /ZI /Od /I "." /I ".." /I "../visualc" /D "_DEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdlld.pdb" /debug /machine:I386 /out:"../lib/fltkdlld.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fltkdll___Win32_Debug_MinSize"
# PROP BASE Intermediate_Dir "fltkdll___Win32_Debug_MinSize"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "fltkdll___Win32_Debug_MinSize"
# PROP Intermediate_Dir "fltkdll___Win32_Debug_MinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /GX /ZI /Od /I "." /I ".." /I "../visualc" /D "_DEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /YX /c
# ADD CPP /nologo /MDd /GX /ZI /Od /I "." /I ".." /I "../visualc" /D "_DEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "_MSC_DLL" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdlld.pdb" /debug /machine:I386 /out:"../lib/fltkdlld.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdlld.pdb" /debug /machine:I386 /out:"../lib/fltkdlld.dll" /implib:"../lib/fltkdlld.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "fltkdll___Win32_Release_MinSize"
# PROP BASE Intermediate_Dir "fltkdll___Win32_Release_MinSize"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "fltkdll___Win32_Release_MinSize"
# PROP Intermediate_Dir "fltkdll___Win32_Release_MinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Os /Ob2 /I "." /I ".." /D "NDEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /YX /c
# ADD CPP /nologo /MD /W3 /GX /Os /Ob2 /I "." /I ".." /D "NDEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "_MSC_DLL" /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdll.pdb" /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdll.pdb" /machine:I386 /out:"../lib/fltkdll.dll" /implib:"../lib/fltkdll.lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "fltkdll - Win32 Release"
# Name "fltkdll - Win32 Debug"
# Name "fltkdll - Win32 Debug MinSize"
# Name "fltkdll - Win32 Release MinSize"
# Begin Source File

SOURCE=..\src\conf.c
# End Source File
# Begin Source File

SOURCE=..\src\conf_del.c
DEP_CPP_CONF_=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_endtrim.c
DEP_CPP_CONF_E=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_error.c
DEP_CPP_CONF_ER=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_get.c
DEP_CPP_CONF_G=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_getboolean.c
DEP_CPP_CONF_GE=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_getlong.c
DEP_CPP_CONF_GET=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_keys.c
DEP_CPP_CONF_K=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_list_free.c
DEP_CPP_CONF_L=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_sections.c
DEP_CPP_CONF_S=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_set.c
DEP_CPP_CONF_SE=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_setboolean.c
DEP_CPP_CONF_SET=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_setlong.c
DEP_CPP_CONF_SETL=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_strcasecmp.c
DEP_CPP_CONF_ST=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_strcnt.c
DEP_CPP_CONF_STR=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\conf_util.c
DEP_CPP_CONF_U=\
	"..\FL\conf.h"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fastarrow.h
# End Source File
# Begin Source File

SOURCE=..\src\filename_absolute.cxx
DEP_CPP_FILEN=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_expand.cxx
DEP_CPP_FILENA=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_ext.cxx
DEP_CPP_FILENAM=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_isdir.cxx
DEP_CPP_FILENAME=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\fl\fl_export.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_list.cxx
DEP_CPP_FILENAME_=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\fl\fl_export.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_match.cxx
DEP_CPP_FILENAME_M=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_setext.cxx
DEP_CPP_FILENAME_S=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\fl\fl_export.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl.cxx
DEP_CPP_FL_CX=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Shared_Image.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Tooltip.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_win32.cxx"\
	"..\src\Fl_x.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_abort.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_add_idle.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Adjuster.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fastarrow.h"\
	"..\src\mediumarrow.h"\
	"..\src\slowarrow.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Align_Group.cxx
DEP_CPP_FL_AL=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Align_Group.H"\
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_arc.cxx
DEP_CPP_FL_AR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\math.h"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_arci.cxx
DEP_CPP_FL_ARC=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\math.h"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_arg.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_ask.cxx
DEP_CPP_FL_AS=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\fl_ask.H"\
	"..\FL\Fl_Box.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Return_Button.H"\
	"..\FL\Fl_Secret_Input.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Bitmap.cxx
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
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_bmp.cxx
DEP_CPP_FL_BM=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Shared_Image.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Box.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_boxtype.cxx
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
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser.cxx
DEP_CPP_FL_BR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Button.cxx
DEP_CPP_FL_BU=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Check_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\fl\fl_round_button.h"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_call_main.c
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Check_Button.cxx
DEP_CPP_FL_CH=\
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
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Choice.cxx
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
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_clip.cxx
DEP_CPP_FL_CL=\
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Clock.cxx
DEP_CPP_FL_CLO=\
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_cmap.h
# End Source File
# Begin Source File

SOURCE=..\src\fl_color.cxx
DEP_CPP_FL_CO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_cmap.h"\
	"..\src\fl_color_win32.cxx"\
	"..\src\fl_color_x.cxx"\
	"..\src\Fl_XColor.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Color_Chooser.cxx
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
	"..\FL\Fl_Float_Input.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Item.H"\
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_compose.cxx
DEP_CPP_FL_COM=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Counter.cxx
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
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_cursor.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_curve.cxx
DEP_CPP_FL_CUR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Dial.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_diamond_box.cxx
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
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_display.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_dnd.cxx
DEP_CPP_FL_DN=\
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
	"..\src\fl_dnd_win32.cxx"\
	"..\src\fl_dnd_x.cxx"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Double_Window.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw.cxx
DEP_CPP_FL_DR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_image.cxx
DEP_CPP_FL_DRA=\
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_draw_image_win32.cxx"\
	"..\src\fl_draw_image_x.cxx"\
	"..\src\Fl_XColor.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_pixmap.cxx
DEP_CPP_FL_DRAW=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_engraved_label.cxx
DEP_CPP_FL_EN=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_file_chooser.cxx
DEP_CPP_FL_FI=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileBrowser.cxx
DEP_CPP_FL_FIL=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\Fl\Fl_FileBrowser.h"\
	"..\Fl\Fl_FileIcon.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileChooser.cxx
DEP_CPP_FL_FILE=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Bitmap.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
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
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileChooser2.cxx
DEP_CPP_FL_FILEC=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\FL\Fl.H"\
	"..\FL\fl_ask.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
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
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileIcon.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileIcon2.cxx
DEP_CPP_FL_FILEIC=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\Fl\Fl_FileIcon.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileInput.cxx
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
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Float_Input.cxx
DEP_CPP_FL_FL=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Float_Input.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_font.cxx
DEP_CPP_FL_FO=\
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_font_win32.cxx"\
	"..\src\fl_font_x.cxx"\
	"..\src\fl_fontsize.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_fontsize.h
# End Source File
# Begin Source File

SOURCE=..\src\Fl_get_key.cxx
DEP_CPP_FL_GE=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_get_key_win32.cxx"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_gif.cxx
DEP_CPP_FL_GI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Shared_Image.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Choice.cxx
DEP_CPP_FL_GL=\
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
	"..\FL\gl.h"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\OpenGL\Fl_Gl_Choice.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Choice.H
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Overlay.cxx
DEP_CPP_FL_GL_=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Gl_Window.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\gl.h"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\OpenGL\Fl_Gl_Choice.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Window.cxx
DEP_CPP_FL_GL_W=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Gl_Window.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\gl.h"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\OpenGL\Fl_Gl_Choice.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_glyph.cxx
DEP_CPP_FL_GLY=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Group.cxx
DEP_CPP_FL_GR=\
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Highlight_Button.cxx
DEP_CPP_FL_HI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Button.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\fl\fl_highlight_button.h"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Image.cxx
DEP_CPP_FL_IM=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input.cxx
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
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input_Browser.cxx
DEP_CPP_FL_INP=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Input.H"\
	"..\fl\fl_input_browser.h"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Menu_Window.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Single_Window.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Item.cxx
DEP_CPP_FL_IT=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\Fl_Divider.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Item.H"\
	"..\FL\Fl_Item_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_key_name.cxx
DEP_CPP_FL_KE=\
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_labeltype.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Light_Button.cxx
DEP_CPP_FL_LI=\
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
	"..\FL\Fl_Light_Button.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_list_fonts.cxx
DEP_CPP_FL_LIS=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_fontsize.h"\
	"..\src\fl_list_fonts_win32.cxx"\
	"..\src\fl_list_fonts_x.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\forms\fl_load_browser.cxx
DEP_CPP_FL_LO=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Browser.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_load_plugin.cxx
DEP_CPP_FL_LOA=\
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
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_lock.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu.cxx
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
	"..\FL\Fl_Item.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Menu_Window.H"\
	"..\FL\Fl_Single_Window.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_.cxx
DEP_CPP_FL_MEN=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Item.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_add.cxx
DEP_CPP_FL_MENU=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\Fl_Divider.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Item.H"\
	"..\FL\Fl_Item_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Bar.cxx
DEP_CPP_FL_MENU_=\
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
	"..\FL\Fl_Menu_Bar.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Button.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_global.cxx
DEP_CPP_FL_MENU_G=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Item.cxx
DEP_CPP_FL_MENU_I=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\Fl_Divider.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Item.H"\
	"..\FL\Fl_Item_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Button.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Window.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_options.cxx
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
	"..\FL\fl_theme.H"\
	"..\FL\Fl_Widget.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Output.cxx
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
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_oval_box.cxx
DEP_CPP_FL_OV=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay.cxx
DEP_CPP_FL_OVE=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay_visual.cxx
DEP_CPP_FL_OVER=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Overlay_Window.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_own_colormap.cxx
DEP_CPP_FL_OW=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pack.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pixmap.cxx
DEP_CPP_FL_PI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Pixmap.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_rect.cxx
DEP_CPP_FL_RE=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Repeat_Button.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Return_Button.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_rgb.cxx
DEP_CPP_FL_RG=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_RGB_Image.cxx
DEP_CPP_FL_RGB=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\Fl\Fl_RGB_Image.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Roller.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_round_box.cxx
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
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_rounded_box.cxx
DEP_CPP_FL_ROUN=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scroll.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_scroll_area.cxx
DEP_CPP_FL_SCR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scrollbar.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_shadow_box.cxx
DEP_CPP_FL_SH=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Shaped_Window.cxx
DEP_CPP_FL_SHA=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Bitmap.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\Fl_Double_Window.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Shaped_Window.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Shared_Image.cxx
DEP_CPP_FL_SHAR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Bitmap.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Shared_Image.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_show_colormap.cxx
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
	"..\FL\fl_show_colormap.H"\
	"..\FL\Fl_Single_Window.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Single_Window.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Slider.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_String_List.cxx
DEP_CPP_FL_ST=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Item.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_String_List.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Style.cxx
DEP_CPP_FL_STY=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\fl_theme.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\math.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Style_start.cxx
DEP_CPP_FL_STYL=\
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
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_symbols.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_sysinfo.cxx
DEP_CPP_FL_SYS=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_sysinfo_win32.cxx"\
	"..\src\fl_sysinfo_x.cxx"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tabs.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Buffer.cxx
DEP_CPP_FL_TE=\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Text_Buffer.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Display.cxx
DEP_CPP_FL_TEX=\
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
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Text_Buffer.H"\
	"..\FL\Fl_Text_Display.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Editor.cxx
DEP_CPP_FL_TEXT=\
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
	"..\FL\Fl_Scrollbar.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Text_Buffer.H"\
	"..\FL\Fl_Text_Display.H"\
	"..\FL\Fl_Text_Editor.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tile.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tooltip.cxx
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
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Valuator.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Input.cxx
DEP_CPP_FL_VAL=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Float_Input.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Value_Input.H"\
	"..\FL\Fl_Widget.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Output.cxx
DEP_CPP_FL_VALU=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Value_Output.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Slider.cxx
DEP_CPP_FL_VALUE=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Input.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Output.H"\
	"..\FL\Fl_Slider.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Valuator.H"\
	"..\FL\Fl_Value_Slider.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_vertex.cxx
DEP_CPP_FL_VE=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\math.h"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_visual.cxx
DEP_CPP_FL_VI=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Widget.cxx
DEP_CPP_FL_WI=\
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window.cxx
DEP_CPP_FL_WIN=\
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_fullscreen.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_hotspot.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_iconize.cxx
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
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_XColor.H
# End Source File
# Begin Source File

SOURCE=..\src\fl_xpm.cxx
DEP_CPP_FL_XP=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Image.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Shared_Image.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\gl_draw.cxx
DEP_CPP_GL_DR=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl_Color.H"\
	"..\FL\fl_draw.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\gl.h"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\src\fl_fontsize.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\gl_start.cxx
DEP_CPP_GL_ST=\
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
	"..\FL\gl.h"\
	"..\FL\win32.H"\
	"..\FL\x.H"\
	"..\OpenGL\Fl_Gl_Choice.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\glut\glut_compatability.cxx
DEP_CPP_GLUT_=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Gl_Window.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Item.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Menu_.H"\
	"..\FL\Fl_Menu_Item.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\gl.h"\
	"..\FL\glut.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\glut\glut_font.cxx
DEP_CPP_GLUT_F=\
	"..\FL\Enumerations.H"\
	"..\FL\Fl.H"\
	"..\FL\Fl_Boxtype.H"\
	"..\FL\Fl_Color.H"\
	"..\fl\fl_export.h"\
	"..\FL\Fl_Flags.H"\
	"..\FL\Fl_Font.H"\
	"..\FL\Fl_Gl_Window.H"\
	"..\FL\Fl_Group.H"\
	"..\FL\Fl_Labeltype.H"\
	"..\FL\Fl_Style.H"\
	"..\FL\Fl_Widget.H"\
	"..\FL\Fl_Window.H"\
	"..\FL\gl.h"\
	"..\FL\glut.H"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\mediumarrow.h
# End Source File
# Begin Source File

SOURCE=..\src\numericsort.c
DEP_CPP_NUMER=\
	"..\FL\Enumerations.H"\
	"..\FL\filename.H"\
	"..\fl\fl_export.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\scandir.c
DEP_CPP_SCAND=\
	"..\src\scandir_win32.c"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\slowarrow.h
# End Source File
# Begin Source File

SOURCE=..\glut\trackball.c
DEP_CPP_TRACK=\
	"..\glut\trackball.h"\
	
# End Source File
# Begin Source File

SOURCE=..\glut\trackball.h
# End Source File
# Begin Source File

SOURCE=..\lib\fltkdll_imagesd.lib
# End Source File
# End Target
# End Project
