# Microsoft Developer Studio Project File - Name="fltk3png" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fltk3png - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltk3png.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltk3png.mak" CFG="fltk3png - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltk3png - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fltk3png - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fltk3png - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release\fltk3png"
# PROP BASE Intermediate_Dir "Release\fltk3png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\fltk3png"
# PROP Intermediate_Dir "Release\fltk3png"
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
# ADD LIB32 /nologo /out:"..\..\lib\fltk3png.lib"

!ELSEIF  "$(CFG)" == "fltk3png - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug\fltk3png"
# PROP BASE Intermediate_Dir "Debug\fltk3png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\fltk3png"
# PROP Intermediate_Dir "Debug\fltk3png"
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
# ADD LIB32 /nologo /out:"..\..\lib\fltk3pngd.lib"

!ENDIF 

# Begin Target

# Name "fltk3png - Win32 Release"
# Name "fltk3png - Win32 Debug"
# Begin Source File

SOURCE=..\..\src\fltk3png\png.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngerror.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngget.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngmem.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngpread.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngread.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngrio.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngrtran.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngrutil.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngset.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngtrans.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngwio.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngwrite.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngwtran.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk3png\pngwutil.c
# End Source File
# End Target
# End Project
