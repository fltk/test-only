//
// "$Id: Widget.h,v 1.7 2003/03/09 07:51:36 spitzak Exp $"
//
// The base class of all widgets.
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#ifndef fltk_Widget_h
#define fltk_Widget_h

#include "Style.h"

namespace fltk {

class FL_API Widget;
class FL_API Window;
class FL_API Image;
class FL_API Group;
struct Cursor;

typedef void (Callback )(Widget*, void*);
typedef Callback* Callback_p; // needed for BORLAND
typedef void (Callback0)(Widget*);
typedef void (Callback1)(Widget*, long);

class FL_API Widget {
  // disable the copy assignment/constructors:
  Widget & operator=(const Widget &);
  Widget(const Widget &);

public:

  Widget(int,int,int,int,const char* =0);
  virtual ~Widget();

  virtual void draw();
  virtual int handle(int);
  int	send(int event);
  virtual void layout();

  const Style* style() const { return style_; }
  void	style(const Style* s) { style_ = s; }
  void	style(const Style& s) { style_ = &s; }
  bool	copy_style(const Style* s);
  static NamedStyle* default_style;
  static void default_glyph(const Widget*,int,int,int,int,int,Flags);

  Group* parent() const	{ return parent_; }
  void	parent(Group* w)	{ parent_ = w; }
  Window* window() const	;

  enum { // values for type():
    RESERVED_TYPE	= 0x64,
    GROUP_TYPE		= 0xe0,
    WINDOW_TYPE		= 0xf0
  };
  uchar	type() const		{ return type_; }
  void	type(uchar t)		{ type_ = t; }
  bool	is_group() const	{ return type_ >= GROUP_TYPE; }
  bool	is_window() const	{ return type_ >= WINDOW_TYPE; }

  int	x() const		{ return x_; }
  void	x(int v)		{ x_ = v; }
  int	y() const		{ return y_; }
  void	y(int v)		{ y_ = v; }
  int	w() const		{ return w_; }
  void	w(int v)		{ w_ = v; }
  int	width()			;
  int	h() const		{ return h_; }
  void	h(int v)		{ h_ = v; }
  int	height()		;
  bool	resize(int,int,int,int)	;
  bool	position(int X,int Y)	{ return resize(X,Y,w_,h_); }
  bool	size(int W,int H)	{ return resize(x_,y_,W,H); }

  const char* label() const	{ return label_; }
  void	label(const char* a);
  void	copy_label(const char* a);

  Image* image() const		{ return image_; }
  void	image(Image* a)		{ image_ = a; }
  void	image(Image& a)		{ image_ = &a; }

  const char *tooltip() const	{ return tooltip_; }
  void	tooltip(const char *t)	{ tooltip_ = t; }

  int	shortcut() const	{ return shortcut_; }
  void	shortcut(int s)		{ shortcut_ = s; }

  Callback_p callback() const	{ return callback_; }
  void	callback(Callback* c, void* p) { callback_=c; user_data_=p; }
  void	callback(Callback* c)	{ callback_=c; }
  void	callback(Callback0*c)	{ callback_=(Callback*)c; }
  void	callback(Callback1*c, long p=0) { callback_=(Callback*)c; user_data_=(void*)p; }
  void*	user_data() const	{ return user_data_; }
  void	user_data(void* v)	{ user_data_ = v; }
  long	argument() const	{ return (long)user_data_; }
  void	argument(long v)	{ user_data_ = (void*)v; }
  uchar when() const		{ return when_; }
  void	when(uchar i)		{ when_ = i; }

  static void default_callback(Widget*, void*);
  void	do_callback()		{ callback_(this,user_data_); }
  void	do_callback(Widget* o,void* arg=0) { callback_(o,arg); }
  void	do_callback(Widget* o,long arg) { callback_(o,(void*)arg); }
  bool	test_label_shortcut() const;
  bool	test_shortcut() const	;
  bool	contains(const Widget*) const;
  bool	inside(const Widget* o) const { return o && o->contains(this); }
  bool	pushed() const		;
  bool	focused() const		;
  bool	belowmouse() const	;

  Flags flags() const		{ return flags_; }
  Flags flags(Flags f)		{ return flags_ = f; }
  Flags set_flag(int c)		{ return flags_ |= c; }
  Flags clear_flag(int c)	{ return flags_ &= ~c; }
  Flags invert_flag(int c)	{ return flags_ ^= c; }

  Flags align() const		{ return flags_&ALIGN_MASK; }
  void	align(unsigned a)	{ flags_ = (flags_ & (~ALIGN_MASK)) | a; }
  bool	visible() const		{ return !(flags_&INVISIBLE); }
  bool	visible_r() const	;
  void	show()			;
  void	hide()			;
  void	set_visible()		{ flags_ &= ~INVISIBLE; }
  void	clear_visible()		{ flags_ |= INVISIBLE; }
  bool	active() const		{ return !(flags_&INACTIVE); }
  bool	active_r() const	;
  void	activate()		;
  void	activate(int b)		{ if (b) activate(); else deactivate(); }
  void	deactivate()		;
  bool	output() const		{ return (flags_&OUTPUT)!=0; }
  void	set_output()		{ flags_ |= OUTPUT; }
  void	clear_output()		{ flags_ &= ~OUTPUT; }
  bool	takesevents() const	{ return !(flags_&(OUTPUT|INVISIBLE|INACTIVE)); }
  bool	changed() const		{ return (flags_&CHANGED)!=0; }
  void	set_changed()		{ flags_ |= CHANGED; }
  void	clear_changed()		{ flags_ &= ~CHANGED; }
  bool	value() const		{ return (flags_&VALUE)!=0; }
  void	set_value()		{ flags_ |= VALUE; }
  void	clear_value()		{ flags_ &= ~VALUE; }
  bool	selected() const	{ return (flags_&SELECTED)!=0; }
  void	set_selected()		{ flags_ |= SELECTED; }
  void	clear_selected()	{ flags_ &= ~SELECTED; }
  bool	click_to_focus()	{ return (flags_ & CLICK_TO_FOCUS) != 0; }
  void  set_click_to_focus()	{ flags_ |= CLICK_TO_FOCUS; }
  void	clear_click_to_focus()	{ flags_ &= ~CLICK_TO_FOCUS; }

