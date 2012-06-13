# Microsoft Developer Studio Project File - Name="fltk3jpegdll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fltk3jpegdll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltk3jpegdll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltk3jpegdll.mak" CFG="fltk3jpegdll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltk3jpegdll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltk3jpegdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fltk3jpegdll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release\fltk3jpegdll"
# PROP BASE Intermediate_Dir "Release\fltk3jpegdll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\fltk3jpegdll"
# PROP Intermediate_Dir "Release\fltk3jpegdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Os /Ob2 /I "..\..\include\fltk3zlib" /I "..\..\include\fltk3png" /I "..\..\include\fltk3jpeg" /I "." /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_NONSTDC_NO_DEPRECATE" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltk3jpegdll.pdb" /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltk3jpegdll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug\fltk3jpegdll"
# PROP BASE Intermediate_Dir "Debug\fltk3jpegdll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\fltk3jpegdll"
# PROP Intermediate_Dir "Debug\fltk3jpegdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /GX /ZI /Od /I "." /I "..\..\include" /I "..\..\include\fltk3zlib" /I "..\..\include\fltk3png" /I "..\..\include\fltk3jpeg" /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_NONSTDC_NO_DEPRECATE" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltk3jpegdlld.pdb" /debug /machine:I386 /out:"Debug\fltk3jpegdll\fltk3jpegdlld.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "fltk3jpegdll - Win32 Release"
# Name "fltk3jpegdll - Win32 Debug"
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jaricom.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcarith.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdarith.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcapimin.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcapistd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jccoefct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jccolor.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jchuff.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcinit.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcmainct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcmarker.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcmaster.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcomapi.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcparam.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcprepct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jcsample.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jctrans.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdapimin.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdapistd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdatadst.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdcolor.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdhuff.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdinput.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdmainct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdmarker.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdmaster.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdmerge.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdpostct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdsample.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jdtrans.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jerror.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jfdctint.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jidctflt.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jidctfst.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jidctint.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jquant1.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jquant2.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3jpeg\jutils.c
# End Source File
# End Target
# End Project
