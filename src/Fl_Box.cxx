//
// "$Id: Fl_Box.cxx,v 1.27 2002/12/15 10:42:53 spitzak Exp $"
//
// This is a box that is invisible due to not having a box. The
// label still prints so it can be used to position labels. Also
// this is useful as a resizable() widget.

#include <fltk/InvisibleBox.h>
using namespace fltk;

static void revert(Style* s) {
  s->color = GRAY75;
  s->box = NO_BOX;
}
// this is unnamed as there is no need for themes to alter this:
static NamedStyle style(0, revert, &InvisibleBox::default_style);
NamedStyle* InvisibleBox::default_style = &::style;

InvisibleBox::InvisibleBox(int x, int y, int w, int h, const char *l)
  : Widget(x,y,w,h,l)
{
  style(default_style);
}

InvisibleBox::InvisibleBox(Box* b, int x, int y, int w, int h, const char *l)
  : Widget(x,y,w,h,l)
{
  style(default_style);
  box(b);
}

extern Widget* fl_did_clipping;

void InvisibleBox::draw() {
  // check for completely blank widgets. We must not clip to their
  // area because it will break lots of programs that assumme these
  // can overlap any other widgets:
  if (box() == NO_BOX
      /* && (!label() && !image() ||
	    align() != ALIGN_CENTER && !(align()&ALIGN_INSIDE))*/) {
    fl_did_clipping = this;
  }
  Widget::draw();
}

//
// End of "$Id: Fl_Box.cxx,v 1.27 2002/12/15 10:42:53 spitzak Exp $".
//
