//
// "$Id: scroll.cxx,v 1.4.2.3.2.2.2.2 2003/11/07 03:47:25 easysw Exp $"
//
// Fl_Scroll test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//


// This file is modified to be printable to postscript (R.K.)

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <string.h>
#include <stdio.h>
#include <FL/fl_draw.H>
#include <FL/fl_math.h>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Ps.H>
#ifdef WIN32
#include <FL/Fl_Gdi.H>
#endif
#include <FL/Fl_Group.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Image.H>
#include "porsche.xpm"


class Drawing : public Fl_Widget {
  void draw();
public:
  Drawing(int X,int Y,int W,int H,const char* L) : Fl_Widget(X,Y,W,H,L) {
    align(FL_ALIGN_TOP);
    box(FL_FLAT_BOX);
    color(FL_WHITE);
  }
  
};


void print(Fl_Widget *, void *w) {

	Fl_PrintingGroup * g = (Fl_PrintingGroup *)w;
#ifdef WIN32
	Fl_Gdi *doc = new Fl_Gdi();
#else
	const char *f = fl_file_chooser("Save", "*.ps" , "*.ps");
	FILE *o = fl_fopen(f, "w");
	Fl_Ps * doc= new Fl_Ps(o,1,PS_PORTRAIT, PS_A4);
#endif
	doc->page(72,72,72,72, PS_LANDSCAPE, PS_ASK); //margins
	doc->fit(g->x(),g->y(),g->w(),g->h()); //fitting into margins
	g->print(doc);
	delete doc;
#ifndef WIN32
	 fclose(o);
#endif
	
};


void Drawing::draw() {
  draw_box();
  fl_push_matrix();
  fl_translate(x()+w()/2, y()+h()/2);
  fl_scale(w()/2, h()/2);
  fl_color(FL_BLACK);
  for (int i = 0; i < 20; i++) {
    for (int j = i+1; j < 20; j++) {
      fl_begin_line();
      fl_vertex(cos(M_PI*i/10+.1), sin(M_PI*i/10+.1));
      fl_vertex(cos(M_PI*j/10+.1), sin(M_PI*j/10+.1));
      fl_end_line();
    }
  }
  fl_pop_matrix();
}

Fl_Scroll* thescroll;

void box_cb(Fl_Widget* o, void*) {
  thescroll->box(((Fl_Button*)o)->value() ? FL_DOWN_FRAME : FL_NO_BOX);
  thescroll->redraw();
}

void type_cb(Fl_Widget*, void* v) {
  long x = (long)v;
  thescroll->type(x);
  thescroll->redraw();
}

Fl_Menu_Item choices[] = {
  {"0", 0, type_cb, (void*)0},
  {"HORIZONTAL", 0, type_cb, (void*)Fl_Scroll::HORIZONTAL},
  {"VERTICAL", 0, type_cb, (void*)Fl_Scroll::VERTICAL},
  {"BOTH", 0, type_cb, (void*)Fl_Scroll::BOTH},
  {"HORIZONTAL_ALWAYS", 0, type_cb, (void*)Fl_Scroll::HORIZONTAL_ALWAYS},
  {"VERTICAL_ALWAYS", 0, type_cb, (void*)Fl_Scroll::VERTICAL_ALWAYS},
  {"BOTH_ALWAYS", 0, type_cb, (void*)Fl_Scroll::BOTH_ALWAYS},
  {0}
};

void align_cb(Fl_Widget*, void* v) {
  thescroll->scrollbar.align(int(v));
  thescroll->redraw();
}

Fl_Menu_Item align_choices[] = {
  {"left+top", 0, align_cb, (void*)(FL_ALIGN_LEFT+FL_ALIGN_TOP)},
  {"left+bottom", 0, align_cb, (void*)(FL_ALIGN_LEFT+FL_ALIGN_BOTTOM)},
  {"right+top", 0, align_cb, (void*)(FL_ALIGN_RIGHT+FL_ALIGN_TOP)},
  {"right+bottom", 0, align_cb, (void*)(FL_ALIGN_RIGHT+FL_ALIGN_BOTTOM)},
  {0}
};

int main(int argc, char** argv) {
  Fl_Window window(5*75,400);
  window.box(FL_NO_BOX);

//encaplulation of the window

  Fl_PrintingGroup gr(0,0,5*75,window.h());
  Fl_Scroll scroll(0,0,5*75,300);

//encaplulation what is inside scrolling
  Fl_PrintingGroup dc(0,0,5*75,8*25+5*75+320);
  int n = 0;
  for (int y=0; y<16; y++) for (int x=0; x<5; x++) {
    char buf[20]; sprintf(buf,"%d",n++);
    Fl_Button* b = new Fl_Button(x*75,y*25+(y>=8?5*75:0),75,25,strdup(buf));
    b->color(n);
    b->labelcolor(FL_WHITE);
  }
  Drawing drawing(0,8*25,5*75,5*75,0);
  Fl_Button b(0, 10*75 + 25,5*75,120,"Pixmap"); 
  Fl_Pixmap *pixmap = new Fl_Pixmap(porsche_xpm); 
  
  b.image(pixmap);
  
  dc.end();
//end encapsulationof widgets
  
  
  
  scroll.end();
  window.resizable(scroll);

  Fl_Box box(0,300,5*75,window.h()-300); // gray area below the scroll
  box.box(FL_FLAT_BOX);

  Fl_Toggle_Button but1(150, 310, 200, 25, "box");
  but1.callback(box_cb);
  
  Fl_Choice choice(150, 335, 200, 25, "type():");
  choice.menu(choices);
  choice.value(3);

  Fl_Choice achoice(150, 360, 200, 25, "scrollbar.align():");
  achoice.menu(align_choices);
  achoice.value(3);

  Fl_Button *b1 = new Fl_Button(5,310, 90, 25, "Print window");
  b1->callback(print,&gr);

  Fl_Button *b2 = new Fl_Button(5,335, 90, 25, "Print \"doc\"");
  b2->callback(print,&dc);

  thescroll = &scroll;

  //scroll.box(FL_DOWN_BOX);
  //scroll.type(Fl_Scroll::VERTICAL);


  gr.end();
//end encapsulationof window

  window.end();
  window.show(argc,argv);
  return Fl::run();
}

//
// End of "$Id: scroll.cxx,v 1.4.2.3.2.2.2.2 2003/11/07 03:47:25 easysw Exp $".
//
