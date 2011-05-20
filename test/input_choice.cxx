//
// "$Id$"
//
// Test program for fltk3::Input_Choice
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

#include <stdio.h>
#include <fltk3/Button.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/InputChoice.h>

void buttcb(fltk3::Widget*,void*data) {
    fltk3::InputChoice *in=(fltk3::InputChoice *)data;
    static int flag = 1;
    flag ^= 1;
    if ( flag ) in->activate();
    else        in->deactivate();
    if (in->changed()) {
        printf("Callback: changed() is set\n");
        in->clear_changed();
    }
}

void input_choice_cb(fltk3::Widget*,void*data) {
    fltk3::InputChoice *in=(fltk3::InputChoice *)data;
    fprintf(stderr, "Value='%s'\n", (const char*)in->value());
}

int main(int argc, char **argv) {
    fltk3::DoubleWindow win(300, 200);

    fltk3::InputChoice in(40,40,100,28,"Test");
    in.callback(input_choice_cb, (void*)&in);
    in.add("one");
    in.add("two");
    in.add("three");
    in.value(1);

    fltk3::Button onoff(40,150,200,28,"Activate/Deactivate");
    onoff.callback(buttcb, (void*)&in);

    win.end();
    win.resizable(win);
    win.show(argc, argv);
    return fltk3::run();
}


//
// End of "$Id$".
//
