// Cartesian.H,v 0.9
//
// Copyright 2000 by Roman Kantor.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License
// version 2 as published by the Free Software Foundation.
//

// This library is distributed  WITHOUT ANY WARRANTY;
// WITHOUT even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.


#include <math.h>
#include <fltk3/run.h>

#include <fltk3/OverlayWindow.h>
#include <fltk3/LightButton.h>
#include <fltk3/draw.h>
#include <fltk3/Clock.h>
#include "pixmaps/porsche.xpm"
#include <fltk3/Pixmap.h>
#include <fltk3/Bitmap.h>
#include <fltk3/RoundButton.h>


#include <fltk3/Printer.h>

//#include "fltk3::printer_chooser.H"

#include <fltk3/FileChooser.h>
#include <fltk3/draw.h>


#define sorceress_width 75
#define sorceress_height 75


// shameles copy from bitmap...
static uchar sorceress_bits[] = {
   0xfc, 0x7e, 0x40, 0x20, 0x90, 0x00, 0x07, 0x80, 0x23, 0x00, 0x00, 0xc6,
   0xc1, 0x41, 0x98, 0xb8, 0x01, 0x07, 0x66, 0x00, 0x15, 0x9f, 0x03, 0x47,
   0x8c, 0xc6, 0xdc, 0x7b, 0xcc, 0x00, 0xb0, 0x71, 0x0e, 0x4d, 0x06, 0x66,
   0x73, 0x8e, 0x8f, 0x01, 0x18, 0xc4, 0x39, 0x4b, 0x02, 0x23, 0x0c, 0x04,
   0x1e, 0x03, 0x0c, 0x08, 0xc7, 0xef, 0x08, 0x30, 0x06, 0x07, 0x1c, 0x02,
   0x06, 0x30, 0x18, 0xae, 0xc8, 0x98, 0x3f, 0x78, 0x20, 0x06, 0x02, 0x20,
   0x60, 0xa0, 0xc4, 0x1d, 0xc0, 0xff, 0x41, 0x04, 0xfa, 0x63, 0x80, 0xa1,
   0xa4, 0x3d, 0x00, 0x84, 0xbf, 0x04, 0x0f, 0x06, 0xfc, 0xa1, 0x34, 0x6b,
   0x01, 0x1c, 0xc9, 0x05, 0x06, 0xc7, 0x06, 0xbe, 0x11, 0x1e, 0x43, 0x30,
   0x91, 0x05, 0xc3, 0x61, 0x02, 0x30, 0x1b, 0x30, 0xcc, 0x20, 0x11, 0x00,
   0xc1, 0x3c, 0x03, 0x20, 0x0a, 0x00, 0xe8, 0x60, 0x21, 0x00, 0x61, 0x1b,
   0xc1, 0x63, 0x08, 0xf0, 0xc6, 0xc7, 0x21, 0x03, 0xf8, 0x08, 0xe1, 0xcf,
   0x0a, 0xfc, 0x4d, 0x99, 0x43, 0x07, 0x3c, 0x0c, 0xf1, 0x9f, 0x0b, 0xfc,
   0x5b, 0x81, 0x47, 0x02, 0x16, 0x04, 0x31, 0x1c, 0x0b, 0x1f, 0x17, 0x89,
   0x4d, 0x06, 0x1a, 0x04, 0x31, 0x38, 0x02, 0x07, 0x56, 0x89, 0x49, 0x04,
   0x0b, 0x04, 0xb1, 0x72, 0x82, 0xa1, 0x54, 0x9a, 0x49, 0x04, 0x1d, 0x66,
   0x50, 0xe7, 0xc2, 0xf0, 0x54, 0x9a, 0x58, 0x04, 0x0d, 0x62, 0xc1, 0x1f,
   0x44, 0xfc, 0x51, 0x90, 0x90, 0x04, 0x86, 0x63, 0xe0, 0x74, 0x04, 0xef,
   0x31, 0x1a, 0x91, 0x00, 0x02, 0xe2, 0xc1, 0xfd, 0x84, 0xf9, 0x30, 0x0a,
   0x91, 0x00, 0x82, 0xa9, 0xc0, 0xb9, 0x84, 0xf9, 0x31, 0x16, 0x81, 0x00,
   0x42, 0xa9, 0xdb, 0x7f, 0x0c, 0xff, 0x1c, 0x16, 0x11, 0x00, 0x02, 0x28,
   0x0b, 0x07, 0x08, 0x60, 0x1c, 0x02, 0x91, 0x00, 0x46, 0x29, 0x0e, 0x00,
   0x00, 0x00, 0x10, 0x16, 0x11, 0x02, 0x06, 0x29, 0x04, 0x00, 0x00, 0x00,
   0x10, 0x16, 0x91, 0x06, 0xa6, 0x2a, 0x04, 0x00, 0x00, 0x00, 0x18, 0x24,
   0x91, 0x04, 0x86, 0x2a, 0x04, 0x00, 0x00, 0x00, 0x18, 0x27, 0x93, 0x04,
   0x96, 0x4a, 0x04, 0x00, 0x00, 0x00, 0x04, 0x02, 0x91, 0x04, 0x86, 0x4a,
   0x0c, 0x00, 0x00, 0x00, 0x1e, 0x23, 0x93, 0x04, 0x56, 0x88, 0x08, 0x00,
   0x00, 0x00, 0x90, 0x21, 0x93, 0x04, 0x52, 0x0a, 0x09, 0x80, 0x01, 0x00,
   0xd0, 0x21, 0x95, 0x04, 0x57, 0x0a, 0x0f, 0x80, 0x27, 0x00, 0xd8, 0x20,
   0x9d, 0x04, 0x5d, 0x08, 0x1c, 0x80, 0x67, 0x00, 0xe4, 0x01, 0x85, 0x04,
   0x79, 0x8a, 0x3f, 0x00, 0x00, 0x00, 0xf4, 0x11, 0x85, 0x06, 0x39, 0x08,
   0x7d, 0x00, 0x00, 0x18, 0xb7, 0x10, 0x81, 0x03, 0x29, 0x12, 0xcb, 0x00,
   0x7e, 0x30, 0x28, 0x00, 0x85, 0x03, 0x29, 0x10, 0xbe, 0x81, 0xff, 0x27,
   0x0c, 0x10, 0x85, 0x03, 0x29, 0x32, 0xfa, 0xc1, 0xff, 0x27, 0x94, 0x11,
   0x85, 0x03, 0x28, 0x20, 0x6c, 0xe1, 0xff, 0x07, 0x0c, 0x01, 0x85, 0x01,
   0x28, 0x62, 0x5c, 0xe3, 0x8f, 0x03, 0x4e, 0x91, 0x80, 0x05, 0x39, 0x40,
   0xf4, 0xc2, 0xff, 0x00, 0x9f, 0x91, 0x84, 0x05, 0x31, 0xc6, 0xe8, 0x07,
   0x7f, 0x80, 0xcd, 0x00, 0xc4, 0x04, 0x31, 0x06, 0xc9, 0x0e, 0x00, 0xc0,
   0x48, 0x88, 0xe0, 0x04, 0x79, 0x04, 0xdb, 0x12, 0x00, 0x30, 0x0c, 0xc8,
   0xe4, 0x04, 0x6d, 0x06, 0xb6, 0x23, 0x00, 0x18, 0x1c, 0xc0, 0x84, 0x04,
   0x25, 0x0c, 0xff, 0xc2, 0x00, 0x4e, 0x06, 0xb0, 0x80, 0x04, 0x3f, 0x8a,
   0xb3, 0x83, 0xff, 0xc3, 0x03, 0x91, 0x84, 0x04, 0x2e, 0xd8, 0x0f, 0x3f,
   0x00, 0x00, 0x5f, 0x83, 0x84, 0x04, 0x2a, 0x70, 0xfd, 0x7f, 0x00, 0x00,
   0xc8, 0xc0, 0x84, 0x04, 0x4b, 0xe2, 0x2f, 0x01, 0x00, 0x08, 0x58, 0x60,
   0x80, 0x04, 0x5b, 0x82, 0xff, 0x01, 0x00, 0x08, 0xd0, 0xa0, 0x84, 0x04,
   0x72, 0x80, 0xe5, 0x00, 0x00, 0x08, 0xd2, 0x20, 0x44, 0x04, 0xca, 0x02,
   0xff, 0x00, 0x00, 0x08, 0xde, 0xa0, 0x44, 0x04, 0x82, 0x02, 0x6d, 0x00,
   0x00, 0x08, 0xf6, 0xb0, 0x40, 0x02, 0x82, 0x07, 0x3f, 0x00, 0x00, 0x08,
   0x44, 0x58, 0x44, 0x02, 0x93, 0x3f, 0x1f, 0x00, 0x00, 0x30, 0x88, 0x4f,
   0x44, 0x03, 0x83, 0x23, 0x3e, 0x00, 0x00, 0x00, 0x18, 0x60, 0xe0, 0x07,
   0xe3, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x70, 0x70, 0xe4, 0x07, 0xc7, 0x1b,
   0xfe, 0x01, 0x00, 0x00, 0xe0, 0x3c, 0xe4, 0x07, 0xc7, 0xe3, 0xfe, 0x1f,
   0x00, 0x00, 0xff, 0x1f, 0xfc, 0x07, 0xc7, 0x03, 0xf8, 0x33, 0x00, 0xc0,
   0xf0, 0x07, 0xff, 0x07, 0x87, 0x02, 0xfc, 0x43, 0x00, 0x60, 0xf0, 0xff,
   0xff, 0x07, 0x8f, 0x06, 0xbe, 0x87, 0x00, 0x30, 0xf8, 0xff, 0xff, 0x07,
   0x8f, 0x14, 0x9c, 0x8f, 0x00, 0x00, 0xfc, 0xff, 0xff, 0x07, 0x9f, 0x8d,
   0x8a, 0x0f, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x07, 0xbf, 0x0b, 0x80, 0x1f,
   0x00, 0x00, 0xff, 0xff, 0xff, 0x07, 0x7f, 0x3a, 0x80, 0x3f, 0x00, 0x80,
   0xff, 0xff, 0xff, 0x07, 0xff, 0x20, 0xc0, 0x3f, 0x00, 0x80, 0xff, 0xff,
   0xff, 0x07, 0xff, 0x01, 0xe0, 0x7f, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x07,
   0xff, 0x0f, 0xf8, 0xff, 0x40, 0xe0, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff,
   0xff, 0xff, 0x40, 0xf0, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0xff,
   0x41, 0xf0, 0xff, 0xff, 0xff, 0x07};

