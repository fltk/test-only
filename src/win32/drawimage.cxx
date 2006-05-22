//
// "$Id$"
//
// _WIN32 image drawing code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2006 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

// This needs improvement so that ARGB32 and RGB32 formats don't waste
// time copying the image to temporary storage!

////////////////////////////////////////////////////////////////
//
// WARNING: This file SHOULDN'T be used directly!
//   - It's included from /src/drawimage.cxx
//
////////////////////////////////////////////////////////////////

#include <fltk/Color.h>
#include <fltk/Font.h>
#include <fltk/Image.h>
#include <fltk/draw.h>
#include <config.h>
#include <stdio.h>
#include <windows.h>

using namespace fltk;

#if defined(__MINGW32__) || defined(__CYGWIN__) // || defined(__BORLANDC__)
// AlphaBlend IS declared in these but only when WINVER is >= 500
extern "C" {
  WINGDIAPI BOOL  WINAPI AlphaBlend(HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION);
}
#endif

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

#define MAXBUFFER 0x40000 // 256k

extern fltk::Image* fl_current_Image;
class fltk::DrawImageHelper {
public:
  static void setmask() {
    // special MASK detector, must be drawing the same color as last time:
    Color c = get_color_index(fltk::getcolor());
    fl_current_Image->flags = ((int)(c&0xffffff00)|Image::DRAWN|Image::USES_FG);
  }
  static void clear_opaque() {
    fl_current_Image->flags = fltk::Image::DRAWN;
  }
};

static void innards(const uchar *buf, PixelType type,
		    const fltk::Rectangle& r1,
		    int linedelta,
		    DrawImageCallback cb, void* userdata)
{
  fltk::Rectangle r; transform(r1,r);

  const int W = r1.w();
  const int H = r1.h();
  const int delta = depth(type);

  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(BITMAPINFO));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  if (buf && (type == RGB32 || type == ARGB32)) {
    if (linedelta >= 0) {
      bmi.bmiHeader.biWidth = linedelta/4;
      bmi.bmiHeader.biHeight = -H;
      bmi.bmiHeader.biSizeImage = linedelta*H;
    } else {
      bmi.bmiHeader.biWidth = -linedelta/4;
      bmi.bmiHeader.biHeight = H;
      buf += linedelta*(H-1);
      bmi.bmiHeader.biSizeImage = -linedelta*H;
    }
  } else {
    static U32* buffer;
    int size = 4*W*H;
    static long buffer_size;
    if (size > buffer_size) {
      delete[] buffer;
      buffer_size = size;
      buffer = new U32[(size+3)/4];
    }
    static U32* line_buffer;
    if (!buf) {
      int size = W*delta;
      static int line_buf_size;
      if (size > line_buf_size) {
	delete[] line_buffer;
	line_buf_size = size;
	line_buffer = new U32[(size+3)/4];
      }
    }
    // needed for MASK:
    uchar mr,mg,mb;
    if (type == MASK)
      fltk::split_color(fltk::getcolor(),mr,mg,mb);

    for (int y=0; y<H; y++) {
      uchar* to = (uchar*)buffer+W*(H-y-1)*4;
      const uchar* from;
      if (!buf) { // run the converter:
        from = cb(userdata, 0, y, W, (uchar*)line_buffer);
      } else {
        from = buf;
        buf += linedelta;
      }
      int i;
      switch (type) {
      case MONO:
	for (i=W; i--; from += delta, to += 4) {
	  to[0] = to[1] = to[2] = *from; to[3] = 0xff;
	}
	break;
      case MASK:
	for (i=W; i--; from += delta, to += 4) {
	  uchar v = 255-*from;
	  to[0] = (mb*v)>>8;
	  to[1] = (mg*v)>>8;
	  to[2] = (mr*v)>>8;
	  to[3] = v;
	}
	break;
      case RGBx:
      case RGB:
	for (i=W; i--; from += delta, to += 4) {
	  to[0] = from[2];
	  to[1] = from[1];
	  to[2] = from[0];
	  to[3] = 0xff;
	}
	break;
      case RGBA:
	for (i=W; i--; from += delta, to += 4) {
	  to[0] = from[2];
	  to[1] = from[1];
	  to[2] = from[0];
	  to[3] = from[3];
	}
	break;
      case RGB32:
      case ARGB32:
	memcpy(to, from, 4*W);
	break;
      default:
	  break; // please gcc3
      }
    }

    bmi.bmiHeader.biWidth = W;
    bmi.bmiHeader.biHeight = H;
    bmi.bmiHeader.biSizeImage = size;
    buf = (uchar*)buffer;
  }

  bool hasalpha = type==MASK || type==RGBA || type==ARGB32;
  if (fl_current_Image) {
    SetDIBitsToDevice(dc, r.x(), r.y(), W, H, 0, 0, 0, H,
		      (LPSTR)buf,
		      &bmi,
		      DIB_RGB_COLORS
		      );
    if (type == MASK)
      DrawImageHelper::setmask();
    else if (hasalpha)
      DrawImageHelper::clear_opaque();
  } else if (!hasalpha && r.w()==W && r.h()==H) {
    SetDIBitsToDevice(dc, r.x(), r.y(), W, H, 0, 0, 0, H,
		      (LPSTR)buf,
		      &bmi,
		      DIB_RGB_COLORS
		      );
  } else {
    HBITMAP rgb = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, NULL, NULL, 0);
    HDC tempdc = CreateCompatibleDC(dc);
    SelectObject(tempdc, rgb);
    SetDIBitsToDevice(tempdc, 0, 0, W, H, 0, 0, 0, H,
		      (LPSTR)buf,
		      &bmi,
		      DIB_RGB_COLORS
		      );
    BLENDFUNCTION m_bf;
    m_bf.BlendOp = AC_SRC_OVER;
    m_bf.BlendFlags = 0;
    m_bf.AlphaFormat = hasalpha ? 1 /*AC_SRC_ALPHA*/ : 0;
    m_bf.SourceConstantAlpha = 0xFF;
    AlphaBlend(dc, r.x(), r.y(), r.w(), r.h(),
	       tempdc, 0, 0, W, H, m_bf);
    DeleteDC(tempdc);
    DeleteObject(rgb);
  }
}

#define DITHER_FILLRECT false

//
// End of "$Id$".
//
