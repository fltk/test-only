/* This plugin defines the default styles that fltk knows about, and contains
   code for some boxtypes that may not be linked into the main program. */

#include <FL/Fl_Plugins.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Style_Util.H>
#include <FL/Fl_Window.H>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(WIN32) && !defined(CYGNUS)
# include <direct.h>
#else
# include <unistd.h>
#endif
#include <string.h>

extern "C" FLDLE void style_plugin();

static bool parse_color(Fl_Style& style, char* s) { 
  style.set_color(fl_parse_color(s)); return 1; }
static bool parse_label_color(Fl_Style& style, char* s) { 
  style.set_label_color(fl_parse_color(s)); return 1; }
static bool parse_selection_color(Fl_Style& style, char* s) { 
  style.set_selection_color(fl_parse_color(s)); return 1; }
static bool parse_selection_text_color(Fl_Style& style, char* s) { 
  style.set_selection_text_color(fl_parse_color(s)); return 1; }
static bool parse_off_color(Fl_Style& style, char* s) { 
  style.set_off_color(fl_parse_color(s)); return 1; }
static bool parse_highlight_color(Fl_Style& style, char* s) { 
  style.set_highlight_color(fl_parse_color(s)); return 1; }
static bool parse_highlight_label_color(Fl_Style& style, char* s) { 
  style.set_highlight_label_color(fl_parse_color(s)); return 1; }
static bool parse_text_color(Fl_Style& style, char* s) { 
  style.set_text_color(fl_parse_color(s)); return 1; }


#include <FL/Fl_Boxtype.H>
struct fl_box {
  char* name;
  Fl_Boxtype bt;
};

static fl_box boxtypes[] = {
{"no_box", &fl_no_box},
{"flat_box", &fl_flat_box},
{"flat_up_box", &fl_flat_up_box},
{"flat_down_box", &fl_flat_down_box},
{"normal_box", &fl_normal_box},
{"down_box", &fl_down_box},
{"thin_box", &fl_thin_box},
{"thin_down_box", &fl_thin_down_box},
{"engraved_box", &fl_engraved_box},
{"embossed_box", &fl_embossed_box},
{"border_box", &fl_border_box},
{"shadow_box", &fl_shadow_box},
{"rounded_box", &fl_rounded_box},
{"rshadow_box", &fl_rshadow_box},
{"rflat_box", &fl_rflat_box},
{"round_box", &fl_round_box},
{"round_down_box", &fl_round_down_box},
{"diamond_box", &fl_diamond_box},
{"diamond_down_box", &fl_diamond_down_box},
{"oval_box", &fl_oval_box},
{"oval_shadow_box", &fl_oval_shadow_box},
{"oval_flat_box", &fl_oval_flat_box},
{"border_frame", &fl_border_frame },
{0,0}
};

static bool parse_boxtype(Fl_Style& style, char* s)
{
  char* w = fl_parse_word(s);
  for (fl_box* b = boxtypes; b->name; b++)
    if (!strcmp(w, b->name)) {
      style.set_box(b->bt);
      return 1;
    }
  return 0;
}

FLDLE void style_plugin()
{
  fl_add_style_entry_def("box", parse_boxtype);
  fl_add_style_entry_def("color", parse_color);
  fl_add_style_entry_def("label_color", parse_label_color);
  fl_add_style_entry_def("selection_color", parse_selection_color);
  fl_add_style_entry_def("selection_text_color", parse_selection_text_color);
  fl_add_style_entry_def("off_color", parse_off_color);
  fl_add_style_entry_def("highlight_color", parse_highlight_color);
  fl_add_style_entry_def("highlight_label_color", parse_highlight_label_color);
  fl_add_style_entry_def("text_color", parse_text_color);
}
