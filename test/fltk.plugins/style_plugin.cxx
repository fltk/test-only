#include <FL/Fl_Plugins.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Style_Util.H>

extern "C" void style_plugin();

#include <FL/Fl_Boxtype.H>
#include <FL/Fl_Shared_Image.H>

struct Fl_Image_Box : Fl_Boxtype_ {
  Fl_Shared_Image* img;

  Fl_Image_Box(char* f);
};

static void image_box_draw(const Fl_Boxtype_* bt, int x, int y, int w, int h,
		      Fl_Color fill, Fl_Flags)
{
  ((Fl_Image_Box*)bt)->img->draw_tiled(x, y, w, h, -w/2, -h/2);
}

Fl_Image_Box::Fl_Image_Box(char* f)
{
  draw_ = image_box_draw;
  img = Fl_Shared_Image::guess(f)->get(f);
}


static bool parse_imagebox(Fl_Style& style, char* s)
{
  char* w = fl_parse_word(s);
  if (strcmp(w, "imagebox")) return 0; // Give the hand to another parser
  w = fl_parse_word(s);
  style.set_box(new Fl_Image_Box(w));
  return 1;
}

FLDLE void style_plugin()
{
  fl_add_style_entry_def("box", parse_imagebox);
}
