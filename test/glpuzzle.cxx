//
// "$Id: glpuzzle.cxx,v 1.11 1999/11/13 23:01:18 carl Exp $"
//
// OpenGL puzzle demo for the Fast Light Tool Kit (FLTK).
// This is rewritten to remove use of the Glut emulation so it can
// demonstrate an Fl_Gl_Window subclass.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glu.h>

////////////////////////////////////////////////////////////////
/*
 * (c) Copyright 1993, 1994, Silicon Graphics, Inc.
 * Trackball code:
 *
 * Implementation of a virtual trackball.
 * Implemented by Gavin Bell, lots of ideas from Thant Tessman and
 *   the August '88 issue of Siggraph's "Computer Graphics," pp. 121-129.
 *
 * Vector manip code:
 *
 * Original code from:
 * David M. Ciemiewicz, Mark Grossman, Henry Moreton, and Paul Haeberli
 *
 * Much mucking with by:
 * Gavin Bell
 */

/*
 * Pass the x and y coordinates of the last and current positions of
 * the mouse, scaled so they are from (-1.0 ... 1.0).
 *
 * The resulting rotation is returned as a quaternion rotation in the
 * first paramater.
 */
void
trackball(float q[4], float p1x, float p1y, float p2x, float p2y);

/*
 * Given two quaternions, add them together to get a third quaternion.
 * Adding quaternions to get a compound rotation is analagous to adding
 * translations to get a compound translation.  When incrementally
 * adding rotations, the first argument here should be the new
 * rotation, the second and third the total rotation (which will be
 * over-written with the resulting new total rotation).
 */
void
add_quats(float *q1, float *q2, float *dest);

/*
 * A useful function, builds a rotation matrix in Matrix based on
 * given quaternion.
 */
void
build_rotmatrix(float m[4][4], float q[4]);

/*
 * This function computes a quaternion based on an axis (defined by
 * the given vector) and an angle about which to rotate.  The angle is
 * expressed in radians.  The result is put into the third argument.
 */
void
axis_to_quat(float a[3], float phi, float q[4]);

/*
 * This size should really be based on the distance from the center of
 * rotation to the point on the object underneath the mouse.  That
 * point would then track the mouse as closely as possible.  This is a
 * simple example, though, so that is left as an Exercise for the
 * Programmer.
 */
#define TRACKBALLSIZE  (0.8)

/*
 * Local function prototypes (not defined in trackball.h)
 */
static float tb_project_to_sphere(float, float, float);
static void normalize_quat(float [4]);

void
vzero(float *v)
{
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;
}

