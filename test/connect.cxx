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

#if 1


#include <fltk3/run.h>
#include <fltk3/Window.h>
#include <fltk3/Button.h>
#include <fltk3connect/all.h>


void ftp_cb(fltk3::Widget*, void *d)
{
  fltk3::FTPConnection *ftp = (fltk3::FTPConnection*)d;
  
  ftp->sync_open("ftp.gnu.org", "anonymous", "fltk@fltk.org");
  //ftp->dir()....
  ftp->close();
}


int main(int argc, char** argv)
{
  // ftp.gnu.org
  fltk3::Window* win = new fltk3::Window(300, 100, "FTP test");
  win->begin();
  fltk3::Button* btn = new fltk3::Button(10, 10, 150, 24, "ftp.gnu.org");
  fltk3::FTPConnection* ftp = new fltk3::FTPConnection(100, 44, 50, 24, "FTP:");
  ftp->align(fltk3::ALIGN_LEFT);
  btn->callback(ftp_cb, ftp);
  win->end();
  win->show(argc, argv);
  fltk3::run();
  return 0;
}



#else



#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <fltk3/run.h>
#include <fltk3/Window.h>
#include <fltk3/ToggleButton.h>

int running;	// actually the pid
fltk3::ToggleButton *Button;

void sigchld(int) {
  waitpid(running, 0, 0);
  running = 0;
  Button->value(0);
}

void cb(Fl_Widget *o, void *) {
  if (((fltk3::ToggleButton*)o)->value()) {
    if (running) return;
    running = fork();
    if (!running) execl("/usr/sbin/pppd","pppd","-detach",0);
    else signal(SIGCHLD, sigchld);
  } else {
    if (!running) return;
    kill(running, SIGINT);
    waitpid(running, 0, 0);
    running = 0;
  }
}

int main(int argc, char ** argv) {
   Fl_Window window(100,50);
   fltk3::ToggleButton button(0,0,100,50,"Connect");
   Button = &button;
   button.color(1,2);
   button.callback(cb,0);
   window.show(argc,argv);
   return fltk3::run();
}



#endif

//
// End of "$Id$".
//
