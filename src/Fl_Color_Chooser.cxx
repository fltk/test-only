//
// "$Id: Fl_Color_Chooser.cxx,v 1.28 2001/07/23 09:50:04 spitzak Exp $"
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

#include <fltk/Fl.h>
#include <fltk/Fl_Color_Chooser.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Item.h>
#include <fltk/math.h>
#include <stdio.h>

// Besides being a useful object on it's own, the Fl_Color_Chooser was
// an attempt to make a complex composite object that could be easily
// imbedded into a user interface.  If you wish to make complex objects
// of your own, be sure to read this code.

// The function fl_color_chooser() creates a window containing a color
// chooser and a few buttons and current-color indicators.  It is an
// easier interface for simple programs that just need a color.

// The "hue box" can be a circle or rectilinear.
// You get a circle by defining this:
// #define CIRCLE 1
// And the "hue box" can auto-update when the value changes
// you get this by defining this:
#define UPDATE_HUE_BOX 1

void Fl_Color_Chooser::hsv2rgb(
	double H, double S, double V, double& r, double& g, double& b) {
  if (S < 5.0e-6) {
    r = g = b = V;
  } else {
    int i = (int)H;  
    double f = H - (float)i;
    double p1 = V*(1.0-S);
    double p2 = V*(1.0-S*f);
    double p3 = V*(1.0-S*(1.0-f));
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

void Fl_Color_Chooser::rgb2hsv(
	double r, double g, double b, double& H, double& S, double& V) {
  double maxv = r > g ? r : g; if (b > maxv) maxv = b;
  V = maxv;
  if (maxv>0) {
    double minv = r < g ? r : g; if (b < minv) minv = b;
    S = 1.0 - double(minv)/maxv;
    if (maxv > minv) {
      if (maxv == r) {H = (g-b)/double(maxv-minv); if (H<0) H += 6.0;}
      else if (maxv == g) H = 2.0+(b-r)/double(maxv-minv);
      else H = 4.0+(r-g)/double(maxv-minv);
    }
  }
}

enum {M_RGB, M_BYTE, M_HEX, M_HSV}; // modes

int Flcc_Value_Input::format(char* buf) {
  Fl_Color_Chooser* c = (Fl_Color_Chooser*)parent()->parent();
  if (c->mode() == M_HEX) return sprintf(buf,"0x%02X", int(value()));
  else return Fl_Valuator::format(buf);
}

void Fl_Color_Chooser::set_valuators() {
  switch (mode()) {
  case M_RGB:
    rvalue.range(0,1); rvalue.step(.001); rvalue.value(r_);
    gvalue.range(0,1); gvalue.step(.001); gvalue.value(g_);
    bvalue.range(0,1); bvalue.step(.001); bvalue.value(b_);
    break;
  case M_BYTE:
  case M_HEX:
    rvalue.range(0,255); rvalue.step(1); rvalue.value(int(255*r_+.5));
    gvalue.range(0,255); gvalue.step(1); gvalue.value(int(255*g_+.5));
    bvalue.range(0,255); bvalue.step(1); bvalue.value(int(255*b_+.5));
    break;
  case M_HSV:
    rvalue.range(0,6); rvalue.step(.001); rvalue.value(hue_);
    gvalue.range(0,1); gvalue.step(.001); gvalue.value(saturation_);
    bvalue.range(0,1); bvalue.step(.001); bvalue.value(value_);
    break;
  }
}

int Fl_Color_Chooser::rgb(double r, double g, double b) {
  if (r == r_ && g == g_ && b == b_) return 0;
  r_ = r; g_ = g; b_ = b;
  double ph = hue_;
  double ps = saturation_;
  double pv = value_;
  rgb2hsv(r,g,b,hue_,saturation_,value_);
  set_valuators();
  if (value_ != pv) {
#ifdef UPDATE_HUE_BOX
    huebox.damage(FL_DAMAGE_SCROLL);
#endif
    valuebox.damage(FL_DAMAGE_EXPOSE);}
  if (hue_ != ph || saturation_ != ps) {
    huebox.damage(FL_DAMAGE_EXPOSE); 
    valuebox.damage(FL_DAMAGE_SCROLL);
  }
  return 1;
}

int Fl_Color_Chooser::hsv(double h, double s, double v) {
  h = fmod(h,6.0); if (h < 0.0) h += 6.0;
  if (s < 0.0) s = 0.0; else if (s > 1.0) s = 1.0;
  if (v < 0.0) v = 0.0; else if (v > 1.0) v = 1.0;
  if (h == hue_ && s == saturation_ && v == value_) return 0;
  double ph = hue_;
  double ps = saturation_;
  double pv = value_;
  hue_ = h; saturation_ = s; value_ = v;
  if (value_ != pv) {
#ifdef UPDATE_HUE_BOX
    huebox.damage(FL_DAMAGE_SCROLL);
#endif
    valuebox.damage(FL_DAMAGE_EXPOSE);}
  if (hue_ != ph || saturation_ != ps) {
    huebox.damage(FL_DAMAGE_EXPOSE); 
    valuebox.damage(FL_DAMAGE_SCROLL);
  }
  hsv2rgb(h,s,v,r_,g_,b_);
  set_valuators();
  return 1;
}

////////////////////////////////////////////////////////////////

static void tohs(double x, double y, double& h, double& s) {
#ifdef CIRCLE
  x = 2*x-1;
  y = 1-2*y;
  s = sqrt(x*x+y*y); if (s > 1.0) s = 1.0;
  h = (3.0/M_PI)*atan2(y,x);
  if (h<0) h += 6.0;
#else
  h = fmod(6.0*x,6.0); if (h < 0.0) h += 6.0;
  s = 1.0-y; if (s < 0.0) s = 0.0; else if (s > 1.0) s = 1.0;
#endif
}

int Flcc_HueBox::handle(int e) {
  static double is;
  Fl_Color_Chooser* c = (Fl_Color_Chooser*)parent();
  switch (e) {
  case FL_PUSH:
    is = c->s();
  case FL_DRAG: {
    double Xf, Yf, H, S;
    int ix = 0; int iy = 0; int iw = w(); int ih = h();
    text_box()->inset(ix, iy, iw, ih);
    Xf = (Fl::event_x()-ix)/double(iw);
    Yf = (Fl::event_y()-iy)/double(ih);
    tohs(Xf, Yf, H, S);
    if (fabs(H-ih) < 3*6.0/w()) H = ih;
    if (fabs(S-is) < 3*1.0/h()) S = is;
    if (Fl::event_state(FL_CTRL)) H = ih;
    if (c->hsv(H, S, c->v())) c->do_callback();
    } return 1;
  default:
    return 0;
  }
}

static void generate_image(void* vv, int X, int Y, int W, uchar* buf) {
  Flcc_HueBox* v = (Flcc_HueBox*)vv;
  int x1 = 0; int y1 = 0; int w1 = v->w(); int h1 = v->h();
  v->text_box()->inset(x1,y1,w1,h1);
  double Yf = double(Y)/h1;
#ifdef UPDATE_HUE_BOX
  const double V = ((Fl_Color_Chooser*)(v->parent()))->v();
#else
  const double V = 1.0;
#endif
  for (int x = X; x < X+W; x++) {
    double Xf = double(x)/w1;
    double H,S; tohs(Xf,Yf,H,S);
    double r,g,b;
    Fl_Color_Chooser::hsv2rgb(H,S,V,r,g,b);
    *buf++ = uchar(255*r+.5);
    *buf++ = uchar(255*g+.5);
    *buf++ = uchar(255*b+.5);
  }
}

void Flcc_HueBox::draw() {
  if (damage()&FL_DAMAGE_ALL) draw_text_frame();
  int x1 = 0; int y1 = 0; int w1 = w(); int h1 = h();
  text_box()->inset(x1,y1,w1,h1);
  if (damage() == FL_DAMAGE_EXPOSE) fl_push_clip(x1+px,y1+py,6,6);
  fl_draw_image(generate_image, this, x1, y1, w1, h1);
  if (damage() == FL_DAMAGE_EXPOSE) fl_pop_clip();
  Fl_Color_Chooser* c = (Fl_Color_Chooser*)parent();
#ifdef CIRCLE
  int X = int(.5*(cos(c->h()*(M_PI/3.0))*c->s()+1) * (w1-6));
  int Y = int(.5*(1-sin(c->h()*(M_PI/3.0))*c->s()) * (h1-6));
#else
  int X = int(c->h()/6.0*(w1-6));
  int Y = int((1-c->s())*(h1-6));
#endif
  if (X < 0) X = 0; else if (X > w1-6) X = w1-6;
  if (Y < 0) Y = 0; else if (Y > h1-6) Y = h1-6;
  //  fl_color(c->v()>.75 ? FL_BLACK : FL_WHITE);
  box()->draw(x1+X, y1+Y, 6, 6, color(), 0);
  px = X; py = Y;
}

////////////////////////////////////////////////////////////////

int Flcc_ValueBox::handle(int e) {
  static double iv;
  Fl_Color_Chooser* c = (Fl_Color_Chooser*)parent();
  switch (e) {
  case FL_PUSH:
    iv = c->v();
  case FL_DRAG: {
    double Yf;
    int x1 = 0; int y1 = 0; int w1 = w(); int h1 = h();
    text_box()->inset(x1,y1,w1,h1);
    Yf = 1-(Fl::event_y()-y1)/double(h1);
    if (fabs(Yf-iv)<(3*1.0/h())) Yf = iv;
    if (c->hsv(c->h(),c->s(),Yf)) c->do_callback();
    } return 1;
  default:
    return 0;
  }
}

static double tr, tg, tb;
static void generate_vimage(void* vv, int X, int Y, int W, uchar* buf) {
  Flcc_ValueBox* v = (Flcc_ValueBox*)vv;
  int x1 = 0; int y1 = 0; int w1 = v->w(); int h1 = v->h();
  v->text_box()->inset(x1,y1,w1,h1);
  double Yf = 255*(1.0-double(Y)/h1);
  uchar r = uchar(tr*Yf+.5);
  uchar g = uchar(tg*Yf+.5);
  uchar b = uchar(tb*Yf+.5);
  for (int x = X; x < X+W; x++) {
    *buf++ = r; *buf++ = g; *buf++ = b;
  }
}

void Flcc_ValueBox::draw() {
  if (damage()&FL_DAMAGE_ALL) draw_text_frame();
  Fl_Color_Chooser* c = (Fl_Color_Chooser*)parent();
  c->hsv2rgb(c->h(),c->s(),1.0,tr,tg,tb);
  int x1 = 0; int y1 = 0; int w1 = w(); int h1 = h();
  text_box()->inset(x1,y1,w1,h1);
  if (damage() == FL_DAMAGE_EXPOSE) fl_push_clip(x1,y1+py,w1,6);
  fl_draw_image(generate_vimage, this, x1, y1, w1, h1);
  if (damage() == FL_DAMAGE_EXPOSE) fl_pop_clip();
  int Y = int((1-c->v()) * (h1-6));
  if (Y < 0) Y = 0; else if (Y > h1-6) Y = h1-6;
  box()->draw(x1, y1+Y, w1, 6, color(), 0);
  py = Y;
}

////////////////////////////////////////////////////////////////

void Fl_Color_Chooser::rgb_cb(Fl_Widget* o, void*) {
  Fl_Color_Chooser* c = (Fl_Color_Chooser*)(o->parent()->parent());
  double r = c->rvalue.value();
  double g = c->gvalue.value();
  double b = c->bvalue.value();
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

void Fl_Color_Chooser::mode_cb(Fl_Widget* o, void*) {
  Fl_Color_Chooser* c = (Fl_Color_Chooser*)(o->parent()->parent());
  // force them to redraw even if value is the same:
  c->rvalue.value(-1);
  c->gvalue.value(-1);
  c->bvalue.value(-1);
  c->set_valuators();
}

////////////////////////////////////////////////////////////////

Fl_Color_Chooser::Fl_Color_Chooser(int X, int Y, int W, int H, const char* L)
  : Fl_Group(0,0,180,100,L),
    huebox(0,0,100,100),
    valuebox(100,0,20,100),
    nrgroup(120,0, 60, 100),
    choice(0,0,60,21),
    rvalue(0,22,60,21),
    gvalue(0,44,60,21),
    bvalue(0,66,60,21)
{
  nrgroup.end();
  choice.begin();
  new Fl_Item("rgb");
  new Fl_Item("byte");
  new Fl_Item("hex");
  new Fl_Item("hsv");
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

Fl_Color Fl_Color_Chooser::value() const {
  Fl_Color ret =
    fl_rgb(uchar(255*r()+.5), uchar(255*g()+.5), uchar(255*b()+.5));
  return ret ? ret : FL_BLACK;
}

void Fl_Color_Chooser::value(Fl_Color c) {
  c = fl_get_color(c);
  rgb(uchar(c>>24)/255.0, uchar(c>>16)/255.0, uchar(c>>8)/255.0);
}

////////////////////////////////////////////////////////////////
// fl_color_chooser():

#include <fltk/Fl_Window.h>
#include <fltk/Fl_Return_Button.h>

extern const char* fl_ok;
extern const char* fl_cancel;

static Fl_Window* window;
static Fl_Color_Chooser* chooser;
static Fl_Box* ok_color;
static Fl_Button* ok_button;
static Fl_Box* cancel_color;
static Fl_Button* cancel_button;

// this location is used to preserve index Fl_Color values:
static Fl_Color picked_color;

static void chooser_cb(Fl_Widget*, void*) {
  ok_color->color(picked_color = chooser->value());
  ok_color->redraw();
}

#define ROWS 4
#define COLS 16

FL_API Fl_Color fl_color_cells[ROWS*COLS] = {
// first the 16 assignable fltk color indexes:
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
// then the closest gray ramps to 0,1/4,1/3,1/2,2/3,3/4,1:
32,32,32,37,37,39,39,43,43,47,47,49,49,55,55,55,
// repeat it twice:
32,32,32,37,37,39,39,43,43,47,47,49,49,55,55,55};

class CellBox : public Fl_Widget {
public:
  CellBox(int X, int Y, int W, int H) : Fl_Widget(X,Y,W,H) {}
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
      FL_THIN_DOWN_BOX->draw(xx,yy,ww,hh,fl_color_cells[Y*COLS+X]);
    }
  }
}

int CellBox::handle(int e) {
  switch (e) {
  case FL_PUSH: return 1;
  case FL_DRAG: return 1;
  case FL_RELEASE: {
    int X = Fl::event_x()*COLS/w();
    if (X < 0 || X >= COLS) return 1;
    int Y = Fl::event_y()*ROWS/h();
    if (Y < 0 || Y >= ROWS) return 1;
    X = X+Y*COLS;
    if (Fl::event_button() > 1) {
      fl_color_cells[X] = picked_color;
      redraw();
    } else {
      chooser->value(picked_color = fl_color_cells[X]);
      ok_color->color(picked_color);
      ok_color->redraw();
    }
    return 1;}
  }
  return Fl_Widget::handle(e);
}

static void ok_cb(Fl_Widget* w, void*) {
  w->window()->set_value();
  w->window()->hide();
}

static void cancel_cb(Fl_Widget* w, void*) {
  w->window()->hide();
}

static void make_it() {
  if (window) return;
  window = new Fl_Window(210,212);
  chooser = new Fl_Color_Chooser(5, 5, 200, 100);
  chooser->callback(chooser_cb);
  new CellBox(5,110,200,52);
  ok_color = new Fl_Box(5, 165, 95, 21);
  ok_color->box(FL_ENGRAVED_BOX);
  ok_button = new Fl_Return_Button(5, 186, 95, 21, fl_ok);
  ok_button->callback(ok_cb);
  cancel_color = new Fl_Box(110, 165, 95, 21);
  cancel_color->box(FL_ENGRAVED_BOX);
  cancel_button = new Fl_Button(110, 186, 95, 21, fl_cancel);
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

int fl_color_chooser(const char* name, double& r, double& g, double& b) {
  make_it();
  chooser->rgb(r,g,b);
  if (!run_it(name)) return 0;
  r = chooser->r();
  g = chooser->g();
  b = chooser->b();
  return 1;
}

int fl_color_chooser(const char* name, uchar& r, uchar& g, uchar& b) {
  make_it();
  chooser->rgb(r/255.0, g/255.0, b/255.0);
  if (!run_it(name)) return 0;
  r = uchar(255*chooser->r()+.5);
  g = uchar(255*chooser->g()+.5);
  b = uchar(255*chooser->b()+.5);
  return 1;
}

int fl_color_chooser(const char* name, Fl_Color& c) {
  make_it();
  chooser->value(c);
  picked_color = c;
  if (!run_it(name)) return 0;
  c = picked_color;
  return 1;
}

//
// End of "$Id: Fl_Color_Chooser.cxx,v 1.28 2001/07/23 09:50:04 spitzak Exp $".
//
