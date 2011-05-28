#include <fltk/Style.h>
#include <fltk/Button.h>
#include <fltk/Symbol.h>
#include <fltk/SharedImage.h>
#include <fltk/draw.h>
#include <fltk/Color.h>
#include <cstdio>
#include <cstring>

#define PAPERCOLOUR  0xFFFFFF00
#define BGCOLOUR     0xF4F4EC00
#define SLIDERCOLOUR 0xC7DAFF00
#define BUTTONCOLOUR GRAY75
#define BARCOLOUR    lerp(GRAY75, 0x8EA2E200, .1)

using namespace fltk;
/* BS: The below is a trial of an XP theme. For now they're not in the
   public API, because I really don't think they should be
   This may change though (2.0 being alpha, and all), so as a reference
   for the future I've got this comment here.
   TODO: move the #include into a header to avoid bloating the library
*/

#include "scrollbar_frames.dat"
#include "fourbuttons.dat"

#define FRAME_SIZE 17

//static unsigned char pixel_frame[FRAME_SIZE][FRAME_SIZE];
static unsigned char const * const frames[26] = {
	frame01, frame02, frame03, frame04, frame05, 
	frame06, frame07, frame08, frame09, frame10,
	frame11, frame12, frame13, frame14, frame15,
	frame16, frame17, frame18, frame19, frame20,
	frame21, frame22, frame23, frame24, frame25,
	frame26};
static Image* img_frame;
static Image* images[26];
static Image* pixie = 0;
static Image* pixie_hilight;
static Image* pixie_return_butt;
static Image* pixie_norm;
static Image* pixie_down;

static bool scroll_glyph_initialised;
static bool xp_glyph_initialised;

static void pixie_box_init_check() {
  if (!xp_glyph_initialised) {
    xp_glyph_initialised = true;
    pixie_hilight = bmpImage::get("xp_button_hi", xp_button_hi);
    pixie_return_butt = bmpImage::get("xp_button_return", xp_button_return);
    pixie_down = bmpImage::get("xp_button_dn", xp_button_dn);
    pixie = pixie_norm = bmpImage::get("xp_button_up", xp_button_up);
  }
}

static void scroll_glyph_init_check() {
  if (!scroll_glyph_initialised) {
    scroll_glyph_initialised = true;
    img_frame = new Image(FRAME_SIZE, FRAME_SIZE, "current_image");
    char buffer[10];
    for (int i = 0; i < 26; i++){
      if (i < 10)
        sprintf(buffer, "frame0%d", i+1);
      else
        sprintf(buffer, "frame%d",  i);
      images[i] = gifImage::get(buffer, frames[i]);
    }
  }
}

static void set_frame(int frame) {
  scroll_glyph_init_check();
  img_frame = images[frame];
}

enum {
  H_TRACK = 0,
  H_SLIDER,
  H_LEFT_ARROW,
  H_RIGHT_ARROW,
  H_SLIDER_HI,
  H_LEFT_ARROW_HI,
  H_RIGHT_ARROW_HI,
  H_SLIDER_PUSHED,
  H_LEFT_ARROW_PUSHED,
  H_RIGHT_ARROW_PUSHED,
  H_KNURL,
  H_KNURL_HI,
  H_KNURL_PUSHED,

  V_TRACK,
  V_SLIDER,
  V_UP_ARROW,
  V_DOWN_ARROW,
  V_SLIDER_HI,
  V_UP_ARROW_HI,
  V_DOWN_ARROW_HI,
  V_SLIDER_PUSHED,
  V_UP_ARROW_PUSHED,
  V_DOWN_ARROW_PUSHED,
  V_KNURL,
  V_KNURL_HI,
  V_KNURL_PUSHED
}; // 13 frames each

