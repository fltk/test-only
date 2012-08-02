//
// "$Id$"
//
// Widget panel actions code for the Fast Light Tool Kit (FLTK).
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

#include "widget_panel_actions.h"
#include "fluid.h"
#include "file.h"
#include "Fl_Function_Type.h"
#include "Fl_Group_Type.h"
#include "Fl_Menu_Type.h"
#include "Fl_Window_Type.h"
#include "WorkspaceType.h"

#include "Fluid_Image.h"
#include <fltk3/run.h>
#include <fltk3/utf8.h>
#include <fltk3/Group.h>
#include <fltk3/Table.h>
#include <fltk3/Input.h>
#include "alignment_panel.h"
#include <fltk3/message.h>
#include <fltk3/Slider.h>
#include <fltk3/Spinner.h>
#include <fltk3/Window.h>
#include "../src/fltk3/flstring.h"
#include <stdio.h>
#include <stdlib.h>
#include "widget_panel.h"
#include <fltk3/show_colormap.h>

extern int reading_file;
extern int gridx;
extern int gridy;
extern int i18n_type;
extern const char* i18n_include;
extern const char* i18n_function;
extern const char* i18n_file;
extern const char* i18n_set;
extern int varused_test, varused;

extern void redraw_browser();



Fl_Panel *the_widget_panel;
Fl_Panel *the_workspace_panel;



////////////////////////////////////////////////////////////////
// The control panels!



// All the callbacks use the argument to indicate whether to load or store.
// This avoids the need for pointers to all the widgets, and keeps the
// code localized in the callbacks.
// A value of Fl_Panel::LOAD means to load.  The hope is that this will not collide
// with any actual useful values for the argument.  I also use this to
// initialized parts of the widget that are nyi by fluid.

// FIXME: move to Fl_Panel
static int haderror;

void name_cb(fltk3::Input* o, void *v) {
  if (v == Fl_Panel::LOAD) {
    static char buf[1024];
    if (Fl_Panel::numselected != 1) {
      snprintf(buf, sizeof(buf), "Widget Properties (%d widgets)", Fl_Panel::numselected);
      o->hide();
    } else {
      o->static_value(Fl_Panel::selected_widget()->name());
      o->show();
      snprintf(buf, sizeof(buf), "%s Properties", Fl_Panel::selected_widget()->title());
    }
    the_widget_panel->label(buf);
  } else {
    if (Fl_Panel::numselected == 1) {
      Fl_Panel::selected_widget()->name(o->value());
      // I don't update window title, as it probably is being closed
      // and wm2 (a window manager) barfs if you retitle and then
      // hide a window:
      // ((fltk3::Window*)(o->parent()->parent()->parent()))->label(Fl_Panel::selected_widget()->title());
    }
  }
}

void name_public_member_cb(fltk3::Choice* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    i->value(Fl_Panel::selected_widget()->public_);
    if (Fl_Panel::selected_widget()->is_in_class()) i->show(); else i->hide();
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
        Fl_Widget_Type *w = ((Fl_Widget_Type*)o);
        if (w->is_in_class()) {
          w->public_ = i->value();
        } else {
          // if this is not in a class, it can be only private or public
          w->public_ = (i->value()>0);
        }
	mod = 1;
      }
    }
    if (mod) {
      set_modflag(1);
      redraw_browser();
    }
  }
}    

void name_public_cb(fltk3::Choice* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    i->value(Fl_Panel::selected_widget()->public_>0);
    if (Fl_Panel::selected_widget()->is_in_class()) i->hide(); else i->show();
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	((Fl_Widget_Type*)o)->public_ = i->value();
	mod = 1;
      }
    }
    if (mod) {
      set_modflag(1);
      redraw_browser();
    }
  }
}    

static char* oldlabel;
static unsigned oldlabellen;

void label_cb(fltk3::Input* i, void *v) {
  if (v == Fl_Panel::LOAD) {
    i->static_value(Fl_Panel::selected_widget()->label());
    if (strlen(i->value()) >= oldlabellen) {
      oldlabellen = strlen(i->value())+128;
      oldlabel = (char*)realloc(oldlabel,oldlabellen);
    }
    strcpy(oldlabel,i->value());
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
        o->label(i->value());
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

static fltk3::Input *image_input;

void image_cb(fltk3::Input* i, void *v) {
  if (v == Fl_Panel::LOAD) {
    image_input = i;
    if (Fl_Panel::selected_type()->is_widget() && !Fl_Panel::selected_type()->is_window()) {
      i->activate();
      i->static_value(((Fl_Widget_Type*)Fl_Panel::selected_widget())->image_name());
    } else i->deactivate();
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
        ((Fl_Widget_Type*)o)->image_name(i->value());
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void image_browse_cb(fltk3::Button* b, void *v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_widget() && !Fl_Panel::selected_type()->is_window())
      b->activate();
    else 
      b->deactivate();
  } else {
    int mod = 0;
    if (ui_find_image(image_input->value())) {
      image_input->value(ui_find_image_name);
      for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
	if (o->selected && o->is_widget()) {
	  ((Fl_Widget_Type*)o)->image_name(ui_find_image_name);
	  mod = 1;
	}
      }
      if (mod) set_modflag(1);
    }
  }
}

static fltk3::Input *inactive_input;

void inactive_cb(fltk3::Input* i, void *v) {
  if (v == Fl_Panel::LOAD) {
    inactive_input = i;
    if (Fl_Panel::selected_type()->is_widget() && !Fl_Panel::selected_type()->is_window()) {
      i->activate();
      i->static_value(((Fl_Widget_Type*)Fl_Panel::selected_widget())->inactive_name());
    } else i->deactivate();
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
        ((Fl_Widget_Type*)o)->inactive_name(i->value());
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void inactive_browse_cb(fltk3::Button* b, void *v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_widget() && !Fl_Panel::selected_type()->is_window()) 
      b->activate();
    else 
      b->deactivate();
  } else {
    int mod = 0;
    if (ui_find_image(inactive_input->value())) {
      inactive_input->value(ui_find_image_name);
      for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
	if (o->selected && o->is_widget()) {
	  ((Fl_Widget_Type*)o)->inactive_name(ui_find_image_name);
	  mod = 1;
	}
      }
      if (mod) set_modflag(1);
    }
  }
}

void tooltip_cb(fltk3::Input* i, void *v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_widget()) {
      i->activate();
      i->static_value(((Fl_Widget_Type*)Fl_Panel::selected_widget())->tooltip());
    } else i->deactivate();
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
        ((Fl_Widget_Type*)o)->tooltip(i->value());
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

fltk3::ValueInput *x_input, *y_input, *w_input, *h_input;

