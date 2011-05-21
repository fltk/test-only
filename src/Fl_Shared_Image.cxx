//
// "$Id$"
//
// Shared image code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <stdio.h>
#include <stdlib.h>
#include <fltk3/utf8.h>
#include "flstring.h"

#include <fltk3/run.h>
#include <fltk3/SharedImage.h>
#include <fltk3/XBMImage.h>
#include <fltk3/XPMImage.h>


//
// Global class vars...
//

fltk3::SharedImage **fltk3::SharedImage::images_ = 0;	// Shared images
int	fltk3::SharedImage::num_images_ = 0;	// Number of shared images
int	fltk3::SharedImage::alloc_images_ = 0;	// Allocated shared images

fltk3::SharedHandler *fltk3::SharedImage::handlers_ = 0;// Additional format handlers
int	fltk3::SharedImage::num_handlers_ = 0;	// Number of format handlers
int	fltk3::SharedImage::alloc_handlers_ = 0;	// Allocated format handlers


//
// Typedef the C API sort function type the only way I know how...
//

extern "C" {
  typedef int (*compare_func_t)(const void *, const void *);
}


/** Returns the fltk3::SharedImage* array */
fltk3::SharedImage **fltk3::SharedImage::images() {
  return images_;
}
/** Returns the total number of shared images in the array. */
int fltk3::SharedImage::num_images() {
  return num_images_;
}


//
// 'fltk3::SharedImage::compare()' - Compare two shared images...
//

int
fltk3::SharedImage::compare(fltk3::SharedImage **i0,		// I - First image
                         fltk3::SharedImage **i1) {	// I - Second image
  int i = strcmp((*i0)->name(), (*i1)->name());

  if (i) return i;
  else if (((*i0)->w() == 0 && (*i1)->original_) ||
           ((*i1)->w() == 0 && (*i0)->original_)) return 0;
  else if ((*i0)->w() != (*i1)->w()) return (*i0)->w() - (*i1)->w();
  else return (*i0)->h() - (*i1)->h();
}


/** 
  Creates an empty shared image.
  The constructors create a new shared image record in the image cache.
  
  <P>The constructors are protected and cannot be used directly
  from a program. Use the get() method instead.
*/
fltk3::SharedImage::SharedImage() : fltk3::Image(0,0,0) {
  name_        = 0;
  refcount_    = 1;
  original_    = 0;
  image_       = 0;
  alloc_image_ = 0;
}


/** 
  Creates a shared image from its filename and its corresponding fltk3::Image* img.
  The constructors create a new shared image record in the image cache.
  
  <P>The constructors are protected and cannot be used directly
  from a program. Use the get() method instead.
*/
fltk3::SharedImage::SharedImage(const char *n,		// I - Filename
                                 fltk3::Image   *img)	// I - Image
  : fltk3::Image(0,0,0) {
  name_ = new char[strlen(n) + 1];
  strcpy((char *)name_, n);

  refcount_    = 1;
  image_       = img;
  alloc_image_ = !img;
  original_    = 1;

  if (!img) reload();
  else update();
}


//
// 'fltk3::SharedImage::add()' - Add a shared image to the array.
//

void
fltk3::SharedImage::add() {
  fltk3::SharedImage	**temp;		// New image pointer array...

  if (num_images_ >= alloc_images_) {
    // Allocate more memory...
    temp = new fltk3::SharedImage *[alloc_images_ + 32];

    if (alloc_images_) {
      memcpy(temp, images_, alloc_images_ * sizeof(fltk3::SharedImage *));

      delete[] images_;
    }

    images_       = temp;
    alloc_images_ += 32;
  }

  images_[num_images_] = this;
  num_images_ ++;

  if (num_images_ > 1) {
    qsort(images_, num_images_, sizeof(fltk3::SharedImage *),
          (compare_func_t)compare);
  }
}


//
// 'fltk3::SharedImage::update()' - Update the dimensions of the shared images.
//

