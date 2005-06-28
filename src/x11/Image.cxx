#include "Picture.h"

void Image::redraw() {
  if (picture) {
    picture->drawn = false;
    if (picture->alpha) {
      XFreePixmap(xdisplay, picture->alpha);
      picture->alpha = 0;
    }
  }
}

bool Image::drawn() const {
  return picture && picture->drawn
    && (!picture->mask ||
	picture->fg == getcolor() && picture->bg == getbgcolor());
}

void Image::make_current() {
  if (!picture) {
    open_display();
    picture = new fltk::Picture;
    picture->rgb = XCreatePixmap(xdisplay, xwindow, w_, h_, xvisual->depth);
  }
  picture->mask = false;
  picture->drawn = true;
  draw_into(picture->rgb, w_, h_);
  fl_current_picture = picture;
}

void Image::set_alpha_bitmap(const uchar* bitmap, int w, int h) {
  if (!picture) {
    open_display();
    picture = new fltk::Picture;
  }
  if (picture->alpha) XFreePixmap(xdisplay, picture->alpha);
  picture->alpha =
    XCreateBitmapFromData(xdisplay, xwindow, (char*)bitmap, (w+7)&-8, h);
  picture->drawn = true;
}

void fl_restore_clip(); // in rect.C

void Image::over(int x, int y) const {
  if (!picture || !picture->drawn || w_ < 1 || h_ < 1) return;
  transform(x,y);
  Rectangle r(x,y,w_,h_);
  if (!intersect_with_clip(r)) return;
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
  } else if (picture->rgb) {
      XCopyArea(xdisplay, picture->rgb, xwindow, gc,
		r.x()-x, r.y()-y, r.w(), r.h(), r.x(), r.y());
  }
}

bool Image::fills_rectangle() const {
  return picture && !picture->alpha;
}
