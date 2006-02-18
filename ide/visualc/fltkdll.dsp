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
# PROP Output_Dir "../../lib"
# PROP Intermediate_Dir "fltkdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /Os /Ob2 /I "." /I "..\.." /I "..\..\.." /D "NDEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib msimg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib msimg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:2.0 /subsystem:windows /dll /pdb:"fltkdll.pdb" /machine:I386 /nodefaultlib:"libc"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fltkdll1"
# PROP BASE Intermediate_Dir "fltkdll1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../lib"
# PROP Intermediate_Dir "fltkdlld"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /GR /GX /ZI /Od /I "." /I "..\.." /I "..\..\.." /D "_DEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib msimg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib msimg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:2.0 /subsystem:windows /dll /pdb:"fltkdlld.pdb" /debug /machine:I386 /out:"../../lib/fltkdlld.dll" /pdbtype:sept
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
# ADD BASE CPP /nologo /MTd /GX /ZI /Od /I "." /I "..\.." /I "../visualc" /D "_DEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /YX /c
# ADD CPP /nologo /MDd /GR /GX /ZI /Od /I "." /I "..\.." /I "..\..\.." /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /D "_DEBUG" /D "_MSC_DLL" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib msimg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdlld.pdb" /debug /machine:I386 /out:"../../lib/fltkdlld.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib msimg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:2.0 /subsystem:windows /dll /pdb:"fltkdlld.pdb" /debug /machine:I386 /out:"../../lib/fltkdlld.dll" /implib:"../../lib/fltkdlld.lib" /pdbtype:sept
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
# ADD BASE CPP /nologo /MT /W3 /GX /Os /Ob2 /I "." /I "..\.." /D "NDEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /YX /c
# ADD CPP /MD /W3 /GR /GX /Os /Ob2 /Gf /Gy /I "." /I "..\.." /I "..\..\.." /D "NDEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /Fr /c
# SUBTRACT CPP /nologo /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib msimg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdll.pdb" /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib msimg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:2.0 /subsystem:windows /dll /pdb:"fltkdll.pdb" /machine:I386 /out:"../../lib/fltkdll.dll" /implib:"../../lib/fltkdll.lib"
# SUBTRACT LINK32 /pdb:none /force

!ENDIF 

# Begin Target

# Name "fltkdll - Win32 Release"
# Name "fltkdll - Win32 Debug"
# Name "fltkdll - Win32 Debug MinSize"
# Name "fltkdll - Win32 Release MinSize"
# Begin Source File

SOURCE=..\..\src\add_idle.cxx
DEP_CPP_ADD_I=\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\run.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\addarc.cxx
DEP_CPP_ADDAR=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\addcurve.cxx
DEP_CPP_ADDCU=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Adjuster.cxx
DEP_CPP_ADJUS=\
	"..\..\fltk\adjuster.h"\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\xbmimage.h"\
	"..\..\src\fastarrow.h"\
	"..\..\src\mediumarrow.h"\
	"..\..\src\slowarrow.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\AlignGroup.cxx
DEP_CPP_ALIGN=\
	"..\..\fltk\aligngroup.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\args.cxx
DEP_CPP_ARGS_=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\error.h"\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\Monitor.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\run.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\array.h
# End Source File
# Begin Source File

SOURCE=..\..\src\BarGroup.cxx
DEP_CPP_BARGR=\
	"..\..\fltk\bargroup.h"\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\bmpImage.cxx
DEP_CPP_BMPIM=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\error.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Browser.cxx
DEP_CPP_BROWS=\
	"..\..\fltk\box.h"\
	"..\..\fltk\browser.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\cursor.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\error.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\font.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\layout.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Button.cxx
DEP_CPP_BUTTO=\
	"..\..\fltk\box.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\CheckButton.cxx
DEP_CPP_CHECK=\
	"..\..\fltk\box.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\checkbutton.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Choice.cxx