class MyWidget: public fltk3::Box{
protected:
  void draw(){
    fltk3::Box::draw();
    fltk3::color(fltk3::RED);
    fltk3::rectf(x()+5,y()+5,w()-10,h()-10);
    fltk3::push_clip(x()+6,y()+6,w()-12,h()-12);
    fltk3::color(fltk3::DARK_GREEN);
    fltk3::rectf(x()+5,y()+5,w()-10,h()-10);
    fltk3::pop_clip();
    fltk3::color(fltk3::YELLOW);
    fltk3::rectf(x()+7,y()+7,w()-14,h()-14);
    fltk3::color(fltk3::BLUE);

    fltk3::rect(x()+8,y()+8,w()-16,h()-16);
    fltk3::push_clip(x()+25,y()+25,w()-50, h()-50);
    fltk3::color(fltk3::BLACK);
    fltk3::rect(x()+24,y()+24,w()-48,h()-48);
    fltk3::line(x()+27,y()+27,x()+w()-27,y()+h()-27);
    fltk3::line(x()+27,y()+h()-27,x()+w()-27,y()+27);
    //fltk3::rect(x()+30,y()+30,w()-60,h()-60);
    fltk3::pop_clip();

  }
public:
    MyWidget(int x, int y):fltk3::Box(x,y,100,100, "Clipping and rect(f):\nYellow rect.framed\nby B-Y-G-R rect. 1 p.\nthick. Your printer may \nrender very thin lines\nsurrounding \"X\""){
      align(fltk3::ALIGN_TOP);
      labelsize(10);
    };
};


