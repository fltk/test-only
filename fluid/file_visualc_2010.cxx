//
// "$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $"
//
// Fluid file routines for the Fast Light Tool Kit (FLTK).
//
// You may find the basic read_* and write_* routines to
// be useful for other programs.  I have used them many times.
// They are somewhat similar to tcl, using matching { and }
// to quote strings.
//
// Copyright 1998-2010 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Fl_Type.h"
#include "../fltk3/filename.h"


static const char *VC2010_Workspace = "VC2010_Workspace";
static const char *VC2010_Project = "VC2010_Project";

extern const char *filename;

// ------------ file conversion ------------------------------------------------

const char *DOS_path(const char *filename) {
  static char buf[2048];
  char *c;
  strcpy(buf, filename);
  for (c=buf;;c++) {
    switch (*c) {
      case '/': *c = '\\'; break;
      case 0: return buf;
    }
  }
  return buf;
}

// ------------ VisualC 2010 ---------------------------------------------------


static int write_sln_configuration(FILE *out, Fl_Workspace_Type *workspace, Fl_Target_Type *tgt) {
  fprintf(out, "\t\t{%s}.Debug|Win32.ActiveCfg = Debug|Win32\r\n", tgt->get_UUID(VC2010_Project));
  fprintf(out, "\t\t{%s}.Debug|Win32.Build.0 = Debug|Win32\r\n", tgt->get_UUID(VC2010_Project));
  fprintf(out, "\t\t{%s}.Release|Win32.ActiveCfg = Release|Win32\r\n", tgt->get_UUID(VC2010_Project));
  fprintf(out, "\t\t{%s}.Release|Win32.Build.0 = Release|Win32\r\n", tgt->get_UUID(VC2010_Project));
  return 0;
}


static int write_sln_project(FILE *out, Fl_Workspace_Type *workspace, Fl_Target_Type *tgt) {
  fprintf(out, "Project(\"{%s}\") = \"%s\", \"%s.vcxproj\", \"{%s}\"\r\n", workspace->get_UUID(VC2010_Workspace), tgt->name(), tgt->name(), tgt->get_UUID(VC2010_Project));
  // dependencies can be found in the project files
  fprintf(out, "EndProject\r\n");
  return 0;
}


/*
 Write a VisualC 2010 Solution file.
 */
