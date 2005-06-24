// Lame test of all types of image drawings.

#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/draw.h>
#include <fltk/rgbImage.h>
#include <fltk/xpmImage.h>
#include <fltk/xbmImage.h>
#include <fltk/Slider.h>
#include <fltk/CheckButton.h>

#include <config.h>
#if USE_X11
#include "list_visuals.cxx"
#include <fltk/visual.h>
#endif

using namespace fltk;

const char cdata[16*16+1] =
"                "
"  R RR    GGG   "
"  RRrrR  GgggG  "
"  Rrrrrr GgggGg "
"  Rrrrrrr GGGGgg"
"  Rrrrrrrr ggGgg"
"  Rrrrrrrrr gGgg"
"   rBrrrrrGGGggg"
"    Bbrrrrrggggg"
"    BBBBrrrrgggg"
"    BbbbBrrrrggg"
"    BbbbBbrrrrgg"
"    BbbbBbbrrrrg"
"    BBBBbbbbrrrr"
"     bbbbbbbbrrr"
"      bbbbbbbbrr";

#define SCALE 4

// Tester for drawimage() call:
class Dtest : public Widget {
public:
  fltk::PixelType pixeltype;
  int delta;
  const unsigned char* data;
  void draw() {
    drawimage(data, pixeltype, Rectangle(16*SCALE,16*SCALE), delta);
  }
  Dtest(PixelType pt, int dt, int x, int y, const char* l, const unsigned char* d) :
    Widget(x,y,16*SCALE,16*SCALE,l), pixeltype(pt), delta(dt), data(d) {
    box(NO_BOX);
    align(ALIGN_BOTTOM);
    labelsize(10);
    tooltip("fltk::drawimage()");
  }
};

// Tester for rgbImage subclass:
class Itest : public Widget {
public:
  Itest(PixelType pt, int dt, int x, int y, const char* l, const unsigned char* d) :
    Widget(x,y,16*SCALE,16*SCALE,l) {
    image(new rgbImage(d, pt, 16*SCALE, 16*SCALE, dt));
    box(NO_BOX);
    align(ALIGN_BOTTOM);
    labelsize(10);
    tooltip("Instance of fltk::rgbImage used as widget->image()");
  }
};

// Bitmap tester:
#include "sorceress.xbm"
class Btest : public Widget {
public:
  Btest(int x, int y, const char* l)
    : Widget(x,y,16*SCALE,16*SCALE,l) {
    image(new xbmImage(sorceress_bits, sorceress_width, sorceress_height));
    box(NO_BOX);
    align(ALIGN_BOTTOM|ALIGN_CLIP);
    labelsize(10);
  }
};

// Monochrome xpm tester:
#include "recycle.xpm"
#include "porsche.xpm"
class Xtest : public Widget {
public:
  Xtest(int x, int y, const char* l, const char* const* data)
    : Widget(x,y,16*SCALE,16*SCALE,l) {
    image(new xpmImage(data,0));
    box(NO_BOX);
    align(ALIGN_BOTTOM|ALIGN_CLIP);
    labelsize(10);
  }
};

unsigned char* builddata(int depth, const unsigned char* pixels) {
  unsigned char* b = new unsigned char[SCALE*SCALE*16*16*depth];
  unsigned char* p = b;
  for (int y = 0; y < 16; y++) {
    for (int y1 = 0; y1 < SCALE; y1++) {
      const char* s = cdata+y*16;
      for (int x = 0; x < 16; x++) {
	int n;
	switch (*s++) {
	case 'R': n = 1; break;
	case 'G': n = 2; break;
	case 'B': n = 3; break;
	case 'r': n = 4; break;
	case 'g': n = 5; break;
	case 'b': n = 6; break;
	default: n = 0; break;
	}
	for (int xx = 0; xx < SCALE; xx++) {
	  const unsigned char* pp = pixels+depth*n;
	  for (int k=0; k < depth; k++) *p++ = *pp++;
	}
      }
    }
  }
  return b;
}

unsigned char monopixels[7] = {0, 60, 160, 40, 30, 130, 20};

unsigned char bgrpixels[7*3] = {
  0, 0, 0,
  0, 0, 255,
  0, 255, 0,
  255,0,0,
  0, 0, 127,
  0, 127, 0,
  127,0,0};

unsigned char rgbpixels[7*3] = {
  0, 0, 0,
  255,0,0,
  0, 255, 0,
  0, 0, 255,
  127,0,0,
  0, 127, 0,
  0, 0, 127};

unsigned char rgbapixels[7*4] = {
  0, 0, 0, 0,
  255,0,0,255,
  0, 255, 0,255,
  0, 0, 255,255,
  127,0,0,127,
  0, 127, 0,127,
  0, 0, 127,127};

unsigned char abgrpixels[7*4] = {
  0, 0, 0, 0,
  255, 0, 0, 255,
  255, 0, 255, 0,
  255, 255, 0, 0,
  127, 0, 0, 127,
  127, 0, 127, 0,
  127, 127, 0, 0
};

