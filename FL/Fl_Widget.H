/
// "$Id$
/
// Widget header file for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2004 by Bill Spitzak and others
/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/

#ifndef Fl_Widget_
#define Fl_Widget_

#include "Enumerations.H



class Fl_Widget
class Fl_Window
class Fl_Group
class Fl_Image
class Fl_Style


// FL_STYLE macro is simplified declaration of widget class style.
// The best way is to include FL_STYLE at the beginning or at the end of the class declaration






#define FL_STYLE 
private: 
  static Fl_Style * style_; 
protected: 
  void apply_style(); 
  void revert_style(); 
public: 
  static Fl_Style * style(); 
  static void style(Fl_Style *); 
private:




typedef void (Fl_Callback )(Fl_Widget*, void*)
typedef Fl_Callback* Fl_Callback_p; // needed for BORLAN
typedef void (Fl_Callback0)(Fl_Widget*)
typedef void (Fl_Callback1)(Fl_Widget*, long)

struct FL_EXPORT Fl_Label 
 

  const char* value
  Fl_Image* image
  Fl_Image* deimage
  uchar type
  uchar font
  uchar size
  unsigned color
  void draw(int,int,int,int, Fl_Align) const 
  void measure(int&, int&) const 
}

/** Fl_Widget is the base class for all widgets in FLTK. You can't create
 * one of these because the constructor is not public. However you can subclass it
 *
 * All "property" accessing methods, such as color(), parent(), or argument() are
 * implemented as trivial inline functions and thus are as fast and small as
 * accessing fields in a structure. Unless otherwise noted, the property setting
 * methods such as color(n) or label(s) are also trivial inline functions, even
 * if they change the widget's appearance. It is up to the user code to call
 * redraw() after these.
 *
class FL_EXPORT Fl_Widget 
 friend class Fl_Group



  Fl_Group* parent_
  Fl_Callback* callback_
  void* user_data_
  int x_,y_,w_,h_
  Fl_Label label_
  int flags_
  unsigned color_
  unsigned color2_
  uchar type_
  uchar damage_
  uchar box_
  uchar align_
  uchar when_
  uchar style_flags_
 

  const char *tooltip_

#  if !defined(WIN32) || !defined(FL_DLL
  // "de-implement" the copy constructors, EXCEPT for when we are using th
  // WIN32 DLL interface, in which case we can't hide them because Microsof
  // requires the copy constructors to implement subclassing..
  Fl_Widget & operator=(const Fl_Widget &) { return *this; 
  Fl_Widget(const Fl_Widget &) {
#  endif // !WIN32 || !FL_DL

protected:
  // Following ate bits of user_flags_ which are set when user sets the particular value
  // Set bit indicates that the style, when applied, can NOT override previous value
  enum {COLOR=1, SELECTION_COLOR=2, BOX=4, LABELTYPE=8, LABELCOLOR=16, LABELFONT=32, LABELSIZE=64, ALIGN=128}
protected

    /** Creates a widget at the given position and size. The Fl_Widget is a
     * protected constructor, but all derived widgets have a matching public
     * constructor. It takes a value for x(), y(), w(), h(), and an optional
     * value for label(). *
  Fl_Widget(int x,int y,int w,int h,const char *label=0)

  void x(int v) {x_ = (short)v;
  void y(int v) {y_ = (short)v;
  void w(int v) {w_ = (short)v;
  void h(int v) {h_ = (short)v;

  int flags() const {return flags_;
  void set_flag(int c) {flags_ |= c;
  void clear_flag(int c) {flags_ &= ~c;
  enum {INACTIVE=1, INVISIBLE=2, OUTPUT=4, SHORTCUT_LABEL=64
        CHANGED=128, VISIBLE_FOCUS=512, COPIED_LABEL = 1024}

  void draw_box() const
  void draw_box(Fl_Boxtype, Fl_Color) const
  void draw_box(Fl_Boxtype, int,int,int,int, Fl_Color) const
  void draw_focus() {draw_focus(box(),x(),y(),w(),h());
  void draw_focus(Fl_Boxtype, int,int,int,int) const
  void draw_label() const
  void draw_label(int, int, int, int) const

public

    /** Destroys the widget. Destroying single widgets is not very common,
     * and it is your responsibility to either remove() them from any
     * enclosing group or destroy that group \e immediately after
     * destroying the children. You almost always want to destroy the
     * parent group instead which will destroy all of the child widgets
     * and groups in that group. *
  virtual ~Fl_Widget()

  virtual void draw() = 0
    /** Handles the specified event. You normally don't call this method directly, but instea
     * let FLTK do it when the user interacts with the widget. \see Fl::event() *
  virtual int handle(int event)
    /** Returns the parent widget, the group the manages this widget. \see Fl_Group *
  Fl_Group* parent() const {return parent_;
    /** \internal *
  void parent(Fl_Group* p) {parent_ = p;} // for hacks only, Fl_Group::add(

    /** Returns the widget type value. *
  uchar type() const {return type_;
    /** Sets the widget type value, which is used for Forms compatability and t
     * simulate RTTI. *
  void type(uchar t) {type_ = t;

    /// \{ \name position and siz
    /** Returns the position of the upper-left corner of the widget in its enclosin
     * Fl_Window (not its parent if that is not an Fl_Window). \see resize() *
  int x() const {return x_;
    /** Returns the position of the widget. \see x(), w(), h() *
  int y() const {return y_;
    /** Returns the width of the widget. \see x(), y(), h() *
  int w() const {return w_;
    /** Returns the height of the widget. \see x(), y(), w() *
  int h() const {return h_;
    /** Change the size or position of the widget. This is a virtual function so that
     * the widget may implement its own handling of resizing. The default version does
     * not call the redraw() method, but instead relies on the parent widget to do so
     * because the parent may know a faster way to update the display, such as scrolling
     * from the old position. \see position(), size(), x(), y(), w(), h() *
  virtual void resize(int x, int y, int width, int height)
  int damage_resize(int,int,int,int)
    /** <i>position(x, y)</i> is a shortcut for <i>resize(x,y,w(),h())</i>. \see resize() *
  void position(int X,int Y) {resize(X,Y,w_,h_);
    /** <i>size(w,h)</i> is a shortcut for <i>resize(x(),y(),w,h)</i>. \see resize() *
  void size(int W,int H) {resize(x_,y_,W,H);
    /// \

    /// \{ \name visual
    /** Gets the label alignment, which controls how the labe
     * is displayed next to or inside the widget. \see labeltype(), image(), label() *
  Fl_Align align() const {return (Fl_Align)align_;
    /** Sets the label alignment, which controls how the label is displayed
     * next to or inside the widget. The default value is \c FL_ALIGN_CENTER,
     * which centers the label inside the widget.
     * The value can be any of these constants bitwise-OR'd together
     * \arg \c FL_ALIGN_BOTTO
     * \arg \c FL_ALIGN_CENTE
     * \arg \c FL_ALIGN_CLI
     * \arg \c FL_ALIGN_INSID
     * \arg \c FL_ALIGN_LEF
     * \arg \c FL_ALIGN_RIGH
     * \arg \c FL_ALIGN_TEXT_OVER_IMAG
     * \arg \c FL_ALIGN_TO
     * \arg \c FL_ALIGN_WRAP \see labeltype(), image(), label() *
  void align(uchar a) {align_ = a; style_flags_ |= ALIGN;
    /** Gets the box type for the widget. \see Fl_Boxtype, color() *
  Fl_Boxtype box() const {return (Fl_Boxtype)box_;
    /** Sets the box type for the widget, which identifies a routine that draws
     * the background of the widget. See Box Types for the available types.
     * The default depends on the widget, but is usually \c FL_NO_BOX or \c FL_UP_BOX. *
  void box(Fl_Boxtype a) {box_ = a; style_flags_ |= BOX;
    /** Gets the background color of the widget. \see selection_color(), box() *
  Fl_Color color() const {return (Fl_Color)color_;
    /** Sets the background color of the widget. The color is passed to the box routine.
     * The color is either an index into an internal table of RGB colors or an RGB colo
     * value generated using fl_rgb_color(). The default for most widgets is \c FL_GRAY.
     * See the enumeration list for predefined colors.
     * Use Fl::set_color() to redefine colors. \see selection_color(), box(). label() *
  void color(unsigned a) {color_ = a; style_flags_ |= COLOR;
    /** Gets the selection color. *
  Fl_Color selection_color() const {return (Fl_Color)color2_;
    /** Sets the selection color, which is defined for Forms compatibility and is
     * usually used to color the widget when it is selected, although some widgets use
     * this color for other purposes. You can set both colors at once with <i>color(a,b)</i>. *
  void selection_color(unsigned a) {color2_ = a; style_flags_ |= SELECTION_COLOR;
    /** The two color form sets both the background and selection colors.
     * See the description of the selection_color() method for more information. \see color() *
  void color(unsigned a, unsigned b) {color_=a; color2_=b; style_flags_ |= COLOR | SELECTION_COLOR;
    /** Get the current label pointer. \see color(), image(), align() *
  const char* label() const {return label_.value;
    /** Set the current label pointer. The label is shown somewhere on or next
     * to the widget. The passed pointer is stored unchanged in the widget
     * (the string is not copied), so if you need to set the label to a formatte
     * value, make sure the buffer is static, global, or allocated. \see align(), box() *
  void label(const char* a)
    /** Set the current label type and label pointer. \see align(), image() *
  void copy_label(const char* a)
      /** Copies given label to internal buffer. This buffer is fried when label is changed or widget is destroyed. *
  void label(Fl_Labeltype a,const char* b) {label_.type = a; label_.value = b; style_flags_ |= LABELTYPE;
    /** Gets the labeltype which identifies the function that draws the label of the widget. *
  Fl_Labeltype labeltype() const {return (Fl_Labeltype)label_.type;
    /** Sets the labeltype which identifies the function that draws the label o
     * the widget. This is generally used for special effects such as embossing
     * or for using the label() pointer as another form of data such as an icon.
     * The value \c FL_NORMAL_LABEL prints the label as plain text. \see label(), align() *
  void labeltype(Fl_Labeltype a) {label_.type = a; style_flags_ |= LABELTYPE;
    /** Gets the label color. \see label(), selectioncolor() *
  Fl_Color labelcolor() const {return (Fl_Color)label_.color;
    /** Sets the label color. The default color is \c FL_BLACK. \see color() *
  void labelcolor(unsigned a) {label_.color=a; style_flags_ |= LABELCOLOR;
    /** Gets the font to use. \see Fl::set_font() *
  Fl_Font labelfont() const {return (Fl_Font)label_.font;
    /** Sets the font to use. Fonts are identified by small 8-bit indexe
     * into a table. See the enumeration list for predefined typefaces.
     * The default value uses a Helvetica typeface (Arial for Microsoft¨ Windows¨).
     * The function Fl::set_font() can define new typefaces. \see Fl::set_font() *
  void labelfont(uchar a) {label_.font=a;style_flags_ |= LABELFONT;
    /** Gets the font size in pixels. \see Fl_FontDesc, Fl_FontSize *
  uchar labelsize() const {return label_.size;
    /** Sets the font size in pixels. The default size is 14 pixels. \see Fl_FontSize*
  void labelsize(uchar a) {label_.size=a; style_flags_ |= LABELSIZE;
    /** Gets the image to use as part of the widget label. \see Fl_Image *
  Fl_Image* image() {return label_.image;
    /** Sets the image to use as part of the widget label. This image is used
     * when drawing the widget in the active state. \see Fl_Image, label(), align() *
  void image(Fl_Image* a) {label_.image=a;
  void image(Fl_Image& a) {label_.image=&a;
    /** Gets the image to use as part of the inactive widget label. \see Fl_Image *
  Fl_Image* deimage() {return label_.deimage;
    /** Sets the image to use as part of the widget label. This image is use
     * when drawing the widget in the inactive state. \see active(), label() *
  void deimage(Fl_Image* a) {label_.deimage=a;
  void deimage(Fl_Image& a) {label_.deimage=&a;
    /** Gets a string of text to display in a popup tooltip window *
  const char *tooltip() const {return tooltip_;
    /** Sets a string of text to display in a popup tooltip window when the user hover
     * the mouse over the widget. The string is not copied, so make sure any formatte
     * string is stored in a static, global, or allocated buffer. *
  void tooltip(const char *t)
    /** Gets the current callback function for the widget. *
    /// \
   
    /// \{ \name callback handlin
  Fl_Callback_p callback() const {return callback_;
    /** Sets the current callback function and argument for the widget. Each widget has a single callback. *
  void callback(Fl_Callback* c, void* p) {callback_=c; user_data_=p;
    /** Sets the current callback function. \see when(), argument(), user_data() *
  void callback(Fl_Callback* c) {callback_=c;
  void callback(Fl_Callback0*c) {callback_=(Fl_Callback*)c;
    /** Gets or sets the current callback function for the widget. Each widget has a single callback. *
  void callback(Fl_Callback1*c, long p=0) {callback_=(Fl_Callback*)c; user_data_=(void*)p;
    /** Gets the current user data. *
  void* user_data() const {return user_data_;
    /** Sets the current user data (<i>void *</i>) argument that is passed to the
     * callback function. \see argument() *
  void user_data(void* v) {user_data_ = v;
    /** Gets the current user data (long) argument. *
  long argument() const {return (long)user_data_;
    /** Sets the current user data (long) argument that is passed
     * to the callback function
     * \note This is implemented by casting the long value to a \c void*
     * and may not be portable on some machines. \see user_data()*
  void argument(long v) {user_data_ = (void*)v;
    /** Returns the callback calling condition. *
  Fl_When when() const {return (Fl_When)when_;
    /** Fl_Widget::when() is a set of bitflags used by subclasses of \c Fl_Widget to decide
     * when to do the callback. If the value is zero then the callback is never done.
     * Other values are described in the individual widgets. This field is in the base clas
     * so that you can scan a panel and do_callback() on all the ones that don't do their
     * own callbacks in response to an "OK" button. \see callback(), user_data() *
  void when(uchar i) {when_ = i;
    /// \

    /// \{ \name event handlin
    /** An invisible widget never gets redrawn and does not get events. The visible()
     * method returns true if the widget is set to be visible. A widget is only visible
     * if visible() is true on it and all of its parents. \see visible_r() *
  int visible() const {return !(flags_&INVISIBLE);
    /** The visible_r() method returns true if the widget and all of its parents are visible. *
  int visible_r() const
    /** Make this widget visible
     * Changing visibility will send \c FL_SHOW or \c FL_HIDE events to the widget.
     * Do not change it if the parent is not visible, as this will send false \c FL_SHOW
     * or \c FL_HIDE events to the widget. redraw() is called if necessary on this
     * or the parent. \see hide(), visible() *
  void show()
    /** Hide this widget. \see show(), visible() *
  void hide()
    /** Makes the widget visible; you must still redraw the parent widget to see a
     * change in the window. Normally you want to use the show() method instead. *
  void set_visible() {flags_ &= ~INVISIBLE;
    /** Hides the widget; you must still redraw the parent to see a change in
     * the window. Normally you want to use the hide() method instead. *
  void clear_visible() {flags_ |= INVISIBLE;
    /** Fl_Widget::active() returns whether the widget is active.  An inactive widge
     * does not get any events, but it does get redrawn. A widget is only active if
     * active() is true on it and all of its parents. *
  int active() const {return !(flags_&INACTIVE);
    /** Fl_Widget::active_r() returns whether the widget and all of its parents are active. *
  int active_r() const
    /** activate() sends \c FL_ACTIVATE to the widget if active_r() is true. *
  void activate()
    /** deactivate() sends \c FL_DEACTIVATE to the widget if active_r() is true
     * Currently you cannot deactivate Fl_Window widgets. *
  void deactivate()
    /** output() means the same as !active() except it does not change
     * how the widget is drawn. The widget will not receive any events.
     * This is useful for making scrollbars or buttons that work as
     * displays rather than input devices. *
  int output() const {return (flags_&OUTPUT);
    /** Sets the widget into output mode. \see output() *
  void set_output() {flags_ |= OUTPUT;
    /** Sets the widget into regular (non-output) mode. \see output() *
  void clear_output() {flags_ &= ~OUTPUT;
    /** This is the same as (active() && !output() && visible()) but is faster. *
  int takesevents() const {return !(flags_&(INACTIVE|INVISIBLE|OUTPUT));
    /** Fl_Widget::changed() is a flag that is turned on when the user
     * changes the value stored in the widget. This is only used by
     * subclasses of Fl_Widget that store values, but is in the base
     * class so it is easier to scan all the widgets in a panel and
     * do_callback() on the changed ones in response to an "OK" button
     *
     * Most widgets turn this flag off when they do the callback,
     * and when the program sets the stored value. *
  int changed() const {return flags_&CHANGED;
    /** Mark this widget as changed. \see Fl_Widget::changed() *
  void set_changed() {flags_ |= CHANGED;
    /** Mark this widget as unchanged. \see Fl_Widget::changed() *
  void clear_changed() {flags_ &= ~CHANGED;
    /** Tries to make this widget be the Fl::focus() widget, by first sending it an
     * \c FL_FOCUS event, and if it returns non-zero, setting Fl::focus() to this widget.
     * You should use this method to assign the focus to an widget. Returns true if the
     * widget accepted the focus. *
  int take_focus()
    /** Enables keyboard focus navigation with this widget; note, however, that this wil
     * not necessarily mean that the widget will accept focus, but for widgets that ca
     * accept focus, this method enables it if it has been disabled. *
  void set_visible_focus() { flags_ |= VISIBLE_FOCUS; 
    /** Disables keyboard focus navigation with this widget; normally,
     * all widgets participate in keyboard focus navigation. *
  void clear_visible_focus() { flags_ &= ~VISIBLE_FOCUS; 
    /** Handle keyboard focus navigation. \see set_visible_focus(), clear_visible_focus() *
  void visible_focus(int v) { if (v) set_visible_focus(); else clear_visible_focus(); 
    /** Returns non-zero if this widget will participate in keyboard focus navigation. *
  int  visible_focus() { return flags_ & VISIBLE_FOCUS; 
    /// \

    /** The default callback, which puts a pointer to the widget on
     * the queue returned by Fl::readqueue().
     * You may want to call this from your own callback. *
  static void default_callback(Fl_Widget *w, void *user_data)
    /** Causes a widget to invoke its callback function, optionally with arbitrary arguments. *
  void do_callback() {callback_(this,user_data_); if (callback_ != default_callback) clear_changed();
  void do_callback(Fl_Widget* o,void* arg=0) {callback_(o,arg); if (callback_ != default_callback) clear_changed();
  void do_callback(Fl_Widget* o,long arg) {callback_(o,(void*)arg); if (callback_ != default_callback) clear_changed();
  int test_shortcut()
  static int test_shortcut(const char*)
    /** Returns 1 if \a b is a child of this widget, or is equal to
     * this widget. Returns 0 if \a b is \c NULL. *
  int contains(const Fl_Widget *b) const 
    /** Returns 1 if this widget is a child of \a a, or is equal t
     * \a a. Returns 0 if \a a is \c NULL. *
  int inside(const Fl_Widget* a) const {return a ? a->contains(this) : 0;

    /// \{ \name drawin
    /** Marks the widget as needing its draw() routine called. *
  void redraw()
    /** Marks the widget or the parent as needing a redraw for the label area of a widget. *
  void redraw_label()
    /** damage() returns non-zero if draw() needs to be called. *
  uchar damage() const {return damage_;
  void clear_damage(uchar c = 0) {damage_ = c;
    /** Set the damage bits for the widget
     * The damage value is actually a bit field that the widget
     * subclass can use to figure out what parts to draw. *
  void damage(uchar damage_flags)
    /** This call declares the widget damaged within the specified bounding box. *
  void damage(uchar damage_flags,int x,int y,int width,int height)
  void draw_label(int x, int y, int width, int height, Fl_Align align_flags) const
  void measure_label(int& xx, int& yy) {label_.measure(xx,yy);
    /// \

    /** Returns a pointer to the primary Fl_Window widget. Returns \c NULL if no window i
     * associated with this widget. \note For an \c Fl_Window widget, this returns it
     * parent window (if any), not this window. *
  Fl_Window* window() const 

    /** \deprecated for fltk 1.0 back compatibility *
  Fl_Color color2() const {return (Fl_Color)color2_;
    /** \deprecated for fltk 1.0 back compatibility *
  void color2(unsigned new_color) {color2_ = new_color; style_flags_ |= SELECTION_COLOR;}


    /** Rerturns number of child widgets for group, -1 if it is not a subclass of Fl_Grou
     * and can not have childrens. *
  virtual int children() const {return -1;
  virtual Fl_Widget* const* array() const {return 0;}

 
  // styled updade subclassess of widget valu

protected
  virtual void apply_style(){}
  virtual void revert_style()
public


  void redraw_style(); // causes to reload style, including all children (id any). Not explicitly used by use
  bool try_color(unsigned c){if ((style_flags_ & COLOR) || (color_ == c)) return 0; color_ = c; return 1;
  bool try_selection_color(unsigned c){if ((style_flags_ & SELECTION_COLOR) || (color2_ == c)) return 0; color2_ = c; return 1;
  bool try_box(uchar b){if ((style_flags_ & BOX) || (box_ ==b)) return 0; box_ = b; return 1;
  bool try_labeltype(Fl_Labeltype t){if((style_flags_ & LABELTYPE)||(t == label_.type)) return 0; label_.type = t; return 1;
  bool try_labelcolor(unsigned c){if ((style_flags_ & LABELCOLOR) ||(c == label_.color)) return 0;  label_.color = c; return 1;
  bool try_labelfont(uchar f){if ((style_flags_ & LABELFONT) || (f == label_.font)) return 0; label_.font = f; return 1;
  bool try_labelsize(uchar s){if ((style_flags_ & LABELSIZE) || (label_.size == s)) return 0; label_.size = s; return 1;



}

// reserved type numbers (necessary for my cheapo RTTI) start here
// grep the header files for "RESERVED_TYPE" to find the next availabl
// number
#define FL_RESERVED_TYPE 10

#endi

/
// End of "$Id$"
/
