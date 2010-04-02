//
// "$Id: forms.H 7365 2010-03-30 15:18:29Z matt $"
//
// Forms emulation header file for the Fast Light Tool Kit (FLTK).
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

#ifndef __FORMS_H__
#define __FORMS_H__

#include "run.h"
#include "Group.h"
#include "Window.h"
#include "draw.h"

typedef fltk3::Widget FL_OBJECT;
typedef fltk3::Window FL_FORM;

////////////////////////////////////////////////////////////////
// Random constants & symbols defined by forms.h file:

#ifndef NULL
#define NULL 0
#endif
#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

#define FL_ON		1
#define FL_OK		1
#define FL_VALID	1
#define FL_PREEMPT	1
#define FL_AUTO		2
#define FL_WHEN_NEEDED	FL_AUTO
#define FL_OFF		0
#define FL_NONE		0
#define FL_CANCEL	0
#define FL_INVALID	0
#define FL_IGNORE	-1
//#define fltk3::CLOSE	-2 // this variable is never used in FLTK Forms. It is removed becaus it conflicts with the window fltk3::CLOSE event

#define FL_LCOL		FL_BLACK
#define FL_COL1		FL_GRAY
#define FL_MCOL		FL_LIGHT1
#define FL_LEFT_BCOL	FL_LIGHT3 // 53 is better match
#define FL_TOP_BCOL	FL_LIGHT2 // 51
#define FL_BOTTOM_BCOL	FL_DARK2  // 40
#define FL_RIGHT_BCOL	FL_DARK3  // 36
#define FL_INACTIVE	FL_INACTIVE_COLOR
#define FL_INACTIVE_COL	FL_INACTIVE_COLOR
#define FL_FREE_COL1	FL_FREE_COLOR
#define FL_FREE_COL2	((Fl_Color)(FL_FREE_COLOR+1))
#define FL_FREE_COL3	((Fl_Color)(FL_FREE_COLOR+2))
#define FL_FREE_COL4	((Fl_Color)(FL_FREE_COLOR+3))
#define FL_FREE_COL5	((Fl_Color)(FL_FREE_COLOR+4))
#define FL_FREE_COL6	((Fl_Color)(FL_FREE_COLOR+5))
#define FL_FREE_COL7	((Fl_Color)(FL_FREE_COLOR+6))
#define FL_FREE_COL8	((Fl_Color)(FL_FREE_COLOR+7))
#define FL_FREE_COL9	((Fl_Color)(FL_FREE_COLOR+8))
#define FL_FREE_COL10	((Fl_Color)(FL_FREE_COLOR+9))
#define FL_FREE_COL11	((Fl_Color)(FL_FREE_COLOR+10))
#define FL_FREE_COL12	((Fl_Color)(FL_FREE_COLOR+11))
#define FL_FREE_COL13	((Fl_Color)(FL_FREE_COLOR+12))
#define FL_FREE_COL14	((Fl_Color)(FL_FREE_COLOR+13))
#define FL_FREE_COL15	((Fl_Color)(FL_FREE_COLOR+14))
#define FL_FREE_COL16	((Fl_Color)(FL_FREE_COLOR+15))
#define FL_TOMATO	((Fl_Color)(131))
#define FL_INDIANRED	((Fl_Color)(164))
#define FL_SLATEBLUE	((Fl_Color)(195))
#define FL_DARKGOLD	((Fl_Color)(84))
#define FL_PALEGREEN	((Fl_Color)(157))
#define FL_ORCHID	((Fl_Color)(203))
#define FL_DARKCYAN	((Fl_Color)(189))
#define FL_DARKTOMATO	((Fl_Color)(113))
#define FL_WHEAT	((Fl_Color)(174))

#define FL_ALIGN_BESIDE	fltk3::ALIGN_INSIDE

#define FL_PUP_TOGGLE	2 // fltk3::MENU_TOGGLE
#define FL_PUP_INACTIVE 1 // fltk3::MENU_INACTIVE
#define FL_NO_FRAME	fltk3::NO_BOX
#define FL_ROUNDED3D_UPBOX 	FL_ROUND_UP_BOX
#define FL_ROUNDED3D_DOWNBOX	FL_ROUND_DOWN_BOX
#define FL_OVAL3D_UPBOX		FL_ROUND_UP_BOX
#define FL_OVAL3D_DOWNBOX	FL_ROUND_DOWN_BOX

#define FL_MBUTTON1	1
#define FL_LEFTMOUSE	1
#define FL_MBUTTON2	2
#define FL_MIDDLEMOUSE	2
#define FL_MBUTTON3	3
#define FL_RIGHTMOUSE	3
#define FL_MBUTTON4	4
#define FL_MBUTTON5	5

#define FL_INVALID_STYLE 255
#define FL_NORMAL_STYLE	fltk3::HELVETICA
#define FL_BOLD_STYLE	fltk3::HELVETICA_BOLD
#define FL_ITALIC_STYLE	fltk3::HELVETICA_ITALIC
#define FL_BOLDITALIC_STYLE fltk3::HELVETICA_BOLD_ITALIC
#define FL_FIXED_STYLE	fltk3::COURIER
#define FL_FIXEDBOLD_STYLE fltk3::COURIER_BOLD
#define FL_FIXEDITALIC_STYLE fltk3::COURIER_ITALIC
#define FL_FIXEDBOLDITALIC_STYLE fltk3::COURIER_BOLD_ITALIC
#define FL_TIMES_STYLE	fltk3::TIMES
#define FL_TIMESBOLD_STYLE fltk3::TIMES_BOLD
#define FL_TIMESITALIC_STYLE fltk3::TIMES_ITALIC
#define FL_TIMESBOLDITALIC_STYLE fltk3::TIMES_BOLD_ITALIC

