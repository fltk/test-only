// button.cxx (example2a)

#include <stdlib.h>
#include <stdio.h>
#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/Button.h>
#if 0
#include <fltk/InvisibleBox.h>
#include <fltk/TiledImage.h>
#include <fltk/SharedImage.h>
#endif

using namespace fltk;

void beepcb(Widget *, void *) {
  printf("\007"); fflush(stdout);
}

void exitcb(Widget *, void *) {
  exit(0);
}

int main(int argc, char ** argv) {
#if 0
  register_images();
#endif
  Window *window = new Window(320,65);

  window->begin();
  Group* ib = new Group(0,0,window->w(),window->h());
  ib->begin();
#if 0
  ib->image(new TiledImage(SharedImage::get("./images/bg.jpeg")));
#endif
  window->resizable(ib);

  Button *b1 = new Button(20, 20, 80, 25, "&Beep");
  b1->callback(beepcb,0);
  
  Button *b2 = new Button(120,20, 80, 25, "");
#if 0
  b2->image(new TiledImage(SharedImage::get("./images/coucou.png")));
#endif

  Button *b3 = new Button(220,20, 80, 25, "E&xit");
  b3->callback(exitcb,0);
  ib->end();
  window->end();
  window->show(argc,argv);
  return run();
}

//
// End of "$Id: button.cxx 5465 2006-09-19 03:21:56Z spitzak $".
//
