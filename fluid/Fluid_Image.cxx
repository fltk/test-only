//
// "$Id: Fluid_Image.cxx,v 1.14 1999/08/28 15:39:09 vincent Exp $"
//
// Pixmap label support for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include "Fl_Type.h"
#include "Fluid_Image.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <FL/filename.H>

extern void goto_source_dir(); // in fluid.C
extern void goto_images_dir(); // in fluid.cxx
extern void leave_source_dir(); // in fluid.C
#define leave_images_dir() leave_source_dir()

const char *images_dir = "";

////////////////////////////////////////////////////////////////

static uchar* fl_store_datas_from_file(const char *filename, size_t &size)
{
  uchar *d=0;
  FILE *fd = fopen(filename, "rb");
  if(!fd) return 0;
  size_t c=512, cc=0, r;
  size = 0;
  do {
    c*=2;
    cc+=c;
    if(d)
      d=(uchar *) realloc(d, cc);
    else
      d=(uchar *) malloc(cc);
    r=fread(d+cc-c, 1, c, fd);
    size+=r;
  } while(r==c);
  if(size!=cc) d=(uchar *) realloc(d, size?size:1);
  return d;
}

////////////////////////////////////////////////////////////////

static char xpm_filetype[] = "XPM";
class pixmap_image : public Fluid_Image {
protected:
  Fl_Shared_Image *p;
  int *linelength;
  char *filetype;
public:
  pixmap_image(const char *name, bool subclass = 1);
  ~pixmap_image();
  virtual void label(Fl_Widget *); // set the label of this widget
  virtual void write_static();
  virtual void write_code();
  static int test_file(char *buffer);
};

int pixmap_image::test_file(char *buffer) {
  return Fl_XPM_Image::test((uchar*)buffer, 0);
}

void pixmap_image::label(Fl_Widget *o) {
  o->image(p);
}

static int image_file_header_written;

#define MAX_CLINESIZE 256
void pixmap_image::write_static() {
  uchar* d=0;
  if (!p) return;
  if(image_file_header_written != write_number)
  {
    write_c("#include <FL/Fl_Shared_Image.H>\n");
    image_file_header_written = write_number;
  }
  if (inlined) {
    size_t l=0;
    goto_images_dir();
    if (filetype == xpm_filetype) {
      write_c("static char *%s[] = {\n", unique_id(this, "datas", filename_name(name()), 0));
      FILE* fp = fopen(name(), "rb");
      if(fp) {
	char s[MAX_CLINESIZE+1];
	do {
	  fgets(s, MAX_CLINESIZE+1, fp);
	} while (!feof(fp) && !strchr(s, '{'));
	while (!feof(fp) && fgets(s, MAX_CLINESIZE+1, fp)) {
	  write_c(s);
	}
	fclose(fp);
      }
    } else {
      d = fl_store_datas_from_file(name(), l);
      if(d) {
#if 0
	write_c("static uchar %s[] = {\n", unique_id(this, "datas", filename_name(name()), 0));
	write_carray((const char*)d, l);
	write_c("};\n");
#else
	write_c("static unsigned char* %s = (unsigned char*)\n",
		unique_id(this, "datas", filename_name(name()), 0));
	write_cstring((const char*)d, l);
	write_c(";\n");
#endif
	free(d);
      }
    }
    leave_images_dir();
  }
}

void pixmap_image::write_code() {
  if (!p) return;
  write_c("%so->image(Fl_%s_Image::get(\"%s\"", indent(), filetype, name());
  if (inlined)
    write_c(", %s%s", filetype==xpm_filetype? "(uchar*)" : "",
	    unique_id(this, "datas", filename_name(name()), 0));
  write_c("));\n");
}

pixmap_image::pixmap_image(const char *name, bool subclass) : Fluid_Image(name) {
  if(!subclass) {
    filetype = xpm_filetype;
    p = Fl_XPM_Image::get((char*) name);
  }
  inlined = 1;
}

pixmap_image::~pixmap_image() {
}

////////////////////////////////////////////////////////////////

class gif_image : public pixmap_image {
public:
  gif_image(const char *name);
  ~gif_image();
  static int test_file(char *buffer);
};

int gif_image::test_file(char *buffer) {
  return Fl_GIF_Image::test((uchar*)buffer, 3);
}

gif_image::gif_image(const char *name) : pixmap_image(name) {
  filetype = "GIF";
  p = Fl_GIF_Image::get((char*)name);
}

gif_image::~gif_image() {
}

////////////////////////////////////////////////////////////////

class png_image : public pixmap_image {
public:
  png_image(const char *name);
  ~png_image();
  static int test_file(char *buffer);
};

int png_image::test_file(char *buffer) {
  return Fl_PNG_Image::test((uchar*)buffer, 3);
}

