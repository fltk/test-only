//
// "$Id:$"
//
// Scalebar source file for FLTK.
//
// Copyright 2002-2012 by Matthias Melcher.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <fltk3/run.h>
#include <fltk3/Scalebar.h>
#include <fltk3/draw.h>
#include <fltk3/Box.h>
//#include <math.h>

static const double INITIALREPEAT = .5;
static const double REPEAT = .05;


fltk3::Scalebar::Scalebar( int x, int y, int w, int h, const char *l )
: fltk3::Scrollbar( x, y, w, h, l ) 
{
  pushed_ = 0;
  slider_size_i_ = 1;
  min_sash_size_ = 0;
}


int fltk3::Scalebar::value() {
  return int(fltk3::Slider::value());
}


int fltk3::Scalebar::value(int position, int size, int top, int total) {
  return scrollvalue(position, size, top, total);
}


int fltk3::Scalebar::slider_size_i() { 
  return slider_size_i_; 
}


void fltk3::Scalebar::drawSliderBg(int x, int y, int w, int h) {
  if (!(damage()&fltk3::DAMAGE_ALL)) { // not a complete redraw
    draw_box();
  }
  fltk3::Color black = active_r() ? fltk3::BLACK : fltk3::INACTIVE_COLOR;
  if (type() == fltk3::VERT_NICE_SLIDER) {
    draw_box(fltk3::THIN_DOWN_BOX, x+w/2-2, y, 4, h, black);
  } else if (type() == fltk3::HOR_NICE_SLIDER) {
    draw_box(fltk3::THIN_DOWN_BOX, x, y+h/2-2, w, 4, black);
  }
}


int fltk3::Scalebar::scrollvalue(int p, int n, int t, int l) {
  //	p = position, first line displayed
  //	n = window, number of lines displayed
  //	t = top, number of first line
  //	l = length, total number of lines
  step(1, 1);
  int minSash;
  if (horizontal()) {
    if ( w()<3*h() ) {
      minSash = 0;
    } else {
      minSash = 3 * h(); // minimum sash size in pixel
      int div = w()-2*h()-minSash;
      if (div) minSash = l*minSash/div; // convert to size in 'lines'
    }
  } else {
    if ( h()<3*w() ) {
      minSash = 0;
    } else {
      minSash = 3 * w(); // minimum sash size in pixel
      int div = h()-2*w()-minSash;
      if (div) minSash = l*minSash/div;
    }
  }
  min_sash_size_ = minSash;
  slider_size_i_ = n;
  n += minSash;
  l += minSash;
  if (p+n > t+l) l = p+n-t;
  slider_size(n >= l ? 1.0 : double(n)/double(l));
  bounds(t, l-n+t);
  return fltk3::Valuator::value(p);
}


