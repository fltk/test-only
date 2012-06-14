//
// "$Id$"
//
// PPP example program for the Fast Light Tool Kit (FLTK).
//
// Program to make a button to turn a ppp connection on/off.
// You must chmod +s /usr/sbin/pppd, and put all the options
// into /etc/ppp/options.
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


#include <fltk3/run.h>
#include <fltk3/Window.h>
#include <fltk3/Button.h>
#include <fltk3connect/all.h>


void ftp_cb(fltk3::Widget*, void *d)
{
#if 0
  fltk3::FTPClient *ftp = (fltk3::FTPClient*)d;
  ftp->sync_open("ftp.gnu.org", "anonymous", "fltk@fltk.org");
  ftp->close();
#endif
#if 0
  unsigned char ip0, ip1, ip2, ip3;
  fltk3::TCPSocket* s = new fltk3::TCPSocket(0, 0, 0, 0, 0);
  s->find_host("fltk.org", ip0, ip1, ip2, ip3);
#endif
#if 1
  fltk3::HTTPClient *http = (fltk3::HTTPClient*)d;
  http->connect("www.fltk.org");
  http->GET("index.php");  
#endif
}


int main(int argc, char** argv)
{
  // ftp.gnu.org
  fltk3::Window* win = new fltk3::Window(300, 100, "FTP test");
  win->begin();
  fltk3::Button* btn = new fltk3::Button(10, 10, 150, 24, "ftp.gnu.org");
#if 0
  fltk3::FTPClient* ftp = new fltk3::FTPClient(100, 44, 50, 24, "FTP:");
  ftp->align(fltk3::ALIGN_LEFT);
  btn->callback(ftp_cb, ftp);
#endif
#if 1
  fltk3::HTTPClient* http = new fltk3::HTTPClient(100, 44, 50, 24, "HTTP:");
  http->align(fltk3::ALIGN_LEFT);
  btn->callback(ftp_cb, http);
#endif
  win->end();
  win->show(argc, argv);
  fltk3::run();
  return 0;
}


//
// End of "$Id$".
//
