//
// "$Id: Fl_Widget.h,v 1.12 2002/03/10 23:10:23 spitzak Exp $"
//
// Widget header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef Fl_Widget_H
#define Fl_Widget_H

#include "Fl_Style.h"

class FL_API Fl_Widget;
class FL_API Fl_Window;
class FL_API Fl_Image;
class FL_API Fl_Group;

typedef void (Fl_Callback )(Fl_Widget*, void*);
typedef Fl_Callback* Fl_Callback_p; // needed for BORLAND
typedef void (Fl_Callback0)(Fl_Widget*);
typedef void (Fl_Callback1)(Fl_Widget*, long);

class FL_API Fl_Widget {
  // disable the copy assignment/constructors:
  Fl_Widget & operator=(const Fl_Widget &);
  Fl_Widget(const Fl_Widget &);

public:

  enum { // values for type():
    RESERVED_TYPE	= 0x64,
    GROUP_TYPE		= 0xe0,
    WINDOW_TYPE		= 0xf0
  };

  virtual void draw();
  virtual int handle(int);
  virtual void layout();
  virtual ~Fl_Widget();

  const Fl_Style* style() const {return style_;}
  void	style(const Fl_Style* s) {style_ = s;}
  void	style(const Fl_Style& s) {style_ = &s;}
  bool	copy_style(const Fl_Style* s);
  static Fl_Named_Style* default_style;
  static void default_glyph(const Fl_Widget*,int,int,int,int,int,Fl_Flags);

  Fl_Group* parent() const	{return parent_;}
  void	parent(Fl_Group* w)	{parent_ = w;}
  Fl_Window* window() const	;

  uchar	type() const		{return type_;}
  void	type(uchar t)		{type_ = t;}
  bool	is_group() const	{return type_ >= GROUP_TYPE;}
  bool	is_window() const	{return type_ >= WINDOW_TYPE;}

  int	x() const		{return x_;}
  void	x(int v)		{x_ = v;}
  int	y() const		{return y_;}
  void	y(int v)		{y_ = v;}
  int	w() const		{return w_;}
  void	w(int v)		{w_ = v;}
  int	width();
  int	h() const		{return h_;}
  void	h(int v)		{h_ = v;}
  int	height();
  bool	resize(int,int,int,int)	;
  bool	position(int X,int Y)	{return resize(X,Y,w_,h_);}
  bool	size(int W,int H)	{return resize(x_,y_,W,H);}

  const char* label() const	{return label_;}
  void	label(const char* a);
  void	copy_label(const char* a);

  Fl_Image* image() const	{return image_;}
  void	image(Fl_Image* a)	{image_ = a;}
  void	image(Fl_Image& a)	{image_ = &a;}

  const char *tooltip() const	{return tooltip_; }
  void	tooltip(const char *t);

  int shortcut() const		{return shortcut_;}
  void shortcut(int s)		{shortcut_ = s;}

  Fl_Callback_p callback() const {return callback_;}
  void	callback(Fl_Callback* c, void* p) {callback_=c; user_data_=p;}
  void	callback(Fl_Callback* c) {callback_=c;}
  void	callback(Fl_Callback0*c) {callback_=(Fl_Callback*)c;}
  void	callback(Fl_Callback1*c, long p=0) {callback_=(Fl_Callback*)c; user_data_=(void*)p;}
  void*	user_data() const	{return user_data_;}
  void	user_data(void* v)	{user_data_ = v;}
  long	argument() const	{return (long)user_data_;}
  void	argument(long v)	{user_data_ = (void*)v;}
  uchar when() const		{return when_;}
  void	when(uchar i)		{when_ = i;}

  static void default_callback(Fl_Widget*, void*);
  void	do_callback()		{callback_(this,user_data_);}
  void	do_callback(Fl_Widget* o,void* arg=0) {callback_(o,arg);}
  void	do_callback(Fl_Widget* o,long arg) {callback_(o,(void*)arg);}
  int	test_shortcut() const	;
  bool	contains(const Fl_Widget*) const;
  bool	inside(const Fl_Widget* o) const {return o && o->contains(this);}
  bool	pushed() const		;
  bool	focused() const		;
  bool	belowmouse() const	;