static void draw_arrows(const Rectangle& R, int drawflags) {
  if (drawflags & ALIGN_TOP) {
    if (drawflags & PUSHED)
      set_frame(V_UP_ARROW_PUSHED);
    else if (drawflags & HIGHLIGHT)
      set_frame(V_UP_ARROW_HI);
    else
      set_frame(V_UP_ARROW);
    img_frame->draw(R);
  } else if (drawflags & ALIGN_BOTTOM) {
    if (drawflags & PUSHED)
      set_frame(V_DOWN_ARROW_PUSHED);
    else if (drawflags & HIGHLIGHT)
      set_frame(V_DOWN_ARROW_HI);
    else
      set_frame(V_DOWN_ARROW);
    img_frame->draw(R);
    
  } else if (drawflags & ALIGN_LEFT) {
    if (drawflags & PUSHED)
      set_frame(H_LEFT_ARROW_PUSHED);
    else if (drawflags & HIGHLIGHT)
      set_frame(H_LEFT_ARROW_HI);
    else
      set_frame(H_LEFT_ARROW);
    img_frame->draw(R);
  } else if (drawflags & ALIGN_RIGHT) {
    if (drawflags & PUSHED)
      set_frame(H_RIGHT_ARROW_PUSHED);
    else if (drawflags & HIGHLIGHT)
      set_frame(H_RIGHT_ARROW_HI);
    else
      set_frame(H_RIGHT_ARROW);
    img_frame->draw(R);
  }
}

static class XPScrollBarGlyph : public Symbol {
  public:
    void _draw(const Rectangle& R) const {
      scroll_glyph_init_check();
      if (drawflags() & ALIGN_MASK) {
	draw_arrows(R, drawflags());
	if (drawflags() & ALIGN_BOTTOM) {
	  if (R.h() > R.w()) {
            set_frame(V_TRACK);
            img_frame->draw_diced(R);
          } else if (R.w() > R.h()) {
            set_frame(H_TRACK);
            img_frame->draw_diced(R);
          }
	}
      } else {
	if (R.x() == 0) { // Vertical
	  if (drawflags() & PUSHED)
	    set_frame(V_SLIDER_PUSHED);
	  else if (drawflags() & HIGHLIGHT)
	    set_frame(V_SLIDER_HI);
	  else
	    set_frame(V_SLIDER);
	  img_frame->draw_diced(R);
	  
	  if (drawflags() & PUSHED)
	    set_frame(V_KNURL_PUSHED);
	  else if (drawflags() & HIGHLIGHT)
	    set_frame(V_KNURL_HI);
	  else
	    set_frame(V_KNURL);

	  Rectangle rr(R);
	  if (rr.h() <= 18) {
	    rr.move_y(4);
	    rr.move_b(-4);
	  } else {
	    rr.move_y((rr.h() / 2) - 7); // Magical constants c/o rainbowsally
	    rr.h(FRAME_SIZE);
	  }
	  push_clip(rr);
	  img_frame->draw_diced(rr);
	  pop_clip();
	} else if (R.y() == 0){
	  if (drawflags() & PUSHED)
	    set_frame(H_SLIDER_PUSHED);
	  else if (drawflags() & HIGHLIGHT)
	    set_frame(H_SLIDER_HI);
	  else
	    set_frame(H_SLIDER);
	  img_frame->draw_diced(R);

	  if (drawflags() & PUSHED)
	    set_frame(H_KNURL_PUSHED);
	  else if (drawflags() & HIGHLIGHT)
	    set_frame(H_KNURL_HI);
	  else
	    set_frame(H_KNURL);

	  Rectangle rr(R);
	  if (rr.w() <= 18) {
	    rr.move_x(4);
	    rr.move_r(-4);
	  } else {
	    rr.move_x((rr.w() / 2) - 7); // Magical constants again c/o rainbowsally
	    rr.w(FRAME_SIZE);
	  }
	  push_clip(rr);
	  img_frame->draw(rr);
	  pop_clip();
	}
      }
    }
    XPScrollBarGlyph() : Symbol("XPScrollBarGlyph") {}
    ~XPScrollBarGlyph() {
      delete img_frame;
    }
} xp_glyph;

static class ArrowGlyph : public Symbol {
  public:
    void _draw(const Rectangle& R) const {
      draw_arrows(R, drawflags());
    }
    ArrowGlyph() : Symbol("arrow glyph") {}
} arrow_glyph;

static class NoGlyph : public Symbol {
  public:
    void _draw(const Rectangle& r) const{}
    NoGlyph() : Symbol("no_glyph") {}
} no_glyph;

class NewFocusBox : public Symbol {
  public:
    NewFocusBox() : Symbol("NewFocusBox") {}
    void _draw(const Rectangle& R) const {
      Rectangle r(R);
      r.inset(3);
      DEFAULT_FOCUS_BOX->_draw(r);
    }
} new_focusbox;

