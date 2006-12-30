static void releaser(void*, const void* data, size_t size) {
  delete[] (U32*)data;
}

struct fltk::Picture {
  int w, h, linedelta;
  unsigned long n; // bytes used
  uchar* data;
  CGImageRef img;

  Picture(PixelType p, int w, int h, int ld, const uchar* d=0) {
    this->w = w;
    this->h = h;
    this->linedelta = ld;
    if (d) {n = 0; data = (uchar*)d;}
    else {n = ld*h; data = (uchar*)(new U32[n/4]);}

    // create an image context
    static CGColorSpaceRef rgbcolorspace = 0;
    static CGColorSpaceRef graycolorspace = 0;
    if (!rgbcolorspace) rgbcolorspace = CGColorSpaceCreateDeviceRGB();
    CGColorSpaceRef colorspace = rgbcolorspace;
    CGDataProviderRef src =
      CGDataProviderCreateWithData( 0L, data, ld*h, d ? 0 : releaser);
    CGImageAlphaInfo bitmapInfo;
    switch (p) {
    case MASK:
      bitmapInfo = kCGImageAlphaOnly;
      break;
    case MONO:
      bitmapInfo = kCGImageAlphaNone;
      if (!graycolorspace) graycolorspace = CGColorSpaceCreateDeviceGray();
      colorspace = graycolorspace;
      break;
    case RGBx: bitmapInfo = kCGImageAlphaNone; break;
    case RGB: bitmapInfo = kCGImageAlphaNone; break;
    case RGBA: bitmapInfo = kCGImageAlphaPremultipliedLast; break;
    case RGBM: bitmapInfo = kCGImageAlphaLast; break;
#if __BIG_ENDIAN__
# define BE(x) x
#else
# define BE(x) CGImageAlphaInfo(x|(2<<12))
#endif
    case RGB32: bitmapInfo = BE(kCGImageAlphaNoneSkipFirst); break;
    case ARGB32: bitmapInfo = BE(kCGImageAlphaPremultipliedFirst); break;
    case MRGB32: bitmapInfo = BE(kCGImageAlphaFirst); break;
    default: bitmapInfo = kCGImageAlphaNone; break;
    }
    if (p==MASK)
      img = CGImageMaskCreate(w, h,
                              8, // bitsPerComponent
                              8, // bitsPerPixel
                              ld, // bytesPerRow
                              src, // provider
                              0L, // decode array (?)
                              true); // shouldInterpolate
    else
      img = CGImageCreate(w, h,
                          8, // bitsPerComponent
                          8*depth(p), // bitsPerPixel
                          ld, // bytesPerRow
                          colorspace,
                          bitmapInfo, // bitmapInfo
                          src, // provider
                          0L, // decode array (?)
                          true, // shouldInterpolate
                          kCGRenderingIntentDefault);
    CGDataProviderRelease(src);
  }

  // special one for xbmImage:
  Picture(int w, int h, CGImageRef i) {
    this->w = w;
    this->h = h;
    n = 0;
    data = 0;
    img = i;
  }

  ~Picture() {
    CGImageRelease(img);
  }

};

unsigned long Image::memused_;

int Image::buffer_width() const {
  if (picture) return picture->w;
  return w();
}

int Image::buffer_height() const {
  if (picture) return picture->h;
  return h();
}

int Image::buffer_linedelta() const {
  if (picture) return picture->linedelta;
  return (w_*depth()+3)&-4;
}

int Image::buffer_depth() const {
  return depth();
}

fltk::PixelType Image::buffer_pixeltype() const {
  return pixeltype();
}

unsigned long Image::mem_used() const {
  if (picture) return picture->n;
  return 0;
}

const uchar* Image::buffer() const {
  if (picture) return picture->data;
  return const_cast<Image*>(this)->buffer();
}

uchar* Image::buffer() {
  if (!picture) {
    open_display();
    picture = new Picture(pixeltype_, w_, h_, (w_*depth()+3)&-4);
    memused_ += picture->n;
  }
  return picture->data;
}

void Image::destroy() {
  if (!picture) return;
  if (picture->n > memused_) memused_ -= picture->n; else memused_ = 0;
  delete picture;
  picture = 0;
  flags &= ~FETCHED;
}

