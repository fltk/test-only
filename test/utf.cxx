// Test of UTF-8 handling in fltk.

#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/Scrollbar.h>
#include <fltk/draw.h>
#include <fltk/string.h>
#include <fltk/Symbol.h>
#include <fltk/Choice.h>
#include <fltk/Font.h>
#include <fltk/utf.h>
#include <fltk/math.h>

using namespace fltk;

fltk::Font** fonts; // list returned by fltk
Scrollbar* scrollbar;

class Drawing : public Widget {
  void draw();
public:
  Drawing(int x, int y, int w, int h) : Widget(x,y,w,h) {}
  void layout();
};

Drawing *drawing;

void Drawing::draw() {
  push_clip(x(),y(),w(),h());
  unsigned scroll = unsigned(scrollbar->value());
  unsigned itemh = unsigned(textsize()+8);
  unsigned base = scroll/itemh*16;
  int y = -(scroll%itemh);
  Symbol* box = this->box();
  for (; y < h(); y+=itemh, base+=16) {
    char buf[20];
    sprintf(buf, "U+%03Xx", base>>4);
    int x1 = 0; int x2 = w()*2/18;
    box->draw(x1,y,x2-x1,itemh,style(),OUTPUT);
    setcolor(labelcolor());
    setfont(labelfont(),labelsize());
    drawtext(buf,x1,y,x2-x1,itemh,0);
    setfont(textfont(),textsize());
    for (int z = 0; z < 16; z++) {
      x1 = x2; x2 = w()*(3+z)/18;
      char* p = buf;
      p += utf8encode(base+z,p);
      *p = 0;
      box->draw(x1,y,x2-x1,itemh,style(),OUTPUT);
      setcolor(labelcolor());
      drawtext(buf,x1,y,x2-x1,itemh,0);
    }
  }
  pop_clip();
}

void Drawing::layout() {
  unsigned itemh = unsigned(textsize()+8);
  //actually the height should be 0x11000*itemh to get all Unicode:
  scrollbar->value(scrollbar->value(),h(),0,0x1000*itemh);
  scrollbar->linesize(itemh);
  Widget::layout();
}

void scroll_cb(Widget* w, void*) {
  drawing->redraw();
}

void font_cb(Widget* w, void*) {
  drawing->textfont(fonts[((Choice*)w)->value()]);
  drawing->redraw();
}

void size_cb(Widget* w, void*) {
  drawing->textsize(rint(((Slider*)w)->value()));
  drawing->relayout();
  drawing->redraw();
}

#define WH 24

int main(int argc, char** argv) {
  Window window(400,400+2*WH);
  window.begin();
  Drawing drawing(0,0,385,400);
  ::drawing = &drawing;
  window.resizable(drawing);
  Scrollbar scrollbar(385,0,15,400);
  scrollbar.set_vertical();
  ::scrollbar = &scrollbar;
  scrollbar.callback(scroll_cb);
  Choice fontchooser(0,400,400,WH);
  int numfonts = fltk::list_fonts(fonts);
  int i; for (i = 0; i < numfonts; i++) fontchooser.add(fonts[i]->name());
  fontchooser.callback(font_cb);
  Slider sizeslider(0,400+WH,400,WH);
  sizeslider.type(Slider::TICK_ABOVE|Slider::LOG);
  sizeslider.range(5,50);
  sizeslider.callback(size_cb);
  window.end();
  window.show(argc,argv);
  sizeslider.value(drawing.textsize());
  for (i=0; i < numfonts; i++)
    if (drawing.textfont()==fonts[i]) {
      fontchooser.value(i);
      break;
    }
  return run();
}
