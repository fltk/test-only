//
// "$Id$"
//
// Unit tests for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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
// Fltk unit tests
// v0.1 - Greg combines Matthias + Ian's tests
// v0.2 - Ian's 02/12/09 fixes applied
// v0.3 - Fixes to circle desc, augmented extent tests, fixed indents, added show(argc,argv)
// v1.0 - Submit for svn
// v1.1 - Matthias seperated all tests into multiple source files for hopefully easier handling

#include <fltk3/run.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/HoldBrowser.h>
#include <fltk3/HelpView.h>
#include <fltk3/Group.h>
#include <fltk3/Box.h>
#include <fltk3/draw.h>		// fltk3::text_extents()

// WINDOW/WIDGET SIZES
#define MAINWIN_W	700				// main window w()
#define MAINWIN_H	400				// main window h()
#define BROWSER_X	10				// browser x()
#define BROWSER_Y	25				// browser y()
#define BROWSER_W	150				// browser w()
#define BROWSER_H	MAINWIN_H-35			// browser h()
#define TESTAREA_X	(BROWSER_W + 20)		// test area x()
#define TESTAREA_Y	25				// test area y()
#define TESTAREA_W	(MAINWIN_W - BROWSER_W - 30)	// test area w()
#define TESTAREA_H	BROWSER_H			// test area h()

typedef void (*UnitTestCallback)(const char*,fltk3::Group*);

class MainWindow *mainwin = 0;
fltk3::HoldBrowser *browser = 0;

// This class helps to automagically register a new test with the unittest app.
// Please see the examples on how this is used. 
class UnitTest {
public:
  UnitTest(const char *label, fltk3::Widget* (*create)()) :
    fWidget(0L)
  {
    fLabel = strdup(label);
    fCreate = create;
    add(this);
  }
  ~UnitTest() {
    delete fWidget;
    free(fLabel);
  }
  const char *label() {
    return fLabel;
  }
  void create() {
    fWidget = fCreate();
    if (fWidget) fWidget->hide();
  }
  void show() {
    if (fWidget) fWidget->show();
  }
  void hide() {
    if (fWidget) fWidget->hide();
  }
  static int numTest() { return nTest; }
  static UnitTest *test(int i) { return fTest[i]; }
private:
  char *fLabel;
  fltk3::Widget *(*fCreate)();
  fltk3::Widget *fWidget;

  static void add(UnitTest *t) {
    fTest[nTest] = t;
    nTest++;
  }
  static int nTest;
  static UnitTest *fTest[200];
};

int UnitTest::nTest = 0;
UnitTest *UnitTest::fTest[];


// The main window needs an additional drawing feature in order to support 
// the viewport alignment test.
class MainWindow : public fltk3::DoubleWindow {
public:
  MainWindow(int w, int h, const char *l=0L) :
    fltk3::DoubleWindow(w, h, l),
    fTestAlignment(0)
  { }
  // this code is used by the viewport alignment test
  void drawAlignmentIndicators() {
    const int sze = 16;
    // top left corner
    fltk3::color(fltk3::GREEN); fltk3::yxline(0, sze, 0, sze);
    fltk3::color(fltk3::RED);   fltk3::yxline(-1, sze, -1, sze);
    fltk3::color(fltk3::WHITE); fltk3::rectf(3, 3, sze-2, sze-2);
    fltk3::color(fltk3::BLACK); fltk3::rect(3, 3, sze-2, sze-2);
    // bottom left corner
    fltk3::color(fltk3::GREEN); fltk3::yxline(0, h()-sze-1, h()-1, sze);
    fltk3::color(fltk3::RED);   fltk3::yxline(-1, h()-sze-1, h(), sze);
    fltk3::color(fltk3::WHITE); fltk3::rectf(3, h()-sze-1, sze-2, sze-2);
    fltk3::color(fltk3::BLACK); fltk3::rect(3, h()-sze-1, sze-2, sze-2);
    // bottom right corner
    fltk3::color(fltk3::GREEN); fltk3::yxline(w()-1, h()-sze-1, h()-1, w()-sze-1);
    fltk3::color(fltk3::RED);   fltk3::yxline(w(), h()-sze-1, h(), w()-sze-1);
    fltk3::color(fltk3::WHITE); fltk3::rectf(w()-sze-1, h()-sze-1, sze-2, sze-2);
    fltk3::color(fltk3::BLACK); fltk3::rect(w()-sze-1, h()-sze-1, sze-2, sze-2);
    // top right corner
    fltk3::color(fltk3::GREEN); fltk3::yxline(w()-1, sze, 0, w()-sze-1);
    fltk3::color(fltk3::RED);   fltk3::yxline(w(), sze, -1, w()-sze-1);
    fltk3::color(fltk3::WHITE); fltk3::rectf(w()-sze-1, 3, sze-2, sze-2);
    fltk3::color(fltk3::BLACK); fltk3::rect(w()-sze-1, 3, sze-2, sze-2);
  }
  void draw() {
    fltk3::DoubleWindow::draw();
    if (fTestAlignment) {
      drawAlignmentIndicators();
    }
  }
  void testAlignment(int v) {
    fTestAlignment = v;
    redraw();
  }
  int fTestAlignment;
};

//------- include the various unit tests as inline code -------

#include "unittest_about.cxx"
#include "unittest_points.cxx"
#include "unittest_lines.cxx"
#include "unittest_rects.cxx"
#include "unittest_circles.cxx"
#include "unittest_text.cxx"
#include "unittest_images.cxx"
#include "unittest_viewport.cxx"
#include "unittest_scrollbarsize.cxx"


// callback whenever the browser value changes
void Browser_CB(fltk3::Widget*, void*) {
  for ( int t=1; t<=browser->size(); t++ ) {
    UnitTest *ti = (UnitTest*)browser->data(t);
    if ( browser->selected(t) ) {
      ti->show();
    } else {
      ti->hide();
    }
  }
}


// this is the main call. It creates the window and adds all previously
// registered tests to the browser widget.
int main(int argc, char **argv) {
  fltk3::args(argc,argv);
  fltk3::visual(fltk3::RGB);
  mainwin = new MainWindow(MAINWIN_W, MAINWIN_H, "Fltk Unit Tests");
  browser = new fltk3::HoldBrowser(BROWSER_X, BROWSER_Y, BROWSER_W, BROWSER_H, "Unit Tests");
  browser->align(fltk3::ALIGN_TOP|fltk3::ALIGN_LEFT);
  browser->when(fltk3::WHEN_CHANGED);
  browser->callback(Browser_CB);

  int i, n = UnitTest::numTest();
  for (i=0; i<n; i++) {
    UnitTest *t = UnitTest::test(i);
    mainwin->begin();
    t->create();
    mainwin->end();
    browser->add(t->label(), (void*)t);
  }

  /////
  mainwin->resizable(mainwin);
  mainwin->show(argc,argv);
  // Select first test in browser, and show that test.
  browser->select(1);
  Browser_CB(browser,0);
  return(fltk3::run());
}

//
// End of "$Id$".
//
