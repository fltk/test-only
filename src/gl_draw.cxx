//
// "$Id$"
//
// OpenGL drawing support routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2018 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// Functions from <FL/gl.h>
// See also Fl_Gl_Window and gl_start.cxx

#include "config_lib.h"
#if defined(FL_PORTING)
#  pragma message "FL_PORTING: implement OpenGL text rendering here"
#endif

#include "flstring.h"
#if HAVE_GL || defined(FL_DOXYGEN)

#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/gl_draw.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window_Driver.H>

#if defined(FL_CFG_GFX_QUARTZ)
#include "drivers/Quartz/Fl_Font.H"
#  define GENLISTSIZE 0

#elif defined(FL_CFG_GFX_GDI)
#include "drivers/GDI/Fl_Font.H"
#define GENLISTSIZE 0x10000

#elif defined(FL_CFG_GFX_XLIB)
#include "drivers/Xlib/Fl_Font.H"
#if USE_XFT
#  define GENLISTSIZE 256
#else
#  define GENLISTSIZE 0x10000
#endif // USE_XFT

#endif
#include <FL/fl_utf8.h>


/** Returns the current font's height */
int   gl_height() {return fl_height();}
/** Returns the current font's descent */
int   gl_descent() {return fl_descent();}
/** Returns the width of the string in the current fnt */
double gl_width(const char* s) {return fl_width(s);}
/** Returns the width of n characters of the string in the current font */
double gl_width(const char* s, int n) {return fl_width(s,n);}
/** Returns the width of the character in the current font */
double gl_width(uchar c) {return fl_width(c);}

static Fl_Font_Descriptor *gl_fontsize;


/**
  Sets the current OpenGL font to the same font as calling fl_font()
  */
void  gl_font(int fontid, int size) {
  fl_font(fontid, size);
  Fl_Font_Descriptor *fl_fontsize = fl_graphics_driver->font_descriptor();
  Fl_Gl_Window_Driver::global()->gl_bitmap_font(fl_fontsize);
  gl_fontsize = fl_fontsize;
}

#if defined(FL_CFG_GFX_QUARTZ) || defined(FL_CFG_GFX_GDI) || defined(FL_CFG_GFX_XLIB)

void gl_remove_displaylist_fonts()
{
# if HAVE_GL
  // clear variables used mostly in fl_font
  fl_graphics_driver->font(0, 0);

  for (int j = 0 ; j < FL_FREE_FONT ; ++j)
  {
    Fl_Font_Descriptor* past = 0;
    Fl_Fontdesc* s    = fl_fonts + j ;
    Fl_Font_Descriptor* f    = s->first;
    while (f != 0) {
      if(f->listbase) {
        if(f == s->first) {
          s->first = f->next;
        }
        else {
          past->next = f->next;
        }

        // It would be nice if this next line was in a destructor somewhere
        glDeleteLists(f->listbase, GENLISTSIZE);

        Fl_Font_Descriptor* tmp = f;
        f = f->next;
        delete tmp;
      }
      else {
        past = f;
        f = f->next;
      }
    }
  }
#endif // HAVE_GL
}
#endif


/**
  Draws an array of n characters of the string in the current font
  at the current position.
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str, int n) {
  Fl_Gl_Window_Driver::global()->draw_string(str, n);
}

void Fl_Gl_Window_Driver::draw_string(const char* str, int n) {
  static unsigned short *buf = NULL;
  static int l = 0;
  int wn = fl_utf8toUtf16(str, n, (unsigned short*)buf, l);
  if(wn >= l) {
    buf = (unsigned short*) realloc(buf, sizeof(unsigned short) * (wn + 1));
    l = wn + 1;
    wn = fl_utf8toUtf16(str, n, (unsigned short*)buf, l);
  }
  n = wn;
  
  int i;
  for (i = 0; i < n; i++) {
    unsigned int r;
    r = (str[i] & 0xFC00) >> 10;
    //if (!gl_fontsize->glok[r]) get_list(r);
    this->get_list(gl_fontsize, r);
  }
  glCallLists(n, GL_UNSIGNED_SHORT, buf);
}

/**
  Draws n characters of the string in the current font at the given position 
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str, int n, int x, int y) {
  glRasterPos2i(x, y);
  gl_draw(str, n);
}

/**
  Draws n characters of the string in the current font at the given position 
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str, int n, float x, float y) {
  glRasterPos2f(x, y);
  gl_draw(str, n);
}

/**
  Draws a nul-terminated string in the current font at the current position
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str) {
  gl_draw(str, strlen(str));
}

/**
  Draws a nul-terminated string in the current font at the given position 
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str, int x, int y) {
  gl_draw(str, strlen(str), x, y);
}

/**
  Draws a nul-terminated string in the current font at the given position 
 \see On the Mac OS X platform, see gl_texture_pile_height(int)
  */
