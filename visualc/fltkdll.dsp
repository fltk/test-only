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
# ADD CPP /nologo /MT /W3 /GR /GX /Os /Ob2 /I "." /I ".." /D "NDEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
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
# ADD CPP /nologo /MTd /GR /GX /ZI /Od /I "." /I ".." /I "../visualc" /D "_DEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
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
# ADD CPP /nologo /MDd /GR /GX /ZI /Od /I "." /I ".." /I "../visualc" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /D "_DEBUG" /D "_MSC_DLL" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
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
# ADD CPP /nologo /MD /W3 /GR /GX /Os /Ob2 /I "." /I ".." /D "NDEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /D "_MSC_DLL" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
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

SOURCE=..\src\fastarrow.h
# End Source File
# Begin Source File

SOURCE=..\src\filename_absolute.cxx
DEP_CPP_FILEN=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl_Export.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_expand.cxx
DEP_CPP_FILENA=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl_Export.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_ext.cxx
DEP_CPP_FILENAM=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl_Export.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_isdir.cxx
DEP_CPP_FILENAME=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl_Export.h"\
	".\config.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_list.cxx
DEP_CPP_FILENAME_=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl_Export.h"\
	".\config.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_match.cxx
DEP_CPP_FILENAME_M=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl_Export.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_setext.cxx
DEP_CPP_FILENAME_S=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl_Export.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl.cxx
DEP_CPP_FL_CX=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Tooltip.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fl_win32.cxx"\
	"..\src\Fl_x.cxx"\
	".\config.h"\
	{$(INCLUDE)}"dirent.h"\
	
NODEP_CPP_FL_CX=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_abort.cxx
DEP_CPP_FL_AB=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\vsnprintf.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_add_idle.cxx
DEP_CPP_FL_AD=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Adjuster.cxx
DEP_CPP_FL_ADJ=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Adjuster.h"\
	"..\fltk\Fl_Bitmap.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Image.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	"..\src\fastarrow.h"\
	"..\src\mediumarrow.h"\
	"..\src\slowarrow.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Align_Group.cxx
DEP_CPP_FL_AL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Align_Group.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_arc.cxx
DEP_CPP_FL_AR=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\math.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_arci.cxx
DEP_CPP_FL_ARC=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\math.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_ARC=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_arg.cxx
DEP_CPP_FL_ARG=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	{$(INCLUDE)}"dirent.h"\
	
NODEP_CPP_FL_ARG=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_ask.cxx
DEP_CPP_FL_AS=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\fl_ask.h"\
	"..\fltk\Fl_Box.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Return_Button.h"\
	"..\fltk\Fl_Secret_Input.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\vsnprintf.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Bar.cxx
DEP_CPP_FL_BA=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Bar.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Bitmap.cxx
DEP_CPP_FL_BI=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Bitmap.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Image.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_BI=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Box.cxx
DEP_CPP_FL_BO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Box.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_boxtype.cxx
DEP_CPP_FL_BOX=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	
NODEP_CPP_FL_BOX=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser.cxx
DEP_CPP_FL_BR=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Bitmap.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Browser.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Image.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Scrollbar.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	"..\src\browser_minus.xbm"\
	"..\src\browser_plus.xbm"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Button.cxx
DEP_CPP_FL_BU=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_call_main.c
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Check_Button.cxx
DEP_CPP_FL_CH=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Check_Button.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Choice.cxx
DEP_CPP_FL_CHO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Choice.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_clip.cxx
DEP_CPP_FL_CL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_CL=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Clock.cxx
DEP_CPP_FL_CLO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Clock.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_cmap.h
# End Source File
# Begin Source File

SOURCE=..\src\fl_color.cxx
DEP_CPP_FL_CO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fl_cmap.h"\
	"..\src\fl_color_win32.cxx"\
	"..\src\fl_color_x.cxx"\
	"..\src\Fl_XColor.H"\
	".\config.h"\
	
NODEP_CPP_FL_CO=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Color_Chooser.cxx
DEP_CPP_FL_COL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Box.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Choice.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Color_Chooser.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Float_Input.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Item.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Numeric_Input.h"\
	"..\fltk\Fl_Return_Button.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Value_Input.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\math.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_compose.cxx
