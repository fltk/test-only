//
// "$Id: fl_xpm.cxx,v 1.20 2002/12/09 04:52:30 spitzak Exp $"
//
// XPM reading code for the Fast Light Tool Kit (FLTK).
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

// Image type that reads an xpm file in from disk.  Normal use of xpm
// files is to imbed them in the source code with #include, in that
// case you want to use an xpmImage object.

// Currently commented out as it has the same name as the non-file
// version. We need to figure out an efficient method of having both
// in-memory and file data, probably by making all the file ones
// subclasses...

#include <fltk/xpmImage.h>
#include <fltk/events.h>
#include <fltk/draw.h>
#include <fltk/x.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace fltk;

static int hexdigit(int x) {
  if (isdigit(x)) return x-'0';
  if (isupper(x)) return x-'A'+10;
  if (islower(x)) return x-'a'+10;
  return 20;
}

#define MAXSIZE 2048
#define INITIALLINES 1024

static char** read(char *name, int oneline = 0) {
  FILE *f=fopen(name, "rb");
  if (!f) return 0;
  // read all the c-strings out of the file:
  char* local_data[INITIALLINES];
  char** data = local_data;
  int malloc_size = INITIALLINES;
  char buffer[MAXSIZE+20];
  int i = 0;
  while (fgets(buffer,MAXSIZE+20,f)) {
    if (buffer[0] != '\"') continue;
    char *p = buffer;
    char *q = buffer+1;
    while (*q != '\"' && p < buffer+MAXSIZE) {
      if (*q == '\\') switch (*++q) {
      case '\n':
	fgets(q,(buffer+MAXSIZE+20)-q,f); break;
      case 0:
	break;
      case 'x': {
	q++;
	int n = 0;
	for (int x = 0; x < 3; x++) {
	  int d = hexdigit(*q);
	  if (d > 15) break;
	  n = (n<<4)+d;
	  q++;
	}
	*p++ = n;
      } break;
      default: {
	int c = *q++;
	if (c>='0' && c<='7') {
	  c -= '0';
	  for (int x=0; x<2; x++) {
	    int d = hexdigit(*q);
	    if (d>7) break;
	    c = (c<<3)+d;
	    q++;
	  }
	}
	*p++ = c;
      } break;
      } else {
	*p++ = *q++;
      }
    }
    *p++ = 0;
    data[i] = new char[p-buffer];
    memcpy(data[i], buffer,p-buffer);
    i++;
    if (i >= malloc_size) {
      malloc_size = 2*malloc_size;
      if (data == local_data) {
	data = (char**)malloc(malloc_size*sizeof(char*));
	memcpy(data, local_data, i*sizeof(char*));
      } else {
	data = (char**)realloc(data, malloc_size*sizeof(char*));
      }
    }
    if (oneline) break;
  }
  fclose(f);
  data[i++] = 0; // null terminator

  if(!data[0])
    return 0;
  if (data == local_data) {
    data = (char**)malloc(i*sizeof(char*));
    memcpy(data, local_data, i*sizeof(char*));
  }
  return data;
}

bool xpmImage::test(const unsigned char *data, size_t)
{
  return (strstr((char*) data,"/* XPM") != 0);
}

void xpmImage::measure(int &W, int &H)
{
  if (w>=0) { 
    W=w; H=h; 
    return; 
  }
  int loaded=0;
  char *const* ldata = (char *const*)data;
  if (!ldata) {
    ldata = ::read((char *)get_filename(), 1);
    if (!ldata) {W = w = 0; return;}
    loaded=1;
  }

  measure_xpm(ldata, w, h);
  if (loaded) {
    delete[] ldata[0];
    free((void*)ldata);
  }
  W=w; H=h;
}

void xpmImage::read()
{
  id = mask = 0;
  int loaded=0;
  char *const* ldata = (char *const*)data;
  if (!ldata) {
    ldata = ::read((char *)get_filename());
    if (!ldata) return;
    loaded=1;
  }
  int w, h;
  measure_xpm(ldata, w, h);
  Pixmap pixmap = create_offscreen(w, h);
  id = (void*)pixmap;
  fl_begin_offscreen(pixmap);
  uchar *bitmap = 0;
  set_mask_bitmap(&bitmap);
  draw_xpm(ldata, 0, 0, NO_COLOR);
  set_mask_bitmap(0);
  if (bitmap) {
    mask = (void*)create_bitmap(bitmap, w, h);
    delete[] bitmap;
  }
  fl_end_offscreen();

  if(loaded) {
    char* const* save = ldata;
    while (*ldata) delete[] *ldata++;
    free((void*)save);
  }
  return;
}

//
// End of "$Id: fl_xpm.cxx,v 1.20 2002/12/09 04:52:30 spitzak Exp $"
//
