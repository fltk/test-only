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

#if USE_XFT
XRenderPictFormat* fl_rgba_xrender_format;
void fl_xrender_draw_image(XWindow, fltk::PixelType type,
			   const fltk::Rectangle& from,
			   const fltk::Rectangle& to);
#endif

bool Image::drawn() const {
  if (!(flags&DRAWN)) return false;
  if ((flags&USES_FG) && ((PixmapPair*)picture)->fg != getcolor()) return false;
  if ((flags&USES_BG) && ((PixmapPair*)picture)->bg != getbgcolor()) return false;
  return true;
}

void Image::make_current() {
//  printf("make_current %p\n",this);
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

void Image::over(const fltk::Rectangle& from, const fltk::Rectangle& to) const {
  if (!drawn()) {
    const_cast<Image*>(this)->update();
    if (!(flags&DRAWN) || w_ < 1 || h_ < 1) return;
  }
  PixmapPair* picture = (PixmapPair*)(this->picture);
#if USE_XFT
  if (fl_rgba_xrender_format && picture->rgb) {
    fl_xrender_draw_image(picture->rgb,
			  (flags&OPAQUE) ? RGB : (flags&Image::MASK) ? fltk::MASK : RGBA,
			  from, to);
    return;
  }
#endif
  // XLib version:
  // unfortunately scaling does not work, so I just center and clip
  // to the transformed rectangle.
  // This is the rectangle I want to fill:
  Rectangle r2(to); transform(r2);
  // Center the image in that rectangle:
  Rectangle r1(r2,from.w(),from.h());
  // now figure out what area we will draw:
  Rectangle r(r1);
  if (r.w() >= r2.w()) {r.x(r2.x()); r.w(r2.w());}
  if (r.h() >= r2.h()) {r.y(r2.y()); r.h(r2.h());}
  // We must clip it because otherwise we can't do the alpha:
  if (!intersect_with_clip(r)) return;
  // figure out top-left corner of source:
  const int sx = r.x()-r1.x()+from.x();
  const int sy = r.y()-r1.y()+from.y();
  if (flags & OPAQUE) {
    XCopyArea(xdisplay, picture->rgb, xwindow, gc,
	      sx, sy, r.w(), r.h(), r.x(), r.y());
  } else if (picture->rgb) {
    // There seems to be no way to combine a mask with a clip region,
    // so I just replace it. This will be incorrect if the current
    // clip is not a rectangle
    XSetClipMask(xdisplay, gc, picture->alpha);
    XSetClipOrigin(xdisplay, gc, r.x()-sx, r.y()-sy);
    XCopyArea(xdisplay, picture->rgb, xwindow, gc,
	      sx, sy, r.w(), r.h(), r.x(), r.y());
    XSetClipOrigin(xdisplay, gc, 0, 0);
    fl_restore_clip();
  } else if (picture->alpha) {
    // bitmap with only the alpha...
    XSetStipple(xdisplay, gc, picture->alpha);
    XSetTSOrigin(xdisplay, gc, r.x()-sx, r.y()-sy);
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

void xbmImage::update() {
  Image* i = this;
  if (!i->picture) {
    open_display();
    i->picture = new PixmapPair;
  }
  PixmapPair* picture = (PixmapPair*)(i->picture);
  if (picture->alpha) XFreePixmap(xdisplay, picture->alpha);
  picture->alpha =
    XCreateBitmapFromData(xdisplay, xwindow, (char*)pixels(), (w()+7)&-8, h());
  i->flags = DRAWN;
}

