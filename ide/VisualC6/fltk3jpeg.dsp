# Microsoft Developer Studio Project File - Name="fltk3jpeg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fltk3jpeg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltk3jpeg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltk3jpeg.mak" CFG="fltk3jpeg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltk3jpeg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fltk3jpeg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fltk3jpeg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release\fltk3jpeg"
# PROP BASE Intermediate_Dir "Release\fltk3jpeg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\fltk3jpeg"
# PROP Intermediate_Dir "Release\fltk3jpeg"
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
# ADD LIB32 /nologo /out:"..\..\lib\fltk3jpeg.lib"

!ELSEIF  "$(CFG)" == "fltk3jpeg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug\fltk3jpeg"
# PROP BASE Intermediate_Dir "Debug\fltk3jpeg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\fltk3jpeg"
# PROP Intermediate_Dir "Debug\fltk3jpeg"
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
# ADD LIB32 /nologo /out:"..\..\lib\fltk3jpegd.lib"

!ENDIF 

# Begin Target

# Name "fltk3jpeg - Win32 Release"
# Name "fltk3jpeg - Win32 Debug"
# Begin Source File

SOURCE=..\..\src\jpeg\jaricom.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcarith.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdarith.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcapimin.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcapistd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jccoefct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jccolor.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jchuff.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcinit.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcmainct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcmarker.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcmaster.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcomapi.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcparam.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcprepct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jcsample.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jctrans.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdapimin.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdapistd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdatadst.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdcolor.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdhuff.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdinput.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdmainct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdmarker.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdmaster.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdmerge.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdpostct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdsample.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jdtrans.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jerror.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jfdctint.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jidctflt.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jidctfst.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jidctint.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jquant1.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jquant2.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpeg\jutils.c
# End Source File
# End Target
# End Project
