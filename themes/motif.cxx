// motif.cxx

// fltk plugin for Motif appearance, including ugly blue color!

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Style.H>
#include <FL/fl_draw.H>
#include <stdio.h>
#include <string.h>

static void motif_glyph(int /*t*/, int x,int y,int w,int h, Fl_Color bc,
                        Fl_Color fc, Fl_Flags f, Fl_Boxtype bt)
{
  FL_NORMAL_BOX->draw(x,y,w,h,bc,f);
}

extern "C" fltk_theme(int, char**);
int fltk_theme(int, char**) {
  fl_background(0xB0C0D800);
  fl_normal_box.data = "UUJJUUJJUUJJ";
  fl_normal_box.dx_ = 3;
  fl_normal_box.dy_ = 3;
  fl_normal_box.dw_ = 6;
  fl_normal_box.dh_ = 6;
  fl_down_box.data = "JJUUJJUUJJUU";
  fl_down_box.dx_ = 3;
  fl_down_box.dy_ = 3;
  fl_down_box.dw_ = 6;
  fl_down_box.dh_ = 6;
  Fl_Style* s;
  if ((s = Fl_Style::find("menu_item"))) {
    s->set_box(FL_FLAT_UP_BOX);
    s->set_highlight_color(FL_GRAY);
    s->set_highlight_label_color(0);
    s->set_on_color(FL_DARK2);
    s->set_off_color(FL_GRAY);
  }
  if ((s = Fl_Style::find("check_button"))) {
    s->set_on_color(FL_DARK2);
    s->set_off_color(FL_GRAY);
  }
  Fl_Widget::default_style.set_highlight_color(FL_LIGHT2);
  Fl_Widget::default_style.set_glyph(motif_glyph);
  if ((s = Fl_Style::find("scrollbar"))) {
    s->set_box(FL_DOWN_BOX);
  }
  if ((s = Fl_Style::find("input"))) {
    s->set_selection_color(FL_BLACK);
    s->set_selection_text_color(FL_WHITE);
    s->set_box(FL_DOWN_BOX);
  }
  if ((s = Fl_Style::find("output"))) {
    s->set_color(FL_GRAY);
    s->set_box(FL_DOWN_BOX);
  }
  return 0;
}
