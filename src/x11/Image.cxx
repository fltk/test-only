struct PixmapPair {
  XWindow rgb;
  XWindow alpha;
  Color fg;
  Color bg;
  PixmapPair() {rgb = alpha = 0;}
  ~PixmapPair() {
    fltk::stop_drawing(rgb);
    if (alpha) XFreePixmap(xdisplay, alpha);
    if (rgb) XFreePixmap(xdisplay, rgb);
  }
};

extern fltk::Image* fl_current_Image;

bool Image::drawn() const {
  if (!(flags&DRAWN)) return false;
  if ((flags&USES_FG) && ((PixmapPair*)picture)->fg != getcolor()) return false;
  if ((flags&USES_BG) && ((PixmapPair*)picture)->bg != getbgcolor()) return false;
  return true;
}

void Image::make_current() {
  if (!picture) {
    open_display();
    picture = new PixmapPair;
  }
  PixmapPair* picture = (PixmapPair*)(this->picture);
  if (picture->alpha) {
    XFreePixmap(xdisplay, picture->alpha);
    picture->alpha = 0;
  }
//   if (!picture->rgb) {
//     picture->rgb = XCreatePixmap(xdisplay, xwindow, w_, h_, xvisual->depth);
//   }
  picture->fg = fltk::getcolor();
  picture->bg = fltk::getbgcolor();
  flags = 0; //DRAWN | OPAQUE;
  //  draw_into(picture->rgb, w_, h_);
  fl_current_Image = this;
}

void fl_restore_clip(); // in rect.C

#if USE_XFT
XRenderPictFormat* fl_rgba_xrender_format;
void fl_xrender_draw_image(XWindow target,
			   const fltk::Rectangle& r1, bool alpha);
extern bool fl_trivial_transform();
#endif

void Image::over(int x, int y) const {
  if (!(flags&DRAWN) || w_ < 1 || h_ < 1) return;
  PixmapPair* picture = (PixmapPair*)(this->picture);
#if USE_XFT
  if (fl_rgba_xrender_format && picture->rgb) {
    fl_xrender_draw_image(picture->rgb, fltk::Rectangle(x,y,w_,h_),
			  !(flags&OPAQUE));
    return;
  }
#endif
  // XLib version:
  // unfortunately scaling does not work, so I just center and clip
  // to the transformed rectangle.
  // This is the rectangle I want to fill:
  Rectangle r2(x,y,w_,h_); transform(r2);
  // Center the image in that rectangle:
  Rectangle r1(r2,w_,h_);
  // now figure out what area we will draw:
  Rectangle r(r1);
  if (w_ >= r2.w()) {r.x(r2.x()); r.w(r2.w());}
  if (h_ >= r2.h()) {r.y(r2.y()); r.h(r2.h());}
  // We must clip it because otherwise we can't do the alpha:
  if (!intersect_with_clip(r)) return;
  if (flags & OPAQUE) {
    XCopyArea(xdisplay, picture->rgb, xwindow, gc,
	      r.x()-r1.x(), r.y()-r1.y(), r.w(), r.h(), r.x(), r.y());
  } else if (picture->rgb) {
    // There seems to be no way to combine a mask with a clip region,
    // so I just replace it. This will be incorrect if the current
    // clip is not a rectangle
    XSetClipMask(xdisplay, gc, picture->alpha);
    XSetClipOrigin(xdisplay, gc, r1.x(), r1.y());
    XCopyArea(xdisplay, picture->rgb, xwindow, gc,
	      r.x()-r1.x(), r.y()-r1.y(), r.w(), r.h(), r.x(), r.y());
    XSetClipOrigin(xdisplay, gc, 0, 0);
    fl_restore_clip();
  } else if (picture->alpha) {
    // bitmap with only the alpha...
    XSetStipple(xdisplay, gc, picture->alpha);
    XSetTSOrigin(xdisplay, gc, r1.x(), r1.y());
    XSetFillStyle(xdisplay, gc, FillStippled);
    XFillRectangle(xdisplay, xwindow, gc, r.x(), r.y(), r.w(), r.h());
    XSetFillStyle(xdisplay, gc, FillSolid);
  }
}

bool Image::fills_rectangle() const {
  return flags & OPAQUE;
}

void Image::destroy() {
  if (fl_current_Image==this) fl_current_Image = 0;
  delete (PixmapPair*)picture;
  picture = 0;
  flags = 0;
}

#include <fltk/xbmImage.h>

void xbmImage::_draw(const fltk::Rectangle& r) const
{
  if (!drawn()) {
    Image* i = const_cast<xbmImage*>(this);
    if (!i->picture) {
      open_display();
      i->picture = new PixmapPair;
    }
    PixmapPair* picture = (PixmapPair*)(i->picture);
    if (picture->alpha) XFreePixmap(xdisplay, picture->alpha);
    picture->alpha =
      XCreateBitmapFromData(xdisplay, xwindow, (char*)array, (w()+7)&-8, h());
    i->flags = DRAWN;
  }
  Image::_draw(r);
}