// hacks to change the labeltype() when passed to fl_set_object_lstyle():
#define FL_SHADOW_STYLE		(fltk3::SHADOW_LABEL<<8)
#define FL_ENGRAVED_STYLE	(fltk3::ENGRAVED_LABEL<<8)
#define FL_EMBOSSED_STYLE	(fltk3::EMBOSSED_LABEL<<0)

// size values are different from XForms, match older Forms:
#define FL_TINY_SIZE	8
#define FL_SMALL_SIZE	11 // 10
//#define FL_NORMAL_SIZE	14 // 12
#define FL_MEDIUM_SIZE	18 // 14
#define FL_LARGE_SIZE	24 // 18
#define FL_HUGE_SIZE	32 // 24
#define FL_DEFAULT_SIZE	FL_SMALL_SIZE
#define FL_TINY_FONT	FL_TINY_SIZE
#define FL_SMALL_FONT	FL_SMALL_SIZE
#define FL_NORMAL_FONT	FL_NORMAL_SIZE
#define FL_MEDIUM_FONT	FL_MEDIUM_SIZE
#define FL_LARGE_FONT	FL_LARGE_SIZE
#define FL_HUGE_FONT	FL_HUGE_SIZE
#define FL_NORMAL_FONT1	FL_SMALL_FONT
#define FL_NORMAL_FONT2	FL_NORMAL_FONT
#define FL_DEFAULT_FONT	FL_SMALL_FONT

#define FL_RETURN_END_CHANGED	fltk3::WHEN_RELEASE
#define FL_RETURN_CHANGED	fltk3::WHEN_CHANGED
#define FL_RETURN_END		fltk3::WHEN_RELEASE_ALWAYS
#define FL_RETURN_ALWAYS	(fltk3::WHEN_CHANGED|fltk3::WHEN_NOT_CHANGED)

#define FL_BOUND_WIDTH	3

typedef int FL_Coord;
typedef int FL_COLOR;

////////////////////////////////////////////////////////////////
// fltk interaction:

#define FL_CMD_OPT void
extern FL_EXPORT void fl_initialize(int*, char*[], const char*, FL_CMD_OPT*, int);
inline void fl_finish() {}

typedef void (*FL_IO_CALLBACK) (int, void*);
inline void fl_add_io_callback(int fd, short w, FL_IO_CALLBACK cb, void* v) {
  fltk3::add_fd(fd,w,cb,v);}
inline void fl_remove_io_callback(int fd, short, FL_IO_CALLBACK) {
  fltk3::remove_fd(fd);} // removes all the callbacks!

// type of callback is different and no "id" number is returned:
inline void fl_add_timeout(long msec, void (*cb)(void*), void* v) {
  fltk3::add_timeout(msec*.001, cb, v);}
inline void fl_remove_timeout(int) {}

// type of callback is different!
inline void fl_set_idle_callback(void (*cb)()) {fltk3::set_idle(cb);}

FL_EXPORT fltk3::Widget* fl_do_forms(void);
FL_EXPORT fltk3::Widget* fl_check_forms();
inline fltk3::Widget* fl_do_only_forms(void) {return fl_do_forms();}
inline fltk3::Widget* fl_check_only_forms(void) {return fl_check_forms();}

// because of new redraw behavior, these are no-ops:
inline void fl_freeze_object(fltk3::Widget*) {}
inline void fl_unfreeze_object(fltk3::Widget*) {}
inline void fl_freeze_form(fltk3::Window*) {}
inline void fl_unfreeze_form(fltk3::Window*) {}
inline void fl_freeze_all_forms() {}
inline void fl_unfreeze_all_forms() {}

inline void fl_set_focus_object(fltk3::Window*, fltk3::Widget* o) {fltk3::focus(o);}
inline void fl_reset_focus_object(fltk3::Widget* o) {fltk3::focus(o);}
#define fl_set_object_focus fl_set_focus_object

// void fl_set_form_atclose(fltk3::Window*w,int (*cb)(fltk3::Window*,void*),void* v)
// void fl_set_atclose(int (*cb)(fltk3::Window*,void*),void*)
// fl_set_form_atactivate/atdeactivate not implemented!

////////////////////////////////////////////////////////////////
// fltk3::Widget:

inline void fl_set_object_boxtype(fltk3::Widget* o, fltk3::Boxtype a) {o->box(a);}
inline void fl_set_object_lsize(fltk3::Widget* o,int s) {o->labelsize(s);}

/* forms lib font indexes must be byte sized - extract correct byte from style word */
inline void fl_set_object_lstyle(fltk3::Widget* o,int a) {
  o->labelfont((Fl_Font)(a&0xff)); o->labeltype((fltk3::Labeltype)(a>>8));}
inline void fl_set_object_lcol(fltk3::Widget* o, Fl_Color a) {o->labelcolor(a);}
#define fl_set_object_lcolor  fl_set_object_lcol
inline void fl_set_object_lalign(fltk3::Widget* o, fltk3::Align a) {o->align(a);}
#define fl_set_object_align fl_set_object_lalign
inline void fl_set_object_color(fltk3::Widget* o,Fl_Color a,Fl_Color b) {o->color(a,b);}
inline void fl_set_object_label(fltk3::Widget* o, const char* a) {o->label(a); o->redraw();}
inline void fl_set_object_position(fltk3::Widget*o,int x,int y) {o->position(x,y);}
inline void fl_set_object_size(fltk3::Widget* o, int w, int h) {o->size(w,h);}
inline void fl_set_object_geometry(fltk3::Widget* o,int x,int y,int w,int h) {o->resize(x,y,w,h);}

inline void fl_get_object_geometry(fltk3::Widget* o,int*x,int*y,int*w,int*h) {
  *x = o->x(); *y = o->y(); *w = o->w(); *h = o->h();}
inline void fl_get_object_position(fltk3::Widget* o,int*x,int*y) {
  *x = o->x(); *y = o->y();}

