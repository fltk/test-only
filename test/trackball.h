/
 * (c) Copyright 1993, 1994, Silicon Graphics, Inc
 * ALL RIGHTS RESERVE
 * Permission to use, copy, modify, and distribute this software fo
 * any purpose and without fee is hereby granted, provided that the abov
 * copyright notice appear in all copies and that both the copyright notic
 * and this permission notice appear in supporting documentation, and tha
 * the name of Silicon Graphics, Inc. not be used in advertisin
 * or publicity pertaining to distribution of the software without specific
 * written prior permission
 
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY O
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICO
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF AN
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS O
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEE
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND O
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH TH
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE
 
 * US Government Users Restricted Right
 * Use, duplication, or disclosure by the Government is subject t
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagrap
 * (c)(1)(ii) of the Rights in Technical Data and Computer Softwar
 * clause at DFARS 252.227-7013 and/or in similar or successo
 * clauses in the FAR or the DOD or NASA FAR Supplement
 * Unpublished-- rights reserved under the copyright laws of th
 * United States.  Contractor/manufacturer is Silicon Graphics
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311
 
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc
 *
/
 * trackball.
 * A virtual trackball implementatio
 * Written by Gavin Bell for Silicon Graphics, November 1988
 *

/
 * Pass the x and y coordinates of the last and current positions o
 * the mouse, scaled so they are from (-1.0 ... 1.0)
 
 * The resulting rotation is returned as a quaternion rotation in th
 * first paramater
 *
voi
trackball(float q[4], float p1x, float p1y, float p2x, float p2y)

/
 * Given two quaternions, add them together to get a third quaternion
 * Adding quaternions to get a compound rotation is analagous to addin
 * translations to get a compound translation.  When incrementall
 * adding rotations, the first argument here should be the ne
 * rotation, the second and third the total rotation (which will b
 * over-written with the resulting new total rotation)
 *
voi
add_quats(float *q1, float *q2, float *dest)

/
 * A useful function, builds a rotation matrix in Matrix based o
 * given quaternion
 *
voi
build_rotmatrix(float m[4][4], float q[4])

/
 * This function computes a quaternion based on an axis (defined b
 * the given vector) and an angle about which to rotate.  The angle i
 * expressed in radians.  The result is put into the third argument
 *
voi
axis_to_quat(float a[3], float phi, float q[4])

