/* Values of the bits stored in Widget::damage(). */

#ifndef fltk_damage_h
#define fltk_damage_h

namespace fltk {

enum {
  DAMAGE_VALUE		= 0x01,
  DAMAGE_PUSHED		= 0x02,
  DAMAGE_SCROLL		= 0x04,
  DAMAGE_OVERLAY	= 0x04, // reused value
  DAMAGE_HIGHLIGHT	= 0x08,
  DAMAGE_CHILD		= 0x10,
  DAMAGE_CHILD_LABEL	= 0x20,
  DAMAGE_EXPOSE		= 0x40,
  DAMAGE_CONTENTS	= 0x40, // reused value
  DAMAGE_ALL		= 0x80
};

}

#endif
