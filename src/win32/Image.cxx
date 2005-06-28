#include "Picture.h"

// Extra bitblt functions:
#define NOTSRCAND       0x00220326 /* dest = (NOT source) AND dest */
#define NOTSRCINVERT    0x00990066 /* dest = (NOT source) XOR dest */
#define SRCORREVERSE    0x00DD0228 /* dest = source OR (NOT dest) */
#define SRCNAND         0x007700E6 /* dest = NOT (source AND dest) */
#define MASKPAT         0x00E20746 /* dest = (src & pat) | (!src & dst) */
#define COPYFG          0x00CA0749 /* dest = (pat & src) | (!pat & dst) */
#define COPYBG          0x00AC0744 /* dest = (!pat & src) | (pat & dst) */

# if defined(__MINGW32__) || defined(__CYGWIN__) // || defined(__BORLANDC__)
// AlphaBlend IS declared in these but only when WINVER is >= 500
extern "C" {
  WINGDIAPI BOOL  WINAPI AlphaBlend(HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION);
}
#  define AC_SRC_ALPHA		  0x01
# endif

void Image::redraw() {
  if (picture) picture->drawn = false;
}

bool Image::drawn() const {
  return picture && picture->drawn
    && (!picture->mask || picture->fg == getcolor());
}

void Image::make_current() {
  if (!picture) {
    open_display();
    picture = new fltk::Picture;
    //picture->rgb = CreateCompatibleBitmap(getDC(), w_, h_);

    // Use CreateDIBSection instead, it seems to be only reliable way to
    // make AlphaBlend function working correctly always..
    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = w_;
    bmi.bmiHeader.biHeight = h_;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = w_ * h_ * 4;
    picture->rgb =
      CreateDIBSection(getDC(), &bmi, DIB_RGB_COLORS, NULL, NULL, 0x0);
  }
  picture->mask = false;
  picture->isbitmap = false;
  picture->opaque = false;
  picture->drawn = true;
  draw_into(picture->rgb, w_, h_);
  fl_current_picture = picture;
}

void Image::set_alpha_bitmap(const uchar* bitmap, int w, int h) {
  if (!picture) {
    open_display();
    picture = new fltk::Picture;
  }
  if (picture->rgb) DeleteObject(picture->rgb);
  picture->isbitmap = true;
  picture->opaque = false;
  // this won't work when the user changes display mode during run or
  // has two screens with differnet depths
  static uchar hiNibble[16] =
  { 0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
    0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0 };
  static uchar loNibble[16] =
  { 0x00, 0x08, 0x04, 0x0c, 0x02, 0x0a, 0x06, 0x0e,
    0x01, 0x09, 0x05, 0x0d, 0x03, 0x0b, 0x07, 0x0f };
  int Bpr = (w+7)/8;			//: bytes per row
  int pad = Bpr&1, w1 = (w+7)/8; //shr = ((w-1)&7)+1;
  uchar *newarray = new uchar[(Bpr+pad)*h], *dst = newarray;
  const uchar* src = bitmap;
  for (int i=0; i<h; i++) {
    //: this is slooow, but we do it only once per pixmap
    for (int j=w1; j>0; j--) {
      uchar b = *src++;
      *dst++ = ( hiNibble[b&15] ) | ( loNibble[(b>>4)&15] );
    }
    if (pad)
      *dst++ = 0;
  }
  picture->rgb = CreateBitmap(w, h, 1, 1, newarray);
  delete[] newarray;
  picture->drawn = true;
}

// Link against msimg32 lib to get the AlphaBlend function:
#if defined(_MSC_VER)
# pragma comment(lib, "msimg32.lib")
#endif

void Image::over(int x, int y) const {
  if (!picture || !picture->drawn || w_ < 1 || h_ < 1) return;
  transform(x,y);
  HDC tempdc = CreateCompatibleDC(dc);
  SelectObject(tempdc, picture->rgb);
  if (!picture->isbitmap) {
    if (picture->opaque) {
      BitBlt(dc, x, y, w_, h_, tempdc, 0, 0, SRCCOPY);
    } else {
      BLENDFUNCTION m_bf;
      m_bf.BlendOp = AC_SRC_OVER;
      m_bf.BlendFlags = 0;
      m_bf.AlphaFormat = 1; //AC_SRC_ALPHA;
      m_bf.SourceConstantAlpha = 0xFF;
      AlphaBlend(dc, x, y, w_, h_, tempdc, 0, 0, w_, h_, m_bf);
    }
  } else {
    setbrush();
    // On my machine this does not draw the right color! But lots of
    // documentation indicates that this should work. Seems to work
    // a bit better if you set the text color:
    SetTextColor(dc, 0); //current_xpixel^xpixel(getbgcolor()));
    BitBlt(dc, x, y, w_, h_, tempdc, 0, 0, MASKPAT);
  }
  DeleteDC(tempdc);
}

bool Image::fills_rectangle() const {
  return picture && picture->opaque;
}
