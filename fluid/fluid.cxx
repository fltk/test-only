//
// "$Id$"
//
// FLUID main entry for the Fast Light Tool Kit (FLTK).
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

#include <fltk3/run.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Box.h>
#include <fltk3/Button.h>
#include <fltk3/FileIcon.h>
#include <fltk3/HelpDialog.h>
#include <fltk3/HoldBrowser.h>
#include <fltk3/MenuBar.h>
#include <fltk3/Input.h>
#include <fltk3/Plugin.h>
#include <fltk3/ask.h>
#include <fltk3/draw.h>
#include <fltk3/FileChooser.h>
#include <fltk3/PNGImage.h>
#include <fltk3/Printer.h>
#include <fltk3/message.h>
#include <fltk3/filename.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h> // time(), localtime(), etc.

#include "../src/flstring.h"
#include "alignment_panel.h"
#include "function_panel.h"
#include "template_panel.h"
#include "workspace_panel.h"

#if defined(WIN32) && !defined(__CYGWIN__)
#  include <direct.h>
#  include <windows.h>
#  include <io.h>
#  include <fcntl.h>
#  include <commdlg.h>
#  include <fltk3/x.h>
#  ifndef __WATCOMC__
// Visual C++ 2005 incorrectly displays a warning about the use of POSIX APIs
// on Windows, which is supposed to be POSIX compliant...
#    define access _access
#    define chdir _chdir
#    define getcwd _getcwd
#  endif // !__WATCOMC__
#else
#  include <unistd.h>
#endif
#ifdef __EMX__
#  include <X11/Xlibint.h>
#endif

#include "about_panel.h"
#include "undo.h"

#include "Fl_Type.h"

extern "C"
{
#if defined(HAVE_LIBPNG) && defined(HAVE_LIBZ)
#  include <zlib.h>
#  ifdef HAVE_PNG_H
#    include <png.h>
#  else
#    include <libpng/png.h>
#  endif // HAVE_PNG_H
#endif // HAVE_LIBPNG && HAVE_LIBZ
}

static fltk3::HelpDialog *help_dialog = 0;

fltk3::Preferences	fluid_prefs(fltk3::Preferences::USER, "fltk.org", "fluid");
int gridx = 5;
int gridy = 5;
int snap = 1;
int show_guides = 1;
int show_comments = 1;
int show_coredevmenus = 1;

// File history info...
char	absolute_history[10][FLTK3_PATH_MAX];
char	relative_history[10][FLTK3_PATH_MAX];

void	load_history();
void	update_history(const char *);

// Shell command support...
void	show_shell_window();

fltk3::MenuItem *save_item = 0L;
fltk3::MenuItem *history_item = 0L;
fltk3::MenuItem *widgetbin_item = 0L;
fltk3::MenuItem *sourceview_item = 0L;

////////////////////////////////////////////////////////////////

const char *filename;
void set_filename(const char *c);
void set_modflag(int mf);
int modflag;

static char* pwd;
static char in_source_dir;
void goto_source_dir() {
  if (in_source_dir) return;
  if (!filename || !*filename) return;
  const char *p = fltk3::filename_name(filename);
  if (p <= filename) return; // it is in the current directory
  char buffer[FLTK3_PATH_MAX];
  strlcpy(buffer, filename, sizeof(buffer));
  int n = p-filename; if (n>1) n--; buffer[n] = 0;
  if (!pwd) {
    pwd = getcwd(0,FLTK3_PATH_MAX);
    if (!pwd) {fprintf(stderr,"getwd : %s\n",strerror(errno)); return;}
  }
  if (chdir(buffer)<0) {fprintf(stderr, "Can't chdir to %s : %s\n",
				buffer, strerror(errno)); return;}
  in_source_dir = 1;
}

void leave_source_dir() {
  if (!in_source_dir) return;
  if (chdir(pwd)<0) {fprintf(stderr, "Can't chdir to %s : %s\n",
			     pwd, strerror(errno));}
  in_source_dir = 0;
}

char is_workspace() {
  if (Fl_Type::first && (Fl_Type::first->is_target() || Fl_Type::first->is_category() ))
    return 1;
  return 0;
}

char position_window(fltk3::Window *w, const char *prefsName, int Visible, int X, int Y, int W=0, int H=0 ) {
  fltk3::Preferences pos(fluid_prefs, prefsName);
  if (prevpos_button->value()) {
    pos.get("x", X, X);
    pos.get("y", Y, Y);
    if ( W!=0 ) {
      pos.get("w", W, W);
      pos.get("h", H, H);
      w->resize( X, Y, W, H );
    }
    else
      w->position( X, Y );
  }
  pos.get("visible", Visible, Visible);
  return Visible;
}

void save_position(fltk3::Window *w, const char *prefsName) {
  fltk3::Preferences pos(fluid_prefs, prefsName);
  pos.set("x", w->x());
  pos.set("y", w->y());
  pos.set("w", w->w());
  pos.set("h", w->h());
  pos.set("visible", (int)(w->shown() && w->visible()));
}

fltk3::Window *main_window;
fltk3::MenuBar *main_menubar;

static char* cutfname(int which = 0) {
  static char name[2][FLTK3_PATH_MAX];
  static char beenhere = 0;

  if (!beenhere) {
    beenhere = 1;
    fluid_prefs.getUserdataPath(name[0], sizeof(name[0]));
    strlcat(name[0], "cut_buffer", sizeof(name[0]));
    fluid_prefs.getUserdataPath(name[1], sizeof(name[1]));
    strlcat(name[1], "dup_buffer", sizeof(name[1]));
  }

  return name[which];
}

void save_cb(fltk3::Widget *, void *v) {
  const char *c = filename;
  if (v || !c || !*c) {
    fltk3::file_chooser_ok_label("Save");
    if (is_workspace())      
      c=fltk3::file_chooser("Save Workspace To:", "FLUID Workspace (*.flw)", c);
    else
      c=fltk3::file_chooser("Save UI Design To:", "FLUID Design (*.f[ld])", c);
    fltk3::file_chooser_ok_label(NULL);
    if (!c) return;

    if (!access(c, 0)) {
      const char *basename;
      if ((basename = strrchr(c, '/')) != NULL)
        basename ++;
#if defined(WIN32) || defined(__EMX__)
      if ((basename = strrchr(c, '\\')) != NULL)
        basename ++;
#endif // WIN32 || __EMX__
      else
        basename = c;

      if (fltk3::choice("The file \"%s\" already exists.\n"
                    "Do you want to replace it?", "Cancel",
		    "Replace", NULL, basename) == 0) return;
    }

    if (v != (void *)2) set_filename(c);
  }
  if (!write_file(c)) {
    fltk3::alert("Error writing %s: %s", c, strerror(errno));
    return;
  }

  if (v != (void *)2) {
    set_modflag(0);
    undo_save = undo_current;
  }
}

