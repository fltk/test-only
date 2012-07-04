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
  labeltype_(fltk3::NORMAL_LABEL),
  labelcolor_(fltk3::FOREGROUND_COLOR),
  align_(fltk3::ALIGN_CENTER),
  textfont_(fltk3::HELVETICA),
  textsize_(fltk3::NORMAL_SIZE),
  textcolor_(fltk3::FOREGROUND_COLOR),
  color_(fltk3::GRAY),
  color2_(fltk3::GRAY),
  box_(fltk3::NO_BOX),
  private_(0),
  labelfont_set_(1),
  labelsize_set_(1),
  labeltype_set_(1),
  labelcolor_set_(1),
  align_set_(1),
  textfont_set_(1),
  textsize_set_(1),
  textcolor_set_(1),
  color_set_(1),
  color2_set_(1),
  box_set_(1)
{
}


fltk3::Style::Style(Style *parent) :
  version_(parent->update()),
  parent_(parent),
  labelfont_(parent->labelfont_),
  labelsize_(parent->labelsize_),
  labeltype_(parent->labeltype_),
  labelcolor_(parent->labelcolor_),
  align_(parent->align_),
  textfont_(parent->textfont_),
  textsize_(parent->textsize_),
  textcolor_(parent->textcolor_),
  color_(parent->color_),
  color2_(parent->color2_),
  box_(parent->box_),
  private_(0),
  labelfont_set_(0),
  labelsize_set_(0),
  labeltype_set_(0),
  labelcolor_set_(0),
  align_set_(0),
  textfont_set_(0),
  textsize_set_(0),
  textcolor_set_(0),
  color_set_(0),
  color2_set_(0),
  box_set_(0)
{
}


fltk3::Style::~Style()
{
}


fltk3::Style *fltk3::Style::as_public() 
{
  if (!private_)
    return this;
  else {
    refresh();
    Style *s = new Style(this);
    s->private_ = 0;
    while (parent_ && parent_->private_ && parent_->parent_)
      parent_ = parent_->parent_;
    return this;
  }
}

fltk3::Style *fltk3::Style::make_private() 
{
  if (private_)
    return this;
  else {
    refresh();
    Style *s = new Style(this);
    s->private_ = 1;
    return s;
  }
}


unsigned int fltk3::Style::update()
{
  if (version_==current_ || parent_==0L)
    return current_;
  parent_->refresh();
  
  if (!labelfont_set_)
    labelfont_ = parent_->labelfont_;
  if (!labelsize_set_)
    labelsize_ = parent_->labelsize_;
  if (!labeltype_set_)
    labeltype_ = parent_->labeltype_;
  if (!labelcolor_set_)
    labelcolor_ = parent_->labelcolor_;
  if (!align_set_)
    align_ = parent_->align_;
  if (!textfont_set_)
    textfont_ = parent_->textfont_;
  if (!textsize_set_)
    textsize_ = parent_->textsize_;
  if (!textcolor_set_)
    textcolor_ = parent_->textcolor_;
  if (!color_set_)
    color_ = parent_->color_;
  if (!color2_set_)
    color2_ = parent_->color2_;
  if (!box_set_)
    box_ = parent_->box_;
  
  version_ = current_;
  return current_;
}


//
// End of "$Id$".
//