void gl_draw(const char* str, float x, float y) {
  gl_draw(str, strlen(str), x, y);
}

static void gl_draw_invert(const char* str, int n, int x, int y) {
  glRasterPos2i(x, -y);
  gl_draw(str, n);
}

/**
  Draws a string formatted into a box, with newlines and tabs expanded,
  other control characters changed to ^X. and aligned with the edges or
  center. Exactly the same output as fl_draw().
  */
void gl_draw(
  const char* str, 	// the (multi-line) string
  int x, int y, int w, int h, 	// bounding box
  Fl_Align align)
{
  fl_draw(str, x, -y-h, w, h, align, gl_draw_invert, NULL, 0);
}

/** Measure how wide and tall the string will be when drawn by the gl_draw() function */
void gl_measure(const char* str, int& x, int& y) {
  fl_measure(str,x,y,0);
}

/**
  Outlines the given rectangle with the current color.
  If Fl_Gl_Window::ortho() has been called, then the rectangle will
  exactly fill the given pixel rectangle.
  */
void gl_rect(int x, int y, int w, int h) {
  if (w < 0) {w = -w; x = x-w;}
  if (h < 0) {h = -h; y = y-h;}
  glBegin(GL_LINE_STRIP);
  glVertex2i(x+w-1, y+h-1);
  glVertex2i(x+w-1, y);
  glVertex2i(x, y);
  glVertex2i(x, y+h-1);
  glVertex2i(x+w, y+h-1);
  glEnd();
}


/**
  Sets the curent OpenGL color to an FLTK color.

  For color-index modes it will use fl_xpixel(c), which is only
  right if the window uses the default colormap!
  */
void gl_color(Fl_Color i) {
  if (Fl_Gl_Window_Driver::global()->overlay_color(i)) return;
  uchar red, green, blue;
  Fl::get_color(i, red, green, blue);
  glColor3ub(red, green, blue);
}


#if defined(FL_CFG_GFX_XLIB)
#include <FL/platform.H>
#include <GL/glx.h>


void Fl_X11_Gl_Window_Driver::gl_bitmap_font(Fl_Font_Descriptor *fl_fontsize) {
  if (!fl_fontsize->listbase) {
#if !USE_XFT
    fl_fontsize->listbase = glGenLists(GENLISTSIZE);
#else // Fltk-1.1.8 style GL font selection
    // FIXME:  warning Ideally, for XFT, we really need a glXUseXftFont implementation here...
    // FIXME:  warning GL font selection is basically wrong here
    /* OksiD had a fairly sophisticated scheme for storing multiple X fonts in a XUtf8FontStruct,
     * then sorting through them at draw time (for normal X rendering) to find which one can
     * render the current glyph... But for now, just use the first font in the list for GL...
     */
     XFontStruct *font = fl_xfont.value();
     int base = font->min_char_or_byte2;
     int count = font->max_char_or_byte2-base+1;
     fl_fontsize->listbase = glGenLists(GENLISTSIZE);
     glXUseXFont(font->fid, base, count, fl_fontsize->listbase+base);
#endif // !USE_XFT
     }
     glListBase(fl_fontsize->listbase);
}


void Fl_X11_Gl_Window_Driver::get_list(Fl_Font_Descriptor *gl_fd, int r) {
  if (gl_fd->glok[r]) return;
  gl_fd->glok[r] = 1;
# if USE_XFT
  // FIXME
# else
  unsigned int ii = r * 0x400;
  for (int i = 0; i < 0x400; i++) {
    XFontStruct *font = NULL;
    unsigned short id;
    fl_XGetUtf8FontAndGlyph(gl_fd->font, ii, &font, &id);
    if (font) glXUseXFont(font->fid, id, 1, gl_fd->listbase+ii);
    ii++;
  }
# endif
}

#if HAVE_GL_OVERLAY
extern uchar fl_overlay;
int Fl_X11_Gl_Window_Driver::overlay_color(Fl_Color i) {
  if (fl_overlay) {glIndexi(int(fl_xpixel(i))); return 1;}
  return 0;
}
#endif // HAVE_GL_OVERLAY
#endif // FL_CFG_GFX_XLIB


#if defined(FL_CFG_GFX_GDI)