typedef void (*Forms_CB)(fltk3::Widget*, long);
inline void fl_set_object_callback(fltk3::Widget*o,Forms_CB c,long a) {o->callback(c,a);}
#define fl_set_call_back      fl_set_object_callback
inline void fl_call_object_callback(fltk3::Widget* o) {o->do_callback();}
inline void fl_trigger_object(fltk3::Widget* o) {o->do_callback();}
inline void fl_set_object_return(fltk3::Widget* o, int v) {
  o->when((fltk3::When)(v|fltk3::WHEN_RELEASE));}

inline void fl_redraw_object(fltk3::Widget* o) {o->redraw();}
inline void fl_show_object(fltk3::Widget* o) {o->show();}
inline void fl_hide_object(fltk3::Widget* o) {o->hide();}
inline void fl_free_object(fltk3::Widget* x) {delete x;}
inline void fl_delete_object(fltk3::Widget* o) {o->parent()->remove(*o);}
inline void fl_activate_object(fltk3::Widget* o) {o->activate();}
inline void fl_deactivate_object(fltk3::Widget* o) {o->deactivate();}

inline void fl_add_object(fltk3::Window* f, fltk3::Widget* x) {f->add(x);}
inline void fl_insert_object(fltk3::Widget* o, fltk3::Widget* b) {b->parent()->insert(*o,b);}

inline fltk3::Window* FL_ObjWin(fltk3::Widget* o) {return o->window();}

////////////////////////////////////////////////////////////////
// things that appered in the demos a lot that I don't emulate, but
// I did not want to edit out of all the demos...

inline int fl_get_border_width() {return 3;}
inline void fl_set_border_width(int) {}
inline void fl_set_object_dblbuffer(fltk3::Widget*, int) {}
inline void fl_set_form_dblbuffer(fltk3::Window*, int) {}

////////////////////////////////////////////////////////////////
// fltk3::Window:

inline void fl_free_form(fltk3::Window* x) {delete x;}
inline void fl_redraw_form(fltk3::Window* f) {f->redraw();}

inline fltk3::Window* fl_bgn_form(fltk3::Boxtype b,int w,int h) {
  fltk3::Window* g = new fltk3::Window(w,h,0);
  g->box(b);
  return g;
}
FL_EXPORT void fl_end_form();
inline void fl_addto_form(fltk3::Window* f) {f->begin();}
inline fltk3::Group* fl_bgn_group() {return new fltk3::Group(0,0,0,0,0);}
inline void fl_end_group() {fltk3::Group::current()->forms_end();}
inline void fl_addto_group(fltk3::Widget* o) {((fltk3::Group* )o)->begin();}
#define resizebox _ddfdesign_kludge()

inline void fl_scale_form(fltk3::Window* f, double x, double y) {
  f->resizable(f); f->size(int(f->w()*x),int(f->h()*y));}
inline void fl_set_form_position(fltk3::Window* f,int x,int y) {f->position(x,y);}
inline void fl_set_form_size(fltk3::Window* f, int w, int h) {f->size(w,h);}
inline void fl_set_form_geometry(fltk3::Window* f,int x,int y,int w,int h) {
  f->resize(x,y,w,h);}
#define fl_set_initial_placement fl_set_form_geometry
inline void fl_adjust_form_size(fltk3::Window*) {}

FL_EXPORT void fl_show_form(fltk3::Window* f,int p,int b,const char* n);
enum {	// "p" argument values:
  FL_PLACE_FREE = 0,	// make resizable
  FL_PLACE_MOUSE = 1,	// mouse centered on form
  FL_PLACE_CENTER = 2,	// center of the screen
  FL_PLACE_POSITION = 4,// fixed position, resizable
  FL_PLACE_SIZE = 8,	// fixed size, normal fltk behavior
  FL_PLACE_GEOMETRY =16,// fixed size and position
  FL_PLACE_ASPECT = 32,	// keep aspect ratio (ignored)
  FL_PLACE_FULLSCREEN=64,// fill screen
  FL_PLACE_HOTSPOT = 128,// enables hotspot
  FL_PLACE_ICONIC = 256,// iconic (ignored)
  FL_FREE_SIZE=(1<<14),	// force resizable
  FL_FIX_SIZE =(1<<15)	// force off resizable
};
#define FL_PLACE_FREE_CENTER (FL_PLACE_CENTER|FL_FREE_SIZE)
#define FL_PLACE_CENTERFREE  (FL_PLACE_CENTER|FL_FREE_SIZE)
enum {	// "b" arguement values:
  FL_NOBORDER = 0,
  FL_FULLBORDER,
  FL_TRANSIENT
//FL_MODAL = (1<<8)	// not implemented yet in Forms
};
inline void fl_set_form_hotspot(fltk3::Window* w,int x,int y) {w->hotspot(x,y);}
inline void fl_set_form_hotobject(fltk3::Window* w, fltk3::Widget* o) {w->hotspot(o);}
extern FL_EXPORT char fl_flip;	// in forms.C
inline void fl_flip_yorigin() {fl_flip = 1;}

#define fl_prepare_form_window fl_show_form
inline void fl_show_form_window(fltk3::Window*) {}

inline void fl_raise_form(fltk3::Window* f) {f->show();}

inline void fl_hide_form(fltk3::Window* f) {f->hide();}
inline void fl_pop_form(fltk3::Window* f) {f->show();}

extern FL_EXPORT char fl_modal_next; // in forms.C
inline void fl_activate_all_forms() {}
inline void fl_deactivate_all_forms() {fl_modal_next = 1;}
inline void fl_deactivate_form(fltk3::Window*w) {w->deactivate();}
inline void fl_activate_form(fltk3::Window*w) {w->activate();}

inline void fl_set_form_title(fltk3::Window* f, const char* s) {f->label(s);}
inline void fl_title_form(fltk3::Window* f, const char* s) {f->label(s);}

