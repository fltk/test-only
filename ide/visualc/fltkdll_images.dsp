# Microsoft Developer Studio Project File - Name="fltkdll_images" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fltkdll_images - Win32 Debug MinSize
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll_images.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll_images.mak" CFG="fltkdll_images - Win32 Debug MinSize"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltkdll_images - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltkdll_images - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltkdll_images - Win32 Debug MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltkdll_images - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
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
# PROP Output_Dir "../../lib"
# PROP Intermediate_Dir "fltkdll_images"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /GR /GX /Os /Ob2 /I "." /I "..\.." /I "../visualc" /I "../../images/zlib" /D "NDEBUG" /D "FL_DLL" /D "FL_SHARED" /D "FL_IMAGES_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 fltkdll.lib opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib fltk_z.lib fltk_png.lib fltk_jpeg.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdll_images.pdb" /machine:I386 /libpath:"../../lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltkdll_images - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fltkdll_images1"
# PROP BASE Intermediate_Dir "fltkdll_images1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../lib"
# PROP Intermediate_Dir "fltkdll_imagesd"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /GR /GX /ZI /Od /I "../visualc" /I "." /I "..\.." /I "../../images/zlib" /D "_DEBUG" /D "FL_DLL" /D "FL_SHARED" /D "FL_IMAGES_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 fltkdlld.lib opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib fltk_zd.lib fltk_pngd.lib fltk_jpegd.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:no /pdb:"fltkdll_imagesd.pdb" /debug /machine:I386 /out:"../../lib/fltkdll_imagesd.dll" /pdbtype:sept /libpath:"../../lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltkdll_images - Win32 Debug MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fltkdll_images___Win32_Debug_MinSize"
# PROP BASE Intermediate_Dir "fltkdll_images___Win32_Debug_MinSize"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "fltkdll_images___Win32_Debug_MinSize"
# PROP Intermediate_Dir "fltkdll_images___Win32_Debug_MinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /GX /ZI /Od /I "." /I "..\.." /I "../visualc" /D "_DEBUG" /D "FL_DLL" /D "FL_SHARED" /D "FL_IMAGES_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD CPP /nologo /MDd /GR /GX /ZI /Od /I "../visualc" /I "." /I "..\.." /I "../../images/zlib" /D "FL_DLL" /D "FL_SHARED" /D "FL_IMAGES_LIBRARY" /D "_DEBUG" /D "_MSC_DLL" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 fltkdlld.lib opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:no /pdb:"fltkdll_imagesd.pdb" /debug /machine:I386 /out:"../../lib/fltkdll_imagesd.dll" /pdbtype:sept /libpath:"../../lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 fltkdlld.lib opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib fltk_zd.lib fltk_pngd.lib fltk_jpegd.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:no /pdb:"fltkdll_imagesd.pdb" /debug /machine:I386 /out:"../../lib/fltkdll_imagesd.dll" /implib:"../../lib/fltkdll_imagesd.lib" /pdbtype:sept /libpath:"../../lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltkdll_images - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "fltkdll_images___Win32_Release_MinSize"
# PROP BASE Intermediate_Dir "fltkdll_images___Win32_Release_MinSize"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "fltkdll_images___Win32_Release_MinSize"
# PROP Intermediate_Dir "fltkdll_images___Win32_Release_MinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Os /Ob2 /I "." /I "..\.." /D "NDEBUG" /D "FL_DLL" /D "FL_SHARED" /D "FL_IMAGES_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# ADD CPP /nologo /MD /GR /GX /Os /Ob2 /I "." /I "..\.." /I "../visualc" /I "../../images/zlib" /D "NDEBUG" /D "FL_DLL" /D "FL_SHARED" /D "FL_IMAGES_LIBRARY" /D "_MSC_DLL" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /D "USE_CONF" /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 fltkdll.lib opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdll_images.pdb" /machine:I386 /libpath:"../../lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 fltkdll.lib opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib fltk_z.lib fltk_png.lib fltk_jpeg.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdll_images.pdb" /machine:I386 /out:"../../lib/fltkdll_images.dll" /implib:"../../lib/fltkdll_images.lib" /libpath:"../../lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "fltkdll_images - Win32 Release"
# Name "fltkdll_images - Win32 Debug"
# Name "fltkdll_images - Win32 Debug MinSize"
# Name "fltkdll_images - Win32 Release MinSize"
# Begin Source File

SOURCE=..\..\images\Fl_Guess_Image.cxx
DEP_CPP_FL_GU=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\fltk\xbmimage.h"\
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
	
# SUBTRACT CPP /D "FL_DLL"
# End Source File
# Begin Source File

SOURCE=..\..\images\fl_jpeg.cxx
DEP_CPP_FL_JP=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\symbol.h"\
	"..\..\images\libjpeg\jconfig.h"\
	"..\..\images\libjpeg\jerror.h"\
	"..\..\images\libjpeg\jmorecfg.h"\
	"..\..\images\libjpeg\jpegint.h"\
	"..\..\images\libjpeg\jpeglib.h"\
	".\config.h"\
	{$(INCLUDE)}"jconfig.h"\
	{$(INCLUDE)}"jerror.h"\
	{$(INCLUDE)}"jmorecfg.h"\
	{$(INCLUDE)}"jpegint.h"\
	{$(INCLUDE)}"jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\images\fl_png.cxx
DEP_CPP_FL_PN=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\images\libpng\png.h"\
	"..\..\images\libpng\pngconf.h"\
	"..\..\images\zlib\zconf.h"\
	"..\..\images\zlib\zlib.h"\
	".\config.h"\
	{$(INCLUDE)}"png.h"\
	{$(INCLUDE)}"pngconf.h"\
	{$(INCLUDE)}"X11\extensions\XI.h"\
	{$(INCLUDE)}"X11\extensions\XInput.h"\
	{$(INCLUDE)}"X11\X.h"\
	{$(INCLUDE)}"X11\Xatom.h"\
	{$(INCLUDE)}"X11\Xfuncproto.h"\
	{$(INCLUDE)}"X11\Xlib.h"\
	{$(INCLUDE)}"X11\Xmd.h"\
	{$(INCLUDE)}"X11\Xosdefs.h"\
	{$(INCLUDE)}"X11\Xutil.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
NODEP_CPP_FL_PN=\
	"..\..\..\INCLUDE\alloc.h"\
	"..\..\images\libpng\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\HelpDialog.cxx
DEP_CPP_HELPD=\
	"..\..\fltk\ask.h"\
	"..\..\fltk\button.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\DoubleBufferWindow.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\HelpDialog.h"\
	"..\..\fltk\HelpView.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	".\fltk\string.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\HelpView.cxx
DEP_CPP_HELPV=\
	"..\..\fltk\box.h"\
	"..\..\fltk\color.h"\
	"..\..\fltk\cursor.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\font.h"\
	"..\..\fltk\group.h"\
	"..\..\fltk\HelpView.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\xpmimage.h"\
	".\fltk\string.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\src\xpmFileImage.cxx
DEP_CPP_XPMFI=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\events.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\mac.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\win32.h"\
	"..\..\fltk\x.h"\
	"..\..\fltk\xpmimage.h"\
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
# End Target
# End Project