class MyWidget2: public fltk3::Box{
protected:
  void draw(){
    fltk3::Box::draw();
    int d;
//    fltk3::line_style(0);
    for(d=y()+5;d<48+y();d+=2){
      fltk3::xyline(x()+5,d,x()+48);
    }

    

   fltk3::push_clip(x()+52,y()+5,45,43);
   for(d=y()+5;d<150+y();d+=3){
     fltk3::line(x()+52,d,x()+92,d-40);
   }
   fltk3::pop_clip();







    
    fltk3::line_style(fltk3::DASH);
    fltk3::xyline(x()+5,y()+55,x()+48);
    fltk3::line_style(fltk3::DOT);
    fltk3::xyline(x()+5,y()+58,x()+48);
    fltk3::line_style(fltk3::DASHDOT);
    fltk3::xyline(x()+5,y()+61,x()+48);
    fltk3::line_style(fltk3::DASHDOTDOT);
    fltk3::xyline(x()+5,y()+64,x()+48);
    fltk3::line_style(0,0,(char*)"\7\3\7\2");
    fltk3::xyline(x()+5,y()+67,x()+48);





      
    fltk3::line_style(0);




    fltk3::line(x()+5,y()+72,x()+25,y()+95);
    fltk3::line(x()+8,y()+72,x()+28,y()+95,x()+31,y()+72);

    fltk3::color(fltk3::YELLOW);
    fltk3::polygon(x()+11, y()+72,x()+27,y()+91,x()+29,y()+72);
    fltk3::color(fltk3::RED);
    fltk3::loop(x()+11, y()+72,x()+27,y()+91,x()+29,y()+72);

	fltk3::color(fltk3::BLUE); ////
	fltk3::line_style(fltk3::SOLID, 6);
    fltk3::loop(x()+31, y()+12,x()+47,y()+31,x()+49,y()+12);
	fltk3::line_style(0);


    fltk3::color(200,0,200);
    fltk3::polygon(x()+35,y()+72,x()+33,y()+95,x()+48,y()+95,x()+43,y()+72);
    fltk3::color(fltk3::GREEN);
    fltk3::loop(x()+35,y()+72,x()+33,y()+95,x()+48,y()+95,x()+43,y()+72);

 

	fltk3::color(fltk3::BLUE);    
    fltk3::yxline(x()+65,y()+63,y()+66);
	fltk3::color(fltk3::GREEN);    
    fltk3::yxline(x()+66,y()+66,y()+63);

    fltk3::color(fltk3::BLUE);
    fltk3::rect(x()+80,y()+55,5,5);
    fltk3::color(fltk3::YELLOW);
    fltk3::rectf(x()+81,y()+56,3,3);
    fltk3::color(fltk3::BLACK);
    fltk3::point(x()+82,y()+57);

    fltk3::color(fltk3::BLUE);
    fltk3::rect(x()+56, y()+79, 24, 17);
    fltk3::color(fltk3::CYAN);
    fltk3::rectf(x()+57, y()+80, 22 , 15 );
    fltk3::color(fltk3::RED);
    fltk3::arc(x()+57, y()+80, 22 ,15 ,40, 270);
    fltk3::color(fltk3::YELLOW);
    fltk3::pie(x()+58, y()+81, 20 ,13 ,40, 270);




	fltk3::line_style(0);


	fltk3::color(fltk3::BLACK);
  fltk3::point(x()+58,y()+58);
	fltk3::color(fltk3::RED);
	fltk3::yxline(x()+59,y()+58,y()+59);
  fltk3::color(fltk3::GREEN);
	fltk3::yxline(x()+60,y()+59,y()+58);
  fltk3::color(fltk3::BLACK);
	fltk3::xyline(x()+61,y()+58,x()+62);
  fltk3::color(fltk3::RED);
	fltk3::xyline(x()+62,y()+59,x()+61);

  fltk3::color(fltk3::GREEN);
  fltk3::yxline(x()+57,y()+58,y()+59,x()+58);
  fltk3::color(fltk3::BLUE);
  fltk3::xyline(x()+58,y()+60,x()+56,y()+58);
  fltk3::color(fltk3::RED);
  fltk3::xyline(x()+58,y()+61,x()+56,y()+63);
  fltk3::color(fltk3::GREEN);
  fltk3::yxline(x()+57,y()+63,y()+62,x()+58);

  fltk3::color(fltk3::BLUE);
  fltk3::line(x()+58,y()+63, x()+60, y()+65);
  fltk3::color(fltk3::BLACK);
  fltk3::line(x()+61,y()+65, x()+59, y()+63);









    fltk3::color(fltk3::BLACK);
  };
  
public:
  MyWidget2(int x, int y):fltk3::Box(x,y,100,100, "Integer primitives"){
      labelsize(10);
      align(fltk3::ALIGN_TOP);
    };
};


