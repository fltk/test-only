//
// "$Id: Fl_Widget.H,v 1.27 1999/09/20 16:36:46 vincent Exp $"
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

typedef void (Fl_Callback )(Fl_Widget*, void*);
typedef Fl_Callback* Fl_Callback_p; // needed for BORLAND
typedef void (Fl_Callback0)(Fl_Widget*);
typedef void (Fl_Callback1)(Fl_Widget*, long);

class Fl_Widget {
  friend class Fl_Group;

public:

  virtual void draw() = 0;
  virtual int handle(int);
  virtual void layout();
  virtual ~Fl_Widget();

  const Fl_Style* style() const {return style_;}
  int style(Fl_Style* s);
  int style(Fl_Style* s, Fl_Style* p);
  int style(Fl_Style& s) {return style(&s);}
  int copy_style(const Fl_Style* s);
  FL_IMPORT static Fl_Style default_style;

  Fl_Group* parent() const {return parent_;}
  Fl_Window* window() const ;
  void parent(const Fl_Widget* w) {parent_=(Fl_Group*)w;} // for hacks only

  uchar type() const {return type_;}
  void type(uchar t) {type_ = t;}

  int x() const {return x_;}
  int y() const {return y_;}
  int w() const {return w_;}
  int h() const {return h_;}
  void resize(int,int,int,int);
  int damage_resize(int,int,int,int);
  void position(int X,int Y) {resize(X,Y,w_,h_);}
  void size(int W,int H) {resize(x_,y_,W,H);}

  const char* label() const {return label_;}
  void label(const char* a) {label_=a;}
  void label(const char *a, const char *loc);

  Fl_Image* image() const {return image_;}
  void image(Fl_Image* a) {image_ = a;}
  void image(Fl_Image& a) {image_ = &a;}

  const char *tooltip() const {return tooltip_; }
  void tooltip(const char *t) {tooltip_ = t;}
  void tooltip(const char *t, const char *loc);

  Fl_Callback_p callback() const {return callback_;}
  void callback(Fl_Callback* c, void* p) {callback_=c; user_data_=p;}
  void callback(Fl_Callback* c) {callback_=c;}
  void callback(Fl_Callback0*c) {callback_=(Fl_Callback*)c;}
  void callback(Fl_Callback1*c, long p=0) {callback_=(Fl_Callback*)c; user_data_=(void*)p;}
  void* user_data() const {return user_data_;}
  void user_data(void* v) {user_data_ = v;}
  long argument() const {return (long)user_data_;}
  void argument(long v) {user_data_ = (void*)v;}
  Fl_When when() const {return (Fl_When)when_;}
  void when(uchar i) {when_ = i;}

  FL_IMPORT static void default_callback(Fl_Widget*, void*);
  void do_callback() {callback_(this,user_data_);}
  void do_callback(Fl_Widget* o,void* arg=0) {callback_(o,arg);}
  void do_callback(Fl_Widget* o,long arg) {callback_(o,(void*)arg);}
  int test_shortcut();
  FL_IMPORT static int test_shortcut(const char*);
  int contains(const Fl_Widget*) const ;
  int inside(const Fl_Widget* o) const {return o ? o->contains(this) : 0;}

  Fl_Flags flags() const {return flags_;}
  Fl_Flags align() const {return flags_;}
  void align(unsigned a) {flags_ = flags_&~FL_ALIGN_MASK|a;}

