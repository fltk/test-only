// "$Id: fl_xpm.cxx,v 1.25 2004/07/27 07:03:08 spitzak Exp $"
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

/*! \class fltk::xpmFileImage

  Image type that reads a .xpm (X Pixmap) file in from disk.

  The normal use of a .xpm file is to #include it into
  your source code. In that case you want to use an fltk::xpmImage
  object.

*/

#include <fltk/SharedImage.h>
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

/*! Test a block of data read from the start of the file to see if it
  looks like the start of a .xpm file. This returns true if the 
  data contains "/\* XPM".
*/
bool xpmFileImage::test(const unsigned char *data, unsigned)
{
  return (strstr((char*) data,"/* XPM") != 0);
}

void xpmFileImage::_measure(float &W, float &H) const
{
  if(w()>=0) { 
    W=w(); H=h(); 
    return; 
  }
  int loaded=0;
  char *const* ldata = (char *const*)datas;
  if (!ldata) {
    ldata = ::read((char *)get_filename(), 1);
    if (!ldata) {
      W = H = 0;
      const_cast<xpmFileImage*>(this)->setsize(0,0);
      return;
    }
    loaded=1;
  }

  int _W,_H;
  measure_xpm(ldata,_W,_H);
  const_cast<xpmFileImage*>(this)->setsize(_W,_H);

  if (loaded) {
    delete[] ldata[0];
    free((void*)ldata);
  }
  W=w(); 
  H=h();
}

void xpmFileImage::read()
{
  //id = mask = 0;
  int loaded=0;
  char *const* ldata = (char *const*)datas;
  if (!ldata) {
    ldata = ::read((char *)get_filename());
    if (!ldata) return;
    loaded=1;
  }
  int w, h;
  measure_xpm(ldata, w, h);

  ImageDraw idraw(const_cast<xpmFileImage*>(this));

  uchar *bitmap = 0;
  set_mask_bitmap(&bitmap);
  draw_xpm(ldata, 0, 0, NO_COLOR);
  set_mask_bitmap(0);
  if (bitmap) {
    (const_cast<xpmFileImage*>(this))->set_alpha_bitmap(bitmap, this->w(), this->h());
    delete[] bitmap;
  }

  if (loaded) {
    char* const* p = ldata;
    while (*p) delete[] *p++;
    free((void*)ldata);
  }

  return;
}

//
// End of "$Id: fl_xpm.cxx,v 1.25 2004/07/27 07:03:08 spitzak Exp $"
//