class MyWidget3: public fltk3::Box{
protected:
  void draw(){
    fltk3::Box::draw();
    double d;
//    fltk3::line_style(0);
    fltk3::push_clip(x()+5,y()+5,45,43);
    for(d=y()+5;d<95+y();d+=1.63){
      fltk3::begin_line();
      fltk3::vertex(x()+5,d);
      fltk3::vertex(x()+48,d);
      fltk3::end_line();
    }
    fltk3::pop_clip();

    fltk3::push_clip(x()+52,y()+5,45,43);
    for(d=y()+5;d<150+y();d+=2.3052){
      fltk3::begin_line();
      fltk3::vertex(x()+52,d);
      fltk3::vertex(x()+92,d-43);
      fltk3::end_line();
    }
    fltk3::pop_clip();

  };
public:
  MyWidget3(int x, int y):fltk3::Box(x,y,100,100, "Sub-pixel drawing of\nlines 1.63 points apart\nOn the screen you\ncan see aliasing, the\nprinter should render\nthem properly"){
      labelsize(10);
      align(fltk3::ALIGN_TOP);
    };
};



class MyWidget4: public fltk3::Box{
protected:
  void draw(){
    fltk3::Box::draw();
    fltk3::push_matrix();
    fltk3::translate(x(),y());
    fltk3::scale(.75,.75);

    
    
    fltk3::line_style(fltk3::SOLID , 5);
    fltk3::begin_line();
    fltk3::vertex(10, 160);
    fltk3::vertex(40, 160);
    fltk3::vertex(40, 190);
    fltk3::end_line();
    fltk3::line_style(0);


    fltk3::color(fltk3::RED);
    fltk3::line_style(fltk3::SOLID | fltk3::CAP_FLAT |fltk3::JOIN_MITER , 5);
    fltk3::begin_line();
    fltk3::vertex(10, 150);
    fltk3::vertex(50, 150);
    fltk3::vertex(50, 190);
    fltk3::end_line();
    fltk3::line_style(0);

    fltk3::color(fltk3::GREEN);   
    fltk3::line_style(fltk3::SOLID | fltk3::CAP_ROUND |fltk3::JOIN_ROUND , 5);
    fltk3::begin_line();
    fltk3::vertex(10, 140);
    fltk3::vertex(60, 140);
    fltk3::vertex(60, 190);
    fltk3::end_line();
    fltk3::line_style(0);

    fltk3::color(fltk3::BLUE);
    fltk3::line_style(fltk3::SOLID | fltk3::CAP_SQUARE |fltk3::JOIN_BEVEL , 5);
    fltk3::begin_line();
    fltk3::vertex(10, 130);
    fltk3::vertex(70, 130);
    fltk3::vertex(70, 190);
    fltk3::end_line();
    fltk3::line_style(0);

    fltk3::color(fltk3::BLACK);
    fltk3::line_style(fltk3::DASH , 5);
    fltk3::begin_line();
    fltk3::vertex(10, 120);
    fltk3::vertex(80, 120);
    fltk3::vertex(80, 190);
    fltk3::end_line();
    fltk3::line_style(0);

    fltk3::color(fltk3::RED);
    fltk3::line_style(fltk3::DASH |fltk3::CAP_FLAT , 5);
    fltk3::begin_line();
    fltk3::vertex(10, 110);
    fltk3::vertex(90, 110);
    fltk3::vertex(90, 190);
    fltk3::end_line();
    fltk3::line_style(0);
 
    fltk3::color(fltk3::GREEN);
    fltk3::line_style(fltk3::DASH |fltk3::CAP_ROUND , 5);
    fltk3::begin_line();
    fltk3::vertex(10, 100);
    fltk3::vertex(100, 100);
    fltk3::vertex(100, 190);
    fltk3::end_line();
    fltk3::line_style(0);


    fltk3::color(fltk3::BLUE);
    fltk3::line_style(fltk3::DASH |fltk3::CAP_SQUARE , 5);
    fltk3::begin_line();
    fltk3::vertex(10, 90);
    fltk3::vertex(110, 90);
    fltk3::vertex(110, 190);
    fltk3::end_line();
    fltk3::line_style(0);

    fltk3::color(fltk3::BLACK);
    fltk3::line_style(fltk3::DOT, 5);
    fltk3::begin_line();
    fltk3::vertex(10, 80);
    fltk3::vertex(120, 80);
    fltk3::vertex(120, 190);
    fltk3::end_line();
    fltk3::line_style(0);

    fltk3::color(fltk3::RED);
    fltk3::line_style(fltk3::DOT | fltk3::CAP_FLAT, 5);
    fltk3::begin_line();
    fltk3::vertex(10, 70);
    fltk3::vertex(130, 70);
    fltk3::vertex(130, 190);
    fltk3::end_line();
    fltk3::line_style(0);

    fltk3::color(fltk3::GREEN);
    fltk3::line_style(fltk3::DOT | fltk3::CAP_ROUND, 5);
    fltk3::begin_line();
    fltk3::vertex(10, 60);
    fltk3::vertex(140, 60);
    fltk3::vertex(140, 190);
    fltk3::end_line();
    fltk3::line_style(0);

    fltk3::color(fltk3::BLUE);
    fltk3::line_style(fltk3::DOT | fltk3::CAP_SQUARE, 5);
    fltk3::begin_line();
    fltk3::vertex(10, 50);
    fltk3::vertex(150, 50);
    fltk3::vertex(150, 190);
    fltk3::end_line();
    fltk3::line_style(0);

    fltk3::color(fltk3::BLACK);
    fltk3::line_style(fltk3::DASHDOT |fltk3::CAP_ROUND |fltk3::JOIN_ROUND , 5);
    fltk3::begin_line();
    fltk3::vertex(10, 40);
    fltk3::vertex(160, 40);
    fltk3::vertex(160, 190);
    fltk3::end_line();
    fltk3::line_style(0);

    fltk3::color(fltk3::RED);
    fltk3::line_style(fltk3::DASHDOTDOT |fltk3::CAP_SQUARE |fltk3::JOIN_BEVEL , 5);
    fltk3::begin_line();
    fltk3::vertex(10, 30);
    fltk3::vertex(170, 30);
    fltk3::vertex(170, 190);
    fltk3::end_line();
    fltk3::line_style(0);


    fltk3::color(fltk3::GREEN);
    fltk3::line_style(fltk3::DASHDOTDOT |fltk3::CAP_ROUND |fltk3::JOIN_ROUND , 5);
    fltk3::begin_line();
    fltk3::vertex(10, 20);
    fltk3::vertex(180, 20);
    fltk3::vertex(180, 190);
    fltk3::end_line();
    fltk3::line_style(0);


    fltk3::color(fltk3::BLUE);
    fltk3::line_style(0, 5, (char*)"\12\3\4\2\2\1");
    fltk3::begin_line();
    fltk3::vertex(10, 10);
    fltk3::vertex(190, 10);
    fltk3::vertex(190, 190);

    fltk3::end_line();
    fltk3::line_style(0);
    fltk3::pop_matrix();    

    fltk3::color(fltk3::BLACK);



 
  };
public:
  MyWidget4(int x, int y):fltk3::Box(x,y,150,150, "Line styles"){
      labelsize(10);
      align(fltk3::ALIGN_TOP);
    };
};