  int visible() const {return !(flags_&FL_INVISIBLE);}
  int visible_r() const;
  void show();
  void hide();
  void set_visible() {flags_ &= ~FL_INVISIBLE;}
  void clear_visible() {flags_ |= FL_INVISIBLE;}
  int active() const {return !(flags_&FL_INACTIVE);}
  int active_r() const;
  void activate();
  void activate(int b) { if (b) activate(); else deactivate(); }
  void deactivate();
  int output() const {return (flags_&FL_OUTPUT);}
  void set_output() {flags_ |= FL_OUTPUT;}
  void clear_output() {flags_ &= ~FL_OUTPUT;}
  int takesevents() const {return !(flags_&(FL_OUTPUT|FL_INVISIBLE|FL_INACTIVE));}
  int changed() const {return flags_&FL_CHANGED;}
  void set_changed() {flags_ |= FL_CHANGED;}
  void clear_changed() {flags_ &= ~FL_CHANGED;}
  int value() const {return flags_&FL_VALUE;}
  void set_value() {flags_ |= FL_VALUE;}
  void clear_value() {flags_ &= ~FL_VALUE;}
  int value(int);
  int set();
  int clear();
  void setonly();

  int take_focus();
  void throw_focus();

  void redraw();
  uchar damage() const {return damage_;}
  void clear_damage(uchar c = 0) {damage_ = c;}
  void damage(uchar c);
  void damage(uchar c,int,int,int,int);

  void draw_label(int,int,int,int, Fl_Flags) const; // draw label anywhere
  void draw_label(int,int,int,int, Fl_Color, Fl_Flags) const;
  void measure_label(int&, int&) const;

  Fl_Boxtype box()		const {return style_->box;}
  Fl_Glyph glyph()		const {return style_->glyph;}
  Fl_Font label_font()		const {return style_->label_font;}
  Fl_Font text_font()		const {return style_->text_font;}
  Fl_Labeltype label_type()	const {return style_->label_type;}
  Fl_Color color()		const {return style_->color;}
  Fl_Color up_color()		const {return style_->color;}
  Fl_Color label_color()	const {return style_->label_color;}
  Fl_Color selection_color()	const {return style_->selection_color;}
  Fl_Color selection_text_color() const {return style_->selection_text_color;}
  Fl_Color on_color()		const {return style_->selection_color;}
  Fl_Color down_color()		const {return style_->selection_color;}
  Fl_Color off_color()		const {return style_->off_color;}
  Fl_Color highlight_color()	const {return style_->highlight_color;}
  Fl_Color highlight_label_color() const{return style_->highlight_label_color;}
  Fl_Color text_color()		const {return style_->text_color;}
  unsigned label_size()		const {return style_->label_size;}
  unsigned text_size()		const {return style_->text_size;}

  void box(Fl_Boxtype a)	{setp((const void**)&style_->box, a);}
  void glyph(Fl_Glyph a)	{setp((const void**)&style_->glyph, (const void*)a);}
  void label_font(Fl_Font a)	{setp((const void**)&style_->label_font, a);}
  void text_font(Fl_Font a)	{setp((const void**)&style_->text_font, a);}
  void label_type(Fl_Labeltype a) {setp((const void**)&style_->label_type, a);}
  void color(Fl_Color a)	{seti(&style_->color, a);}
  void up_color(Fl_Color a)	{seti(&style_->color, a);}
  void label_color(Fl_Color a)	{seti(&style_->label_color, a);}
  void selection_color(Fl_Color a) {seti(&style_->selection_color, a);}
  void selection_text_color(Fl_Color a) {seti(&style_->selection_text_color, a);}
  void on_color(Fl_Color a)	{seti(&style_->selection_color, a);}
  void down_color(Fl_Color a)	{seti(&style_->selection_color, a);}
  void off_color(Fl_Color a)	{seti(&style_->off_color, a);}
  void highlight_color(Fl_Color a) {seti(&style_->highlight_color, a);}
  void highlight_label_color(Fl_Color a) {seti(&style_->highlight_label_color, a);}
  void text_color(Fl_Color a)	{seti(&style_->text_color, a);}
  void label_size(unsigned a)	{seti(&style_->label_size, a);}
  void text_size(unsigned a)	{seti(&style_->text_size, a);}

#ifndef FLTK_2  // back-compatability section:
  Fl_Boxtype down_box()		const {return FL_DOWN_BOX;}
  Fl_Boxtype fly_box()		const {return style_->box;}
  Fl_Color color2()		const {return style_->selection_color;}
  Fl_Color color3()		const {return style_->off_color;}
  Fl_Labeltype labeltype()	const {return style_->label_type;}
  Fl_Color labelcolor()		const {return style_->label_color;}
  Fl_Color down_labelcolor()	const {return style_->selection_text_color;}
  Fl_Font labelfont()		const {return style_->label_font;}
  unsigned labelsize()		const {return style_->label_size;}
  Fl_Color fly_color()		const {return style_->highlight_color;}
  Fl_Font textfont()		const {return style_->text_font;}
  unsigned textsize()		const {return style_->text_size;}
  Fl_Color textcolor()		const {return style_->text_color;}
  Fl_Color selected_textcolor()	const {return style_->selection_text_color;}
  Fl_Color cursor_color()	const {return style_->off_color;}

