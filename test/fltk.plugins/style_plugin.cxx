#include <FL/Fl_Plugins.H>
#include <FL/Fl_Style.H>
#include <FL/Fl_Style_Util.H>
#include <FL/Fl_Boxtype.H>
#if defined(WIN32) && !defined(CYGNUS)
# include <direct.h>
#else
# include <unistd.h>
#endif
#include <string.h>

extern "C" void style_plugin();

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
    ((Fl_Image_Box*)bt)->img->draw_tiled(x, y, w, h, -w/2, -h/2);
  FL_NORMAL_BOX->draw(x,y,w,h,fill,flags|FL_FRAME_ONLY);
}

Fl_Image_Box::Fl_Image_Box(char* f)
{
  draw_ = image_box_draw;
  char s[1024];
  getcwd(s, 1023);
  strcat(s, "/");
  strcat(s, f);
  img = Fl_Shared_Image::guess(s)->get(s);
  dx_=dy_=2; dw_=dh_=4;
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
  char s[1024];
  getcwd(s, 1023);
  strcat(s, "/");
  strcat(s, f);
  img = Fl_Shared_Image::guess(s)->get(s);
  dx_=dy_=0; dw_=dh_=0;
}


static bool parse_imagebox(Fl_Style& style, char* s)
{
  char* w = fl_parse_word(s);
  bool flat = 0;
  if ( (flat = strcmp(w, "image_box")) && strcmp(w, "image_flat_box")) 
    return 0; // Give the hand to another parser
  w = fl_parse_word(s);
  style.set_box(flat? new Fl_Image_NoBorderBox(w) : new Fl_Image_Box(w));
  return 1;
}

FLDLE void style_plugin()
{
  fl_add_style_entry_def("box", parse_imagebox);
}
