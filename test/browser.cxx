/
// "$Id$
/
// Browser test program for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2003 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

/
This is a test of how the browser draws lines
This is a second line
This is a third

That was a blank line above this

@r@_Right justif
@c@_Center justif
@_Left justif

@bBold tex
@iItalic tex
@b@iBold Itali
@fFixed widt
@f@bBold Fixe
@f@iItalic Fixe
@f@i@bBold Italic Fixe
@lLarg
@l@bLarge bol
@sSmal
@s@bSmall bol
@s@iSmall itali
@s@i@bSmall italic bol
@uunderscor
@C1RE
@C2Gree
@C4Blu

	You should try different browser types
	Fl_Browse
	Fl_Select_Browse
	Fl_Hold_Browse
	Fl_Multi_Browse
*

#include <FL/Fl.H
#include <FL/Fl_Select_Browser.H
#include <FL/Fl_Double_Window.H
#include <FL/Fl_Button.H
#include <FL/Fl_Int_Input.H
#include <FL/fl_ask.H
#include <stdio.h
#include <string.h
#include <errno.h
#include <stdlib.h

Fl_Select_Browser *browser
Fl_Button	*top
		*bottom
		*middle
		*visible
		*swap
Fl_Int_Input	*field

void b_cb(Fl_Widget* o, void*) 
  printf("callback, selection = %d, event_clicks = %d\n"
	 ((Fl_Browser*)o)->value(), Fl::event_clicks())


void show_cb(Fl_Widget *o, void *) 
  int line = atoi(field->value())

  if (!line) 
    fl_alert("Please enter a number in the text field\n
             "before clicking on the buttons.")
    return
  

  if (o == top
    browser->topline(line)
  else if (o == bottom
    browser->bottomline(line)
  else if (o == middle
    browser->middleline(line)
  els
    browser->make_visible(line)


void swap_cb(Fl_Widget *o, void *) 
  int a = -1, b = -1
  for ( int t=0; t<browser->size(); t++ ) {	// find two selected item
    if ( browser->selected(t) ) 
      if ( a < 0 
	{ a = t; 
      else
	{ b = t; break; 
    
  
  browser->swap(a, b);				// swap the


int main(int argc, char **argv) 
  int i
  if (!Fl::args(argc,argv,i)) Fl::fatal(Fl::help)
  const char* fname = (i < argc) ? argv[i] : "browser.cxx"
  Fl_Window window(400,400,fname)
  browser = new Fl_Select_Browser(0,0,400,350,0)
  browser->type(FL_MULTI_BROWSER)
  //browser->type(FL_HOLD_BROWSER)
  //browser->color(42)
  browser->callback(b_cb)
  // browser->scrollbar_right()
  //browser->has_scrollbar(Fl_Browser::BOTH_ALWAYS)
  if (!browser->load(fname)) 
#ifdef _MSC_VE
    // if 'browser' was started from the VisualC environment in Win32,
    // the current directory is set to the environment itself,
    // so we need to correct the browser file pat
    int done = 1
    if ( i == argc )
    
      fname = "../test/browser.cxx"
      done = browser->load(fname)
    
    if ( !done 
    
      printf("Can't load %s, %s\n", fname, strerror(errno))
      exit(1)
    
#els
    printf("Can't load %s, %s\n", fname, strerror(errno))
    exit(1)
#endi
  
  browser->position(0)

  field = new Fl_Int_Input(50, 350, 350, 25, "Line #:")
  field->callback(show_cb)

  top = new Fl_Button(0, 375, 80, 25, "Top")
  top->callback(show_cb)

  bottom = new Fl_Button(80, 375, 80, 25, "Bottom")
  bottom->callback(show_cb)

  middle = new Fl_Button(160, 375, 80, 25, "Middle")
  middle->callback(show_cb)

  visible = new Fl_Button(240, 375, 80, 25, "Make Vis.")
  visible->callback(show_cb)

  swap = new Fl_Button(320, 375, 80, 25, "Swap")
  swap->callback(swap_cb)
  swap->tooltip("Swaps two selected lines\n(Use CTRL-click to select two lines)")

  window.resizable(browser)
  window.show(argc,argv)
  return Fl::run()


/
// End of "$Id$"
/

