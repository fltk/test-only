//
// "$Id: Fl_Color_Chooser.cxx,v 1.32 2002/12/09 04:52:24 spitzak Exp $"
//
// Color chooser for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/ColorChooser.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <fltk/Item.h>
#include <fltk/math.h>
#include <stdio.h>
using namespace fltk;

// Besides being a useful object on it's own, the ColorChooser was
// an attempt to make a complex composite object that could be easily
// imbedded into a user interface.  If you wish to make complex objects
// of your own, be sure to read this code.

// The function color_chooser() creates a window containing a color
// chooser and a few buttons and current-color indicators.  It is an
// easier interface for simple programs that just need a color.

// The "hue box" can be a circle or rectilinear.
// You get a circle by defining this:
// #define CIRCLE 1
// And the "hue box" can auto-update when the value changes
// you get this by defining this:
#define UPDATE_HUE_BOX 1

void ColorChooser::hsv2rgb(
	float H, float S, float V, float& r, float& g, float& b) {
  if (S < 5.0e-6) {
    r = g = b = V;
  } else {
    int i = (int)H;  
    float f = H - (float)i;
    float p1 = V*(1.0f-S);
    float p2 = V*(1.0f-S*f);
    float p3 = V*(1.0f-S*(1.0f-f));
    switch (i) {
    case 0: r = V;   g = p3;  b = p1;  break;
    case 1: r = p2;  g = V;   b = p1;  break;
    case 2: r = p1;  g = V;   b = p3;  break;
    case 3: r = p1;  g = p2;  b = V;   break;
    case 4: r = p3;  g = p1;  b = V;   break;
    case 5: r = V;   g = p1;  b = p2;  break;
    }
  }
}

void ColorChooser::rgb2hsv(
	float r, float g, float b, float& H, float& S, float& V) {
  float maxv = r > g ? r : g; if (b > maxv) maxv = b;
  V = maxv;
  if (maxv>0) {
    float minv = r < g ? r : g; if (b < minv) minv = b;
    S = 1.0f - float(minv)/maxv;
    if (maxv > minv) {
      if (maxv == r) {H = (g-b)/float(maxv-minv); if (H<0) H += 6;}
      else if (maxv == g) H = 2+(b-r)/float(maxv-minv);
      else H = 4+(r-g)/float(maxv-minv);
    }
  }
}

enum {M_RGB, M_BYTE, M_HEX, M_HSV}; // modes

int ccValueInput::format(char* buf) {
  ColorChooser* c = (ColorChooser*)parent()->parent();
  if (c->mode() == M_HEX) return sprintf(buf,"0x%02X", int(value()));
  else return Valuator::format(buf);
}

void ColorChooser::set_valuators() {
  switch (mode()) {
  case M_RGB:
    rvalue.range(0,1); rvalue.step(.001); rvalue.value(r_);
    gvalue.range(0,1); gvalue.step(.001); gvalue.value(g_);
    bvalue.range(0,1); bvalue.step(.001); bvalue.value(b_);
    break;
  case M_BYTE:
  case M_HEX:
    rvalue.range(0,255); rvalue.step(1); rvalue.value(int(255*r_+.5f));
    gvalue.range(0,255); gvalue.step(1); gvalue.value(int(255*g_+.5f));
    bvalue.range(0,255); bvalue.step(1); bvalue.value(int(255*b_+.5f));
    break;
  case M_HSV:
    rvalue.range(0,6); rvalue.step(.001); rvalue.value(hue_);
    gvalue.range(0,1); gvalue.step(.001); gvalue.value(saturation_);
    bvalue.range(0,1); bvalue.step(.001); bvalue.value(value_);
    break;
  }
}

int ColorChooser::rgb(float r, float g, float b) {
  if (r == r_ && g == g_ && b == b_) return 0;
  r_ = r; g_ = g; b_ = b;
  float ph = hue_;
  float ps = saturation_;
  float pv = value_;
  rgb2hsv(r,g,b,hue_,saturation_,value_);
  set_valuators();
  if (value_ != pv) {
#ifdef UPDATE_HUE_BOX
    huebox.redraw(DAMAGE_CONTENTS);
#endif
    valuebox.redraw(DAMAGE_VALUE);}
  if (hue_ != ph || saturation_ != ps) {
    huebox.redraw(DAMAGE_VALUE); 
    valuebox.redraw(DAMAGE_CONTENTS);
  }
  return 1;
}

