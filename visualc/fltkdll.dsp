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
# ADD CPP /nologo /MT /W1 /GR /GX /Os /Ob2 /I "." /I ".." /D "NDEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
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
# ADD CPP /nologo /MD /W1 /GR /GX /Os /Ob2 /I "." /I ".." /D "NDEBUG" /D "FL_SHARED" /D "FL_DLL" /D "FL_LIBRARY" /D "FL_GL_LIBRARY" /D "FL_GLUT_LIBRARY" /D "_MSC_DLL" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
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

SOURCE=..\src\array.h
# End Source File
# Begin Source File

SOURCE=..\src\CycleButton.cxx
DEP_CPP_CYCLE=\
	"..\fltk\box.h"\
	"..\fltk\button.h"\
	"..\fltk\color.h"\
	"..\fltk\cyclebutton.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\item.h"\
	"..\fltk\menu.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fastarrow.h
# End Source File
# Begin Source File

SOURCE=..\src\filename_absolute.cxx
DEP_CPP_FILEN=\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_expand.cxx
DEP_CPP_FILENA=\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_ext.cxx
DEP_CPP_FILENAM=\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_isdir.cxx
DEP_CPP_FILENAME=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\utf.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_list.cxx
DEP_CPP_FILENAME_=\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\utf.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_match.cxx
DEP_CPP_FILENAME_M=\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\filename_setext.cxx
DEP_CPP_FILENAME_S=\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl.cxx
DEP_CPP_FL_CX=\
	"..\fltk\browser.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\error.h"\
	"..\fltk\events.h"\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\fl_version.h"\
	"..\fltk\flags.h"\
	"..\fltk\font.h"\
	"..\fltk\group.h"\
	"..\fltk\layout.h"\
	"..\fltk\mac.h"\
	"..\fltk\menu.h"\
	"..\fltk\Monitor.h"\
	"..\fltk\run.h"\
	"..\fltk\scrollbar.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\tooltip.h"\
	"..\fltk\utf.h"\
	"..\fltk\valuator.h"\
	"..\fltk\visual.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	"..\src\Fl_mac.cxx"\
	"..\src\fl_win32.cxx"\
	"..\src\Fl_x.cxx"\
	"..\src\xutf8.cxx"\
	".\config.h"\
	{$(INCLUDE)}"iconv.h"\
	{$(INCLUDE)}"X11\extensions\Xdbe.h"\
	{$(INCLUDE)}"X11\extensions\Xdbeproto.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_abort.cxx
DEP_CPP_FL_AB=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\error.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\utf.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\fltk\string.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_add_idle.cxx
DEP_CPP_FL_AD=\
	"..\fltk\fl_api.h"\
	"..\fltk\run.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Adjuster.cxx
DEP_CPP_FL_ADJ=\
	"..\fltk\adjuster.h"\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\image.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	"..\fltk\xbmimage.h"\
	"..\src\fastarrow.h"\
	"..\src\mediumarrow.h"\
	"..\src\slowarrow.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Align_Group.cxx
DEP_CPP_FL_AL=\
	"..\fltk\aligngroup.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_arc.cxx
DEP_CPP_FL_AR=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\math.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_arci.cxx
DEP_CPP_FL_ARC=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\math.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_arg.cxx
DEP_CPP_FL_ARG=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\error.h"\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\Monitor.h"\
	"..\fltk\run.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_ask.cxx
DEP_CPP_FL_AS=\
	"..\fltk\ask.h"\
	"..\fltk\button.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\input.h"\
	"..\fltk\returnbutton.h"\
	"..\fltk\secretinput.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	".\fltk\string.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Bar.cxx
DEP_CPP_FL_BA=\
	"..\fltk\bargroup.h"\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Bitmap.cxx
DEP_CPP_FL_BI=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\image.h"\
	"..\fltk\mac.h"\
	"..\fltk\symbol.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\fltk\xbmimage.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_bmp.cxx
DEP_CPP_FL_BM=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\error.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\image.h"\
	"..\fltk\mac.h"\
	"..\fltk\sharedimage.h"\
	"..\fltk\symbol.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Box.cxx
DEP_CPP_FL_BO=\
	"..\fltk\color.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\invisiblebox.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Boxtype.cxx