void
fltk3::SharedImage::update() {
  if (image_) {
    w(image_->w());
    h(image_->h());
    d(image_->d());
    data(image_->data(), image_->count());
  }
}

/**
  The destructor free all memory and server resources that are
  used by the image. The destructor is protected and cannot be
  used directly from a program. Use the fltk3::SharedImage::release() method
  instead.
*/
fltk3::SharedImage::~SharedImage() {
  if (name_) delete[] (char *)name_;
  if (alloc_image_) delete image_;
}


//
/** 
  Releases and possibly destroys (if refcount <=0) a shared image. 
  In the latter case, it will reorganize the shared image array so that no hole will occur.
*/
void fltk3::SharedImage::release() {
  int	i;	// Looping var...

  refcount_ --;
  if (refcount_ > 0) return;

  for (i = 0; i < num_images_; i ++)
    if (images_[i] == this) {
      num_images_ --;

      if (i < num_images_) {
        memmove(images_ + i, images_ + i + 1,
               (num_images_ - i) * sizeof(fltk3::SharedImage *));
      }

      break;
    }

  delete this;

  if (num_images_ == 0 && images_) {
    delete[] images_;

    images_       = 0;
    alloc_images_ = 0;
  }
}


//
/** Reloads the shared image from disk */
void fltk3::SharedImage::reload() {
  // Load image from disk...
  int		i;		// Looping var
  FILE		*fp;		// File pointer
  uchar		header[64];	// Buffer for auto-detecting files
  fltk3::Image	*img;		// New image

  if (!name_) return;

  if ((fp = fl_fopen(name_, "rb")) != NULL) {
    if (fread(header, 1, sizeof(header), fp)==0) { /* ignore */ }
    fclose(fp);
  } else {
    return;
  }

  // Load the image as appropriate...
  if (memcmp(header, "#define", 7) == 0) // XBM file
    img = new fltk3::XBMImage(name_);
  else if (memcmp(header, "/* XPM */", 9) == 0) // XPM file
    img = new fltk3::XPMImage(name_);
  else {
    // Not a standard format; try an image handler...
    for (i = 0, img = 0; i < num_handlers_; i ++) {
      img = (handlers_[i])(name_, header, sizeof(header));

      if (img) break;
    }
  }

  if (img) {
    if (alloc_image_) delete image_;

    alloc_image_ = 1;

    if ((img->w() != w() && w()) || (img->h() != h() && h())) {
      // Make sure the reloaded image is the same size as the existing one.
      fltk3::Image *temp = img->copy(w(), h());
      delete img;
      image_ = temp;
    } else {
      image_ = img;
    }

    update();
  }
}


//
// 'fltk3::SharedImage::copy()' - Copy and resize a shared image...
//

fltk3::Image *
fltk3::SharedImage::copy(int W, int H) {
  fltk3::Image		*temp_image;	// New image file
  fltk3::SharedImage	*temp_shared;	// New shared image

  // Make a copy of the image we're sharing...
  if (!image_) temp_image = 0;
  else temp_image = image_->copy(W, H);

  // Then make a new shared image...
  temp_shared = new fltk3::SharedImage();

  temp_shared->name_ = new char[strlen(name_) + 1];
  strcpy((char *)temp_shared->name_, name_);

  temp_shared->refcount_    = 1;
  temp_shared->image_       = temp_image;
  temp_shared->alloc_image_ = 1;

  temp_shared->update();

  return temp_shared;
}


//
// 'fltk3::SharedImage::color_average()' - Blend colors...
//

void
fltk3::SharedImage::color_average(fltk3::Color c,	// I - Color to blend with
                               float    i) {	// I - Blend fraction
  if (!image_) return;

  image_->color_average(c, i);
  update();
}


//
// 'fltk3::SharedImage::desaturate()' - Convert the image to grayscale...
//

void
fltk3::SharedImage::desaturate() {
  if (!image_) return;

  image_->desaturate();
  update();
}


//
// 'fltk3::SharedImage::draw()' - Draw a shared image...
//

