bool Image::drawn() const {return flags & DRAWN;}

extern fltk::Image* fl_current_Image;

void Image::make_current() {
  if (!picture) {
    open_display();
    // Current kludge is to rely on drawimage() creating the CGImageRef
    // object and stashing it here
    // picture = create something here...;
  }
  flags = DRAWN;
  //draw_into((CGImageRef)picture, w_, h_);
  fl_current_Image = this;
}

void Image::over(int x, int y) const {
  if (!(flags&DRAWN) || !picture || w_ < 1 || h_ < 1) return;
  transform(x,y);
  CGRect rect = { x,y,w_,h_ };
  fltk::begin_quartz_image(rect, Rectangle(0,0,w_,h_));
  CGContextDrawImage(fltk::quartz_gc, rect, (CGImageRef)picture);
  fltk::end_quartz_image();
}

bool Image::fills_rectangle() const {
  return (flags&OPAQUE) && picture;
}

void Image::destroy() {
  if (fl_current_Image==this) fl_current_Image = 0;
  if (picture) {
    //stop_drawing((CGImageRef)picture);
    CGImageRelease((CGImageRef)picture);
    picture = 0;
  }
  flags = 0;
}

#include <fltk/xbmImage.h>

void xbmImage::_draw(const fltk::Rectangle& r) const
{
  if (!drawn()) {
    Image* i = const_cast<xbmImage*>(this);
    if (i->picture) CGImageRelease((CGImageRef)(i->picture));
    static uchar reverse[16] = /* Bit reversal lookup table */
      { 0x00, 0x88, 0x44, 0xcc, 0x22, 0xaa, 0x66, 0xee, 
	0x11, 0x99, 0x55, 0xdd, 0x33, 0xbb, 0x77, 0xff };
    int rowBytes = (w_+7)>>3 ;
    uchar *bmask = new uchar[rowBytes*h_];
    uchar *dst = bmask;
    const uchar *src = (uchar*)array;
    for ( int i=rowBytes*h_; i>0; i--,src++ ) {
      *dst++ = ((reverse[*src & 0x0f] & 0xf0) | (reverse[(*src >> 4) & 0x0f] & 0x0f))^0xff;
    }
    CGDataProviderRef srcp = CGDataProviderCreateWithData( 0L, bmask, rowBytes*h_, 0L);
    i->picture = CGImageMaskCreate( w_, h_, 1, 1, rowBytes, srcp, 0L, false);
    CGDataProviderRelease(srcp);
    i->flags = DRAWN;
  }
  Image::_draw(r);
}