void
vset(float *v, float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

void
vsub(const float *src1, const float *src2, float *dst)
{
    dst[0] = src1[0] - src2[0];
    dst[1] = src1[1] - src2[1];
    dst[2] = src1[2] - src2[2];
}

void
vcopy(const float *v1, float *v2)
{
    register int i;
    for (i = 0 ; i < 3 ; i++)
        v2[i] = v1[i];
}

void
vcross(const float *v1, const float *v2, float *cross)
{
    float temp[3];

    temp[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    temp[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    temp[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
    vcopy(temp, cross);
}

float
vlength(const float *v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void
vscale(float *v, float div)
{
    v[0] *= div;
    v[1] *= div;
    v[2] *= div;
}

void
vnormal(float *v)
{
    vscale(v,1.0/vlength(v));
}

float
vdot(const float *v1, const float *v2)
{
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

void
vadd(const float *src1, const float *src2, float *dst)
{
    dst[0] = src1[0] + src2[0];
    dst[1] = src1[1] + src2[1];
    dst[2] = src1[2] + src2[2];
}

/*
 * Ok, simulate a track-ball.  Project the points onto the virtual
 * trackball, then figure out the axis of rotation, which is the cross
 * product of P1 P2 and O P1 (O is the center of the ball, 0,0,0)
 * Note:  This is a deformed trackball-- is a trackball in the center,
 * but is deformed into a hyperbolic sheet of rotation away from the
 * center.  This particular function was chosen after trying out
 * several variations.
 *
 * It is assumed that the arguments to this routine are in the range
 * (-1.0 ... 1.0)
 */
void
trackball(float q[4], float p1x, float p1y, float p2x, float p2y)
{
    float a[3]; /* Axis of rotation */
    float phi;  /* how much to rotate about axis */
    float p1[3], p2[3], d[3];
    float t;

    if (p1x == p2x && p1y == p2y) {
        /* Zero rotation */
        vzero(q);
        q[3] = 1.0;
        return;
    }

    /*
     * First, figure out z-coordinates for projection of P1 and P2 to
     * deformed sphere
     */
    vset(p1,p1x,p1y,tb_project_to_sphere(TRACKBALLSIZE,p1x,p1y));
    vset(p2,p2x,p2y,tb_project_to_sphere(TRACKBALLSIZE,p2x,p2y));

    /*
     *  Now, we want the cross product of P1 and P2
     */
    vcross(p2,p1,a);

    /*
     *  Figure out how much to rotate around that axis.
     */
    vsub(p1,p2,d);
    t = vlength(d) / (2.0*TRACKBALLSIZE);

    /*
     * Avoid problems with out-of-control values...
     */
    if (t > 1.0) t = 1.0;
    if (t < -1.0) t = -1.0;
    phi = 2.0 * asin(t);

    axis_to_quat(a,phi,q);
}

/*
 *  Given an axis and angle, compute quaternion.
 */
void
axis_to_quat(float a[3], float phi, float q[4])
{
    vnormal(a);
    vcopy(a,q);
    vscale(q,sin(phi/2.0));
    q[3] = cos(phi/2.0);
}

/*
 * Project an x,y pair onto a sphere of radius r OR a hyperbolic sheet
 * if we are away from the center of the sphere.
 */
static float
tb_project_to_sphere(float r, float x, float y)
{
    float d, t, z;

    d = sqrt(x*x + y*y);
    if (d < r * 0.70710678118654752440) {    /* Inside sphere */
        z = sqrt(r*r - d*d);
    } else {           /* On hyperbola */
        t = r / 1.41421356237309504880;
        z = t*t / d;
    }
    return z;
}

/*
 * Given two rotations, e1 and e2, expressed as quaternion rotations,
 * figure out the equivalent single rotation and stuff it into dest.
 *
 * This routine also normalizes the result every RENORMCOUNT times it is
 * called, to keep error from creeping in.
 *
 * NOTE: This routine is written so that q1 or q2 may be the same
 * as dest (or each other).
 */

#define RENORMCOUNT 97

void
add_quats(float q1[4], float q2[4], float dest[4])
{
    static int count=0;
    float t1[4], t2[4], t3[4];
    float tf[4];

    vcopy(q1,t1);
    vscale(t1,q2[3]);

    vcopy(q2,t2);
    vscale(t2,q1[3]);

    vcross(q2,q1,t3);
    vadd(t1,t2,tf);
    vadd(t3,tf,tf);
    tf[3] = q1[3] * q2[3] - vdot(q1,q2);

    dest[0] = tf[0];
    dest[1] = tf[1];
    dest[2] = tf[2];
    dest[3] = tf[3];

    if (++count > RENORMCOUNT) {
        count = 0;
        normalize_quat(dest);
    }
}

/*
 * Quaternions always obey:  a^2 + b^2 + c^2 + d^2 = 1.0
 * If they don't add up to 1.0, dividing by their magnitued will
 * renormalize them.
 *
 * Note: See the following for more information on quaternions:
 *
 * - Shoemake, K., Animating rotation with quaternion curves, Computer
 *   Graphics 19, No 3 (Proc. SIGGRAPH'85), 245-254, 1985.
 * - Pletinckx, D., Quaternion calculus as a basic tool in computer
 *   graphics, The Visual Computer 5, 2-13, 1989.
 */
static void
normalize_quat(float q[4])
{
    int i;
    float mag;

    mag = (q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
    for (i = 0; i < 4; i++) q[i] /= mag;
}

/*
 * Build a rotation matrix, given a quaternion rotation.
 *
 */
void
build_rotmatrix(float m[4][4], float q[4])
{
    m[0][0] = 1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]);
    m[0][1] = 2.0 * (q[0] * q[1] - q[2] * q[3]);
    m[0][2] = 2.0 * (q[2] * q[0] + q[1] * q[3]);
    m[0][3] = 0.0;

    m[1][0] = 2.0 * (q[0] * q[1] + q[2] * q[3]);
    m[1][1]= 1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]);
    m[1][2] = 2.0 * (q[1] * q[2] - q[0] * q[3]);
    m[1][3] = 0.0;

    m[2][0] = 2.0 * (q[2] * q[0] - q[1] * q[3]);
    m[2][1] = 2.0 * (q[1] * q[2] + q[0] * q[3]);
    m[2][2] = 1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]);
    m[2][3] = 0.0;

    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[3][3] = 1.0;
}

// end of trackball code
////////////////////////////////////////////////////////////////

#define WIDTH 4
#define HEIGHT 5
#define PIECES 10
#define OFFSETX -2
#define OFFSETY -2.5
#define OFFSETZ -0.5

typedef char Config[HEIGHT][WIDTH];

struct puzzle {
  struct puzzle *backptr;
  struct puzzle *solnptr;
  Config pieces;
  struct puzzle *next;
  unsigned hashvalue;
};

#define HASHSIZE 10691

struct puzzlelist {
  struct puzzle *puzzle;
  struct puzzlelist *next;
};

static char convert[PIECES + 1] =
{0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 4};

static unsigned char colors[PIECES + 1][3] =
{
  {0, 0, 0},
  {255, 255, 127},
  {255, 255, 127},
  {255, 255, 127},
  {255, 255, 127},
  {255, 127, 255},
  {255, 127, 255},
  {255, 127, 255},
  {255, 127, 255},
  {255, 127, 127},
  {255, 255, 255},
};

void changeState(void);

static struct puzzle *hashtable[HASHSIZE];
static struct puzzle *startPuzzle;
static struct puzzlelist *puzzles;
static struct puzzlelist *lastentry;

int curX, curY;

#define MOVE_SPEED 0.2
static unsigned char movingPiece;
static float move_x, move_y;
static float curquat[4];

static char xsize[PIECES + 1] =
{0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2};
static char ysize[PIECES + 1] =
{0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 2};
static float zsize[PIECES + 1] =
{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.6};

static Config startConfig =
{
  {8, 10, 10, 7},
  {8, 10, 10, 7},
  {6, 9, 9, 5},
  {6, 4, 3, 5},
  {2, 0, 0, 1}
};

static Config thePuzzle =
{
  {8, 10, 10, 7},
  {8, 10, 10, 7},
  {6, 9, 9, 5},
  {6, 4, 3, 5},
  {2, 0, 0, 1}
};

static int xadds[4] =
{-1, 0, 1, 0};
static int yadds[4] =
{0, -1, 0, 1};

static long W = 400, H = 300;
static GLint viewport[4];

#define srandom srand
#define random() (rand() >> 2)

unsigned
hash(Config config)
{
  int i, j, value;

  value = 0;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      value = value + convert[config[i][j]];
      value *= 6;
    }
  }
  return (value);
}

int
solution(Config config)
{
  if (config[4][1] == 10 && config[4][2] == 10)
    return (1);
  return (0);
}

