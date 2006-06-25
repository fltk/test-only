//
// "$Id: arc.cxx 5115 2006-05-12 16:00:00Z fabien $"
//
// Arc drawing test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2006 by Bill Spitzak and others.
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

#include <config.h>

#if USE_CAIRO

#include <fltk/run.h>
#include <fltk/x.h>
#include <fltk/DoubleBufferWindow.h>
#include <fltk/ValueSlider.h>
#include <fltk/draw.h>
#include <fltk/math.h>

using namespace fltk;

class CairoWindow : public Window {
public:
    CairoWindow(int w, int h) : Window(w,h),draw_cb_(0) {}

    void draw() {
	// prepare for cairo context
	cairo_surface_t * surface;
	cairo_t *cr, *old_cc;
	if (draw_cb_) {
	    old_cc = cc;
#if !USE_X11
	    surface = cairo_create_surface(this);
	    cr = ::cairo_create(surface);
	    cc= cr;
#endif
	}
	else {cr=old_cc=0;surface=0;}

	Group::draw();

	if (draw_cb_) {
	
	    draw_cb_(*this, surface, cr); // enjoy cairo features here !

	    // release cairo context
	    cc = old_cc;
#if !USE_X11
	    cairo_destroy(cr);
	    cairo_surface_destroy (surface);
#endif
	}
    }

    typedef void (*draw_cb) (CairoWindow& self, cairo_surface_t* surface,cairo_t* def);
    void set_draw_cb(draw_cb  cb){draw_cb_=cb;}
private:
    draw_cb draw_cb_;
};

// put your drawing stuff here
float drawargs[7] = {90, 90, 100, 100, 0, 360, 0};
const char* name[7] = {"X", "Y", "W", "H", "start", "end", "rotate"};

void my_cairo_draw_cb(CairoWindow& window, cairo_surface_t* surface,cairo_t* cr) {
	
    int w= window.w(), h = window.h(); 

    
#if 1
    fltk::push_clip(0,0, w, h);
    fltk::setcolor(fltk::BLACK);
    fltk::fillrect(0,0,w, h);
    fltk::push_matrix();
    //    if (drawargs[6]) {
      fltk::translate(w/2.0f, h/2.0f);
      fltk::rotate(drawargs[6]);
      fltk::translate(-w/2.0f, -h/2.0f);
      //}
    fltk::addarc(drawargs[0],drawargs[1],drawargs[2],drawargs[3],drawargs[4],drawargs[5]);
    fltk::closepath();
    fltk::addarc(120,120,40,40,0,-360);
    fltk::setcolor(fltk::GRAY33);
    fltk::fillstrokepath(fltk::WHITE);
    // draw a hardware circle to see how well rotations match:
    fltk::setcolor(fltk::GRAY33);
    fltk::addchord(fltk::Rectangle(20,20,(int)(drawargs[2]+1),(int)(drawargs[3]+1)),drawargs[4],drawargs[5]);
    fltk::fillstrokepath(fltk::WHITE);
    // now draw non-rotated hardware circle to check if it inscribes:
    fltk::pop_matrix();
    fltk::setcolor(fltk::GRAY40);
    fltk::fillrect(10,(int)(270-drawargs[3]),(int)drawargs[2],(int)drawargs[3]);
    fltk::setcolor(fltk::GRAY90);
    fltk::strokerect(10,(int)(270-drawargs[3]),(int) drawargs[2],(int) drawargs[3]);
    fltk::setcolor(fltk::GRAY10);
    fltk::addchord(fltk::Rectangle(10,(int) (270-drawargs[3]),(int) drawargs[2],(int)drawargs[3]),drawargs[4],drawargs[5]);
    fltk::fillstrokepath(fltk::GRAY90);
    fltk::pop_clip();
#else
    double xc = 0.5;
    double yc = 0.5;
    double radius = 0.4;
    double angle1 = 45.0  * (M_PI/180.0);  /* angles are specified */
    double angle2 = 180.0 * (M_PI/180.0);  /* in radians           */
    
    cairo_set_line_width (cr, 0.03);
    cairo_scale (cr, w,h);
    cairo_arc (cr, xc, yc, radius, angle1, angle2);
    cairo_stroke (cr);
    
    /* draw helping lines */
    cairo_set_source_rgba (cr, 1,0.2,0.2,0.6);
    cairo_arc (cr, xc, yc, 0.05, 0, 2*M_PI);
    cairo_fill (cr);
    cairo_arc (cr, xc, yc, radius, angle1, angle1);
    cairo_line_to (cr, xc, yc);
    cairo_arc (cr, xc, yc, radius, angle2, angle2);
    cairo_line_to (cr, xc, yc);
    cairo_stroke (cr);
#endif

}

int main(int argc, char** argv) {
    CairoWindow window(300,500);
    window.resizable(&window);
    window.color(fltk::WHITE);
    window.show(argc,argv);
    window.set_draw_cb(my_cairo_draw_cb);
    
    return fltk::run();
}
#else
#include <fltk/ask.h>
int main(int argc, char** argv) {
  fltk::message("please configure fltk with CAIRO enabled (--enable-cairo)"); 
  return 0;
}

#endif

//
// End of "$Id: arc.cxx 5115 2006-05-12 16:00:00Z fabien $".
//