int ColorChooser::hsv(float h, float s, float v) {
  h = fmodf(h,6.0f); if (h < 0) h += 6;
  if (s < 0) s = 0; else if (s > 1) s = 1;
  if (v < 0) v = 0; else if (v > 1) v = 1;
  if (h == hue_ && s == saturation_ && v == value_) return 0;
  float ph = hue_;
  float ps = saturation_;
  float pv = value_;
  hue_ = h; saturation_ = s; value_ = v;
  if (value_ != pv) {
#ifdef UPDATE_HUE_BOX
    huebox.redraw(DAMAGE_CONTENTS);
#endif
    valuebox.redraw(DAMAGE_VALUE);}
  if (hue_ != ph || saturation_ != ps) {
    huebox.redraw(DAMAGE_VALUE); 
    valuebox.redraw(DAMAGE_CONTENTS);
  }
  hsv2rgb(h,s,v,r_,g_,b_);
  set_valuators();
  return 1;
}

////////////////////////////////////////////////////////////////

static void tohs(float x, float y, float& h, float& s) {
#ifdef CIRCLE
  x = 2*x-1;
  y = 1-2*y;
  s = sqrtf(x*x+y*y); if (s > 1) s = 1;
  h = float(3.0/M_PI)*atan2f(y,x);
  if (h<0) h += 6;
#else
  h = fmodf(6*x,6.0f); if (h < 0) h += 6;
  s = 1-y; if (s < 0) s = 0; else if (s > 1) s = 1;
#endif
}

int ccHueBox::handle(int e) {
  static float is;
  ColorChooser* c = (ColorChooser*)parent();
  switch (e) {
  case PUSH:
    is = c->s();
  case DRAG: {
    float Xf, Yf, H, S;
    int ix = 0; int iy = 0; int iw = w(); int ih = h();
    box()->inset(ix, iy, iw, ih);
    Xf = (event_x()-ix)/float(iw);
    Yf = (event_y()-iy)/float(ih);
    tohs(Xf, Yf, H, S);
    if (fabsf(H-ih) < 3*6.0f/w()) H = ih;
    if (fabsf(S-is) < 3*1.0f/h()) S = is;
    if (event_state(CTRL)) H = ih;
    if (c->hsv(H, S, c->v())) c->do_callback();
    } return 1;
  default:
    return 0;
  }
}

static void generate_image(void* vv, int X, int Y, int W, uchar* buf) {
  ccHueBox* v = (ccHueBox*)vv;
  int x1 = 0; int y1 = 0; int w1 = v->w(); int h1 = v->h();
  v->box()->inset(x1,y1,w1,h1);
  float Yf = float(Y)/h1;
#ifdef UPDATE_HUE_BOX
  const float V = ((ColorChooser*)(v->parent()))->v();
#else
  const float V = 1.0f;
#endif
  for (int x = X; x < X+W; x++) {
    float Xf = float(x)/w1;
    float H,S; tohs(Xf,Yf,H,S);
    float r,g,b;
    ColorChooser::hsv2rgb(H,S,V,r,g,b);
    *buf++ = uchar(255*r+.5f);
    *buf++ = uchar(255*g+.5f);
    *buf++ = uchar(255*b+.5f);
  }
}

void ccHueBox::draw() {
  if (damage()&DAMAGE_ALL) draw_frame();
  int x1 = 0; int y1 = 0; int w1 = w(); int h1 = h();
  box()->inset(x1,y1,w1,h1);
  if (damage() == DAMAGE_VALUE) push_clip(x1+px,y1+py,6,6);
  drawimage(generate_image, this, x1, y1, w1, h1);
  if (damage() == DAMAGE_VALUE) pop_clip();
  ColorChooser* c = (ColorChooser*)parent();
#ifdef CIRCLE
  int X = int(.5f*(cosf(c->h()*float(M_PI/3.0))*c->s()+1) * (w1-6));
  int Y = int(.5f*(1-sinf(c->h()*float(M_PI/3.0))*c->s()) * (h1-6));
#else
  int X = int(c->h()/6.0f*(w1-6));
  int Y = int((1-c->s())*(h1-6));
#endif
  if (X < 0) X = 0; else if (X > w1-6) X = w1-6;
  if (Y < 0) Y = 0; else if (Y > h1-6) Y = h1-6;
  //  color(c->v()>.75 ? BLACK : WHITE);
  buttonbox()->draw(x1+X, y1+Y, 6, 6, color(), 0);
  px = X; py = Y;
}

////////////////////////////////////////////////////////////////