void x_cb(fltk3::ValueInput *i, void *v) {
  if (v == Fl_Panel::LOAD) {
    x_input = i;
    if (Fl_Panel::selected_type()->is_widget()) {
      i->value(((Fl_Widget_Type *)Fl_Panel::selected_widget())->o->x());
      x_input->activate();
    } else x_input->deactivate();
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
        fltk3::Widget *w = ((Fl_Widget_Type *)o)->o;
	w->resize((int)i->value(), w->y(), w->w(), w->h());
	if (w->window()) w->window()->redraw();
	if (o->is_window()) {
          ((fltk3::Window *)w)->size_range(gridx, gridy, fltk3::w(), fltk3::h(),
                                           gridx, gridy, 0);
	}
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void y_cb(fltk3::ValueInput *i, void *v) {
  if (v == Fl_Panel::LOAD) {
    y_input = i;
    if (Fl_Panel::selected_type()->is_widget()) {
      i->value(((Fl_Widget_Type *)Fl_Panel::selected_widget())->o->y());
      y_input->activate();
    } else y_input->deactivate();
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
        fltk3::Widget *w = ((Fl_Widget_Type *)o)->o;
	w->resize(w->x(), (int)i->value(), w->w(), w->h());
	if (w->window()) w->window()->redraw();
	if (o->is_window()) {
          ((fltk3::Window *)w)->size_range(gridx, gridy, fltk3::w(), fltk3::h(),
                                           gridx, gridy, 0);
	}
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void w_cb(fltk3::ValueInput *i, void *v) {
  if (v == Fl_Panel::LOAD) {
    w_input = i;
    if (Fl_Panel::selected_type()->is_widget()) {
      i->value(((Fl_Widget_Type *)Fl_Panel::selected_widget())->o->w());
      w_input->activate();
    } else w_input->deactivate();
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
        fltk3::Widget *w = ((Fl_Widget_Type *)o)->o;
	w->resize(w->x(), w->y(), (int)i->value(), w->h());
	if (w->window()) w->window()->redraw();
	if (o->is_window()) {
          ((fltk3::Window *)w)->size_range(gridx, gridy, fltk3::w(), fltk3::h(),
                                           gridx, gridy, 0);
	}
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void h_cb(fltk3::ValueInput *i, void *v) {
  if (v == Fl_Panel::LOAD) {
    h_input = i;
    if (Fl_Panel::selected_type()->is_widget()) {
      i->value(((Fl_Widget_Type *)Fl_Panel::selected_widget())->o->h());
      h_input->activate();
    } else h_input->deactivate();
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
        fltk3::Widget *w = ((Fl_Widget_Type *)o)->o;
	w->resize(w->x(), w->y(), w->w(), (int)i->value());
	if (w->window()) w->window()->redraw();
	if (o->is_window()) {
          ((fltk3::Window *)w)->size_range(gridx, gridy, fltk3::w(), fltk3::h(),
                                           gridx, gridy, 0);
	}
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void wc_relative_cb(fltk3::LightButton *i, void *v) {
  if (v == Fl_Panel::LOAD) {
    if (!strcmp(Fl_Panel::selected_widget()->type_name(), "widget_class")) {
      i->show();
      i->value(((Fl_Widget_Class_Type *)Fl_Panel::selected_widget())->wc_relative);
    } else {
      i->hide();
    }
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && !strcmp(Fl_Panel::selected_widget()->type_name(), "widget_class")) {
        Fl_Widget_Class_Type *t = (Fl_Widget_Class_Type *)o;
        t->wc_relative = i->value();
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

////////////////////////////////////////////////////////////////

// turn number to string or string to number for saving to file:
// does not work for hierarchial menus!

const char *item_name(fltk3::MenuItem* m, int i) {
  if (m) {
    while (m->label()) {
      if (m->argument() == i) return m->label();
      m++;
    }
  }
  static char buffer[20];
  sprintf(buffer, "%d", i);
  return buffer;
}
int item_number(fltk3::MenuItem* m, const char* i) {
  if (m && i) {
    if (i[0]=='F' && i[1]=='L' && i[2]=='_') i += 3;
    while (m->label()) {
      if (!strcmp(m->label(), i)) return int(m->argument());
      m++;
    }
  }
  return atoi(i);
}

extern fltk3::NoBox* const ZERO_BOX;
fltk3::NoBox zeroBox("FluidZeroBox");
fltk3::NoBox* const ZERO_BOX = &zeroBox;

fltk3::MenuItem boxmenu[] = {
  {"NO_BOX",0,0,(void *)ZERO_BOX},
  {"boxes",0,0,0,fltk3::SUBMENU},
  {"UP_BOX",0,0,(void *)fltk3::UP_BOX},
  {"DOWN_BOX",0,0,(void *)fltk3::DOWN_BOX},
  {"FLAT_BOX",0,0,(void *)fltk3::FLAT_BOX},
  {"BORDER_BOX",0,0,(void *)fltk3::BORDER_BOX},
  {"THIN_UP_BOX",0,0,(void *)fltk3::THIN_UP_BOX},
  {"THIN_DOWN_BOX",0,0,(void *)fltk3::THIN_DOWN_BOX},
  {"ENGRAVED_BOX",0,0,(void *)fltk3::ENGRAVED_BOX},
  {"EMBOSSED_BOX",0,0,(void *)fltk3::EMBOSSED_BOX},
  {"ROUND_UP_BOX",0,0,(void *)fltk3::ROUND_UP_BOX},
  {"ROUND_DOWN_BOX",0,0,(void *)fltk3::ROUND_DOWN_BOX},
  {"DIAMOND_UP_BOX",0,0,(void *)fltk3::DIAMOND_UP_BOX},
  {"DIAMOND_DOWN_BOX",0,0,(void *)fltk3::DIAMOND_DOWN_BOX},
  {"SHADOW_BOX",0,0,(void *)fltk3::SHADOW_BOX},
  {"ROUNDED_BOX",0,0,(void *)fltk3::ROUNDED_BOX},
  {"RSHADOW_BOX",0,0,(void *)fltk3::RSHADOW_BOX},
  {"RFLAT_BOX",0,0,(void *)fltk3::RFLAT_BOX},
  {"OVAL_BOX",0,0,(void *)fltk3::OVAL_BOX},
  {"OSHADOW_BOX",0,0,(void *)fltk3::OSHADOW_BOX},
  {"OFLAT_BOX",0,0,(void *)fltk3::OFLAT_BOX},
  {"PLASTIC_UP_BOX",0,0,(void *)fltk3::PLASTIC_UP_BOX},
  {"PLASTIC_DOWN_BOX",0,0,(void *)fltk3::PLASTIC_DOWN_BOX},
  {"PLASTIC_THIN_UP_BOX",0,0,(void *)fltk3::PLASTIC_THIN_UP_BOX},
  {"PLASTIC_THIN_DOWN_BOX",0,0,(void *)fltk3::PLASTIC_THIN_DOWN_BOX},
  {"PLASTIC_ROUND_UP_BOX",0,0,(void *)fltk3::PLASTIC_ROUND_UP_BOX},
  {"PLASTIC_ROUND_DOWN_BOX",0,0,(void *)fltk3::PLASTIC_ROUND_DOWN_BOX},
  {"CLASSIC_UP_BOX",0,0,(void *)fltk3::CLASSIC_UP_BOX},
  {"CLASSIC_DOWN_BOX",0,0,(void *)fltk3::CLASSIC_DOWN_BOX},
  {"CLASSIC_THIN_UP_BOX",0,0,(void *)fltk3::CLASSIC_THIN_UP_BOX},
  {"CLASSIC_THIN_DOWN_BOX",0,0,(void *)fltk3::CLASSIC_THIN_DOWN_BOX},
  {"CLASSIC_ROUND_UP_BOX",0,0,(void *)fltk3::CLASSIC_ROUND_UP_BOX},
  {"CLASSIC_ROUND_DOWN_BOX",0,0,(void *)fltk3::CLASSIC_ROUND_DOWN_BOX},
  {0},
  {"frames",0,0,0,fltk3::SUBMENU},
  {"UP_FRAME",0,0,(void *)fltk3::UP_FRAME},
  {"DOWN_FRAME",0,0,(void *)fltk3::DOWN_FRAME},
  {"THIN_UP_FRAME",0,0,(void *)fltk3::THIN_UP_FRAME},
  {"THIN_DOWN_FRAME",0,0,(void *)fltk3::THIN_DOWN_FRAME},
  {"ENGRAVED_FRAME",0,0,(void *)fltk3::ENGRAVED_FRAME},
  {"EMBOSSED_FRAME",0,0,(void *)fltk3::EMBOSSED_FRAME},
  {"BORDER_FRAME",0,0,(void *)fltk3::BORDER_FRAME},
  {"SHADOW_FRAME",0,0,(void *)fltk3::SHADOW_FRAME},
  {"ROUNDED_FRAME",0,0,(void *)fltk3::ROUNDED_FRAME},
  {"OVAL_FRAME",0,0,(void *)fltk3::OVAL_FRAME},
  {"PLASTIC_UP_FRAME",0,0,(void *)fltk3::PLASTIC_UP_FRAME},
  {"PLASTIC_DOWN_FRAME",0,0,(void *)fltk3::PLASTIC_DOWN_FRAME},
  {"CLASSIC_UP_FRAME",0,0,(void *)fltk3::CLASSIC_UP_FRAME},
  {"CLASSIC_DOWN_FRAME",0,0,(void *)fltk3::CLASSIC_DOWN_FRAME},
  {"CLASSIC_THIN_UP_FRAME",0,0,(void *)fltk3::CLASSIC_THIN_UP_FRAME},
  {"CLASSIC_THIN_DOWN_FRAME",0,0,(void *)fltk3::CLASSIC_THIN_DOWN_FRAME},
  {0},
  {0}};

const char *boxname(fltk3::Box *b) {
  if (b==0L) b = ZERO_BOX;
  for (int j = 0; j < int(sizeof(boxmenu)/sizeof(*boxmenu)); j++)
    if (boxmenu[j].user_data() == b) 
      return boxmenu[j].label();
  return 0;
}

fltk3::Box* boxptr(const char *i) {
  if (i[0]=='F' && i[1]=='L' && i[2]=='_') i += 3;
  if (strncmp(i, "GTK_", 4)==0) i += 4; // GTK is the defaut scheme in FLTK3
  for (int j = 0; j < int(sizeof(boxmenu)/sizeof(*boxmenu)); j++)
    if (boxmenu[j].label() && !strcmp(boxmenu[j].label(), i)) {
      return (fltk3::Box*)(boxmenu[j].user_data());
    }
  return 0;
}

void box_cb(fltk3::Choice* i, void *v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_menu_item()) {i->deactivate(); return;} else i->activate();
    fltk3::Box* n = Fl_Panel::selected_widget()->o->box(); if (!n) n = ZERO_BOX;
    for (int j = 0; j < int(sizeof(boxmenu)/sizeof(*boxmenu)); j++)
      if (boxmenu[j].user_data() == n) {i->value(j); break;}
  } else {
    int mod = 0;
    int m = i->value();
    fltk3::Box* n = (fltk3::Box*)boxmenu[m].user_data();
    if (!n) return; // should not happen
    if (n == ZERO_BOX) n = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
        q->o->box(n);
        q->redraw();
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void down_box_cb(fltk3::Choice* i, void *v) {
  if (v == Fl_Panel::LOAD) {
    fltk3::Box* n;
    if (Fl_Panel::selected_type()->is_button() && !Fl_Panel::selected_type()->is_menu_item())
      n = ((fltk3::Button*)(Fl_Panel::selected_widget()->o))->down_box();
    else if (!strcmp(Fl_Panel::selected_widget()->type_name(), "fltk3::InputChoice"))
      n = ((fltk3::InputChoice*)(Fl_Panel::selected_widget()->o))->down_box();
    else if (Fl_Panel::selected_type()->is_menu_button())
      n = ((fltk3::Menu_*)(Fl_Panel::selected_widget()->o))->down_box();
    else {
      i->deactivate(); return;
    }
    i->activate();
    if (!n) n = ZERO_BOX;
    for (int j = 0; j < int(sizeof(boxmenu)/sizeof(*boxmenu)); j++)
      if (boxmenu[j].user_data() == n) {i->value(j); break;}
  } else {
    int mod = 0;
    int m = i->value();
    fltk3::Box* n = (fltk3::Box*)boxmenu[m].user_data();
    if (!n) return; // should not happen
    if (n == ZERO_BOX) n = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected) {
	if (o->is_button() && !o->is_menu_item()) {
	  Fl_Widget_Type* q = (Fl_Widget_Type*)o;
          ((fltk3::Button*)(q->o))->down_box(n);
          if (((fltk3::Button*)(q->o))->value()) q->redraw();
	} else if (!strcmp(o->type_name(), "fltk3::InputChoice")) {
	  Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	  ((fltk3::InputChoice*)(q->o))->down_box(n);
	} else if (o->is_menu_button()) {
	  Fl_Widget_Type* q = (Fl_Widget_Type*)o;
          ((fltk3::Menu_*)(q->o))->down_box(n);
	}
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

////////////////////////////////////////////////////////////////

const int ZERO_ENTRY = 1000;

fltk3::MenuItem whenmenu[] = {
  {"Never",0,0,(void*)ZERO_ENTRY},
  {"Release",0,0,(void*)fltk3::WHEN_RELEASE},
  {"Changed",0,0,(void*)fltk3::WHEN_CHANGED},
  {"Enter key",0,0,(void*)fltk3::WHEN_ENTER_KEY},
  //{"Release or Enter",0,0,(void*)(fltk3::WHEN_ENTER_KEY|fltk3::WHEN_RELEASE)},
  {0}};

fltk3::MenuItem whensymbolmenu[] = {
  {"fltk3::WHEN_NEVER",0,0,(void*)(fltk3::WHEN_NEVER)},
  {"fltk3::WHEN_CHANGED",0,0,(void*)(fltk3::WHEN_CHANGED)},
  {"fltk3::WHEN_RELEASE",0,0,(void*)(fltk3::WHEN_RELEASE)},
  {"fltk3::WHEN_RELEASE_ALWAYS",0,0,(void*)(fltk3::WHEN_RELEASE_ALWAYS)},
  {"fltk3::WHEN_ENTER_KEY",0,0,(void*)(fltk3::WHEN_ENTER_KEY)},
  {"fltk3::WHEN_ENTER_KEY_ALWAYS",0,0,(void*)(fltk3::WHEN_ENTER_KEY_ALWAYS)},
  {0}};

void when_cb(fltk3::Choice* i, void *v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_menu_item()) {i->deactivate(); return;} else i->activate();
    int n = Fl_Panel::selected_widget()->o->when() & (~fltk3::WHEN_NOT_CHANGED);
    if (!n) n = ZERO_ENTRY;
    for (int j = 0; j < int(sizeof(whenmenu)/sizeof(*whenmenu)); j++)
      if (whenmenu[j].argument() == n) {i->value(j); break;}
  } else {
    int mod = 0;
    int m = i->value();
    int n = int(whenmenu[m].argument());
    if (!n) return; // should not happen
    if (n == ZERO_ENTRY) n = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	q->o->when(n|(q->o->when()&fltk3::WHEN_NOT_CHANGED));
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void when_button_cb(fltk3::LightButton* i, void *v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_widget()->is_menu_item()) {i->deactivate(); return;} else i->activate();
    i->value(Fl_Panel::selected_widget()->o->when()&fltk3::WHEN_NOT_CHANGED);
  } else {
    int mod = 0;
    int n = i->value() ? fltk3::WHEN_NOT_CHANGED : 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	q->o->when(n|(q->o->when()&~fltk3::WHEN_NOT_CHANGED));
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

const char* subclassname(Fl_Type* l) {
  if (l->is_widget()) {
    Fl_Widget_Type* p = (Fl_Widget_Type*)l;
    const char* c = p->subclass();
    if (c) return c;
    if (l->is_class()) return "fltk3::Group";
    if (p->o->type() == fltk3::WINDOW+1) return "fltk3::DoubleWindow";
    if (strcmp(p->type_name(), "fltk3::Input") == 0) {
      if (p->o->type() == fltk3::FLOAT_INPUT) return "fltk3::FloatInput";
      if (p->o->type() == fltk3::INT_INPUT) return "fltk3::IntInput";
    }
  }
  return l->type_name();
}

// the recursive part sorts all children, returns pointer to next:
Fl_Type *sort(Fl_Type *parent) {
  Fl_Type *f, *n=0;
  for (f = parent ? parent->next : Fl_Type::first; ; f = n) {
    if (!f || (parent && f->level <= parent->level)) return f;
    n = sort(f);
    if (!f->selected) continue;
    Fl_Type *g; // we will insert before this
                // sort widgets by position, so we get a decent Tab navigation
    if (f->is_widget() && !f->is_menu_item()) {
      fltk3::Widget* fw = ((Fl_Widget_Type*)f)->o;
      for (g = parent->next; g != f; g = g->next) {
        if (!g->selected || g->level > f->level) continue;
        fltk3::Widget* gw = ((Fl_Widget_Type*)g)->o;
        if (gw->y() > fw->y()) break;
        if (gw->y() == fw->y() && gw->x() > fw->x()) break;
      }
    } else if (f->is_tool()) {
      // sort tools by name
      Fl_Tool_Type* ft = ((Fl_Tool_Type*)f);
      for (g = parent->next; g != f; g = g->next) {
        if (!g->selected || g->level > f->level) continue;
        if (fltk3::event_alt()) {
          if (fltk3::strcasecmp(g->name(), ft->name())>0) break;
        } else {
          if (fltk3::strcmp(g->name(), ft->name())>0) break;
        }
      }
    }
    if (g != f) f->move_before(g);
  }
}

void resizable_cb(fltk3::LightButton* i,void* v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_menu_item()) {i->deactivate(); return;}
    if (Fl_Panel::numselected > 1) {i->deactivate(); return;}
    i->activate();
    i->value(Fl_Panel::selected_widget()->resizable());
  } else {
    Fl_Panel::selected_widget()->resizable(i->value());
    set_modflag(1);
  }
}

void hotspot_cb(fltk3::LightButton* i,void* v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::numselected > 1) {i->deactivate(); return;}
    if (Fl_Panel::selected_widget()->is_menu_item()) i->label("divider");
    else i->label("hotspot");
    i->activate();
    i->value(Fl_Panel::selected_widget()->hotspot());
  } else {
    Fl_Panel::selected_widget()->hotspot(i->value());
    if (Fl_Panel::selected_type()->is_menu_item()) {Fl_Panel::selected_widget()->redraw(); return;}
    if (i->value()) {
      Fl_Type *p = Fl_Panel::selected_widget()->parent;
      if (!p || !p->is_widget()) return;
      while (!p->is_window()) p = p->parent;
      for (Fl_Type *o = p->next; o && o->level > p->level; o = o->next) {
	if (o->is_widget() && o != Fl_Panel::selected_widget())
	  ((Fl_Widget_Type*)o)->hotspot(0);
      }
    }
    set_modflag(1);
  }
}

void visible_cb(fltk3::LightButton* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    i->value(Fl_Panel::selected_widget()->o->visible());
    if (Fl_Panel::selected_type()->is_window()) i->deactivate();
    else i->activate();
  } else {
    int mod = 0;
    int n = i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	n ? q->o->show() : q->o->hide();
	q->redraw();
	mod = 1;
	if (n && q->parent && q->parent->type_name()) {
	  if (!strcmp(q->parent->type_name(), "fltk3::TabGroup")) {
	    ((fltk3::TabGroup *)q->o->parent())->value(q->o);
	  } else if (!strcmp(q->parent->type_name(), "fltk3::WizardGroup")) {
	    ((fltk3::WizardGroup *)q->o->parent())->value(q->o);
	  }
	}
      }
    }
    if (mod) set_modflag(1);
  }
}

void active_cb(fltk3::LightButton* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    i->value(Fl_Panel::selected_widget()->o->active());
    if (Fl_Panel::selected_type()->is_window()) i->deactivate();
    else i->activate();
  } else {
    int mod = 0;
    int n = i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	n ? q->o->activate() : q->o->deactivate();
	q->redraw();
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

////////////////////////////////////////////////////////////////

fltk3::MenuItem fontmenu[] = {
  {"Helvetica"},
  {"Helvetica bold"},
  {"Helvetica italic"},
  {"Helvetica bold italic"},
  {"Courier"},
  {"Courier bold"},
  {"Courier italic"},
  {"Courier bold italic"},
  {"Times"},
  {"Times bold"},
  {"Times italic"},
  {"Times bold italic"},
  {"Symbol"},
  {"Terminal"},
  {"Terminal Bold"},
  {"Zapf Dingbats"},
  {0}};

void labelfont_cb(fltk3::Choice* i, void *v) {
  if (v == Fl_Panel::LOAD) {
    int n = Fl_Panel::selected_widget()->o->labelfont();
    if (n > 15) n = 0;
    i->value(n);
  } else {
    int mod = 0;
    int n = i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	q->o->labelfont(n);
	q->redraw();
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void labelsize_cb(fltk3::ValueInput* i, void *v) {
  int n;
  if (v == Fl_Panel::LOAD) {
    n = Fl_Panel::selected_widget()->o->labelsize();
  } else {
    int mod = 0;
    n = int(i->value());
    if (n <= 0) n = Fl_Widget_Type::default_size;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	q->o->labelsize(n);
	q->redraw();
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
  i->value(n);
}

extern const char *ui_find_image_name;

fltk3::MenuItem labeltypemenu[] = {
  {"NORMAL_LABEL",0,0,(void*)0},
  {"SHADOW_LABEL",0,0,(void*)fltk3::SHADOW_LABEL},
  {"ENGRAVED_LABEL",0,0,(void*)fltk3::ENGRAVED_LABEL},
  {"EMBOSSED_LABEL",0,0,(void*)fltk3::EMBOSSED_LABEL},
  {"NO_LABEL",0,0,(void*)(fltk3::NO_LABEL)},
  {0}};

void labeltype_cb(fltk3::Choice* i, void *v) {
  if (v == Fl_Panel::LOAD) {
    int n;
    n = Fl_Panel::selected_widget()->o->labeltype();
    i->when(fltk3::WHEN_RELEASE);
    for (int j = 0; j < int(sizeof(labeltypemenu)/sizeof(*labeltypemenu)); j++)
      if (labeltypemenu[j].argument() == n) {i->value(j); break;}
  } else {
    int mod = 0;
    int m = i->value();
    int n = int(labeltypemenu[m].argument());
    if (n<0) return; // should not happen
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* p = (Fl_Widget_Type*)o;
	p->o->labeltype((fltk3::Labeltype)n);
	p->redraw();
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

////////////////////////////////////////////////////////////////

void color_cb(fltk3::Button* i, void *v) {
  fltk3::Color c = Fl_Panel::selected_widget()->o->color();
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_menu_item()) {i->deactivate(); return;} else i->activate();
  } else {
    int mod = 0;
    fltk3::Color d = fltk3::show_colormap(c);
    if (d == c) return;
    c = d;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	q->o->color(c); q->o->redraw();
        if (q->parent && q->parent->type_name() == tabs_type_name) {
          if (q->o->parent()) q->o->parent()->redraw();
        }
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
  i->color(c); i->labelcolor(fltk3::contrast(fltk3::BLACK,c)); i->redraw();
}

void color2_cb(fltk3::Button* i, void *v) {
  fltk3::Color c = Fl_Panel::selected_widget()->o->selection_color();
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_menu_item()) {i->deactivate(); return;} else i->activate();
  } else {
    int mod = 0;
    fltk3::Color d = fltk3::show_colormap(c);
    if (d == c) return;
    c = d;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	q->o->selection_color(c); q->o->redraw();
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
  i->color(c); i->labelcolor(fltk3::contrast(fltk3::BLACK,c)); i->redraw();
}

void labelcolor_cb(fltk3::Button* i, void *v) {
  fltk3::Color c = Fl_Panel::selected_widget()->o->labelcolor();
  if (v != Fl_Panel::LOAD) {
    int mod = 0;
    fltk3::Color d = fltk3::show_colormap(c);
    if (d == c) return;
    c = d;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	q->o->labelcolor(c); q->redraw();
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
  i->color(c); i->labelcolor(fltk3::contrast(fltk3::BLACK,c)); i->redraw();
}

static fltk3::Button* relative(fltk3::Widget* o, int i) {
  fltk3::Group* g = (fltk3::Group*)(o->parent());
  return (fltk3::Button*)(g->child(g->find(*o)+i));
}

fltk3::MenuItem alignmenu[] = {
  {"fltk3::ALIGN_CENTER",0,0,(void*)(fltk3::ALIGN_CENTER)},
  {"fltk3::ALIGN_TOP",0,0,(void*)(fltk3::ALIGN_TOP)},
  {"fltk3::ALIGN_BOTTOM",0,0,(void*)(fltk3::ALIGN_BOTTOM)},
  {"fltk3::ALIGN_LEFT",0,0,(void*)(fltk3::ALIGN_LEFT)},
  {"fltk3::ALIGN_RIGHT",0,0,(void*)(fltk3::ALIGN_RIGHT)},
  {"fltk3::ALIGN_INSIDE",0,0,(void*)(fltk3::ALIGN_INSIDE)},
  {"fltk3::ALIGN_CLIP",0,0,(void*)(fltk3::ALIGN_CLIP)},
  {"fltk3::ALIGN_WRAP",0,0,(void*)(fltk3::ALIGN_WRAP)},
  {"fltk3::ALIGN_TEXT_OVER_IMAGE",0,0,(void*)(fltk3::ALIGN_TEXT_OVER_IMAGE)},
  {"fltk3::ALIGN_TOP_LEFT",0,0,(void*)(fltk3::ALIGN_TOP_LEFT)},
  {"fltk3::ALIGN_TOP_RIGHT",0,0,(void*)(fltk3::ALIGN_TOP_RIGHT)},
  {"fltk3::ALIGN_BOTTOM_LEFT",0,0,(void*)(fltk3::ALIGN_BOTTOM_LEFT)},
  {"fltk3::ALIGN_BOTTOM_RIGHT",0,0,(void*)(fltk3::ALIGN_BOTTOM_RIGHT)},
  {"fltk3::ALIGN_LEFT_TOP",0,0,(void*)(fltk3::ALIGN_LEFT_TOP)},
  {"fltk3::ALIGN_RIGHT_TOP",0,0,(void*)(fltk3::ALIGN_RIGHT_TOP)},
  {"fltk3::ALIGN_LEFT_BOTTOM",0,0,(void*)(fltk3::ALIGN_LEFT_BOTTOM)},
  {"fltk3::ALIGN_RIGHT_BOTTOM",0,0,(void*)(fltk3::ALIGN_RIGHT_BOTTOM)},
  {0}};

void align_cb(fltk3::Button* i, void *v) {
  fltk3::Align b = fltk3::Align(fl_uintptr_t(i->user_data()));
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_menu_item()) {i->deactivate(); return;} else i->activate();
    i->value(Fl_Panel::selected_widget()->o->align() & b);
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	fltk3::Align x = q->o->align();
	fltk3::Align y;
	if (i->value()) {
	  y = x | b;
	  if (b == fltk3::ALIGN_LEFT || b == fltk3::ALIGN_TOP) {
	    fltk3::Button *b1 = relative(i,+1);
	    b1->clear();
	    y = y & ~(b1->argument());
	  }
	  if (b == fltk3::ALIGN_RIGHT || b == fltk3::ALIGN_BOTTOM) {
	    fltk3::Button *b1 = relative(i,-1);
	    b1->clear();
	    y = y & ~(b1->argument());
	  }
	} else {
	  y = x & ~b;
	}
	if (x != y) {
          q->o->align(y);
	  q->redraw();
	  mod = 1;
	}
      }
    }
    if (mod) set_modflag(1);
  }
}

void align_position_cb(fltk3::Choice *i, void *v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_menu_item()) {i->deactivate(); return;} else i->activate();
    fltk3::MenuItem *mi = (fltk3::MenuItem*)i->menu();
    fltk3::Align b = Fl_Panel::selected_widget()->o->align() & fltk3::ALIGN_POSITION_MASK;
    for (;mi->text;mi++) {
      if ((fltk3::Align)(mi->argument())==b)
        i->value(mi);
    }
  } else {
    const fltk3::MenuItem *mi = i->menu() + i->value();
    fltk3::Align b = fltk3::Align(fl_uintptr_t(mi->user_data()));
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	fltk3::Align x = q->o->align();
	fltk3::Align y = (x & ~fltk3::ALIGN_POSITION_MASK) | b;
	if (x != y) {
          q->o->align(y);
	  q->redraw();
	  mod = 1;
	}
      }
    }
    if (mod) set_modflag(1);
  }
}

void align_text_image_cb(fltk3::Choice *i, void *v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_menu_item()) {i->deactivate(); return;} else i->activate();
    fltk3::MenuItem *mi = (fltk3::MenuItem*)i->menu();
    fltk3::Align b = Fl_Panel::selected_widget()->o->align() & fltk3::ALIGN_IMAGE_MASK;
    for (;mi->text;mi++) {
      if ((fltk3::Align)(mi->argument())==b)
        i->value(mi);
    }
  } else {
    const fltk3::MenuItem *mi = i->menu() + i->value();
    fltk3::Align b = fltk3::Align(fl_uintptr_t(mi->user_data()));
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	fltk3::Align x = q->o->align();
	fltk3::Align y = (x & ~fltk3::ALIGN_IMAGE_MASK) | b;
	if (x != y) {
          q->o->align(y);
	  q->redraw();
	  mod = 1;
	}
      }
    }
    if (mod) set_modflag(1);
  }
}