static class XPBox : public Box {
  protected:
    const char* data_;
    const Box* down_;
    bool type_return_;
  public:
    inline const char* data() const {return data_;}
    inline void data(const char* d) {data_ = d;}
    void _draw(const Rectangle& R) const {
      pixie_box_init_check();

      if (drawflags() & PUSHED || (drawflags() & STATE))
        pixie = pixie_down;
      else if (drawflags() & HIGHLIGHT)
        pixie = pixie_hilight;
      else if (is_return_box())
        pixie = pixie_return_butt;
      else pixie = pixie_norm;
       pixie->draw_diced(R);
    }

    bool fills_rectangle() const {return true;}
    bool is_frame() const {return false;}
    bool is_return_box() const {return type_return_;}
    XPBox(const char* name, int dx, int dy, int dw, int dh,
          const char* pattern, const Box* down = 0) : Box(name),
          data_(pattern), down_(down) {
      set_inset(dx, dy, -dw, -dh);
      if(strcmp(pattern, "return") == 0)
        type_return_ = true;
      else
        type_return_ = false;
    }

} xp_box("XPDownBox", 5, 5, 10, 10, "down"), xp_return_box("XPDownBox", 5, 5, 10, 10, "return", &xp_box);

extern "C" bool fltk_theme() {
  Style* s;
  if((s = Style::find("default"))) {
    s->scrollbar_width(17);
    s->color(PAPERCOLOUR);
    s->buttoncolor(BUTTONCOLOUR);
    s->labelsize(12.0);
    s->textsize(12.0);
    s->leading(4);
    s->selection_color(0x316AC500);
    s->alt_color(BARCOLOUR); // used by some buttons
  }
  if ((s = Style::find("Window"))) {
    s->color(BGCOLOUR);
  }
  if ((s = Style::find("Scrollbar"))) {
    s->highlight_color(s->color());
    s->color(-1);
    s->glyph(&xp_glyph);
    s->box(&xp_glyph);
    s->buttonbox(&xp_glyph);
  }
  if ((s = Style::find("PopupMenu"))) {
    s->glyph(&arrow_glyph);
    s->buttonbox(&xp_box);
    s->highlight_color(s->color());
  }
  if ((s = Style::find("Choice"))) {
    s->glyph(&arrow_glyph);
    s->color(WHITE);
    s->highlight_color(s->color());
    s->box(&xp_box);
  }
  if ((s = Style::find("Button"))) {
    s->buttonbox(&xp_box);
    s->highlight_color(WHITE);
    s->focusbox(&new_focusbox);
    s->glyph(&no_glyph);
  }
  if ((s = Style::find("ReturnButton"))) {
    s->buttonbox(&xp_return_box);
    s->highlight_color(WHITE);
    s->focusbox(&new_focusbox);
    s->glyph(&no_glyph);
  }
  if ((s = Style::find("Adjuster"))) {
    s->buttonbox(&xp_box);
    s->color(BGCOLOUR);
    s->buttoncolor(SLIDERCOLOUR);
    s->highlight_color(s->color());
  }
  if ((s = Style::find("Slider"))) {
    s->buttonbox(&xp_box);
    s->color(BGCOLOUR);
    s->buttoncolor(SLIDERCOLOUR);
    s->highlight_color(s->color());
  }
  if ((s = Style::find("InputBrowser"))) {
    s->glyph(&arrow_glyph);
    s->highlight_color(s->color());
  }
  if ((s = Style::find("ValueInput"))) {
    s->glyph(&arrow_glyph);
    s->color(PAPERCOLOUR);
    s->buttoncolor(SLIDERCOLOUR);
    s->highlight_color(s->color());
  }
  if ((s = Style::find("CycleButton"))) {
    s->box(&xp_box);
    s->highlight_color(s->color());
  }
  if ((s = Style::find("Menu"))) {
    s->color(BGCOLOUR);
    s->highlight_color(DARK_BLUE);
  }
  if ((s = Style::find("BarGroup"))) {
    s->glyph(&arrow_glyph);
    s->highlight_color(s->color());
  }
  if ((s = Style::find("Item"))) {
    s->color(PAPERCOLOUR);
    s->buttoncolor(SLIDERCOLOUR);
    s->selection_color(0x316AC500);
    s->leading(5);
  }
  return true;
 
}

