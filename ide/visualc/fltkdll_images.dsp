# Microsoft Developer Studio Project File - Name="fltkdll_images" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fltkdll_images - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll_images.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltkdll_images.mak" CFG="fltkdll_images - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltkdll_images - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fltkdll_images - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Output_Dir "..\..\lib\"
# PROP Intermediate_Dir "fltkdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Os /Ob2 /I "." /I "../.." /I "../../fltk/compat" /I "../visualc" /I "../../images/zlib" /D "NDEBUG" /D "FL_SHARED" /D "FL_IMAGES_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /c
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
# ADD LINK32 fltk2dll.lib fltk2_z.lib fltk2_png.lib fltk2_jpeg.lib opengl32.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib msimg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdll_images.pdb" /machine:I386 /nodefaultlib:"libc" /out:"..\..\lib\fltk2dll_images.dll" /libpath:"../../lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fltkdll_images - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fltkdll_images1"
# PROP BASE Intermediate_Dir "fltkdll_images1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\"
# PROP Intermediate_Dir "fltkdlld"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /I "../../images/zlib" /I "../visualc" /I "." /I "../.." /I "../../fltk/compat" /D "_DEBUG" /D "FL_SHARED" /D "FL_IMAGES_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /Fr /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib msimg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 fltk2dlld.lib fltk2_zd.lib fltk2_pngd.lib fltk2_jpegd.lib opengl32.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib msimg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /pdb:"fltkdll_imagesd.pdb" /debug /machine:I386 /nodefaultlib:"libcd" /out:"..\..\lib\fltk2dll_imagesd.dll" /pdbtype:sept /libpath:"../../lib"
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "fltkdll_images - Win32 Release"
# Name "fltkdll_images - Win32 Debug"
# Begin Source File

SOURCE=..\..\images\FileIcon2.cxx
DEP_CPP_FILEI=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fileicon.h"\
	"..\..\fltk\filename.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\symbol.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\images\Fl_Guess_Image.cxx
DEP_CPP_FL_GU=\
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
	"..\..\fltk\xbmimage.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\images\fl_jpeg.cxx
DEP_CPP_FL_JP=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\symbol.h"\
	"..\..\images\libjpeg\jconfig.h"\
	"..\..\images\libjpeg\jerror.h"\
	"..\..\images\libjpeg\jmorecfg.h"\
	"..\..\images\libjpeg\jpegint.h"\
	"..\..\images\libjpeg\jpeglib.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\images\fl_png.cxx
DEP_CPP_FL_PN=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
	"..\..\fltk\error.h"\
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
	"..\..\images\libpng\png.h"\
	"..\..\images\libpng\pngconf.h"\
	"..\..\images\zlib\zconf.h"\
	"..\..\images\zlib\zlib.h"\
	".\config.h"\
	
NODEP_CPP_FL_PN=\
	"..\..\images\libpng\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\images\HelpDialog.cxx
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
	"..\..\fltk\input.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\scrollbar.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\slider.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\style.h"\
	"..\..\fltk\symbol.h"\
	"..\..\fltk\valuator.h"\
	"..\..\fltk\widget.h"\
	"..\..\fltk\window.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\images\images_core.cxx
DEP_CPP_IMAGE=\
	"..\..\fltk\color.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\images\pnmImage.cxx
DEP_CPP_PNMIM=\
	"..\..\fltk\color.h"\
	"..\..\fltk\error.h"\
	"..\..\fltk\fl_api.h"\
	"..\..\fltk\flags.h"\
	"..\..\fltk\image.h"\
	"..\..\fltk\PixelType.h"\
	"..\..\fltk\pnmImage.h"\
	"..\..\fltk\rectangle.h"\
	"..\..\fltk\run.h"\
	"..\..\fltk\sharedimage.h"\
	"..\..\fltk\string.h"\
	"..\..\fltk\symbol.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\images\xpmFileImage.cxx
DEP_CPP_XPMFI=\
	"..\..\fltk\color.h"\
	"..\..\fltk\draw.h"\
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
	"..\..\fltk\xpmimage.h"\
	".\config.h"\
	
# End Source File
# End Target
# End Project