////////////////////////////////////////////////////////////////

void callback_cb(CodeEditor* i, void *v) {
  if (v == Fl_Panel::LOAD) {
    const char *cbtext = Fl_Panel::selected_widget()->callback();
    i->buffer()->text( cbtext ? cbtext : "" );
  } else {
    int mod = 0;
    char *c = i->buffer()->text();
    const char *d = c_check(c);
    if (d) {
      fltk3::message("Error in callback: %s",d);
      if (i->window()) i->window()->make_current();
      haderror = 1;
    }
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected) {
        o->callback(c);
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
    free(c);
  }
}

void user_data_cb(fltk3::Input *i, void *v) {
  if (v == Fl_Panel::LOAD) {
    i->static_value(Fl_Panel::selected_widget()->user_data());
  } else {
    int mod = 0;
    const char *c = i->value();
    const char *d = c_check(c);
    if (d) {fltk3::message("Error in user_data: %s",d); haderror = 1; return;}
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected) {
        o->user_data(c);
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void user_data_type_cb(fltk3::Input *i, void *v) {
  static const char *dflt = "void*";
  if (v == Fl_Panel::LOAD) {
    const char *c = Fl_Panel::selected_widget()->user_data_type();
    if (!c) c = dflt;
    i->static_value(c);
  } else {
    int mod = 0;
    const char *c = i->value();
    const char *d = c_check(c);
    if (!*c) i->value(dflt);
    else if (!strcmp(c,dflt)) c = 0;
    if (!d) {
      if (c && *c && c[strlen(c)-1] != '*' && strcmp(c,"long"))
	d = "must be pointer or long";
    }
    if (d) {fltk3::message("Error in type: %s",d); haderror = 1; return;}
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected) {
        o->user_data_type(c);
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

// "v_attributes" let user type in random code for attribute settings:

void v_input_cb(fltk3::Input* i, void* v) {
  int n = fl_intptr_t(i->user_data());
  if (v == Fl_Panel::LOAD) {
    i->static_value(Fl_Panel::selected_widget()->extra_code(n));
  } else {
    int mod = 0;
    const char *c = i->value();
    const char *d = c_check(c&&c[0]=='#' ? c+1 : c);
    if (d) {fltk3::message("Error in %s: %s",i->label(),d); haderror = 1; return;}
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type *t = (Fl_Widget_Type*)o;
	t->extra_code(n,c);
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void subclass_cb(fltk3::Input* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_menu_item()) {i->deactivate(); return;} else i->activate();
    i->static_value(Fl_Panel::selected_widget()->subclass());
  } else {
    int mod = 0;
    const char *c = i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type *t = (Fl_Widget_Type*)o;
	t->subclass(c);
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

////////////////////////////////////////////////////////////////

// textstuff: set textfont, textsize, textcolor attributes:

void textfont_cb(fltk3::Choice* i, void* v) {
  fltk3::Font n; int s; fltk3::Color c;
  if (v == Fl_Panel::LOAD) {
    if (!Fl_Panel::selected_widget()->textstuff(0,n,s,c)) {i->deactivate(); return;}
    i->activate();
    if (n > 15) n = fltk3::HELVETICA;
    i->value(n);
  } else {
    int mod = 0;
    n = (fltk3::Font)i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	q->textstuff(1,n,s,c);
	q->o->redraw();
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void textsize_cb(fltk3::ValueInput* i, void* v) {
  fltk3::Font n; int s; fltk3::Color c;
  if (v == Fl_Panel::LOAD) {
    if (!Fl_Panel::selected_widget()->textstuff(0,n,s,c)) {i->deactivate(); return;}
    i->activate();
  } else {
    int mod = 0;
    s = int(i->value());
    if (s <= 0) s = Fl_Widget_Type::default_size;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	q->textstuff(2,n,s,c);
	q->o->redraw();
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
  i->value(s);
}

void textcolor_cb(fltk3::Button* i, void* v) {
  fltk3::Font n; int s; fltk3::Color c;
  if (v == Fl_Panel::LOAD) {
    if (!Fl_Panel::selected_widget()->textstuff(0,n,s,c)) {i->deactivate(); return;}
    i->activate();
  } else {
    int mod = 0;
    c = i->color();
    fltk3::Color d = fltk3::show_colormap(c);
    if (d == c) return;
    c = d;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	q->textstuff(3,n,s,c); q->o->redraw();
	mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
  i->color(c); i->labelcolor(fltk3::contrast(fltk3::BLACK,c)); i->redraw();
}

////////////////////////////////////////////////////////////////
// Kludges to the panel for subclasses:

void min_w_cb(fltk3::ValueInput* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (!Fl_Panel::selected_type()->is_window()) {i->parent()->hide(); return;}
    i->parent()->show();
    i->value(((Fl_Window_Type*)Fl_Panel::selected_widget())->pMinW);
  } else {
    int mod = 0;
    int n = (int)i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_window()) {
        ((Fl_Window_Type*)Fl_Panel::selected_widget())->pMinW = n;
        mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void min_h_cb(fltk3::ValueInput* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (!Fl_Panel::selected_type()->is_window()) return;
    i->value(((Fl_Window_Type*)Fl_Panel::selected_widget())->pMinH);
  } else {
    int mod = 0;
    int n = (int)i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_window()) {
        ((Fl_Window_Type*)Fl_Panel::selected_widget())->pMinH = n;
        mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void max_w_cb(fltk3::ValueInput* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (!Fl_Panel::selected_type()->is_window()) return;
    i->value(((Fl_Window_Type*)Fl_Panel::selected_widget())->pMaxW);
  } else {
    int mod = 0;
    int n = (int)i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_window()) {
        ((Fl_Window_Type*)Fl_Panel::selected_widget())->pMaxW = n;
        mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void max_h_cb(fltk3::ValueInput* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (!Fl_Panel::selected_type()->is_window()) return;
    i->value(((Fl_Window_Type*)Fl_Panel::selected_widget())->pMaxH);
  } else {
    int mod = 0;
    int n = (int)i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_window()) {
        ((Fl_Window_Type*)Fl_Panel::selected_widget())->pMaxH = n;
        mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}

void set_min_size_cb(fltk3::Button*, void* v) {
  if (v == Fl_Panel::LOAD) {
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_window()) {
        Fl_Window_Type *win = (Fl_Window_Type*)Fl_Panel::selected_widget();
        win->pMinW = win->o->w();
        win->pMinH = win->o->h();
        mod = 1;
      }
    }
    Fl_Panel::propagate_load(the_widget_panel);
    if (mod) set_modflag(1);
  }
}

void set_max_size_cb(fltk3::Button*, void* v) {
  if (v == Fl_Panel::LOAD) {
  } else {
    int mod = 0;
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_window()) {
        Fl_Window_Type *win = (Fl_Window_Type*)Fl_Panel::selected_widget();
        win->pMaxW = win->o->w();
        win->pMaxH = win->o->h();
        mod = 1;
      }
    }
    Fl_Panel::propagate_load(the_widget_panel);
    if (mod) set_modflag(1);
  }
}

void slider_size_cb(fltk3::ValueInput* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_window()) 
      i->parent()->hide(); 
    else
      i->parent()->show();
    if (Fl_Panel::selected_type()->is_valuator()<2) {i->deactivate(); return;}
    i->activate();
    i->value(((fltk3::Slider*)(Fl_Panel::selected_widget()->o))->slider_size());
  } else {
    int mod = 0;
    double n = i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	if (q->is_valuator()>=2) {
	  ((fltk3::Slider*)(q->o))->slider_size(n);
	  q->o->redraw();
	  mod = 1;
	}
      }
    }
    if (mod) set_modflag(1);
  }
}

void min_cb(fltk3::ValueInput* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_widget()->is_valuator()) {
      i->activate();
      i->value(((fltk3::Valuator*)(Fl_Panel::selected_widget()->o))->minimum());
    } else if (Fl_Panel::selected_type()->is_spinner()) {
      i->activate();
      i->value(((fltk3::Spinner*)(Fl_Panel::selected_widget()->o))->minimum());
    } else {
      i->deactivate();
      return;
    }
  } else {
    int mod = 0;
    double n = i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	if (q->is_valuator()) {
	  ((fltk3::Valuator*)(q->o))->minimum(n);
	  q->o->redraw();
	  mod = 1;
	} else if (q->is_spinner()) {
          ((fltk3::Spinner*)(q->o))->minimum(n);
          q->o->redraw();
          mod = 1;
        }
      }
    }
    if (mod) set_modflag(1);
  }
}

void max_cb(fltk3::ValueInput* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_valuator()) {
      i->activate();
      i->value(((fltk3::Valuator*)(Fl_Panel::selected_widget()->o))->maximum());
    } else if (Fl_Panel::selected_type()->is_spinner()) {
      i->activate();
      i->value(((fltk3::Spinner*)(Fl_Panel::selected_widget()->o))->maximum());
    } else {
      i->deactivate();
      return;
    }
  } else {
    int mod = 0;
    double n = i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	if (q->is_valuator()) {
	  ((fltk3::Valuator*)(q->o))->maximum(n);
	  q->o->redraw();
	  mod = 1;
        } else if (q->is_spinner()) {
          ((fltk3::Spinner*)(q->o))->maximum(n);
          q->o->redraw();
          mod = 1;
	}
      }
    }
    if (mod) set_modflag(1);
  }
}

