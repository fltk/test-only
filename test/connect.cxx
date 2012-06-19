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
#include <fltk3/Input.h>
#include <fltk3/TextDisplay.h>
#include <fltk3/TextBuffer.h>
#include <fltk3connect/all.h>

#include <stdio.h>


fltk3::Window *win;
fltk3::Group *grp, *top;
fltk3::HTTPClient *httpClient;
fltk3::Input *url;
fltk3::Button *reload;
fltk3::TextDisplay *text;


static void refresh_text_cb(fltk3::Widget* w, void*)
{
  text->buffer()->text( (char*)httpClient->file_data() );
}


static void reload_cb(fltk3::Widget* w, void*)
{
  httpClient->GET( url->value() );
}


int main(int argc, char** argv)
{
  win = new fltk3::Window(400, 600, "HTTP Client Test");
  grp = new fltk3::Group(5, 5, 390, 590);
  
  top = new fltk3::Group(0, 0, 390, 20);

  url = new fltk3::Input(0, 0, 320, 20);
  url->value("http://www.fltk.org/index.php");
  url->when(fltk3::WHEN_RELEASE);
  url->callback(reload_cb);
  reload = new fltk3::Button(320, 0, 50, 20, "Load");
  reload->callback(reload_cb);
  httpClient = new fltk3::HTTPClient(370, 0, 20, 20);
  httpClient->callback(refresh_text_cb);
  
  top->resizable(url);
  top->end();
  
  text = new fltk3::TextDisplay(0, 25, 390, 565);
  text->buffer(new fltk3::TextBuffer());
  
  grp->resizable(text);
  grp->end();
  
  win->resizable(grp);
  win->end();

  win->show(argc, argv);
  return fltk3::run();
}


//
// End of "$Id$".
//
