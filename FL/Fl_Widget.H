//
// "$Id: Fl_Widget.H,v 1.42 1999/12/17 20:18:06 bill Exp $"
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

#include "Fl_Style.H"

class Fl_Widget;
class Fl_Window;
class Fl_Image;
class Fl_Group;

typedef void (Fl_Callback )(Fl_Widget*, void*);
typedef Fl_Callback* Fl_Callback_p; // needed for BORLAND
typedef void (Fl_Callback0)(Fl_Widget*);
typedef void (Fl_Callback1)(Fl_Widget*, long);

class FL_API Fl_Widget {
  // disable the copy assignment/constructors:
  Fl_Widget & operator=(const Fl_Widget &);
  Fl_Widget(const Fl_Widget &);

public:

  virtual void draw() = 0;
  virtual void draw_n_clip();
  virtual int handle(int);
  virtual void layout();
  virtual ~Fl_Widget();

  const Fl_Style* style() const {return style_;}
  void	style(const Fl_Style* s) {style_ = s;}
  void	style(const Fl_Style& s) {style_ = &s;}
  int	copy_style(const Fl_Style* s);
  static Fl_Style* default_style;

  Fl_Group* parent() const	{return parent_;}
  void	parent(Fl_Group* w)	{parent_ = w;}
  Fl_Window* window() const	;

  uchar	type() const		{return type_;}
  void	type(uchar t)		{type_ = t;}

  int	x() const		{return x_;}
  void	x(int v)		{x_ = v;}
  int	y() const		{return y_;}
  void	y(int v)		{y_ = v;}
  int	w() const		{return w_;}
  void	w(int v)		{w_ = v;}
  int	h() const		{return h_;}
  void	h(int v)		{h_ = v;}
  void	resize(int,int,int,int)	;
  int	damage_resize(int,int,int,int);
  void	position(int X,int Y)	{resize(X,Y,w_,h_);}
  void	size(int W,int H)	{resize(x_,y_,W,H);}

  const char* label() const	{return label_;}
  void	label(const char* a)	{label_=a;}
  void	label(const char *a, const char *loc);

  Fl_Image* image() const	{return image_;}
  void	image(Fl_Image* a)	{image_ = a;}
  void	image(Fl_Image& a)	{image_ = &a;}

  const char *tooltip() const	{return tooltip_; }
  void	tooltip(const char *t)	{tooltip_ = t;}
  void	tooltip(const char *t, const char *loc);

  Fl_Callback_p callback() const {return callback_;}
  void	callback(Fl_Callback* c, void* p) {callback_=c; user_data_=p;}
  void	callback(Fl_Callback* c) {callback_=c;}
  void	callback(Fl_Callback0*c) {callback_=(Fl_Callback*)c;}
  void	callback(Fl_Callback1*c, long p=0) {callback_=(Fl_Callback*)c; user_data_=(void*)p;}
  void*	user_data() const	{return user_data_;}
  void	user_data(void* v)	{user_data_ = v;}
  long	argument() const	{return (long)user_data_;}
  void	argument(long v)	{user_data_ = (void*)v;}
  Fl_When when() const		{return (Fl_When)when_;}
  void	when(uchar i)		{when_ = i;}

  static void default_callback(Fl_Widget*, void*);
  void	do_callback()		{callback_(this,user_data_);}
  void	do_callback(Fl_Widget* o,void* arg=0) {callback_(o,arg);}
  void	do_callback(Fl_Widget* o,long arg) {callback_(o,(void*)arg);}
  int	test_shortcut()		;
  static int test_shortcut(const char*);
  int	contains(const Fl_Widget*) const;
  int	inside(const Fl_Widget* o) const {return o ? o->contains(this) : 0;}

  Fl_Flags flags() const	{return flags_;}
  void	set_flag(int c)		{flags_ |= c;}
  void	clear_flag(int c)	{flags_ &= ~c;}

  int	visible() const		{return !(flags_&FL_INVISIBLE);}
  int	visible_r() const	;
  void	show()			;
  void	hide()			;
  void	set_visible()		{flags_ &= ~FL_INVISIBLE;}
  void	clear_visible()		{flags_ |= FL_INVISIBLE;}
  int	active() const		{return !(flags_&FL_INACTIVE);}
  int	active_r() const	;
  void	activate()		;
  void	activate(int b)		{if (b) activate(); else deactivate();}
  void	deactivate()		;
  int	output() const		{return (flags_&FL_OUTPUT);}
  void	set_output()		{flags_ |= FL_OUTPUT;}
  void	clear_output()		{flags_ &= ~FL_OUTPUT;}
  int	takesevents() const	{return (!output()&&active_r()&&visible_r());}
  int	changed() const		{return flags_&FL_CHANGED;}
  void	set_changed()		{flags_ |= FL_CHANGED;}
  void	clear_changed()		{flags_ &= ~FL_CHANGED;}
  int	value() const		{return (flags_&FL_VALUE)!=0;}
  void	set_value()		{flags_ |= FL_VALUE;}
  void	clear_value()		{flags_ &= ~FL_VALUE;}
  int	value(int)		;
  int	set()			;
  int	clear()			;
  void	setonly()		;