void Fl_WinAPI_Gl_Window_Driver::gl_bitmap_font(Fl_Font_Descriptor *fl_fontsize) {
  if (!fl_fontsize->listbase) {
    fl_fontsize->listbase = glGenLists(GENLISTSIZE);
     /* old, unused WIN32 code
     int base = fl_fontsize->metr.tmFirstChar;
     int count = fl_fontsize->metr.tmLastChar-base+1;
     HFONT oldFid = (HFONT)SelectObject((HDC)fl_graphics_driver->gc(), fl_fontsize->fid);
     fl_fontsize->listbase = glGenLists(256);
     wglUseFontBitmaps((HDC)fl_graphics_driver->gc(), base, count, fl_fontsize->listbase+base);
     SelectObject((HDC)fl_graphics_driver->gc(), oldFid);
     */
  }
  glListBase(fl_fontsize->listbase);
}


void Fl_WinAPI_Gl_Window_Driver::get_list(Fl_Font_Descriptor *gl_fd, int r) {
  if (gl_fd->glok[r]) return;
  gl_fd->glok[r] = 1;
  unsigned int ii = r * 0x400;
  HFONT oldFid = (HFONT)SelectObject((HDC)fl_graphics_driver->gc(), gl_fd->fid);
  wglUseFontBitmapsW((HDC)fl_graphics_driver->gc(), ii, ii + 0x03ff, gl_fd->listbase+ii);
  SelectObject((HDC)fl_graphics_driver->gc(), oldFid);
}

#if HAVE_GL_OVERLAY
extern uchar fl_overlay;
extern int fl_overlay_depth;
int Fl_WinAPI_Gl_Window_Driver::overlay_color(Fl_Color i) {
  if (fl_overlay && fl_overlay_depth) {
    if (fl_overlay_depth < 8) {
      // only black & white produce the expected colors.  This could
      // be improved by fixing the colormap set in Fl_Gl_Overlay.cxx
      int size = 1<<fl_overlay_depth;
      if (!i) glIndexi(size-2);
      else if (i >= size-2) glIndexi(size-1);
      else glIndexi(i);
    } else {
      glIndexi(i ? i : FL_GRAY_RAMP);
    }
    return 1;
  }
  return 0;
}
#endif // HAVE_GL_OVERLAY
#endif // FL_CFG_GFX_GDI


void gl_draw_image(const uchar* b, int x, int y, int w, int h, int d, int ld) {
  if (!ld) ld = w*d;
  glPixelStorei(GL_UNPACK_ROW_LENGTH, ld/d);
  glRasterPos2i(x,y);
  glDrawPixels(w,h,d<4?GL_RGB:GL_RGBA,GL_UNSIGNED_BYTE,(const ulong*)b);
}


#if defined(FL_CFG_GFX_QUARTZ) || defined(FL_DOXYGEN)

#if ! defined(FL_DOXYGEN)

#include <FL/platform.H>
#if !defined(kCGBitmapByteOrder32Host) // doc says available 10.4 but some 10.4 don't have it
#  define kCGBitmapByteOrder32Host 0
#endif

#if  MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_4
#  include <OpenGL/glext.h>
#  define GL_TEXTURE_RECTANGLE_ARB GL_TEXTURE_RECTANGLE_EXT
#endif // MAC_OS_X_VERSION_MAX_ALLOWED

/* Text drawing to an OpenGL scene under Mac OS X is implemented using textures, as recommended by Apple.
 This allows to use any font at any size, and any Unicode character.
 Some old Apple hardware doesn't implement the required GL_EXT_texture_rectangle extension.
 For these, glutStrokeString() is used to draw text. In that case, it's possible to vary text size,
 but not text font, and only ASCII characters can be drawn.
 */

static float gl_scale = 1; // set to 2 for high resolution Fl_Gl_Window
static int has_texture_rectangle = 0; // true means GL_EXT_texture_rectangle is available

#include <FL/glu.h>  // for gluUnProject() and gluCheckExtension()
#include <FL/glut.H> // for glutStrokeString() and glutStrokeLength()

// manages a fifo pile of pre-computed string textures
class gl_texture_fifo {
  friend class Fl_Cocoa_Gl_Window_Driver;
private:
  typedef struct { // information for a pre-computed texture
    GLuint texName; // its name
    char *utf8; //its text
    Fl_Font_Descriptor *fdesc; // its font
    float ratio; // used without rectangle texture
    int scale; // 1 or 2 for low/high resolution
  } data;
  data *fifo; // array of pile elements
  int size_; // pile height
  int current; // the oldest texture to have entered the pile
  int last; // pile top
  int textures_generated; // true after glGenTextures has been called
  void display_texture(int rank);
  int compute_texture(const char* str, int n);
  int already_known(const char *str, int n);
public:
  gl_texture_fifo(int max = 100); // 100 = default height of texture pile
  inline int size(void) {return size_; };
  ~gl_texture_fifo(void);
};