void step_cb(fltk3::ValueInput* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_valuator()) {
      i->activate();
      i->value(((fltk3::Valuator*)(Fl_Panel::selected_widget()->o))->step());
    } else if (Fl_Panel::selected_type()->is_spinner()) {
      i->activate();
      i->value(((fltk3::Spinner*)(Fl_Panel::selected_widget()->o))->step());
    } else {
      i->deactivate();
      return;
    }
  } else {
    int mod = 0;
    double n = i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
        Fl_Widget_Type* q = (Fl_Widget_Type*)o;
        if (q->is_valuator()) {
          ((fltk3::Valuator*)(q->o))->step(n);
          q->o->redraw();
          mod = 1;
        } else if (q->is_spinner()) {
          ((fltk3::Spinner*)(q->o))->step(n);
          q->o->redraw();
          mod = 1;
        }
      }
    }
    if (mod) set_modflag(1);
  }
}

void value_cb(fltk3::ValueInput* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    if (Fl_Panel::selected_type()->is_valuator()) {
      i->activate();
      i->value(((fltk3::Valuator*)(Fl_Panel::selected_widget()->o))->value());
    } else if (Fl_Panel::selected_type()->is_button()) {
      i->activate();
      i->value(((fltk3::Button*)(Fl_Panel::selected_widget()->o))->value());
    } else if (Fl_Panel::selected_type()->is_spinner()) {
      i->activate();
      i->value(((fltk3::Spinner*)(Fl_Panel::selected_widget()->o))->value());
    } else 
      i->deactivate();
  } else {
    int mod = 0;
    double n = i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
	Fl_Widget_Type* q = (Fl_Widget_Type*)o;
	if (q->is_valuator()) {
	  ((fltk3::Valuator*)(q->o))->value(n);
	  mod = 1;
	} else if (q->is_button()) {
	  ((fltk3::Button*)(q->o))->value(n != 0);
	  if (q->is_menu_item()) q->redraw();
	  mod = 1;
        } else if (q->is_spinner()) {
          ((fltk3::Spinner*)(q->o))->value(n);
          mod = 1;
	}
      }
    }
    if (mod) set_modflag(1);
  }
}