unsigned char bgrapixels[7*4] = {
  0, 0, 0, 0,
  0, 0, 255, 255,
  0, 255, 0, 255,
  255, 0, 0, 255,
  0, 0, 127, 127,
  0, 127, 0, 127,
  127, 0, 0, 127
};

unsigned char argbpixels[7*4] = {
  0, 0, 0, 0,
  255, 255, 0, 0,
  255, 0, 255, 0,
  255, 0, 0, 255,
  127, 127, 0, 0,
  127, 0, 127, 0,
  127, 0, 0, 127,
};

unsigned char rgbmpixels[7*4] = {
  0, 0, 0, 0,
  255,0,0,255,
  0, 255, 0,255,
  0, 0, 255,255,
  255,0,0,127,
  0, 255, 0,127,
  0, 0, 255,127};

unsigned char mbgrpixels[7*4] = {
  0, 0, 0, 0,
  255, 0, 0, 255,
  255, 0, 255, 0,
  255, 255, 0, 0,
  127, 0, 0, 255,
  127, 0, 255, 0,
  127, 255, 0, 0
};

unsigned char bgrmpixels[7*4] = {
  0, 0, 0, 0,
  0, 0, 255, 255,
  0, 255, 0, 255,
  255, 0, 0, 255,
  0, 0, 255, 127,
  0, 255, 0, 127,
  255, 0, 0, 127
};

unsigned char mrgbpixels[7*4] = {
  0, 0, 0, 0,
  255, 255, 0, 0,
  255, 0, 255, 0,
  255, 0, 0, 255,
  127, 255, 0, 0,
  127, 0, 255, 0,
  127, 0, 0, 255,
};

#if USE_X11
int visid = -1;
int arg(int argc, char **argv, int &i) {
  if (argv[i][1] == 'v') {
    if (i+1 >= argc) return 0;
    visid = atoi(argv[i+1]);
    i += 2;
    return 2;
  }
  return 0;
}
#endif

void bgcallback(Widget* w, void *v) {
  fltk::Color c = fltk::Color(((Slider*)w)->value());
  ((Window*)v)->color(c);
  fltk::Widget::default_style->buttoncolor(c);
  fltk::redraw();
}

void fgcallback(Widget* w, void *v) {
  fltk::Color c = fltk::Color(((Slider*)w)->value());
  fltk::Widget::default_style->labelcolor(c);
  fltk::redraw();
}

void activecallback(Widget* w, void* v) {
  if (w->value()) ((Widget*)v)->activate();
  else ((Widget*)v)->deactivate();
}

