// style.cxx

// Demo of compiled code to set styles.

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/fl_draw.H>

void motif_glyph(int t, int x,int y,int w,int h,
		 Fl_Color c, Fl_Flags f)
{
  FL_NORMAL_BOX->draw(x,y,w,h,c,f);
}

void motif() {
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
  Fl_Menu_Item::default_style.set_box(FL_FLAT_UP_BOX);
  Fl_Menu_Item::default_style.set_highlight_color(FL_GRAY);
  Fl_Menu_Item::default_style.set_highlight_label_color(0);
  Fl_Menu_Item::default_style.set_on_color(FL_DARK2);
  Fl_Menu_Item::default_style.set_off_color(FL_GRAY);
  Fl_Check_Button::default_style.set_on_color(FL_DARK2);
  Fl_Check_Button::default_style.set_off_color(FL_GRAY);
  Fl_Widget::default_style.set_highlight_color(FL_LIGHT2);
  Fl_Widget::default_style.set_glyph(motif_glyph);
  Fl_Scrollbar::default_style.set_box(FL_DOWN_BOX);
  Fl_Input::default_style.set_selection_color(FL_BLACK);
  Fl_Input::default_style.set_selection_text_color(FL_WHITE);
  Fl_Output::default_style.set_color(FL_GRAY);
  Fl::redraw();
}

void windoze() {
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
  Fl_Menu_Item::default_style.set_box(FL_FLAT_BOX);
  Fl_Menu_Item::default_style.set_highlight_color(FL_BLUE_SELECTION_COLOR);
  Fl_Menu_Item::default_style.set_highlight_label_color(FL_WHITE);
  Fl_Menu_Item::default_style.set_on_color(FL_WHITE);
  Fl_Menu_Item::default_style.set_off_color(FL_WHITE);
  Fl_Check_Button::default_style.set_on_color(FL_WHITE);
  Fl_Check_Button::default_style.set_off_color(FL_WHITE);
  Fl_Widget::default_style.set_highlight_color(0);
  Fl_Widget::default_style.set_glyph(fl_glyph);
  Fl_Scrollbar::default_style.set_box(FL_FLAT_BOX);
  Fl_Input::default_style.set_selection_color(FL_BLUE_SELECTION_COLOR);
  Fl_Input::default_style.set_selection_text_color(FL_WHITE);
  Fl_Output::default_style.set_color(FL_WHITE);
  Fl::redraw();
}

void fltk_classic() {
  fl_background(0xC0C0C000);
  fl_normal_box.data = "AAAAWUJJUTNN";
  fl_normal_box.dx_ = 3;
  fl_normal_box.dy_ = 3;
  fl_normal_box.dw_ = 6;
  fl_normal_box.dh_ = 6;
  fl_down_box.data = "NNTUJJUWAAAA";
  fl_down_box.dx_ = 3;
  fl_down_box.dy_ = 3;
  fl_down_box.dw_ = 6;
  fl_down_box.dh_ = 6;
  Fl_Menu_Item::default_style.set_box(FL_FLAT_BOX);
  Fl_Menu_Item::default_style.set_highlight_color(FL_WHITE);
  Fl_Menu_Item::default_style.set_highlight_label_color(0);
  Fl_Menu_Item::default_style.set_on_color(FL_GRAY);
  Fl_Menu_Item::default_style.set_off_color(FL_GRAY);
  Fl_Check_Button::default_style.set_on_color(FL_GRAY);
  Fl_Check_Button::default_style.set_off_color(FL_GRAY);
  Fl_Widget::default_style.set_highlight_color(0);
  Fl_Widget::default_style.set_glyph(fl_glyph);
  Fl_Scrollbar::default_style.set_box(FL_FLAT_BOX);
  Fl_Input::default_style.set_selection_color(FL_DARK2);
  Fl_Input::default_style.set_selection_text_color(FL_BLACK);
  Fl_Output::default_style.set_color(FL_GRAY);
  Fl::redraw();
}

// CET - for testing only-- will be removed
#if 0 //ndef WIN32
#include <signal.h>
static void stylechange(int) {
  if (Fl::style()) Fl::load_styles(1);
}
		     
//  MRS - this signal stuff is driving me crazy - need to update configure
//        to get the f**king cast right, or not use it.  Leave this commented
//        out until this is done so we don't keep changing it back and forth!
//
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = (void (*)(int))stylechange;
  sigaction(SIGUSR1, &sa, 0);
}
#endif

#include <FL/fl_file_chooser.H>
#include <FL/fl_ask.H>
#include "style_ui.cxx"

int main(int argc, char** argv) {
  Fl_Window* w = make_window();
  check1->on_color(FL_WHITE); // test the style bit flags
  w->show(argc, argv);
  return Fl::run();
}


/*#include <FL/Fl_Shared_Image.H>
// to make sure images routines are linked in ...
// this function is never called !
void style_use_whole_fltk()
{
  Fl_Shared_Image::get(0,0);
}
*/
