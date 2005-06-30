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
  if (!picture->rgb) {
    picture->rgb = XCreatePixmap(xdisplay, xwindow, w_, h_, xvisual->depth);
  }
  picture->fg = fltk::getcolor();
  picture->bg = fltk::getbgcolor();
  flags = DRAWN | OPAQUE;
  draw_into(picture->rgb, w_, h_);
  fl_current_Image = this;
}

void fl_restore_clip(); // in rect.C

void Image::over(int x, int y) const {
  if (!(flags&DRAWN) || w_ < 1 || h_ < 1) return;
  transform(x,y);
  Rectangle r(x,y,w_,h_);
  if (!intersect_with_clip(r)) return;
  PixmapPair* picture = (PixmapPair*)(this->picture);
  if (picture->alpha) {
    int ox = x; while (ox < 0) ox += w_;
    int oy = y; while (oy < 0) oy += h_;
    if (picture->rgb) {
      // There seems to be no way to combine a mask with a clip region,
      // so I just replace it. This will be incorrect if the current
      // clip is not a rectangle
      XSetClipMask(xdisplay, gc, picture->alpha);
      XSetClipOrigin(xdisplay, gc, ox, oy);
      XCopyArea(xdisplay, picture->rgb, xwindow, gc,
		r.x()-x, r.y()-y, r.w(), r.h(), r.x(), r.y());
      XSetClipOrigin(xdisplay, gc, 0, 0);
      fl_restore_clip();
    } else {
      // bitmap with only the alpha...
      XSetStipple(xdisplay, gc, picture->alpha);
      XSetTSOrigin(xdisplay, gc, ox, oy);
      XSetFillStyle(xdisplay, gc, FillStippled);
      XFillRectangle(xdisplay, xwindow, gc, r.x(), r.y(), r.w(), r.h());
      XSetFillStyle(xdisplay, gc, FillSolid);
    }
  } else {
    XCopyArea(xdisplay, picture->rgb, xwindow, gc,
	      r.x()-x, r.y()-y, r.w(), r.h(), r.x(), r.y());
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