DEP_CPP_FL_COM=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Counter.cxx
DEP_CPP_FL_COU=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Counter.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Output.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_cursor.cxx
DEP_CPP_FL_CU=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_CU=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_curve.cxx
DEP_CPP_FL_CUR=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Dial.cxx
DEP_CPP_FL_DI=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Dial.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\math.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_diamond_box.cxx
DEP_CPP_FL_DIA=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_display.cxx
DEP_CPP_FL_DIS=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_dnd.cxx
DEP_CPP_FL_DN=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fl_dnd_win32.cxx"\
	"..\src\fl_dnd_x.cxx"\
	
NODEP_CPP_FL_DN=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Double_Window.cxx
DEP_CPP_FL_DO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Double_Window.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	
NODEP_CPP_FL_DO=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw.cxx
DEP_CPP_FL_DR=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_image.cxx
DEP_CPP_FL_DRA=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fl_draw_image_win32.cxx"\
	"..\src\fl_draw_image_x.cxx"\
	"..\src\Fl_XColor.H"\
	".\config.h"\
	
NODEP_CPP_FL_DRA=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_pixmap.cxx
DEP_CPP_FL_DRAW=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_engraved_label.cxx
DEP_CPP_FL_EN=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_file_chooser.cxx
DEP_CPP_FL_FI=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Browser.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Choice.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\fl_file_chooser.h"\
	"..\fltk\Fl_FileBrowser.h"\
	"..\fltk\Fl_FileChooser.h"\
	"..\fltk\Fl_FileIcon.h"\
	"..\fltk\Fl_FileInput.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Return_Button.h"\
	"..\fltk\Fl_Scrollbar.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	".\config.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileBrowser.cxx
DEP_CPP_FL_FIL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Browser.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_FileBrowser.h"\
	"..\fltk\Fl_FileIcon.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Scrollbar.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\vsnprintf.h"\
	".\config.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileChooser.cxx
DEP_CPP_FL_FILE=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Bitmap.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Browser.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Choice.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_FileBrowser.h"\
	"..\fltk\Fl_FileChooser.h"\
	"..\fltk\Fl_FileIcon.h"\
	"..\fltk\Fl_FileInput.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Image.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Return_Button.h"\
	"..\fltk\Fl_Scrollbar.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\vsnprintf.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileChooser2.cxx
DEP_CPP_FL_FILEC=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl.h"\
	"..\fltk\fl_ask.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Browser.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Choice.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_FileBrowser.h"\
	"..\fltk\Fl_FileChooser.h"\
	"..\fltk\Fl_FileIcon.h"\
	"..\fltk\Fl_FileInput.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Return_Button.h"\
	"..\fltk\Fl_Scrollbar.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\vsnprintf.h"\
	".\config.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileIcon.cxx
DEP_CPP_FL_FILEI=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_FileIcon.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileIcon2.cxx
DEP_CPP_FL_FILEIC=\
	"..\fltk\Enumerations.h"\
	"..\fltk\filename.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_FileIcon.h"\
	"..\fltk\Fl_Flags.h"\
	".\config.h"\
	{$(INCLUDE)}"dirent.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileInput.cxx
DEP_CPP_FL_FILEIN=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_FileInput.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Float_Input.cxx
DEP_CPP_FL_FL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Float_Input.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Numeric_Input.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_font.cxx
DEP_CPP_FL_FO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fl_font_win32.cxx"\
	"..\src\fl_font_x.cxx"\
	"..\src\fl_font_xft.cxx"\
	".\config.h"\
	
NODEP_CPP_FL_FO=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_fontsize.h
# End Source File
# Begin Source File

SOURCE=..\src\Fl_get_key.cxx
DEP_CPP_FL_GE=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fl_get_key_win32.cxx"\
	
NODEP_CPP_FL_GE=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Choice.cxx
DEP_CPP_FL_GL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\gl.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\OpenGL\Fl_Gl_Choice.H"\
	".\config.h"\
	
NODEP_CPP_FL_GL=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Choice.H
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Overlay.cxx
DEP_CPP_FL_GL_=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Gl_Window.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\gl.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\OpenGL\Fl_Gl_Choice.H"\
	".\config.h"\
	