float boxcoords[][3] =
{
  {0.2, 0.2, 0.9},
  {0.8, 0.2, 0.9},
  {0.8, 0.8, 0.9},
  {0.2, 0.8, 0.9},
  {0.2, 0.1, 0.8},
  {0.8, 0.1, 0.8},
  {0.9, 0.2, 0.8},
  {0.9, 0.8, 0.8},
  {0.8, 0.9, 0.8},
  {0.2, 0.9, 0.8},
  {0.1, 0.8, 0.8},
  {0.1, 0.2, 0.8},
  {0.2, 0.1, 0.2},
  {0.8, 0.1, 0.2},
  {0.9, 0.2, 0.2},
  {0.9, 0.8, 0.2},
  {0.8, 0.9, 0.2},
  {0.2, 0.9, 0.2},
  {0.1, 0.8, 0.2},
  {0.1, 0.2, 0.2},
  {0.2, 0.2, 0.1},
  {0.8, 0.2, 0.1},
  {0.8, 0.8, 0.1},
  {0.2, 0.8, 0.1},
};

float boxnormals[][3] =
{
  {0, 0, 1},            /* 0 */
  {0, 1, 0},
  {1, 0, 0},
  {0, 0, -1},
  {0, -1, 0},
  {-1, 0, 0},
  {0.7071, 0.7071, 0.0000},  /* 6 */
  {0.7071, -0.7071, 0.0000},
  {-0.7071, 0.7071, 0.0000},
  {-0.7071, -0.7071, 0.0000},
  {0.7071, 0.0000, 0.7071},  /* 10 */
  {0.7071, 0.0000, -0.7071},
  {-0.7071, 0.0000, 0.7071},
  {-0.7071, 0.0000, -0.7071},
  {0.0000, 0.7071, 0.7071},  /* 14 */
  {0.0000, 0.7071, -0.7071},
  {0.0000, -0.7071, 0.7071},
  {0.0000, -0.7071, -0.7071},
  {0.5774, 0.5774, 0.5774},  /* 18 */
  {0.5774, 0.5774, -0.5774},
  {0.5774, -0.5774, 0.5774},
  {0.5774, -0.5774, -0.5774},
  {-0.5774, 0.5774, 0.5774},
  {-0.5774, 0.5774, -0.5774},
  {-0.5774, -0.5774, 0.5774},
  {-0.5774, -0.5774, -0.5774},
};

int boxfaces[][4] =
{
  {0, 1, 2, 3},         /* 0 */
  {9, 8, 16, 17},
  {6, 14, 15, 7},
  {20, 23, 22, 21},
  {12, 13, 5, 4},
  {19, 11, 10, 18},
  {7, 15, 16, 8},       /* 6 */
  {13, 14, 6, 5},
  {18, 10, 9, 17},
  {19, 12, 4, 11},
  {1, 6, 7, 2},         /* 10 */
  {14, 21, 22, 15},
  {11, 0, 3, 10},
  {20, 19, 18, 23},
  {3, 2, 8, 9},         /* 14 */
  {17, 16, 22, 23},
  {4, 5, 1, 0},
  {20, 21, 13, 12},
  {2, 7, 8, -1},        /* 18 */
  {16, 15, 22, -1},
  {5, 6, 1, -1},
  {13, 21, 14, -1},
  {10, 3, 9, -1},
  {18, 17, 23, -1},
  {11, 4, 0, -1},
  {20, 12, 19, -1},
};

#define NBOXFACES (sizeof(boxfaces)/sizeof(boxfaces[0]))

/* Draw a box.  Bevel as desired. */
void
drawBox(int piece, float xoff, float yoff)
{
  int xlen, ylen;
  int i, k;
  float x, y, z;
  float zlen;
  float *v;

  xlen = xsize[piece];
  ylen = ysize[piece];
  zlen = zsize[piece];

  glColor3ubv(colors[piece]);
  glBegin(GL_QUADS);
  for (i = 0; i < 18; i++) {
    glNormal3fv(boxnormals[i]);
    for (k = 0; k < 4; k++) {
      if (boxfaces[i][k] == -1)
        continue;
      v = boxcoords[boxfaces[i][k]];
      x = v[0] + OFFSETX;
      if (v[0] > 0.5)
        x += xlen - 1;
      y = v[1] + OFFSETY;
      if (v[1] > 0.5)
        y += ylen - 1;
      z = v[2] + OFFSETZ;
      if (v[2] > 0.5)
        z += zlen - 1;
      glVertex3f(xoff + x, yoff + y, z);
    }
  }
  glEnd();
  glBegin(GL_TRIANGLES);
  for (i = 18; i < int(NBOXFACES); i++) {
    glNormal3fv(boxnormals[i]);
    for (k = 0; k < 3; k++) {
      if (boxfaces[i][k] == -1)
        continue;
      v = boxcoords[boxfaces[i][k]];
      x = v[0] + OFFSETX;
      if (v[0] > 0.5)
        x += xlen - 1;
      y = v[1] + OFFSETY;
      if (v[1] > 0.5)
        y += ylen - 1;
      z = v[2] + OFFSETZ;
      if (v[2] > 0.5)
        z += zlen - 1;
      glVertex3f(xoff + x, yoff + y, z);
    }
  }
  glEnd();
}

