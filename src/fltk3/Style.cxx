//
// "$Id$"
//
// Base widget class for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

#include <fltk3/Style.h>

unsigned int fltk3::Style::current_ = 0;

fltk3::Style fltk3::default_style;


fltk3::Style::Style() :
  version_(current_),
  parent_(0),
  labelfont_(fltk3::HELVETICA),
  labelsize_(fltk3::NORMAL_SIZE),
  private_(0),
  labelfont_set_(1),
  labelsize_set_(1)
{
}


fltk3::Style::Style(Style *parent) :
  version_(current_),
  parent_(parent),
  labelfont_(parent->labelfont_),
  labelsize_(parent->labelsize_),
  private_(0),
  labelfont_set_(0),
  labelsize_set_(0)
{
}


fltk3::Style::~Style()
{
}


fltk3::Style *fltk3::Style::make_private() 
{
  if (private_)
    return this;
  else {
    Style *s = new Style(this);
    s->private_ = 1;
    return s;
  }
}


void fltk3::Style::update()
{
  if (version_==current_ || parent_==0L)
    return;
  parent_->refresh();
  
  if (!labelfont_set_)
    labelfont_ = parent_->labelfont_;
  if (!labelsize_set_)
    labelsize_ = parent_->labelsize_;
  
  version_ = current_;
}


//
// End of "$Id$".
//
