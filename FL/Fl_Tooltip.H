#ifndef _FL_TOOLTIP_H_
#define _FL_TOOLTIP_H_

extern void (*fl_tooltip_enter)(Fl_Widget *);
extern void (*fl_tooltip_exit)(Fl_Widget *);

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>

class Fl_TooltipBox;
class Fl_Menu_Window;

class Fl_Tooltip {
public:
  // These functions are user-called functions
  static float delay() { return default_style.delay_; }
  static void delay(float f) { default_style.delay_ = (f < 0.1) ? 0.1 : f; }
  static int font() { return default_style.textfont_; }
  static int size() { return default_style.textsize_; }
  static void font(int i) { default_style.textfont_ = i; }
  static void size(int s) { default_style.textsize_ = s; }
  static void color(uchar c) { default_style.color_ = c; }
  static uchar color() { return default_style.color_; }
  static void textcolor(uchar c) { default_style.textcolor_ = c; }
  static uchar textcolor() { return default_style.textcolor_; }
  static void boxtype(uchar b) { default_style.box_ = b; }
  static uchar boxtype() { return default_style.box_; }

  // This function is called by widgets
  // when the pointer enters them
  static void enter(Fl_Widget *w) {fl_tooltip_enter(w);}

  // This function must be called when
  // an event != FL_MOVE has occured in
  // the widget
  static inline void exit(Fl_Widget *w) {fl_tooltip_exit(w);}

  static void loadstyle();

  friend class Fl_TooltipBox;
  friend class Fl_Widget;

protected:
  struct Style {
    float delay_;
    uchar color_;
    uchar box_;
    uchar textfont_;
    uchar textsize_;
    uchar textcolor_;
  };

  static Style default_style;

private:
  static Fl_TooltipBox *box;
  static Fl_Menu_Window *window;
  static Fl_Widget *widget;
  static int shown;

  static void tooltip_timeout(Fl_Widget *);
  static void tooltip_enter(Fl_Widget *);
  static void tooltip_exit(Fl_Widget *);

};


#endif
