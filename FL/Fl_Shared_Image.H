//
// "$Id: Fl_Shared_Image.H,v 1.1 1999/08/28 15:39:07 vincent Exp $"
//
// Image file header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Shared_Image_H
#define Fl_Shared_Image_H

#include <FL/Fl_Image.H>
#include <stddef.h>

// Shared images class. 
class Fl_Shared_Image : public Fl_Image {
protected:
  Fl_Shared_Image* l1;    // Left leaf in the binary tree
  Fl_Shared_Image* l2;    // Right leaf in the binary tree
  char* 	   name;  // Used to indentify the image, and as filename
  unsigned char*   datas; // If non zero, pointers on inlined compressed datas
  unsigned int     used;  // Last time used, for cache handling purpose
  size_t           mem;   // Size of the image in pixels, for cache handling purpose

  Fl_Shared_Image() { };  // Constructor is private on purpose,
                          // use fl_get_shared_image rather

  size_t count();
  static void check_mem_usage();

  char* get_filename();  // Return the filename obtained from the concatenation
                         // of the image root directory and this image name
  virtual void read() = 0;

  static void Fl_Shared_Image::insert(Fl_Shared_Image*& p, Fl_Shared_Image* ifs);
  static Fl_Shared_Image* Fl_Shared_Image::find(Fl_Shared_Image* ifs, char* name);


public:
  // return an Fl_Shared_Image, using the create function if an image with
  // the given name doesn't already exist. Use datas, or read from the
  // file with filename name if datas==0.
  static Fl_Shared_Image* get(Fl_Shared_Image* (*create)(), 
			      char* name, unsigned char *datas=0);

  virtual void draw(int X, int Y, int W, int H, int cx, int cy);
};

void fl_set_images_root_directory(char *d);
void fl_set_images_cache_size(size_t l);


/* Specific image format functions. Add you own file format here. */

// PNG image class
class Fl_PNG_Image : Fl_Shared_Image {
  void read();		// Uncompress PNG datas
  Fl_PNG_Image() { }
  static Fl_Shared_Image* create() { return new Fl_PNG_Image; } // Instantiate
public:
// Check the given buffer if is is in PNG format
  static bool test(unsigned char *datas=0, size_t size=0);
  void measure(int& W, int& H); // Return width and heigth
  static Fl_Shared_Image* get(char* name, uchar* datas = 0) {
    return Fl_Shared_Image::get(create, name, datas);
  }
};

class Fl_GIF_Image : Fl_Shared_Image {
  void read();
  Fl_GIF_Image() { }
  static Fl_Shared_Image* create() { return new Fl_GIF_Image; }
public:
  static bool test(unsigned char *datas=0, size_t size=0);
  void measure(int& W, int& H);
  static Fl_Shared_Image* get(char* name, uchar* datas = 0) {
    return Fl_Shared_Image::get(create, name, datas);
  }
};

class Fl_XPM_Image : Fl_Shared_Image {
  void read();
  Fl_XPM_Image() { }
  static Fl_Shared_Image* create() { return new Fl_XPM_Image; }
public:
  static bool test(unsigned char *datas=0, size_t size=0);
  void measure(int& W, int& H);
  static Fl_Shared_Image* get(char* name, uchar* datas = 0) {
    return Fl_Shared_Image::get(create, name, datas);
  }
};

class Fl_BMP_Image : Fl_Shared_Image {
  void read();
  Fl_BMP_Image() { }
  static Fl_Shared_Image* create() { return new Fl_BMP_Image; }
public:
  static bool test(unsigned char *datas=0, size_t size=0);
  void measure(int& W, int& H);
  static Fl_Shared_Image* get(char* name, uchar* datas = 0) {
    return Fl_Shared_Image::get(create, name, datas);
  }
};

class Fl_JPEG_Image : Fl_Shared_Image {
  void read();
  Fl_JPEG_Image() { }
  static Fl_Shared_Image* create() { return new Fl_JPEG_Image; }
public:
  static bool test(unsigned char *datas=0, size_t size=0);
  void measure(int& W, int& H);
  static Fl_Shared_Image* get(char* name, uchar* datas = 0) {
    return Fl_Shared_Image::get(create, name, datas);
  }
};

#endif

//
// End of "$Id: Fl_Shared_Image.H,v 1.1 1999/08/28 15:39:07 vincent Exp $"
//