float containercoords[][3] =
{
  {-0.1, -0.1, 1.0},
  {-0.1, -0.1, -0.1},
  {4.1, -0.1, -0.1},
  {4.1, -0.1, 1.0},
  {1.0, -0.1, 0.6},     /* 4 */
  {3.0, -0.1, 0.6},
  {1.0, -0.1, 0.0},
  {3.0, -0.1, 0.0},
  {1.0, 0.0, 0.0},      /* 8 */
  {3.0, 0.0, 0.0},
  {3.0, 0.0, 0.6},
  {1.0, 0.0, 0.6},
  {0.0, 0.0, 1.0},      /* 12 */
  {4.0, 0.0, 1.0},
  {4.0, 0.0, 0.0},
  {0.0, 0.0, 0.0},
  {0.0, 5.0, 0.0},      /* 16 */
  {0.0, 5.0, 1.0},
  {4.0, 5.0, 1.0},
  {4.0, 5.0, 0.0},
  {-0.1, 5.1, -0.1},    /* 20 */
  {4.1, 5.1, -0.1},
  {4.1, 5.1, 1.0},
  {-0.1, 5.1, 1.0},
};

float containernormals[][3] =
{
  {0, -1, 0},
  {0, -1, 0},
  {0, -1, 0},
  {0, -1, 0},
  {0, -1, 0},
  {0, 1, 0},
  {0, 1, 0},
  {0, 1, 0},
  {1, 0, 0},
  {1, 0, 0},
  {1, 0, 0},
  {-1, 0, 0},
  {-1, 0, 0},
  {-1, 0, 0},
  {0, 1, 0},
  {0, 0, -1},
  {0, 0, -1},
  {0, 0, 1},
  {0, 0, 1},
  {0, 0, 1},
  {0, 0, 1},
  {0, 0, 1},
  {0, 0, 1},
  {0, 0, 1},
};

int containerfaces[][4] =
{
  {1, 6, 4, 0},
  {0, 4, 5, 3},
  {1, 2, 7, 6},
  {7, 2, 3, 5},
  {16, 19, 18, 17},

  {23, 22, 21, 20},
  {12, 11, 8, 15},
  {10, 13, 14, 9},

  {15, 16, 17, 12},
  {2, 21, 22, 3},
  {6, 8, 11, 4},

  {1, 0, 23, 20},
  {14, 13, 18, 19},
  {9, 7, 5, 10},

  {12, 13, 10, 11},

  {1, 20, 21, 2},
  {4, 11, 10, 5},

  {15, 8, 19, 16},
  {19, 8, 9, 14},
  {8, 6, 7, 9},
  {0, 3, 13, 12},
  {13, 3, 22, 18},
  {18, 22, 23, 17},
  {17, 23, 0, 12},
};

#define NCONTFACES (sizeof(containerfaces)/sizeof(containerfaces[0]))

/* Draw the container */
void
drawContainer(void)
{
  int i, k;
  float *v;

  /* Y is reversed here because the model has it reversed */

  /* Arbitrary bright wood-like color */
  glColor3ub(209, 103, 23);
  glBegin(GL_QUADS);
  for (i = 0; i < int(NCONTFACES); i++) {
    v = containernormals[i];
    glNormal3f(v[0], -v[1], v[2]);
    for (k = 3; k >= 0; k--) {
      v = containercoords[containerfaces[i][k]];
      glVertex3f(v[0] + OFFSETX, -(v[1] + OFFSETY), v[2] + OFFSETZ);
    }
  }
  glEnd();
}

void
drawAll(void)
{
  int i, j;
  int piece;
  char done[PIECES + 1];
  float m[4][4];

  build_rotmatrix(m, curquat);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -10);
  glMultMatrixf(&(m[0][0]));
  glRotatef(180, 0, 0, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (i = 1; i <= PIECES; i++) {
    done[i] = 0;
  }
  glLoadName(0);
  drawContainer();
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      piece = thePuzzle[i][j];
      if (piece == 0)
        continue;
      if (done[piece])
        continue;
      done[piece] = 1;
      glLoadName(piece);
      if (piece == movingPiece) {
        drawBox(piece, move_x, move_y);
      } else {
        drawBox(piece, j, i);
      }
    }
  }
}

class Puzzle_Window : public Fl_Gl_Window {
public:
  void draw();
  int handle(int);
  Puzzle_Window(int W, int H) : Fl_Gl_Window(W,H) {}
} *puzzle_window;

void
init(void)
{
  static float lmodel_ambient[] =
  {0.0, 0.0, 0.0, 0.0};
  static float lmodel_twoside[] =
  {GL_FALSE};
  static float lmodel_local[] =
  {GL_FALSE};
  static float light0_ambient[] =
  {0.1, 0.1, 0.1, 1.0};
  static float light0_diffuse[] =
  {1.0, 1.0, 1.0, 0.0};
  static float light0_position[] =
  {0.8660254, 0.5, 1, 0};
  static float light0_specular[] =
  {0.0, 0.0, 0.0, 0.0};
  static float bevel_mat_ambient[] =
  {0.0, 0.0, 0.0, 1.0};
  static float bevel_mat_shininess[] =
  {40.0};
  static float bevel_mat_specular[] =
  {0.0, 0.0, 0.0, 0.0};
  static float bevel_mat_diffuse[] =
  {1.0, 0.0, 0.0, 0.0};

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);

  glClearColor(0.5, 0.5, 0.5, 0.0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glEnable(GL_LIGHT0);

  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lmodel_local);
  glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glEnable(GL_LIGHTING);

  glMaterialfv(GL_FRONT, GL_AMBIENT, bevel_mat_ambient);
  glMaterialfv(GL_FRONT, GL_SHININESS, bevel_mat_shininess);
  glMaterialfv(GL_FRONT, GL_SPECULAR, bevel_mat_specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, bevel_mat_diffuse);

  glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glShadeModel(GL_FLAT);
}

void Puzzle_Window::draw() {
  if (!valid()) {
    ::init(); // should only be called when context created, but
	      // fltk does not give an indication of that...
    W = w();
    H = h();
    glViewport(0, 0, W, H);
    glGetIntegerv(GL_VIEWPORT, viewport);
  }
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, viewport[2]*1.0/viewport[3], 0.1, 100.0);
  drawAll();
}