DEP_CPP_FL_BOX=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\math.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	".\fltk\string.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Browser.cxx
DEP_CPP_FL_BR=\
	"..\fltk\box.h"\
	"..\fltk\browser.h"\
	"..\fltk\button.h"\
	"..\fltk\color.h"\
	"..\fltk\cursor.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\error.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\font.h"\
	"..\fltk\group.h"\
	"..\fltk\item.h"\
	"..\fltk\layout.h"\
	"..\fltk\menu.h"\
	"..\fltk\scrollbar.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Button.cxx
DEP_CPP_FL_BU=\
	"..\fltk\box.h"\
	"..\fltk\button.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_call_main.c
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Check_Button.cxx
DEP_CPP_FL_CH=\
	"..\fltk\box.h"\
	"..\fltk\button.h"\
	"..\fltk\checkbutton.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Choice.cxx
DEP_CPP_FL_CHO=\
	"..\fltk\box.h"\
	"..\fltk\choice.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\item.h"\
	"..\fltk\menu.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_clip.cxx
DEP_CPP_FL_CL=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Clock.cxx
DEP_CPP_FL_CLO=\
	"..\fltk\clock.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_cmap.h
# End Source File
# Begin Source File

SOURCE=..\src\fl_color.cxx
DEP_CPP_FL_CO=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fl_cmap.h"\
	"..\src\fl_color_mac.cxx"\
	"..\src\fl_color_win32.cxx"\
	"..\src\fl_color_x.cxx"\
	"..\src\XColorMap.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Color_Chooser.cxx
DEP_CPP_FL_COL=\
	"..\fltk\ask.h"\
	"..\fltk\box.h"\
	"..\fltk\button.h"\
	"..\fltk\choice.h"\
	"..\fltk\color.h"\
	"..\fltk\colorchooser.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\floatinput.h"\
	"..\fltk\group.h"\
	"..\fltk\input.h"\
	"..\fltk\item.h"\
	"..\fltk\math.h"\
	"..\fltk\menu.h"\
	"..\fltk\numericinput.h"\
	"..\fltk\returnbutton.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\valueinput.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_compose.cxx
DEP_CPP_FL_COM=\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_cursor.cxx
DEP_CPP_FL_CU=\
	"..\fltk\color.h"\
	"..\fltk\cursor.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_curve.cxx
DEP_CPP_FL_CUR=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\math.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Dial.cxx
DEP_CPP_FL_DI=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\dial.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\math.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_diamond_box.cxx
DEP_CPP_FL_DIA=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_display.cxx
DEP_CPP_FL_DIS=\
	"..\fltk\fl_api.h"\
	"..\fltk\run.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_dnd.cxx
DEP_CPP_FL_DN=\
	"..\fltk\color.h"\
	"..\fltk\cursor.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\run.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	"..\src\fl_dnd_mac.cxx"\
	"..\src\fl_dnd_win32.cxx"\
	"..\src\fl_dnd_x.cxx"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw.cxx
DEP_CPP_FL_DR=\
	"..\fltk\ask.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\font.h"\
	"..\fltk\math.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_image.cxx
DEP_CPP_FL_DRA=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\error.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fl_draw_image_mac.cxx"\
	"..\src\fl_draw_image_win32.cxx"\
	"..\src\fl_draw_image_x.cxx"\
	"..\src\XColorMap.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_draw_pixmap.cxx
DEP_CPP_FL_DRAW=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_engraved_label.cxx
DEP_CPP_FL_EN=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\labeltype.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_file_chooser.cxx
DEP_CPP_FL_FI=\
	"..\fltk\browser.h"\
	"..\fltk\button.h"\
	"..\fltk\choice.h"\
	"..\fltk\color.h"\
	"..\fltk\file_chooser.h"\
	"..\fltk\filebrowser.h"\
	"..\fltk\filechooser.h"\
	"..\fltk\fileicon.h"\
	"..\fltk\fileinput.h"\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\input.h"\
	"..\fltk\menu.h"\
	"..\fltk\returnbutton.h"\
	"..\fltk\scrollbar.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileBrowser.cxx
