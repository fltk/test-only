/* Values of the bits stored in Widget::layout_damage(). */

#ifndef fltk_layout_h
#define fltk_layout_h

namespace fltk {

enum {
  LAYOUT_X	= 0x01,
  LAYOUT_Y	= 0x02,
  LAYOUT_XY	= 0x03,
  LAYOUT_W	= 0x04,
  LAYOUT_H	= 0x08,
  LAYOUT_WH	= 0x0C,
  LAYOUT_XYWH	= 0x0F,
  LAYOUT_CHILD	= 0x10,
  LAYOUT_DAMAGE	= 0x80
};

}

#endif