  Fl_Flags flags() const	{return flags_;}
  Fl_Flags flags(Fl_Flags f)	{return flags_ = f;}
  Fl_Flags set_flag(int c)	{return flags_ |= c;}
  Fl_Flags clear_flag(int c)	{return flags_ &= ~c;}
  Fl_Flags invert_flag(int c)	{return flags_ ^= c;}

  Fl_Flags align() const	{return flags_&FL_ALIGN_MASK;}
  void	align(unsigned a)	{flags_ = (flags_ & (~FL_ALIGN_MASK)) | a;}
  bool	visible() const		{return !(flags_&FL_INVISIBLE);}
  bool	visible_r() const	;
  void	show()			;
  void	hide()			;
  void	set_visible()		{flags_ &= ~FL_INVISIBLE;}
  void	clear_visible()		{flags_ |= FL_INVISIBLE;}
  bool	active() const		{return !(flags_&FL_INACTIVE);}
  bool	active_r() const	;
  void	activate()		;
  void	activate(int b)		{if (b) activate(); else deactivate();}
  void	deactivate()		;
  bool	output() const		{return (flags_&FL_OUTPUT)!=0;}
  void	set_output()		{flags_ |= FL_OUTPUT;}
  void	clear_output()		{flags_ &= ~FL_OUTPUT;}
  bool	takesevents() const	{return !(flags_&(FL_OUTPUT|FL_INVISIBLE|FL_INACTIVE));}
  bool	changed() const		{return (flags_&FL_CHANGED)!=0;}
  void	set_changed()		{flags_ |= FL_CHANGED;}
  void	clear_changed()		{flags_ &= ~FL_CHANGED;}
  bool	value() const		{return (flags_&FL_VALUE)!=0;}
  void	set_value()		{flags_ |= FL_VALUE;}
  void	clear_value()		{flags_ &= ~FL_VALUE;}
  bool	selected() const	{return (flags_&FL_SELECTED)!=0;}
  void	set_selected()		{flags_ |= FL_SELECTED;}
  void	clear_selected()	{flags_ &= ~FL_SELECTED;}

  bool	take_focus()		;
  void	throw_focus()		;

  void	redraw()		;
  void	redraw(uchar c)		;
  void	redraw_label()		;
  void	redraw(int,int,int,int);
  uchar	damage() const		{return damage_;}
  void	set_damage(uchar c)	{damage_ = c;} // should be called damage(c)

  void  relayout()		;
  void	relayout(uchar damage)	;
  uchar layout_damage() const	{return layout_damage_;}
  void	layout_damage(uchar c)	{layout_damage_ = c;}

  void	make_current() const	;

  void  draw_frame() const ;
  void  draw_box() const	;
  void  draw_glyph(int t, int x,int y,int w,int h, Fl_Flags f) const
    { glyph()(this,t,x,y,w,h,f); }
  void  draw_label(int x,int y,int w,int h, Fl_Flags) const ;
  void  draw_inside_label(int x,int y,int w,int h, Fl_Flags) const ;
  void	draw_inside_label() const;

  void	measure_label(int&, int&) const ;

  Fl_Boxtype	box()			const;
  Fl_Boxtype	button_box()		const;
  Fl_Boxtype	focus_box()		const;
  Fl_Glyph	glyph()			const;
  Fl_Font	label_font()		const;
  Fl_Font	text_font()		const;
  Fl_Labeltype	label_type()		const;
  Fl_Color	color()			const;
  Fl_Color	label_color()		const;
  Fl_Color	selection_color()	const;
  Fl_Color	selection_text_color()	const;
  Fl_Color	button_color()		const;
  Fl_Color	highlight_color()	const;
  Fl_Color	highlight_label_color()	const;
  Fl_Color	text_color()		const;
  unsigned	label_size()		const;
  unsigned	text_size()		const;
  unsigned	leading()		const;
  Fl_Flags	scrollbar_align() const {return style()->scrollbar_align;}
  unsigned	scrollbar_width() const {return style()->scrollbar_width;}

