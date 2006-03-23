// Test of List to see if all menus can handle it.

#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/PopupMenu.h>
#include <fltk/Choice.h>
#include <fltk/Browser.h>
#include <fltk/StringList.h>
#include <fltk/Item.h>
#include <fltk/string.h>
using namespace fltk;

const int SIZE = 50;
const int DEPTH = 3;

class MyList : public List {
public:

  int children(const Menu* group, const int* indexes, int level) {
    if (level < DEPTH) return SIZE;
    return -1;
  }

  Widget* child(const Menu* group, const int* indexes, int level) {
    int n = indexes[level];
    // FLTK is never supposed to ask for a widget outside the children range,
    // so this should never print:
    if (n < 0 || n >= SIZE) {printf("Asked for item %d\n", n); return 0;}
    // Construct reusable widget that we will return:
    static Widget* widget;
    if (!widget) {
      Group::current(0);
      widget = new Item();
    }
    // produce a label for the widget. You can reuse the buffer:
    static char buffer[100];
    if (level) sprintf(buffer, "Level %d Item %d", level, n);
    else sprintf(buffer, "Top Item %d", n);
    widget->label(buffer);
    // We must clear flags so it does not accidentally think the item
    // is selected or (if a parent) is opened:
    widget->clear_flag(fltk::VALUE);
    // This indicates we have no idea what the dimensions are:
    widget->w(0);
    return widget;
  }
};

int main(int argc, char** argv) {
  MyList list;
  Window window(330,300,"Sample fltk::List subclass");
  window.begin();
  Browser browser(10,10,200,280);
  browser.list(&list);
  browser.indented(true);
  Choice choice(220,10,100,25);
  choice.list(&list);
  PopupMenu button(220,55,100,25,"pushme");
  button.list(&list);
  window.resizable(browser);
  window.resize_align(ALIGN_TOP);
  window.end();
  window.show(argc, argv);
  return fltk::run();
}
