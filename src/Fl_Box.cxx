//
// "$Id: Fl_Box.cxx,v 1.26 2002/12/09 04:52:24 spitzak Exp $"
//
// Back compatability widget

#include <fltk/Widget.h>
#include <FL/Fl_Box.h>
using namespace fltk;

static void revert(Style* s) {
  s->color = GRAY75;
  s->box = NO_BOX;
}
// this is unnamed as there is no need for themes to alter this:
static NamedStyle style(0, revert, &Fl_Box::default_style);
NamedStyle* Fl_Box::default_style = &::style;

Fl_Box::Fl_Box(int x, int y, int w, int h, const char *l)
  : Widget(x,y,w,h,l)
{
  style(default_style);
}

Fl_Box::Fl_Box(Box* b, int x, int y, int w, int h, const char *l)
  : Widget(x,y,w,h,l)
{
  style(default_style);
  box(b);
}

extern Widget* fl_did_clipping;

void Fl_Box::draw() {
  // check for completely blank widgets. We must not clip to their
  // area because it will break lots of programs that assumme these
  // can overlap any other widgets:
  if (box() == NO_BOX && 
    (!label() && !image() ||
     align() != ALIGN_CENTER && !(align()&ALIGN_INSIDE))) {
    fl_did_clipping = this;
    return;
  }
  Widget::draw();
}

//
// End of "$Id: Fl_Box.cxx,v 1.26 2002/12/09 04:52:24 spitzak Exp $".
//
