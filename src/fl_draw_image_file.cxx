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
#include <FL/fl_draw_image_file.H>
#include <FL/x.H>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fl_restore_clip(); // in fl_rect.cxx

static char *root=0;

static fl_image_file_struct  *first_image = 0;

static int used=0;
static size_t mem_usage_limit=0;


void fl_set_image_file_mem_usage_limit(size_t l)
{
  mem_usage_limit = l;
}

fl_image_file_struct *lifs;
static size_t count(fl_image_file_struct* ifs) {
  size_t s = ifs->mem;
  if(ifs->l1) s += count(ifs->l1);
  if(ifs->l2) s += count(ifs->l2);
  if(ifs->id && (lifs->id == 0 || ifs->used<lifs->used)) lifs=ifs;
  return s;
}
static void check_mem_usage()
{
  if(mem_usage_limit==0 || first_image==NULL) return;

 again:
  lifs=first_image;
  size_t s=count(first_image);
  if(lifs->id && s>mem_usage_limit+lifs->mem)
  {
    lifs->mem=0;
    fl_delete_offscreen(lifs->id);
    lifs->id=0;
    if(lifs->mask) {
      fl_delete_offscreen(lifs->mask);
      lifs->mask = 0;
    }
    goto again;
  }
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

static void insert(fl_image_file_struct*& p, fl_image_file_struct* ifs) {
  if(p == 0)
    p = ifs;
  else {
    int c = strcmp(ifs->filename, p->filename);
    if(c<0) insert(p->l1, ifs);
    else insert(p->l2, ifs);
  }
}

static fl_image_file_struct* find(fl_image_file_struct* ifs, char* name) {
  if(ifs == 0) return 0;
  int c = strcmp(name, ifs->filename);
  if(c == 0) return ifs;
  else if(c<0) return find(ifs->l1, name);
  else return find(ifs->l2, name);
}


static char* convert(char *n)
{
  static char *s;
// warning : the returned pointer will be available only until next call to convert
  if(s) free(s);
  if(!root) root=strdup("");
  s = (char*) malloc(strlen(n)+strlen(root)+1);
  strcpy(s, root);
  strcat(s, n);
  return s;
}


fl_image_file_struct* fl_get_image_file_struct(char* filename,  
        Fl_Image_File_Measure_F *measure,  Fl_Image_File_Read_F *read,
        unsigned char *datas)
{
  fl_image_file_struct *ifs=find(first_image, filename);
  if(!ifs)
  {
    ifs=new fl_image_file_struct;
    ifs->filename = strdup(filename);
    ifs->measure=measure;
    ifs->read=read;
    ifs->measure(convert(filename), datas, ifs->w, ifs->h);
    ifs->mem=0;
    ifs->l1 = ifs->l2 = 0;
    ifs->datas=datas;
    ifs->id=ifs->mask=0;

    // The quick fix seems no longer useful under WIN32 ?
/*#ifdef WIN32
    // this is a very very quick fix for WIN32 pixmap problem :
    // we read here the image and not at draw time
    ifs->id = ifs->read(convert(ifs->filename), ifs->datas, ifs->mask);
    ifs->mem=ifs->w*ifs->h;
#endif*/
    insert(first_image, ifs);
  }
  else if(ifs->datas==NULL) ifs->datas=datas;
  ifs->used = used++;
  return ifs;
}

// Ensure that the image and mask are decoded
// Take care of the image cache state
void fl_prepare_image_file(fl_image_file_struct *ifs)
{
  if(!ifs->id && !ifs->mask) // Need to uncompress the image
  {
    ifs->id = ifs->read(convert(ifs->filename), ifs->datas, ifs->mask);
    if(!ifs->id) return; 
    ifs->mem=ifs->w*ifs->h;
    ifs->used=used++; // do it before check_mem_usage
    check_mem_usage();
  }
  else
    ifs->used=used++;
}

void fl_draw_image_file(fl_image_file_struct *ifs, int X, int Y, int W, int H, 
			int cx, int cy)
{
  if(ifs->w==0) return;
  fl_prepare_image_file(ifs);
#ifdef WIN32
  if (ifs->mask) {
    HDC new_gc = CreateCompatibleDC(fl_gc);
    SelectObject(new_gc, (void*)ifs->mask);
    BitBlt(fl_gc, X, Y, W, H, new_gc, cx, cy, SRCAND);
    SelectObject(new_gc, (void*)ifs->id);
    BitBlt(fl_gc, X, Y, W, H, new_gc, cx, cy, SRCPAINT);
    DeleteDC(new_gc);
  } else {
    fl_copy_offscreen(X, Y, W, H, ifs->id, cx, cy);
  }
#else
  if (ifs->mask) {
    // I can't figure out how to combine a mask with existing region,
    // so cut the image down to a clipped rectangle:
    int nx, ny; fl_clip_box(X,Y,W,H,nx,ny,W,H);
    cx += nx-X; X = nx;
    cy += ny-Y; Y = ny;
    // make X use the bitmap as a mask:
    XSetClipMask(fl_display, fl_gc, ifs->mask);
    int ox = X-cx; if (ox < 0) ox += ifs->w;
    int oy = Y-cy; if (oy < 0) oy += ifs->h;
    XSetClipOrigin(fl_display, fl_gc, X-cx, Y-cy);
  }
  fl_copy_offscreen(X, Y, W, H, ifs->id, cx, cy);
  if (ifs->mask) {
    // put the old clip region back
    XSetClipOrigin(fl_display, fl_gc, 0, 0);
    fl_restore_clip();
  }
#endif
}