void
solidifyChain(struct puzzle *puzzle)
{
  static char buf[30];
  int i = 0;
  while (puzzle->backptr) {
    i++;
    puzzle->backptr->solnptr = puzzle;
    puzzle = puzzle->backptr;
  }
  sprintf(buf, "%d moves to complete!", i);
  puzzle_window->label(buf);
}

int
addConfig(Config config, struct puzzle *back)
{
  unsigned hashvalue;
  struct puzzle *newpiece;
  struct puzzlelist *newlistentry;

  hashvalue = hash(config);

  newpiece = hashtable[hashvalue % HASHSIZE];
  while (newpiece != NULL) {
    if (newpiece->hashvalue == hashvalue) {
      int i, j;

      for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < HEIGHT; j++) {
          if (convert[config[j][i]] !=
            convert[newpiece->pieces[j][i]])
            goto nomatch;
        }
      }
      return 0;
    }
  nomatch:
    newpiece = newpiece->next;
  }

  newpiece = (struct puzzle *) malloc(sizeof(struct puzzle));
  newpiece->next = hashtable[hashvalue % HASHSIZE];
  newpiece->hashvalue = hashvalue;
  memcpy(newpiece->pieces, config, HEIGHT * WIDTH);
  newpiece->backptr = back;
  newpiece->solnptr = NULL;
  hashtable[hashvalue % HASHSIZE] = newpiece;

  newlistentry = (struct puzzlelist *) malloc(sizeof(struct puzzlelist));
  newlistentry->puzzle = newpiece;
  newlistentry->next = NULL;

  if (lastentry) {
    lastentry->next = newlistentry;
  } else {
    puzzles = newlistentry;
  }
  lastentry = newlistentry;

  if (back == NULL) {
    startPuzzle = newpiece;
  }
  if (solution(config)) {
    solidifyChain(newpiece);
    return 1;
  }
  return 0;
}

/* Checks if a space can move */
int
canmove0(Config pieces, int x, int y, int dir, Config newpieces)
{
  char piece;
  int xadd, yadd;
  int l, m;

  xadd = xadds[dir];
  yadd = yadds[dir];

  if (x + xadd < 0 || x + xadd >= WIDTH ||
    y + yadd < 0 || y + yadd >= HEIGHT)
    return 0;
  piece = pieces[y + yadd][x + xadd];
  if (piece == 0)
    return 0;
  memcpy(newpieces, pieces, HEIGHT * WIDTH);
  for (l = 0; l < WIDTH; l++) {
    for (m = 0; m < HEIGHT; m++) {
      if (newpieces[m][l] == piece)
        newpieces[m][l] = 0;
    }
  }
  xadd = -xadd;
  yadd = -yadd;
  for (l = 0; l < WIDTH; l++) {
    for (m = 0; m < HEIGHT; m++) {
      if (pieces[m][l] == piece) {
        int newx, newy;

        newx = l + xadd;
        newy = m + yadd;
        if (newx < 0 || newx >= WIDTH ||
          newy < 0 || newy >= HEIGHT)
          return 0;
        if (newpieces[newy][newx] != 0)
          return 0;
        newpieces[newy][newx] = piece;
      }
    }
  }
  return 1;
}

/* Checks if a piece can move */
int
canmove(Config pieces, int x, int y, int dir, Config newpieces)
{
  int xadd, yadd;

  xadd = xadds[dir];
  yadd = yadds[dir];

  if (x + xadd < 0 || x + xadd >= WIDTH ||
    y + yadd < 0 || y + yadd >= HEIGHT)
    return 0;
  if (pieces[y + yadd][x + xadd] == pieces[y][x]) {
    return canmove(pieces, x + xadd, y + yadd, dir, newpieces);
  }
  if (pieces[y + yadd][x + xadd] != 0)
    return 0;
  return canmove0(pieces, x + xadd, y + yadd, (dir + 2) % 4, newpieces);
}

int
generateNewConfigs(struct puzzle *puzzle)
{
  int i, j, k;
  Config pieces;
  Config newpieces;

  memcpy(pieces, puzzle->pieces, HEIGHT * WIDTH);
  for (i = 0; i < WIDTH; i++) {
    for (j = 0; j < HEIGHT; j++) {
      if (pieces[j][i] == 0) {
        for (k = 0; k < 4; k++) {
          if (canmove0(pieces, i, j, k, newpieces)) {
            if (addConfig(newpieces, puzzle))
              return 1;
          }
        }
      }
    }
  }
  return 0;
}

void
freeSolutions(void)
{
  struct puzzlelist *nextpuz;
  struct puzzle *puzzle, *next;
  int i;

  while (puzzles) {
    nextpuz = puzzles->next;
    free((char *) puzzles);
    puzzles = nextpuz;
  }
  lastentry = NULL;
  for (i = 0; i < HASHSIZE; i++) {
    puzzle = hashtable[i];
    hashtable[i] = NULL;
    while (puzzle) {
      next = puzzle->next;
      free((char *) puzzle);
      puzzle = next;
    }
  }
  startPuzzle = NULL;
}