void fltk3::Scalebar::drawSlider(int x, int y, int w, int h) {  
  double val;
  if (minimum() == maximum())
    val = 0.5;
  else {
    val = (value()-minimum())/(maximum()-minimum());
    if (val > 1.0) val = 1.0;
    else if (val < 0.0) val = 0.0;
  }
  
  int W = (horizontal() ? w : h);
  int X, S;
  if (type()==fltk3::HOR_FILL_SLIDER || type() == fltk3::VERT_FILL_SLIDER) {
    S = int(val*W+.5);
    if (minimum()>maximum()) {S = W-S; X = W-S;}
    else X = 0;
  } else {
    S = int(slider_size()*W+.5);
    int T = (horizontal() ? h : w)/2+1;
    if (type()==fltk3::VERT_NICE_SLIDER || type()==fltk3::HOR_NICE_SLIDER) T += 4;
    if (S < T) S = T;
    X = int(val*(W-S)+.5);
  }
  int xsl, ysl, wsl, hsl;
  if (horizontal()) {
    xsl = x+X;
    wsl = S;
    ysl = y;
    hsl = h;
  } else {
    ysl = y+X;
    hsl = S;
    xsl = x;
    wsl = w;
  }
  
  if (damage()&fltk3::DAMAGE_ALL) { // complete redraw
    drawSliderBg(x, y, w, h);
  } else { // partial redraw, clip off new position of slider
    if (X > 0) {
      if (horizontal()) fltk3::push_clip(x, ysl, X, hsl);
      else fltk3::push_clip(xsl, y, wsl, X);
      drawSliderBg(x, y, w, h);
      fltk3::pop_clip();
    }
    if (X+S < W) {
      if (horizontal()) fltk3::push_clip(xsl+wsl, ysl, x+w-xsl-wsl, hsl);
      else fltk3::push_clip(xsl, ysl+hsl, wsl, y+h-ysl-hsl);
      drawSliderBg(x, y, w, h);
      fltk3::pop_clip();
    }
  }
  
  fltk3::Box* box1 = slider();
  if (!box1) {box1 = box()->up(); if (!box1) box1 = fltk3::UP_BOX;}
  if (type() == fltk3::VERT_NICE_SLIDER) {
    draw_box(box1, xsl, ysl, wsl, hsl, fltk3::GRAY);
    int d = (hsl-4)/2;
    draw_box(fltk3::THIN_DOWN_BOX, xsl+2, ysl+d, wsl-4, hsl-2*d,selection_color());
  } else if (type() == fltk3::HOR_NICE_SLIDER) {
    draw_box(box1, xsl, ysl, wsl, hsl, fltk3::GRAY);
    int d = (wsl-4)/2;
    draw_box(fltk3::THIN_DOWN_BOX, xsl+d, ysl+2, wsl-2*d, hsl-4,selection_color());
  } else {
    if (wsl>0 && hsl>0) draw_box(box1, xsl, ysl, wsl, hsl, selection_color());
    if ( horizontal() ) {
      if ( hsl>6 && wsl>=hsl*2.5f ) {
	draw_box( pushed_==4 ? fltk3::down(box1) : box1, xsl+2, ysl+2, hsl-4, hsl-4, selection_color() );
	draw_box( pushed_==5 ? fltk3::down(box1) : box1, xsl+wsl-hsl+2, ysl+2, hsl-4, hsl-4, selection_color() );
      }
    } else {
      if ( wsl>6 && hsl>=wsl*2.5f ) {
	draw_box( pushed_==4 ? fltk3::down(box1) : box1, xsl+2, ysl+2, wsl-4, wsl-4, selection_color() );
	draw_box( pushed_==5 ? fltk3::down(box1) : box1, xsl+2, ysl+hsl-wsl+2, wsl-4, wsl-4, selection_color() );
      }
    }
  }  
  draw_label(xsl, ysl, wsl, hsl);
}

void fltk3::Scalebar::draw(){
  if (damage()&fltk3::DAMAGE_ALL) draw_box();
  int X =     fltk3::box_dx(box());
  int Y =     fltk3::box_dy(box());
  int W = w()-fltk3::box_dw(box());
  int H = h()-fltk3::box_dh(box());
  if (horizontal()) {
    if (W < 3*H) {fltk3::Slider::draw(X,Y,W,H); return;}
    drawSlider(X+H,Y,W-2*H,H);
    if (damage()&fltk3::DAMAGE_ALL) {
      // left arrow box
      draw_box((pushed_==1) ? fltk3::down(slider()) : slider(),
	X, Y, H, H, selection_color());
      // right arrow box
      draw_box((pushed_==2) ? fltk3::down(slider()) : slider(),
	X+W-H, Y, H, H, selection_color());
      // left and right arrows
      if (active_r())
	fltk3::color(labelcolor());
      else
	fltk3::color(fltk3::inactive(labelcolor()));
      int d1 = (H-4)/3; if (d1 < 1) d1 = 1;
      int x1 = X+(H-d1-1)/2;
      int y1 = Y+(H-2*d1-1)/2;
      fltk3::polygon(x1, y1+d1, x1+d1, y1+2*d1, x1+d1, y1);
      x1 += (W-H);
      fltk3::polygon(x1, y1, x1, y1+2*d1, x1+d1, y1+d1);
    }
  } else { // vertical
    if (H < 3*W) {fltk3::Slider::draw(X,Y,W,H); return;}
    drawSlider(X,Y+W,W,H-2*W);
    if (damage()&fltk3::DAMAGE_ALL) {
      draw_box((pushed_==1) ? fltk3::down(slider()) : slider(),
	X, Y, W, W, selection_color());
      draw_box((pushed_==2) ? fltk3::down(slider()) : slider(),
	X, Y+H-W, W, W, selection_color());
      if (active_r())
	fltk3::color(labelcolor());
      else
	fltk3::color(fltk3::inactive(labelcolor()));
      int d1 = (W-4)/3; if (d1 < 1) d1 = 1;
      int x1 = X+(W-2*d1-1)/2;
      int y1 = Y+(W-d1-1)/2;
      fltk3::polygon(x1, y1+d1, x1+2*d1, y1+d1, x1+d1, y1);
      y1 += H-W;
      fltk3::polygon(x1, y1, x1+d1, y1+d1, x1+2*d1, y1);
    }
  }
}

