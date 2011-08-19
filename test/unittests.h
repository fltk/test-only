//
// "$Id$"
//
// Unit tests for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

#ifndef UNITTESTS_H
#define UNITTESTS_H

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

extern class MainWindow *mainwin;
extern fltk3::HoldBrowser *browser;

// This class helps to automatically register a new test with the unittest app.
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

#endif

//
// End of "$Id$".
//