typedef void (*Forms_FormCB)(fltk3::Widget*);
inline void fl_set_form_callback(fltk3::Window* f,Forms_FormCB c) {f->callback(c);}
#define fl_set_form_call_back fl_set_form_callback

inline void fl_init() {}
FL_EXPORT void fl_set_graphics_mode(int,int);

inline int fl_form_is_visible(fltk3::Window* f) {return f->visible();}

inline int fl_mouse_button() {return fltk3::event_button();}
#define fl_mousebutton fl_mouse_button

#define fl_free       free
#define fl_malloc     malloc
#define fl_calloc     calloc
#define fl_realloc    realloc

////////////////////////////////////////////////////////////////
// Drawing functions.  Only usable inside an Fl_Free object?

inline void fl_drw_box(fltk3::Boxtype b,int x,int y,int w,int h,Fl_Color bgc,int=3) {
    fl_draw_box(b,x,y,w,h,bgc);}
inline void fl_drw_frame(fltk3::Boxtype b,int x,int y,int w,int h,Fl_Color bgc,int=3) {
    fl_draw_box(b,x,y,w,h,bgc);}

inline void fl_drw_text(fltk3::Align align, int x, int y, int w, int h,
		  Fl_Color fgcolor, int size, Fl_Font style,
		  const char* s) {
  fl_font(style,size);
  fl_color(fgcolor);
  fl_draw(s,x,y,w,h,align);
}

// this does not work except for CENTER...
inline void fl_drw_text_beside(fltk3::Align align, int x, int y, int w, int h,
		  Fl_Color fgcolor, int size, Fl_Font style,
		  const char* s) {
  fl_font(style,size);
  fl_color(fgcolor);
  fl_draw(s,x,y,w,h,align);
}

inline void fl_set_font_name(Fl_Font n,const char* s) {fltk3::set_font(n,s);}

inline void fl_mapcolor(Fl_Color c, uchar r, uchar g, uchar b) {fltk3::set_color(c,r,g,b);}

#define fl_set_clipping(x,y,w,h) fl_push_clip(x,y,w,h)
#define fl_unset_clipping() fl_pop_clip()

////////////////////////////////////////////////////////////////
// Forms classes:

inline fltk3::Widget* fl_add_new(fltk3::Widget* p) {return p;}
inline fltk3::Widget* fl_add_new(uchar t,fltk3::Widget* p) {p->type(t); return p;}

#define forms_constructor(type,name) \
inline type* name(uchar t,int x,int y,int w,int h,const char* l) { \
 return (type*)(fl_add_new(t, new type(x,y,w,h,l)));}
#define forms_constructort(type,name) \
inline type* name(uchar t,int x,int y,int w,int h,const char* l) { \
 return (type*)(fl_add_new(new type(t,x,y,w,h,l)));}
#define forms_constructorb(type,name) \
inline type* name(fltk3::Boxtype t,int x,int y,int w,int h,const char* l) { \
 return (type*)(fl_add_new(new type(t,x,y,w,h,l)));}

#include "Fl_FormsBitmap.H"
#define FL_NORMAL_BITMAP fltk3::NO_BOX
forms_constructorb(Fl_FormsBitmap, fl_add_bitmap)
inline void fl_set_bitmap_data(fltk3::Widget* o, int w, int h, const uchar* b) {
    ((Fl_FormsBitmap*)o)->set(w,h,b);
}

#include "Fl_FormsPixmap.H"
#define FL_NORMAL_PIXMAP fltk3::NO_BOX
forms_constructorb(Fl_FormsPixmap, fl_add_pixmap)
inline void fl_set_pixmap_data(fltk3::Widget* o, char*const* b) {
    ((Fl_FormsPixmap*)o)->set(b);
}
//inline void fl_set_pixmap_file(fltk3::Widget*, const char*);
inline void fl_set_pixmap_align(fltk3::Widget* o,fltk3::Align a,int,int) {o->align(a);}
//inline void fl_set_pixmap_colorcloseness(int, int, int);

#include "Fl_Box.H"
forms_constructorb(Fl_Box, fl_add_box)

#include "Fl_Browser.H"
forms_constructor(Fl_Browser, fl_add_browser)

inline void fl_clear_browser(fltk3::Widget* o) {
    ((Fl_Browser*)o)->clear();}
inline void fl_add_browser_line(fltk3::Widget* o, const char* s) {
    ((Fl_Browser*)o)->add(s);}
inline void fl_addto_browser(fltk3::Widget* o, const char* s) {
    ((Fl_Browser*)o)->add(s);} /* should also scroll to bottom */
//inline void fl_addto_browser_chars(fltk3::Widget*, const char*)
//#define fl_append_browser fl_addto_browser_chars
inline void fl_insert_browser_line(fltk3::Widget* o, int n, const char* s) {
    ((Fl_Browser*)o)->insert(n,s);}
inline void fl_delete_browser_line(fltk3::Widget* o, int n) {
    ((Fl_Browser*)o)->remove(n);}
inline void fl_replace_browser_line(fltk3::Widget* o, int n, const char* s) {
    ((Fl_Browser*)o)->replace(n,s);}
inline char* fl_get_browser_line(fltk3::Widget* o, int n) {
    return (char*)(((Fl_Browser*)o)->text(n));}
inline int fl_load_browser(fltk3::Widget* o, const char* f) {
    return ((Fl_Browser*)o)->load(f);}
inline void fl_select_browser_line(fltk3::Widget* o, int n) {
    ((Fl_Browser*)o)->select(n,1);}
inline void fl_deselect_browser_line(fltk3::Widget* o, int n) {
    ((Fl_Browser*)o)->select(n,0);}
inline void fl_deselect_browser(fltk3::Widget* o) {
    ((Fl_Browser*)o)->deselect();}
inline int fl_isselected_browser_line(fltk3::Widget* o, int n) {
    return ((Fl_Browser*)o)->selected(n);}
