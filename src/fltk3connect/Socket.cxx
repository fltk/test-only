//
// "$Id$"
//
// FLTK3 connect library source file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2002-2012 by Matthias Melcher and others.
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

#include <fltk3connect/Socket.h>


void fltk3::Socket::draw()
{
  fltk3::Group::draw();
}


fltk3::Socket::Socket(int x, int y, int w, int h, const char *l)
: fltk3::Group(x, y, w, h, l),
  pStatus(CLOSED),
  pActive(0),
  pFirstByteTimeout(0),
  pNextByteTimeout(0)
{
}


fltk3::Socket::~Socket()
{
}


int fltk3::Socket::connect(const char*)
{
  return -1;
}


int fltk3::Socket::send(const void*, int)
{
  return -1;
}


char fltk3::Socket::send(const char *text)
{
  if (text) {
    return send(text, strlen(text));
  } else {
    return -1;
  }
}


int fltk3::Socket::recv_all(void*, int)
{
  return -1;
}


int fltk3::Socket::recv(void*, int)
{
  return -1;
}


int fltk3::Socket::peek(void*, int) 
{
  return -1;
}


void fltk3::Socket::close()
{
}


char fltk3::Socket::on_connect()
{
  return 0;
}


char fltk3::Socket::on_receive()
{
  return 0;
}


char fltk3::Socket::on_close()
{
  return 0;
}


char fltk3::Socket::on_timeout()
{
  return 0;
}


int fltk3::Socket::monitor(int trigger, ...)
{
  return -1;
}


int fltk3::Socket::status() const
{
  return pStatus;
}


int fltk3::Socket::available() const
{
  return -1;
}


void fltk3::Socket::flush_input(int n)
{
}


void fltk3::Socket::flush_output()
{
}


void fltk3::Socket::timeout(double first_byte, double next_byte)
{
  pFirstByteTimeout = first_byte;
  pNextByteTimeout = next_byte;
}


//
// End of "$Id$".
//