NODEP_CPP_FL_GL_=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Window.cxx
DEP_CPP_FL_GL_W=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Gl_Window.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\gl.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\OpenGL\Fl_Gl_Choice.H"\
	".\config.h"\
	
NODEP_CPP_FL_GL_W=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_glyph.cxx
DEP_CPP_FL_GLY=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Group.cxx
DEP_CPP_FL_GR=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Tooltip.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Highlight_Button.cxx
DEP_CPP_FL_HI=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Highlight_Button.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Image.cxx
DEP_CPP_FL_IM=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Image.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_IM=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input.cxx
DEP_CPP_FL_IN=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input_Browser.cxx
DEP_CPP_FL_INP=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Browser.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Input_Browser.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Menu_Window.h"\
	"..\fltk\Fl_Scrollbar.h"\
	"..\fltk\Fl_Single_Window.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Item.cxx
DEP_CPP_FL_IT=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Check_Button.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Divider.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Image.h"\
	"..\fltk\Fl_Item.h"\
	"..\fltk\Fl_Item_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_key_name.cxx
DEP_CPP_FL_KE=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_KE=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_labeltype.cxx
DEP_CPP_FL_LA=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Image.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Light_Button.cxx
DEP_CPP_FL_LI=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Check_Button.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Light_Button.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_list_fonts.cxx
DEP_CPP_FL_LIS=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fl_list_fonts_win32.cxx"\
	"..\src\fl_list_fonts_x.cxx"\
	".\config.h"\
	
NODEP_CPP_FL_LIS=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\forms\fl_load_browser.cxx
DEP_CPP_FL_LO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Browser.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Scrollbar.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_load_plugin.cxx
DEP_CPP_FL_LOA=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\fl_load_plugin.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_lock.cxx
DEP_CPP_FL_LOC=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	".\config.h"\
	{$(INCLUDE)}"config\_epilog.h"\
	{$(INCLUDE)}"config\_msvc_warnings_off.h"\
	{$(INCLUDE)}"config\_prolog.h"\
	{$(INCLUDE)}"config\stl_apcc.h"\
	{$(INCLUDE)}"config\stl_apple.h"\
	{$(INCLUDE)}"config\stl_as400.h"\
	{$(INCLUDE)}"config\stl_bc.h"\
	{$(INCLUDE)}"config\stl_como.h"\
	{$(INCLUDE)}"config\stl_confix.h"\
	{$(INCLUDE)}"config\stl_dec.h"\
	{$(INCLUDE)}"config\stl_dec_vms.h"\
	{$(INCLUDE)}"config\stl_fujitsu.h"\
	{$(INCLUDE)}"config\stl_gcc.h"\
	{$(INCLUDE)}"config\stl_hpacc.h"\
	{$(INCLUDE)}"config\stl_ibm.h"\
	{$(INCLUDE)}"config\stl_intel.h"\
	{$(INCLUDE)}"config\stl_kai.h"\
	{$(INCLUDE)}"config\stl_msvc.h"\
	{$(INCLUDE)}"config\stl_mwerks.h"\
	{$(INCLUDE)}"config\stl_mycomp.h"\
	{$(INCLUDE)}"config\stl_sco.h"\
	{$(INCLUDE)}"config\stl_select_lib.h"\
	{$(INCLUDE)}"config\stl_sgi.h"\
	{$(INCLUDE)}"config\stl_solaris.h"\
	{$(INCLUDE)}"config\stl_sunpro.h"\
	{$(INCLUDE)}"config\stl_symantec.h"\
	{$(INCLUDE)}"config\stl_watcom.h"\
	{$(INCLUDE)}"config\stl_wince.h"\
	{$(INCLUDE)}"config\stlcomp.h"\
	{$(INCLUDE)}"config\vc_select_lib.h"\
	{$(INCLUDE)}"pthread.h"\
	{$(INCLUDE)}"stl\_abbrevs.h"\
	{$(INCLUDE)}"stl\_config.h"\
	{$(INCLUDE)}"stl\_config_compat.h"\
	{$(INCLUDE)}"stl\_config_compat_post.h"\
	{$(INCLUDE)}"stl\_epilog.h"\
	{$(INCLUDE)}"stl\_prolog.h"\
	{$(INCLUDE)}"stl\_site_config.h"\
	{$(INCLUDE)}"stl_user_config.h"\
	
