//
// "$Id: symbols.cxx,v 1.13 2004/05/15 20:52:47 spitzak Exp $"
//
// Symbol test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
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

#include <fltk/Window.h>
#include <fltk/Browser.h>
#include <fltk/Symbol.h>
#include <fltk/run.h>
#include <fltk/string.h>
#include <fltk/Slider.h>
#include <stdlib.h>

using namespace fltk;

int sort(const void* a, const void* b) {
  const Symbol* A = *(const Symbol**)a;
  const Symbol* B = *(const Symbol**)b;
  return strcasecmp(A->name(), B->name());
}

struct {const char* name; const char* text;} specialtable[] = {
  {"b", "@b;bold@n;\t@@b;"},
  {"B", "@B11;\t@n;@@B#;(bg color)"},
  {"C", "@C11;color\t@n;@@C#;"},
  {"f", "@f;fixed\t@n;@@f;"},
  {"i", "@i;italic\t@n;@@i;"},
  {"n", "@n;normal\t@@n; (turn off all fx)"},
  {"t", "@t;type\t@n;@@t; (screen font)"},
  {"s", "@s+4;big\t@n;@@s+#;"},
  {"S", "@S-4;small\t@n;@@s-#;"},
  {"x", "@x10;t@x+3;e@x-4;x@x+5;t\t@n@@x#; (displace x)"},
  {"y", "@y-1;t@y+1;e@y+3;x@y+5;t;\t@n@@y#; (displace y)"},
};

void slider_cb(Widget* w, void* v) {
  Browser* browser = (Browser*)v;
  browser->textsize(((Slider*)w)->value());
  // make it remeasure all the child items (not sure if this is a bug
  // in the Browser or not:
  for (int i = 0; i < browser->children(); i++)
    browser->child(i)->h(0);
  browser->relayout();
  browser->redraw();
}

int main(int argc, char** argv) {
  Window window(170,600);
  window.begin();
  const int sliderh = 25;
  Browser browser(0,0,window.w(),window.h()-sliderh);
  int widths[] = {48,0};
  browser.column_widths(widths);
  browser.color(GRAY85);
  window.resizable(browser);
  Slider slider(0, browser.h(), window.w(), sliderh);
  slider.type(Slider::TICK_ABOVE|Slider::LOG);
  slider.range(1,60);
  slider.step(1);
  slider.value(browser.textsize());
  slider.callback(slider_cb, &browser);
  window.end();
  const Symbol* table[1000];
  int n = 0;
  int i; for (i=0;;) {
    const Symbol* symbol = Symbol::iterate(i);
    if (!symbol) break;
    table[n++] = symbol;
  }
  qsort(table, n, sizeof(*table), sort);
  for (i=0; i<n; i++) {
    const Symbol* symbol = table[i];
    char buffer[256];
    for (unsigned j = 0; j < sizeof(specialtable)/sizeof(*specialtable); j++) {
      if (!strcmp(specialtable[j].name, symbol->name())) {
	browser.add(specialtable[j].text);
	goto CONTINUE;
      }
    }
    snprintf(buffer, 256, "@%s;\t@n@@%s;", symbol->name(), symbol->name());
    browser.add(buffer);
  CONTINUE:;
  }
  window.show(argc, argv);
  return fltk::run();
}