inline int fl_get_browser_topline(fltk3::Widget* o) {
    return ((Fl_Browser*)o)->topline();}
inline int fl_get_browser(fltk3::Widget* o) {
    return ((Fl_Browser*)o)->value();}
inline int fl_get_browser_maxline(fltk3::Widget* o) {
    return ((Fl_Browser*)o)->size();}
//linline int fl_get_browser_screenlines(fltk3::Widget*);
inline void fl_set_browser_topline(fltk3::Widget* o, int n) {
    ((Fl_Browser*)o)->topline(n);}
inline void fl_set_browser_fontsize(fltk3::Widget* o, int s) {
    ((Fl_Browser*)o)->textsize(s);}
inline void fl_set_browser_fontstyle(fltk3::Widget* o, Fl_Font s) {
    ((Fl_Browser*)o)->textfont(s);}
inline void fl_set_browser_specialkey(fltk3::Widget* o, char c) {
    ((Fl_Browser*)o)->format_char(c);}
//inline void fl_set_browser_vscrollbar(fltk3::Widget*, int);
//inline void fl_set_browser_hscrollbar(fltk3::Widget*, int);
//inline void fl_set_browser_leftslider(fltk3::Widget*, int);
//#define fl_set_browser_leftscrollbar fl_set_browser_leftslider
//inline void fl_set_browser_line_selectable(fltk3::Widget*, int, int);
//inline void fl_get_browser_dimension(fltk3::Widget*,int*,int*,int*,int*);
//inline void fl_set_browser_dblclick_callback(fltk3::Widget*,FL_CALLBACKPTR,long);
//inline void fl_set_browser_xoffset(fltk3::Widget*, FL_Coord);
//inline void fl_set_browser_scrollbarsize(fltk3::Widget*, int, int);
inline void fl_setdisplayed_browser_line(fltk3::Widget* o, int n, int i) {
    ((Fl_Browser*)o)->display(n,i);}
inline int fl_isdisplayed_browser_line(fltk3::Widget* o, int n) {
    return ((Fl_Browser*)o)->displayed(n);}

#include "Button.h"

#define FL_NORMAL_BUTTON	0
#define FL_TOUCH_BUTTON		4
#define FL_INOUT_BUTTON		5
#define FL_RETURN_BUTTON	6
#define FL_HIDDEN_RET_BUTTON	7
#define FL_PUSH_BUTTON		FL_TOGGLE_BUTTON
#define FL_MENU_BUTTON		9

FL_EXPORT fltk3::Button* fl_add_button(uchar t,int x,int y,int w,int h,const char* l);
inline int fl_get_button(fltk3::Widget* b) {return ((fltk3::Button*)b)->value();}
inline void fl_set_button(fltk3::Widget* b, int v) {((fltk3::Button*)b)->value(v);}
inline int fl_get_button_numb(fltk3::Widget*) {return fltk3::event_button();}
inline void fl_set_button_shortcut(fltk3::Widget* b, const char* s,int=0) {
    ((fltk3::Button*)b)->shortcut(s);}
//#define fl_set_object_shortcut(b,s) fl_set_button_shortcut(b,s)

#include "LightButton.h"
forms_constructor(fltk3::LightButton, fl_add_lightbutton)

#include "RoundButton.h"
forms_constructor(fltk3::RoundButton, fl_add_roundbutton)
forms_constructor(fltk3::RoundButton, fl_add_round3dbutton)

#include "CheckButton.h"
forms_constructor(fltk3::CheckButton, fl_add_checkbutton)

inline fltk3::Widget* fl_add_bitmapbutton(int t,int x,int y,int w,int h,const char* l) {fltk3::Widget* o = fl_add_button(t,x,y,w,h,l); return o;}
inline void fl_set_bitmapbutton_data(fltk3::Widget* o,int a,int b,uchar* c) {
  (new Fl_Bitmap(c,a,b))->label(o);}  // does not delete old Fl_Bitmap!

inline fltk3::Widget* fl_add_pixmapbutton(int t,int x,int y,int w,int h,const char* l) {fltk3::Widget* o = fl_add_button(t,x,y,w,h,l); return o;}
inline void fl_set_pixmapbutton_data(fltk3::Widget* o, const char*const* c) {
  (new Fl_Pixmap(c))->label(o);}  // does not delete old Fl_Pixmap!

// Fl_Canvas object not yet implemented!

#include "Fl_Chart.H"

forms_constructor(Fl_Chart, fl_add_chart)
inline void fl_clear_chart(fltk3::Widget* o) {
  ((Fl_Chart*)o)->clear();}
inline void fl_add_chart_value(fltk3::Widget* o,double v,const char* s,uchar c){
  ((Fl_Chart*)o)->add(v,s,c);}
inline void fl_insert_chart_value(fltk3::Widget* o, int i, double v, const char* s, uchar c) {
  ((Fl_Chart*)o)->insert(i,v,s,c);}
inline void fl_replace_chart_value(fltk3::Widget* o, int i, double v, const char* s, uchar c) {
  ((Fl_Chart*)o)->replace(i,v,s,c);}
inline void fl_set_chart_bounds(fltk3::Widget* o, double a, double b) {
  ((Fl_Chart*)o)->bounds(a,b);}
inline void fl_set_chart_maxnumb(fltk3::Widget* o, int v) {
  ((Fl_Chart*)o)->maxsize(v);}
inline void fl_set_chart_autosize(fltk3::Widget* o, int v) {
  ((Fl_Chart*)o)->autosize(v);}
inline void fl_set_chart_lstyle(fltk3::Widget* o, Fl_Font v) {
  ((Fl_Chart*)o)->textfont(v);}
inline void fl_set_chart_lsize(fltk3::Widget* o, int v) {
  ((Fl_Chart*)o)->textsize(v);}
inline void fl_set_chart_lcolor(fltk3::Widget* o, Fl_Color v) {
  ((Fl_Chart*)o)->textcolor(v);}
