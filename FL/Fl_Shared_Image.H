//
// "$Id: Fl_Shared_Image.H,v 1.6 1999/09/01 00:23:21 vincent Exp $"
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

class Fl_Image_Type;

// Shared images class. 
class Fl_Shared_Image : public Fl_Image {
protected:
  Fl_Shared_Image* l1;    // Left leaf in the binary tree
  Fl_Shared_Image* l2;    // Right leaf in the binary tree
  char* 	   name;  // Used to indentify the image, and as filename
  unsigned char*   datas; // If non zero, pointers on inlined compressed datas
  unsigned int     used;  // Last time used, for cache handling purpose
  int              refcount; // Number of time this image has been get

  Fl_Shared_Image() { };  // Constructor is private on purpose,
                          // use the get function rather
  ~Fl_Shared_Image();

  void find_less_used();
  static void check_mem_usage();

  char* get_filename();   // Return the filename obtained from the concatenation
                          // of the image root directory and this image name

  virtual void read() = 0;// decompress the image and create its pixmap

  static void Fl_Shared_Image::insert(Fl_Shared_Image*& p, Fl_Shared_Image* image);
  static Fl_Shared_Image* Fl_Shared_Image::find(Fl_Shared_Image* image, char* name);
  void remove_from_tree(Fl_Shared_Image*& p, Fl_Shared_Image* image);


public:
  // Return an Fl_Shared_Image, using the create function if an image with
  // the given name doesn't already exist. Use datas, or read from the
  // file with filename name if datas==0.
  static Fl_Shared_Image* get(Fl_Shared_Image* (*create)(), 
			      char* name, unsigned char *datas=0);

  // Reload the image, useful if it has changed on disk, or if the datas
  // in memory have changed (you can also give a new pointer on datas)
  void reload(uchar* datas=0);
  static void reload(char* name, uchar* datas=0);

  // Remove an image from the database and delete it if its refcount has 
  // falled to zero
  // Each remove decrement the refcount, each get increment it
  // Return 1 if it has been really deleted.
  bool remove();
  static bool remove(char* name);

  // Try to guess the filetype
  static Fl_Image_Type* guess(char* name, unsigned char *datas=0);

  // Set the position where images are looked for on disk
  static void set_root_directory(char *d);

  // Set the size of the cache (0 = unlimited is the default)
  static void set_cache_size(size_t l);

  virtual void draw(int X, int Y, int W, int H, int cx, int cy);
};



// Description of a file format
struct Fl_Image_Type {
  // Name of the filetype as it appear in the source code (uppercase)
  char* name;
  // Function to test the filetype
  bool (*test)(unsigned char *datas, size_t size=0); 
  // Function to get/create an image of this type
  Fl_Shared_Image* (*get)(char* name, uchar* datas=0);
};
extern Fl_Image_Type fl_image_filetypes[];

/* Specific image format functions. Add you own file format here. */

// PNG image class
class Fl_PNG_Image : Fl_Shared_Image {
  void read();		// Uncompress PNG datas
  Fl_PNG_Image() { }
  static Fl_Shared_Image* create() { return new Fl_PNG_Image; } // Instantiate
public:
// Check the given buffer if is is in PNG format
  static bool test(unsigned char *datas, size_t size=0);
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
  static bool test(unsigned char *datas, size_t size=0);
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
  static bool test(unsigned char *datas, size_t size=0);
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
  static bool test(unsigned char *datas, size_t size=0);
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
  static bool test(unsigned char *datas, size_t size=0);
  void measure(int& W, int& H);
  static Fl_Shared_Image* get(char* name, uchar* datas = 0) {
    return Fl_Shared_Image::get(create, name, datas);
  }
};

class Fl_Bitmap;
extern Fl_Bitmap nosuch_bitmap;

class Fl_UNKNOWN_Image {
public:
  static bool test(unsigned char *, size_t =0) { return 1; };
  static Fl_Shared_Image* get(char*, uchar* = 0) { 
    return (Fl_Shared_Image*) &nosuch_bitmap; 
  };
};

#endif

//
// End of "$Id: Fl_Shared_Image.H,v 1.6 1999/09/01 00:23:21 vincent Exp $"
//