class MyWidget5: public fltk3::Box{
protected:
  void draw(){
    fltk3::Box::draw();
    //fltk3::push_clip(x(),y(),w(),h());
    fltk3::push_matrix();
    
    
    fltk3::translate(x(),y());
    fltk3::push_matrix();
    fltk3::mult_matrix(1,3,0,1,0,-20);
    fltk3::color(fltk3::GREEN);
    fltk3::begin_polygon();
    fltk3::vertex(10,10);
    fltk3::vertex(100,-80);
    fltk3::vertex(100,-190);
    fltk3::end_polygon();

    fltk3::color(fltk3::RED);
    fltk3::line_style(fltk3::DASHDOT, 7);
    fltk3::begin_loop();


    fltk3::vertex(10,10);
    fltk3::vertex(100,-80);
    fltk3::vertex(100,-190);
    fltk3::end_loop();
    fltk3::line_style(0);

    fltk3::color(fltk3::BLUE);
    fltk3::line_style(fltk3::SOLID, 3);
    fltk3::begin_loop();
    fltk3::circle(60,-50,30);
    fltk3::end_loop();
    fltk3::line_style(0);
    
    fltk3::pop_matrix();
    fltk3::scale(1.8,1);

    fltk3::color(fltk3::YELLOW);
    fltk3::begin_polygon();
    fltk3::arc(30,90,20,-45,200);
    fltk3::end_polygon();

    fltk3::color(fltk3::BLACK);
    fltk3::line_style(fltk3::DASH, 3);
    fltk3::begin_line();
    fltk3::arc(30,90,20,-45,200);
    fltk3::end_line();
    fltk3::line_style(0);

    fltk3::translate(15,0);
    fltk3::scale(1.5,3);
    fltk3::begin_complex_polygon();
    fltk3::vertex(30,70);
    fltk3::arc(45,55,10,200,90);
    fltk3::arc(55,45,8,-170,20);
    fltk3::vertex(60,40);
    fltk3::vertex(30,20);
    fltk3::vertex(40,5);
    fltk3::vertex(60,25);
    //fltk3::vertex(50,50);
    fltk3::curve(35,30,30,53,0,35,65,65);
    fltk3::gap();
    fltk3::vertex(50,25);
    fltk3::vertex(40,10);
    fltk3::vertex(35,20);
    fltk3::end_complex_polygon();

    fltk3::pop_matrix();

//    fltk3::color(fltk3::BLACK);
//    fltk3::line_style(0);
    //fltk3::pop_clip();



  };
public:
  MyWidget5(int x, int y):fltk3::Box(x,y,230,250, "Complex (double) drawings:\nBlue ellipse may not be\ncorrectly transformed\ndue to non-orthogonal\ntransformation"){
      labelsize(10);
      align(fltk3::ALIGN_TOP);
    };
};