#define fl_set_chart_lcol   fl_set_chart_lcolor

#include "Choice.h"

#define FL_NORMAL_CHOICE	0
#define FL_NORMAL_CHOICE2	0
#define FL_DROPLIST_CHOICE	0

forms_constructor(fltk3::Choice, fl_add_choice)
inline void fl_clear_choice(fltk3::Widget* o) {
    ((fltk3::Choice*)o)->clear();}
inline void fl_addto_choice(fltk3::Widget* o, const char* s) {
    ((fltk3::Choice*)o)->add(s);}
inline void fl_replace_choice(fltk3::Widget* o, int i, const char* s) {
    ((fltk3::Choice*)o)->replace(i-1,s);}
inline void fl_delete_choice(fltk3::Widget* o, int i) {
    ((fltk3::Choice*)o)->remove(i-1);}
inline void fl_set_choice(fltk3::Widget* o, int i) {
    ((fltk3::Choice*)o)->value(i-1);}
// inline void fl_set_choice_text(fltk3::Widget*, const char*);
inline int fl_get_choice(fltk3::Widget* o) {
    return ((fltk3::Choice*)o)->value()+1;}
// inline const char* fl_get_choice_item_text(fltk3::Widget*, int);
// inline int fl_get_choice_maxitems(fltk3::Widget*);
inline const char* fl_get_choice_text(fltk3::Widget* o) {
    return ((fltk3::Choice*)o)->text();}
inline void fl_set_choice_fontsize(fltk3::Widget* o, int x) {
    ((fltk3::Choice*)o)->textsize(x);}
inline void fl_set_choice_fontstyle(fltk3::Widget* o, Fl_Font x) {
    ((fltk3::Choice*)o)->textfont(x);}
// inline void fl_set_choice_item_mode(fltk3::Widget*, int, unsigned);
// inline void fl_set_choice_item_shortcut(fltk3::Widget*, int, const char*);

#include "Fl_Clock.H"
forms_constructort(Fl_Clock, fl_add_clock)
inline void fl_get_clock(fltk3::Widget* o, int* h, int* m, int* s) {
    *h = ((Fl_Clock*)o)->hour();
    *m = ((Fl_Clock*)o)->minute();
    *s = ((Fl_Clock*)o)->second();
}

#include "Fl_Counter.H"
forms_constructor(Fl_Counter, fl_add_counter)
inline void fl_set_counter_value(fltk3::Widget* o, double v) {
    ((Fl_Counter*)o)->value(v);}
inline void fl_set_counter_bounds(fltk3::Widget* o, double a, double b) {
    ((Fl_Counter*)o)->bounds(a,b);}
inline void fl_set_counter_step(fltk3::Widget* o, double a, double b) {
    ((Fl_Counter*)o)->step(a,b);}
inline void fl_set_counter_precision(fltk3::Widget* o, int v) {
    ((Fl_Counter*)o)->precision(v);}
inline void fl_set_counter_return(fltk3::Widget* o, int v) {
    ((Fl_Counter*)o)->when((fltk3::When)(v|fltk3::WHEN_RELEASE));}
inline double fl_get_counter_value(fltk3::Widget* o) {
    return ((Fl_Counter*)o)->value();}
inline void fl_get_counter_bounds(fltk3::Widget* o, float* a, float* b) {
  *a = float(((Fl_Counter*)o)->minimum());
  *b = float(((Fl_Counter*)o)->maximum());
}
//inline void fl_set_counter_filter(fltk3::Widget*,const char* (*)(fltk3::Widget*,double,int));

// Cursor stuff cannot be emulated because it uses X stuff
inline void fl_set_cursor(fltk3::Window* w, Fl_Cursor c) {w->cursor(c);}
#define FL_INVISIBLE_CURSOR FL_CURSOR_NONE
#define FL_DEFAULT_CURSOR FL_CURSOR_DEFAULT

#include "Fl_Dial.H"

#define FL_DIAL_COL1 FL_GRAY
#define FL_DIAL_COL2 37

forms_constructor(Fl_Dial, fl_add_dial)
inline void fl_set_dial_value(fltk3::Widget* o, double v) {
  ((Fl_Dial*)o)->value(v);}
inline double fl_get_dial_value(fltk3::Widget* o) {
  return ((Fl_Dial*)o)->value();}
inline void fl_set_dial_bounds(fltk3::Widget* o, double a, double b) {
  ((Fl_Dial*)o)->bounds(a, b);}
inline void fl_get_dial_bounds(fltk3::Widget* o, float* a, float* b) {
  *a = float(((Fl_Dial*)o)->minimum());
  *b = float(((Fl_Dial*)o)->maximum());
}
inline void fl_set_dial_return(fltk3::Widget* o, int i) {
  ((Fl_Dial*)o)->when((fltk3::When)(i|fltk3::WHEN_RELEASE));}
inline void fl_set_dial_angles(fltk3::Widget* o, int a, int b) {
  ((Fl_Dial*)o)->angles((short)a, (short)b);}
//inline void fl_set_dial_cross(fltk3::Widget* o, int);
// inline void fl_set_dial_direction(fltk3::Widget* o, uchar d) {
//   ((Fl_Dial*)o)->direction(d);}
inline void fl_set_dial_step(fltk3::Widget* o, double v) {
  ((Fl_Dial*)o)->step(v);}

// Frames:

inline fltk3::Widget* fl_add_frame(fltk3::Boxtype i,int x,int y,int w,int h,const char* l) {
  return fl_add_box(i,x-3,y-3,w+6,h+6,l);}

// labelframe nyi
inline fltk3::Widget* fl_add_labelframe(fltk3::Boxtype i,int x,int y,int w,int h,const char* l) {
  fltk3::Widget* o = fl_add_box(i,x-3,y-3,w+6,h+6,l);
  o->align(fltk3::ALIGN_TOP_LEFT);
  return o;
}

