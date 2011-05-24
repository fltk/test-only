//
// "$Id$"
//
// File chooser test program.
//
// Copyright 1999-2010 by Michael Sweet.
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
// Contents:
//
//   main()           - Create a file chooser and wait for a selection to
//                      be made.
//   close_callback() - Close the main window...
//   fc_callback()    - Handle choices in the file chooser...
//   pdf_check()      - Check for and load the first page of a PDF file.
//   ps_check()       - Check for and load the first page of a PostScript
//                      file.
//   show_callback()  - Show the file chooser...
//
//   extra_callback() - circle extra groups (none,group1,check_button);
//

//
// Include necessary headers...
//

#include <stdio.h>
#include <fltk3/FileChooser.h>
#include <fltk3/FileIcon.h>
#include <fltk3/SharedImage.h>
#include <fltk3/PNMImage.h>
#include <fltk3/LightButton.h>
#include <fltk3/DoubleWindow.h>
#include <string.h>


//
// Globals...
//

fltk3::Input		*filter;
fltk3::FileBrowser		*files;
fltk3::FileChooser		*fc;
fltk3::SharedImage		*image = 0;

// for choosing extra groups
fltk3::Choice *ch_extra;
// first extra group
fltk3::Group *encodings = (fltk3::Group*)0;
fltk3::Choice *ch_enc;
// second extra widget
fltk3::CheckButton *version = (fltk3::CheckButton*)0;

//
// Functions...
//

void		close_callback(void);
void		create_callback(void);
void		dir_callback(void);
void		fc_callback(fltk3::FileChooser *, void *);
void		multi_callback(void);
fltk3::Image	*pdf_check(const char *, uchar *, int);
fltk3::Image	*ps_check(const char *, uchar *, int);
void		show_callback(void);

void		extra_callback(fltk3::Choice*,void*);

//
// 'main()' - Create a file chooser and wait for a selection to be made.
//

int			// O - Exit status
main(int  argc,		// I - Number of command-line arguments
     char *argv[])	// I - Command-line arguments
{
  fltk3::DoubleWindow	*window;// Main window
  fltk3::Button		*button;// Buttons
  fltk3::FileIcon		*icon;	// New file icon


  // Make the file chooser...
  fltk3::scheme(NULL);
  fltk3::FileIcon::load_system_icons();

  fc = new fltk3::FileChooser(".", "*", fltk3::FileChooser::SINGLE, "fltk3::FileChooser Test");
  fc->callback(fc_callback);

  // Register the PS and PDF image types...
  fltk3::SharedImage::add_handler(pdf_check);
  fltk3::SharedImage::add_handler(ps_check);

  // Make the main window...
  window = new fltk3::DoubleWindow(400, 215, "File Chooser Test");

  filter = new fltk3::Input(50, 10, 315, 25, "Filter:");
  int argn = 1;
#ifdef __APPLE__
  // OS X may add the process number as the first argument - ignore
  if (argc>argn && strncmp(argv[1], "-psn_", 5)==0)
    argn++;
#endif
  if (argc > argn)
    filter->value(argv[argn]);
  else
    filter->value("PDF Files (*.pdf)\t"
                  "PostScript Files (*.ps)\t"
		  "Image Files (*.{bmp,gif,jpg,png})\t"
		  "C/C++ Source Files (*.{c,C,cc,cpp,cxx})");

  button = new fltk3::Button(365, 10, 25, 25);
  button->labelcolor(fltk3::YELLOW);
  button->callback((fltk3::Callback *)show_callback);

  icon   = fltk3::FileIcon::find(".", fltk3::FileIcon::DIRECTORY);
  icon->label(button);

  button = new fltk3::LightButton(50, 45, 80, 25, "MULTI");
  button->callback((fltk3::Callback *)multi_callback);

  button = new fltk3::LightButton(140, 45, 90, 25, "CREATE");
  button->callback((fltk3::Callback *)create_callback);

  button = new fltk3::LightButton(240, 45, 115, 25, "DIRECTORY");
  button->callback((fltk3::Callback *)dir_callback);

  //
  ch_extra = new fltk3::Choice(150, 75, 150, 25, "Extra Group:");
  ch_extra->add("none|encodings group|check button");
  ch_extra->value(0);
  ch_extra->callback((fltk3::Callback *)extra_callback);
  //
  files = new fltk3::FileBrowser(50, 105, 340, 75, "Files:");
  files->align(fltk3::ALIGN_LEFT);

  button = new fltk3::Button(340, 185, 50, 25, "Close");
  button->callback((fltk3::Callback *)close_callback);

  window->resizable(files);
  window->end();
  window->show(1, argv);

  fltk3::run();

  return (0);
}


void
extra_callback(fltk3::Choice*w,void*)
{
  int val=w->value();
  if (0 == val) fc->add_extra(NULL);
  else if (1 == val) {
    if(!encodings){
      encodings=new fltk3::Group(0,0,254,30);
      ch_enc=new fltk3::Choice(152,2,100,25,"Choose Encoding:");
      ch_enc->add("ASCII|Koi8-r|win1251|Utf-8");
      encodings->end();
    }
    fc->add_extra(encodings);
  } else {
    if (!version) {
      version = new fltk3::CheckButton(5,0,200,25,"Save binary 1.0 version");
    }
    fc->add_extra(version);
  }
}