uchar *image;
int width = 80;
int height = 80;

void make_image() {
  image = new uchar[4*width*height];
  uchar *p = image;
  for (int y = 0; y < height; y++) {
    double Y = double(y)/(height-1);
    for (int x = 0; x < width; x++) {
      double X = double(x)/(width-1);
      *p++ = uchar(255*((1-X)*(1-Y))); // red in upper-left
      *p++ = uchar(255*((1-X)*Y));	// green in lower-left
      *p++ = uchar(255*(X*Y));	// blue in lower-right
      X -= 0.5;
      Y -= 0.5;
      int alpha = (int)(350 * sqrt(X * X + Y * Y));
      if (alpha < 255) *p++ = uchar(alpha);	// alpha transparency
      else *p++ = 255;
      Y += 0.5;
    }
  }
}


void print(fltk3::Widget *, void *w) {
    fltk3::Widget * g = (fltk3::Widget *)w;
 
  fltk3::Printer * p = new fltk3::Printer();
  if (!p->start_job(1)) {
    p->start_page();
    p->print_window(g->window());
    p->end_page();
    p->end_job();
  }
  delete p;
}

/*void print2(fltk3::Widget *, void *w) {
  fltk3::Widget * g = (fltk3::Widget *)w;
  fltk3::Printer * p = fltk3::printer_chooser();
  if(!p) return;
  p->page(fltk3::Printer::A4);
  // fitting inside margins 1 inch wide
  p->place(g, fltk3::INCH, fltk3::INCH, p->page_width() - 2 * fltk3::INCH, p->page_height() - 2 * fltk3::INCH,  fltk3::ALIGN_CENTER);
  fltk3::Device * c = p->set_current();
  fltk3::draw(g); 
  c->set_current();
  delete p;
};*/