DEP_CPP_FL_FIL=\
	"..\fltk\browser.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\filebrowser.h"\
	"..\fltk\fileicon.h"\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\menu.h"\
	"..\fltk\scrollbar.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\utf.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	".\config.h"\
	".\fltk\string.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileChooser.cxx
DEP_CPP_FL_FILE=\
	"..\fltk\browser.h"\
	"..\fltk\button.h"\
	"..\fltk\choice.h"\
	"..\fltk\color.h"\
	"..\fltk\filebrowser.h"\
	"..\fltk\filechooser.h"\
	"..\fltk\fileicon.h"\
	"..\fltk\fileinput.h"\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\image.h"\
	"..\fltk\input.h"\
	"..\fltk\menu.h"\
	"..\fltk\returnbutton.h"\
	"..\fltk\scrollbar.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	"..\fltk\xbmimage.h"\
	".\fltk\string.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileChooser2.cxx
DEP_CPP_FL_FILEC=\
	"..\fltk\ask.h"\
	"..\fltk\browser.h"\
	"..\fltk\button.h"\
	"..\fltk\choice.h"\
	"..\fltk\color.h"\
	"..\fltk\events.h"\
	"..\fltk\filebrowser.h"\
	"..\fltk\filechooser.h"\
	"..\fltk\fileicon.h"\
	"..\fltk\fileinput.h"\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\input.h"\
	"..\fltk\menu.h"\
	"..\fltk\returnbutton.h"\
	"..\fltk\scrollbar.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\utf.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	".\config.h"\
	".\fltk\string.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileIcon.cxx
DEP_CPP_FL_FILEI=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fileicon.h"\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileIcon2.cxx
DEP_CPP_FL_FILEIC=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fileicon.h"\
	"..\fltk\filename.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	".\config.h"\
	".\fltk\string.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_FileInput.cxx
DEP_CPP_FL_FILEIN=\
	"..\fltk\color.h"\
	"..\fltk\events.h"\
	"..\fltk\fileinput.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\input.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Float_Input.cxx
DEP_CPP_FL_FL=\
	"..\fltk\color.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\floatinput.h"\
	"..\fltk\input.h"\
	"..\fltk\numericinput.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_font.cxx
DEP_CPP_FL_FO=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\error.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\font.h"\
	"..\fltk\mac.h"\
	"..\fltk\math.h"\
	"..\fltk\style.h"\
	"..\fltk\utf.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fl_font_mac.cxx"\
	"..\src\fl_font_win32.cxx"\
	"..\src\fl_font_x.cxx"\
	"..\src\fl_font_xft.cxx"\
	"..\src\IFont.h"\
	".\config.h"\
	".\fltk\string.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_fontsize.h
# End Source File
# Begin Source File

SOURCE=..\src\Fl_get_key.cxx
DEP_CPP_FL_GE=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\Fl_get_key_mac.cxx"\
	"..\src\fl_get_key_win32.cxx"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_gif.cxx
DEP_CPP_FL_GI=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\image.h"\
	"..\fltk\mac.h"\
	"..\fltk\sharedimage.h"\
	"..\fltk\symbol.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Choice.cxx
DEP_CPP_FL_GL=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\gl.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\visual.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	"..\opengl\glchoice.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Choice.H
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Overlay.cxx
DEP_CPP_FL_GL_=\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\gl.h"\
	"..\fltk\glwindow.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	"..\opengl\glchoice.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\Fl_Gl_Window.cxx
DEP_CPP_FL_GL_W=\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\error.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\gl.h"\
	"..\fltk\glwindow.h"\
	"..\fltk\group.h"\
	"..\fltk\layout.h"\
	"..\fltk\mac.h"\
	"..\fltk\run.h"\
	"..\fltk\style.h"\
	"..\fltk\visual.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	"..\opengl\glchoice.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_glyph.cxx
DEP_CPP_FL_GLY=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Group.cxx
DEP_CPP_FL_GR=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\layout.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Highlight_Button.cxx
DEP_CPP_FL_HI=\
	"..\fltk\button.h"\
	"..\fltk\color.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\highlightbutton.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Image.cxx
