//
// "$Id: Fl_Shared_Image.cxx,v 1.1 1999/08/28 15:39:11 vincent Exp $"
//
// Image drawing code for the Fast Light Tool Kit (FLTK).
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

// Draw an image that is stored compressed in a file or in memory. 
// Keep uncompressed images in memory for later use. 

#include <config.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/x.H>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *root=0;

static Fl_Shared_Image  *first_image = 0;

static int image_used=0;
static size_t mem_usage_limit=0;

// This contains the total number of pixmap pixels in the cache
// WARNING : this is updated incrementally, so beware that it keeps balanced !
static size_t mem_used=0; 

void fl_set_images_cache_size(size_t l)
{
  mem_usage_limit = l;
}

Fl_Shared_Image *limage;
size_t Fl_Shared_Image::count() {
  size_t s = mem;
  if(l1) s += l1->count();
  if(l2) s += l2->count();
  if(id && (limage->id == 0 || used<limage->used)) limage=this;
  return s;
}
void Fl_Shared_Image::check_mem_usage()
{
  if(mem_usage_limit==0 || first_image==NULL || mem_used < mem_usage_limit) 
    return;

  do {
    limage=first_image;
    first_image->count();
    if(limage->id) {
      mem_used -= limage->mem;
      limage->mem=0;
      fl_delete_offscreen(limage->id);
      limage->id=0;
      if(limage->mask) {
	fl_delete_offscreen(limage->mask);
	limage->mask = 0;
      }
    } else return;
  } while(mem_used >= mem_usage_limit);
}

void fl_set_images_root_directory(char *d)
{
  if(root) free(root);
  if(d[0] && d[strlen(d)-1]!='/')
  {
    root=(char *) malloc(strlen(d)+2);
    strcpy(root, d);
    strcat(root, "/");
  }
  else
    root=strdup(d);
}

void Fl_Shared_Image::insert(Fl_Shared_Image*& p, Fl_Shared_Image* image) {
  if(p == 0)
    p = image;
  else {
    int c = strcmp(image->name, p->name);
    if(c<0) insert(p->l1, image);
    else insert(p->l2, image);
  }
}

Fl_Shared_Image* Fl_Shared_Image::find(Fl_Shared_Image* image, char* name) {
  if(image == 0) return 0;
  int c = strcmp(name, image->name);
  if(c == 0) return image;
  else if(c<0) return find(image->l1, name);
  else return find(image->l2, name);
}


char* Fl_Shared_Image::get_filename()
{
  static char *s;
// warning : the returned pointer will be available only until next call to convert
  if(s) free(s);
  if(!root) root=strdup("");
  s = (char*) malloc(strlen(name)+strlen(root)+1);
  strcpy(s, root);
  strcat(s, name);
  return s;
}


Fl_Shared_Image* Fl_Shared_Image::get(Fl_Shared_Image* (*create)(),
				      char* name, unsigned char *datas)
{
  Fl_Shared_Image *image=Fl_Shared_Image::find(first_image, name);
  if(!image)
  {
    int dummy;
    image=create();
    image->name = strdup(name);
    image->datas=datas;
    image->w = -1;
    image->measure(dummy, dummy);
    image->mem=0;
    image->l1 = image->l2 = 0;
    image->id=image->mask=0;
    Fl_Shared_Image::insert(first_image, image);
  }
  else if(image->datas==NULL) image->datas=datas;
  image->used = image_used++;
  return image;
}

void Fl_Shared_Image::draw(int X, int Y, int W, int H, 
				int cx, int cy)
{
  if(w==0) return;
  if(!id && !mask) // Need to uncompress the image ?
  {
    mem=w*h;
    used = image_used++; // do this before check_mem_usage
    mem_used += mem;
    check_mem_usage();

    read();
    if(!id && !mask) return; 
  }
  else
    used = image_used++;
  _draw(X, Y, W, H, cx, cy);
}

//
// End of "$Id: Fl_Shared_Image.cxx,v 1.1 1999/08/28 15:39:11 vincent Exp $"
//