  int	take_focus()		;
  void	throw_focus()		;

  void	redraw()		;
  uchar	damage() const		{return damage_;}
  void	clear_damage(uchar c=0)	{damage_ = c;}
  void	damage(uchar c)		;
  void	damage(uchar c,int,int,int,int);

  void	draw_box() const	;
  void	draw_frame() const	;
  void	draw_label() const	;
  void	draw_label(Fl_Color) const;
  void	draw_glyph(int, int,int,int,int, Fl_Flags) const;
  Fl_Color draw_button() const	;
  void	draw_button_label(int,int,int,int,Fl_Color) const;
  void	draw_label(int,int,int,int, Fl_Flags) const;
  void	draw_label(int,int,int,int, Fl_Color, Fl_Flags) const;
  void	measure_label(int&, int&) const;

  Fl_Boxtype	box()			const;
  Fl_Boxtype	glyph_box()		const;
  Fl_Glyph	glyph()			const;
  Fl_Font	label_font()		const;
  Fl_Font	text_font()		const;
  Fl_Labeltype	label_type()		const;
  Fl_Color	color()			const;
  Fl_Color	label_color()		const;
  Fl_Color	selection_color()	const;
  Fl_Color	selection_text_color()	const;
  Fl_Color	off_color()		const;
  Fl_Color	on_color()		const {return selection_color();}
  Fl_Color	up_color()		const {return color();}
  Fl_Color	down_color()		const {return selection_color();}
  Fl_Color	highlight_color()	const;
  Fl_Color	highlight_label_color()	const;
  Fl_Color	text_color()		const;
  unsigned	label_size()		const;
  unsigned	text_size()		const;
  unsigned	leading()		const;

  void	box(Fl_Boxtype)		;
  void	glyph_box(Fl_Boxtype)	;
  void	glyph(Fl_Glyph)		;
  void	label_font(Fl_Font)	;
  void	text_font(Fl_Font)	;
  void	label_type(Fl_Labeltype);
  void	color(Fl_Color)		;
  void	label_color(Fl_Color)	;
  void	selection_color(Fl_Color);
  void	selection_text_color(Fl_Color);
  void	off_color(Fl_Color)	;
  void	on_color(Fl_Color a)	{selection_color(a);}
  void	up_color(Fl_Color a)	{color(a);}
  void	down_color(Fl_Color a)	{selection_color(a);}
  void	highlight_color(Fl_Color);
  void	highlight_label_color(Fl_Color);
  void	text_color(Fl_Color a)	;
  void	label_size(unsigned a)	;
  void	text_size(unsigned a)	;
  void	leading(unsigned a)	;

#ifndef FLTK_2  // back-compatability section:

  Fl_Flags align() const	{return flags_;}
  void	align(unsigned a)	{flags_ = flags_&~FL_ALIGN_MASK|a;}

  Fl_Boxtype	down_box()		const {return glyph_box();}
  Fl_Boxtype	slider()		const {return glyph_box();}
  Fl_Boxtype	box2()			const {return glyph_box();}
  Fl_Boxtype	fly_box()		const {return box();}
  Fl_Color	color2()		const {return selection_color();}
  Fl_Color	color3()		const {return off_color();}
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
  Fl_Color	cursor_color()		const {return off_color();}

  void down_box(Fl_Boxtype a)           {glyph_box(a);}
  void slider(Fl_Boxtype a)             {glyph_box(a);}
  void fly_box(Fl_Boxtype)              {}
  void color2(Fl_Color a)               {selection_color(a);}
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
  void cursor_color(Fl_Color a)         {off_color(a);}

#endif

protected:

  Fl_Widget(int,int,int,int,const char* =0);

private:

  const Fl_Style*	style_;
  Fl_Group*		parent_;
  Fl_Callback*		callback_;
  void*			user_data_;
  const char*		label_;
  Fl_Image*		image_;
  const char*		tooltip_;
  unsigned		flags_;
  int			x_,y_,w_,h_;
  uchar			type_;
  uchar			damage_;
  uchar			when_;

};

// reserved type numbers (necessary for my cheapo RTTI) start here.
// grep the header files for "RESERVED_TYPE" to find the next available
// number.
#define FL_RESERVED_TYPE 100

#endif

//
// End of "$Id: Fl_Widget.H,v 1.42 1999/12/17 20:18:06 bill Exp $".
//
