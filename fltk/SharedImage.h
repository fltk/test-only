// "$Id$"
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

/*! \class fltk::SharedImage

Images that are all put in a tree by "name" (usually a filename)
so that if the same name is used more than once the same instance
is used.

This is probably obsolete and will be deleted. The base Symbol class
now has a lookup-by-name function, and the Image class could have
methods added to it to throw away the pixmaps of lru images.

*/

#ifndef fltk_SharedImage_h
#define fltk_SharedImage_h

#include "Image.h"

namespace fltk {

struct FL_IMAGES_API ImageType;


class FL_API SharedImage : public Image {
// fabien : introducing SharedImage handlers and uniform loading api inspired from 1.1.x
public:
    /*! get an image of this name and dimensions , can be already loaded or not */
  static SharedImage *get(const char *n); // future impl. should care about W,H dims

    /*! a SharedImageHandler accepts handling a filename 
	by analizing its extension and/or eventually its header,
	if it handles it it returns a non null pointer on the loaded concrete image
    */
  typedef SharedImage *(*Handler)(const char * filename, uchar *header, int headerlen);
    /*! adds a new handler for hanling a concrete type of image, typically one handler per image type should be registered */
  static void add_handler(Handler f);
    /*! removes a concrete handler */
  static void remove_handler(Handler f);

private:
  static Handler *handlers_;		// Additional format handlers
  static int	num_handlers_;		// Number of format handlers
  static int	alloc_handlers_;	// Allocated format handlers

protected:
  static const char* shared_image_root;

  static int image_used;
  static unsigned mem_usage_limit;

  static unsigned mem_used;

  SharedImage* l1;    // Left leaf in the binary tree
  SharedImage* l2;    // Right leaf in the binary tree
  const char*  name;  // Used to indentify the image, and as filename
  const uchar* inline_data; // If non zero, pointers on inlined *COMPRESSED* image data
  unsigned int used;  // Last time used, for cache handling purpose
  int          refcount; // Number of time this image has been get

  SharedImage() { };  // Constructor is private on purpose,
                          // use the get function rather
  //~SharedImage();

  void find_less_used();
  static void check_mem_usage();

  /*! Return the filename obtained from the concatenation
    of the image root directory and this image name
    WARNING : the returned pointer will be
    available only until next call to get_filename */
  const char* get_filename() const;

  static const char* get_filename(const char*);

  virtual void read() = 0;/*!< decompress the image and create its pixmap */

  static void insert(SharedImage*& p, SharedImage* image);
  static SharedImage* find(SharedImage* image, const char* name);
  void remove_from_tree(SharedImage*& p, SharedImage* image);


public:
  static SharedImage  *first_image;

  /*!
    Look for a SharedImage created with the same value for \a name,
    and if not found, use \a create to make a new one.

    If \a inline_data is not NULL, it is assummed to point at a static
    block of memory containg the image data, in whatever compressed
    form the actual class can handle.  (some formats can only handle
    files).

    If \a inline_data is NULL, then the \a name is the filename to read
    to draw the image.
  */
  static SharedImage* get(SharedImage* (*create)(),
			  const char* name, const uchar* inline_data=0);

  /*!
    Reload the image, useful if it has changed on disk, or if the
    inline_data in memory have changed (you can also give a new
    pointer to inline data by passing a non-zero value here).
  */
  void reload(const uchar* inline_data=0);

  /*! Look up an image by name and if found call reload() on it. */
  static void reload(const char* name, const uchar* inline_data=0);

  /*! Remove an image from the database and delete it if its refcount has
    fallen to zero
    Each remove() decrements the refcount, each get() increments it
    Return 1 if it has been really deleted. */
  int remove();
  static int remove(const char* name);

  /*! Clear the cache for this image and all of its children in the binary tree */
  void clear_cache();

  /*! Set the position where images are looked for on disk */
  static void set_root_directory(const char* d);

  /*! Set the size of the cache (0 = unlimited is the default) */
  static void set_cache_size(unsigned l);

  void update();
  void _draw(const Rectangle&) const;

};

////////////////////////////////////////////////////////////////

/*! Description of an Image file format */
struct FL_IMAGES_API ImageType {
  // Name of the filetype as it appear in the source code LOWERCASE!!!
  const char* name;
  // Function to test the filetype
  bool (*test)(const uchar* file_header, unsigned size);
  // Function to get/create an image of this type
  SharedImage* (*get)(const char* name, const uchar* inline_data);
};
extern FL_IMAGES_API ImageType image_filetypes[];

/*! Try to guess the filetype
  Beware that calling this force you to link in all image types ! */
FL_IMAGES_API ImageType* guess_image(const char* name, const uchar* inline_data=0);

////////////////////////////////////////////////////////////////

// 
// bmp and gif classes are build in libfltk2 so they are FL_API
//

class FL_API gifImage : public SharedImage {
  void read();
  gifImage() { }
  static SharedImage* create() { return new gifImage; }
public:
  static bool test(const uchar* file_header, unsigned size=0);
  void _measure(int& W, int& H) const;
  static SharedImage* get(const char* name, const uchar* inline_data = 0) {
    return SharedImage::get(create, name, inline_data);
  }
};

class FL_API bmpImage : public SharedImage {
  void read();
  bmpImage() { }
  static SharedImage* create() { return new bmpImage; }
public:
  static bool test(const uchar* file_header, unsigned size=0);
  void _measure(int& W, int& H) const;
  static SharedImage* get(const char* name, const uchar* inline_data = 0) {
    return SharedImage::get(create, name, inline_data);
  }
};

// Name collision with xpmImage:
class FL_IMAGES_API xpmFileImage : public SharedImage {
  void read();
  xpmFileImage() { }
  static SharedImage* create() { return new xpmFileImage; }
public:
  static bool test(const uchar* file_header, unsigned size=0);
  void _measure(int& W, int& H) const;
  static SharedImage* get(const char* name, const uchar* inline_data = 0) {
    return SharedImage::get(create, name, inline_data);
  }
};

// 
// jpeg and png classes are in libfltk2_images so they are FL_IMAGES_API
//

class FL_IMAGES_API jpegImage : public SharedImage {
  void read();
  jpegImage() { }
  static SharedImage* create() { return new jpegImage; }
public:
  static bool test(const uchar* file_header, unsigned size=0);
  void _measure(int& W, int& H) const;
  static SharedImage* get(const char* name, const uchar* inline_data = 0) {
    return SharedImage::get(create, name, inline_data);
  }
};

class FL_IMAGES_API pngImage : public SharedImage {
  void read();
  pngImage() { }
  static SharedImage* create() { return new pngImage; }
public:
  static bool test(const uchar* file_header, unsigned size=0);
  void _measure(int& W, int& H) const;
  static SharedImage* get(const char* name, const uchar* inline_data = 0) {
    return SharedImage::get(create, name, inline_data);
  }
};

  extern FL_API void register_images(); // return always true only for automatic lib init purpose see images_core.cxx trick
  extern FL_API void unregister_images();
}

#endif

// End of "$Id$"