gl_texture_fifo::gl_texture_fifo(int max)
{
  size_ = max;
  last = current = -1;
  textures_generated = 0;
  fifo = (data*)calloc(size_, sizeof(data));
}

gl_texture_fifo::~gl_texture_fifo()
{
  for (int i = 0; i < size_; i++) {
    if (fifo[i].utf8) free(fifo[i].utf8);
    if (textures_generated) glDeleteTextures(1, &fifo[i].texName);
    }
  free(fifo);
}

// displays a pre-computed texture on the GL scene
void gl_texture_fifo::display_texture(int rank)
{
  //setup matrices
  GLint matrixMode;
  glGetIntegerv (GL_MATRIX_MODE, &matrixMode);
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity ();
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity ();
  float winw = gl_scale * Fl_Window::current()->w();
  float winh = gl_scale * Fl_Window::current()->h();
  // GL_COLOR_BUFFER_BIT for glBlendFunc, GL_ENABLE_BIT for glEnable / glDisable
  glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT); 
  glDisable (GL_DEPTH_TEST); // ensure text is not removed by depth buffer test.
  glEnable (GL_BLEND); // for text fading
  glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // ditto
  glDisable(GL_LIGHTING);
  GLfloat pos[4];
  glGetFloatv(GL_CURRENT_RASTER_POSITION, pos);
  float R = 2;
  if (!has_texture_rectangle) {
    R *= fifo[rank].ratio;
  }
  glScalef (R/winw, R/winh, 1.0f);
  glTranslatef (-winw/R, -winh/R, 0.0f);
  GLint width;
  if (has_texture_rectangle) {
    glEnable (GL_TEXTURE_RECTANGLE_ARB);
    glBindTexture (GL_TEXTURE_RECTANGLE_ARB, fifo[rank].texName);
    GLint height;
    glGetTexLevelParameteriv(GL_TEXTURE_RECTANGLE_ARB, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_RECTANGLE_ARB, 0, GL_TEXTURE_HEIGHT, &height);
    CGRect bounds = CGRectMake (pos[0], pos[1] - gl_scale*fl_descent(), width, height);
    //write the texture on screen
    glBegin (GL_QUADS);
    glTexCoord2f (0.0f, 0.0f); // draw lower left in world coordinates
    glVertex2f (bounds.origin.x, bounds.origin.y);
    
    glTexCoord2f (0.0f, height); // draw upper left in world coordinates
    glVertex2f (bounds.origin.x, bounds.origin.y + bounds.size.height);
    
    glTexCoord2f (width, height); // draw upper right in world coordinates
    glVertex2f (bounds.origin.x + bounds.size.width, bounds.origin.y + bounds.size.height);
    
    glTexCoord2f (width, 0.0f); // draw lower right in world coordinates
    glVertex2f (bounds.origin.x + bounds.size.width, bounds.origin.y);
    glEnd ();
  } else {
    glTranslatef(pos[0]*2/R, pos[1]*2/R, 0.0);
    glutStrokeString(GLUT_STROKE_ROMAN, (uchar*)fifo[rank].utf8);
    width = fl_width(fifo[rank].utf8);
  }
  glPopAttrib();
  
  // reset original matrices
  glPopMatrix(); // GL_MODELVIEW
  glMatrixMode (GL_PROJECTION);
  glPopMatrix();
  glMatrixMode (matrixMode);
  
  //set the raster position to end of string
  pos[0] += width;
  GLdouble modelmat[16];
  glGetDoublev (GL_MODELVIEW_MATRIX, modelmat);
  GLdouble projmat[16];
  glGetDoublev (GL_PROJECTION_MATRIX, projmat);
  GLdouble objX, objY, objZ;
  GLint viewport[4];
  glGetIntegerv (GL_VIEWPORT, viewport);
  gluUnProject(pos[0], pos[1], pos[2], modelmat, projmat, viewport, &objX, &objY, &objZ);
  glRasterPos2d(objX, objY);
}