int
continueSolving(void)
{
  struct puzzle *nextpuz;
  int i, j;
  int movedPiece;
  int movedir;
  int fromx, fromy;
  int tox, toy;

  if (startPuzzle == NULL)
    return 0;
  if (startPuzzle->solnptr == NULL) {
    freeSolutions();
    return 0;
  }
  nextpuz = startPuzzle->solnptr;
  movedPiece = 0;
  movedir = 0;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      if (startPuzzle->pieces[i][j] != nextpuz->pieces[i][j]) {
        if (startPuzzle->pieces[i][j]) {
          movedPiece = startPuzzle->pieces[i][j];
          fromx = j;
          fromy = i;
          if (i < HEIGHT - 1 && nextpuz->pieces[i + 1][j] == movedPiece) {
            movedir = 3;
          } else {
            movedir = 2;
          }
          goto found_piece;
        } else {
          movedPiece = nextpuz->pieces[i][j];
          if (i < HEIGHT - 1 &&
            startPuzzle->pieces[i + 1][j] == movedPiece) {
            fromx = j;
            fromy = i + 1;
            movedir = 1;
          } else {
            fromx = j + 1;
            fromy = i;
            movedir = 0;
          }
          goto found_piece;
        }
      }
    }
  }
  puzzle_window->label("What!  No change?");
  freeSolutions();
  return 0;

found_piece:
  if (!movingPiece) {
    movingPiece = movedPiece;
    move_x = fromx;
    move_y = fromy;
  }
  move_x += xadds[movedir] * MOVE_SPEED;
  move_y += yadds[movedir] * MOVE_SPEED;

  tox = fromx + xadds[movedir];
  toy = fromy + yadds[movedir];

  if (move_x > tox - MOVE_SPEED / 2 && move_x < tox + MOVE_SPEED / 2 &&
    move_y > toy - MOVE_SPEED / 2 && move_y < toy + MOVE_SPEED / 2) {
    startPuzzle = nextpuz;
    movingPiece = 0;
  }
  memcpy(thePuzzle, startPuzzle->pieces, HEIGHT * WIDTH);
  changeState();
  return 1;
}

int
solvePuzzle(void)
{
  struct puzzlelist *nextpuz;
  char buf[256];
  int i;

  if (solution(thePuzzle)) {
    puzzle_window->label("Puzzle already solved!");
    return 0;
  }
  addConfig(thePuzzle, NULL);
  i = 0;

  while (puzzles) {
    i++;
    if (generateNewConfigs(puzzles->puzzle))
      break;
    nextpuz = puzzles->next;
    free((char *) puzzles);
    puzzles = nextpuz;
  }
  if (puzzles == NULL) {
    freeSolutions();
    sprintf(buf, "I can't solve it! (%d positions examined)", i);
    puzzle_window->label(buf);
    return 1;
  }
  return 1;
}

int
selectPiece(int mousex, int mousey)
{
  puzzle_window->make_current();
  long hits;
  GLuint selectBuf[1024];
  GLuint closest;
  GLuint dist;

  glSelectBuffer(1024, selectBuf);
  (void) glRenderMode(GL_SELECT);
  glInitNames();

  /* Because LoadName() won't work with no names on the stack */
  glPushName(0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix(mousex, H - mousey, 4, 4, viewport);
  gluPerspective(45, viewport[2]*1.0/viewport[3], 0.1, 100.0);

  drawAll();

  hits = glRenderMode(GL_RENDER);
  if (hits <= 0) {
    return 0;
  }
  closest = 0;
  dist = 0xFFFFFFFFU; //2147483647;
  while (hits) {
    if (selectBuf[(hits - 1) * 4 + 1] < dist) {
      dist = selectBuf[(hits - 1) * 4 + 1];
      closest = selectBuf[(hits - 1) * 4 + 3];
    }
    hits--;
  }
  return closest;
}

void
nukePiece(int piece)
{
  int i, j;

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      if (thePuzzle[i][j] == piece) {
        thePuzzle[i][j] = 0;
      }
    }
  }
}

void
multMatrices(const GLfloat a[16], const GLfloat b[16], GLfloat r[16])
{
  int i, j;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      r[i * 4 + j] =
        a[i * 4 + 0] * b[0 * 4 + j] +
        a[i * 4 + 1] * b[1 * 4 + j] +
        a[i * 4 + 2] * b[2 * 4 + j] +
        a[i * 4 + 3] * b[3 * 4 + j];
    }
  }
}

void
makeIdentity(GLfloat m[16])
{
  m[0 + 4 * 0] = 1;
  m[0 + 4 * 1] = 0;
  m[0 + 4 * 2] = 0;
  m[0 + 4 * 3] = 0;
  m[1 + 4 * 0] = 0;
  m[1 + 4 * 1] = 1;
  m[1 + 4 * 2] = 0;
  m[1 + 4 * 3] = 0;
  m[2 + 4 * 0] = 0;
  m[2 + 4 * 1] = 0;
  m[2 + 4 * 2] = 1;
  m[2 + 4 * 3] = 0;
  m[3 + 4 * 0] = 0;
  m[3 + 4 * 1] = 0;
  m[3 + 4 * 2] = 0;
  m[3 + 4 * 3] = 1;
}

/*
   ** inverse = invert(src)
 */
int
invertMatrix(const GLfloat src[16], GLfloat inverse[16])
{
  int i, j, k, swap;
  double t;
  GLfloat temp[4][4];

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      temp[i][j] = src[i * 4 + j];
    }
  }
  makeIdentity(inverse);

  for (i = 0; i < 4; i++) {
    /* 
       ** Look for largest element in column */
    swap = i;
    for (j = i + 1; j < 4; j++) {
      if (fabs(temp[j][i]) > fabs(temp[i][i])) {
        swap = j;
      }
    }

    if (swap != i) {
      /* 
         ** Swap rows. */
      for (k = 0; k < 4; k++) {
        t = temp[i][k];
        temp[i][k] = temp[swap][k];
        temp[swap][k] = t;

        t = inverse[i * 4 + k];
        inverse[i * 4 + k] = inverse[swap * 4 + k];
        inverse[swap * 4 + k] = t;
      }
    }
    if (temp[i][i] == 0) {
      /* 
         ** No non-zero pivot.  The matrix is singular, which
         shouldn't ** happen.  This means the user gave us a
         bad matrix. */
      return 0;
    }
    t = temp[i][i];
    for (k = 0; k < 4; k++) {
      temp[i][k] /= t;
      inverse[i * 4 + k] /= t;
    }
    for (j = 0; j < 4; j++) {
      if (j != i) {
        t = temp[j][i];
        for (k = 0; k < 4; k++) {
          temp[j][k] -= temp[i][k] * t;
          inverse[j * 4 + k] -= inverse[i * 4 + k] * t;
        }
      }
    }
  }
  return 1;
}