int ccValueBox::handle(int e) {
  static float iv;
  ColorChooser* c = (ColorChooser*)parent();
  switch (e) {
  case PUSH:
    iv = c->v();
  case DRAG: {
    float Yf;
    int x1 = 0; int y1 = 0; int w1 = w(); int h1 = h();
    box()->inset(x1,y1,w1,h1);
    Yf = 1-(event_y()-y1)/float(h1);
    if (fabsf(Yf-iv)<(3*1.0f/h())) Yf = iv;
    if (c->hsv(c->h(),c->s(),Yf)) c->do_callback();
    } return 1;
  default:
    return 0;
  }
}

static float tr, tg, tb;
static void generate_vimage(void* vv, int X, int Y, int W, uchar* buf) {
  ccValueBox* v = (ccValueBox*)vv;
  int x1 = 0; int y1 = 0; int w1 = v->w(); int h1 = v->h();
  v->box()->inset(x1,y1,w1,h1);
  float Yf = 255*(1-float(Y)/h1);
  uchar r = uchar(tr*Yf+.5f);
  uchar g = uchar(tg*Yf+.5f);
  uchar b = uchar(tb*Yf+.5f);
  for (int x = X; x < X+W; x++) {
    *buf++ = r; *buf++ = g; *buf++ = b;
  }
}

void ccValueBox::draw() {
  if (damage()&DAMAGE_ALL) draw_frame();
  ColorChooser* c = (ColorChooser*)parent();
  c->hsv2rgb(c->h(),c->s(),1.0f,tr,tg,tb);
  int x1 = 0; int y1 = 0; int w1 = w(); int h1 = h();
  box()->inset(x1,y1,w1,h1);
  if (damage() == DAMAGE_VALUE) push_clip(x1,y1+py,w1,6);
  drawimage(generate_vimage, this, x1, y1, w1, h1);
  if (damage() == DAMAGE_VALUE) pop_clip();
  int Y = int((1-c->v()) * (h1-6));
  if (Y < 0) Y = 0; else if (Y > h1-6) Y = h1-6;
  buttonbox()->draw(x1, y1+Y, w1, 6, color(), 0);
  py = Y;
}

////////////////////////////////////////////////////////////////

void ColorChooser::rgb_cb(Widget* o, void*) {
  ColorChooser* c = (ColorChooser*)(o->parent()->parent());
  float r = float(c->rvalue.value());
  float g = float(c->gvalue.value());
  float b = float(c->bvalue.value());
  if (c->mode() == M_HSV) {
    if (c->hsv(r,g,b)) c->do_callback();
    return;
  }
  if (c->mode() != M_RGB) {
    r = r/255;
    g = g/255;
    b = b/255;
  }
  if (c->rgb(r,g,b)) c->do_callback();
}

void ColorChooser::mode_cb(Widget* o, void*) {
  ColorChooser* c = (ColorChooser*)(o->parent()->parent());
  // force them to redraw even if value is the same:
  c->rvalue.value(-1);
  c->gvalue.value(-1);
  c->bvalue.value(-1);
  c->set_valuators();
}

////////////////////////////////////////////////////////////////

ColorChooser::ColorChooser(int X, int Y, int W, int H, const char* L)
  : Group(0,0,180,100,L,true),
    huebox(0,0,100,100),
    valuebox(100,0,20,100),
    nrgroup(120,0, 60, 100, 0, true),
    choice(0,0,60,21),
    rvalue(0,22,60,21),
    gvalue(0,44,60,21),
    bvalue(0,66,60,21)
{
  nrgroup.end();
  choice.begin();
  new Item("rgb");
  new Item("byte");
  new Item("hex");
  new Item("hsv");
  choice.end();
  end();
  resizable(huebox);
  sizes();
  resize(X,Y,W,H);
  r_ = g_ = b_ = 0;
  hue_ = 0.0;
  saturation_ = 0.0;
  value_ = 0.0;
  set_valuators();
  rvalue.callback(rgb_cb);
  gvalue.callback(rgb_cb);
  bvalue.callback(rgb_cb);
  choice.callback(mode_cb);
}

Color ColorChooser::value() const {
  Color ret =
    fltk::color(uchar(255*r()+.5f), uchar(255*g()+.5f), uchar(255*b()+.5f));
  return ret ? ret : BLACK;
}

void ColorChooser::value(Color c) {
  uchar r,g,b; split_color(c,r,g,b);
  rgb(r/255.0f, g/255.0f, b/255.0f);
}