void save_template_cb(fltk3::Widget *, void *) {
  // Setup the template panel...
  if (!template_panel) make_template_panel();

  template_clear();
  template_browser->add("New Template");
  template_load();

  template_name->show();
  template_name->value("");

  template_instance->hide();

  template_delete->show();
  template_delete->deactivate();

  template_submit->label("Save");
  template_submit->deactivate();

  template_panel->label("Save Template");

  // Show the panel and wait for the user to do something...
  template_panel->show();
  while (template_panel->shown()) fltk3::wait();

  // Get the template name, return if it is empty...
  const char *c = template_name->value();
  if (!c || !*c) return;

  // Convert template name to filename_with_underscores
  char safename[FLTK3_PATH_MAX], *safeptr;
  strlcpy(safename, c, sizeof(safename));
  for (safeptr = safename; *safeptr; safeptr ++) {
    if (isspace(*safeptr)) *safeptr = '_';
  }

  // Find the templates directory...
  char filename[FLTK3_PATH_MAX];
  fluid_prefs.getUserdataPath(filename, sizeof(filename));

  strlcat(filename, "templates", sizeof(filename));
#if defined(WIN32) && !defined(__CYGWIN__)
  if (access(filename, 0)) mkdir(filename);
#else
  if (access(filename, 0)) mkdir(filename, 0777);
#endif // WIN32 && !__CYGWIN__

  strlcat(filename, "/", sizeof(filename));
  strlcat(filename, safename, sizeof(filename));

  char *ext = filename + strlen(filename);
  if (ext >= (filename + sizeof(filename) - 5)) {
    fltk3::alert("The template name \"%s\" is too long!", c);
    return;
  }

  // Save the .fl file...
  strcpy(ext, ".fl");

  if (!access(filename, 0)) {
    if (fltk3::choice("The template \"%s\" already exists.\n"
                  "Do you want to replace it?", "Cancel",
		  "Replace", NULL, c) == 0) return;
  }

  if (!write_file(filename)) {
    fltk3::alert("Error writing %s: %s", filename, strerror(errno));
    return;
  }

#if defined(HAVE_LIBPNG) && defined(HAVE_LIBZ)
  // Get the screenshot, if any...
  Fl_Type *t;

  for (t = Fl_Type::first; t; t = t->next) {
    // Find the first window...
    if (t->is_window()) break;
  }

  if (!t) return;

  // Grab a screenshot...
  Fl_Window_Type *wt = (Fl_Window_Type *)t;
  uchar *pixels;
  int w, h;

  if ((pixels = wt->read_image(w, h)) == NULL) return;

  // Save to a PNG file...
  strcpy(ext, ".png");

  FILE *fp;

  if ((fp = fltk3::fopen(filename, "wb")) == NULL) {
    delete[] pixels;
    fltk3::alert("Error writing %s: %s", filename, strerror(errno));
    return;
  }

  png_structp pptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  png_infop iptr = png_create_info_struct(pptr);
  png_bytep ptr = (png_bytep)pixels;

  png_init_io(pptr, fp);
  png_set_IHDR(pptr, iptr, w, h, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  png_set_sRGB(pptr, iptr, PNG_sRGB_INTENT_PERCEPTUAL);

  png_write_info(pptr, iptr);

  for (int i = h; i > 0; i --, ptr += w * 3) {
    png_write_row(pptr, ptr);
  }

  png_write_end(pptr, iptr);
  png_destroy_write_struct(&pptr, &iptr);

  fclose(fp);

#  if 0 // The original PPM output code...
  strcpy(ext, ".ppm");
  fp = fltk3::fopen(filename, "wb");
  fprintf(fp, "P6\n%d %d 255\n", w, h);
  fwrite(pixels, w * h, 3, fp);
  fclose(fp);
#  endif // 0

  delete[] pixels;
#endif // HAVE_LIBPNG && HAVE_LIBZ
}

void revert_cb(fltk3::Widget *,void *) {
  if (modflag) {
    if (!fltk3::choice("This user interface has been changed. Really revert?",
                   "Cancel", "Revert", NULL)) return;
  }
  undo_suspend();
  if (!read_file(filename, 0)) {
    undo_resume();
    fltk3::message("Can't read %s: %s", filename, strerror(errno));
    return;
  }
  undo_resume();
  set_modflag(0);
  undo_clear();
}

void exit_cb(fltk3::Widget *,void *) {
  if (modflag)
    switch (fltk3::choice("Do you want to save changes to this user\n"
                      "interface before exiting?", "Cancel",
                      "Save", "Don't Save"))
    {
      case 0 : /* Cancel */
          return;
      case 1 : /* Save */
          save_cb(NULL, NULL);
	  if (modflag) return;	// Didn't save!
    }

  save_position(main_window,"main_window_pos");

  if (widgetbin_panel) {
    save_position(widgetbin_panel,"widgetbin_pos");
    delete widgetbin_panel;
  }
  if (sourceview_panel) {
    fltk3::Preferences svp(fluid_prefs, "sourceview");
    svp.set("autorefresh", sv_autorefresh->value());
    svp.set("autoposition", sv_autoposition->value());
    svp.set("tab", sv_tab->find(sv_tab->value()));
    save_position(sourceview_panel,"sourceview_pos");
    delete sourceview_panel;
  }
  if (about_panel)
    delete about_panel;
  if (help_dialog)
    delete help_dialog;

  undo_clear();

  exit(0);
}

#ifdef __APPLE__
#  include <fltk3/x.h>

void
apple_open_cb(const char *c) {
  if (modflag) {
    switch (fltk3::choice("Do you want to save changes to this user\n"
                      "interface before opening another one?", "Don't Save",
                      "Save", "Cancel"))
    {
      case 0 : /* Cancel */
          return;
      case 1 : /* Save */
          save_cb(NULL, NULL);
	  if (modflag) return;	// Didn't save!
    }
  }
  const char *oldfilename;
  oldfilename = filename;
  filename    = NULL;
  set_filename(c);
  undo_suspend();
  if (!read_file(c, 0)) {
    undo_resume();
    fltk3::message("Can't read %s: %s", c, strerror(errno));
    free((void *)filename);
    filename = oldfilename;
    if (main_window) main_window->label(filename);
    return;
  }

  // Loaded a file; free the old filename...
  set_modflag(0);
  undo_resume();
  undo_clear();
  if (oldfilename) free((void *)oldfilename);
}
#endif // __APPLE__

void open_cb(fltk3::Widget *, void *v) {
  if (!v && modflag) {
    switch (fltk3::choice("Do you want to save changes to this user\n"
                      "interface before opening another one?", "Cancel",
                      "Save", "Don't Save"))
    {
      case 0 : /* Cancel */
          return;
      case 1 : /* Save */
          save_cb(NULL, NULL);
	  if (modflag) return;	// Didn't save!
    }
  }
  const char *c;
  const char *oldfilename;
  fltk3::file_chooser_ok_label("Open");
  c = fltk3::file_chooser("Open:", "FLUID Designs (*.f[ld])\tFLUID Workspaces (*.flw)", filename);
  fltk3::file_chooser_ok_label(NULL);
  if (!c) return;
  oldfilename = filename;
  filename    = NULL;
  set_filename(c);
  if (v != 0) undo_checkpoint();
  undo_suspend();
  if (!read_file(c, v!=0)) {
    undo_resume();
    fltk3::message("Can't read %s: %s", c, strerror(errno));
    free((void *)filename);
    filename = oldfilename;
    if (main_window) set_modflag(modflag);
    return;
  }
  undo_resume();
  if (v) {
    // Inserting a file; restore the original filename...
    free((void *)filename);
    filename = oldfilename;
    set_modflag(1);
  } else {
    // Loaded a file; free the old filename...
    set_modflag(0);
    undo_clear();
    if (oldfilename) free((void *)oldfilename);
  }
}

void open_history_cb(fltk3::Widget *, void *v) {
  if (modflag) {
    switch (fltk3::choice("Do you want to save changes to this user\n"
                      "interface before opening another one?", "Cancel",
                      "Save", "Don't Save"))
    {
      case 0 : /* Cancel */
          return;
      case 1 : /* Save */
          save_cb(NULL, NULL);
	  if (modflag) return;	// Didn't save!
    }
  }
  const char *oldfilename = filename;
  filename = NULL;
  set_filename((char *)v);
  undo_suspend();
  if (!read_file(filename, 0)) {
    undo_resume();
    undo_clear();
    fltk3::message("Can't read %s: %s", filename, strerror(errno));
    free((void *)filename);
    filename = oldfilename;
    if (main_window) main_window->label(filename);
    return;
  }
  set_modflag(0);
  undo_resume();
  undo_clear();
  if (oldfilename) free((void *)oldfilename);
}

void new_cb(fltk3::Widget *, void *v) {
  // Check if the current file has been modified...
  if (!v && modflag) {
    // Yes, ask the user what to do...
    switch (fltk3::choice("Do you want to save changes to this user\n"
                      "interface before creating a new one?", "Cancel",
                      "Save", "Don't Save"))
    {
      case 0 : /* Cancel */
          return;
      case 1 : /* Save */
          save_cb(NULL, NULL);
	  if (modflag) return;	// Didn't save!
    }
  }

  // Setup the template panel...
  if (!template_panel) make_template_panel();

  template_clear();
  template_browser->add("Blank");
  template_load();

  template_name->hide();
  template_name->value("");

  template_instance->show();
  template_instance->deactivate();
  template_instance->value("");

  template_delete->hide();

  template_submit->label("New");
  template_submit->deactivate();

  template_panel->label("New");

  // Show the panel and wait for the user to do something...
  template_panel->show();
  while (template_panel->shown()) fltk3::wait();

  // See if the user chose anything...
  int item = template_browser->value();
  if (item < 1) return;

  // Clear the current data...
  delete_all();
  set_filename(NULL);

  // Load the template, if any...
  const char *tname = (const char *)template_browser->data(item);

  if (tname) {
    // Grab the instance name...
    const char *iname = template_instance->value();

    if (iname && *iname) {
      // Copy the template to a temp file, then read it in...
      char line[1024], *ptr, *next;
      FILE *infile, *outfile;

      if ((infile = fltk3::fopen(tname, "r")) == NULL) {
	fltk3::alert("Error reading template file \"%s\":\n%s", tname,
        	 strerror(errno));
	set_modflag(0);
	undo_clear();
	return;
      }

      if ((outfile = fltk3::fopen(cutfname(1), "w")) == NULL) {
	fltk3::alert("Error writing buffer file \"%s\":\n%s", cutfname(1),
        	 strerror(errno));
	fclose(infile);
	set_modflag(0);
	undo_clear();
	return;
      }

      while (fgets(line, sizeof(line), infile)) {
	// Replace @INSTANCE@ with the instance name...
	for (ptr = line; (next = strstr(ptr, "@INSTANCE@")) != NULL; ptr = next + 10) {
	  fwrite(ptr, next - ptr, 1, outfile);
	  fputs(iname, outfile);
	}

	fputs(ptr, outfile);
      }

      fclose(infile);
      fclose(outfile);

      undo_suspend();
      read_file(cutfname(1), 0);
      unlink(cutfname(1));
      undo_resume();
    } else {
      // No instance name, so read the template without replacements...
      undo_suspend();
      read_file(tname, 0);
      undo_resume();
    }
  }

  set_modflag(0);
  undo_clear();
}

int exit_early = 0;
int compile_only = 0;
int compile_strings = 0;
int header_file_set = 0;
int code_file_set = 0;
const char* header_file_name = ".h";
const char* code_file_name = ".cxx";
int i18n_type = 0;
const char* i18n_include = "";
const char* i18n_function = "";
const char* i18n_file = "";
const char* i18n_set = "";
char i18n_program[FLTK3_PATH_MAX] = "";

void write_cb(fltk3::Widget *, void *) {
  if (!filename) {
    save_cb(0,0);
    if (!filename) return;
  }
  char cname[FLTK3_PATH_MAX];
  char hname[FLTK3_PATH_MAX];
  strlcpy(i18n_program, fltk3::filename_name(filename), sizeof(i18n_program));
  fltk3::filename_setext(i18n_program, sizeof(i18n_program), "");
  if (*code_file_name == '.' && strchr(code_file_name, '/') == NULL) {
    strlcpy(cname, fltk3::filename_name(filename), sizeof(cname));
    fltk3::filename_setext(cname, sizeof(cname), code_file_name);
  } else {
    strlcpy(cname, code_file_name, sizeof(hname));
  }
  if (*header_file_name == '.' && strchr(header_file_name, '/') == NULL) {
    strlcpy(hname, fltk3::filename_name(filename), sizeof(hname));
    fltk3::filename_setext(hname, sizeof(hname), header_file_name);
  } else {
    strlcpy(hname, header_file_name, sizeof(hname));
  }
  if (!compile_only) goto_source_dir();
  int x = write_code(cname,hname);
  if (!compile_only) leave_source_dir();
  strlcat(cname, " and ", sizeof(cname));
  strlcat(cname, hname, sizeof(cname));
  if (compile_only) {
    if (!x) {fprintf(stderr,"%s : %s\n",cname,strerror(errno)); exit(1);}
  } else {
    if (!x) {
      fltk3::message("Can't write %s: %s", cname, strerror(errno));
    } else if (completion_button->value()) {
      fltk3::message("Wrote %s", cname);
    }
  }
}

void write_strings_cb(fltk3::Widget *, void *) {
  static const char *exts[] = { ".txt", ".po", ".msg" };
  if (!filename) {
    save_cb(0,0);
    if (!filename) return;
  }
  char sname[FLTK3_PATH_MAX];
  strlcpy(sname, fltk3::filename_name(filename), sizeof(sname));
  fltk3::filename_setext(sname, sizeof(sname), exts[i18n_type]);
  if (!compile_only) goto_source_dir();
  int x = write_strings(sname);
  if (!compile_only) leave_source_dir();
  if (compile_only) {
    if (x) {fprintf(stderr,"%s : %s\n",sname,strerror(errno)); exit(1);}
  } else {
    if (x) {
      fltk3::message("Can't write %s: %s", sname, strerror(errno));
    } else if (completion_button->value()) {
      fltk3::message("Wrote %s", sname);
    }
  }
}

void openwidget_cb(fltk3::Widget *, void *) {
  if (!Fl_Type::current) {
    fltk3::message("Please select a widget");
    return;
  }
  Fl_Type::current->open();
}

void toggle_overlays(fltk3::Widget *,void *);

void select_all_cb(fltk3::Widget *,void *);
void select_none_cb(fltk3::Widget *,void *);

void group_cb(fltk3::Widget *, void *);

void ungroup_cb(fltk3::Widget *, void *);

extern int pasteoffset;
static int ipasteoffset;

void copy_cb(fltk3::Widget*, void*) {
  if (!Fl_Type::current) {
    fltk3::beep();
    return;
  }
  ipasteoffset = 10;
  if (!write_file(cutfname(),1)) {
    fltk3::message("Can't write %s: %s", cutfname(), strerror(errno));
    return;
  }
}

extern void select_only(Fl_Type *);
void cut_cb(fltk3::Widget *, void *) {
  if (!Fl_Type::current) {
    fltk3::beep();
    return;
  }
  if (!write_file(cutfname(),1)) {
    fltk3::message("Can't write %s: %s", cutfname(), strerror(errno));
    return;
  }
  undo_checkpoint();
  set_modflag(1);
  ipasteoffset = 0;
  Fl_Type *p = Fl_Type::current->parent;
  while (p && p->selected) p = p->parent;
  delete_all(1);
  if (p) select_only(p);
}

void delete_cb(fltk3::Widget *, void *) {
  if (!Fl_Type::current) {
    fltk3::beep();
    return;
  }
  undo_checkpoint();
  set_modflag(1);
  ipasteoffset = 0;
  Fl_Type *p = Fl_Type::current->parent;
  while (p && p->selected) p = p->parent;
  delete_all(1);
  if (p) select_only(p);
}

extern int force_parent;

void paste_cb(fltk3::Widget*, void*) {
  //if (ipasteoffset) force_parent = 1;
  pasteoffset = ipasteoffset;
  if (gridx>1) pasteoffset = ((pasteoffset-1)/gridx+1)*gridx;
  if (gridy>1) pasteoffset = ((pasteoffset-1)/gridy+1)*gridy;
  undo_checkpoint();
  undo_suspend();
  if (!read_file(cutfname(), 1)) {
    fltk3::message("Can't read %s: %s", cutfname(), strerror(errno));
  }
  undo_resume();
  pasteoffset = 0;
  ipasteoffset += 10;
  force_parent = 0;
}

// Duplicate the selected widgets...
void duplicate_cb(fltk3::Widget*, void*) {
  if (!Fl_Type::current) {
    fltk3::beep();
    return;
  }

  if (!write_file(cutfname(1),1)) {
    fltk3::message("Can't write %s: %s", cutfname(1), strerror(errno));
    return;
  }

  pasteoffset  = 0;
  force_parent = 1;

  undo_checkpoint();
  undo_suspend();
  if (!read_file(cutfname(1), 1)) {
    fltk3::message("Can't read %s: %s", cutfname(1), strerror(errno));
  }
  unlink(cutfname(1));
  undo_resume();

  force_parent = 0;
}

void earlier_cb(fltk3::Widget*,void*);

void later_cb(fltk3::Widget*,void*);

Fl_Type *sort(Fl_Type *parent);

static void sort_cb(fltk3::Widget *,void *) {
  sort((Fl_Type*)0);
}

void show_project_cb(fltk3::Widget *, void *);
void show_grid_cb(fltk3::Widget *, void *);
void show_settings_cb(fltk3::Widget *, void *);
void show_global_settings_cb(fltk3::Widget *, void *);

void align_widget_cb(fltk3::Widget *, long);
void widget_size_cb(fltk3::Widget *, long);

void about_cb(fltk3::Widget *, void *) {
  if (!about_panel) make_about_panel();
  about_panel->show();
}

void show_help(const char *name) {
  const char	*docdir;
  char		helpname[FLTK3_PATH_MAX];

  if (!help_dialog) help_dialog = new fltk3::HelpDialog();

  if ((docdir = getenv("FLTK_DOCDIR")) == NULL) {
#ifdef __EMX__
    // Doesn't make sense to have a hardcoded fallback
    static char fltk_docdir[FLTK3_PATH_MAX];

    strlcpy(fltk_docdir, __XOS2RedirRoot("/XFree86/lib/X11/fltk/doc"),
            sizeof(fltk_docdir));

    docdir = fltk_docdir;
#else
    docdir = FLTK_DOCDIR;
#endif // __EMX__
  }
  snprintf(helpname, sizeof(helpname), "%s/%s", docdir, name);

  // make sure that we can read the file
  FILE *f = fopen(helpname, "rb");
  if (f) {
    fclose(f);
    help_dialog->load(helpname);
  } else {
    // if we can not read the file, we display the canned version instead
    // or ask the native browser to open the page on www.fltk.org
    if (strcmp(name, "fluid.html")==0) {
      if (!fltk3::SharedImage::find("embedded:/fluid-org.png"))
        new fltk3::PNGImage("embedded:/fluid-org.png", fluid_org_png, sizeof(fluid_org_png));
      help_dialog->value
      (
       "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
       "<html><head><title>FLTK: Programming with FLUID</title></head><body>\n"
       "<h2>What is FLUID?</h2>\n"
       "The Fast Light User Interface Designer, or FLUID, is a graphical editor "
       "that is used to produce FLTK source code. FLUID edits and saves its state "
       "in <code>.fl</code> files. These files are text, and you can (with care) "
       "edit them in a text editor, perhaps to get some special effects.<p>\n"
       "FLUID can \"compile\" the <code>.fl</code> file into a <code>.cxx</code> "
       "and a <code>.h</code> file. The <code>.cxx</code> file defines all the "
       "objects from the <code>.fl</code> file and the <code>.h</code> file "
       "declares all the global ones. FLUID also supports localization "
       "(Internationalization) of label strings using message files and the GNU "
       "gettext or POSIX catgets interfaces.<p>\n"
       "A simple program can be made by putting all your code (including a <code>"
       "main()</code> function) into the <code>.fl</code> file and thus making the "
       "<code>.cxx</code> file a single source file to compile. Most programs are "
       "more complex than this, so you write other <code>.cxx</code> files that "
       "call the FLUID functions. These <code>.cxx</code> files must <code>"
       "#include</code> the <code>.h</code> file or they can <code>#include</code> "
       "the <code>.cxx</code> file so it still appears to be a single source file.<p>"
       "<img src=\"embedded:/fluid-org.png\"></p>"
       "<p>More information is available online at <a href="
       "\"http://www.fltk.org/doc-1.3/fluid.html\">http://www.fltk.org/</a>"
       "</body></html>"
       );
    } else if (strcmp(name, "license.html")==0) {
      fltk3::open_uri("http://www.fltk.org/doc-1.3/license.html");
      return;
    } else if (strcmp(name, "index.html")==0) {
      fltk3::open_uri("http://www.fltk.org/doc-1.3/index.html");
      return;
    } else {
      snprintf(helpname, sizeof(helpname), "http://www.fltk.org/%s", name);
      fltk3::open_uri(helpname);
      return;
    }
  }
  help_dialog->show();
}

void help_cb(fltk3::Widget *, void *) {
  show_help("fluid.html");
}

void manual_cb(fltk3::Widget *, void *) {
  show_help("index.html");
}

// ------------ file conversion ------------------------------------------------

extern int write_fltk_makefiles();
extern int write_fltk_cmake();
extern int write_fltk_ide_xcode4();
extern int write_fltk_ide_visualc6();
extern int write_fltk_ide_visualc2008();
extern int write_fltk_ide_visualc2010();

void write_makefiles_cb(fltk3::Widget*, void*) {
  // make sure that our workspace file has a proper file name and path
  if (!filename) {
    save_cb(0, 0);
    if (!filename)
      return;
  }
  // TODO: we need workspace settings that declare which of these systems will be written
  // TODO: we need to collect error messages and output them
  if ((wks_env&FL_ENV_MAKE) && write_fltk_makefiles()) {
    int v = fltk3::choice("Error writing Makefile build system", "Cancel", "Continue", 0);
    if (v==0) return;
  }
  if ((wks_env&FL_ENV_CMAKE) && write_fltk_cmake()) {
    int v = fltk3::choice("Error writing Makefile build system", "Cancel", "Continue", 0);
    if (v==0) return;
  }
  if ((wks_env&FL_ENV_XC4) && write_fltk_ide_xcode4()) {
    int v = fltk3::choice("Error writing Xcode 4 build system", "Cancel", "Continue", 0);
    if (v==0) return;
  }
  if ((wks_env&FL_ENV_VC6) && write_fltk_ide_visualc6()) {
    int v = fltk3::choice("Error writing VisualC 6 build system", "Cancel", "Continue", 0);
    if (v==0) return;
  }
  if ((wks_env&FL_ENV_VC2008) && write_fltk_ide_visualc2008()) {
    int v = fltk3::choice("Error writing VisualC 2008 build system", "Cancel", "Continue", 0);
    if (v==0) return;
  }
  if ((wks_env&FL_ENV_VC2010) && write_fltk_ide_visualc2010()) {
    int v = fltk3::choice("Error writing VisualC 2010 build system", "Cancel", "Continue", 0);
    if (v==0) return;
  }
}


////////////////////////////////////////////////////////////////

void print_menu_cb(fltk3::Widget *, void *) {
  Fl_Window_Type *windows[1000];
  int num_windows = 0, winpage = 0;
  int	fontsize = 14; // assumin 72 dpi
  int width, height;
  char buffer[1024];
  Fl_Type *t;
  
  const char *basename = strrchr(filename, '/');
  if (basename) basename ++;
  else basename = filename;

  // Get the time and date...
  time_t curtime = time(NULL);
  struct tm *curdate = localtime(&curtime);
  char date[1024];
  strftime(date, sizeof(date), "%c", curdate);

  // count pages
  for (t = Fl_Type::first; t; t = t->next) {
    if (t->is_window()) {
      windows[num_windows] = (Fl_Window_Type *)t;
      num_windows ++;
    }
  }
  
  // print pages
  fltk3::Printer * p = new fltk3::Printer();
  if (!p->start_job(num_windows)) {
    // Print each of the windows...
    for (winpage = 0; winpage < num_windows; winpage ++) {
      p->start_page();
      p->printable_rect(&width, &height);
      
      // draw a headline
      fltk3::font(fltk3::HELVETICA_BOLD, fontsize);
      fltk3::color(0, 0, 0);
      fltk3::draw(basename, 0, fontsize);
      fltk3::draw(date, (width - (int)fltk3::width(date)) / 2, fontsize);      
      sprintf(buffer, "%d/%d", winpage + 1, num_windows);
      fltk3::draw(buffer, width - (int)fltk3::width(buffer), fontsize);
      
      // find a good size for printing
      fltk3::Widget *w = windows[winpage]->o;
      int ww = w->w(), hh = w->h();
      double sx = (0.95*width)/ww;
      double sy = (0.95*(height-30))/hh;
      double scale = (sx<sy)?sx:sy;
      if (scale>1.0) scale = 1.0;
      
      // set the scale, and center the image at the top
      p->scale(scale, scale);
      p->printable_rect(&width, &height);
      p->origin(0.5*(width-ww), 30/scale);
      
      // draw the widget
      p->print_widget(w);
      
      // reset stuff (probably not needed)
      p->scale(1.0, 1.0);
      p->origin(0.0, 0.0);
      
      // flush the page out
      p->end_page();
    }
    p->end_job();
  }
  delete p;
}

////////////////////////////////////////////////////////////////

extern fltk3::MenuItem New_Menu[];

void toggle_widgetbin_cb(fltk3::Widget *, void *);
void toggle_sourceview_cb(fltk3::DoubleWindow *, void *);
void convert_1_to_3_cb(fltk3::Widget *, void *);
void convert_2_to_3_cb(fltk3::Widget *, void *);
void write_makefiles_cb(fltk3::Widget *, void *);

extern fltk3::DoubleWindow* show_workspace_panel();
void workspace_settings_cb(fltk3::Widget*, void*) {
  show_workspace_panel();
}

fltk3::MenuItem Main_Menu[] = {
{"&File",0,0,0,fltk3::SUBMENU},
  {"&New...", fltk3::COMMAND+'n', new_cb, 0},
  {"&Open...", fltk3::COMMAND+'o', open_cb, 0},
  {"&Insert...", fltk3::COMMAND+'i', open_cb, (void*)1, fltk3::MENU_DIVIDER},
  {"&Save", fltk3::COMMAND+'s', save_cb, 0},
  {"Save &As...", fltk3::COMMAND+fltk3::SHIFT+'s', save_cb, (void*)1},
  {"Sa&ve A Copy...", 0, save_cb, (void*)2},
  {"Save &Template...", 0, save_template_cb},
  {"&Revert...", 0, revert_cb, 0, fltk3::MENU_DIVIDER},
  {"&Print...", fltk3::COMMAND+'p', print_menu_cb},
  {"Write &Code...", fltk3::COMMAND+fltk3::SHIFT+'c', write_cb, 0},
  {"&Write Strings...", fltk3::COMMAND+fltk3::SHIFT+'w', write_strings_cb, 0, fltk3::MENU_DIVIDER},
  {relative_history[0], fltk3::COMMAND+'0', open_history_cb, absolute_history[0]},
  {relative_history[1], fltk3::COMMAND+'1', open_history_cb, absolute_history[1]},
  {relative_history[2], fltk3::COMMAND+'2', open_history_cb, absolute_history[2]},
  {relative_history[3], fltk3::COMMAND+'3', open_history_cb, absolute_history[3]},
  {relative_history[4], fltk3::COMMAND+'4', open_history_cb, absolute_history[4]},
  {relative_history[5], fltk3::COMMAND+'5', open_history_cb, absolute_history[5]},
  {relative_history[6], fltk3::COMMAND+'6', open_history_cb, absolute_history[6]},
  {relative_history[7], fltk3::COMMAND+'7', open_history_cb, absolute_history[7]},
  {relative_history[8], fltk3::COMMAND+'8', open_history_cb, absolute_history[8]},
  {relative_history[9], fltk3::COMMAND+'9', open_history_cb, absolute_history[9], fltk3::MENU_DIVIDER},
  {"&Quit", fltk3::COMMAND+'q', exit_cb},
  {0},
{"&Edit",0,0,0,fltk3::SUBMENU},
  {"&Undo", fltk3::COMMAND+'z', undo_cb},
  {"&Redo", fltk3::COMMAND+fltk3::SHIFT+'z', redo_cb, 0, fltk3::MENU_DIVIDER},
  {"C&ut", fltk3::COMMAND+'x', cut_cb},
  {"&Copy", fltk3::COMMAND+'c', copy_cb},
  {"&Paste", fltk3::COMMAND+'v', paste_cb},
  {"Dup&licate", fltk3::COMMAND+'u', duplicate_cb},
  {"&Delete", fltk3::DeleteKey, delete_cb, 0, fltk3::MENU_DIVIDER},
  {"Select &All", fltk3::COMMAND+'a', select_all_cb},
  {"Select &None", fltk3::COMMAND+fltk3::SHIFT+'a', select_none_cb, 0, fltk3::MENU_DIVIDER},
  {"Pr&operties...", fltk3::FKey+1, openwidget_cb},
  {"&Sort",0,sort_cb},
  {"&Earlier", fltk3::FKey+2, earlier_cb},
  {"&Later", fltk3::FKey+3, later_cb},
  {"&Group", fltk3::FKey+7, group_cb},
  {"Ung&roup", fltk3::FKey+8, ungroup_cb,0, fltk3::MENU_DIVIDER},
  {"Hide O&verlays",fltk3::COMMAND+fltk3::SHIFT+'o',toggle_overlays},
  {"Show Widget &Bin...",fltk3::ALT+'b',toggle_widgetbin_cb},
  {"Show Source Code...",fltk3::ALT+fltk3::SHIFT+'s', (fltk3::Callback*)toggle_sourceview_cb, 0, fltk3::MENU_DIVIDER},
  {"Pro&ject Settings...",fltk3::ALT+'p',show_project_cb},
  {"GU&I Settings...",fltk3::ALT+fltk3::SHIFT+'p',show_settings_cb,0,fltk3::MENU_DIVIDER},
  {"Global &FLTK Settings...",fltk3::ALT+fltk3::SHIFT+'g',show_global_settings_cb},
  {0},
{"&New", 0, 0, (void *)New_Menu, fltk3::SUBMENU_POINTER},
{"&Layout",0,0,0,fltk3::SUBMENU},
  {"&Align",0,0,0,fltk3::SUBMENU},
    {"&Left",0,(fltk3::Callback *)align_widget_cb,(void*)10},
    {"&Center",0,(fltk3::Callback *)align_widget_cb,(void*)11},
    {"&Right",0,(fltk3::Callback *)align_widget_cb,(void*)12},
    {"&Top",0,(fltk3::Callback *)align_widget_cb,(void*)13},
    {"&Middle",0,(fltk3::Callback *)align_widget_cb,(void*)14},
    {"&Bottom",0,(fltk3::Callback *)align_widget_cb,(void*)15},
    {0},
  {"&Space Evenly",0,0,0,fltk3::SUBMENU},
    {"&Across",0,(fltk3::Callback *)align_widget_cb,(void*)20},
    {"&Down",0,(fltk3::Callback *)align_widget_cb,(void*)21},
    {0},
  {"&Make Same Size",0,0,0,fltk3::SUBMENU},
    {"&Width",0,(fltk3::Callback *)align_widget_cb,(void*)30},
    {"&Height",0,(fltk3::Callback *)align_widget_cb,(void*)31},
    {"&Both",0,(fltk3::Callback *)align_widget_cb,(void*)32},
    {0},
  {"&Center In Group",0,0,0,fltk3::SUBMENU},
    {"&Horizontal",0,(fltk3::Callback *)align_widget_cb,(void*)40},
    {"&Vertical",0,(fltk3::Callback *)align_widget_cb,(void*)41},
    {0},
  {"Set &Widget Size",0,0,0,fltk3::SUBMENU|fltk3::MENU_DIVIDER},
    {"&Tiny",fltk3::ALT+'1',(fltk3::Callback *)widget_size_cb,(void*)8,0,fltk3::NORMAL_LABEL,fltk3::HELVETICA,8},
    {"&Small",fltk3::ALT+'2',(fltk3::Callback *)widget_size_cb,(void*)11,0,fltk3::NORMAL_LABEL,fltk3::HELVETICA,11},
    {"&Normal",fltk3::ALT+'3',(fltk3::Callback *)widget_size_cb,(void*)14,0,fltk3::NORMAL_LABEL,fltk3::HELVETICA,14},
    {"&Medium",fltk3::ALT+'4',(fltk3::Callback *)widget_size_cb,(void*)18,0,fltk3::NORMAL_LABEL,fltk3::HELVETICA,18},
    {"&Large",fltk3::ALT+'5',(fltk3::Callback *)widget_size_cb,(void*)24,0,fltk3::NORMAL_LABEL,fltk3::HELVETICA,24},
    {"&Huge",fltk3::ALT+'6',(fltk3::Callback *)widget_size_cb,(void*)32,0,fltk3::NORMAL_LABEL,fltk3::HELVETICA,32},
    {0},
  {"&Grid and Size Settings...",fltk3::COMMAND+'g',show_grid_cb},
  {0},
{"&Shell",0,0,0,fltk3::SUBMENU},
  {"Execute &Command...",fltk3::ALT+'x',(fltk3::Callback *)show_shell_window},
  {"Execute &Again...",fltk3::ALT+'g',(fltk3::Callback *)do_shell_command,0,fltk3::MENU_DIVIDER},
  {"Workspace",0,0,0,fltk3::SUBMENU},
    {"Write Makefile",fltk3::COMMAND+'m',write_makefiles_cb},
    {"Settings...",0,workspace_settings_cb},
    {0},
  {"Build Application",fltk3::COMMAND+'b',(fltk3::Callback *)0L},
  {"Run Application",fltk3::COMMAND+'r',(fltk3::Callback *)0L},
  {"Convert", 0, 0, 0, fltk3::SUBMENU},
    {"FLTK 1 to 3", 0, convert_1_to_3_cb},
    {"FLTK 2 to 3", 0, convert_2_to_3_cb},
    {0},
  {0},
{"&Help",0,0,0,fltk3::SUBMENU},
  {"&Rapid development with FLUID...",0,help_cb},
  {"&FLTK Programmers Manual...",0,manual_cb, 0, fltk3::MENU_DIVIDER},
  {"&About FLUID...",0,about_cb},
  {0},
{0}};

#define BROWSERWIDTH 300
#define BROWSERHEIGHT 500
#define WINWIDTH 300
#define MENUHEIGHT 25
#define WINHEIGHT (BROWSERHEIGHT+MENUHEIGHT)

extern void fill_in_New_Menu();

void scheme_cb(fltk3::Choice *, void *) {
  if (compile_only)
    return;

  switch (scheme_choice->value()) {
    case 0 : // Default
      fltk3::scheme(NULL);
      break;
    case 1 : // None
      fltk3::scheme("none");
      break;
    case 2 : // Plastic
      fltk3::scheme("plastic");
      break;
    case 3 : // GTK+
      fltk3::scheme("gtk+");
      break;
  }

  fluid_prefs.set("scheme", scheme_choice->value());
}

void toggle_widgetbin_cb(fltk3::Widget *, void *) {
  if (!widgetbin_panel) {
    make_widgetbin();
    if (!position_window(widgetbin_panel,"widgetbin_pos", 1, 320, 30)) return;
  }

  if (widgetbin_panel->visible()) {
    widgetbin_panel->hide();
    widgetbin_item->label("Show Widget &Bin...");
  } else {
    widgetbin_panel->show();
    widgetbin_item->label("Hide Widget &Bin");
  }
}


void toggle_sourceview_cb(fltk3::DoubleWindow *, void *) {
  if (!sourceview_panel) {
    make_sourceview();
    sourceview_panel->callback((fltk3::Callback*)toggle_sourceview_cb);
    fltk3::Preferences svp(fluid_prefs, "sourceview");
    int autorefresh;
    svp.get("autorefresh", autorefresh, 1);
    sv_autorefresh->value(autorefresh);
    int autoposition;
    svp.get("autoposition", autoposition, 1);
    sv_autoposition->value(autoposition);
    int tab;
    svp.get("tab", tab, 0);
    if (tab>=0 && tab<sv_tab->children()) sv_tab->value(sv_tab->child(tab));
    if (!position_window(sourceview_panel,"sourceview_pos", 0, 320, 120, 550, 500)) return;
  }

  if (sourceview_panel->visible()) {
    sourceview_panel->hide();
    sourceview_item->label("Show Source Code...");
  } else {
    sourceview_panel->show();
    sourceview_item->label("Hide Source Code...");
    update_sourceview_cb(0,0);
  }
}

void toggle_sourceview_b_cb(fltk3::Button*, void *) {
  toggle_sourceview_cb(0,0);
}

void make_main_window() {
  if (!compile_only) {
    fluid_prefs.get("snap", snap, 1);
    fluid_prefs.get("gridx", gridx, 5);
    fluid_prefs.get("gridy", gridy, 5);
    fluid_prefs.get("show_guides", show_guides, 0);
    fluid_prefs.get("widget_size", Fl_Widget_Type::default_size, 14);
    fluid_prefs.get("show_comments", show_comments, 1);
    make_layout_window();
    make_shell_window();
  }

  if (!main_window) {
    fltk3::Widget *o;
    main_window = new fltk3::DoubleWindow(WINWIDTH,WINHEIGHT,"fluid");
    main_window->box(fltk3::NO_BOX);
    o = make_widget_browser(0,MENUHEIGHT,BROWSERWIDTH,BROWSERHEIGHT);
    o->box(fltk3::FLAT_BOX);
    o->tooltip("Double-click to view or change an item.");
    main_window->resizable(o);
    main_menubar = new fltk3::MenuBar(0,0,BROWSERWIDTH,MENUHEIGHT);
    main_menubar->menu(Main_Menu);
    // quick access to all dynamic menu items
    save_item = (fltk3::MenuItem*)main_menubar->find_item(save_cb);
    history_item = (fltk3::MenuItem*)main_menubar->find_item(open_history_cb);
    widgetbin_item = (fltk3::MenuItem*)main_menubar->find_item(toggle_widgetbin_cb);
    sourceview_item = (fltk3::MenuItem*)main_menubar->find_item((fltk3::Callback*)toggle_sourceview_cb);
    main_menubar->global();
    fill_in_New_Menu();
    main_window->end();
  }

  if (!compile_only) {
    load_history();
    make_settings_window();
    make_global_settings_window();
  }
}

// Load file history from preferences...
void load_history() {
  int	i;		// Looping var
  int	max_files;


  fluid_prefs.get("recent_files", max_files, 5);
  if (max_files > 10) max_files = 10;

  for (i = 0; i < max_files; i ++) {
    fluid_prefs.get( fltk3::Preferences::Name("file%d", i), absolute_history[i], "", sizeof(absolute_history[i]));
    if (absolute_history[i][0]) {
      // Make a relative version of the filename for the menu...
      fltk3::filename_relative(relative_history[i], sizeof(relative_history[i]),
                           absolute_history[i]);

      if (i == 9) history_item[i].flags = fltk3::MENU_DIVIDER;
      else history_item[i].flags = 0;
    } else break;
  }

  for (; i < 10; i ++) {
    if (i) history_item[i-1].flags |= fltk3::MENU_DIVIDER;
    history_item[i].hide();
  }
}

// Update file history from preferences...
void update_history(const char *flname) {
  int	i;		// Looping var
  char	absolute[FLTK3_PATH_MAX];
  int	max_files;


  fluid_prefs.get("recent_files", max_files, 5);
  if (max_files > 10) max_files = 10;

  fltk3::filename_absolute(absolute, sizeof(absolute), flname);

  for (i = 0; i < max_files; i ++)
#if defined(WIN32) || defined(__APPLE__)
    if (!strcasecmp(absolute, absolute_history[i])) break;
#else
    if (!strcmp(absolute, absolute_history[i])) break;
#endif // WIN32 || __APPLE__

  if (i == 0) return;

  if (i >= max_files) i = max_files - 1;

  // Move the other flnames down in the list...
  memmove(absolute_history + 1, absolute_history,
          i * sizeof(absolute_history[0]));
  memmove(relative_history + 1, relative_history,
          i * sizeof(relative_history[0]));

  // Put the new file at the top...
  strlcpy(absolute_history[0], absolute, sizeof(absolute_history[0]));

  fltk3::filename_relative(relative_history[0], sizeof(relative_history[0]),
                       absolute_history[0]);

  // Update the menu items as needed...
  for (i = 0; i < max_files; i ++) {
    fluid_prefs.set( fltk3::Preferences::Name("file%d", i), absolute_history[i]);
    if (absolute_history[i][0]) {
      if (i == 9) history_item[i].flags = fltk3::MENU_DIVIDER;
      else history_item[i].flags = 0;
    } else break;
  }

  for (; i < 10; i ++) {
    fluid_prefs.set( fltk3::Preferences::Name("file%d", i), "");
    if (i) history_item[i-1].flags |= fltk3::MENU_DIVIDER;
    history_item[i].hide();
  }
}

// ********** portable process class definition **********

class Fl_Process {
public:
  // construction / destruction
  Fl_Process() {_fpt= NULL;}
  ~Fl_Process() {if (_fpt) close();}

  // FIXME: popen needs the utf8 equivalen fl_popen
  FILE * popen	(const char *cmd, const char *mode="r");
  //not necessary here: FILE * fltk3::fopen	(const char *file, const char *mode="r");
  int  close();

  FILE * desc() const { return _fpt;} // non null if file is open
  char * get_line(char * line, size_t s) const {return _fpt ? fgets(line, s, _fpt) : NULL;}

#if defined(WIN32)  && !defined(__CYGWIN__)
protected:
  HANDLE pin[2], pout[2], perr[2];
  char ptmode;
  PROCESS_INFORMATION pi;
  STARTUPINFO si;

  static bool createPipe(HANDLE * h, BOOL bInheritHnd=TRUE);

private:
  FILE * freeHandles()  {
    clean_close(pin[0]);    clean_close(pin[1]);
    clean_close(pout[0]);   clean_close(pout[1]);
    clean_close(perr[0]);   clean_close(perr[1]);
    return NULL; // convenient for error management
  }
  static void clean_close(HANDLE& h);
#endif

protected:
  FILE * _fpt;
};

#if defined(WIN32)  && !defined(__CYGWIN__)
bool Fl_Process::createPipe(HANDLE * h, BOOL bInheritHnd) {
  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof(sa);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle = bInheritHnd;
  return CreatePipe (&h[0],&h[1],&sa,0) ? true : false;
}
#endif
// portable open process:
FILE * Fl_Process::popen(const char *cmd, const char *mode) {
#if defined(WIN32)  && !defined(__CYGWIN__)
  // PRECONDITIONS
  if (!mode || !*mode || (*mode!='r' && *mode!='w') ) return NULL;
  if (_fpt) close(); // close first before reuse

  ptmode = *mode;
  pin[0] = pin[1] = pout[0] = pout[1] = perr[0] = perr[1] = INVALID_HANDLE_VALUE;
  // stderr to stdout wanted ?
  int fusion = (strstr(cmd,"2>&1") !=NULL);

  // Create windows pipes
  if (!createPipe(pin) || !createPipe(pout) || (!fusion && !createPipe(perr) ) )
	return freeHandles(); // error

  // Initialize Startup Info
  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb           = sizeof(STARTUPINFO);
  si.dwFlags    = STARTF_USESTDHANDLES;
  si.hStdInput    = pin[0];
  si.hStdOutput   = pout[1];
  si.hStdError  = fusion ? pout[1] : perr [1];

  if ( CreateProcess(NULL, (LPTSTR) cmd,NULL,NULL,TRUE,
		     DETACHED_PROCESS,NULL,NULL, &si, &pi)) {
    // don't need theses handles inherited by child process:
    clean_close(pin[0]); clean_close(pout[1]); clean_close(perr[1]);
    HANDLE & h = *mode == 'r' ? pout[0] : pin[1];
    _fpt = _fdopen(_open_osfhandle((fl_intptr_t) h,_O_BINARY),mode);
    h= INVALID_HANDLE_VALUE;  // reset the handle pointer that is shared
    // with _fpt so we don't free it twice
  }

  if (!_fpt)  freeHandles();
  return _fpt;
#else
  _fpt=::popen(cmd,mode);
  return _fpt;
#endif
}

int Fl_Process::close() {
#if defined(WIN32)  && !defined(__CYGWIN__)
  if (_fpt) {
    fclose(_fpt);
    clean_close(perr[0]);
    clean_close(pin[1]);
    clean_close(pout[0]);
    _fpt = NULL;
    return 0;
  }
  return -1;
#else
  int ret = ::pclose(_fpt);
  _fpt=NULL;
  return ret;
#endif
}

#if defined(WIN32)  && !defined(__CYGWIN__)
void Fl_Process::clean_close(HANDLE& h) {
  if (h!= INVALID_HANDLE_VALUE) CloseHandle(h);
  h = INVALID_HANDLE_VALUE;
}
#endif
// ********** Fl_Process class end **********

static Fl_Process s_proc;

// Shell command support...

static bool prepare_shell_command(const char * &command)  { // common pre-shell command code all platforms
  shell_window->hide();
  if (s_proc.desc()) {
    fltk3::alert("Previous shell command still running!");
    return false;
  }
  if ((command = shell_command_input->value()) == NULL || !*command) {
    fltk3::alert("No shell command entered!");
    return false;
  }
  if (shell_savefl_button->value()) {
    save_cb(0, 0);
  }
  if (shell_writecode_button->value()) {
    compile_only = 1;
    write_cb(0, 0);
    compile_only = 0;
  }
  if (shell_writemsgs_button->value()) {
    compile_only = 1;
    write_strings_cb(0, 0);
    compile_only = 0;
  }
  return true;
}

#if !defined(__MWERKS__)
// Support the full piped shell command...
void
shell_pipe_cb(int, void*) {
  char	line[1024]="";		// Line from command output...

  if (s_proc.get_line(line, sizeof(line)) != NULL) {
    // Add the line to the output list...
    shell_run_buffer->append(line);
  } else {
    // End of file; tell the parent...
    fltk3::remove_fd(fileno(s_proc.desc()));
    s_proc.close();
    shell_run_buffer->append("... END SHELL COMMAND ...\n");
  }

  shell_run_display->scroll(shell_run_display->count_lines(0,
                            shell_run_buffer->length(), 1), 0);
}

void
do_shell_command(fltk3::ReturnButton*, void*) {
  const char	*command=NULL;	// Command to run

  if (!prepare_shell_command(command)) return;

  // Show the output window and clear things...
  shell_run_buffer->text("");
  shell_run_buffer->append(command);
  shell_run_buffer->append("\n");
  shell_run_window->label("Shell Command Running...");

  if (s_proc.popen((char *)command) == NULL) {
    fltk3::alert("Unable to run shell command: %s", strerror(errno));
    return;
  }

  shell_run_button->deactivate();
  shell_run_window->hotspot(shell_run_display);
  shell_run_window->show();

  fltk3::add_fd(fileno(s_proc.desc()), shell_pipe_cb);

  while (s_proc.desc()) fltk3::wait();

  shell_run_button->activate();
  shell_run_window->label("Shell Command Complete");
  fltk3::beep();

  while (shell_run_window->shown()) fltk3::wait();
}
#else
// Just do basic shell command stuff, no status window...
void
do_shell_command(fltk3::ReturnButton*, void*) {
  const char	*command;	// Command to run
  int		status;		// Status from command...

  if (!prepare_shell_command(command)) return;

  if ((status = system(command)) != 0) {
    fltk3::alert("Shell command returned status %d!", status);
  } else if (completion_button->value()) {
    fltk3::message("Shell command completed successfully!");
  }
}
#endif // !__MWERKS__

void
show_shell_window() {
  shell_window->hotspot(shell_command_input);
  shell_window->show();
}

void set_filename(const char *c) {
  if (filename) free((void *)filename);
  filename = c ? strdup(c) : NULL;

  if (filename) update_history(filename);

  set_modflag(modflag);
}

//
// The Source View system offers an immediate preview of the code
// files that will be generated by FLUID. It also marks the code
// generated for the last selected item in the header and the source
// file.
//
// Can we patent this?  ;-)  - Matt, mm@matthiasm.com
//

//
// Update the header and source code highlighting depending on the
// currently selected object
//
void update_sourceview_position()
{
  if (!sourceview_panel || !sourceview_panel->visible())
    return;
  if (sv_autoposition->value()==0)
    return;
  if (sourceview_panel && sourceview_panel->visible() && Fl_Type::current) {
    int pos0, pos1;
    if (sv_source->visible_r()) {
      pos0 = Fl_Type::current->code_position;
      pos1 = Fl_Type::current->code_position_end;
      if (pos0>=0) {
        if (pos1<pos0)
          pos1 = pos0;
        sv_source->buffer()->highlight(pos0, pos1);
        int line = sv_source->buffer()->count_lines(0, pos0);
        sv_source->scroll(line, 0);
      }
    }
    if (sv_header->visible_r()) {
      pos0 = Fl_Type::current->header_position;
      pos1 = Fl_Type::current->header_position_end;
      if (pos0>=0) {
        if (pos1<pos0)
          pos1 = pos0;
        sv_header->buffer()->highlight(pos0, pos1);
        int line = sv_header->buffer()->count_lines(0, pos0);
        sv_header->scroll(line, 0);
      }
    }
  }
}

void update_sourceview_position_cb(fltk3::TabGroup*, void*)
{
  update_sourceview_position();
}

static char *sv_source_filename = 0;
static char *sv_header_filename = 0;

//
// Generate a header and source file in a temporary directory and
// load those into the Code Viewer widgets.
//
void update_sourceview_cb(fltk3::Button*, void*)
{
  if (!sourceview_panel || !sourceview_panel->visible())
    return;
  // generate space for the source and header file filenames
  if (!sv_source_filename) {
    sv_source_filename = (char*)malloc(FLTK3_PATH_MAX);
    fluid_prefs.getUserdataPath(sv_source_filename, FLTK3_PATH_MAX);
    strlcat(sv_source_filename, "source_view_tmp.cxx", FLTK3_PATH_MAX);
  }
  if (!sv_header_filename) {
    sv_header_filename = (char*)malloc(FLTK3_PATH_MAX);
    fluid_prefs.getUserdataPath(sv_header_filename, FLTK3_PATH_MAX);
    strlcat(sv_header_filename, "source_view_tmp.h", FLTK3_PATH_MAX);
  }

  strlcpy(i18n_program, fltk3::filename_name(sv_source_filename), sizeof(i18n_program));
  fltk3::filename_setext(i18n_program, sizeof(i18n_program), "");
  const char *code_file_name_bak = code_file_name;
  code_file_name = sv_source_filename;
  const char *header_file_name_bak = header_file_name;
  header_file_name = sv_header_filename;

  // generate the code and load the files
  write_sourceview = 1;
  // generate files
  if (write_code(sv_source_filename, sv_header_filename))
  {
    // load file into source editor
    int pos = sv_source->top_line();
    sv_source->buffer()->loadfile(sv_source_filename);
    sv_source->scroll(pos, 0);
    // load file into header editor
    pos = sv_header->top_line();
    sv_header->buffer()->loadfile(sv_header_filename);
    sv_header->scroll(pos, 0);
    // update the source code highlighting
    update_sourceview_position();
  }
  write_sourceview = 0;

  code_file_name = code_file_name_bak;
  header_file_name = header_file_name_bak;
}

void update_sourceview_timer(void*)
{
  update_sourceview_cb(0,0);
}

// Set the "modified" flag and update the title of the main window...
void set_modflag(int mf) {
  const char	*basename;
  static char	title[FLTK3_PATH_MAX];

  modflag = mf;

  if (main_window) {
    if (!filename) basename = "Untitled.fl";
    else if ((basename = strrchr(filename, '/')) != NULL) basename ++;
#if defined(WIN32) || defined(__EMX__)
    else if ((basename = strrchr(filename, '\\')) != NULL) basename ++;
#endif // WIN32 || __EMX__
    else basename = filename;

    if (modflag) {
      snprintf(title, sizeof(title), "%s (modified)", basename);
      main_window->label(title);
    } else main_window->label(basename);
  }
  // if the UI was modified in any way, update the Source View panel
  if (sourceview_panel && sourceview_panel->visible() && sv_autorefresh->value())
  {
    // we will only update ealiest 0.5 seconds after the last change, and only
    // if no other change was made, so dragging a widget will not generate any
    // CPU load
    fltk3::remove_timeout(update_sourceview_timer, 0);
    fltk3::add_timeout(0.5, update_sourceview_timer, 0);
  }

  // Enable/disable the Save menu item...
  if (modflag) save_item->activate();
  else save_item->deactivate();
}

////////////////////////////////////////////////////////////////

static int arg(int argc, char** argv, int& i) {
  if (argv[i][1] == 'c' && !argv[i][2]) {compile_only = 1; i++; return 1;}
  if (argv[i][1] == 'c' && argv[i][2] == 's' && !argv[i][3]) {compile_only = 1; compile_strings = 1; i++; return 1;}
  if (argv[i][1] == 'o' && !argv[i][2] && i+1 < argc) {
    code_file_name = argv[i+1];
    code_file_set  = 1;
    i += 2;
    return 2;
  }
  if (argv[i][1] == 'h' && !argv[i][2]) {
    header_file_name = argv[i+1];
    header_file_set  = 1;
    i += 2;
    return 2;
  }
  fltk3::PluginManager pm("commandline");
  int j, n = pm.plugins();
  for (j=0; j<n; j++) {
    Fl_Commandline_Plugin *pi = (Fl_Commandline_Plugin*)pm.plugin(j);
    int r = pi->arg(argc, argv, i);
    if (r) return r;
  }
  return 0;
}

#if ! (defined(WIN32) && !defined (__CYGWIN__))

int quit_flag = 0;
#include <signal.h>
#ifdef _sigargs
#define SIGARG _sigargs
#else
#ifdef __sigargs
#define SIGARG __sigargs
#else
#define SIGARG int // you may need to fix this for older systems
#endif
#endif

extern "C" {
static void sigint(SIGARG) {
  signal(SIGINT,sigint);
  quit_flag = 1;
}
}
#endif


int main(int argc,char **argv) {
  int i = 1;
  
  if (!fltk3::args(argc,argv,i,arg) || i < argc-1) {
    static const char *msg = 
      "usage: %s <switches> name.fl\n"
      " -c : write .cxx and .h and exit\n"
      " -cs : write .cxx and .h and strings and exit\n"
      " -o <name> : .cxx output filename, or extension if <name> starts with '.'\n"
      " -h <name> : .h output filename, or extension if <name> starts with '.'\n";
    int len = strlen(msg) + strlen(argv[0]) + strlen(fltk3::help);
    fltk3::PluginManager pm("commandline");
    int i, n = pm.plugins();
    for (i=0; i<n; i++) {
      Fl_Commandline_Plugin *pi = (Fl_Commandline_Plugin*)pm.plugin(i);
      if (pi) len += strlen(pi->help());
    }
    char *buf = (char*)malloc(len+1);
    sprintf(buf, msg, argv[0]);
    for (i=0; i<n; i++) {
      Fl_Commandline_Plugin *pi = (Fl_Commandline_Plugin*)pm.plugin(i);
      if (pi) strcat(buf, pi->help());
    }
    strcat(buf, fltk3::help);
#ifdef _MSC_VER
    fltk3::message("%s\n", buf);
#else
    fprintf(stderr, "%s\n", buf);
#endif
    free(buf);
    return 1;
  }
  if (exit_early)
    exit(0);
  
  const char *c = argv[i];

  fltk3::register_images();

  make_main_window();


  if (c) set_filename(c);
  if (!compile_only) {
#ifdef __APPLE__
    fl_open_callback(apple_open_cb);
#endif // __APPLE__
    fltk3::visual((fltk3::Mode)(fltk3::DOUBLE|fltk3::INDEX));
    fltk3::FileIcon::load_system_icons();
    main_window->callback(exit_cb);
    position_window(main_window,"main_window_pos", 1, 10, 30, WINWIDTH, WINHEIGHT );
    main_window->show(argc,argv);
    toggle_widgetbin_cb(0,0);
    toggle_sourceview_cb(0,0);
    if (!c && openlast_button->value() && absolute_history[0][0]) {
      // Open previous file when no file specified...
      open_history_cb(0, absolute_history[0]);
    }
  }
  undo_suspend();
  if (c && !read_file(c,0)) {
    if (compile_only) {
      fprintf(stderr,"%s : %s\n", c, strerror(errno));
      exit(1);
    }
    fltk3::message("Can't read %s: %s", c, strerror(errno));
  }
  undo_resume();
  if (compile_only) {
    if (compile_strings) write_strings_cb(0,0);
    write_cb(0,0);
    exit(0);
  }
  set_modflag(0);
  undo_clear();
#ifndef WIN32
  signal(SIGINT,sigint);
#endif

  grid_cb(horizontal_input, 0); // Makes sure that windows get snap params...

#ifdef WIN32
  fltk3::run();
#else
  while (!quit_flag) fltk3::wait();

  if (quit_flag) exit_cb(0,0);
#endif // WIN32

  undo_clear();

  return (0);
}

//
// End of "$Id$".
//