/*
   ** This is a screwball function.  What it does is the following:
   ** Given screen x and y coordinates, compute the corresponding object space 
   **   x and y coordinates given that the object space z is 0.9 + OFFSETZ.
   ** Since the tops of (most) pieces are at z = 0.9 + OFFSETZ, we use that 
   **   number.
 */
int
computeCoords(int piece, int mousex, int mousey,
  GLfloat * selx, GLfloat * sely)
{
  GLfloat modelMatrix[16];
  GLfloat projMatrix[16];
  GLfloat finalMatrix[16];
  GLfloat in[4];
  GLfloat a, b, c, d;
  GLfloat top, bot;
  GLfloat z;
  GLfloat w;
  GLfloat height;

  if (piece == 0)
    return 0;
  height = zsize[piece] - 0.1 + OFFSETZ;

  glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
  glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
  multMatrices(modelMatrix, projMatrix, finalMatrix);
  if (!invertMatrix(finalMatrix, finalMatrix))
    return 0;

  in[0] = (2.0 * (mousex - viewport[0]) / viewport[2]) - 1;
  in[1] = (2.0 * ((H - mousey) - viewport[1]) / viewport[3]) - 1;

  a = in[0] * finalMatrix[0 * 4 + 2] +
    in[1] * finalMatrix[1 * 4 + 2] +
    finalMatrix[3 * 4 + 2];
  b = finalMatrix[2 * 4 + 2];
  c = in[0] * finalMatrix[0 * 4 + 3] +
    in[1] * finalMatrix[1 * 4 + 3] +
    finalMatrix[3 * 4 + 3];
  d = finalMatrix[2 * 4 + 3];

  /* 
     ** Ok, now we need to solve for z: **   (a + b z) / (c + d 

     z) = height. ** ("height" is the height in object space we 

     want to solve z for) ** ** ==>  a + b z = height c +
     height d z **      bz - height d z = height c - a ** z =
     (height c - a) / (b - height d) */
  top = height * c - a;
  bot = b - height * d;
  if (bot == 0.0)
    return 0;

  z = top / bot;

  /* 
     ** Ok, no problem. ** Now we solve for x and y.  We know
     that w = c + d z, so we compute it. */
  w = c + d * z;

  /* 
     ** Now for x and y: */
  *selx = (in[0] * finalMatrix[0 * 4 + 0] +
    in[1] * finalMatrix[1 * 4 + 0] +
    z * finalMatrix[2 * 4 + 0] +
    finalMatrix[3 * 4 + 0]) / w - OFFSETX;
  *sely = (in[0] * finalMatrix[0 * 4 + 1] +
    in[1] * finalMatrix[1 * 4 + 1] +
    z * finalMatrix[2 * 4 + 1] +
    finalMatrix[3 * 4 + 1]) / w - OFFSETY;
  return 1;
}

static int selected;
static int selectx, selecty;
static float selstartx, selstarty;

void
grabPiece(int piece, float selx, float sely)
{
  int hit;

  selectx = int(selx);
  selecty = int(sely);
  if (selectx < 0 || selecty < 0 || selectx >= WIDTH || selecty >= HEIGHT) {
    return;
  }
  hit = thePuzzle[selecty][selectx];
  if (hit != piece)
    return;
  if (hit) {
    movingPiece = hit;
    while (selectx > 0 && thePuzzle[selecty][selectx - 1] == movingPiece) {
      selectx--;
    }
    while (selecty > 0 && thePuzzle[selecty - 1][selectx] == movingPiece) {
      selecty--;
    }
    move_x = selectx;
    move_y = selecty;
    selected = 1;
    selstartx = selx;
    selstarty = sely;
  } else {
    selected = 0;
  }
  changeState();
}

void
moveSelection(float selx, float sely)
{
  float deltax, deltay;
  int dir;
  Config newpieces;

  if (!selected)
    return;
  deltax = selx - selstartx;
  deltay = sely - selstarty;

  if (fabs(deltax) > fabs(deltay)) {
    deltay = 0;
    if (deltax > 0) {
      if (deltax > 1)
        deltax = 1;
      dir = 2;
    } else {
      if (deltax < -1)
        deltax = -1;
      dir = 0;
    }
  } else {
    deltax = 0;
    if (deltay > 0) {
      if (deltay > 1)
        deltay = 1;
      dir = 3;
    } else {
      if (deltay < -1)
        deltay = -1;
      dir = 1;
    }
  }
  if (canmove(thePuzzle, selectx, selecty, dir, newpieces)) {
    move_x = deltax + selectx;
    move_y = deltay + selecty;
    if (deltax > 0.5) {
      memcpy(thePuzzle, newpieces, HEIGHT * WIDTH);
      selectx++;
      selstartx++;
    } else if (deltax < -0.5) {
      memcpy(thePuzzle, newpieces, HEIGHT * WIDTH);
      selectx--;
      selstartx--;
    } else if (deltay > 0.5) {
      memcpy(thePuzzle, newpieces, HEIGHT * WIDTH);
      selecty++;
      selstarty++;
    } else if (deltay < -0.5) {
      memcpy(thePuzzle, newpieces, HEIGHT * WIDTH);
      selecty--;
      selstarty--;
    }
  } else {
    if (deltay > 0 && thePuzzle[selecty][selectx] == 10 &&
      selectx == 1 && selecty == 3) {
      /* Allow visual movement of solution piece outside of the 

         box */
      move_x = selectx;
      move_y = sely - selstarty + selecty;
    } else {
      move_x = selectx;
      move_y = selecty;
    }
  }
}

