// essai.cxx

// Demonstration of a plugin that replaces the boxes such that a pixmap
// is used to draw everything.

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/fl_draw.H>
#include <stdio.h>
#include <string.h>

static Fl_Style* find_style(const char* name) {
  for (Fl_Style_Definer* p = Fl_Style_Definer::first; p; p = p->next)
    if (!strcasecmp(name, p->name)) return p->style;
  return 0;
}

#include <FL/Fl_Boxtype.H>
#include <FL/Fl_Shared_Image.H>

struct Fl_Image_Box : Fl_Boxtype_ {
  Fl_Shared_Image* img;
  Fl_Image_Box(char* f);
};

static void image_box_draw(const Fl_Boxtype_* bt, int x, int y, int w, int h,
		      Fl_Color fill, Fl_Flags flags)
{
  if (!(flags&FL_FRAME_ONLY))
    ((Fl_Image_Box*)bt)->img->draw_tiled(x+bt->dx_, y+bt->dy_, w-bt->dw_, h-bt->dh_, -w/2, -h/2);
  FL_NORMAL_BOX->draw(x,y,w,h,fill,flags|FL_FRAME_ONLY);
}

// this function needs some work.  Return relative pathname:
static char* dirname;
static char* end_of_dir;
static char* fullname(char* f) {
  if (*f == '/') return f;
  strncpy(end_of_dir, f, 256);
  return dirname;
}

Fl_Image_Box::Fl_Image_Box(char* f) {
  draw_ = image_box_draw;
  img = Fl_JPEG_Image::get(fullname(f));
  dx_=dy_=2; dw_=dh_=4;
  rectangular = 1;
}

struct Fl_Image_NoBorderBox : Fl_Boxtype_ {
  Fl_Shared_Image* img;
  Fl_Image_NoBorderBox(char* f);
};

static void image_noborderbox_draw(const Fl_Boxtype_* bt, int x, int y, int w, int h,
		      Fl_Color fill, Fl_Flags flags)
{
  if (!(flags&FL_FRAME_ONLY))
    ((Fl_Image_Box*)bt)->img->draw_tiled(x, y, w, h, -w/2, -h/2);
  FL_FLAT_BOX->draw(x,y,w,h,fill,flags|FL_FRAME_ONLY);
}

Fl_Image_NoBorderBox::Fl_Image_NoBorderBox(char* f)
{
  draw_ = image_noborderbox_draw;
  img = Fl_JPEG_Image::get(fullname(f));
  dx_=dy_=0; dw_=dh_=0;
  rectangular = 1;
}

extern "C"
bool fltk_theme(const char* filename) {
  char dir[1024];
  strncpy(dir, filename, 1024-256);
  char* c = dir;
  for (char* p = dir; *p;) if (*p++ == '/') c = p;
  dirname = dir; end_of_dir = c;
  fl_background(0xD0D0E000); // it would be nice to figure out color from image
  Fl_Boxtype flat = new Fl_Image_NoBorderBox("bg.jpeg");
  Fl_Boxtype box = new Fl_Image_Box("bg2.jpeg");
  Fl_Widget::default_style.set_box(box);
  Fl_Window::default_style.set_box(flat);
  Fl_Style* s;
  if ((s = find_style("menu_item"))) {
    s->set_box(flat);
  }
  if ((s = find_style("tabs"))) {
    s->set_box(box);
  }
  return true;
}