#include "Fl_Free.H"
inline Fl_Free*
fl_add_free(int t,double x,double y,double w,double h,const char* l,
	    FL_HANDLEPTR hdl) {
 return (Fl_Free*)(fl_add_new(
   new Fl_Free(t,int(x),int(y),int(w),int(h),l,hdl)));
}

#include "ask.h"
#include "fl_show_colormap.H"

inline int fl_show_question(const char* c, int = 0) {return fltk3::choice("%s",fltk3::text_no,fltk3::text_yes,0L,c);}
FL_EXPORT void fl_show_message(const char *,const char *,const char *);
FL_EXPORT void fl_show_alert(const char *,const char *,const char *,int=0);
FL_EXPORT int fl_show_question(const char *,const char *,const char *);
inline const char *fl_show_input(const char *l,const char*d=0) {return fltk3::input(l,d);}
FL_EXPORT /*const*/ char *fl_show_simple_input(const char *label, const char *deflt = 0);
FL_EXPORT int fl_show_choice(
    const char *m1,
    const char *m2,
    const char *m3,
    int numb,
    const char *b0,
    const char *b1,
    const char *b2);

inline void fl_set_goodies_font(Fl_Font a, Fl_Fontsize b) {fltk3::message_font(a,b);}
#define fl_show_messages fltk3::message
inline int fl_show_choices(const char* c,int n,const char* b1,const char* b2,
			   const char* b3, int) {
  return fl_show_choice(0,c,0,n,b1,b2,b3);
}

#include "filename.H"
#include "Fl_File_Chooser.H"
inline int do_matching(char* a, const char* b) {return fl_filename_match(a,b);}

// Forms-compatable file chooser (implementation in fselect.C):
FL_EXPORT char* fl_show_file_selector(const char* message,const char* dir,
			    const char* pat,const char* fname);
FL_EXPORT char*	fl_get_directory();
FL_EXPORT char*	fl_get_pattern();
FL_EXPORT char*	fl_get_filename();

#include "Input.h"
forms_constructor(fltk3::Input, fl_add_input)
inline void fl_set_input(fltk3::Widget* o, const char* v) {
    ((fltk3::Input*)o)->value(v);}
inline void fl_set_input_return(fltk3::Widget* o, int x) {
    ((fltk3::Input*)o)->when((fltk3::When)(x | fltk3::WHEN_RELEASE));}
inline void fl_set_input_color(fltk3::Widget* o, Fl_Color a, Fl_Color b) {
    ((fltk3::Input*)o)->textcolor(a);
    ((fltk3::Input*)o)->cursor_color(b);
}
// inline void fl_set_input_scroll(fltk3::Widget*, int);
inline void fl_set_input_cursorpos(fltk3::Widget* o, int x, int /*y*/) {
  ((fltk3::Input*)o)->position(x);}
// inline void fl_set_input_selected(fltk3::Widget*, int);
// inline void fl_set_input_selected_range(fltk3::Widget*, int, int);
// inline void fl_set_input_maxchars(fltk3::Widget*, int);
// inline void fl_set_input_format(fltk3::Widget*, int, int);
// inline void fl_set_input_hscrollbar(fltk3::Widget*, int);
// inline void fl_set_input_vscrollbar(fltk3::Widget*, int);
// inline void fl_set_input_xoffset(fltk3::Widget*, int);
// inline void fl_set_input_topline(fltk3::Widget*, int);
// inline void fl_set_input_scrollbarsize(fltk3::Widget*, int, int);
// inline int fl_get_input_topline(fltk3::Widget*);
// inline int fl_get_input_screenlines(fltk3::Widget*);
inline int fl_get_input_cursorpos(fltk3::Widget* o, int*x, int*y) {
  *x = ((fltk3::Input*)o)->position(); *y = 0; return *x;}
// inline int fl_get_input_numberoflines(fltk3::Widget*);
// inline void fl_get_input_format(fltk3::Widget*, int*, int*);
inline const char* fl_get_input(fltk3::Widget* o) {return ((fltk3::Input*)o)->value();}

#include "Fl_Menu_Button.H"

// types are not implemented, they all act like FL_PUSH_MENU:
#define FL_TOUCH_MENU		0
#define FL_PUSH_MENU		1
#define FL_PULLDOWN_MENU	2
forms_constructor(Fl_Menu_Button, fl_add_menu)

inline void fl_clear_menu(fltk3::Widget* o) {
    ((Fl_Menu_Button*)o)->clear();}
inline void fl_set_menu(fltk3::Widget* o, const char* s) {
    ((Fl_Menu_Button*)o)->clear(); ((Fl_Menu_Button*)o)->add(s);}
inline void fl_addto_menu(fltk3::Widget* o, const char* s) {
    ((Fl_Menu_Button*)o)->add(s);}
inline void fl_replace_menu_item(fltk3::Widget* o, int i, const char* s) {
    ((Fl_Menu_Button*)o)->replace(i-1,s);}
inline void fl_delete_menu_item(fltk3::Widget* o, int i) {
    ((Fl_Menu_Button*)o)->remove(i-1);}
inline void fl_set_menu_item_shortcut(fltk3::Widget* o, int i, const char* s) {
    ((Fl_Menu_Button*)o)->shortcut(i-1,fl_old_shortcut(s));}
inline void fl_set_menu_item_mode(fltk3::Widget* o, int i, long x) {
    ((Fl_Menu_Button*)o)->mode(i-1,x);}
inline void fl_show_menu_symbol(fltk3::Widget*, int ) {
/*    ((Fl_Menu_Button*)o)->show_menu_symbol(i); */}
// inline void fl_set_menu_popup(fltk3::Widget*, int);
inline int fl_get_menu(fltk3::Widget* o) {
    return ((Fl_Menu_Button*)o)->value()+1;}