int main(int argc, char** argv) {

#if USE_X11
  int i = 1;
  if (args(argc,argv,i,::arg) < argc) {
    fprintf(stderr," -v # : use visual\n%s\n",help);
    exit(1);
  }

  if (visid >= 0) {
    open_display();
    XVisualInfo templt; int num;
    templt.visualid = visid;
    xvisual = XGetVisualInfo(xdisplay, VisualIDMask, &templt, &num);
    if (!xvisual) {
      fprintf(stderr, "No visual with id %d, use one of:\n",visid);
      list_visuals();
      exit(1);
    }
    xcolormap = XCreateColormap(xdisplay, RootWindow(xdisplay,xscreen),
				xvisual->visual, AllocNone);
    xpixel(BLACK); // make sure black is allocated in overlay visuals
  } else {
    visual(RGB_COLOR);
  }
#endif

  const int COLS = 4;
  const int ROWS = 7;

  Window window(COLS*(16*SCALE+10)+10, 10+ROWS*(16*SCALE+15)+60+150);
  window.begin();
  Group group(0,0,window.w(),window.h()-60-150);
  window.resizable(group);
  group.resizable(group);
  group.begin();

  int y = 10;
  int x = 10;
#define nextxy() if ((x+=16*SCALE+10)>=window.w()) y += 16*SCALE+(x=10)+5

  Widget* w;

  w = new Dtest(fltk::BGR, 3, x, y, "BGR", builddata(3, bgrpixels));
  nextxy();

  w = new Dtest(fltk::RGB, 3, x, y, "RGB", builddata(3, rgbpixels));
  nextxy();

  w = new Dtest(fltk::RGB, 4, x, y, "RGB,4", builddata(4, rgbapixels));
  w->tooltip("drawimage() of an rgb image except the pixels are 4 apart, "
	      "some drivers will screw up and not skip the 4th byte");
  nextxy();

  w = new Itest(fltk::RGB, 3, x, y, "RGB image", builddata(3, rgbpixels));
  nextxy();

  w = new Dtest(fltk::RGBA, 4, x, y, "RGBA", builddata(4, rgbapixels));
  w->tooltip("drawimage() of 4-byte rgba. Should be transparent, not black");
  nextxy();

  w = new Itest(fltk::RGBA, 4, x, y, "RGBAimage", builddata(4, rgbapixels));
  nextxy();

  w = new Dtest(fltk::ABGR, 4, x, y, "ABGR", builddata(4, abgrpixels));
  nextxy();

  w = new Itest(fltk::ABGR, 4, x, y, "ABGRimage", builddata(4, abgrpixels));
  nextxy();

  w = new Dtest(fltk::BGRA, 4, x, y, "BGRA", builddata(4, bgrapixels));
  nextxy();

  w = new Itest(fltk::BGRA, 4, x, y, "BGRAimage", builddata(4, bgrapixels));
  nextxy();

  w = new Dtest(fltk::ARGB, 4, x, y, "ARGB", builddata(4, argbpixels));
  nextxy();

  w = new Itest(fltk::ARGB, 4, x, y, "ARGBimage", builddata(4, argbpixels));
  nextxy();

  w = new Dtest(fltk::RGBM, 4, x, y, "RGBM", builddata(4, rgbmpixels));
  nextxy();

  w = new Itest(fltk::RGBM, 4, x, y, "RGBMimage", builddata(4, rgbmpixels));
  nextxy();

  w = new Dtest(fltk::MBGR, 4, x, y, "MBGR", builddata(4, mbgrpixels));
  nextxy();

  w = new Itest(fltk::MBGR, 4, x, y, "MBGRimage", builddata(4, mbgrpixels));
  nextxy();

  w = new Dtest(fltk::BGRM, 4, x, y, "BGRM", builddata(4, bgrmpixels));
  nextxy();

  w = new Itest(fltk::BGRM, 4, x, y, "BGRMimage", builddata(4, bgrmpixels));
  nextxy();

  w = new Dtest(fltk::MRGB, 4, x, y, "MRGB", builddata(4, mrgbpixels));
  nextxy();

  w = new Itest(fltk::MRGB, 4, x, y, "MRGBimage", builddata(4, mrgbpixels));
  nextxy();

  w = new Dtest(fltk::MONO, 1, x, y, "MONO", builddata(1, monopixels));
  nextxy();

  w = new Itest(fltk::MONO, 1, x, y, "MONOimage", builddata(1, monopixels));
  nextxy();

  w = new Dtest(fltk::MASK, 1, x, y, "MASK", builddata(1, monopixels));
  nextxy();

  w = new Itest(fltk::MASK, 1, x, y, "MASKimage", builddata(1, monopixels));
  nextxy();

  w = new Dtest(fltk::MONO, 3, x, y, "MONO,3", builddata(3, rgbpixels));
  w->tooltip("3-byte seperation between pixels. This "
	      "can be used to show the user one channel of an image");
  nextxy();

  w = new Xtest(x, y, "xpmImage", porsche_xpm);
  w->tooltip("In older versions of FLTK, the only transparency available "
	     "was from XPM and GIF images. This should continue to work.");
  nextxy();

  w = new Xtest(x, y, "MASK xpmImage", recycle_xpm);
  w->tooltip("In some versions of FLTK2, the only MASK style image was "
	     "special monochrome XPM files. These should continue to work.");
  nextxy();

  w = new Btest(x, y, "xbmImage");
  w->tooltip("In older versions of FLTK, the only MASK style image was "
	     "the fltk::xbmImage object, which only had 1 bit for pixel. "
	     "This should continue to work and display the same colors "
	     "as maskXPM and MASK.");
  nextxy();

  group.end(); y = group.h();
  CheckButton* b = new CheckButton(20,y,100,20,"active");
  b->tooltip("activate()/deactivate() the widgets. The ones using an Image "
	     "will redraw to show the grayed-out state.");
  b->set();
  b->callback(activecallback, &group);
  y += 20;

  Slider* s = new Slider(20,y,window.w()-30,20,"bg");
  s->tooltip("background color, change this to see transparency");
  s->range(1,255);
  s->step(1);
  s->value(fltk::GRAY75);
  s->align(ALIGN_LEFT);
  s->callback(bgcallback, &window);
  s = new Slider(20,y+20,window.w()-30,20,"fg");
  s->tooltip("labelcolor, change this to see if MASK works");
  s->range(1,255);
  s->step(1);
  s->value(fltk::BLACK);
  s->callback(fgcallback, &window);
  s->align(ALIGN_LEFT);

  Widget text(0,y+40,window.w(),window.h()-y-40,
	     "All the colored images should look alike, except the "
	     "top row has black squares for the background.\n\n"
	     "The MASK images in the bottom, and the xpm and bitmap "
	     "images, should draw using identical foreground color and "
	     "gray out when inactive is checked.\n\n"
	     "The MONO images should be black & white.");
  text.align(ALIGN_LEFT|ALIGN_INSIDE|ALIGN_WRAP);

  window.end();

#if USE_X11
  // try to set the icon on X
  uchar* data = builddata(4, bgrapixels);
  const unsigned width = 16*SCALE;
  const unsigned height = 16*SCALE;
  unsigned* icon = new unsigned[width*height+2];
  icon[0] = width;
  icon[1] = height;
  memcpy(icon+2, data, width*height*4);
  window.icon(icon);
#endif

  window.show(argc, argv);

  return fltk::run();
}
