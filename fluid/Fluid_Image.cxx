//
// "$Id: Fluid_Image.cxx,v 1.24 2001/06/28 16:15:35 robertk Exp $"
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
#include <FL/conf.h>

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

class generic_image : public Fluid_Image {
protected:
  Fl_Shared_Image *p;
  int *linelength;
  Fl_Image_Type* filetype;
public:
  generic_image(const char *name);
  ~generic_image();
  virtual void label(Fl_Widget *); // set the label of this widget
  virtual void write_static();
  virtual void write_code();
  static int test_file(char *buffer);
};

int generic_image::test_file(char *buffer) {
  Fl_Image_Type* ft = Fl_Shared_Image::guess("", (uchar*)buffer);
  return ft->name != 0;
}

void generic_image::label(Fl_Widget *o) {
  o->image(p);
}

static int image_file_header_written;

#define MAX_CLINESIZE 256
void generic_image::write_static() {
  uchar* d=0;
  if (!p) return;
  if(image_file_header_written != write_number)
  {
    write_c("\n#include <FL/Fl_Shared_Image.H>\n");
    image_file_header_written = write_number;
  }
  if (inlined) {
    size_t l=0;
    goto_images_dir();
    if (filetype->name && !strcasecmp(filetype->name, "xpm")) {
      write_c("static const char *%s[] = {\n", unique_id(this, "datas", filename_name(name()), 0));
      FILE* fp = fopen(name(), "rb");
      if(fp) {
	indentation += 2;
	char s[MAX_CLINESIZE+1];
	do {
	  fgets(s, MAX_CLINESIZE+1, fp);
	} while (!feof(fp) && !strchr(s, '{'));
	while (!feof(fp) && fgets(s, MAX_CLINESIZE+1, fp)) {
		write_c(indent());
	    write_craw(s);
	  //	  write_c(s);
	}
	indentation -= 2;
	fclose(fp);
      }
    } else {
      d = fl_store_datas_from_file(name(), l);
      if(d) {
#if 1
	write_c("static const unsigned char %s[%d] = {\n", unique_id(this, "datas", filename_name(name()), 0), l);
	write_carray((const char*)d, l);
	write_c("};\n");
#else
	write_c("static const unsigned char %s[] =\n",
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

void generic_image::write_code() {
  if (!p) return;
  write_c("%so->image(Fl_%s_Image::get(\"%s\"", indent(), filetype->name, name());
  if (inlined)
    write_c(", %s%s", (filetype->name && !strcasecmp(filetype->name, "xpm")) ? 
		"(uchar*)" : "", unique_id(this, "datas", filename_name(name()), 0));
  write_c("));\n");
}

generic_image::generic_image(const char *name) : Fluid_Image(name) {
  filetype = Fl_Shared_Image::guess((char *) name);
  p = filetype->get((char*) name);
  inlined = 1;
}

generic_image::~generic_image() {
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
  int w, h;
  p->measure(w, h);
  int n = ((w+7)/8)*h;
#if 1 // older one
  write_c("static const unsigned char %s[%d] = {\n",
	  unique_id(this, "bits", filename_name(name()), 0), n);
  write_carray((const char*)(p->array), n);
  write_c("};\n");
#else // this seems to produce slightly shorter c++ files
  write_c("static const unsigned char %s[] =\n",
	  unique_id(this, "bits", filename_name(name()), 0));
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
    if (generic_image::test_file(buffer)) {
      ret = new generic_image(name);
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

static int cancel, modal;
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
// End of "$Id: Fluid_Image.cxx,v 1.24 2001/06/28 16:15:35 robertk Exp $".
//
