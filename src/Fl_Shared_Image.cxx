//
// "$Id: Fl_Shared_Image.cxx,v 1.32 2003/11/04 08:11:01 spitzak Exp $"
//
// Image drawing code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

// Draw an image that is stored compressed in a file or in memory. 
// Keep uncompressed images in memory for later use. 

#include <config.h>
#include <fltk/events.h>
#include <fltk/draw.h>
#include <fltk/SharedImage.h>
#include <fltk/xbmImage.h>
#include <fltk/x.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace fltk;

const char*	SharedImage::shared_image_root=0;
SharedImage*	SharedImage::first_image = 0;
int		SharedImage::image_used=0;
unsigned	SharedImage::mem_usage_limit=0;
unsigned	SharedImage::mem_used=0;
int		SharedImage::forbid_delete = 1;

// static unsigned mem_used=0; (now moved to Fl.cxx !)
// This contains the total number of pixmap pixels in the cache
// WARNING : this is updated incrementally, so beware that it keeps balanced
// when deleting or creating pixmaps !

SharedImage::~SharedImage()
{
  if(forbid_delete)
    fprintf(stderr, 
      "FLTK user error : deleting an SharedImage object is forbiden !\n");
  if (id) mem_used -= w()*h();
}

void SharedImage::set_cache_size(unsigned l)
{
  mem_usage_limit = l;
}

static SharedImage *limage; // used to find the less used image
void SharedImage::find_less_used() {
  if(l1) l1->find_less_used();
  if(l2) l2->find_less_used();
  if(id && (limage->id == 0 || used<limage->used)) limage=this;
}
void SharedImage::check_mem_usage()
{
  if(mem_usage_limit==0 || first_image==NULL || mem_used < mem_usage_limit) 
    return;

  do {
    limage=first_image;
    first_image->find_less_used();
    if (limage->id) {
      mem_used -= limage->w() * limage->h();
      limage->destroy_cache();
    } else return;
  } while(mem_used >= mem_usage_limit);
}


class shared_image_destructor_class {
  int dummy;
public:
  shared_image_destructor_class() { dummy = 0; }
  ~shared_image_destructor_class() {
    if (SharedImage::first_image) SharedImage::first_image->clear_cache();
  }
};

shared_image_destructor_class shared_image_destructor;

void SharedImage::clear_cache()
{
  if (id) {
    mem_used -= w()*h();
    destroy_cache();
  }
  if (l1) l1->clear_cache();
  if (l2) l2->clear_cache();
}

void SharedImage::set_root_directory(const char *d) {
  shared_image_root = d;
}

void SharedImage::insert(SharedImage*& p, SharedImage* image) {
  if(p == 0)
    p = image;
  else {
    int c = strcmp(image->name, p->name);
    if(c<0) insert(p->l1, image);
    else insert(p->l2, image);
  }
}

SharedImage* SharedImage::find(SharedImage* image, const char* name) {
  if(image == 0) return 0;
  int c = strcmp(name, image->name);
  if(c == 0) return image;
  else if(c<0) return find(image->l1, name);
  else return find(image->l2, name);
}


const char* SharedImage::get_filename() const {
  return get_filename(name);
}

const char* SharedImage::get_filename(const char* name)
{
  if (name[0] == '/' || !shared_image_root || !*shared_image_root)
    return name;
  int m = strlen(shared_image_root);
  int n = strlen(name) + m + 2;
  static char *s;
  if (s) free(s);
  s = (char*) malloc(n+1);
  strcpy(s, shared_image_root);
  if (s[m-1] != '/') s[m++] = '/';
  strcpy(s+m, name);
  return s;
}


SharedImage* SharedImage::get(SharedImage* (*create)(),
				      const char* name, const uchar *datas)
{
  SharedImage *image=SharedImage::find(first_image, name);
  if(!image)
  {
    image=create();
    image->refcount = 1;
    image->name = strdup(name);
    image->datas=datas;
    image->w_ = -1; // We mark the fact the it has never been measured yet
    image->l1 = image->l2 = 0;
    image->id=image->mask=0;
    SharedImage::insert(first_image, image);
  } else {
    if(image->datas==NULL) image->datas=datas;
    image->refcount++;
  }
  image->used = image_used++;
  return image;
}

void SharedImage::reload(const uchar* pdatas)
{
  if (id) {
    mem_used -= w()*h();
    destroy_cache();
  }
  if (pdatas) datas = pdatas;
  float W=w_; float H=h_; measure(W,H);
}
void SharedImage::reload(const char* name, const uchar* pdatas)
{
  SharedImage *image=SharedImage::find(first_image, name);
  if (image) image->reload(pdatas);
}

void SharedImage::remove_from_tree(SharedImage*& p, SharedImage* image) {
  if (p) {
    int c = strcmp(image->name, p->name);
    if (c == 0) {
      if (image->l1) {
	p = image->l1;
	if (image->l2) insert(first_image, image->l2);
      } else
	p = image->l2;
    } else if (c<0) remove_from_tree(p->l1, image);
    else remove_from_tree(p->l2, image);
  }
}

int SharedImage::remove()
{
  if (--refcount) return 0;
  remove_from_tree(first_image, this);
  forbid_delete = 0;
  delete this;
  forbid_delete = 1;
  return 1;
}
int SharedImage::remove(const char* name)
{
  SharedImage *image=SharedImage::find(first_image, name);
  if (image) return image->remove();
  else return 0;
}

void SharedImage::_draw(int x, int y, int w, int h, const Style* style, Flags flags) const
{
  if (w_ < 0) {float W=w; float H=h; measure(W,H);}
  if (w_ == 0) return;
  const_cast<SharedImage*>(this)->used =
    image_used++; // do this before check_mem_usage
  if (!id) // Need to uncompress the image ?
  {
    mem_used += w_*h_;
    check_mem_usage();

    const_cast<SharedImage*>(this)->read();
    if (!id) { // could not read the image for some reason ?
      mem_used -= w_*h_;
      const_cast<SharedImage*>(this)->w_ = 0; // Will never try again ...
      return; 
    }
  }
  draw_cache(x,y,w,h,style,flags);
}

//
// End of "$Id: Fl_Shared_Image.cxx,v 1.32 2003/11/04 08:11:01 spitzak Exp $"
//
