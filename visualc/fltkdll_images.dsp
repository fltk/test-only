# Microsoft Developer Studio Project File - Name="fltkdll_images" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fltkdll_images - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll_images.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll_images.mak" CFG="fltkdll_images - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltkdll_images - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltkdll_images - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fltkdll_images - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "fltkdll_images0"
# PROP BASE Intermediate_Dir "fltkdll_images0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "fltkdll_images"
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
# ADD LINK32 fltkdll.lib opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdll_images.pdb" /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltkdll_images - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fltkdll_images1"
# PROP BASE Intermediate_Dir "fltkdll_images1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "fltkdll_imagesd"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /GX /Od /I "." /I ".." /I "../visualc" /D "_DEBUG" /D "FL_DLL" /D "FL_IMAGES_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /ZI /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 fltkdlld.lib opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:no /pdb:"fltkdll_imagesd.pdb" /debug /machine:I386 /out:"../lib/fltkdll_imagesd.dll" /pdbtype:sept /libpath:"../lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "fltkdll_images - Win32 Release"
# Name "fltkdll_images - Win32 Debug"
# Begin Source File

SOURCE=..\images\Fl_Guess_Image.cxx

!IF  "$(CFG)" == "fltkdll_images - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll_images - Win32 Debug"

DEP_CPP_FL_GU=\
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

SOURCE=..\images\fl_jpeg.cxx

!IF  "$(CFG)" == "fltkdll_images - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll_images - Win32 Debug"

DEP_CPP_FL_JP=\
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
	".\config.h"\
	{$(INCLUDE)}"jconfig.h"\
	{$(INCLUDE)}"jmorecfg.h"\
	{$(INCLUDE)}"jpeglib.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_FL_JP=\
	"..\..\..\..\Program Files\DevStudio\VC\INCLUDE\jerror.h"\
	"..\..\..\..\Program Files\DevStudio\VC\INCLUDE\jpegint.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\images\fl_png.cxx

!IF  "$(CFG)" == "fltkdll_images - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll_images - Win32 Debug"

DEP_CPP_FL_PN=\
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
	".\config.h"\
	{$(INCLUDE)}"sys\types.h"\
	

!ENDIF 

# End Source File
# End Target
# End Project