void
dropSelection(void)
{
  if (!selected)
    return;
  movingPiece = 0;
  selected = 0;
  changeState();
}

static int left_mouse, middle_mouse;
static int mousex, mousey;
static int solving;
static int spinning;
static float lastquat[4];
static int sel_piece;

#include <FL/Fl_Menu_Item.H>

void set_solving(int);

void solve_cb(Fl_Widget*, void*) {set_solving(!solving);}

void reset_cb(Fl_Widget*, void*) {
  set_solving(0);
  memcpy(thePuzzle, startConfig, HEIGHT * WIDTH);
  puzzle_window->redraw();
}

int piece;
void delete_cb(Fl_Widget*, void*) {
  set_solving(0);
  if (piece) {nukePiece(piece); puzzle_window->redraw();}
}

void quit_cb(Fl_Widget*, void*) {
  exit(0);
}

void reset_view_cb(Fl_Widget*, void*) {
  trackball(curquat, 0.0, 0.0, 0.0, 0.0);
  puzzle_window->redraw();
  spinning = false;
  changeState();
}

Fl_Menu_Item menu[] = {
  {"solve it for me", 's', solve_cb},
  {"reset the puzzle", 'r', reset_cb},
  {"delete this piece", 'd', delete_cb},
  {"reset view", 'o', reset_view_cb},
  {"quit", 'q', quit_cb},
  {0}};

void set_solving(int s) {
  if (s == solving) return;
  if (s) {
    menu[0].text = "stop solving";
    puzzle_window->cursor(FL_CURSOR_WAIT);
    Fl::flush();
    if (solvePuzzle()) solving = 1;
  } else {
    freeSolutions();
    solving = 0;
    menu[0].text = "solve it for me";
    puzzle_window->cursor(FL_CURSOR_DEFAULT);
    movingPiece = 0;
  }
  changeState();
}

int Puzzle_Window::handle(int event) {
  int x = Fl::event_x();
  int y = Fl::event_y();
  switch (event) {
  case FL_KEYBOARD:
    switch (Fl::event_key()) {
    case FL_Escape:
    case 'Q':
    case 'q':
      exit(0);
      break;
    case 'S':
    case 's':
      solve_cb(this,0);
      break;
    case 'D':
    case 'd':
      piece = selectPiece(x, y);
      delete_cb(this,0);
      break;
    case 'R':
    case 'r':
      reset_cb(this,0);
      break;
    case 'O':
    case 'o':
      reset_view_cb(this,0);
      break;
    default:
      break;
    }
    return 1;

  case FL_PUSH:
    mousex = curX = x;
    mousey = curY = y;
    switch (Fl::event_button()) {
    case 1:
      set_solving(0);
      left_mouse = true;
      sel_piece = selectPiece(mousex, mousey);
      if (!sel_piece) {
	left_mouse = false;
	middle_mouse = true; // let it rotate object
      } else {
	float selx, sely;
	if (computeCoords(sel_piece, mousex, mousey, &selx, &sely)) {
	  grabPiece(sel_piece, selx, sely);
	}
      }
      redraw();
      break;
    case 2:
      middle_mouse = true;
      redraw();
      break;
    default:
      piece = selectPiece(x, y);
      if (piece) menu[2].activate(); else menu[2].deactivate();
      const Fl_Menu_Item* m = menu->popup(x, y);
      if (m) m->do_callback(this);
      return 1;
    }
    // fall through to drag handler:

  case FL_DRAG:
    if (middle_mouse && !left_mouse) {
      if (mousex != x || mousey != y) {
	trackball(lastquat,
		  (2.0*mousex - W) / W,
		  (H - 2.0*mousey) / H,
		  (2.0*x - W) / W,
		  (H - 2.0*y) / H);
	spinning = 1;
      } else {
	spinning = 0;
      }
      changeState();
    } else {
      float selx, sely;
      computeCoords(sel_piece, x, y, &selx, &sely);
      moveSelection(selx, sely);
    }
    mousex = x;
    mousey = y;
    redraw();
    return 1;

  case FL_RELEASE:
    if (left_mouse) {
      left_mouse = false;
      dropSelection();
      redraw();
    } else if (middle_mouse) {
      middle_mouse = GL_FALSE;
      redraw();
    }
    return 1;

  }
  return Fl_Gl_Window::handle(event);
}


void
animate(void)
{
  if (spinning) {
    add_quats(lastquat, curquat, curquat);
  }
  puzzle_window->redraw();
  if (solving) {
    if (!continueSolving()) {
      set_solving(0);
    }
  }
  if (!solving && !spinning) {
    Fl::set_idle(0);
  }
}

void
changeState(void)
{
  if (puzzle_window->visible() && (solving || spinning)) {
    Fl::set_idle(animate);
  } else {
    Fl::set_idle(0);
  }
}

int
main(int argc, char **argv)
{
  puzzle_window = new Puzzle_Window(W,H);
  puzzle_window->resizable(puzzle_window);
  puzzle_window->show(argc, argv);
  trackball(curquat, 0.0, 0.0, 0.0, 0.0);
  srandom(time(NULL));
  return Fl::run();
}

//
// End of "$Id: glpuzzle.cxx,v 1.11 1999/11/13 23:01:18 carl Exp $".
//
