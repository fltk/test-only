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

// DM C/C++ compiler comes with very old Win32 API headers. Modifying existing
// ones (from MS PSDK for example) is time-consuming job, so I have decided
// to declare necessary stuff here.
#if __DMC__ || __SC__ || __RCC__
  typedef struct _BLENDFUNCTION {
    BYTE BlendOp;
    BYTE BlendFlags;
    BYTE SourceConstantAlpha;
    BYTE AlphaFormat;
  } BLENDFUNCTION,*PBLENDFUNCTION,*LPBLENDFUNCTION;
  extern "C" {
    WINGDIAPI BOOL WINAPI AlphaBlend(HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION);
  }
  #define AC_SRC_OVER   0x00
  #define AC_SRC_ALPHA  0x01
#endif // __DMC__

bool Image::drawn() const {
  if (!(flags&DRAWN)) return false;
  if (flags&USES_BG) return true; // bitmap
  if (!(flags&USES_FG)) return true;
  // special MASK detector, must be drawing the same color as last time:
  Color c = get_color_index(fltk::getcolor());
  return ((int)(c&0xffffff00)|DRAWN|USES_FG) == flags;
}

extern fltk::Image* fl_current_Image;

void Image::make_current() {
  if (!picture) {
    open_display();
    //picture = CreateCompatibleBitmap(getDC(), w_, h_);

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
    picture =
      CreateDIBSection(getDC(), &bmi, DIB_RGB_COLORS, NULL, NULL, 0x0);
  }
  flags = OPAQUE|DRAWN;
  draw_into((HBITMAP)picture, w_, h_);
  fl_current_Image = this;
}

// Link against msimg32 lib to get the AlphaBlend function:
#if defined(_MSC_VER)
# pragma comment(lib, "msimg32.lib")
#endif

void Image::over(const fltk::Rectangle& from, const fltk::Rectangle& to) const {
  if (!drawn()) {
    const_cast<Image*>(this)->update();
    if (!picture || w_ < 1 || h_ < 1) return;
  }
  fltk::Rectangle R(to);
  fltk::Rectangle F(from); fltk::transform(F,R);
  HDC tempdc = CreateCompatibleDC(dc);
  SelectObject(tempdc, (HBITMAP)picture);
  if (!(flags&USES_BG)) { // not a bitmap
    if ((flags&OPAQUE) && R.w()==from.w() && R.h()==from.h()) {
      BitBlt(dc, R.x(), R.y(), from.w(), from.h(), tempdc, from.x(), from.y(), SRCCOPY);
    } else {
      BLENDFUNCTION m_bf;
      m_bf.BlendOp = AC_SRC_OVER;
      m_bf.BlendFlags = 0;
      m_bf.AlphaFormat = (flags&OPAQUE) ? 0 : 1; //AC_SRC_ALPHA;
      m_bf.SourceConstantAlpha = 0xFF;
      AlphaBlend(dc, R.x(), R.y(), R.w(), R.h(), tempdc,
		 from.x(), from.y(), from.w(), from.h(), m_bf);
    }
  } else {
    // Bitmap implementation:
    // This does not work! If anybody can figure out the correct
    // BitBlt incantation, please tell me! I want 1's in the mask
    // to be the current color, and 0's to be unchanged.
    // Possibly this is a bug in our Nvidia driver?
    setbrush();
    SetTextColor(dc, 0); //current_xpixel^xpixel(getbgcolor()));
    BitBlt(dc, R.x(), R.y(), R.w(), R.h(), tempdc, from.x(), from.y(), MASKPAT);
  }
  DeleteDC(tempdc);
}

bool Image::fills_rectangle() const {
  return (flags&OPAQUE) && picture;
}

void Image::destroy() {
  if (fl_current_Image==this) fl_current_Image = 0;
  if (picture) {
    stop_drawing((HBITMAP)picture);
    DeleteObject((HBITMAP)picture);
    picture = 0;
  }
  flags = 0;
}

#include <fltk/xbmImage.h>

void xbmImage::update()
{
  destroy();
  static uchar hiNibble[16] =
    { 0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
      0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0 };
  static uchar loNibble[16] =
    { 0x00, 0x08, 0x04, 0x0c, 0x02, 0x0a, 0x06, 0x0e,
      0x01, 0x09, 0x05, 0x0d, 0x03, 0x0b, 0x07, 0x0f };
  int Bpr = (w_+7)/8;			//: bytes per row
  int pad = Bpr&1, w1 = (w_+7)/8; //shr = ((w-1)&7)+1;
  uchar *newarray = new uchar[(Bpr+pad)*h_], *dst = newarray;
  const uchar* src = pixels();
  for (int y=0; y<h_; y++) {
    //: this is slooow, but we do it only once per pixmap
    for (int j=w1; j>0; j--) {
      uchar b = *src++;
      *dst++ = ( hiNibble[b&15] ) | ( loNibble[(b>>4)&15] );
    }
    if (pad)
      *dst++ = 0;
  }
  picture = CreateBitmap(w_, h_, 1, 1, newarray);
  delete[] newarray;
  flags = Image::DRAWN|Image::USES_BG;
}