int fltk3::Scalebar::handle(int event){
  static int evDown, evVal, evSize;
  // we have to do a lot of stuff that will be done again in the Scrollbar handle
  if ( event==fltk3::PUSH ) {
    int X=0; int Y=0; int W=w(); int H=h();
    int sashWdt, sashPos, mouseDown, scaleWdt, wdt;
    if ( horizontal() ) {
      // check if we are in the arrow areas and set our marker for drawing if so
      evDown = fltk3::event_x();
      if (W >= 3*H) {
	X += H; W -= 2*H;
	if ( evDown<X )   { pushed_ = 1; goto handleEv; }
	if ( evDown>X+W ) { pushed_ = 2; goto handleEv; }
      }
      // check if the user clicked into the sash scaling area
      mouseDown = evDown-X;
      scaleWdt = H; wdt = W;
    } else {
      evDown = fltk3::event_y();
      if (H >= 3*W) {
	Y += W; H -= 2*W;
	if ( evDown<Y )   { pushed_ = 1; goto handleEv; }
	if ( evDown>Y+H ) { pushed_ = 2; goto handleEv; }
      }
      mouseDown = evDown-Y;
      scaleWdt = W; wdt = H;
    }
    sashWdt = (int)( slider_size()*wdt+0.5f );
    double val = (maximum()-minimum()) ? (value()-minimum())/(maximum()-minimum()) : 0.5;
    if ( val > 1.0 ) sashPos = wdt-sashWdt;
    else if ( val < 0.0 ) sashPos = 0;
    else sashPos = int(val*(wdt-sashWdt)+.5);
    if ( sashWdt<2.5f*scaleWdt) { // scalers are not visible, so don't use them!
      pushed_ = 3;
    } else if ( mouseDown>=sashPos && mouseDown<sashPos+scaleWdt-2 ) { 
      pushed_ = 4; evVal = value(); evSize = slider_size_i_; 
    } else if ( mouseDown>=sashPos+sashWdt-scaleWdt+2 && mouseDown<sashPos+sashWdt ) { 
      pushed_ = 5; evVal = value(); evSize = slider_size_i_; 
    } else {
      pushed_ = 3;
    }
  }
  
handleEv:
  if ( pushed_>3 ) {
    switch ( event ) {
    case fltk3::PUSH:
    case fltk3::DRAG: {
	int evDelta;
	int max = (int)maximum(), min = (int)minimum();
	int v = value(), s = slider_size_i_;
	if ( horizontal() ) {
	  evDelta = evDown-fltk3::event_x();
	  int nLines = (int)(maximum() - minimum() + slider_size_i_ + min_sash_size_);
	  int wdt = w()-2*h();
	  evDelta = evDelta*nLines/wdt;
	} else {
	  evDelta = evDown-fltk3::event_y();
	  int nLines = (int)(maximum() - minimum() + slider_size_i_ + min_sash_size_);
	  int wdt = h()-2*w();
	  evDelta = evDelta*nLines/wdt;
	}
	if ( pushed_ == 4 ) { // left or upper sash box
	  int maxx = max-min+slider_size_i_;
	  if ( evVal-evDelta < min ) evDelta = evVal-min;
	  if ( evDelta < -evSize ) evDelta = -evSize;
	  scrollvalue( evVal-evDelta, evSize+evDelta, min, maxx );
	} else if ( pushed_ == 5 ) { // right or lower sash box
	  int maxx = max-min+slider_size_i_;
	  if ( evDelta > evSize ) evDelta = evSize;
	  if ( evVal+evSize-evDelta-min > maxx ) evDelta = evVal+evSize-min-maxx;
	  scrollvalue( evVal, evSize-evDelta, min, maxx );
	}
	damage(fltk3::DAMAGE_ALL);
	if ( v != value() || s != slider_size_i_ ) {
	  do_callback();
	}
	return 1;
      }
    case fltk3::RELEASE:			
      damage(fltk3::DAMAGE_ALL);
      pushed_ = 0; 
      return 1; 
    }
  } else {
    if ( event == fltk3::RELEASE ) {
      pushed_ = 0;
      damage(fltk3::DAMAGE_ALL);
    }
  }
  return fltk3::Scrollbar::handle( event );
}

//
// End of "$Id:$".
//