NODEP_CPP_FL_LOC=\
	"..\..\usr\include\pthread.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu.cxx
DEP_CPP_FL_ME=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Item.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Menu_Window.h"\
	"..\fltk\Fl_Single_Window.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_.cxx
DEP_CPP_FL_MEN=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Item.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_add.cxx
DEP_CPP_FL_MENU=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Divider.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Item.h"\
	"..\fltk\Fl_Item_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\src\array.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Bar.cxx
DEP_CPP_FL_MENU_=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Bar.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Button.cxx
DEP_CPP_FL_MENU_B=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Item.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Button.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_global.cxx
DEP_CPP_FL_MENU_G=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Item.cxx
DEP_CPP_FL_MENU_I=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Divider.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Item.h"\
	"..\fltk\Fl_Item_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Button.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Window.cxx
DEP_CPP_FL_MENU_W=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_Window.h"\
	"..\fltk\Fl_Single_Window.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	
NODEP_CPP_FL_MENU_W=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Numeric_Input.cxx
DEP_CPP_FL_NU=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Numeric_Input.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Output.cxx
DEP_CPP_FL_OU=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Output.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_oval_box.cxx
DEP_CPP_FL_OV=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay.cxx
DEP_CPP_FL_OVE=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_OVE=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay_visual.cxx
DEP_CPP_FL_OVER=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	
NODEP_CPP_FL_OVER=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Overlay_Window.cxx
DEP_CPP_FL_OVERL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Double_Window.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Overlay_Window.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	
NODEP_CPP_FL_OVERL=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_own_colormap.cxx
DEP_CPP_FL_OW=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	
NODEP_CPP_FL_OW=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pack.cxx
DEP_CPP_FL_PA=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Pack.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pixmap.cxx
DEP_CPP_FL_PI=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Image.h"\
	"..\fltk\Fl_Pixmap.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_PI=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_plastic_box.cxx
DEP_CPP_FL_PL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_ProgressBar.cxx
DEP_CPP_FL_PR=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Box.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_ProgressBar.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Radio_Button.cxx
DEP_CPP_FL_RA=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Check_Button.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\fl_radio_button.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_rect.cxx
DEP_CPP_FL_RE=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_RE=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Repeat_Button.cxx
DEP_CPP_FL_REP=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Repeat_Button.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Return_Button.cxx
DEP_CPP_FL_RET=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Button.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Return_Button.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_rgb.cxx
DEP_CPP_FL_RG=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_RG=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_RGB_Image.cxx
DEP_CPP_FL_RGB=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Image.h"\
	"..\fltk\Fl_RGB_Image.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_RGB=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Roller.cxx
DEP_CPP_FL_RO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Roller.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_round_box.cxx
DEP_CPP_FL_ROU=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_rounded_box.cxx
DEP_CPP_FL_ROUN=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scroll.cxx
DEP_CPP_FL_SC=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Scroll.h"\
	"..\fltk\Fl_Scrollbar.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_scroll_area.cxx
DEP_CPP_FL_SCR=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_SCR=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scrollbar.cxx
DEP_CPP_FL_SCRO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Scrollbar.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_shadow_box.cxx
DEP_CPP_FL_SH=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Shaped_Window.cxx
DEP_CPP_FL_SHA=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Bitmap.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Double_Window.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Image.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Shaped_Window.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_SHA=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_show_colormap.cxx
DEP_CPP_FL_SHO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\fl_show_colormap.h"\
	"..\fltk\Fl_Single_Window.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Single_Window.cxx
DEP_CPP_FL_SI=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Single_Window.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Slider.cxx
DEP_CPP_FL_SL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_String_List.cxx
DEP_CPP_FL_ST=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Item.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_String_List.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Style.cxx
DEP_CPP_FL_STY=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\fl_load_plugin.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\math.h"\
	"..\fltk\vsnprintf.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Style_Set.cxx