void
fltk3::SharedImage::draw(int X, int Y, int W, int H, int cx, int cy) {
  if (image_) image_->draw(X, Y, W, H, cx, cy);
  else fltk3::Image::draw(X, Y, W, H, cx, cy);
}


//
// 'fltk3::SharedImage::uncache()' - Uncache the shared image...
//

void fltk3::SharedImage::uncache()
{
  if (image_) image_->uncache();
}



/** Finds a shared image from its named and size specifications */
fltk3::SharedImage* fltk3::SharedImage::find(const char *n, int W, int H) {
  fltk3::SharedImage	*key,		// Image key
			**match;	// Matching image

  if (num_images_) {
    key = new fltk3::SharedImage();
    key->name_ = new char[strlen(n) + 1];
    strcpy((char *)key->name_, n);
    key->w(W);
    key->h(H);

    match = (fltk3::SharedImage **)bsearch(&key, images_, num_images_,
                                        sizeof(fltk3::SharedImage *),
                                        (compare_func_t)compare);

    delete key;

    if (match) {
      (*match)->refcount_ ++;
      return *match;
    }
  }

  return 0;
}


/** 
 \brief Find or load an image that can be shared by multiple widgets.
 
 Gets a shared image, if it exists already ; it will return it.
 If it does not exist or if it exist but with other size, 
 then the existing image is deleted and replaced
 by a new image from the n filename of the proper dimension.
 If n is not a valid image filename, then get() will return NULL.
 
 Shared JPEG and PNG images can also be created from memory by using their 
 named memory access constructor.
 
 \param n name of the image
 \param W, H desired size
 
 \see fltk3::SharedImage::find(const char *n, int W, int H)
 \see fltk3::SharedImage::release() 
 \see fltk3::JPEGImage::JPEGImage(const char *name, const unsigned char *data)
 \see fltk3::PNGImage::fltk3::PNGImage (const char *name_png, const unsigned char *buffer, int maxsize)
*/
fltk3::SharedImage* fltk3::SharedImage::get(const char *n, int W, int H) {
  fltk3::SharedImage	*temp;		// Image

  if ((temp = find(n, W, H)) != NULL) return temp;

  if ((temp = find(n)) == NULL) {
    temp = new fltk3::SharedImage(n);

    if (!temp->image_) {
      delete temp;
      return NULL;
    }

    temp->add();
  }

  if ((temp->w() != W || temp->h() != H) && W && H) {
    temp = (fltk3::SharedImage *)temp->copy(W, H);
    temp->add();
  }

  return temp;
}



/** Adds a shared image handler, which is basically a test function for adding new formats */
void fltk3::SharedImage::add_handler(fltk3::SharedHandler f) {
  int			i;		// Looping var...
  fltk3::SharedHandler	*temp;		// New image handler array...

  // First see if we have already added the handler...
  for (i = 0; i < num_handlers_; i ++) {
    if (handlers_[i] == f) return;
  }

  if (num_handlers_ >= alloc_handlers_) {
    // Allocate more memory...
    temp = new fltk3::SharedHandler [alloc_handlers_ + 32];

    if (alloc_handlers_) {
      memcpy(temp, handlers_, alloc_handlers_ * sizeof(fltk3::SharedHandler));

      delete[] handlers_;
    }

    handlers_       = temp;
    alloc_handlers_ += 32;
  }

  handlers_[num_handlers_] = f;
  num_handlers_ ++;
}



/** Removes a shared image handler */
void fltk3::SharedImage::remove_handler(fltk3::SharedHandler f) {
  int	i;				// Looping var...

  // First see if the handler has been added...
  for (i = 0; i < num_handlers_; i ++) {
    if (handlers_[i] == f) break;
  }

  if (i >= num_handlers_) return;

  // OK, remove the handler from the array...
  num_handlers_ --;

  if (i < num_handlers_) {
    // Shift later handlers down 1...
    memmove(handlers_ + i, handlers_ + i + 1,
           (num_handlers_ - i) * sizeof(fltk3::SharedHandler ));
  }
}


//
// End of "$Id$".
//
