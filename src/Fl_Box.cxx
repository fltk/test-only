//
// "$Id: Fl_Box.cxx,v 1.30 2003/11/04 08:16:04 spitzak Exp $"
//

/** Widget with default box of NO_BOX.

    This is a box that is invisible due to not having a box. The
    label still prints so it can be used to position labels. Also
    this is useful as a resizable() widget.
*/

#include <fltk/InvisibleBox.h>
using namespace fltk;

static void revert(Style* s) {
  s->color_ = GRAY75;
  s->box_ = NO_BOX;
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

void InvisibleBox::draw() {
  Widget::draw();
}

//
// End of "$Id: Fl_Box.cxx,v 1.30 2003/11/04 08:16:04 spitzak Exp $".
//