  void down_box(Fl_Boxtype)	{}
  void fly_box(Fl_Boxtype)	{}
  void color2(Fl_Color a)	{seti(&style_->selection_color, a);}
  void labeltype(Fl_Labeltype a) {setp((const void**)&style_->label_type, a);}
  void labelcolor(Fl_Color a)	{seti(&style_->label_color, a);}
  void down_labelcolor(Fl_Color a) {seti(&style_->selection_text_color, a);}
  void labelfont(Fl_Font a)	{setp((const void**)&style_->label_font, a);}
  void labelfont(unsigned a)	{setp((const void**)&style_->label_font, fl_fonts+a);}
  void labelsize(unsigned a)	{seti(&style_->label_size, a);}
  void fly_color(Fl_Color a)	{seti(&style_->highlight_color, a);}
  void textfont(Fl_Font a)	{setp((const void**)&style_->text_font, a);}
  void textfont(unsigned a)	{setp((const void**)&style_->text_font, fl_fonts+a);}
  void textsize(unsigned a)	{seti(&style_->text_size, a);}
  void textcolor(Fl_Color a)	{seti(&style_->text_color, a);}
  void selected_textcolor(Fl_Color a) {seti(&style_->selection_text_color, a);}
  void cursor_color(Fl_Color a)	{seti(&style_->off_color, a);}
#endif

protected:

  Fl_Widget(int,int,int,int,const char* =0);

  void x(int v) {x_ = v;}
  void y(int v) {y_ = v;}
  void w(int v) {w_ = v;}
  void h(int v) {h_ = v;}

  void set_flag(unsigned c) {flags_ |= c;}
  void clear_flag(unsigned c) {flags_ &= ~c;}

  void draw_box() const; // draws the style's box around widget and fills
  void draw_frame() const; // just draws the edge of the box if possible
  void draw_label() const; // draw inside labels only
  void draw_label(Fl_Color) const;
  void draw_glyph(int, int,int,int,int, Fl_Flags) const;
  Fl_Color draw_button() const; // special messy one for button style widgets
  void draw_button_label(int,int,int,int,Fl_Color) const;

  void setp(const void* const *, const void*);
  void seti(const unsigned*, unsigned);

private:

  const Fl_Style* style_;
  Fl_Group* parent_;
  Fl_Callback* callback_;
  void* user_data_;
  const char* label_;
  Fl_Image* image_;
  const char* tooltip_;
  unsigned flags_;
  int x_,y_,w_,h_;
  uchar type_;
  uchar damage_;
  uchar when_;

  // "de-implement" the copy constructors:
  Fl_Widget & operator=(const Fl_Widget &);
  Fl_Widget(const Fl_Widget &);

};

// reserved type numbers (necessary for my cheapo RTTI) start here.
// grep the header files for "RESERVED_TYPE" to find the next available
// number.
#define FL_RESERVED_TYPE 100

#endif

//
// End of "$Id: Fl_Widget.H,v 1.27 1999/09/20 16:36:46 vincent Exp $".
//
