/* Values of the bits stored in Widget::layout_damage(). */

#ifndef fltk_layout_h
#define fltk_layout_h

namespace fltk {

/*! \addtogroup layout
  When the Widget::layout() method is called,
  Widget::layout_damage() contains bitflags indicating what has
  changed since it was last called.
  \{ */

enum {
  LAYOUT_X	= 0x01, /*!< Widget::x() changed by resize() */
  LAYOUT_Y	= 0x02, /*!< Widget::y() changed by resize() */
  LAYOUT_XY	= 0x03, /*!< Same as LAYOUT_X|LAYOUT_Y */
  LAYOUT_W	= 0x04,	/*!< Widget::w() changed by resize() */
  LAYOUT_H	= 0x08,	/*!< Widget::h() changed by resize() */
  LAYOUT_WH	= 0x0C, /*!< Same as LAYOUT_W|LAYOUT_H */
  LAYOUT_XYWH	= 0x0F, /*!< Same as LAYOUT_XY|LAYOUT_WH */
  LAYOUT_CHILD	= 0x10, /*!< Widget::layout() needs to be called on a child of this group widget. */
  LAYOUT_DAMAGE	= 0x80	/*!< Widget::relayout() was called. */
};
/*! \} */

}

#endif