png_image::png_image(const char *name) : pixmap_image(name) {
  filetype = "PNG";
  p = Fl_PNG_Image::get((char*)name);
}

png_image::~png_image() {
}

////////////////////////////////////////////////////////////////

class bmp_image : public pixmap_image {
public:
  bmp_image(const char *name);
  ~bmp_image();
  static int test_file(char *buffer);
};

int bmp_image::test_file(char *buffer) {
  return Fl_BMP_Image::test((uchar*)buffer, 1024);
}

bmp_image::bmp_image(const char *name) : pixmap_image(name) {
  filetype = "BMP";
  p = Fl_BMP_Image::get((char*)name);
}

bmp_image::~bmp_image() {
}

////////////////////////////////////////////////////////////////

class jpeg_image : public pixmap_image {
public:
  jpeg_image(const char *name);
  ~jpeg_image();
  static int test_file(char *buffer);
};

int jpeg_image::test_file(char *buffer) {
  return Fl_JPEG_Image::test((uchar*)buffer, 1024);
}

jpeg_image::jpeg_image(const char *name) : pixmap_image(name) {
  filetype = "JPEG";
  p = Fl_JPEG_Image::get((char*)name);
}

jpeg_image::~jpeg_image() {
}

////////////////////////////////////////////////////////////////
#include <FL/Fl_Bitmap.H>

class bitmap_image : public Fluid_Image {
  Fl_Bitmap *p;
public:
  ~bitmap_image();
  bitmap_image(const char *name, FILE *);
  virtual void label(Fl_Widget *); // set the label of this widget
  virtual void write_static();
  virtual void write_code();
  static int test_file(char *buffer);
};

// bad test, always do this last!
int bitmap_image::test_file(char *buffer) {
  return (strstr(buffer,"#define ") != 0);
}

void bitmap_image::label(Fl_Widget *o) {
  o->image(p);
}

static int bitmap_header_written;

void bitmap_image::write_static() {
  if (!p) return;
  write_c("\n");
  if (bitmap_header_written != write_number) {
    write_c("#include <FL/Fl_Bitmap.H>\n");
    bitmap_header_written = write_number;
  }
#if 0 // older one
  write_c("static unsigned char %s[] = {  \n",
	  unique_id(this, "bits", filename_name(name()), 0));
  int n = ((p->w+7)/8)*p->h;
  int linelength = 0;
  for (int i = 0; i < n; i++) {
    if (i) {write_c(","); linelength++;}
    if (linelength > 75) {write_c("\n"); linelength=0;}
    int v = p->array[i];
    write_c("%d",v);
    linelength++; if (v>9) linelength++; if (v>99) linelength++;
  }
  write_c("\n};\n");
#else // this seems to produce slightly shorter c++ files
  write_c("static unsigned char %s[] =\n",
	  unique_id(this, "bits", filename_name(name()), 0));
  int w, h;
  p->measure(w, h);
  int n = ((w+7)/8)*h;
  write_cstring((const char*)(p->array), n);
  write_c(";\n");
#endif
  write_c("static Fl_Bitmap %s(%s, %d, %d);\n",
	  unique_id(this, "bitmap", filename_name(name()), 0),
	  unique_id(this, "bits", filename_name(name()), 0),
	  w, h);
}

void bitmap_image::write_code() {
  if (!p) return;
  write_c("%so->image(%s);\n", indent(),
	  unique_id(this, "bitmap", filename_name(name()), 0));
}

#define nosuch_width 16
#define nosuch_height 16
static unsigned char nosuch_bits[] = {
   0xff, 0xf0, 0x81, 0x88, 0xd5, 0x90, 0x69, 0xa8, 0x55, 0x94, 0x69, 0xaa,
   0x55, 0x94, 0x69, 0xaa, 0xd5, 0x94, 0xa9, 0xa8, 0x55, 0x95, 0xa9, 0xa9,
   0x55, 0x95, 0xa9, 0xab, 0x01, 0x81, 0xff, 0xff};
static Fl_Bitmap nosuch_bitmap(nosuch_bits, nosuch_width, nosuch_height);

bitmap_image::bitmap_image(const char *name, FILE *f) : Fluid_Image(name) {
  p = &nosuch_bitmap; // if any problems with parse we exit with this
  if (!f) return;
  char buffer[1024];
  char junk[1024];
  int wh[2]; // width and height
  int i;
  for (i = 0; i<2; i++) {
    for (;;) {
      if (!fgets(buffer,1024,f)) return;
      int r = sscanf(buffer,"#define %s %d",junk,&wh[i]);
      if (r >= 2) break;
    }
  }
  // skip to data array:
  for (;;) {
    if (!fgets(buffer,1024,f)) return;
    if (!strncmp(buffer,"static ",7)) break;
  }
  int n = ((wh[0]+7)/8)*wh[1];
  uchar *data = new uchar[n];
  // read the data:
  i = 0;
  for (;i<n;) {
    if (!fgets(buffer,1024,f)) return;
    const char *a = buffer;
    while (*a && i<n) {
      int t;
      if (sscanf(a," 0x%x",&t)>0) data[i++] = t;
      while (*a && *a++ != ',');
    }
  }
  p = new Fl_Bitmap(data,wh[0],wh[1]);
}