DEP_CPP_FL_IM=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\image.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input.cxx
DEP_CPP_FL_IN=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\input.h"\
	"..\fltk\math.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\utf.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Input_Browser.cxx
DEP_CPP_FL_INP=\
	"..\fltk\box.h"\
	"..\fltk\browser.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\input.h"\
	"..\fltk\inputbrowser.h"\
	"..\fltk\menu.h"\
	"..\fltk\menuwindow.h"\
	"..\fltk\Monitor.h"\
	"..\fltk\scrollbar.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Item.cxx
DEP_CPP_FL_IT=\
	"..\fltk\box.h"\
	"..\fltk\button.h"\
	"..\fltk\checkbutton.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\divider.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\image.h"\
	"..\fltk\item.h"\
	"..\fltk\itemgroup.h"\
	"..\fltk\menu.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_key_name.cxx
DEP_CPP_FL_KE=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_labeltype.cxx
DEP_CPP_FL_LA=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\image.h"\
	"..\fltk\labeltype.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	".\config.h"\
	".\fltk\string.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Light_Button.cxx
DEP_CPP_FL_LI=\
	"..\fltk\box.h"\
	"..\fltk\button.h"\
	"..\fltk\checkbutton.h"\
	"..\fltk\color.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\lightbutton.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_list_fonts.cxx
DEP_CPP_FL_LIS=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\font.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\utf.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fl_list_fonts_mac.cxx"\
	"..\src\fl_list_fonts_win32.cxx"\
	"..\src\fl_list_fonts_x.cxx"\
	"..\src\IFont.h"\
	".\config.h"\
	".\fltk\string.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_load_plugin.cxx
DEP_CPP_FL_LO=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\load_plugin.h"\
	"..\fltk\mac.h"\
	"..\fltk\utf.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\dlload_osx.cxx"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_lock.cxx
DEP_CPP_FL_LOC=\
	"..\fltk\fl_api.h"\
	"..\fltk\run.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu.cxx
DEP_CPP_FL_ME=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\item.h"\
	"..\fltk\labeltype.h"\
	"..\fltk\menu.h"\
	"..\fltk\menuwindow.h"\
	"..\fltk\Monitor.h"\
	"..\fltk\run.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_.cxx
DEP_CPP_FL_MEN=\
	"..\fltk\color.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\item.h"\
	"..\fltk\menu.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_add.cxx
DEP_CPP_FL_MENU=\
	"..\fltk\color.h"\
	"..\fltk\divider.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\item.h"\
	"..\fltk\itemgroup.h"\
	"..\fltk\menu.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\src\array.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Bar.cxx
DEP_CPP_FL_MENU_=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\item.h"\
	"..\fltk\menu.h"\
	"..\fltk\menubar.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\tooltip.h"\
	"..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Button.cxx
DEP_CPP_FL_MENU_B=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\item.h"\
	"..\fltk\menu.h"\
	"..\fltk\popupmenu.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_global.cxx
DEP_CPP_FL_MENU_G=\
	"..\fltk\color.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\menu.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Item.cxx
DEP_CPP_FL_MENU_I=\
	"..\fl\fl_menu_item.h"\
	"..\fltk\color.h"\
	"..\fltk\divider.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\item.h"\
	"..\fltk\itemgroup.h"\
	"..\fltk\menu.h"\
	"..\fltk\popupmenu.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Menu_Window.cxx
DEP_CPP_FL_MENU_W=\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\menuwindow.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Multi_Image.cxx
DEP_CPP_FL_MU=\
	"..\fltk\color.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\multiimage.h"\
	"..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Numeric_Input.cxx
DEP_CPP_FL_NU=\
	"..\fltk\color.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\input.h"\
	"..\fltk\numericinput.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Output.cxx
DEP_CPP_FL_OU=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\input.h"\
	"..\fltk\output.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_oval_box.cxx
DEP_CPP_FL_OV=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay.cxx
DEP_CPP_FL_OVE=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_overlay_visual.cxx
DEP_CPP_FL_OVER=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_own_colormap.cxx
DEP_CPP_FL_OW=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\visual.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pack.cxx
DEP_CPP_FL_PA=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\layout.h"\
	"..\fltk\packedgroup.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Pixmap.cxx
