/
// "$Id$
/
// Math header file for the Fast Light Tool Kit (FLTK)
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

#ifndef fl_math_
#  define fl_math_

// Apple's ProjectBuilder has the nasty habit of including recursivel
// down the file tree. To avoid re-including <FL/math.h> we must
// directly include the systems math file. (Plus, I could not find a
// predefined macro for ProjectBuilder builds, so we have to define it
// in the project
#  if defined(__APPLE__) && defined(__PROJECTBUILDER__
#    include "/usr/include/math.h
#  els
#    include <math.h
#  endi

#  ifdef __EMX_
#    include <float.h
#  endi


#  ifndef M_P
#    define M_PI            3.1415926535897932384
#    define M_PI_2          1.5707963267948966192
#    define M_PI_4          0.7853981633974483096
#    define M_1_PI          0.3183098861837906715
#    define M_2_PI          0.6366197723675813430
#  endif // !M_P

#  ifndef M_SQRT
#    define M_SQRT2         1.4142135623730950488
#    define M_SQRT1_2       0.7071067811865475244
#  endif // !M_SQRT

#  if (defined(WIN32) || defined(CRAY)) && !defined(__MINGW32__) && !defined(__MWERKS__

inline double rint(double v) {return floor(v+.5);
inline double copysign(double a, double b) {return b<0 ? -a : a;

#  endif // (WIN32 || CRAY) && !__MINGW32__ && !__MWERKS_

#endif // !fl_math_


/
// End of "$Id$"
/
