// "$Id$"
//
// Copyright 1998-2004 by Bill Spitzak and others.
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

// Draw an image that is stored compressed in a file or in memory. 
// Keep uncompressed images in memory for later use. 
// WAS: why? it's already in the Pixmap, ready for drawing!

#include <config.h>
#include <fltk/events.h>
#include <fltk/draw.h>
#include <fltk/SharedImage.h>
#include <fltk/xbmImage.h>
#include <fltk/string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

using namespace fltk;

// This fixes linkage problem in VC++ 6.0
#if defined(_MSC_VER) && defined(__cplusplus)
  extern "C" const char* newstring(const char *from);
#endif

const char*	SharedImage::shared_image_root=0;
SharedImage*	SharedImage::first_image = 0;
int		SharedImage::image_used=0;
unsigned	SharedImage::mem_usage_limit=0;
unsigned	SharedImage::mem_used=0;


// static unsigned mem_used=0; (now moved to Fl.cxx !)
// This contains the total number of pixmap pixels in the cache
// WARNING : this is updated incrementally, so beware that it keeps balanced
// when deleting or creating pixmaps !

void SharedImage::set_cache_size(unsigned l)
{
  mem_usage_limit = l;
}

static SharedImage *limage; // used to find the less used image
void SharedImage::find_less_used() {
  if(l1) l1->find_less_used();
  if(l2) l2->find_less_used();
  if(drawn() && (!limage->drawn() || used<limage->used)) limage=this;
}
void SharedImage::check_mem_usage()
{
  if(mem_usage_limit==0 || first_image==NULL || mem_used < mem_usage_limit) 
    return;

  do {
    limage=first_image;
    first_image->find_less_used();
    if (limage->drawn()) {
      mem_used -= limage->w() * limage->h();
      limage->destroy();
    } else return;
  } while(mem_used >= mem_usage_limit);
}

// WAS: this is probably a waste of time! No modern system requires these
// to be deleted.
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
  if (drawn()) {
    mem_used -= w()*h();
    destroy();
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

/*! Prepends the shared_image_root to the name. This is what get_filename()
  would return for a SharedImage with this name. */
const char* SharedImage::get_filename(const char* name)
{
  if (name[0] == '/' || !shared_image_root || !*shared_image_root)
    return name;
  int m = strlen(shared_image_root);
  int n = strlen(name) + m + 2;
  static char *s;
  delete[] s;
  s = new char[n+1];
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
    image->name = newstring(name);
    image->datas=datas;
    image->setsize(-1,-1); // We mark the fact the it has never been measured yet
    image->l1 = image->l2 = 0;
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
  if (drawn()) {
    mem_used -= w()*h();
    destroy();
  }
  if (pdatas) datas = pdatas;
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
  if (drawn()) mem_used -= w()*h();
  return 1;
}
int SharedImage::remove(const char* name)
{
  SharedImage *image=SharedImage::find(first_image, name);
  if (image) return image->remove();
  else return 0;
}

void SharedImage::_draw(const Rectangle& r) const
{
  if (this->w() < 0) { int W = r.w(); int H = r.h(); measure(W,H); }
  if (this->w() == 0) return;
  const_cast<SharedImage*>(this)->used =
    image_used++; // do this before check_mem_usage
  if (!drawn()) // Need to uncompress the image ?
  {
    mem_used += this->w()*this->h();
    check_mem_usage();

    const_cast<SharedImage*>(this)->read();
    if (!drawn()) { // could not read the image for some reason ?
      mem_used -= this->w()*this->h();
      const_cast<SharedImage*>(this)->setsize(0,0); // Will never try again ...
      return; 
    }
  }
  Image::_draw(r);
}

//
// End of "$Id$"
//