DEP_CPP_CHOIC=\
	"..\..\fltk\box.h"\
	"..\..\fltk\choice.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\clip.cxx
DEP_CPP_CLIP_=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	"..\..\src\osx\clip.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Clock.cxx
DEP_CPP_CLOCK=\
	"..\..\fltk\clock.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Color.cxx
DEP_CPP_COLOR=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\color_chooser.cxx
DEP_CPP_COLOR_=\
	"..\..\fltk\ask.h"\
	"..\..\fltk\box.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\colorchooser.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\floatinput.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\layout.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\numericinput.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\popupmenu.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\returnbutton.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\valueinput.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\colormap.h
# End Source File
# Begin Source File

SOURCE=..\..\src\ComboBox.cxx
DEP_CPP_COMBO=\
	"..\..\fltk\box.h"\
	"..\..\fltk\choice.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\ComboBox.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\compose.cxx
DEP_CPP_COMPO=\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Cursor.cxx
DEP_CPP_CURSO=\
	"..\..\fltk\color.h"\
	"..\..\fltk\cursor.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\CycleButton.cxx
DEP_CPP_CYCLE=\
	"..\..\fltk\box.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\cyclebutton.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\default_glyph.cxx
DEP_CPP_DEFAU=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Dial.cxx
DEP_CPP_DIAL_=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\dial.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\DiamondBox.cxx
DEP_CPP_DIAMO=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\dnd.cxx
DEP_CPP_DND_C=\
	"..\..\fltk\color.h"\
	"..\..\fltk\cursor.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\run.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	"..\..\src\osx\dnd.cxx"\
	"..\..\src\win32\dnd.cxx"\
	"..\..\src\x11\dnd.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\draw_xpm.cxx
DEP_CPP_DRAW_=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\drawimage.cxx
DEP_CPP_DRAWI=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\error.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\font.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\src\osx\drawimage.cxx"\
	"..\..\src\win32\drawimage.cxx"\
	"..\..\src\x11\drawimage.cxx"\
	"..\..\src\x11\XColorMap.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\drawtext.cxx
DEP_CPP_DRAWT=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\font.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\dump_compose.c
# End Source File
# Begin Source File

SOURCE=..\..\src\EngravedLabel.cxx
DEP_CPP_ENGRA=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\labeltype.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\error.cxx
DEP_CPP_ERROR=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\error.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\utf.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\event_key_state.cxx
DEP_CPP_EVENT=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\src\osx\event_key_state.cxx"\
	"..\..\src\win32\event_key_state.cxx"\
	"..\..\src\x11\event_key_state.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\fastarrow.h
# End Source File
# Begin Source File

SOURCE=..\..\src\file_chooser.cxx
DEP_CPP_FILE_=\
	"..\..\fltk\browser.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\choice.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\file_chooser.h"\
	"..\..\fltk\filebrowser.h"\
	"..\..\fltk\filechooser.h"\
	"..\..\fltk\fileicon.h"\
	"..\..\fltk\fileinput.h"\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\returnbutton.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\FileBrowser.cxx
DEP_CPP_FILEB=\
	"..\..\fltk\browser.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\filebrowser.h"\
	"..\..\fltk\fileicon.h"\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\utf.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\FileChooser.cxx
DEP_CPP_FILEC=\
	"..\..\fltk\browser.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\choice.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\filebrowser.h"\
	"..\..\fltk\filechooser.h"\
	"..\..\fltk\fileicon.h"\
	"..\..\fltk\fileinput.h"\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\returnbutton.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\xbmimage.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\FileChooser2.cxx
DEP_CPP_FILECH=\
	"..\..\fltk\ask.h"\
	"..\..\fltk\browser.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\choice.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\filebrowser.h"\
	"..\..\fltk\filechooser.h"\
	"..\..\fltk\fileicon.h"\
	"..\..\fltk\fileinput.h"\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\returnbutton.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\utf.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\FileIcon.cxx
