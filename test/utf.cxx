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
#include <fltk/Tooltip.h>
#include <fltk/events.h>
using namespace fltk;

fltk::Font** fonts; // list returned by fltk
Scrollbar* scrollbar;

class Drawing : public Widget {
  void draw();
  int handle(int);
public:
  Drawing(int x, int y, int w, int h) : Widget(x,y,w,h) {}
  void layout();
};

Drawing *drawing;

// Return color to draw behind given code
Color code_color(unsigned ucs) {
  if (ucs < 32 || ucs==127 // ascii control characters
      // || ucs >= 0x80 && ucs <= 0x9f // C1 control characters
      || ucs >= 0xd800 && ucs <= 0xdfff // surrogate pairs
      || ucs >= 0xfdd0 && ucs <= 0xfdef // noncharacters
      || (ucs&0xffff) >= 0xfffe // noncharacters
      ) return GRAY50;
  if (ucs & 0x100) return GRAY80;
  return WHITE;
}

void Drawing::draw() {
  push_clip(x(),y(),w(),h());
  unsigned scroll = unsigned(scrollbar->value());
  unsigned itemh = unsigned(textsize()+8);
  unsigned base = scroll*16;
  int y = 0;
  Symbol* box = this->box();
  Style style = *(this->style());
  for (; y < h(); y+=itemh, base+=16) {
    char buf[20];
    sprintf(buf, "U+%03Xx", base>>4);
    int x1 = 0; int x2 = w()*2/18;
    style.color_ = (base&0x100) ? GRAY80 : WHITE;
    box->draw(x1,y,x2-x1,itemh,&style,OUTPUT);
    setcolor(labelcolor());
    setfont(labelfont(),labelsize());
    drawtext(buf,x1,y,x2-x1,itemh,0);
    setfont(textfont(),textsize());
    for (int z = 0; z < 16; z++) {
      x1 = x2; x2 = w()*(3+z)/18;
      char* p = buf;
      // if (base < 0x100) *p++ = base+z; else // demonstrate cp1252 emulation
      p += utf8encode(base+z,p);
      *p = 0;
      style.color_ = code_color(base+z);
      box->draw(x1,y,x2-x1,itemh,&style,OUTPUT);
      setcolor(labelcolor());
      drawtext(buf,x1,y,x2-x1,itemh,0);
    }
  }
  pop_clip();
}

const char* generator(Widget*, void* data) {
  unsigned ucs = (unsigned)data;
  static char buffer[20];
  sprintf(buffer,"U+%04x",ucs);
  return buffer;
}

int Drawing::handle(int event) {
  switch (event) {
  case ENTER:
  case MOVE:
  case DRAG:
  case PUSH:
  case RELEASE:
    break;
  default:
    return Widget::handle(event);
  }
  unsigned itemh = unsigned(textsize()+8);
  int x = event_x()*18/w()-2;
  int y = event_y()/itemh;
  if (x < 0 || x > 15 || y < 0 || event_y()>h()) {
    Tooltip::exit();
    return true;
  }
  unsigned scroll = unsigned(scrollbar->value());
  unsigned base = scroll*16;
  unsigned ucs = base+y*16+x;
  Tooltip::enter(this, w()*(2+x)/18, y*itemh, (w()+9)/18, itemh,
		 generator, (void*)ucs);
  return true;
}

void Drawing::layout() {
  unsigned itemh = unsigned(textsize()+8);
  //actually the height should be 0x11000*itemh to get all Unicode:
  scrollbar->value(scrollbar->value(),h()/itemh,0,0x1000);
  scrollbar->linesize(1);
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