// pre-computes a string texture
int gl_texture_fifo::compute_texture(const char* str, int n)
{
  Fl_Graphics_Driver *prev_driver = fl_graphics_driver;
  fl_graphics_driver = Fl_Display_Device::display_device()->driver();
  current = (current + 1) % size_;
  if (current > last) last = current;
  if ( fifo[current].utf8 ) free(fifo[current].utf8);
  fifo[current].utf8 = (char *)malloc(n + 1);
  memcpy(fifo[current].utf8, str, n);
  fifo[current].utf8[n] = 0;
  fl_graphics_driver->font_descriptor(gl_fontsize);
  int w, h;
  w = fl_width(fifo[current].utf8, n) * gl_scale;
  h = fl_height() * gl_scale;
  fifo[current].scale = gl_scale;
  fifo[current].fdesc = gl_fontsize;
  if (has_texture_rectangle) {
    //write str to a bitmap just big enough
    CGColorSpaceRef lut = CGColorSpaceCreateDeviceRGB();
    void *base = NULL;
    if (fl_mac_os_version < 100600) base = calloc(4*w, h);
    void* save_gc = fl_graphics_driver->gc();
    CGContextRef gc = CGBitmapContextCreate(base, w, h, 8, w*4, lut,
                                  (CGBitmapInfo)(kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host));
    fl_graphics_driver->gc(gc);
    CGColorSpaceRelease(lut);
    GLfloat colors[4];
    glGetFloatv(GL_CURRENT_COLOR, colors);
    fl_color((uchar)(colors[0]*255), (uchar)(colors[1]*255), (uchar)(colors[2]*255));
    CGContextTranslateCTM(gc, 0, h - gl_scale*fl_descent());
    CGContextScaleCTM(gc, gl_scale, gl_scale);
    fl_draw(str, n, 0, 0);
    //put this bitmap in a texture
    glPushAttrib(GL_TEXTURE_BIT);
    glBindTexture (GL_TEXTURE_RECTANGLE_ARB, fifo[current].texName);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, w);
    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA8, w, h, 0,  GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV, CGBitmapContextGetData(gc));
    glPopAttrib();
    CGContextRelease(gc);
    fl_graphics_driver->gc(save_gc);
    if (base) free(base);
  } else {
    fifo[current].ratio = float(w)/glutStrokeLength(GLUT_STROKE_ROMAN, (uchar*)fifo[current].utf8);
  }
  fl_graphics_driver = prev_driver;
  return current;
}

// returns rank of pre-computed texture for a string if it exists
int gl_texture_fifo::already_known(const char *str, int n)
{
  int rank;
  for ( rank = 0; rank <= last; rank++) {
    if ( memcmp(str, fifo[rank].utf8, n) == 0 && fifo[rank].utf8[n] == 0 &&
      fifo[rank].fdesc == gl_fontsize && fifo[rank].scale == gl_scale) return rank;
  }
  return -1;
}

static gl_texture_fifo *gl_fifo = NULL; // points to the texture pile class instance

// draws a utf8 string using pre-computed texture if available
void Fl_Cocoa_Gl_Window_Driver::draw_string(const char* str, int n)
{
  Fl_Gl_Window *gwin = Fl_Window::current()->as_gl_window();
  gl_scale = (gwin ? gwin->pixels_per_unit() : 1);
  
  //fprintf(stderr,"gl_scale=%d\n",gl_scale);
  if (! gl_fifo) gl_fifo = new gl_texture_fifo();
  if (!gl_fifo->textures_generated) {
    has_texture_rectangle = gluCheckExtension((GLubyte*)"GL_EXT_texture_rectangle", glGetString(GL_EXTENSIONS));
    if (has_texture_rectangle) for (int i = 0; i < gl_fifo->size_; i++) glGenTextures(1, &(gl_fifo->fifo[i].texName));
    gl_fifo->textures_generated = 1;
  }
  int rank = gl_fifo->already_known(str, n);
  if (rank == -1) {
    rank = gl_fifo->compute_texture(str, n);
  }
  gl_fifo->display_texture(rank);
}

void gl_texture_reset()
{
  if (gl_fifo) gl_texture_pile_height(gl_texture_pile_height());
}

#endif  //! defined(FL_DOXYGEN)

/** \addtogroup group_macosx
 @{ */

/**
 \brief Returns the current height of the pile of pre-computed string textures
 *
 The default value is 100
 */
int gl_texture_pile_height(void)
{
  if (! gl_fifo) gl_fifo = new gl_texture_fifo();
  return gl_fifo->size();
}

/**
 \brief Changes the height of the pile of pre-computed string textures
 *
 Strings that are often re-displayed can be processed much faster if
 this pile is set high enough to hold all of them.
 \param max Height of the texture pile
 */
void gl_texture_pile_height(int max)
{
  if (gl_fifo) delete gl_fifo;
  gl_fifo = new gl_texture_fifo(max);
}

/** @} */

#endif // FL_CFG_GFX_QUARTZ

#endif // HAVE_GL

//
// End of "$Id$".
//