void Image::setpixeltype(PixelType p) {
  if (pixeltype_ != p) destroy();
  pixeltype_ = p;
}

void Image::setsize(int w, int h) {
  if (picture && (w > picture->w || h > picture->h)) destroy();
  w_ = w;
  h_ = h;
}

uchar* Image::linebuffer(int y) {
  buffer();
  return picture->data+y*picture->linedelta;
}

void Image::setpixels(const uchar* buf, int y) {
  buffer();
  flags &= ~COPIED;
  uchar* to = picture->data+y*picture->linedelta;
  if (buf != to) memcpy(to, buf, width()*depth());
}

void Image::setpixels(const uchar* buf, const fltk::Rectangle& r, int linedelta)
{
  if (r.empty()) return;
  buffer();
  flags &= ~COPIED;
  uchar* to = picture->data+r.y()*picture->linedelta+r.x()*buffer_depth();
  // see if we can do it all at once:
  if (r.w() == picture->w && (r.h()==1 || linedelta == picture->linedelta)) {
    memcpy(to, buf, r.h()*linedelta);
  } else {
    for (int y = 0; y < r.h(); y++) {
      memcpy(to, buf, r.w()*depth());
      buf += linedelta;
      to += picture->linedelta;
    }
  }
}

void Image::fetch_if_needed() const {
  if (!(flags&FETCHED)) {
    Image* thisimage = const_cast<Image*>(this);
    thisimage->fetch();
    thisimage->flags |= FETCHED;
    // make errors have non-zero size:
    if (w_ < 0 || h_ < 0) {
      thisimage->destroy();
      thisimage->w_ = thisimage->h_ = 12;
    }
  }
}

extern void fl_set_quartz_ctm();

void Image::draw(const fltk::Rectangle& from, const fltk::Rectangle& to) const {
  fetch_if_needed();
  if (!picture) {fillrect(to); return;}
  CGContextSaveGState(quartz_gc);
  fl_set_quartz_ctm();
  if (!from.x() && !from.y() && from.w()==w_ && from.h()==h_) {
    CGRect rect = {{to.x(), -to.y()}, {to.w(), -to.h()}};
    CGContextDrawImage(quartz_gc, rect, picture->img);
  } else {
#if 1 // does not work before Tiger!!!
    CGRect irect = {{from.x(), from.y()}, {from.w(), from.h()}};
    CGImageRef clip = CGImageCreateWithImageInRect(picture->img, irect);
    CGRect rect = {{to.x(), -to.y()}, {to.w(), -to.h()}};
    CGContextDrawImage(quartz_gc, rect, clip);
    CGImageRelease(clip);
#endif
  }
  CGContextRestoreGState(quartz_gc);
}

void Image::setimage(const uchar* source, PixelType p, int w, int h, int ld)
{
  setsize(w,h);
  setpixeltype(p);
  if (ld & 3) {
    // can't handle linedelta that is not a multiple of 4?
    if (picture && !(picture->n)) destroy();
    setpixels(source, Rectangle(w, h), ld);
  } else {
    destroy();
    picture = new Picture(p, w, h, ld, source);
    flags = FETCHED;
  }
}

////////////////////////////////////////////////////////////////

#include <fltk/xbmImage.h>

bool xbmImage::fetch()
{
  destroy();
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
  CGDataProviderRef srcp =
    CGDataProviderCreateWithData( 0L, bmask, rowBytes*h_, releaser);
  picture = new Picture(w_,h_,CGImageMaskCreate( w_, h_, 1, 1, rowBytes, srcp, 0L, true));
  CGDataProviderRelease(srcp);
  flags = FETCHED;
  return true;
}

////////////////////////////////////////////////////////////////

// drawimage() calls this to see if a direct draw will work. Returns
// true if successful, false if an Image must be used to emulate it.

static bool innards(const uchar *buf, fltk::PixelType type,
		    const fltk::Rectangle& r1,
		    int linedelta,
		    DrawImageCallback cb, void* userdata)
{
  return false;
}

//
// End of "$Id: Image.cxx 1478 2006-12-30 03:54:06Z spitzak $".
//