////////////////////////////////////////////////////////////////

// subtypes:


void subtype_cb(fltk3::Choice* i, void* v) {
  if (v == Fl_Panel::LOAD) {
    fltk3::MenuItem* m = Fl_Panel::selected_widget()->subtypes();
    if (!m) {i->deactivate(); return;}
    i->menu(m);
    int j;
    for (j = 0;; j++) {
      if (!m[j].text) {j = 0; break;}
      if (Fl_Panel::selected_type()->is_spinner()) {
        if (m[j].argument() == ((fltk3::Spinner*)Fl_Panel::selected_widget()->o)->type()) break;
      } else {
        if (m[j].argument() == Fl_Panel::selected_widget()->o->type()) break;
      }
    }
    i->value(j);
    i->activate();
    i->redraw();
  } else {
    int mod = 0;
    int n = int(i->mvalue()->argument());
    fltk3::MenuItem* m = Fl_Panel::selected_widget()->subtypes();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_widget()) {
        Fl_Widget_Type* q = (Fl_Widget_Type*)o;
        if (q->subtypes()==m) {
          if (q->is_spinner())
            ((fltk3::Spinner*)q->o)->type(n);
          else
            q->o->type(n);
          q->redraw();
          mod = 1;
        }
      }
    }
    if (mod) set_modflag(1);
  }
}