DEP_CPP_FL_PI=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\image.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\fltk\xpmimage.h"\
	".\config.h"\
	".\fltk\string.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_plastic_box.cxx
DEP_CPP_FL_PL=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_ProgressBar.cxx
DEP_CPP_FL_PR=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\progressbar.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Radio_Button.cxx
DEP_CPP_FL_RA=\
	"..\fltk\box.h"\
	"..\fltk\button.h"\
	"..\fltk\checkbutton.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\radiobutton.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_rect.cxx
DEP_CPP_FL_RE=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\math.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Repeat_Button.cxx
DEP_CPP_FL_REP=\
	"..\fltk\button.h"\
	"..\fltk\color.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\repeatbutton.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Return_Button.cxx
DEP_CPP_FL_RET=\
	"..\fltk\button.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\returnbutton.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_rgb.cxx
DEP_CPP_FL_RG=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_RGB_Image.cxx
DEP_CPP_FL_RGB=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\image.h"\
	"..\fltk\mac.h"\
	"..\fltk\rgbimage.h"\
	"..\fltk\symbol.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"jconfig.h"\
	{$(INCLUDE)}"jerror.h"\
	{$(INCLUDE)}"jmorecfg.h"\
	{$(INCLUDE)}"jpegint.h"\
	{$(INCLUDE)}"jpeglib.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Roller.cxx
DEP_CPP_FL_RO=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\math.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\thumbwheel.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_round_box.cxx
DEP_CPP_FL_ROU=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_rounded_box.cxx
DEP_CPP_FL_ROUN=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scroll.cxx
DEP_CPP_FL_SC=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\layout.h"\
	"..\fltk\scrollbar.h"\
	"..\fltk\scrollgroup.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_scroll_area.cxx
DEP_CPP_FL_SCR=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Scrollbar.cxx
DEP_CPP_FL_SCRO=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\scrollbar.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_shadow_box.cxx
DEP_CPP_FL_SH=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Shaped_Window.cxx
DEP_CPP_FL_SHA=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\image.h"\
	"..\fltk\mac.h"\
	"..\fltk\shapedwindow.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	"..\fltk\xbmimage.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\shape.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Shared_Image.cxx
DEP_CPP_FL_SHAR=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\image.h"\
	"..\fltk\mac.h"\
	"..\fltk\sharedimage.h"\
	"..\fltk\symbol.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\fltk\xbmimage.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_show_colormap.cxx
DEP_CPP_FL_SHO=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\menuwindow.h"\
	"..\fltk\Monitor.h"\
	"..\fltk\show_colormap.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Slider.cxx
DEP_CPP_FL_SL=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\math.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_String_List.cxx
DEP_CPP_FL_ST=\
	"..\fltk\color.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\item.h"\
	"..\fltk\menu.h"\
	"..\fltk\stringlist.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Style.cxx
DEP_CPP_FL_STY=\
	"..\fltk\color.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\math.h"\
	"..\fltk\run.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	".\config.h"\
	".\fltk\string.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Style_Set.cxx
DEP_CPP_FL_STYL=\
	"..\fltk\color.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\style.h"\
	"..\fltk\styleset.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_symbols.cxx
DEP_CPP_FL_SY=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\error.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\math.h"\
	"..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tabs.cxx
DEP_CPP_FL_TA=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\layout.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\tabgroup.h"\
	"..\fltk\tooltip.h"\
	"..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Buffer.cxx
DEP_CPP_FL_TE=\
	"..\fltk\fl_api.h"\
	"..\fltk\textbuffer.h"\
	"..\fltk\utf.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Display.cxx
DEP_CPP_FL_TEX=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\scrollbar.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\textbuffer.h"\
	"..\fltk\textdisplay.h"\
	"..\fltk\utf.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Text_Editor.cxx
DEP_CPP_FL_TEXT=\
	"..\fltk\color.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\scrollbar.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\textbuffer.h"\
	"..\fltk\textdisplay.h"\
	"..\fltk\texteditor.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tile.cxx
DEP_CPP_FL_TI=\
	"..\fltk\color.h"\
	"..\fltk\cursor.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\layout.h"\
	"..\fltk\style.h"\
	"..\fltk\tiledgroup.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tiled_Image.cxx