static int write_sln_file(FILE *out, Fl_Workspace_Type *workspace) {
  
  const char *workspace_id = workspace->get_UUID(VC2010_Workspace);
  
  fprintf(out, "Microsoft Visual Studio Solution File, Format Version 11.00\r\n");
  fprintf(out, "# Visual C++ Express 2010\r\n");
  
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"demo\", \"demo.vcxproj\", \"{09427220-8C9C-498A-8D50-1638D3FB87E5}\"\r\n");
  fprintf(out, "\tProjectSection(ProjectDependencies) = postProject\r\n");
  fprintf(out, "\t\t{BA275A4C-FD92-4AC2-8996-1A32E9BC4E3A} = {BA275A4C-FD92-4AC2-8996-1A32E9BC4E3A}\r\n");
  fprintf(out, "\t\t{9CF889F1-AEFE-43BA-BA18-B2B09EF8ADEE} = {9CF889F1-AEFE-43BA-BA18-B2B09EF8ADEE}\r\n");
  fprintf(out, "\tEndProjectSection\r\n");
  fprintf(out, "EndProject\r\n");
  write_sln_project(out, workspace, Fl_Target_Type::find("adjuster"));
  write_sln_project(out, workspace, Fl_Target_Type::find("arc"));
  //fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"arc\", \"arc.vcxproj\", \"{39ADA7A1-A2C1-4F0B-8B92-04E335570C27}\"\r\n");
  //fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"ask\", \"ask.vcxproj\", \"{DB6BAA16-C589-448F-9AB5-5969016549EB}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"bitmap\", \"bitmap.vcxproj\", \"{65B5BB3F-EE54-4604-B49B-7676AED83AAB}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"boxtype\", \"boxtype.vcxproj\", \"{E212D4B4-63D9-4D8D-84DB-45C8BC191462}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"browser\", \"browser.vcxproj\", \"{2352F92C-8663-4637-B919-D76F0A9EC05E}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"button\", \"button.vcxproj\", \"{0363AC35-F325-4C24-9DB9-E83337D55787}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"buttons\", \"buttons.vcxproj\", \"{67CE1C24-B0C0-4CD6-8D0F-68513C2398B9}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"checkers\", \"checkers.vcxproj\", \"{92EEF887-8305-4D00-86B7-D3F98CC7AD87}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"clock\", \"clock.vcxproj\", \"{122160ED-446E-4B15-8CB4-A2058DC7269A}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"colbrowser\", \"colbrowser.vcxproj\", \"{D0BC7525-99E7-4247-B860-1B3CF2FF8165}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"color_chooser\", \"color_chooser.vcxproj\", \"{BDACB9FC-6769-4A2D-A636-36CC25E3AC75}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"cube\", \"cube.vcxproj\", \"{0973844B-3E5F-4C38-95FF-E8935243D287}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"CubeView\", \"CubeView.vcxproj\", \"{E30BB28D-DFA0-479C-B670-8CD872224B38}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"cursor\", \"cursor.vcxproj\", \"{0D1DB0EE-E997-4550-85D4-BDE209B75AD6}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"curve\", \"curve.vcxproj\", \"{27D45BA6-A403-4A71-B6D6-57DC0CEDCE15}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"doublebuffer\", \"doublebuffer.vcxproj\", \"{EDDBF169-77C1-496E-9EFE-E500107E6E97}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"editor\", \"editor.vcxproj\", \"{0BDAEBF1-7A8D-434B-A543-4663464E972D}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fast_slow\", \"fast_slow.vcxproj\", \"{2D29D6C9-B4A6-444C-A311-106A9C8A986F}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"file_chooser\", \"file_chooser.vcxproj\", \"{6927F349-29DF-4D60-BC8F-5A3F5E133735}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fltk\", \"fltk.lib.vcxproj\", \"{E070AAFC-9D03-41A3-BC7D-30887EA0D50F}\"\r\n");
  fprintf(out, "\tProjectSection(ProjectDependencies) = postProject\r\n");
  fprintf(out, "\t\t{D640A221-F95A-40FF-AC0E-0E8B615C7681} = {D640A221-F95A-40FF-AC0E-0E8B615C7681}\r\n");
  fprintf(out, "\t\t{08B82852-90B3-4767-A5D2-F0A4FCCB2377} = {08B82852-90B3-4767-A5D2-F0A4FCCB2377}\r\n");
  fprintf(out, "\tEndProjectSection\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fltkdll\", \"fltkdll.vcxproj\", \"{F0B8F4BD-955D-43CB-980C-805364D04A25}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fltkgl\", \"fltkgl.vcxproj\", \"{F7974A9C-C255-4385-96BC-E24EE0816F7C}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fltkimages\", \"fltkimages.vcxproj\", \"{6E8E1663-B88D-4454-ADF2-279666A93306}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fluid\", \"fluid.vcxproj\", \"{8AED3078-8CD8-40C9-A8FF-46080024F1EB}\"\r\n");
  fprintf(out, "\tProjectSection(ProjectDependencies) = postProject\r\n");
  fprintf(out, "\t\t{F0B8F4BD-955D-43CB-980C-805364D04A25} = {F0B8F4BD-955D-43CB-980C-805364D04A25}\r\n");
  fprintf(out, "\tEndProjectSection\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fonts\", \"fonts.vcxproj\", \"{DF2A883A-7356-4603-9CB1-E8F1E6B1549D}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fractals\", \"fractals.vcxproj\", \"{58A83386-65E6-4F22-8712-8B6B7E62913D}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fullscreen\", \"fullscreen.vcxproj\", \"{2147B9FD-7D65-4854-9770-D7B8767DB9AE}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"gl_overlay\", \"gl_overlay.vcxproj\", \"{8D8D210D-F628-48BB-9127-D8003DF22018}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"glpuzzle\", \"glpuzzle.vcxproj\", \"{BFE8CC78-6B58-4305-A096-796A3088BB26}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"hello\", \"hello.vcxproj\", \"{ADA2462A-3656-41D2-B8A8-8D942AA0F8E9}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"help\", \"help.vcxproj\", \"{9F3F86DA-3CC5-481F-8201-166933B5C8FF}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"iconize\", \"iconize.vcxproj\", \"{37C32832-DCE4-4BAA-9306-826064B30CCB}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"image\", \"image.vcxproj\", \"{3322F289-B025-4DB1-A7D1-FBA3840F8A0B}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"inactive\", \"inactive.vcxproj\", \"{B5271094-DE51-4E60-B4FA-D8F0BDB969FE}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"input\", \"input.vcxproj\", \"{1C1B5A00-5F5D-4290-A07B-8A5A4A78570A}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"keyboard\", \"keyboard.vcxproj\", \"{4DFA9F84-0457-4FE4-8008-283270610487}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"label\", \"label.vcxproj\", \"{C3E7B77C-EED2-4046-9A30-9446C6F096A0}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"line_style\", \"line_style.vcxproj\", \"{8B25CCE0-0988-4999-AE65-5A5EE31BB44D}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"mandelbrot\", \"mandelbrot.vcxproj\", \"{7BBECC0F-9381-4BF2-87CA-3FD1DA93DEAF}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"menubar\", \"menubar.vcxproj\", \"{78079B1F-152B-4E83-87CB-364586AB3AAC}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"message\", \"message.vcxproj\", \"{E1A8934E-CB8D-4F74-B78A-EC7952BC4CBE}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"minimum\", \"minimum.vcxproj\", \"{F658ABE4-7FE1-4404-9B7C-55E0D660F1B3}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"navigation\", \"navigation.vcxproj\", \"{AA0ED6EA-2DA3-4486-AFBD-76CC9D206A1E}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"output\", \"output.vcxproj\", \"{1CA77C94-5785-4B38-B91A-1EAFBD885BC2}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"overlay\", \"overlay.vcxproj\", \"{3B0BF144-5D75-4D4A-BF75-7347186C808F}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"pack\", \"pack.vcxproj\", \"{CEB7B88B-3AA5-4DD6-818F-CF19E12BF1E1}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"pixmap\", \"pixmap.vcxproj\", \"{1CD667DD-E7AE-4F36-A1F7-F41FB59786BE}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"pixmap_browser\", \"pixmap_browser.vcxproj\", \"{289C605C-94CC-437F-836B-2FE41528EEEC}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"preferences\", \"preferences.vcxproj\", \"{D04E36D7-1C68-43E6-BE30-A7793CE3C799}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"radio\", \"radio.vcxproj\", \"{AD5ACED4-F6EE-4AC0-B502-23CFD10EF603}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"resize\", \"resize.vcxproj\", \"{46771042-C840-4314-BA3C-C5BC2FBD7CBB}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"resizebox\", \"resizebox.vcxproj\", \"{2E03AF33-56A1-4366-B016-184CD96F3B56}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"scroll\", \"scroll.vcxproj\", \"{97541B63-87AA-4ACE-BBF5-175F8D1389EC}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"shape\", \"shape.vcxproj\", \"{45797F58-624B-4FA0-A7A3-5956114D8215}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"subwindow\", \"subwindow.vcxproj\", \"{7A2467D2-B03A-4964-A289-EF233EB39F69}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"sudoku\", \"sudoku.vcxproj\", \"{AE750416-51BA-485C-BD76-EF11B4536EE8}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"symbols\", \"symbols.vcxproj\", \"{3E822491-B134-43B0-A40E-9348FFAB527F}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"tabs\", \"tabs.vcxproj\", \"{AF932F77-1804-4DA4-A6DC-950795D6AFC1}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"threads\", \"threads.vcxproj\", \"{45236F71-7031-40BC-ADCD-0535A64C04B0}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"tile\", \"tile.vcxproj\", \"{28D24031-1A17-497A-B926-59CF78076DED}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"tiled_image\", \"tiled_image.vcxproj\", \"{23F17042-704E-4198-8017-A8584E884CF7}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"valuators\", \"valuators.vcxproj\", \"{CEE12ACB-20E4-45E4-AD5E-D15D7CFF68C3}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fltkjpeg\", \"jpeg.vcxproj\", \"{08B82852-90B3-4767-A5D2-F0A4FCCB2377}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fltkzlib\", \"zlib.vcxproj\", \"{E1D9CE3F-400D-40E8-AD0D-61C29B1847FF}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"fltkpng\", \"libpng.vcxproj\", \"{D640A221-F95A-40FF-AC0E-0E8B615C7681}\"\r\n");
  fprintf(out, "\tProjectSection(ProjectDependencies) = postProject\r\n");
  fprintf(out, "\t\t{E1D9CE3F-400D-40E8-AD0D-61C29B1847FF} = {E1D9CE3F-400D-40E8-AD0D-61C29B1847FF}\r\n");
  fprintf(out, "\tEndProjectSection\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"input_choice\", \"input_choice.vcxproj\", \"{50651D04-C8F7-4C2B-B412-0848573BDF88}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"utf8\", \"utf8.vcxproj\", \"{ADA2462A-3656-41D2-B7A8-8D942AA0F8F9}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"cairo_test\", \"cairo_test.vcxproj\", \"{CF98E92E-C789-47E6-A2DA-398EB7991A2B}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"table\", \"table.vcxproj\", \"{33331FB8-2AF9-4D15-BEE3-5F69658A6F1F}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"tree\", \"tree.vcxproj\", \"{2C3FB329-620B-41A0-8845-A063C9EA8807}\"\r\n");
  fprintf(out, "\tProjectSection(ProjectDependencies) = postProject\r\n");
  fprintf(out, "\t\t{8AED3078-8CD8-40C9-A8FF-46080024F1EB} = {8AED3078-8CD8-40C9-A8FF-46080024F1EB}\r\n");
  fprintf(out, "\tEndProjectSection\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"blocks\", \"blocks.vcxproj\", \"{2D92C98B-F186-4BF0-A9D4-51D42C93F6E8}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"rotated_text\", \"rotated_text.vcxproj\", \"{E88CCAF5-5DB9-4A46-8C91-97C5697F167A}\"\r\n");
  fprintf(out, "\tProjectSection(ProjectDependencies) = postProject\r\n");
  fprintf(out, "\t\t{E070AAFC-9D03-41A3-BC7D-30887EA0D50F} = {E070AAFC-9D03-41A3-BC7D-30887EA0D50F}\r\n");
  fprintf(out, "\tEndProjectSection\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"device\", \"device.vcxproj\", \"{BA275A4C-FD92-4AC2-8996-1A32E9BC4E3A}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"native-filechooser\", \"native-filechooser.vcxproj\", \"{9CF889F1-AEFE-43BA-BA18-B2B09EF8ADEE}\"\r\n");
  fprintf(out, "EndProject\r\n");
  fprintf(out, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"unittests\", \"unittests.vcxproj\", \"{86C52ED6-C710-40E6-86A2-61F757B20CF7}\"\r\n");
  fprintf(out, "EndProject\r\n");
  
  fprintf(out, "Global\r\n");
  
  fprintf(out, "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\r\n");
  fprintf(out, "\t\tDebug|Win32 = Debug|Win32\r\n");
  fprintf(out, "\t\tRelease|Win32 = Release|Win32\r\n");
  fprintf(out, "\tEndGlobalSection\r\n");
  
  fprintf(out, "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\r\n");
  fprintf(out, "\t\t{09427220-8C9C-498A-8D50-1638D3FB87E5}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{09427220-8C9C-498A-8D50-1638D3FB87E5}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{09427220-8C9C-498A-8D50-1638D3FB87E5}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{09427220-8C9C-498A-8D50-1638D3FB87E5}.Release|Win32.Build.0 = Release|Win32\r\n");
  write_sln_configuration(out, workspace, Fl_Target_Type::find("adjuster"));
  write_sln_configuration(out, workspace, Fl_Target_Type::find("arc"));
  fprintf(out, "\t\t{DB6BAA16-C589-448F-9AB5-5969016549EB}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{DB6BAA16-C589-448F-9AB5-5969016549EB}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{DB6BAA16-C589-448F-9AB5-5969016549EB}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{DB6BAA16-C589-448F-9AB5-5969016549EB}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{DB6BAA16-C589-448F-9AB5-5969016549EB}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{DB6BAA16-C589-448F-9AB5-5969016549EB}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{DB6BAA16-C589-448F-9AB5-5969016549EB}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{DB6BAA16-C589-448F-9AB5-5969016549EB}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{65B5BB3F-EE54-4604-B49B-7676AED83AAB}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{65B5BB3F-EE54-4604-B49B-7676AED83AAB}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{65B5BB3F-EE54-4604-B49B-7676AED83AAB}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{65B5BB3F-EE54-4604-B49B-7676AED83AAB}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{65B5BB3F-EE54-4604-B49B-7676AED83AAB}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{65B5BB3F-EE54-4604-B49B-7676AED83AAB}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{65B5BB3F-EE54-4604-B49B-7676AED83AAB}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{65B5BB3F-EE54-4604-B49B-7676AED83AAB}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{E212D4B4-63D9-4D8D-84DB-45C8BC191462}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E212D4B4-63D9-4D8D-84DB-45C8BC191462}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E212D4B4-63D9-4D8D-84DB-45C8BC191462}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{E212D4B4-63D9-4D8D-84DB-45C8BC191462}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{E212D4B4-63D9-4D8D-84DB-45C8BC191462}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E212D4B4-63D9-4D8D-84DB-45C8BC191462}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E212D4B4-63D9-4D8D-84DB-45C8BC191462}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{E212D4B4-63D9-4D8D-84DB-45C8BC191462}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{2352F92C-8663-4637-B919-D76F0A9EC05E}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2352F92C-8663-4637-B919-D76F0A9EC05E}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2352F92C-8663-4637-B919-D76F0A9EC05E}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{2352F92C-8663-4637-B919-D76F0A9EC05E}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{2352F92C-8663-4637-B919-D76F0A9EC05E}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2352F92C-8663-4637-B919-D76F0A9EC05E}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2352F92C-8663-4637-B919-D76F0A9EC05E}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{2352F92C-8663-4637-B919-D76F0A9EC05E}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{0363AC35-F325-4C24-9DB9-E83337D55787}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{0363AC35-F325-4C24-9DB9-E83337D55787}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{0363AC35-F325-4C24-9DB9-E83337D55787}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{0363AC35-F325-4C24-9DB9-E83337D55787}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{0363AC35-F325-4C24-9DB9-E83337D55787}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{0363AC35-F325-4C24-9DB9-E83337D55787}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{0363AC35-F325-4C24-9DB9-E83337D55787}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{0363AC35-F325-4C24-9DB9-E83337D55787}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{67CE1C24-B0C0-4CD6-8D0F-68513C2398B9}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{67CE1C24-B0C0-4CD6-8D0F-68513C2398B9}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{67CE1C24-B0C0-4CD6-8D0F-68513C2398B9}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{67CE1C24-B0C0-4CD6-8D0F-68513C2398B9}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{67CE1C24-B0C0-4CD6-8D0F-68513C2398B9}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{67CE1C24-B0C0-4CD6-8D0F-68513C2398B9}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{67CE1C24-B0C0-4CD6-8D0F-68513C2398B9}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{67CE1C24-B0C0-4CD6-8D0F-68513C2398B9}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{92EEF887-8305-4D00-86B7-D3F98CC7AD87}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{92EEF887-8305-4D00-86B7-D3F98CC7AD87}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{92EEF887-8305-4D00-86B7-D3F98CC7AD87}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{92EEF887-8305-4D00-86B7-D3F98CC7AD87}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{92EEF887-8305-4D00-86B7-D3F98CC7AD87}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{92EEF887-8305-4D00-86B7-D3F98CC7AD87}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{92EEF887-8305-4D00-86B7-D3F98CC7AD87}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{92EEF887-8305-4D00-86B7-D3F98CC7AD87}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{122160ED-446E-4B15-8CB4-A2058DC7269A}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{122160ED-446E-4B15-8CB4-A2058DC7269A}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{122160ED-446E-4B15-8CB4-A2058DC7269A}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{122160ED-446E-4B15-8CB4-A2058DC7269A}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{122160ED-446E-4B15-8CB4-A2058DC7269A}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{122160ED-446E-4B15-8CB4-A2058DC7269A}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{122160ED-446E-4B15-8CB4-A2058DC7269A}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{122160ED-446E-4B15-8CB4-A2058DC7269A}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{D0BC7525-99E7-4247-B860-1B3CF2FF8165}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{D0BC7525-99E7-4247-B860-1B3CF2FF8165}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{D0BC7525-99E7-4247-B860-1B3CF2FF8165}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{D0BC7525-99E7-4247-B860-1B3CF2FF8165}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{D0BC7525-99E7-4247-B860-1B3CF2FF8165}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{D0BC7525-99E7-4247-B860-1B3CF2FF8165}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{D0BC7525-99E7-4247-B860-1B3CF2FF8165}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{D0BC7525-99E7-4247-B860-1B3CF2FF8165}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{BDACB9FC-6769-4A2D-A636-36CC25E3AC75}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{BDACB9FC-6769-4A2D-A636-36CC25E3AC75}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{BDACB9FC-6769-4A2D-A636-36CC25E3AC75}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{BDACB9FC-6769-4A2D-A636-36CC25E3AC75}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{BDACB9FC-6769-4A2D-A636-36CC25E3AC75}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{BDACB9FC-6769-4A2D-A636-36CC25E3AC75}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{BDACB9FC-6769-4A2D-A636-36CC25E3AC75}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{BDACB9FC-6769-4A2D-A636-36CC25E3AC75}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{0973844B-3E5F-4C38-95FF-E8935243D287}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{0973844B-3E5F-4C38-95FF-E8935243D287}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{0973844B-3E5F-4C38-95FF-E8935243D287}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{0973844B-3E5F-4C38-95FF-E8935243D287}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{0973844B-3E5F-4C38-95FF-E8935243D287}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{0973844B-3E5F-4C38-95FF-E8935243D287}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{0973844B-3E5F-4C38-95FF-E8935243D287}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{0973844B-3E5F-4C38-95FF-E8935243D287}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{E30BB28D-DFA0-479C-B670-8CD872224B38}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E30BB28D-DFA0-479C-B670-8CD872224B38}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E30BB28D-DFA0-479C-B670-8CD872224B38}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{E30BB28D-DFA0-479C-B670-8CD872224B38}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{E30BB28D-DFA0-479C-B670-8CD872224B38}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E30BB28D-DFA0-479C-B670-8CD872224B38}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E30BB28D-DFA0-479C-B670-8CD872224B38}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{E30BB28D-DFA0-479C-B670-8CD872224B38}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{0D1DB0EE-E997-4550-85D4-BDE209B75AD6}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{0D1DB0EE-E997-4550-85D4-BDE209B75AD6}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{0D1DB0EE-E997-4550-85D4-BDE209B75AD6}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{0D1DB0EE-E997-4550-85D4-BDE209B75AD6}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{0D1DB0EE-E997-4550-85D4-BDE209B75AD6}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{0D1DB0EE-E997-4550-85D4-BDE209B75AD6}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{0D1DB0EE-E997-4550-85D4-BDE209B75AD6}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{0D1DB0EE-E997-4550-85D4-BDE209B75AD6}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{27D45BA6-A403-4A71-B6D6-57DC0CEDCE15}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{27D45BA6-A403-4A71-B6D6-57DC0CEDCE15}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{27D45BA6-A403-4A71-B6D6-57DC0CEDCE15}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{27D45BA6-A403-4A71-B6D6-57DC0CEDCE15}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{27D45BA6-A403-4A71-B6D6-57DC0CEDCE15}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{27D45BA6-A403-4A71-B6D6-57DC0CEDCE15}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{27D45BA6-A403-4A71-B6D6-57DC0CEDCE15}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{27D45BA6-A403-4A71-B6D6-57DC0CEDCE15}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{EDDBF169-77C1-496E-9EFE-E500107E6E97}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{EDDBF169-77C1-496E-9EFE-E500107E6E97}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{EDDBF169-77C1-496E-9EFE-E500107E6E97}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{EDDBF169-77C1-496E-9EFE-E500107E6E97}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{EDDBF169-77C1-496E-9EFE-E500107E6E97}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{EDDBF169-77C1-496E-9EFE-E500107E6E97}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{EDDBF169-77C1-496E-9EFE-E500107E6E97}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{EDDBF169-77C1-496E-9EFE-E500107E6E97}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{0BDAEBF1-7A8D-434B-A543-4663464E972D}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{0BDAEBF1-7A8D-434B-A543-4663464E972D}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{0BDAEBF1-7A8D-434B-A543-4663464E972D}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{0BDAEBF1-7A8D-434B-A543-4663464E972D}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{0BDAEBF1-7A8D-434B-A543-4663464E972D}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{0BDAEBF1-7A8D-434B-A543-4663464E972D}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{0BDAEBF1-7A8D-434B-A543-4663464E972D}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{0BDAEBF1-7A8D-434B-A543-4663464E972D}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{2D29D6C9-B4A6-444C-A311-106A9C8A986F}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2D29D6C9-B4A6-444C-A311-106A9C8A986F}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2D29D6C9-B4A6-444C-A311-106A9C8A986F}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{2D29D6C9-B4A6-444C-A311-106A9C8A986F}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{2D29D6C9-B4A6-444C-A311-106A9C8A986F}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2D29D6C9-B4A6-444C-A311-106A9C8A986F}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2D29D6C9-B4A6-444C-A311-106A9C8A986F}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{2D29D6C9-B4A6-444C-A311-106A9C8A986F}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{6927F349-29DF-4D60-BC8F-5A3F5E133735}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{6927F349-29DF-4D60-BC8F-5A3F5E133735}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{6927F349-29DF-4D60-BC8F-5A3F5E133735}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{6927F349-29DF-4D60-BC8F-5A3F5E133735}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{6927F349-29DF-4D60-BC8F-5A3F5E133735}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{6927F349-29DF-4D60-BC8F-5A3F5E133735}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{6927F349-29DF-4D60-BC8F-5A3F5E133735}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{6927F349-29DF-4D60-BC8F-5A3F5E133735}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{E070AAFC-9D03-41A3-BC7D-30887EA0D50F}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E070AAFC-9D03-41A3-BC7D-30887EA0D50F}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E070AAFC-9D03-41A3-BC7D-30887EA0D50F}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{E070AAFC-9D03-41A3-BC7D-30887EA0D50F}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{E070AAFC-9D03-41A3-BC7D-30887EA0D50F}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E070AAFC-9D03-41A3-BC7D-30887EA0D50F}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E070AAFC-9D03-41A3-BC7D-30887EA0D50F}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{E070AAFC-9D03-41A3-BC7D-30887EA0D50F}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{F0B8F4BD-955D-43CB-980C-805364D04A25}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{F0B8F4BD-955D-43CB-980C-805364D04A25}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{F0B8F4BD-955D-43CB-980C-805364D04A25}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{F0B8F4BD-955D-43CB-980C-805364D04A25}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{F0B8F4BD-955D-43CB-980C-805364D04A25}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{F0B8F4BD-955D-43CB-980C-805364D04A25}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{F0B8F4BD-955D-43CB-980C-805364D04A25}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{F0B8F4BD-955D-43CB-980C-805364D04A25}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{F7974A9C-C255-4385-96BC-E24EE0816F7C}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{F7974A9C-C255-4385-96BC-E24EE0816F7C}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{F7974A9C-C255-4385-96BC-E24EE0816F7C}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{F7974A9C-C255-4385-96BC-E24EE0816F7C}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{F7974A9C-C255-4385-96BC-E24EE0816F7C}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{F7974A9C-C255-4385-96BC-E24EE0816F7C}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{F7974A9C-C255-4385-96BC-E24EE0816F7C}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{F7974A9C-C255-4385-96BC-E24EE0816F7C}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{6E8E1663-B88D-4454-ADF2-279666A93306}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{6E8E1663-B88D-4454-ADF2-279666A93306}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{6E8E1663-B88D-4454-ADF2-279666A93306}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{6E8E1663-B88D-4454-ADF2-279666A93306}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{6E8E1663-B88D-4454-ADF2-279666A93306}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{6E8E1663-B88D-4454-ADF2-279666A93306}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{6E8E1663-B88D-4454-ADF2-279666A93306}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{6E8E1663-B88D-4454-ADF2-279666A93306}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{8AED3078-8CD8-40C9-A8FF-46080024F1EB}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{8AED3078-8CD8-40C9-A8FF-46080024F1EB}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{8AED3078-8CD8-40C9-A8FF-46080024F1EB}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{8AED3078-8CD8-40C9-A8FF-46080024F1EB}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{8AED3078-8CD8-40C9-A8FF-46080024F1EB}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{8AED3078-8CD8-40C9-A8FF-46080024F1EB}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{8AED3078-8CD8-40C9-A8FF-46080024F1EB}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{8AED3078-8CD8-40C9-A8FF-46080024F1EB}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{DF2A883A-7356-4603-9CB1-E8F1E6B1549D}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{DF2A883A-7356-4603-9CB1-E8F1E6B1549D}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{DF2A883A-7356-4603-9CB1-E8F1E6B1549D}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{DF2A883A-7356-4603-9CB1-E8F1E6B1549D}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{DF2A883A-7356-4603-9CB1-E8F1E6B1549D}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{DF2A883A-7356-4603-9CB1-E8F1E6B1549D}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{DF2A883A-7356-4603-9CB1-E8F1E6B1549D}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{DF2A883A-7356-4603-9CB1-E8F1E6B1549D}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{58A83386-65E6-4F22-8712-8B6B7E62913D}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{58A83386-65E6-4F22-8712-8B6B7E62913D}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{58A83386-65E6-4F22-8712-8B6B7E62913D}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{58A83386-65E6-4F22-8712-8B6B7E62913D}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{58A83386-65E6-4F22-8712-8B6B7E62913D}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{58A83386-65E6-4F22-8712-8B6B7E62913D}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{58A83386-65E6-4F22-8712-8B6B7E62913D}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{58A83386-65E6-4F22-8712-8B6B7E62913D}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{2147B9FD-7D65-4854-9770-D7B8767DB9AE}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2147B9FD-7D65-4854-9770-D7B8767DB9AE}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2147B9FD-7D65-4854-9770-D7B8767DB9AE}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{2147B9FD-7D65-4854-9770-D7B8767DB9AE}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{2147B9FD-7D65-4854-9770-D7B8767DB9AE}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2147B9FD-7D65-4854-9770-D7B8767DB9AE}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2147B9FD-7D65-4854-9770-D7B8767DB9AE}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{2147B9FD-7D65-4854-9770-D7B8767DB9AE}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{8D8D210D-F628-48BB-9127-D8003DF22018}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{8D8D210D-F628-48BB-9127-D8003DF22018}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{8D8D210D-F628-48BB-9127-D8003DF22018}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{8D8D210D-F628-48BB-9127-D8003DF22018}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{8D8D210D-F628-48BB-9127-D8003DF22018}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{8D8D210D-F628-48BB-9127-D8003DF22018}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{8D8D210D-F628-48BB-9127-D8003DF22018}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{8D8D210D-F628-48BB-9127-D8003DF22018}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{BFE8CC78-6B58-4305-A096-796A3088BB26}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{BFE8CC78-6B58-4305-A096-796A3088BB26}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{BFE8CC78-6B58-4305-A096-796A3088BB26}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{BFE8CC78-6B58-4305-A096-796A3088BB26}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{BFE8CC78-6B58-4305-A096-796A3088BB26}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{BFE8CC78-6B58-4305-A096-796A3088BB26}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{BFE8CC78-6B58-4305-A096-796A3088BB26}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{BFE8CC78-6B58-4305-A096-796A3088BB26}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B8A8-8D942AA0F8E9}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B8A8-8D942AA0F8E9}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B8A8-8D942AA0F8E9}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B8A8-8D942AA0F8E9}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B8A8-8D942AA0F8E9}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B8A8-8D942AA0F8E9}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B8A8-8D942AA0F8E9}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B8A8-8D942AA0F8E9}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{9F3F86DA-3CC5-481F-8201-166933B5C8FF}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{9F3F86DA-3CC5-481F-8201-166933B5C8FF}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{9F3F86DA-3CC5-481F-8201-166933B5C8FF}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{9F3F86DA-3CC5-481F-8201-166933B5C8FF}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{9F3F86DA-3CC5-481F-8201-166933B5C8FF}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{9F3F86DA-3CC5-481F-8201-166933B5C8FF}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{9F3F86DA-3CC5-481F-8201-166933B5C8FF}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{9F3F86DA-3CC5-481F-8201-166933B5C8FF}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{37C32832-DCE4-4BAA-9306-826064B30CCB}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{37C32832-DCE4-4BAA-9306-826064B30CCB}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{37C32832-DCE4-4BAA-9306-826064B30CCB}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{37C32832-DCE4-4BAA-9306-826064B30CCB}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{37C32832-DCE4-4BAA-9306-826064B30CCB}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{37C32832-DCE4-4BAA-9306-826064B30CCB}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{37C32832-DCE4-4BAA-9306-826064B30CCB}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{37C32832-DCE4-4BAA-9306-826064B30CCB}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{3322F289-B025-4DB1-A7D1-FBA3840F8A0B}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{3322F289-B025-4DB1-A7D1-FBA3840F8A0B}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{3322F289-B025-4DB1-A7D1-FBA3840F8A0B}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{3322F289-B025-4DB1-A7D1-FBA3840F8A0B}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{3322F289-B025-4DB1-A7D1-FBA3840F8A0B}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{3322F289-B025-4DB1-A7D1-FBA3840F8A0B}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{3322F289-B025-4DB1-A7D1-FBA3840F8A0B}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{3322F289-B025-4DB1-A7D1-FBA3840F8A0B}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{B5271094-DE51-4E60-B4FA-D8F0BDB969FE}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{B5271094-DE51-4E60-B4FA-D8F0BDB969FE}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{B5271094-DE51-4E60-B4FA-D8F0BDB969FE}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{B5271094-DE51-4E60-B4FA-D8F0BDB969FE}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{B5271094-DE51-4E60-B4FA-D8F0BDB969FE}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{B5271094-DE51-4E60-B4FA-D8F0BDB969FE}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{B5271094-DE51-4E60-B4FA-D8F0BDB969FE}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{B5271094-DE51-4E60-B4FA-D8F0BDB969FE}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{1C1B5A00-5F5D-4290-A07B-8A5A4A78570A}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{1C1B5A00-5F5D-4290-A07B-8A5A4A78570A}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{1C1B5A00-5F5D-4290-A07B-8A5A4A78570A}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{1C1B5A00-5F5D-4290-A07B-8A5A4A78570A}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{1C1B5A00-5F5D-4290-A07B-8A5A4A78570A}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{1C1B5A00-5F5D-4290-A07B-8A5A4A78570A}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{1C1B5A00-5F5D-4290-A07B-8A5A4A78570A}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{1C1B5A00-5F5D-4290-A07B-8A5A4A78570A}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{4DFA9F84-0457-4FE4-8008-283270610487}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{4DFA9F84-0457-4FE4-8008-283270610487}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{4DFA9F84-0457-4FE4-8008-283270610487}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{4DFA9F84-0457-4FE4-8008-283270610487}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{4DFA9F84-0457-4FE4-8008-283270610487}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{4DFA9F84-0457-4FE4-8008-283270610487}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{4DFA9F84-0457-4FE4-8008-283270610487}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{4DFA9F84-0457-4FE4-8008-283270610487}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{C3E7B77C-EED2-4046-9A30-9446C6F096A0}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{C3E7B77C-EED2-4046-9A30-9446C6F096A0}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{C3E7B77C-EED2-4046-9A30-9446C6F096A0}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{C3E7B77C-EED2-4046-9A30-9446C6F096A0}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{C3E7B77C-EED2-4046-9A30-9446C6F096A0}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{C3E7B77C-EED2-4046-9A30-9446C6F096A0}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{C3E7B77C-EED2-4046-9A30-9446C6F096A0}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{C3E7B77C-EED2-4046-9A30-9446C6F096A0}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{8B25CCE0-0988-4999-AE65-5A5EE31BB44D}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{8B25CCE0-0988-4999-AE65-5A5EE31BB44D}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{8B25CCE0-0988-4999-AE65-5A5EE31BB44D}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{8B25CCE0-0988-4999-AE65-5A5EE31BB44D}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{8B25CCE0-0988-4999-AE65-5A5EE31BB44D}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{8B25CCE0-0988-4999-AE65-5A5EE31BB44D}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{8B25CCE0-0988-4999-AE65-5A5EE31BB44D}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{8B25CCE0-0988-4999-AE65-5A5EE31BB44D}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{7BBECC0F-9381-4BF2-87CA-3FD1DA93DEAF}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{7BBECC0F-9381-4BF2-87CA-3FD1DA93DEAF}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{7BBECC0F-9381-4BF2-87CA-3FD1DA93DEAF}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{7BBECC0F-9381-4BF2-87CA-3FD1DA93DEAF}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{7BBECC0F-9381-4BF2-87CA-3FD1DA93DEAF}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{7BBECC0F-9381-4BF2-87CA-3FD1DA93DEAF}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{7BBECC0F-9381-4BF2-87CA-3FD1DA93DEAF}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{7BBECC0F-9381-4BF2-87CA-3FD1DA93DEAF}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{78079B1F-152B-4E83-87CB-364586AB3AAC}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{78079B1F-152B-4E83-87CB-364586AB3AAC}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{78079B1F-152B-4E83-87CB-364586AB3AAC}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{78079B1F-152B-4E83-87CB-364586AB3AAC}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{78079B1F-152B-4E83-87CB-364586AB3AAC}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{78079B1F-152B-4E83-87CB-364586AB3AAC}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{78079B1F-152B-4E83-87CB-364586AB3AAC}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{78079B1F-152B-4E83-87CB-364586AB3AAC}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{E1A8934E-CB8D-4F74-B78A-EC7952BC4CBE}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E1A8934E-CB8D-4F74-B78A-EC7952BC4CBE}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E1A8934E-CB8D-4F74-B78A-EC7952BC4CBE}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{E1A8934E-CB8D-4F74-B78A-EC7952BC4CBE}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{E1A8934E-CB8D-4F74-B78A-EC7952BC4CBE}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E1A8934E-CB8D-4F74-B78A-EC7952BC4CBE}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E1A8934E-CB8D-4F74-B78A-EC7952BC4CBE}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{E1A8934E-CB8D-4F74-B78A-EC7952BC4CBE}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{F658ABE4-7FE1-4404-9B7C-55E0D660F1B3}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{F658ABE4-7FE1-4404-9B7C-55E0D660F1B3}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{F658ABE4-7FE1-4404-9B7C-55E0D660F1B3}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{F658ABE4-7FE1-4404-9B7C-55E0D660F1B3}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{F658ABE4-7FE1-4404-9B7C-55E0D660F1B3}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{F658ABE4-7FE1-4404-9B7C-55E0D660F1B3}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{F658ABE4-7FE1-4404-9B7C-55E0D660F1B3}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{F658ABE4-7FE1-4404-9B7C-55E0D660F1B3}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{AA0ED6EA-2DA3-4486-AFBD-76CC9D206A1E}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{AA0ED6EA-2DA3-4486-AFBD-76CC9D206A1E}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{AA0ED6EA-2DA3-4486-AFBD-76CC9D206A1E}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{AA0ED6EA-2DA3-4486-AFBD-76CC9D206A1E}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{AA0ED6EA-2DA3-4486-AFBD-76CC9D206A1E}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{AA0ED6EA-2DA3-4486-AFBD-76CC9D206A1E}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{AA0ED6EA-2DA3-4486-AFBD-76CC9D206A1E}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{AA0ED6EA-2DA3-4486-AFBD-76CC9D206A1E}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{1CA77C94-5785-4B38-B91A-1EAFBD885BC2}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{1CA77C94-5785-4B38-B91A-1EAFBD885BC2}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{1CA77C94-5785-4B38-B91A-1EAFBD885BC2}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{1CA77C94-5785-4B38-B91A-1EAFBD885BC2}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{1CA77C94-5785-4B38-B91A-1EAFBD885BC2}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{1CA77C94-5785-4B38-B91A-1EAFBD885BC2}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{1CA77C94-5785-4B38-B91A-1EAFBD885BC2}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{1CA77C94-5785-4B38-B91A-1EAFBD885BC2}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{3B0BF144-5D75-4D4A-BF75-7347186C808F}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{3B0BF144-5D75-4D4A-BF75-7347186C808F}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{3B0BF144-5D75-4D4A-BF75-7347186C808F}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{3B0BF144-5D75-4D4A-BF75-7347186C808F}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{3B0BF144-5D75-4D4A-BF75-7347186C808F}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{3B0BF144-5D75-4D4A-BF75-7347186C808F}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{3B0BF144-5D75-4D4A-BF75-7347186C808F}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{3B0BF144-5D75-4D4A-BF75-7347186C808F}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{CEB7B88B-3AA5-4DD6-818F-CF19E12BF1E1}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{CEB7B88B-3AA5-4DD6-818F-CF19E12BF1E1}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{CEB7B88B-3AA5-4DD6-818F-CF19E12BF1E1}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{CEB7B88B-3AA5-4DD6-818F-CF19E12BF1E1}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{CEB7B88B-3AA5-4DD6-818F-CF19E12BF1E1}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{CEB7B88B-3AA5-4DD6-818F-CF19E12BF1E1}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{CEB7B88B-3AA5-4DD6-818F-CF19E12BF1E1}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{CEB7B88B-3AA5-4DD6-818F-CF19E12BF1E1}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{1CD667DD-E7AE-4F36-A1F7-F41FB59786BE}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{1CD667DD-E7AE-4F36-A1F7-F41FB59786BE}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{1CD667DD-E7AE-4F36-A1F7-F41FB59786BE}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{1CD667DD-E7AE-4F36-A1F7-F41FB59786BE}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{1CD667DD-E7AE-4F36-A1F7-F41FB59786BE}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{1CD667DD-E7AE-4F36-A1F7-F41FB59786BE}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{1CD667DD-E7AE-4F36-A1F7-F41FB59786BE}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{1CD667DD-E7AE-4F36-A1F7-F41FB59786BE}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{289C605C-94CC-437F-836B-2FE41528EEEC}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{289C605C-94CC-437F-836B-2FE41528EEEC}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{289C605C-94CC-437F-836B-2FE41528EEEC}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{289C605C-94CC-437F-836B-2FE41528EEEC}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{289C605C-94CC-437F-836B-2FE41528EEEC}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{289C605C-94CC-437F-836B-2FE41528EEEC}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{289C605C-94CC-437F-836B-2FE41528EEEC}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{289C605C-94CC-437F-836B-2FE41528EEEC}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{D04E36D7-1C68-43E6-BE30-A7793CE3C799}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{D04E36D7-1C68-43E6-BE30-A7793CE3C799}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{D04E36D7-1C68-43E6-BE30-A7793CE3C799}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{D04E36D7-1C68-43E6-BE30-A7793CE3C799}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{D04E36D7-1C68-43E6-BE30-A7793CE3C799}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{D04E36D7-1C68-43E6-BE30-A7793CE3C799}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{D04E36D7-1C68-43E6-BE30-A7793CE3C799}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{D04E36D7-1C68-43E6-BE30-A7793CE3C799}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{AD5ACED4-F6EE-4AC0-B502-23CFD10EF603}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{AD5ACED4-F6EE-4AC0-B502-23CFD10EF603}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{AD5ACED4-F6EE-4AC0-B502-23CFD10EF603}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{AD5ACED4-F6EE-4AC0-B502-23CFD10EF603}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{AD5ACED4-F6EE-4AC0-B502-23CFD10EF603}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{AD5ACED4-F6EE-4AC0-B502-23CFD10EF603}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{AD5ACED4-F6EE-4AC0-B502-23CFD10EF603}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{AD5ACED4-F6EE-4AC0-B502-23CFD10EF603}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{46771042-C840-4314-BA3C-C5BC2FBD7CBB}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{46771042-C840-4314-BA3C-C5BC2FBD7CBB}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{46771042-C840-4314-BA3C-C5BC2FBD7CBB}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{46771042-C840-4314-BA3C-C5BC2FBD7CBB}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{46771042-C840-4314-BA3C-C5BC2FBD7CBB}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{46771042-C840-4314-BA3C-C5BC2FBD7CBB}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{46771042-C840-4314-BA3C-C5BC2FBD7CBB}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{46771042-C840-4314-BA3C-C5BC2FBD7CBB}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{2E03AF33-56A1-4366-B016-184CD96F3B56}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2E03AF33-56A1-4366-B016-184CD96F3B56}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2E03AF33-56A1-4366-B016-184CD96F3B56}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{2E03AF33-56A1-4366-B016-184CD96F3B56}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{2E03AF33-56A1-4366-B016-184CD96F3B56}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2E03AF33-56A1-4366-B016-184CD96F3B56}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2E03AF33-56A1-4366-B016-184CD96F3B56}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{2E03AF33-56A1-4366-B016-184CD96F3B56}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{97541B63-87AA-4ACE-BBF5-175F8D1389EC}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{97541B63-87AA-4ACE-BBF5-175F8D1389EC}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{97541B63-87AA-4ACE-BBF5-175F8D1389EC}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{97541B63-87AA-4ACE-BBF5-175F8D1389EC}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{97541B63-87AA-4ACE-BBF5-175F8D1389EC}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{97541B63-87AA-4ACE-BBF5-175F8D1389EC}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{97541B63-87AA-4ACE-BBF5-175F8D1389EC}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{97541B63-87AA-4ACE-BBF5-175F8D1389EC}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{45797F58-624B-4FA0-A7A3-5956114D8215}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{45797F58-624B-4FA0-A7A3-5956114D8215}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{45797F58-624B-4FA0-A7A3-5956114D8215}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{45797F58-624B-4FA0-A7A3-5956114D8215}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{45797F58-624B-4FA0-A7A3-5956114D8215}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{45797F58-624B-4FA0-A7A3-5956114D8215}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{45797F58-624B-4FA0-A7A3-5956114D8215}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{45797F58-624B-4FA0-A7A3-5956114D8215}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{7A2467D2-B03A-4964-A289-EF233EB39F69}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{7A2467D2-B03A-4964-A289-EF233EB39F69}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{7A2467D2-B03A-4964-A289-EF233EB39F69}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{7A2467D2-B03A-4964-A289-EF233EB39F69}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{7A2467D2-B03A-4964-A289-EF233EB39F69}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{7A2467D2-B03A-4964-A289-EF233EB39F69}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{7A2467D2-B03A-4964-A289-EF233EB39F69}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{7A2467D2-B03A-4964-A289-EF233EB39F69}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{AE750416-51BA-485C-BD76-EF11B4536EE8}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{AE750416-51BA-485C-BD76-EF11B4536EE8}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{AE750416-51BA-485C-BD76-EF11B4536EE8}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{AE750416-51BA-485C-BD76-EF11B4536EE8}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{AE750416-51BA-485C-BD76-EF11B4536EE8}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{AE750416-51BA-485C-BD76-EF11B4536EE8}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{AE750416-51BA-485C-BD76-EF11B4536EE8}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{AE750416-51BA-485C-BD76-EF11B4536EE8}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{3E822491-B134-43B0-A40E-9348FFAB527F}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{3E822491-B134-43B0-A40E-9348FFAB527F}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{3E822491-B134-43B0-A40E-9348FFAB527F}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{3E822491-B134-43B0-A40E-9348FFAB527F}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{3E822491-B134-43B0-A40E-9348FFAB527F}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{3E822491-B134-43B0-A40E-9348FFAB527F}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{3E822491-B134-43B0-A40E-9348FFAB527F}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{3E822491-B134-43B0-A40E-9348FFAB527F}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{AF932F77-1804-4DA4-A6DC-950795D6AFC1}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{AF932F77-1804-4DA4-A6DC-950795D6AFC1}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{AF932F77-1804-4DA4-A6DC-950795D6AFC1}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{AF932F77-1804-4DA4-A6DC-950795D6AFC1}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{AF932F77-1804-4DA4-A6DC-950795D6AFC1}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{AF932F77-1804-4DA4-A6DC-950795D6AFC1}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{AF932F77-1804-4DA4-A6DC-950795D6AFC1}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{AF932F77-1804-4DA4-A6DC-950795D6AFC1}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{45236F71-7031-40BC-ADCD-0535A64C04B0}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{45236F71-7031-40BC-ADCD-0535A64C04B0}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{45236F71-7031-40BC-ADCD-0535A64C04B0}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{45236F71-7031-40BC-ADCD-0535A64C04B0}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{45236F71-7031-40BC-ADCD-0535A64C04B0}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{45236F71-7031-40BC-ADCD-0535A64C04B0}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{45236F71-7031-40BC-ADCD-0535A64C04B0}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{45236F71-7031-40BC-ADCD-0535A64C04B0}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{28D24031-1A17-497A-B926-59CF78076DED}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{28D24031-1A17-497A-B926-59CF78076DED}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{28D24031-1A17-497A-B926-59CF78076DED}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{28D24031-1A17-497A-B926-59CF78076DED}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{28D24031-1A17-497A-B926-59CF78076DED}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{28D24031-1A17-497A-B926-59CF78076DED}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{28D24031-1A17-497A-B926-59CF78076DED}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{28D24031-1A17-497A-B926-59CF78076DED}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{23F17042-704E-4198-8017-A8584E884CF7}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{23F17042-704E-4198-8017-A8584E884CF7}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{23F17042-704E-4198-8017-A8584E884CF7}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{23F17042-704E-4198-8017-A8584E884CF7}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{23F17042-704E-4198-8017-A8584E884CF7}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{23F17042-704E-4198-8017-A8584E884CF7}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{23F17042-704E-4198-8017-A8584E884CF7}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{23F17042-704E-4198-8017-A8584E884CF7}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{CEE12ACB-20E4-45E4-AD5E-D15D7CFF68C3}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{CEE12ACB-20E4-45E4-AD5E-D15D7CFF68C3}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{CEE12ACB-20E4-45E4-AD5E-D15D7CFF68C3}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{CEE12ACB-20E4-45E4-AD5E-D15D7CFF68C3}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{CEE12ACB-20E4-45E4-AD5E-D15D7CFF68C3}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{CEE12ACB-20E4-45E4-AD5E-D15D7CFF68C3}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{CEE12ACB-20E4-45E4-AD5E-D15D7CFF68C3}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{CEE12ACB-20E4-45E4-AD5E-D15D7CFF68C3}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{08B82852-90B3-4767-A5D2-F0A4FCCB2377}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{08B82852-90B3-4767-A5D2-F0A4FCCB2377}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{08B82852-90B3-4767-A5D2-F0A4FCCB2377}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{08B82852-90B3-4767-A5D2-F0A4FCCB2377}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{08B82852-90B3-4767-A5D2-F0A4FCCB2377}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{08B82852-90B3-4767-A5D2-F0A4FCCB2377}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{08B82852-90B3-4767-A5D2-F0A4FCCB2377}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{08B82852-90B3-4767-A5D2-F0A4FCCB2377}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{E1D9CE3F-400D-40E8-AD0D-61C29B1847FF}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E1D9CE3F-400D-40E8-AD0D-61C29B1847FF}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E1D9CE3F-400D-40E8-AD0D-61C29B1847FF}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{E1D9CE3F-400D-40E8-AD0D-61C29B1847FF}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{E1D9CE3F-400D-40E8-AD0D-61C29B1847FF}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E1D9CE3F-400D-40E8-AD0D-61C29B1847FF}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E1D9CE3F-400D-40E8-AD0D-61C29B1847FF}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{E1D9CE3F-400D-40E8-AD0D-61C29B1847FF}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{D640A221-F95A-40FF-AC0E-0E8B615C7681}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{D640A221-F95A-40FF-AC0E-0E8B615C7681}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{D640A221-F95A-40FF-AC0E-0E8B615C7681}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{D640A221-F95A-40FF-AC0E-0E8B615C7681}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{D640A221-F95A-40FF-AC0E-0E8B615C7681}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{D640A221-F95A-40FF-AC0E-0E8B615C7681}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{D640A221-F95A-40FF-AC0E-0E8B615C7681}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{D640A221-F95A-40FF-AC0E-0E8B615C7681}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{50651D04-C8F7-4C2B-B412-0848573BDF88}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{50651D04-C8F7-4C2B-B412-0848573BDF88}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{50651D04-C8F7-4C2B-B412-0848573BDF88}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{50651D04-C8F7-4C2B-B412-0848573BDF88}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{50651D04-C8F7-4C2B-B412-0848573BDF88}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{50651D04-C8F7-4C2B-B412-0848573BDF88}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{50651D04-C8F7-4C2B-B412-0848573BDF88}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{50651D04-C8F7-4C2B-B412-0848573BDF88}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B7A8-8D942AA0F8F9}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B7A8-8D942AA0F8F9}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B7A8-8D942AA0F8F9}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B7A8-8D942AA0F8F9}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B7A8-8D942AA0F8F9}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B7A8-8D942AA0F8F9}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B7A8-8D942AA0F8F9}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{ADA2462A-3656-41D2-B7A8-8D942AA0F8F9}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{CF98E92E-C789-47E6-A2DA-398EB7991A2B}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{CF98E92E-C789-47E6-A2DA-398EB7991A2B}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{CF98E92E-C789-47E6-A2DA-398EB7991A2B}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{CF98E92E-C789-47E6-A2DA-398EB7991A2B}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{CF98E92E-C789-47E6-A2DA-398EB7991A2B}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{CF98E92E-C789-47E6-A2DA-398EB7991A2B}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{CF98E92E-C789-47E6-A2DA-398EB7991A2B}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{CF98E92E-C789-47E6-A2DA-398EB7991A2B}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{33331FB8-2AF9-4D15-BEE3-5F69658A6F1F}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{33331FB8-2AF9-4D15-BEE3-5F69658A6F1F}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{33331FB8-2AF9-4D15-BEE3-5F69658A6F1F}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{33331FB8-2AF9-4D15-BEE3-5F69658A6F1F}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{33331FB8-2AF9-4D15-BEE3-5F69658A6F1F}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{33331FB8-2AF9-4D15-BEE3-5F69658A6F1F}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{33331FB8-2AF9-4D15-BEE3-5F69658A6F1F}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{33331FB8-2AF9-4D15-BEE3-5F69658A6F1F}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{2C3FB329-620B-41A0-8845-A063C9EA8807}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2C3FB329-620B-41A0-8845-A063C9EA8807}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2C3FB329-620B-41A0-8845-A063C9EA8807}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{2C3FB329-620B-41A0-8845-A063C9EA8807}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{2C3FB329-620B-41A0-8845-A063C9EA8807}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2C3FB329-620B-41A0-8845-A063C9EA8807}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2C3FB329-620B-41A0-8845-A063C9EA8807}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{2C3FB329-620B-41A0-8845-A063C9EA8807}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{2D92C98B-F186-4BF0-A9D4-51D42C93F6E8}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2D92C98B-F186-4BF0-A9D4-51D42C93F6E8}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{2D92C98B-F186-4BF0-A9D4-51D42C93F6E8}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{2D92C98B-F186-4BF0-A9D4-51D42C93F6E8}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{2D92C98B-F186-4BF0-A9D4-51D42C93F6E8}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2D92C98B-F186-4BF0-A9D4-51D42C93F6E8}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{2D92C98B-F186-4BF0-A9D4-51D42C93F6E8}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{2D92C98B-F186-4BF0-A9D4-51D42C93F6E8}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{E88CCAF5-5DB9-4A46-8C91-97C5697F167A}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E88CCAF5-5DB9-4A46-8C91-97C5697F167A}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{E88CCAF5-5DB9-4A46-8C91-97C5697F167A}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{E88CCAF5-5DB9-4A46-8C91-97C5697F167A}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{E88CCAF5-5DB9-4A46-8C91-97C5697F167A}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E88CCAF5-5DB9-4A46-8C91-97C5697F167A}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{E88CCAF5-5DB9-4A46-8C91-97C5697F167A}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{E88CCAF5-5DB9-4A46-8C91-97C5697F167A}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{BA275A4C-FD92-4AC2-8996-1A32E9BC4E3A}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{BA275A4C-FD92-4AC2-8996-1A32E9BC4E3A}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{BA275A4C-FD92-4AC2-8996-1A32E9BC4E3A}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{BA275A4C-FD92-4AC2-8996-1A32E9BC4E3A}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{BA275A4C-FD92-4AC2-8996-1A32E9BC4E3A}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{BA275A4C-FD92-4AC2-8996-1A32E9BC4E3A}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{BA275A4C-FD92-4AC2-8996-1A32E9BC4E3A}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{BA275A4C-FD92-4AC2-8996-1A32E9BC4E3A}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{9CF889F1-AEFE-43BA-BA18-B2B09EF8ADEE}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{9CF889F1-AEFE-43BA-BA18-B2B09EF8ADEE}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{9CF889F1-AEFE-43BA-BA18-B2B09EF8ADEE}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{9CF889F1-AEFE-43BA-BA18-B2B09EF8ADEE}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{9CF889F1-AEFE-43BA-BA18-B2B09EF8ADEE}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{9CF889F1-AEFE-43BA-BA18-B2B09EF8ADEE}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{9CF889F1-AEFE-43BA-BA18-B2B09EF8ADEE}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{9CF889F1-AEFE-43BA-BA18-B2B09EF8ADEE}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\t\t{86C52ED6-C710-40E6-86A2-61F757B20CF7}.Debug Cairo|Win32.ActiveCfg = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{86C52ED6-C710-40E6-86A2-61F757B20CF7}.Debug Cairo|Win32.Build.0 = Debug Cairo|Win32\r\n");
  fprintf(out, "\t\t{86C52ED6-C710-40E6-86A2-61F757B20CF7}.Debug|Win32.ActiveCfg = Debug|Win32\r\n");
  fprintf(out, "\t\t{86C52ED6-C710-40E6-86A2-61F757B20CF7}.Debug|Win32.Build.0 = Debug|Win32\r\n");
  fprintf(out, "\t\t{86C52ED6-C710-40E6-86A2-61F757B20CF7}.Release Cairo|Win32.ActiveCfg = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{86C52ED6-C710-40E6-86A2-61F757B20CF7}.Release Cairo|Win32.Build.0 = Release Cairo|Win32\r\n");
  fprintf(out, "\t\t{86C52ED6-C710-40E6-86A2-61F757B20CF7}.Release|Win32.ActiveCfg = Release|Win32\r\n");
  fprintf(out, "\t\t{86C52ED6-C710-40E6-86A2-61F757B20CF7}.Release|Win32.Build.0 = Release|Win32\r\n");
  fprintf(out, "\tEndGlobalSection\r\n");
  
  fprintf(out, "\tGlobalSection(SolutionProperties) = preSolution\r\n");
  fprintf(out, "\t\tHideSolutionNode = FALSE\r\n");
  fprintf(out, "\tEndGlobalSection\r\n");
  
  fprintf(out, "EndGlobal\r\n");
  
  return 0;
}