////////////////////////////////////////////////////////////////

// FIXME: move content to Fl_Panel
void set_cb(fltk3::Button*, void*) {
  haderror = 0;
  fltk3::Widget*const* a = the_widget_panel->array();
  for (int i=the_widget_panel->children(); i--;) {
    fltk3::Widget* o = *a++;
    if (o->changed()) {
      o->do_callback();
      if (haderror) return;
      o->clear_changed();
    }
  }
}

void ok_cb(fltk3::ReturnButton* o, void* v) {
  set_cb(o,v);
  if (!haderror) the_widget_panel->hide();
}

void revert_cb(fltk3::Button*, void*) {
  // We have to revert all dynamically changing fields:
  // but for now only the first label works...
  if (Fl_Panel::numselected == 1) Fl_Panel::selected_widget()->label(oldlabel);
  Fl_Panel::propagate_load(the_widget_panel);
}

void cancel_cb(fltk3::Button* o, void* v) {
  revert_cb(o,v);
  the_widget_panel->hide();
}

void toggle_overlays(fltk3::Widget *,void *); // in Fl_Window_Type.cxx
void overlay_cb(fltk3::Button*o,void *v) {
  toggle_overlays(o,v);
}

void leave_live_mode_cb(fltk3::Widget*, void*);

void live_mode_cb(fltk3::Button*o,void *) {
  /// \todo live mode should end gracefully when the application quits
  ///       or when the user closes the live widget
  static Fl_Type *live_type = 0L;
  static fltk3::Widget *live_widget = 0L;
  static fltk3::Window *live_window = 0L;
  // if 'o' is 0, we must quit live mode
  if (!o) {
    o = wLiveMode;
    o->value(0);
  }
  if (o->value()) {
    if (Fl_Panel::numselected == 1) {
      fltk3::Group::current(0L);
      live_widget = Fl_Panel::selected_widget()->enter_live_mode(1);
      if (live_widget) {
        live_type = Fl_Panel::selected_widget();
        fltk3::Group::current(0);
        int w = live_widget->w();
        int h = live_widget->h();
        live_window = new fltk3::DoubleWindow(w+20, h+55, "Fluid Live Mode Widget");
        live_window->box(fltk3::FLAT_BOX);
        live_window->color(fltk3::GREEN);
        fltk3::Group *rsz = new fltk3::Group(0, h+20, 130, 35);
        rsz->box(fltk3::NO_BOX);
        fltk3::Widget *rsz_dummy = new fltk3::Widget(110, h+20, 1, 25);
        rsz_dummy->box(fltk3::NO_BOX);
        rsz->resizable(rsz_dummy);
        fltk3::Button *btn = new fltk3::Button(10, h+20, 100, 25, "Exit Live Mode");
        btn->labelsize(12);
        btn->callback(leave_live_mode_cb);
        rsz->end();
        live_window->add(live_widget);
        live_widget->position(10, 10);
        live_window->resizable(live_widget);
        live_window->set_modal(); // block all other UI
        live_window->callback(leave_live_mode_cb);
        if (Fl_Panel::selected_type()->is_window()) {
          Fl_Window_Type *w = (Fl_Window_Type*)Fl_Panel::selected_widget();
          int mw = w->pMinW; if (mw>0) mw += 20;
          int mh = w->pMinH; if (mh>0) mh += 55;
          int MW = w->pMaxW; if (MW>0) MW += 20; 
          int MH = w->pMaxH; if (MH>2) MH += 55;
          if (mw || mh || MW || MH)
            live_window->size_range(mw, mh, MW, MH);
        }
        live_window->show();
      } else o->value(0);
    } else o->value(0);
  } else {
    if (live_type)
      live_type->leave_live_mode();
    if (live_window) {
      live_window->hide();
      fltk3::delete_widget(live_window);
    }
    live_type = 0L;
    live_widget = 0L;
    live_window = 0L;
  }
}