DEP_CPP_FILEI=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fileicon.h"\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\FileIcon2.cxx
DEP_CPP_FILEIC=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fileicon.h"\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\FileInput.cxx
DEP_CPP_FILEIN=\
	"..\..\fltk\color.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fileinput.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\filename_ext.cxx
DEP_CPP_FILEN=\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\filename_isdir.cxx
DEP_CPP_FILENA=\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\utf.h"\
	".\config.h"\
	

!IF  "$(CFG)" == "fltkdll - Win32 Release"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Debug MinSize"

!ELSEIF  "$(CFG)" == "fltkdll - Win32 Release MinSize"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\filename_list.cxx
DEP_CPP_FILENAM=\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\utf.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\filename_match.cxx
DEP_CPP_FILENAME=\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\filename_normalize.cxx
DEP_CPP_FILENAME_=\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\string.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\fillarc.cxx
DEP_CPP_FILLA=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\fillrect.cxx
DEP_CPP_FILLR=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Fl_Menu_Item.cxx
DEP_CPP_FL_ME=\
	"..\..\fl\fl_menu_item.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\divider.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\itemgroup.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\popupmenu.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\FloatInput.cxx
DEP_CPP_FLOAT=\
	"..\..\fltk\color.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\floatinput.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\numericinput.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\fltk_theme.cxx
DEP_CPP_FLTK_=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\Monitor.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\utf.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\src\win32\fltk_theme.cxx"\
	"..\..\src\x11\fltk_theme.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Font.cxx
DEP_CPP_FONT_=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\error.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\font.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\utf.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	"..\..\src\osx\Font.cxx"\
	"..\..\src\osx\utf8tomac.cxx"\
	"..\..\src\win32\Font.cxx"\
	"..\..\src\x11\Font.cxx"\
	"..\..\src\x11\Font_xft.cxx"\
	"..\..\src\x11\Font_xlfd.cxx"\
	"..\..\src\x11\IFont.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\gifImage.cxx
DEP_CPP_GIFIM=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Group.cxx
DEP_CPP_GROUP=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\layout.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\GSave.cxx
DEP_CPP_GSAVE=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\HighlightButton.cxx
DEP_CPP_HIGHL=\
	"..\..\fltk\button.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\highlightbutton.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Image.cxx
DEP_CPP_IMAGE=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\fltk\xbmimage.h"\
	"..\..\src\osx\Image.cxx"\
	"..\..\src\win32\Image.cxx"\
	"..\..\src\x11\Image.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Input.cxx
DEP_CPP_INPUT=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\utf.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\InputBrowser.cxx
DEP_CPP_INPUTB=\
	"..\..\fltk\box.h"\
	"..\..\fltk\browser.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\inputbrowser.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\menuwindow.h"\
	"..\..\fltk\Monitor.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\InvisibleWidget.cxx
DEP_CPP_INVIS=\
	"..\..\fltk\color.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\invisiblebox.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Item.cxx
DEP_CPP_ITEM_=\
	"..\..\fltk\button.h"\
	"..\..\fltk\checkbutton.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\divider.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\itemgroup.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\key_name.cxx
DEP_CPP_KEY_N=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\LightButton.cxx
DEP_CPP_LIGHT=\
	"..\..\fltk\box.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\checkbutton.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\lightbutton.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\list_fonts.cxx
DEP_CPP_LIST_=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\font.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\utf.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\src\osx\list_fonts.cxx"\
	"..\..\src\win32\list_fonts.cxx"\
	"..\..\src\x11\IFont.h"\
	"..\..\src\x11\list_fonts.cxx"\
	"..\..\src\x11\list_fonts_xlfd.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\load_plugin.cxx
DEP_CPP_LOAD_=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\load_plugin.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\utf.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\src\dlload_osx.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\lock.cxx
DEP_CPP_LOCK_=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\run.h"\
	"..\..\fltk\Threads.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\src\win32\lock.cxx"\
	"..\..\src\x11\lock.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\mediumarrow.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Menu.cxx
