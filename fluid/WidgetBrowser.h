//
// "$Id$"
//
// WidgetBrowser header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
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

#ifndef FLUID3_WIDGET_BROWSER_H
#define FLUID3_WIDGET_BROWSER_H

#include <fltk3/Widget.h>
#include <fltk3/Browser.h>
#include <fltk3/run.h>
#include <fltk3/draw.h>


class Fl_Type;


fltk3::Widget *make_widget_browser(int x,int y,int w,int h);


namespace fluid3 {

  class WidgetBrowser : public fltk3::Browser_ 
  {
    friend class ::Fl_Type;
    
    static void handle_second_single_click_i(void*);
    
    Fl_Type* last_clicked;
    
    // required routines for fltk3::Browser_ subclass:
    void *item_first() const ;
    void *item_next(void *) const ;
    void *item_prev(void *) const ;
    int item_selected(void *) const ;
    void item_select(void *,int);
    int item_width(void *) const ;
    int item_height(void *) const ;
    void item_draw(void *,int,int,int,int) const ;
    int incr_height() const ;
    
  public:	
    
    void handle_second_single_click();
    int handle(int);
    void callback();
    WidgetBrowser(int,int,int,int,const char * =0);
  };
  
};


extern fluid3::WidgetBrowser *widget_browser;

void select(Fl_Type *o, int v);
void select_only(Fl_Type *o);
void deselect();
void redraw_browser();


#endif


//
// End of "$Id$".
//