  bool	take_focus()		;
  void	throw_focus()		;

  void	redraw()		;
  void	redraw(uchar c)		;
  void	redraw_label()		;
  void	redraw(int,int,int,int)	;
  uchar	damage() const		{ return damage_; }
  void	set_damage(uchar c)	{ damage_ = c; } // should be called damage(c)

  void  relayout()		;
  void	relayout(uchar damage)	;
  uchar layout_damage() const	{ return layout_damage_; }
  void	layout_damage(uchar c)	{ layout_damage_ = c; }

  void	make_current() const	;
  void	add_timeout(float)	;
  void	repeat_timeout(float)	;
  void  remove_timeout()	;

  void  draw_frame() const	;
  void  draw_box() const	;
  void  draw_glyph(int t, int x,int y,int w,int h, Flags f) const
    { glyph()(this,t,x,y,w,h,f); }
  void  draw_label(int x,int y,int w,int h, Color, Flags) const ;
  void  draw_inside_label(int x,int y,int w,int h) const ;
  void	draw_inside_label() const;
  void	draw_background() const	;
  void	cursor(Cursor*) const	;

  void	measure_label(int&, int&) const ;

  Box*	box()			const;
  Box*	buttonbox()		const;
  Box*	focusbox()		const;
  GlyphStyle glyph()		const;
  Font*	labelfont()		const;
  Font*	textfont()		const;
  LabelType* labeltype()	const;
  Color	color()			const;
  Color	textcolor()		const;
  Color	selection_color()	const;
  Color	selection_textcolor()	const;
  Color	buttoncolor()		const;
  Color	labelcolor()		const;
  Color	highlight_color()	const;
  Color	highlight_labelcolor()	const;
  float labelsize()		const;
  float textsize()		const;
  float leading()		const;
  unsigned char scrollbar_align() const;
  unsigned char scrollbar_width() const;

  void box(Box*)		;
  void buttonbox(Box*)		;
  void focusbox(Box*)		;
  void glyph(GlyphStyle)	;
  void labelfont(Font*)		;
  void textfont(Font*)		;
  void labeltype(LabelType*)	;
  void color(Color)		;
  void textcolor(Color a)	;
  void selection_color(Color)	;
  void selection_textcolor(Color);
  void buttoncolor(Color)	;
  void labelcolor(Color)	;
  void highlight_color(Color)	;
  void highlight_labelcolor(Color);
  void labelsize(float a)	;
  void textsize(float a)	;
  void leading(float a)		;
  void scrollbar_align(unsigned char);
  void scrollbar_width(unsigned char);

#ifdef FLTK_1_WIDGET  // back-compatability section:

  Box*	down_box()		const { return box(); }
  Box*	slider()		const { return buttonbox(); }
  Box*	box2()			const { return box(); }
  Box*	fly_box()		const { return box(); }
  Color	color2()		const { return selection_color(); }
  Color	color3()		const { return buttoncolor(); }
  Color	down_labelcolor()	const { return selection_textcolor(); }
  Color	fly_color()		const { return highlight_color(); }
  Color	selected_textcolor()	const { return selection_textcolor(); }
  Color	cursor_color()		const { return selection_color(); }

  void down_box(Box* a)		{ box(a); }
  void slider(Box* a)		{ buttonbox(a); }
  void fly_box(Box*)		{ }
  void color(Color a, Color b)	{ color(a); selection_color(b); }
  void color2(Color a)		{ selection_color(a); }
  void color3(Color a)		{ buttoncolor(a); }
  void down_labelcolor(Color a)	{ selection_textcolor(a); }
  void labelfont(unsigned a)	{ labelfont(font(a)); }
  void fly_color(Color a)	{ highlight_color(a); }
  void textfont(unsigned a)	{ textfont(font(a)); }
  void selected_textcolor(Color a) { selection_textcolor(a); }
  void cursor_color(Color a)	{ selection_color(a); }

#endif

private:

  const char*		label_;
  Image*		image_;
  int			shortcut_; // encode in the label?
  unsigned		flags_;
  const Style*	style_;
  Callback*		callback_;
  void*			user_data_;
  const char*		tooltip_; // make this into another widget?
  Group*		parent_;
  int			x_,y_,w_,h_;
  uchar			type_;
  uchar			damage_;
  uchar			layout_damage_;
  uchar			when_;

};

enum { // Widget::when() values
  WHEN_NEVER		= 0,
  WHEN_CHANGED		= 1,
  WHEN_RELEASE		= 4,
  WHEN_RELEASE_ALWAYS	= 6,
  WHEN_ENTER_KEY	= 8,
  WHEN_ENTER_KEY_ALWAYS	=10,
  WHEN_ENTER_KEY_CHANGED=11,
  WHEN_NOT_CHANGED	= 2 // modifier bit to disable changed() test
};

}

#endif

//
// End of "$Id: Widget.h,v 1.7 2003/03/09 07:51:36 spitzak Exp $".
//
