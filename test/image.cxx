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
	bool d2 = false;
	switch (*s++) {
	case 'R': n = 1; break;
	case 'G': n = 2; break;
	case 'B': n = 3; break;
	case 'r': n = 1; d2 = true; break;
	case 'g': n = 2; d2 = true; break;
	case 'b': n = 3; d2 = true; break;
	default: n = 0; break;
	}
	for (int xx = 0; xx < SCALE; xx++) {
	  const unsigned char* pp = pixels+depth*n;
	  for (int k=0; k < depth; k++)
	    if (d2) *p++ = *pp++ >> 1;
	    else *p++ = *pp++;
	}
      }
    }
  }
  return b;
}

unsigned char rgbpixels[4*3] = {
  0, 0, 0,
  255,0,0,
  0, 255, 0,
  0, 0, 255};

unsigned char rgbapixels[4*4] = {
  0, 0, 0, 0,
  255,0,0,255,
  0, 255, 0,255,
  0, 0, 255,255};

unsigned char bgrpixels[4*3] = {
  0, 0, 0,
  0, 0, 255,
  0, 255, 0,
  255,0,0};

unsigned char abgrpixels[4*4] = {
  0, 0, 0, 0,
  255, 0, 0, 255,
  255, 0, 255, 0,
  255, 255, 0, 0};

unsigned char lumpixels[4] = {0, 60, 160, 40};

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

  Window window(3*16*SCALE+40,10+5*(16*SCALE+15)+60);
  window.begin();
  Group group(0,0,3*16*SCALE+40,10+5*(16*SCALE+15));
  window.resizable(group);
  group.resizable(group);
  group.begin();

  int y = 10;
  int x1 = 10;
  int x2 = 20+16*SCALE;
  int x3 = 30+32*SCALE;

  Widget* w;

  w = new Dtest(fltk::RGB, 3, x1, y, "RGB", builddata(3, rgbpixels));
  w->tooltip("drawimage() of a 3-byte/pixel rgb image");
  w = new Dtest(fltk::RGB, 4, x2, y, "RGB,4", builddata(4, rgbapixels));
  w->tooltip("drawimage() of an rgb image except the pixels are 4 apart, "
	      "some drivers will screw up and not skip the 4th byte");
  w = new Itest(fltk::RGB, 3, x3, y, "RGB image", builddata(3, rgbpixels));
  w->tooltip("rgbImage() used as widget->image()");
  y += 16*SCALE+15;

  w = new Dtest(fltk::RGBA, 4, x1, y, "RGBA", builddata(4, rgbapixels));
  w->tooltip("drawimage() of 4-byte rgba. Should be transparent, not black");
  w = new Dtest(fltk::ABGR, 4, x2, y, "ABGR", builddata(4, abgrpixels));
  w->tooltip("drawimage() of 4-byte abgr. NYI!");
  w = new Itest(fltk::RGBA, 4, x3, y, "RGBAimage", builddata(4, rgbapixels));
  w->tooltip("rgbImage() with alpha used as widget->image()");
  y += 16*SCALE+15;

  w = new Dtest(fltk::BGR, 3, x1, y, "BGR", builddata(3, bgrpixels));
  w->tooltip("drawimage() of 3-byte bgr. NYI!");
  w = new Dtest(fltk::LUMINANCE, 1, x2, y, "LUMINANCE", builddata(1, lumpixels));
  w->tooltip("drawimage() of 1-byte b&&w image");
  w = new Itest(fltk::LUMINANCE, 1, x3, y, "LUMimage", builddata(1, lumpixels));
  w->tooltip("1-byte b&&w image used as widget->image()");
  y += 16*SCALE+15;

  w = new Dtest(fltk::LUMINANCE, 3, x1, y, "LUM,3", builddata(3, rgbpixels));
  w->tooltip("1-byte b&&w with 3-pixel seperation between pixels. This "
	      "can be used to show the user one channel of an image");
  w = new Dtest(fltk::MASK, 1, x2, y, "MASK", builddata(1, lumpixels));
  w->tooltip("1-byte opacity image. This should be transparent with the "
	      "foreground drawn in the current color. See maskXPM for what "
	      "it should look like.");
  w = new Itest(fltk::MASK, 1, x3, y, "MASKimage", builddata(1, lumpixels));
  w->tooltip("1-byte opacity image used as widget->image(). Should look "
	      "identical to left one and match colors of maskXPM");
  y += 16*SCALE+15;

  w = new Xtest(x1, y, "XPM", porsche_xpm);
  w->tooltip("Older FLTK special-cased the XPM's 1-bit alpha. This is "
	      "what the RGBA sample above should look like.");
  w = new Xtest(x2, y, "maskXPM", recycle_xpm);
  w->tooltip("FLTK recognizes some XPM's as being 8-bit masks. Older "
	      "FLTK special-cases them. The MASK "
	      "samples above should be colored just like this.");
  w = new Btest(x3, y, "Bitmap");
  w->tooltip("The only transparent image FLTK originally drew was the "
	      "xpmImage object. This is still maintained for compatability "
	      "and the colors used here, by maskXPM, and MASK, should all "
	      "match. If they don't, maskXPM is the one that works right.");
  y += 16*SCALE+15;

  group.end();
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

  window.end();
  window.show(argc, argv);
  return fltk::run();
}