extern void redraw_overlays();
extern void check_redraw_corresponding_parent(Fl_Type*);
extern void redraw_browser();
extern void update_sourceview_position();
extern void workspace_panel_set_cb(fltk3::Widget*, void*);

// Called when ui changes what objects are selected:
// p is selected object, null for all deletions (we must throw away
// old panel in that case, as the object may no longer exist)
void selection_changed(Fl_Type *p) {
  // store all changes to the current selected objects:
  if (p && the_widget_panel && the_widget_panel->visible()) {
    set_cb(0,0);
    // if there was an error, we try to leave the selected set unchanged:
    if (haderror) {
      Fl_Type *q = 0;
      for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
	o->new_selected = o->selected;
	if (!q && o->selected) q = o;
      }
      if (!p || !p->selected) p = q;
      Fl_Type::current = p;
      redraw_browser();
      return;
    }
  }
  if (p && the_workspace_panel && the_workspace_panel->visible()) {
    workspace_panel_set_cb(0,0);
    // FIXME: haderror
  }
  // update the selected flags to new set:
  Fl_Type *q = 0;
  for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
    o->selected = o->new_selected;
    if (!q && o->selected) q = o;
  }
  if (!p || !p->selected) p = q;
  Fl_Type::current = p;
  check_redraw_corresponding_parent(p);
  redraw_overlays();
  // load the panel with the new settings:
  if (p && p->is_widget() && the_widget_panel && the_widget_panel->visible()) {
    the_widget_panel->load(&Fl_Type::is_widget);
    if (the_workspace_panel) the_workspace_panel->hide();
  } else if (p && p->is_tool() && the_workspace_panel && the_workspace_panel->visible()) {
    the_workspace_panel->load(&Fl_Type::is_tool);
    if (the_widget_panel) the_widget_panel->hide();
  } else {
    if (the_widget_panel) the_widget_panel->hide();
    if (the_workspace_panel) the_workspace_panel->hide();
  }
  // update the source viewer to show the code for the selected object
  update_sourceview_position();
}

