#include "Picture.h"

void Image::redraw() {
  if (picture) picture->drawn = false;
}

bool Image::drawn() const {
  return picture && picture->drawn;
}

void Image::make_current() {
  if (!picture) {
    open_display();
    picture = new fltk::Picture;
    // Current kludge is to rely on drawimage() creating the CGImageRef
    // object and stashing it here
    //picture->rgb = create something here...;
  }
  picture->drawn = true;
  picture->opaque = false;
  //draw_into(picture->rgb, w_, h_);
  fl_current_picture = picture;
}

void Image::set_alpha_bitmap(const uchar* bitmap, int w, int h) {
  if (!picture) {
    open_display();
    picture = new fltk::Picture;
  }
  if (picture->rgb) CGImageRelease(picture->rgb);
  static uchar reverse[16] = /* Bit reversal lookup table */
    { 0x00, 0x88, 0x44, 0xcc, 0x22, 0xaa, 0x66, 0xee, 
      0x11, 0x99, 0x55, 0xdd, 0x33, 0xbb, 0x77, 0xff };
  int rowBytes = (w+7)>>3 ;
  uchar *bmask = new uchar[rowBytes*h];
  uchar *dst = bmask;
  const uchar *src = bitmap;
  for ( int i=rowBytes*h; i>0; i--,src++ ) {
    *dst++ = ((reverse[*src & 0x0f] & 0xf0) | (reverse[(*src >> 4) & 0x0f] & 0x0f))^0xff;
  }
  CGDataProviderRef srcp = CGDataProviderCreateWithData( 0L, bmask, rowBytes*h, 0L);
  picture->rgb = CGImageMaskCreate( w, h, 1, 1, rowBytes, srcp, 0L, false);
  CGDataProviderRelease(srcp);
  picture->opaque = false;
  picture->drawn = true;
}

void Image::over(int x, int y) const {
  if (!picture || !picture->drawn || w_ < 1 || h_ < 1) return;
  transform(x,y);
  CGRect rect = { x,y,w_,h_ };
  fltk::begin_quartz_image(rect, Rectangle(0,0,w_,h_));
  CGContextDrawImage(fltk::quartz_gc, rect, picture->rgb);
  fltk::end_quartz_image();
}

bool Image::fills_rectangle() const {
  return picture && picture->opaque;
}