//
// 'close_callback()' - Close the main window...
//

void
close_callback(void)
{
  exit(0);
}


//
// 'create_callback()' - Handle clicks on the create button.
//

void
create_callback(void)
{
  fc->type(fc->type() ^ fltk3::FileChooser::CREATE);
}


//
// 'dir_callback()' - Handle clicks on the directory button.
//

void
dir_callback(void)
{
  fc->type(fc->type() ^ fltk3::FileChooser::DIRECTORY);
}


//
// 'fc_callback()' - Handle choices in the file chooser...
//

void
fc_callback(fltk3::FileChooser *fc,	// I - File chooser
            void            *data)	// I - Data
{
  const char		*filename;	// Current filename


  printf("fc_callback(fc = %p, data = %p)\n", fc, data);

  filename = fc->value();

  printf("    filename = \"%s\"\n", filename ? filename : "(null)");
}


//
// 'multi_callback()' - Handle clicks on the multi button.
//

void
multi_callback(void)
{
  fc->type(fc->type() ^ fltk3::FileChooser::MULTI);
}


//
// 'pdf_check()' - Check for and load the first page of a PDF file.
//

fltk3::Image *			// O - Page image or NULL
pdf_check(const char *name,	// I - Name of file
          uchar      *header,	// I - Header data
	  int)			// I - Length of header data (unused)
{
  const char	*home;		// Home directory
  char		preview[FLTK3_PATH_MAX],	// Preview filename
		command[FLTK3_PATH_MAX];	// Command


  if (memcmp(header, "%PDF", 4) != 0)
    return 0;

  home = getenv("HOME");
  sprintf(preview, "%s/.preview.ppm", home ? home : "");

  sprintf(command,
          "gs -r100 -dFIXED -sDEVICE=ppmraw -dQUIET -dNOPAUSE -dBATCH "
	  "-sstdout=\"%%stderr\" -sOUTPUTFILE=\'%s\' "
	  "-dFirstPage=1 -dLastPage=1 \'%s\' 2>/dev/null", preview, name);

  if (system(command)) return 0;

  return new fltk3::PNMImage(preview);
}


//
// 'ps_check()' - Check for and load the first page of a PostScript file.
//

fltk3::Image *			// O - Page image or NULL
ps_check(const char *name,	// I - Name of file
         uchar      *header,	// I - Header data
	 int)			// I - Length of header data (unused)
{
  const char	*home;		// Home directory
  char		preview[FLTK3_PATH_MAX],	// Preview filename
		outname[FLTK3_PATH_MAX],	// Preview PS file
		command[FLTK3_PATH_MAX];	// Command
  FILE		*in,		// Input file
		*out;		// Output file
  int		page;		// Current page
  char		line[256];	// Line from file


  if (memcmp(header, "%!", 2) != 0)
    return 0;

  home = getenv("HOME");
  sprintf(preview, "%s/.preview.ppm", home ? home : "");

  if (memcmp(header, "%!PS", 4) == 0) {
    // PS file has DSC comments; extract the first page...
    sprintf(outname, "%s/.preview.ps", home ? home : "");

    if (strcmp(name, outname) != 0) {
      in   = fltk3::fopen(name, "rb");
      out  = fltk3::fopen(outname, "wb");
      page = 0;

      while (fgets(line, sizeof(line), in) != NULL) {
	if (strncmp(line, "%%Page:", 7) == 0) {
          page ++;
	  if (page > 1) break;
	}

	fputs(line, out);
      }

      fclose(in);
      fclose(out);
    }
  } else {
    // PS file doesn't have DSC comments; do the whole file...
    strncpy(outname, name, sizeof(outname) - 1);
    outname[sizeof(outname) - 1] = '\0';
  }

  sprintf(command,
          "gs -r100 -dFIXED -sDEVICE=ppmraw -dQUIET -dNOPAUSE -dBATCH "
	  "-sstdout=\"%%stderr\" -sOUTPUTFILE=\'%s\' \'%s\' 2>/dev/null",
	  preview, outname);

  if (system(command)) return 0;

  return new fltk3::PNMImage(preview);
}


//
// 'show_callback()' - Show the file chooser...
//

void
show_callback(void)
{
  int	i;			// Looping var
  int	count;			// Number of files selected
  char	relative[FLTK3_PATH_MAX];	// Relative filename


  if (filter->value()[0])
    fc->filter(filter->value());

  fc->show();

  while (fc->visible()) {
    fltk3::wait();
  }

  count = fc->count();
  if (count > 0)
  {
    files->clear();

    for (i = 1; i <= count; i ++)
    {
      if (!fc->value(i))
        break;

      fltk3::filename_relative(relative, sizeof(relative), fc->value(i));

      files->add(relative,
                 fltk3::FileIcon::find(fc->value(i), fltk3::FileIcon::PLAIN));
    }

    files->redraw();
  }
}


//
// End of "$Id$".
//