////////////////////////////////////////////////////////////////
// Writing the C code:

// test to see if user named a function, or typed in code:
int is_name(const char *c) {
  for (; *c; c++) if (ispunct(*c) && *c!='_' && *c!=':') return 0;
  return 1;
}

// Test to see if name() is an array entry.  If so, and this is the
// highest number, return name[num+1].  Return null if not the highest
// number or a field or function.  Return name() if not an array entry.
const char *array_name(Fl_Widget_Type *o) {
  const char *c = o->name();
  if (!c) return 0;
  const char *d;
  for (d = c; *d != '['; d++) {
    if (!*d) return c;
    if (ispunct(*d) && *d!='_') return 0;
  }
  int num = atoi(d+1);
  int sawthis = 0;
  Fl_Type *t = o->prev;
  Fl_Type *tp = o;
  const char *cn = o->class_name(1);
  for (; t && t->class_name(1) == cn; tp = t, t = t->prev);
  for (t = tp; t && t->class_name(1) == cn; t = t->next) {
    if (t == o) {sawthis=1; continue;}
    const char *e = t->name();
    if (!e) continue;
    if (strncmp(c,e,d-c)) continue;
    int n1 = atoi(e+(d-c)+1);
    if (n1 > num || (n1==num && sawthis)) return 0;
  }
  static char buffer[128];
  // MRS: we want strncpy() here...
  strncpy(buffer,c,d-c+1);
  snprintf(buffer+(d-c+1),sizeof(buffer) - (d-c+1), "%d]",num+1);
  return buffer;
}

// Test to see if extra code is a declaration:
int isdeclare(const char *c) {
  while (isspace(*c)) c++;
  if (*c == '#') return 1;
  if (!strncmp(c,"extern",6)) return 1;
  if (!strncmp(c,"typedef",7)) return 1;
  if (!strncmp(c,"using",5)) return 1;
  return 0;
}

void leave_live_mode_cb(fltk3::Widget*, void*) {
  live_mode_cb(0, 0);
}


//
// End of "$Id$".
//