bitmap_image::~bitmap_image() {
  if (p && p != &nosuch_bitmap) {
    delete[] (uchar*)(p->array);
    delete p;
  }
}

////////////////////////////////////////////////////////////////

static Fluid_Image** images; // sorted list
static int numimages;
static int tablesize;

Fluid_Image* Fluid_Image::find(const char *name) {
  if (!name || !*name) return 0;

  // first search to see if it exists already:
  int a = 0;
  int b = numimages;
  while (a < b) {
    int c = (a+b)/2;
    int i = strcmp(name,images[c]->name_);
    if (i < 0) b = c;
    else if (i > 0) a = c+1;
    else return images[c];
  }

  // no, so now see if the file exists:

  Fluid_Image *ret = 0;

  goto_images_dir();
  FILE *f = fopen(name,"rb");

  if (!f) {
    read_error("%s : %s",name,strerror(errno));
    leave_images_dir();
  } else {
    // now see if we can identify the type, by reading in some data
    // and asking all the types we know about:
    char buffer[1025];
    fread(buffer, 1, 1024, f);
    rewind(f);
    buffer[1024] = 0; // null-terminate so strstr() works
    if (pixmap_image::test_file(buffer)) {
      ret = new pixmap_image(name,0);
    } else if (png_image::test_file(buffer)) {
      ret = new png_image(name);
    } else if (bmp_image::test_file(buffer)) {
      ret = new bmp_image(name);
    } else if (jpeg_image::test_file(buffer)) {
      ret = new jpeg_image(name);
    } else if (gif_image::test_file(buffer)) {
      ret = new gif_image(name);
    } else if (bitmap_image::test_file(buffer)) {
      ret = new bitmap_image(name,f);
    } else {
      ret = 0;
      read_error("%s : unrecognized image format", name);
    }
    fclose(f);
  }
  leave_images_dir();
  if (!ret) ret = new bitmap_image(name, 0);

  // make a new entry in the table:
  numimages++;
  if (numimages > tablesize) {
    tablesize = tablesize ? 2*tablesize : 16;
    images = (Fluid_Image**)realloc(images, tablesize*sizeof(Fluid_Image*));
  }
  for (b = numimages-1; b > a; b--) images[b] = images[b-1];
  images[a] = ret;

  return ret;
}

Fluid_Image::Fluid_Image(const char *name) {
  name_ = strdup(name);
  written = 0;
  refcount = 0;
  inlined=0;
}

void Fluid_Image::increment() {
  ++refcount;
}

void Fluid_Image::decrement() {
  --refcount;
  if (refcount > 0) return;
  delete this;
}

Fluid_Image::~Fluid_Image() {
  int a;
  for (a = 0; a<numimages; a++) if (images[a] == this) break;
  if(a<numimages) {
    numimages--;
    for (; a < numimages; a++) images[a] = images[a+1];
  }
  free((void*)name_);
}

////////////////////////////////////////////////////////////////

#include <FL/fl_file_chooser.H>

Fluid_Image *ui_find_image(Fluid_Image *old) {
  goto_images_dir();
  const char *name = fl_file_chooser("Image", "*.{bm|xbm|xpm|gif|png|bmp|jpg|jpeg}",
				     old ? old->name() : 0);
  Fluid_Image *ret = (name && *name) ? Fluid_Image::find(name) : 0;
  leave_images_dir();
  return ret;
}

////////////////////////////////////////////////////////////////

static bool cancel, modal;
void browse_dir_cb();

#include "image_file_panel.h"
#include "image_file_panel.cxx"

void browse_dir_cb()
{
  char *f = fl_file_chooser("Images directory","",
			    images_dir_input->value());
  if(f) images_dir_input->value(f);
}

void set_images_dir_cb(Fl_Widget *, void *) {
  goto_source_dir();
  if(!images_dir_window) make_images_dir_window();
  images_dir_input->value(images_dir);
  images_dir_window->show();
  cancel=0; modal=1;
  while(modal) Fl::wait();
  if(!cancel)
    images_dir = images_dir_input->value();
  leave_source_dir();
}
 
//
// End of "$Id: Fluid_Image.cxx,v 1.14 1999/08/28 15:39:09 vincent Exp $".
//
