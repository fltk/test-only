#include <FL/Fl_Menu_Item.H>
#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/ValueSlider.h>
#include <fltk/draw.h>
#include <fltk/Choice.h>
using namespace fltk;

Window *form;
Slider *sliders[8];
Choice *choice[3];

class test_box: public Window {
  void draw();
public:
  test_box(int x,int y,int w,int h,const char *l=0)
    : Window(x,y,w,h,l) {}
} *test;

void test_box::draw() {
  Window::draw();
  setcolor(fltk::color((uchar)(sliders[0]->value()),
		       (uchar)(sliders[1]->value()),
		       (uchar)(sliders[2]->value())));
  char buf[5];
  buf[0] = char(sliders[4]->value());
  buf[1] = char(sliders[5]->value());
  buf[2] = char(sliders[6]->value());
  buf[3] = char(sliders[7]->value());
  buf[4] = 0;
  line_style(
    (int)(choice[0]->item()->user_data()) +
    (int)(choice[1]->item()->user_data()) +
    (int)(choice[2]->item()->user_data()),
    (int)(sliders[3]->value()),
    buf);
  strokerect(10,10,w()-20,h()-20);
  addvertex(35, 35);
  addvertex(w()-35, h()-35);
  addvertex(w()-40, 35);
  addvertex(35, h()/2);
  strokepath();
  // you must reset the line type when done:
  line_style(SOLID);
}

Fl_Menu_Item style_menu[] = {
  {"fltk::SOLID",	0, 0, (void*)SOLID},
  {"fltk::DASH",	0, 0, (void*)DASH},
  {"fltk::DOT",		0, 0, (void*)DOT},
  {"fltk:;DASHDOT",	0, 0, (void*)DASHDOT},
  {"fltk::DASHDOTDOT",	0, 0, (void*)DASHDOTDOT},
  {0}
};

Fl_Menu_Item cap_menu[] = {
  {"default",		0, 0, 0},
  {"fltk::CAP_FLAT",	0, 0, (void*)CAP_FLAT},
  {"fltk:;CAP_ROUND",	0, 0, (void*)CAP_ROUND},
  {"fltk::CAP_SQUARE",	0, 0, (void*)CAP_SQUARE},
  {0}
};

Fl_Menu_Item join_menu[] = {
  {"default",		0, 0, 0},
  {"fltk::JOIN_MITER",	0, 0, (void*)JOIN_MITER},
  {"fltk::JOIN_ROUND",	0, 0, (void*)JOIN_ROUND},
  {"fltk::JOIN_BEVEL",	0, 0, (void*)JOIN_BEVEL},
  {0}
};

void do_redraw(Widget*,void*)
{
    test->redraw();
}

void makeform(const char *) {
  form = new Window(500,210,"line_style() test");
  form->begin();
  sliders[0]= new ValueSlider(280,10,180,20,"R");
  sliders[0]->maximum(255);
  sliders[1]= new ValueSlider(280,30,180,20,"G");
  sliders[1]->maximum(255);
  sliders[2]= new ValueSlider(280,50,180,20,"B");
  sliders[2]->maximum(255);
  choice[0]= new Choice(280,70,180,20,"Style");
  choice[0]->menu(style_menu);
  choice[1]= new Choice(280,90,180,20,"Cap");
  choice[1]->menu(cap_menu);
  choice[2]= new Choice(280,110,180,20,"Join");
  choice[2]->menu(join_menu);
  sliders[3]= new ValueSlider(280,130,180,20,"Width");
  sliders[3]->maximum(20);
  sliders[4] = new Slider(200,170,70,20,"Dash");
  sliders[4]->align(ALIGN_TOP|ALIGN_LEFT);
  sliders[4]->maximum(40);
  sliders[5] = new Slider(270,170,70,20);
  sliders[5]->maximum(40);
  sliders[6] = new Slider(340,170,70,20);
  sliders[6]->maximum(40);
  sliders[7] = new Slider(410,170,70,20);
  sliders[7]->maximum(40);
  int i;
  for (i=0;i<8;i++) {
    sliders[i]->type(1);
    if (i<4) sliders[i]->align(ALIGN_LEFT);
    sliders[i]->callback((Callback*)do_redraw);
    sliders[i]->step(1);
  }
  for (i=0;i<3;i++) {
    choice[i]->value(0);
    choice[i]->callback((Callback*)do_redraw);
  }
  test=new test_box(10,10,190,190);
  form->resizable(test);
  form->end();
}

int main(int argc, char **argv) {
  makeform(argv[0]);
  form->show(argc,argv);
  return run();
}
