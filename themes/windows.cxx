// Windows-95 style (this is also the fltk default)

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <stdio.h>
#include <string.h>

static Fl_Style* find_style(const char* name) {
  for (Fl_Style_Definer* p = Fl_Style_Definer::first; p; p = p->next)
    if (!strcasecmp(name, p->name)) return p->style;
  return 0;
}

extern "C"
bool fltk_theme(const char*) {
  fl_background(0xC0C0C000);
  fl_normal_box.data = "2AAUWMMTT";
  fl_normal_box.dx_ = 2;
  fl_normal_box.dy_ = 2;
  fl_normal_box.dw_ = 4;
  fl_normal_box.dh_ = 4;
  fl_down_box.data = "2UWMMPPAA";
  fl_down_box.dx_ = 2;
  fl_down_box.dy_ = 2;
  fl_down_box.dw_ = 4;
  fl_down_box.dh_ = 4;
  Fl_Style* s;
  if ((s = find_style("menu_item"))) {
    s->set_box(FL_FLAT_BOX);
    s->set_highlight_color(FL_BLUE_SELECTION_COLOR);
    s->set_highlight_label_color(FL_WHITE);
    s->set_on_color(FL_WHITE);
    s->set_off_color(FL_WHITE);
  }
  if ((s = find_style("check_button"))) {
    s->set_on_color(FL_WHITE);
    s->set_off_color(FL_WHITE);
  }
  Fl_Widget::default_style.set_highlight_color(0);
  Fl_Widget::default_style.set_glyph(fl_glyph);
  if ((s = find_style("scrollbar"))) {
    s->set_box(FL_FLAT_BOX);
  }
  if ((s = find_style("input"))) {
    s->set_box(FL_DOWN_BOX);
    s->set_selection_color(FL_BLUE_SELECTION_COLOR);
    s->set_selection_text_color(FL_WHITE);
  }
  if ((s = find_style("output"))) {
    s->set_color(FL_WHITE);
  }
  return true;
}
