//
// "$Id: fl_xpm.cxx,v 1.6 1999/08/26 19:30:06 vincent Exp $"
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

//
// provide functions to measure and decompress XPM files
//

#include <FL/fl_draw.H>
#include <FL/fl_draw_image_file.H>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

extern uchar **fl_mask_bitmap; // used by fl_draw_pixmap.cxx to store mask


#define MAXSIZE 1024
static char **data;

static int hexdigit(int x) {
  if (isdigit(x)) return x-'0';
  if (isupper(x)) return x-'A'+10;
  if (islower(x)) return x-'a'+10;
  return 20;
}

static void read (char *name, int max = MAXSIZE) {
  FILE *f=fopen(name, "rb");
  if (!f) { data=0; return; } // for subclasses
  // read all the c-strings out of the file:
  data = new char*[MAXSIZE+1];
  char buffer[MAXSIZE+20];
  int i = 0;
  while (i < max && fgets(buffer,MAXSIZE+20,f)) {
    if (buffer[0] != '\"') continue;
    char *p = buffer;
    char *q = buffer+1;
    while (*q != '\"') {
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
    memcpy(data[i],buffer,p-buffer);
    i++;
  }
  data[i++] = 0; // put a null at the end
  fclose(f);
}


bool fl_is_xpm(unsigned char *datas=0, size_t size=0)
{
  return (strstr((char*) datas,"/* XPM") != 0);
}

int fl_measure_xpm(char *filename, uchar *pdatas, int &w, int &h)
{
  bool loaded=0;
  char *const* datas = (char *const*)pdatas;
  if(!datas)
  {
    read(filename, 1);
    if(data == 0) return w=0;
    datas=data;
    loaded=1;
  }

  fl_measure_pixmap(datas, w, h);
  if(loaded) {
    while(*datas) delete *datas++;
    delete[] data;
  }
  return w;
}

Fl_Offscreen fl_read_xpm(char *filename, uchar *pdatas, Fl_Offscreen &mask)
{
  bool loaded=0;
  char *const* datas = (char *const*)pdatas;
  if(!datas)
  {
    read(filename);
    if(data == 0) return 0;
    datas=data;
    loaded=1;
  }
  int w, h;
  fl_measure_pixmap(datas, w, h);
  Fl_Offscreen id;
  id = fl_create_offscreen(w, h);
  fl_begin_offscreen((Fl_Offscreen)id);
  uchar *bitmap = 0;
  fl_mask_bitmap = &bitmap;
  fl_draw_pixmap(datas, 0, 0, FL_BLACK);
  fl_mask_bitmap = 0;
  if (bitmap) {
    mask = (Fl_Offscreen) fl_create_bitmap(bitmap, w, h);
    delete[] bitmap;
  }
  fl_end_offscreen();

  if(loaded) {
    while(*datas) delete *datas++;
    delete[] data;
  }
  return id;
}

//
// End of "$Id: fl_xpm.cxx,v 1.6 1999/08/26 19:30:06 vincent Exp $"
//