DEP_CPP_FL_TIL=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\symbol.h"\
	"..\fltk\tiledimage.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Tooltip.cxx
DEP_CPP_FL_TO=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\menuwindow.h"\
	"..\fltk\Monitor.h"\
	"..\fltk\run.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\tooltip.h"\
	"..\fltk\widget.h"\
	"..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Valuator.cxx
DEP_CPP_FL_VA=\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\math.h"\
	"..\fltk\style.h"\
	"..\fltk\valuator.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Input.cxx
DEP_CPP_FL_VAL=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\floatinput.h"\
	"..\fltk\group.h"\
	"..\fltk\input.h"\
	"..\fltk\math.h"\
	"..\fltk\numericinput.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\valueinput.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Value_Slider.cxx
DEP_CPP_FL_VALU=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\slider.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\valueslider.h"\
	"..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\fl_vertex.cxx
DEP_CPP_FL_VE=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\math.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_visual.cxx
DEP_CPP_FL_VI=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\visual.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\Xdbe.h"\
	{$(INCLUDE)}"X11\extensions\Xdbeproto.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Widget.cxx
DEP_CPP_FL_WI=\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\layout.h"\
	"..\fltk\run.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window.cxx
DEP_CPP_FL_WIN=\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\layout.h"\
	"..\fltk\mac.h"\
	"..\fltk\run.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_fullscreen.cxx
DEP_CPP_FL_WIND=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\Monitor.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_hotspot.cxx
DEP_CPP_FL_WINDO=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\Monitor.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_Window_iconize.cxx
DEP_CPP_FL_WINDOW=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\Fl_XColor.H
# End Source File
# Begin Source File

SOURCE=..\src\fltk_theme.cxx
DEP_CPP_FLTK_=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\mac.h"\
	"..\fltk\Monitor.h"\
	"..\fltk\style.h"\
	"..\fltk\utf.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\x.h"\
	"..\src\fltk_theme_win32.cxx"\
	"..\src\fltk_theme_x.cxx"\
	".\config.h"\
	".\fltk\string.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\gl_draw.cxx
DEP_CPP_GL_DR=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\gl.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\utf.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	"..\opengl\glchoice.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\OpenGL\gl_start.cxx
DEP_CPP_GL_ST=\
	"..\fltk\color.h"\
	"..\fltk\draw.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\gl.h"\
	"..\fltk\group.h"\
	"..\fltk\mac.h"\
	"..\fltk\style.h"\
	"..\fltk\visual.h"\
	"..\fltk\widget.h"\
	"..\fltk\win32.h"\
	"..\fltk\window.h"\
	"..\fltk\x.h"\
	"..\opengl\glchoice.h"\
	".\config.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\IFont.h
# End Source File
# Begin Source File

SOURCE=..\src\mediumarrow.h
# End Source File
# Begin Source File

SOURCE=..\src\scandir.c
DEP_CPP_SCAND=\
	"..\fltk\fl_api.h"\
	"..\fltk\utf.h"\
	"..\src\scandir_win32.c"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\slowarrow.h
# End Source File
# Begin Source File

SOURCE=..\src\string.c
DEP_CPP_STRIN=\
	"..\fltk\fl_api.h"\
	".\fltk\string.h"\
	
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

SOURCE=..\src\utf.c
DEP_CPP_UTF_C=\
	"..\fltk\fl_api.h"\
	"..\fltk\utf.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\ValueOutput.cxx
DEP_CPP_VALUE=\
	"..\fltk\box.h"\
	"..\fltk\color.h"\
	"..\fltk\damage.h"\
	"..\fltk\draw.h"\
	"..\fltk\events.h"\
	"..\fltk\fl_api.h"\
	"..\fltk\flags.h"\
	"..\fltk\run.h"\
	"..\fltk\style.h"\
	"..\fltk\symbol.h"\
	"..\fltk\valuator.h"\
	"..\fltk\valueoutput.h"\
	"..\fltk\widget.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\vsnprintf.c
DEP_CPP_VSNPR=\
	"..\fltk\fl_api.h"\
	".\config.h"\
	".\fltk\string.h"\
	
# End Source File
# End Target
# End Project