/*
 Create the list of linked libraries.
 */
static int write_additional_dependencies(FILE *out, Fl_Target_Type *tgt) {
  for (Fl_Type *t = tgt->next; t && (t->level>tgt->level); t = t->next) {
    if (t->is_tool() && ((Fl_Tool_Type*)t)->builds_in(FL_ENV_VC2008)) {
      if (t->is_file() && ((Fl_File_Type*)t)->file_is_library()) {
        fprintf(out, "%s;", t->name());
      } else if (t->is_target_dependency() && tgt->is_app_target()) {
        Fl_Target_Type *dep = Fl_Target_Type::find(t->name());
        if (dep && dep->is_lib_target()) {
          fprintf(out, "%s.lib;", t->name());
        }
      }
    }
  }
  return 0;
}


static int write_configuration(FILE *out, Fl_Target_Type *tgt, char is_debug) {
  // some definitions we will need later
  const char *cfg, *pre, *pre2;
  int opt, lib, cfg_type;
  char is_lib, cfg_path[256];
  
  if (is_debug) {
    cfg = "Debug";  // this is the name of the configuration as well as the path for all temporary files
    pre = "_DEBUG"; // this is a preprocessor definition for compile time configuration
    opt = 0;        // this is the level of optimization
    lib = 3;        // this is the link library (not sure what it actually does...)
    sprintf(cfg_path, "%s%s", tgt->name(), "_debug");
  } else {
    cfg = "Release";
    pre = "NDEBUG";
    opt = 4;
    lib = 2;
    sprintf(cfg_path, "%s%s", tgt->name(), "_release");
  }
  
  if (tgt->is_lib_target()) {
    is_lib = 1;           // are we building a (static) library?
    cfg_type = 4;         // type of configuration
    pre2 = "FL_LIBRARY;"; // additional preprocessor definition
  } else {
    cfg_type = 1;
    is_lib = 0;
    pre2 = "";
  }
  
  fprintf(out, "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='%s|Win32'\">\r\n", cfg);
  fprintf(out, "    <Midl>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>%s;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n", pre);
  fprintf(out, "      <MkTypLibCompatible>true</MkTypLibCompatible>\r\n");
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <TargetEnvironment>Win32</TargetEnvironment>\r\n");
  fprintf(out, "      <TypeLibraryName>.\\%s\\%s.tlb</TypeLibraryName>\r\n", cfg_path, tgt->name());
  fprintf(out, "      <HeaderFileName>\r\n");
  fprintf(out, "      </HeaderFileName>\r\n");
  fprintf(out, "    </Midl>\r\n");
  fprintf(out, "    <ClCompile>\r\n");
  fprintf(out, "      <Optimization>Disabled</Optimization>\r\n");
  if (!is_debug) {
    fprintf(out, "      <InlineFunctionExpansion>AnySuitable</InlineFunctionExpansion>\r\n");
    fprintf(out, "      <FavorSizeOrSpeed>Size</FavorSizeOrSpeed>\r\n");
  }
  fprintf(out, "      <AdditionalIncludeDirectories>.;..\\..\\zlib;..\\..\\png;..\\..\\jpeg;..\\..;%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>_CRT_SECURE_NO_DEPRECATE;WIN32;%s;_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n", pre);
  if (is_debug) {
    fprintf(out, "      <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>\r\n");
  } else {
    fprintf(out, "      <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>\r\n");
  }
  fprintf(out, "      <PrecompiledHeader>\r\n");
  fprintf(out, "      </PrecompiledHeader>\r\n");
  if (!is_debug) {
    fprintf(out, "      <AssemblerListingLocation>$(IntDir)</AssemblerListingLocation>\r\n");
    fprintf(out, "      <ObjectFileName>$(IntDir)</ObjectFileName>\r\n");
    fprintf(out, "      <ProgramDataBaseFileName>$(IntDir)vc$(PlatformToolsetVersion).pdb</ProgramDataBaseFileName>\r\n");
  }
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  if (is_debug) {
    fprintf(out, "      <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>\r\n");
  }
  fprintf(out, "      <CompileAs>Default</CompileAs>\r\n");
  fprintf(out, "    </ClCompile>\r\n");
  
  fprintf(out, "    <ResourceCompile>\r\n");
  fprintf(out, "      <PreprocessorDefinitions>%s;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n", pre);
  fprintf(out, "      <Culture>0x0409</Culture>\r\n");
  fprintf(out, "    </ResourceCompile>\r\n");

  fprintf(out, "    <Link>\r\n");
  // TODO: write the library list
  fprintf(out, "      <AdditionalDependencies>");
  write_additional_dependencies(out, tgt);
  fprintf(out, "comctl32.lib;%%(AdditionalDependencies)</AdditionalDependencies>\r\n");
  fprintf(out, "      <OutputFile>..\\..\\%s\\%s.exe</OutputFile>\r\n", DOS_path(tgt->target_path()), tgt->name());
  fprintf(out, "      <SuppressStartupBanner>true</SuppressStartupBanner>\r\n");
  fprintf(out, "      <AdditionalLibraryDirectories>..\\..\\lib;%%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>\r\n");
  fprintf(out, "      <IgnoreSpecificDefaultLibraries>libcd;%%(IgnoreSpecificDefaultLibraries)</IgnoreSpecificDefaultLibraries>\r\n");
  fprintf(out, "      <GenerateDebugInformation>true</GenerateDebugInformation>\r\n");
  fprintf(out, "      <SubSystem>Windows</SubSystem>\r\n");
  fprintf(out, "      <RandomizedBaseAddress>false</RandomizedBaseAddress>\r\n");
  fprintf(out, "      <DataExecutionPrevention>\r\n");
  fprintf(out, "      </DataExecutionPrevention>\r\n");
  fprintf(out, "      <TargetMachine>MachineX86</TargetMachine>\r\n");
  if (is_debug) {
    fprintf(out, "      <ProgramDatabaseFile>$(IntDir)$(TargetName).pdb</ProgramDatabaseFile>\r\n");
  }
  fprintf(out, "    </Link>\r\n");
  fprintf(out, "  </ItemDefinitionGroup>\r\n");
  
  return 0;
}


