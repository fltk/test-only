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

extern "C" FLDLE void style_plugin();

#include <FL/Fl_Boxtype.H>
#include <FL/Fl_Shared_Image.H>

struct Fl_Image_Box : Fl_Boxtype_ {
  Fl_Shared_Image* normal_img;
  Fl_Shared_Image* down_img;
  Fl_Shared_Image* highlight_img;

  Fl_Image_Box(char*, char*, char*);
};

static void image_box_draw(const Fl_Boxtype_* bt, int x, int y, int w, int h,
		      Fl_Color fill, Fl_Flags flags)
{
  Fl_Shared_Image* img;

  if (flags&FL_VALUE) img = ((Fl_Image_Box*)bt)->down_img;
  else if (flags&FL_HIGHLIGHT) img = ((Fl_Image_Box*)bt)->highlight_img;
  else img = ((Fl_Image_Box*)bt)->normal_img;

  if (!(flags&FL_FRAME_ONLY))
    img->draw_tiled(x+bt->dx_, y+bt->dy_, w-bt->dw_, h-bt->dh_, -w/2, -h/2);
  FL_NORMAL_BOX->draw(x,y,w,h,fill,flags|FL_FRAME_ONLY);
}

Fl_Image_Box::Fl_Image_Box(char* normal_b, char* down_b, char* highlight_b)
{
  draw_ = image_box_draw;
  down = this;
  highlight = this;
  char s[1024];
  getcwd(s, 1023);
  strcat(s, "/");
  strcat(s, normal_b);
  normal_img = Fl_JPEG_Image::get(s);
  getcwd(s, 1023);
  strcat(s, "/");
  strcat(s, down_b);
  down_img = Fl_JPEG_Image::get(s);
  getcwd(s, 1023);
  strcat(s, "/");
  strcat(s, highlight_b);
  highlight_img = Fl_JPEG_Image::get(s);
  dx_=dy_=2; dw_=dh_=4;
  rectangular = 1;
}


struct Fl_Image_NoBorderBox : Fl_Image_Box {
  Fl_Image_NoBorderBox(char*, char*, char*);
};

static void image_noborderbox_draw(const Fl_Boxtype_* bt, int x, int y, int w, int h,
		      Fl_Color fill, Fl_Flags flags)
{
  Fl_Shared_Image* img;

  if (flags&FL_VALUE) img = ((Fl_Image_Box*)bt)->down_img;
  else if (flags&FL_HIGHLIGHT) img = ((Fl_Image_Box*)bt)->highlight_img;
  else img = ((Fl_Image_Box*)bt)->normal_img;

  if (!(flags&FL_FRAME_ONLY))
    img->draw_tiled(x, y, w, h, -w/2, -h/2);
  if (flags&(FL_HIGHLIGHT|FL_VALUE))
    FL_NORMAL_BOX->draw(x,y,w,h,fill,(flags|FL_FRAME_ONLY)&(~FL_VALUE));
  else
    FL_FLAT_BOX->draw(x,y,w,h,fill,flags|FL_FRAME_ONLY);
}

Fl_Image_NoBorderBox::Fl_Image_NoBorderBox(char* normal_b, char* down_b, char* highlight_b)
  : Fl_Image_Box(normal_b, down_b, highlight_b)
{
  draw_ = image_noborderbox_draw;
  dx_=dy_=0; dw_=dh_=0;
}


static bool parse_imagebox(Fl_Style& style, char* s)
{
  char w1[1024], w2[1024], w3[1024];
  strcpy(w1, fl_parse_word(s));
  bool flat = 0;
  if ( (flat = strcmp(w1, "image_box")) && strcmp(w1, "image_flat_box"))
    return 0; // Give the hand to another parser
  if (*fl_parse_word(s) != '(') return 0;
  strcpy(w1, fl_parse_word(s));
  strcpy(w2, fl_parse_word(s));
  strcpy(w3, fl_parse_word(s));
  style.set_box(flat ? (Fl_Image_Box*)new Fl_Image_NoBorderBox(w1, w2, w3) :
                        new Fl_Image_Box(w1, w2, w3));
  return 1;
}

static bool parse_imagebox_glyph(Fl_Style& style, char* s)
{
  char w1[1024], w2[1024], w3[1024];
  strcpy(w1, fl_parse_word(s));
  bool flat = 0;
  if ( (flat = strcmp(w1, "image_box")) && strcmp(w1, "image_flat_box"))
    return 0; // Give the hand to another parser
  if (*fl_parse_word(s) != '(') return 0;
  strcpy(w1, fl_parse_word(s));
  strcpy(w2, fl_parse_word(s));
  strcpy(w3, fl_parse_word(s));
  style.set_glyph_box(flat ? (Fl_Image_Box*)new Fl_Image_NoBorderBox(w1, w2, w3) :
                             new Fl_Image_Box(w1, w2, w3));
  return 1;
}

FLDLE void style_plugin()
{
  fl_add_style_entry_def("box", parse_imagebox);
  fl_add_style_entry_def("glyph_box", parse_imagebox_glyph);
}
