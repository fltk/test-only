//
// "$Id: fl_ask.H,v 1.7.2.4.2.5.2.2 2003/11/07 03:47:22 easysw Exp $"
//
// Standard dialog header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef fl_ask_H
#define fl_ask_H

#include "Enumerations.H"

class Fl_Widget;

enum {
  FL_BEEP_DEFAULT = 0,
  FL_BEEP_MESSAGE,
  FL_BEEP_ERROR,
  FL_BEEP_QUESTION,
  FL_BEEP_PASSWORD,
  FL_BEEP_NOTIFICATION
};

FL_EXPORT void fl_beep(int type = FL_BEEP_DEFAULT);
FL_EXPORT void fl_message(const char *,...);
FL_EXPORT void fl_alert(const char *,...);
FL_EXPORT int fl_ask(const char *,...);
FL_EXPORT int fl_choice(const char *q,const char *b0,const char *b1,const char *b2,...);
FL_EXPORT const char *fl_input(const char *label, const char *deflt = 0, ...);
FL_EXPORT const char *fl_password(const char *label, const char *deflt = 0, ...);

FL_EXPORT Fl_Widget *fl_message_icon();
extern FL_EXPORT Fl_Font fl_message_font_;
extern FL_EXPORT unsigned char fl_message_size_;
inline void fl_message_font(unsigned char f,unsigned char s) {
  fl_message_font_ = (Fl_Font)f; fl_message_size_ = s;}

// pointers you can use to change FLTK to a foreign language:
extern FL_EXPORT const char* fl_no;
extern FL_EXPORT const char* fl_yes;
extern FL_EXPORT const char* fl_ok;
extern FL_EXPORT const char* fl_cancel;
extern FL_EXPORT const char* fl_close;
extern FL_EXPORT const char* fl_choose_file;
extern FL_EXPORT const char* fl_show;
extern FL_EXPORT const char* fl_favorites;
extern FL_EXPORT const char* fl_create_a_new_directory;
extern FL_EXPORT const char* fl_preview;
extern FL_EXPORT const char* fl_file_name;
extern FL_EXPORT const char* fl_manage_favorites;
extern FL_EXPORT const char* fl_unable_to_load_icon;
extern FL_EXPORT const char* fl_unable_to_open_fti;
extern FL_EXPORT const char* fl_expected_letter_fti;
extern FL_EXPORT const char* fl_expected_a_fti;
extern FL_EXPORT const char* fl_expected_b_fti;
extern FL_EXPORT const char* fl_expected_c_fti;
extern FL_EXPORT const char* fl_unknown_command_fti;
extern FL_EXPORT const char* fl_gif_unable_to_open;
extern FL_EXPORT const char* fl_gif_is_not_gif;
extern FL_EXPORT const char* fl_gif_is_version;
extern FL_EXPORT const char* fl_gif_no_colormap;
extern FL_EXPORT const char* fl_gif_unexpected_eof;
extern FL_EXPORT const char* fl_gif_unknown_extension;
extern FL_EXPORT const char* fl_gif_unknown_code;
extern FL_EXPORT const char* fl_gif_lzw_barF;
extern FL_EXPORT const char* fl_show_previous_help_page;
extern FL_EXPORT const char* fl_make_the_help_text_smaller;
extern FL_EXPORT const char* fl_make_the_help_text_larger;
extern FL_EXPORT const char* fl_show_next_help_page;
extern FL_EXPORT const char* fl_help_dialog;
extern FL_EXPORT const char* fl_untitled;
extern FL_EXPORT const char* fl_no_uri;
extern FL_EXPORT const char* fl_unable_to_follow;
extern FL_EXPORT const char* fl_pnm_early_eof;
extern FL_EXPORT const char* fl_ins1_failed;
extern FL_EXPORT const char* fl_ovly1_failed;
extern FL_EXPORT const char* fl_repl1_failed;
extern FL_EXPORT const char* fl_cannot_modify_cb;
extern FL_EXPORT const char* fl_cannot_find_pre;
extern FL_EXPORT const char* fl_ptvl_failed;
extern FL_EXPORT const char* fl_bad_measuremenT;
extern FL_EXPORT const char* fl_error;
extern FL_EXPORT const char* fl_unknown_color;
extern FL_EXPORT const char* fl_xio_error;
extern FL_EXPORT const char* fl_cannot_open_display;
extern FL_EXPORT const char* fl_requires_true_color;
extern FL_EXPORT const char* fl_cannot_do_scanline;
extern FL_EXPORT const char* fl_cannot_bits_per_pixel;
extern FL_EXPORT const char* fl_cannot_24bit;
extern FL_EXPORT const char* fl_cannot_do_bits_per_pixel;
extern FL_EXPORT const char* fl_cannot_do_delta;
extern FL_EXPORT const char* fl_cannot_create_pen;
extern FL_EXPORT const char* fl_gif_lzw_barf;
extern FL_EXPORT const char* fl_bad_measurement;


/*
const char	*Fl_File_Chooser::add_favorites_label = "Add to Favorites";
const char	*Fl_File_Chooser::all_files_label = "All Files (*)";
const char	*Fl_File_Chooser::custom_filter_label = "Custom Filter";
const char	*Fl_File_Chooser::existing_file_label = "Please choose an existing file!";
const char	*Fl_File_Chooser::favorites_label = "Favorites";
const char	*Fl_File_Chooser::filename_label = "Filename:";
const char	*Fl_File_Chooser::filesystems_label = "My Computer";
const char	*Fl_File_Chooser::manage_favorites_label = "Manage Favorites";
const char	*Fl_File_Chooser::new_directory_label = "New Directory?";
const char	*Fl_File_Chooser::preview_label = "Preview";
const char	*Fl_File_Chooser::show_label = "Show:";
*/
#endif

//
// End of "$Id: fl_ask.H,v 1.7.2.4.2.5.2.2 2003/11/07 03:47:22 easysw Exp $".
//