  void box(Fl_Boxtype)		;
  void button_box(Fl_Boxtype)	;
  void focus_box(Fl_Boxtype)	;
  void glyph(Fl_Glyph)		;
  void label_font(Fl_Font)	;
  void text_font(Fl_Font)	;
  void label_type(Fl_Labeltype);
  void color(Fl_Color)		;
  void label_color(Fl_Color)	;
  void selection_color(Fl_Color);
  void selection_text_color(Fl_Color);
  void button_color(Fl_Color)	;
  void highlight_color(Fl_Color);
  void highlight_label_color(Fl_Color);
  void text_color(Fl_Color a)	;
  void label_size(unsigned a)	;
  void text_size(unsigned a)	;
  void leading(unsigned a)	;

#ifndef FLTK_2  // back-compatability section:

  Fl_Boxtype	down_box()		const {return box();}
  Fl_Boxtype	slider()		const {return button_box();}
  Fl_Boxtype	box2()			const {return box();}
  Fl_Boxtype	fly_box()		const {return box();}
  Fl_Color	color2()		const {return selection_color();}
  Fl_Color	color3()		const {return button_color();}
  Fl_Labeltype	labeltype()		const {return label_type();}
  Fl_Color	labelcolor()		const {return label_color();}
  Fl_Color	down_labelcolor()	const {return selection_text_color();}
  Fl_Font	labelfont()		const {return label_font();}
  unsigned	labelsize()		const {return label_size();}
  Fl_Color	fly_color()		const {return highlight_color();}
  Fl_Font	textfont()		const {return text_font();}
  unsigned	textsize()		const {return text_size();}
  Fl_Color	textcolor()		const {return text_color();}
  Fl_Color	selected_textcolor()	const {return selection_text_color();}
  Fl_Color	cursor_color()		const {return selection_color();}

  void down_box(Fl_Boxtype a)           {box(a);}
  void slider(Fl_Boxtype a)             {button_box(a);}
  void fly_box(Fl_Boxtype)              {}
  void color(Fl_Color a, Fl_Color b)    {color(a); selection_color(b);}
  void color2(Fl_Color a)               {selection_color(a);}
  void color3(Fl_Color a)               {button_color(a);}
  void labeltype(Fl_Labeltype a)        {label_type(a);}
  void labelcolor(Fl_Color a)           {label_color(a);}
  void down_labelcolor(Fl_Color a)      {selection_text_color(a);}
  void labelfont(Fl_Font a)             {label_font(a);}
  void labelfont(unsigned a)            {label_font(fl_fonts+a);}
  void labelsize(unsigned a)            {label_size(a);}
  void fly_color(Fl_Color a)            {highlight_color(a);}
  void textfont(Fl_Font a)              {text_font(a);}
  void textfont(unsigned a)             {text_font(fl_fonts+a);}
  void textsize(unsigned a)             {text_size(a);}
  void textcolor(Fl_Color a)            {text_color(a);}
  void selected_textcolor(Fl_Color a)   {selection_text_color(a);}
  void cursor_color(Fl_Color a)         {selection_color(a);}

#endif

protected:

  Fl_Widget(int,int,int,int,const char* =0);

private:

  const char*		label_;
  Fl_Image*		image_;
  int			shortcut_; // encode in the label?
  unsigned		flags_;
  const Fl_Style*	style_;
  Fl_Callback*		callback_;
  void*			user_data_;
  const char*		tooltip_; // make this into another widget?
  Fl_Group*		parent_;
  int			x_,y_,w_,h_;
  uchar			type_;
  uchar			damage_;
  uchar			layout_damage_;
  uchar			when_;

};

#endif

//
// End of "$Id: Fl_Widget.h,v 1.12 2002/03/10 23:10:23 spitzak Exp $".
//