class My_Button:public fltk3::Button{
protected:
  void draw(){
    // fltk3::Button::draw();
       if (type() == fltk3::HIDDEN_BUTTON) return;
      fltk3::Color col = value() ? selection_color() : color();
      draw_box(value() ? (down_box()?down_box():fltk3::down(box())) : box(), col);
     fltk3::color(fltk3::WHITE);
     fltk3::line_style(fltk3::SOLID,5);
     fltk3::line(x()+15,y()+10,x()+w()-15,y()+h()-23);
     fltk3::line(x()+w()-15,y()+10,x()+15,y()+h()-23);
     fltk3::line_style(0);
     draw_label();

  };
public:
  My_Button(int x, int y, int w, int h, const char * label = 0):fltk3::Button(x,y,w,h,label){}
};


int main(int argc, char ** argv) {

  //fltk3::scheme("plastic");



  fltk3::Window * w2 = new fltk3::Window(500,560,"Graphics test");


  fltk3::Group *c2 =new fltk3::Group(3, 43, 494, 514 );

  new MyWidget(10,140);
  new MyWidget2(110,80);
  new MyWidget3(220,140);
  new MyWidget4(330,70);
  new MyWidget5(140,270);

  make_image();
  fltk3::RGBImage *rgb = new fltk3::RGBImage(image, width, height, 4);
  My_Button b_rgb(10,245,100,100,"RGB with alpha");
  b_rgb.image(rgb);

  My_Button b_pixmap(10,345,100,100,"Pixmap");
  fltk3::Pixmap *pixmap = new fltk3::Pixmap(porsche_xpm);
  b_pixmap.image(pixmap);

  My_Button b_bitmap(10,445,100,100,"Bitmap");
b_bitmap.labelcolor(fltk3::GREEN);
  b_bitmap.image(new fltk3::Bitmap(sorceress_bits,sorceress_width,sorceress_height));

  new fltk3::Clock(360,230,120,120);
  fltk3::ReturnButton * ret = new fltk3::ReturnButton (360, 360, 120,30, "Return");
  ret->deactivate();
  fltk3::Button but1(360, 390, 30, 30, "@->|");
  but1.labelcolor(fltk3::DARK3);
  fltk3::Button but2(390, 390, 30, 30, "@UpArrow");
  but2.labelcolor(fltk3::DARK3);
  fltk3::Button but3(420, 390, 30, 30, "@DnArrow");
  but3.labelcolor(fltk3::DARK3);
  fltk3::Button but4(450, 390, 30, 30, "@+");
  but4.labelcolor(fltk3::DARK3);
  fltk3::Button but5(360, 425, 120, 30, "Hello, World");
  but5.labelfont(fltk3::BOLD|fltk3::ITALIC);
  but5.labeltype(fltk3::SHADOW_LABEL);
  but5.box(fltk3::ROUND_UP_BOX);
//  but5.selection_color(fltk3::WHITE);

  fltk3::Button but6(360, 460, 120, 30, "Plastic");
  but6.box(fltk3::PLASTIC_UP_BOX);

  //fltk3::Button but7(, 480, 120, 30, "Engraved box");
  //but7.box(fltk3::ENGRAVED_BOX);
  { fltk3::Group* o = new fltk3::Group(360, 495, 120, 40);
    o->box(fltk3::UP_BOX);
   { fltk3::Group* o = new fltk3::Group(365, 500, 110, 30);
      o->box(fltk3::THIN_UP_FRAME);
      { fltk3::RoundButton* o = new fltk3::RoundButton(365, 500, 40, 30, "rad");
        o->value(1);
      }
      { fltk3::CheckButton* o = new fltk3::CheckButton(410, 500, 60, 30, "check");
        o->value(1);

      }
      o->end();
    }
    o->end();
    o->deactivate();
  }
  fltk3::Box tx(120,492,230,50,"Background is not printed because\nencapsulating group, which we are\n printing, has not set the box type");
  tx.box(fltk3::SHADOW_BOX);
  tx.labelsize(12);

  tx.hide();

  c2->end();
  fltk3::Button *b4 = new fltk3::Button(10,5, 150, 25, "Print");
  b4->callback(print,c2);
  /*fltk3::Button *b5 = new fltk3::Button(165,5, 90, 25, "Print");
  b5->tooltip("This is a tooltip");
  b5->callback(print2,c2);*/

  w2->end();
  w2->show(argc, argv);
   

	fltk3::run();
	return 0;
}