DEP_CPP_MENU_=\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Menu_add.cxx
DEP_CPP_MENU_A=\
	"..\..\fltk\color.h"\
	"..\..\fltk\divider.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\itemgroup.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\src\array.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Menu_global.cxx
DEP_CPP_MENU_G=\
	"..\..\fltk\color.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Menu_popup.cxx
DEP_CPP_MENU_P=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\labeltype.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\menuwindow.h"\
	"..\..\fltk\Monitor.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\run.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\MenuBar.cxx
DEP_CPP_MENUB=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\menubar.h"\
	"..\..\fltk\menuwindow.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\tooltip.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\MenuWindow.cxx
DEP_CPP_MENUW=\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\menuwindow.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\message.cxx
DEP_CPP_MESSA=\
	"..\..\fltk\ask.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\returnbutton.h"\
	"..\..\fltk\secretinput.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\MultiImage.cxx
DEP_CPP_MULTI=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\multiimage.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\NumericInput.cxx
DEP_CPP_NUMER=\
	"..\..\fltk\color.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\numericinput.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Output.cxx
DEP_CPP_OUTPU=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\output.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\OvalBox.cxx
DEP_CPP_OVALB=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\overlay_rect.cxx
DEP_CPP_OVERL=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\own_colormap.cxx
DEP_CPP_OWN_C=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\visual.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\PackedGroup.cxx
DEP_CPP_PACKE=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\layout.h"\
	"..\..\fltk\packedgroup.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\path.cxx
DEP_CPP_PATH_=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\PlasticBox.cxx
DEP_CPP_PLAST=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\PopupMenu.cxx
DEP_CPP_POPUP=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\popupmenu.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\ProgressBar.cxx
DEP_CPP_PROGR=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\progressbar.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\RadioButton.cxx
DEP_CPP_RADIO=\
	"..\..\fltk\box.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\checkbutton.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\radiobutton.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\readimage.cxx
DEP_CPP_READI=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\src\osx\readimage.cxx"\
	"..\..\src\win32\readimage.cxx"\
	"..\..\src\x11\readimage.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\RepeatButton.cxx
DEP_CPP_REPEA=\
	"..\..\fltk\button.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\repeatbutton.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\ReturnButton.cxx
DEP_CPP_RETUR=\
	"..\..\fltk\button.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\returnbutton.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\rgbImage.cxx
DEP_CPP_RGBIM=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\rgbimage.h"\
	"..\..\fltk\symbol.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\RoundBox.cxx
DEP_CPP_ROUND=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\RoundedBox.cxx
DEP_CPP_ROUNDE=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\run.cxx
DEP_CPP_RUN_C=\
	"..\..\fltk\browser.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\error.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\FL_VERSION.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\font.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\layout.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\menuwindow.h"\
	"..\..\fltk\Monitor.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\run.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\tooltip.h"\
	"..\..\fltk\utf.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\visual.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	"..\..\src\osx\mactoutf8.cxx"\
	"..\..\src\osx\run.cxx"\
	"..\..\src\win32\pktdef.h"\
	"..\..\src\win32\run.cxx"\
	"..\..\src\win32\wintab.h"\
	"..\..\src\x11\run.cxx"\
	"..\..\src\x11\xutf8.cxx"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\scandir.c
DEP_CPP_SCAND=\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\utf.h"\
	"..\..\src\win32\scandir.c"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Scrollbar.cxx
DEP_CPP_SCROL=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\ScrollGroup.cxx
DEP_CPP_SCROLL=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\layout.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\scrollgroup.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\scrollrect.cxx
DEP_CPP_SCROLLR=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\setcolor.cxx
DEP_CPP_SETCO=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\src\colormap.h"\
	"..\..\src\osx\setcolor.cxx"\
	"..\..\src\win32\setcolor.cxx"\
	"..\..\src\x11\setcolor.cxx"\
	"..\..\src\x11\XColorMap.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\setdisplay.cxx