inline const char* fl_get_menu_item_text(fltk3::Widget* o, int i) {
    return ((Fl_Menu_Button*)o)->text(i);}
inline int fl_get_menu_maxitems(fltk3::Widget* o) {
    return ((Fl_Menu_Button*)o)->size();}
inline int fl_get_menu_item_mode(fltk3::Widget* o, int i) {
    return ((Fl_Menu_Button*)o)->mode(i);}
inline const char* fl_get_menu_text(fltk3::Widget* o) {
    return ((Fl_Menu_Button*)o)->text();}

#include "Fl_Positioner.H"
#define FL_NORMAL_POSITIONER	0
forms_constructor(Fl_Positioner, fl_add_positioner)
inline void fl_set_positioner_xvalue(fltk3::Widget* o, double v) {
    ((Fl_Positioner*)o)->xvalue(v);}
inline double fl_get_positioner_xvalue(fltk3::Widget* o) {
    return ((Fl_Positioner*)o)->xvalue();}
inline void fl_set_positioner_xbounds(fltk3::Widget* o, double a, double b) {
    ((Fl_Positioner*)o)->xbounds(a,b);}
inline void fl_get_positioner_xbounds(fltk3::Widget* o, float* a, float* b) {
  *a = float(((Fl_Positioner*)o)->xminimum());
  *b = float(((Fl_Positioner*)o)->xmaximum());
}
inline void fl_set_positioner_yvalue(fltk3::Widget* o, double v) {
    ((Fl_Positioner*)o)->yvalue(v);}
inline double fl_get_positioner_yvalue(fltk3::Widget* o) {
    return ((Fl_Positioner*)o)->yvalue();}
inline void fl_set_positioner_ybounds(fltk3::Widget* o, double a, double b) {
    ((Fl_Positioner*)o)->ybounds(a,b);}
inline void fl_get_positioner_ybounds(fltk3::Widget* o, float* a, float* b) {
  *a = float(((Fl_Positioner*)o)->yminimum());
  *b = float(((Fl_Positioner*)o)->ymaximum());
}
inline void fl_set_positioner_xstep(fltk3::Widget* o, double v) {
    ((Fl_Positioner*)o)->xstep(v);}
inline void fl_set_positioner_ystep(fltk3::Widget* o, double v) {
    ((Fl_Positioner*)o)->ystep(v);}
inline void fl_set_positioner_return(fltk3::Widget* o, int v) {
    ((Fl_Positioner*)o)->when((fltk3::When)(v|fltk3::WHEN_RELEASE));}

#include "Slider.h"

#define FL_HOR_BROWSER_SLIDER FL_HOR_SLIDER
#define FL_VERT_BROWSER_SLIDER FL_VERT_SLIDER

forms_constructort(fltk3::Slider, fl_add_slider)
#define FL_SLIDER_COL1 FL_GRAY
inline void fl_set_slider_value(fltk3::Widget* o, double v) {
    ((fltk3::Slider*)o)->value(v);}
inline double fl_get_slider_value(fltk3::Widget* o) {
    return ((fltk3::Slider*)o)->value();}
inline void fl_set_slider_bounds(fltk3::Widget* o, double a, double b) {
    ((fltk3::Slider*)o)->bounds(a, b);}
inline void fl_get_slider_bounds(fltk3::Widget* o, float* a, float* b) {
  *a = float(((fltk3::Slider*)o)->minimum());
  *b = float(((fltk3::Slider*)o)->maximum());
}
inline void fl_set_slider_return(fltk3::Widget* o, int i) {
    ((fltk3::Slider*)o)->when((fltk3::When)(i|fltk3::WHEN_RELEASE));}
inline void fl_set_slider_step(fltk3::Widget* o, double v) {
    ((fltk3::Slider*)o)->step(v);}
// inline void fl_set_slider_increment(fltk3::Widget* o, double v, double);
inline void fl_set_slider_size(fltk3::Widget* o, double v) {
    ((fltk3::Slider*)o)->slider_size(v);}

#include "ValueSlider.h"
forms_constructor(fltk3::ValueSlider, fl_add_valslider)

inline void fl_set_slider_precision(fltk3::Widget* o, int i) {
    ((fltk3::ValueSlider*)o)->precision(i);}
// filter function!

// The forms text object was the same as an Fl_Box except it inverted the
// meaning of fltk3::ALIGN_INSIDE.  Implementation in forms.cxx
class FL_EXPORT Fl_FormsText : public fltk3::Widget {
protected:
    void draw();
public:
    Fl_FormsText(fltk3::Boxtype b, int X, int Y, int W, int H, const char* l=0)
  : fltk3::Widget(X,Y,W,H,l) {box(b); align(fltk3::ALIGN_LEFT);}
};
#define FL_NORMAL_TEXT fltk3::NO_BOX
forms_constructorb(Fl_FormsText, fl_add_text)

#include "Fl_Timer.H"
forms_constructort(Fl_Timer, fl_add_timer)
inline void fl_set_timer(fltk3::Widget* o, double v) {((Fl_Timer*)o)->value(v);}
inline double fl_get_timer(fltk3::Widget* o) {return ((Fl_Timer*)o)->value();}
inline void fl_suspend_timer(fltk3::Widget* o) {((Fl_Timer*)o)->suspended(1);}
inline void fl_resume_timer(fltk3::Widget* o) {((Fl_Timer*)o)->suspended(0);}
inline void fl_set_timer_countup(fltk3::Widget* o,char d) {((Fl_Timer*)o)->direction(d);}
void fl_gettime(long* sec, long* usec);

// Fl_XYPlot nyi


// stuff from DDForms:

inline int fl_double_click() {return fltk3::event_clicks();}
inline void fl_draw() {fltk3::flush();}

#endif	/* define __FORMS_H__ */

//
// End of "$Id: forms.H 7365 2010-03-30 15:18:29Z matt $".
//