static int write_source_file(FILE *out, Fl_File_Type *file, const char *filename=0) {
  if (!filename) filename = file->filename();
  fprintf(out, "    <ClCompile Include=\"..\\..\\%s\">\r\n", DOS_path(filename));
  fprintf(out, "      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">Disabled</Optimization>\r\n");
  fprintf(out, "      <AdditionalIncludeDirectories Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
  fprintf(out, "      <PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "      <AdditionalIncludeDirectories Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\r\n");
  fprintf(out, "      <PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">%%(PreprocessorDefinitions)</PreprocessorDefinitions>\r\n");
  fprintf(out, "    </ClCompile>\r\n");
  return 0;
}


static int write_fluid_source_file(FILE *out, Fl_File_Type *file) {
  char cxxName[2048];
  strcpy(cxxName, file->filename());
  fltk3::filename_setext(cxxName, 2048, ".cxx");  
  return write_source_file(out, file, cxxName);
}
 

static int write_fluid_custom_build(FILE *out, Fl_File_Type *file) {
  char flPath[2048];
  strcpy(flPath, file->filename());
  *((char*)fltk3::filename_name(flPath)) = 0; // keep only the path  

  fprintf(out, "  <ItemGroup>\r\n");
  fprintf(out, "    <CustomBuild Include=\"..\\..\\%s\">\r\n", DOS_path(file->filename()));
  fprintf(out, "      <Message Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">Create .cxx and .h file with fluidd</Message>\r\n");
  fprintf(out, "      <Command Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">cfluid /D %%(Filename).fl\r\n");
  fprintf(out, "      </Command>\r\n");
  fprintf(out, "      <Outputs Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">..\\..\\%s\\%%(Filename).cxx;%%(Outputs)</Outputs>\r\n", DOS_path(flPath));
  fprintf(out, "      <Message Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">Create .cxx and .h file with fluid</Message>\r\n");
  fprintf(out, "      <Command Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">cfluid %%(Filename).fl\r\n");
  fprintf(out, "      </Command>\r\n");
  fprintf(out, "      <Outputs Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">..\\..\\%s\\%%(Filename).cxx;%%(Outputs)</Outputs>\r\n", DOS_path(flPath));
  fprintf(out, "    </CustomBuild>\r\n");
  fprintf(out, "  </ItemGroup>\r\n");
  
  return 0;
}


/*
 Write a VisualC 2010 Project file.
 */
static int write_vcxproj_file(FILE *out, Fl_Target_Type *tgt) {
  
  fprintf(out, "\357\273\277<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
  
  fprintf(out, "<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n");
  
  fprintf(out, "  <ItemGroup Label=\"ProjectConfigurations\">\r\n");
  fprintf(out, "    <ProjectConfiguration Include=\"Debug|Win32\">\r\n");
  fprintf(out, "      <Configuration>Debug</Configuration>\r\n");
  fprintf(out, "      <Platform>Win32</Platform>\r\n");
  fprintf(out, "    </ProjectConfiguration>\r\n");
  fprintf(out, "    <ProjectConfiguration Include=\"Release|Win32\">\r\n");
  fprintf(out, "      <Configuration>Release</Configuration>\r\n");
  fprintf(out, "      <Platform>Win32</Platform>\r\n");
  fprintf(out, "    </ProjectConfiguration>\r\n");
  fprintf(out, "  </ItemGroup>\r\n");
  
  fprintf(out, "  <PropertyGroup Label=\"Globals\">\r\n");
  fprintf(out, "    <ProjectGuid>{%s}</ProjectGuid>\r\n", tgt->get_UUID(VC2010_Project));
  fprintf(out, "  </PropertyGroup>\r\n");
  
  fprintf(out, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\r\n");
  
  fprintf(out, "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">\r\n");
  fprintf(out, "    <ConfigurationType>Application</ConfigurationType>\r\n");
  fprintf(out, "    <UseOfMfc>false</UseOfMfc>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  
  fprintf(out, "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">\r\n");
  fprintf(out, "    <ConfigurationType>Application</ConfigurationType>\r\n");
  fprintf(out, "    <UseOfMfc>false</UseOfMfc>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  
  fprintf(out, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\r\n");
  fprintf(out, "  <ImportGroup Label=\"ExtensionSettings\">\r\n");
  fprintf(out, "  </ImportGroup>\r\n");
  
  fprintf(out, "  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"PropertySheets\">\r\n");
  fprintf(out, "    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\r\n");
  fprintf(out, "    <Import Project=\"$(VCTargetsPath)Microsoft.CPP.UpgradeFromVC71.props\" />\r\n");
  fprintf(out, "  </ImportGroup>\r\n");
  
  fprintf(out, "  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"PropertySheets\">\r\n");
  fprintf(out, "    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\r\n");
  fprintf(out, "    <Import Project=\"$(VCTargetsPath)Microsoft.CPP.UpgradeFromVC71.props\" />\r\n");
  fprintf(out, "  </ImportGroup>\r\n");
  
  fprintf(out, "  <PropertyGroup Label=\"UserMacros\" />\r\n");
  
  fprintf(out, "  <PropertyGroup>\r\n");
  fprintf(out, "    <_ProjectFileVersion>10.0.30319.1</_ProjectFileVersion>\r\n");
  fprintf(out, "    <OutDir Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">..\\..\\%s\\</OutDir>\r\n", DOS_path(tgt->target_path()));
  fprintf(out, "    <IntDir Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">.\\$(ProjectName)_debug\\</IntDir>\r\n");
  fprintf(out, "    <LinkIncremental Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</LinkIncremental>\r\n");
  fprintf(out, "    <OutDir Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">..\\..\\%s\\</OutDir>\r\n", DOS_path(tgt->target_path()));
  fprintf(out, "    <IntDir Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">.\\$(ProjectName)_release\\</IntDir>\r\n");
  fprintf(out, "    <LinkIncremental Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">false</LinkIncremental>\r\n");
  fprintf(out, "    <TargetName Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">$(ProjectName)d</TargetName>\r\n");
  fprintf(out, "  </PropertyGroup>\r\n");
  
  write_configuration(out, tgt, 1);
  write_configuration(out, tgt, 0);
  
  char has_custom_build = 0;
  
  fprintf(out, "  <ItemGroup>\r\n");
  for (Fl_File_Type *file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
    if (file->builds_in(FL_ENV_VC2010)) {
      if (file->file_is_fluid_ui()) {
        write_fluid_source_file(out, file);
        has_custom_build = 1;
      } else if (file->file_is_code()) {
        write_source_file(out, file);
      }
    }
  }
  fprintf(out, "  </ItemGroup>\r\n");
  
  if (has_custom_build) {
    fprintf(out, "  <ItemGroup>\r\n");
    for (Fl_File_Type *file = Fl_File_Type::first_file(tgt); file; file = file->next_file(tgt)) {
      if (file->builds_in(FL_ENV_VC2010)) {
        if (file->file_is_fluid_ui()) {
          write_fluid_custom_build(out, file);
          has_custom_build = 1;
        }
      }
    }
    fprintf(out, "  </ItemGroup>\r\n");
  }
  
  fprintf(out, "  <ItemGroup>\r\n");
  Fl_Target_Dependency_Type *tgt_dep = Fl_Target_Dependency_Type::first_dependency(tgt);
  for ( ; tgt_dep; tgt_dep = tgt_dep->next_dependency(tgt)) {
    Fl_Target_Type *dep = Fl_Target_Type::find(tgt_dep->name());
    if (dep && tgt_dep->builds_in(FL_ENV_VC2010)) {
      if (strcmp(dep->name(), "fltk")==0)
		fprintf(out, "    <ProjectReference Include=\"%s.lib.vcxproj\">\r\n", dep->name());
	  else
        fprintf(out, "    <ProjectReference Include=\"%s.vcxproj\">\r\n", dep->name());
      fprintf(out, "      <Project>{%s}</Project>\r\n", dep->get_UUID(VC2010_Project));
      fprintf(out, "      <ReferenceOutputAssembly>false</ReferenceOutputAssembly>\r\n");
      fprintf(out, "    </ProjectReference>\r\n");
    }
  }
  fprintf(out, "  </ItemGroup>\r\n");
  
  fprintf(out, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\r\n");
  fprintf(out, "  <ImportGroup Label=\"ExtensionTargets\">\r\n");
  fprintf(out, "  </ImportGroup>\r\n");
  
  fprintf(out, "</Project>");
  
  return 0;
}



int write_fltk_ide_visualc2010() {

  Fl_Workspace_Type *workspace = (Fl_Workspace_Type*)Fl_Type::first;
  
  workspace = (Fl_Workspace_Type*)Fl_Type::first;
  
  char buf[2048], base_dir[2048], tgt_base[2048];
  strcpy(base_dir, filename);
  *((char*)fltk3::filename_name(base_dir)) = 0; // keep only the path
  strcpy(tgt_base, base_dir);
  strcpy(buf, base_dir);
  
  // write workspace file (.sln)
  strcat(buf, "ide/VisualC2010/fltk.sln");
  // FIXME: use workspace->name();  
  FILE *out = fopen(buf, "wb");
  write_sln_file(out, workspace);
  fclose(out);
  
  // TODO: write fluid build rule into a file
  // cfluid.cmd 
  
  // write project files (.vcproj)
  for (Fl_Target_Type *tgt = Fl_Target_Type::first_target(workspace); tgt; tgt = tgt->next_target(workspace)) {
    if (tgt->builds_in(FL_ENV_VC2010)) {
      if (tgt->is_app_target() && strcmp(tgt->name(), "adjuster")==0) {
        sprintf(buf, "%side/VisualC2010/%s.vcxproj", base_dir, tgt->name());
        out = fopen(buf, "wb");
        write_vcxproj_file(out, tgt);
        fclose(out);
      } else if (tgt->is_lib_target()) {
        /*
        sprintf(buf, "%side/VisualC2008/%s.vcproj", base_dir, tgt->name());
        out = fopen(buf, "wb");
        write_vcproj_file(out, tgt);
        fclose(out);
        // TODO: no support for dll's yet
         */
      } else {
        printf("Internale error writing unknown VC2010 target!\n");
      }
    }
  }
  
  return 0;  
}

//
// End of "$Id: file.cxx 8870 2011-07-26 21:19:35Z matt $".
//