DEP_CPP_SETDI=\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\run.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\setvisual.cxx
DEP_CPP_SETVI=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\visual.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\ShadowBox.cxx
DEP_CPP_SHADO=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\ShapedWindow.cxx
DEP_CPP_SHAPE=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\shapedwindow.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	"..\..\fltk\xbmimage.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\SharedImage.cxx
DEP_CPP_SHARE=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\xbmimage.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\ShortcutAssignment.cxx
DEP_CPP_SHORT=\
	"..\..\fltk\color.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\show_colormap.cxx
DEP_CPP_SHOW_=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\menuwindow.h"\
	"..\..\fltk\Monitor.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\show_colormap.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Slider.cxx
DEP_CPP_SLIDE=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\slowarrow.h
# End Source File
# Begin Source File

SOURCE=..\..\src\string.c
DEP_CPP_STRIN=\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\string.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\StringList.cxx
DEP_CPP_STRING=\
	"..\..\fltk\color.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\item.h"\
	"..\..\fltk\menu.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\stringlist.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Style.cxx
DEP_CPP_STYLE=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\run.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\StyleSet.cxx
DEP_CPP_STYLES=\
	"..\..\fltk\color.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\styleset.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Symbol.cxx
DEP_CPP_SYMBO=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\error.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\TabGroup.cxx
DEP_CPP_TABGR=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\layout.h"\
	"..\..\fltk\menuwindow.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\tabgroup.h"\
	"..\..\fltk\tooltip.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\TextBuffer.cxx
DEP_CPP_TEXTB=\
	"..\..\fltk\ask.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\textbuffer.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\TextDisplay.cxx
DEP_CPP_TEXTD=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\cursor.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\font.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\layout.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\textbuffer.h"\
	"..\..\fltk\textdisplay.h"\
	"..\..\fltk\utf.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\TextEditor.cxx
DEP_CPP_TEXTE=\
	"..\..\fltk\ask.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\font.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\textbuffer.h"\
	"..\..\fltk\textdisplay.h"\
	"..\..\fltk\texteditor.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\ThumbWheel.cxx
DEP_CPP_THUMB=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\thumbwheel.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\TiledGroup.cxx
DEP_CPP_TILED=\
	"..\..\fltk\color.h"\
	"..\..\fltk\cursor.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\layout.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\tiledgroup.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\TiledImage.cxx
DEP_CPP_TILEDI=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\tiledimage.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Tooltip.cxx
DEP_CPP_TOOLT=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\menuwindow.h"\
	"..\..\fltk\Monitor.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\run.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\tooltip.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\UpBox.cxx
DEP_CPP_UPBOX=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\utf.c
DEP_CPP_UTF_C=\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\utf.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Valuator.cxx
DEP_CPP_VALUA=\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\ValueInput.cxx
DEP_CPP_VALUE=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\floatinput.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\numericinput.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\valueinput.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\ValueOutput.cxx
DEP_CPP_VALUEO=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\run.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\valueoutput.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\ValueSlider.cxx
DEP_CPP_VALUES=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\floatinput.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\input.h"\
	"..\..\fltk\math.h"\
	"..\..\fltk\numericinput.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\valueslider.h"\
	"..\..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\vsnprintf.c
DEP_CPP_VSNPR=\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\string.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Widget.cxx
DEP_CPP_WIDGE=\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\layout.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\run.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Widget_draw.cxx
DEP_CPP_WIDGET=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\labeltype.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Window.cxx
DEP_CPP_WINDO=\
	"..\..\fltk\color.h"\
	"..\..\fltk\damage.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\layout.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\run.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Window_fullscreen.cxx
DEP_CPP_WINDOW=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\Monitor.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Window_hotspot.cxx
DEP_CPP_WINDOW_=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\Monitor.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\Window_iconize.cxx
DEP_CPP_WINDOW_I=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\window.h"\
	"..\..\fltk\x.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\win32\WinMain.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\xbmImage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\xpmImage.cxx
DEP_CPP_XPMIM=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\xpmimage.h"\
	
# End Source File
# End Target
# End Project
