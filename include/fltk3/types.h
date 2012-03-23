/*
 * "$Id$"
 *
 * Simple "C"-style types for the Fast Light Tool Kit (FLTK).
 *
 * Copyright 1998-2010 by Bill Spitzak and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems on the following page:
 *
 *     http://www.fltk.org/str.php
 */

/** \file 
 *  This file contains simple "C"-style type definitions.
 */

#ifndef FLTK3_TYPES_H
#define FLTK3_TYPES_H

/** \name	Miscellaneous */
/*@{*/	/* group: Miscellaneous */

/** unsigned char */
typedef unsigned char uchar;
/** unsigned long */
typedef unsigned long ulong;

namespace fltk3 {
  
  /** Flexible length utf8 Unicode text
   *
   *  \todo FIXME: temporary (?) typedef to mark UTF8 and Unicode conversions
   */
  typedef char *String;
  
  /** Flexible length utf8 Unicode read-only string
   *  \sa fltk3::String
   */
  typedef const char *CString;
  
  /** 24-bit Unicode character + 8-bit indicator for keyboard flags */
  typedef unsigned int Shortcut;
  
  /** 24-bit Unicode character - upper 8-bits are unused */
  typedef unsigned int Char;
  
  /*@}*/	/* group: Miscellaneous */
  
}

#endif

/*
 * End of "$Id$".
 */