DEP_CPP_FL_STYL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\fl_style_set.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_symbols.cxx
DEP_CPP_FL_SY=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tabs.cxx
DEP_CPP_FL_TA=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Tabs.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Buffer.cxx
DEP_CPP_FL_TE=\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Text_Buffer.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Display.cxx
DEP_CPP_FL_TEX=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Scrollbar.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Text_Buffer.h"\
	"..\fltk\Fl_Text_Display.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Editor.cxx
DEP_CPP_FL_TEXT=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Scrollbar.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Text_Buffer.h"\
	"..\fltk\Fl_Text_Display.h"\
	"..\fltk\Fl_Text_Editor.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tile.cxx
DEP_CPP_FL_TI=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Tile.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tiled_Image.cxx
DEP_CPP_FL_TIL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Image.h"\
	"..\fltk\Fl_Tiled_Image.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tooltip.cxx
DEP_CPP_FL_TO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_Window.h"\
	"..\fltk\Fl_Single_Window.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Tooltip.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Valuator.cxx
DEP_CPP_FL_VA=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\math.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Input.cxx
DEP_CPP_FL_VAL=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Float_Input.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Numeric_Input.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Value_Input.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Output.cxx
DEP_CPP_FL_VALU=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Output.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Value_Output.h"\
	"..\fltk\Fl_Widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Slider.cxx
DEP_CPP_FL_VALUE=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Input.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Output.h"\
	"..\fltk\Fl_Slider.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Valuator.h"\
	"..\fltk\Fl_Value_Slider.h"\
	"..\fltk\Fl_Widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_vertex.cxx
DEP_CPP_FL_VE=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\math.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_VE=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_visual.cxx
DEP_CPP_FL_VI=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	
NODEP_CPP_FL_VI=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Widget.cxx
DEP_CPP_FL_WI=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window.cxx
DEP_CPP_FL_WIN=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	
NODEP_CPP_FL_WIN=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_fullscreen.cxx
DEP_CPP_FL_WIND=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_WIND=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_hotspot.cxx
DEP_CPP_FL_WINDO=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_iconize.cxx
DEP_CPP_FL_WINDOW=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	
NODEP_CPP_FL_WINDOW=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_XColor.H
# End Source File
# Begin Source File

SOURCE=..\src\fltk_theme.cxx
DEP_CPP_FLTK_=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\gl_draw.cxx
DEP_CPP_GL_DR=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\gl.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\OpenGL\Fl_Gl_Choice.H"\
	".\config.h"\
	
NODEP_CPP_GL_DR=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\gl_start.cxx
DEP_CPP_GL_ST=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\fl_draw.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\gl.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\OpenGL\Fl_Gl_Choice.H"\
	".\config.h"\
	
NODEP_CPP_GL_ST=\
	"..\fltk\mac.H"\
	
# End Source File
# Begin Source File

SOURCE=..\glut\glut_compatability.cxx
DEP_CPP_GLUT_=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Gl_Window.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Item.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Menu_.h"\
	"..\fltk\Fl_Menu_Item.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\gl.h"\
	"..\fltk\glut.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\glut\glut_font.cxx
DEP_CPP_GLUT_F=\
	"..\fltk\Enumerations.h"\
	"..\fltk\Fl.h"\
	"..\fltk\Fl_Boxtype.h"\
	"..\fltk\Fl_Color.h"\
	"..\fltk\Fl_Export.h"\
	"..\fltk\Fl_Flags.h"\
	"..\fltk\Fl_Font.h"\
	"..\fltk\Fl_Gl_Window.h"\
	"..\fltk\Fl_Group.h"\
	"..\fltk\Fl_Labeltype.h"\
	"..\fltk\Fl_Style.h"\
	"..\fltk\Fl_Widget.h"\
	"..\fltk\Fl_Window.h"\
	"..\fltk\gl.h"\
	"..\fltk\glut.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\mediumarrow.h
# End Source File
# Begin Source File

SOURCE=..\src\scandir.c
DEP_CPP_SCAND=\
	"..\src\scandir_win32.c"\
	".\config.h"\
	{$(INCLUDE)}"dirent.h"\
	
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
# End Target
# End Project