////////////////////////////////////////////////////////////////
// color_chooser():

#include <fltk/Window.h>
#include <fltk/ReturnButton.h>
#include <fltk/ask.h>

static Window* window;
static ColorChooser* chooser;
static Widget* ok_color;
static Button* ok_button;
static Widget* cancel_color;
static Button* cancel_button;

// this location is used to preserve index Color values:
static Color picked_color;

static void chooser_cb(Widget*, void*) {
  ok_color->color(picked_color = chooser->value());
  ok_color->redraw();
}

#define ROWS 4
#define COLS 16

FL_API Color fl_color_cells[ROWS*COLS] = {
// first the 16 assignable fltk color indexes:
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
// then the closest gray ramps to 0,1/4,1/3,1/2,2/3,3/4,1:
32,32,32,37,37,39,39,43,43,47,47,49,49,55,55,55,
// repeat it twice:
32,32,32,37,37,39,39,43,43,47,47,49,49,55,55,55};

class CellBox : public Widget {
public:
  CellBox(int X, int Y, int W, int H) : Widget(X,Y,W,H) {}
  void draw();
  int handle(int);
};

void CellBox::draw() {
  for (int Y = 0; Y < ROWS; Y++) {
    int yy = Y*h()/ROWS;
    int hh = (Y+1)*h()/ROWS - yy;
    for (int X = 0; X < COLS; X++) {
      int xx = X*w()/COLS;
      int ww = (X+1)*w()/COLS - xx;
      THIN_DOWN_BOX->draw(xx,yy,ww,hh,fl_color_cells[Y*COLS+X]);
    }
  }
}

int CellBox::handle(int e) {
  switch (e) {
  case PUSH: return 1;
  case DRAG: return 1;
  case RELEASE: {
    int X = event_x()*COLS/w();
    if (X < 0 || X >= COLS) return 1;
    int Y = event_y()*ROWS/h();
    if (Y < 0 || Y >= ROWS) return 1;
    X = X+Y*COLS;
    if (event_button() > 1) {
      fl_color_cells[X] = picked_color;
      redraw();
    } else {
      chooser->value(picked_color = fl_color_cells[X]);
      ok_color->color(picked_color);
      ok_color->redraw();
    }
    return 1;}
  }
  return Widget::handle(e);
}

static void ok_cb(Widget* w, void*) {
  w->window()->set_value();
  w->window()->hide();
}

static void cancel_cb(Widget* w, void*) {
  w->window()->hide();
}

static void make_it() {
  if (window) return;
  window = new Window(210,212);
  window->begin();
  chooser = new ColorChooser(5, 5, 200, 100);
  chooser->callback(chooser_cb);
  new CellBox(5,110,200,52);
  ok_color = new Widget(5, 165, 95, 21);
  ok_color->box(ENGRAVED_BOX);
  ok_button = new ReturnButton(5, 186, 95, 21, ok);
  ok_button->callback(ok_cb);
  cancel_color = new Widget(110, 165, 95, 21);
  cancel_color->box(ENGRAVED_BOX);
  cancel_button = new Button(110, 186, 95, 21, cancel);
  cancel_button->callback(cancel_cb);
  // window->size_range(210, 240); // minimum usable size?
  window->resizable(chooser);
  window->end();
}

static int run_it(const char* name) {
  window->label(name);
  ok_color->color(chooser->value());
  cancel_color->color(chooser->value());
  window->hotspot(window);
  return window->exec();
}

int fltk::color_chooser(const char* name, float& r, float& g, float& b) {
  make_it();
  chooser->rgb(r,g,b);
  if (!run_it(name)) return 0;
  r = chooser->r();
  g = chooser->g();
  b = chooser->b();
  return 1;
}

int fltk::color_chooser(const char* name, uchar& r, uchar& g, uchar& b) {
  make_it();
  chooser->rgb(r/255.0f, g/255.0f, b/255.0f);
  if (!run_it(name)) return 0;
  r = uchar(255*chooser->r()+.5f);
  g = uchar(255*chooser->g()+.5f);
  b = uchar(255*chooser->b()+.5f);
  return 1;
}

int fltk::color_chooser(const char* name, Color& c) {
  make_it();
  chooser->value(c);
  picked_color = c;
  if (!run_it(name)) return 0;
  c = picked_color;
  return 1;
}

//
// End of "$Id: Fl_Color_Chooser.cxx,v 1.32 2002/12/09 04:52:24 spitzak Exp $".
//
