/
// "$Id$
/
// CubeView class definitions for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2003 by Bill Spitzak and others
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

#ifndef CUBEVIEW_
#define CUBEVIEW_H 
#include <config.h
#include <FL/Fl.H
#if HAVE_G
#  include <FL/Fl_Gl_Window.H
#  include <FL/gl.h
#els
#  include <FL/Fl_Box.H
#endif /* HAVE_GL *

#include <stdlib.h

#if HAVE_G
class CubeView : public Fl_Gl_Window 
#els
class CubeView : public Fl_Box 
#endif /* HAVE_GL *

public
    // this value determines the scaling factor used to draw the cube
    double size

    CubeView(int x,int y,int w,int h,const char *l=0)

    /* Set the rotation about the vertical (y ) axis
     
     * This function is called by the horizontal roller in CubeViewUI and th
     * initialize button in CubeViewUI
     *
    void v_angle(float angle){vAng=angle;}
   
    // Return the rotation about the vertical (y ) axis
    float v_angle(){return vAng;}

    /* Set the rotation about the horizontal (x ) axis
     
     * This function is called by the vertical roller in CubeViewUI and th
     * initialize button in CubeViewUI
     *

    void h_angle(float angle){hAng=angle;}

    // the rotation about the horizontal (x ) axis
    float h_angle(){return hAng;}

    /* Sets the x shift of the cube view camera
     
     * This function is called by the slider in CubeViewUI and th
     * initialize button in CubeViewUI
     *
    void panx(float x){xshift=x;}
    /* Sets the y shift of the cube view camera
     
     * This function is called by the slider in CubeViewUI and th
     * initialize button in CubeViewUI
     *
    void pany(float y){yshift=y;}

#if HAVE_G
    /*The widget class draw() override
     
     *The draw() function initialize Gl for another round o f drawin
     * then calls specialized functions for drawing each of th
     * entities displayed in the cube view
     
     *
    void draw();   
#endif /* HAVE_GL *
private

    /*  Draw the cube boundarie
     
     *Draw the faces of the cube using the boxv[] vertices, usin
     * GL_LINE_LOOP for the faces. The color is \#defined by CUBECOLOR
     *
#if HAVE_G
    void drawCube()
#els
    void drawCube() { 
#endif /* HAVE_GL *
   
    float vAng,hAng
    float xshift,yshift


    float boxv0[3];float boxv1[3]
    float boxv2[3];float boxv3[3]
    float boxv4[3];float boxv5[3]
    float boxv6[3];float boxv7[3]

}
#endi

/
// End of "$Id$"
/
