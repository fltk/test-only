/
// "$Id$
/
// Colormap generation program for the Fast Light Tool Kit (FLTK)
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
// Please report all bugs and problems to "fltk-bugs@easysw.com"
/

// This program produces the contents of "fl_cmap.h" as stdou

// #include <gl/gl.h
#include <stdio.h

// This table is initialized with color values I got by reading th
// colormap on an IRIX 4.3 machine

// "full intensity colors" have been turned down some to make whit
// background less intense by default.  The hope is that this will mak
// fltk programs more friendly on color-adjusted screens.  If you wan
// pure colors you should get them out of the colormap

//#define III 244 // maximum intensity of the basic color

// that results in errors and unshared colormap entries, so full intensity
#define III 255 // maximum intensity of the basic color

static short cmap[256][3] = 
// 3-bit colormap
  {  0,  0,  0},	// blac
  {III,  0,  0},	// re
  {  0,III,  0},	// gree
  {III,III,  0},	// yello
  {  0,  0,III},	// blu
  {III,  0,III},	// magent
  {  0,III,III},	// cya
  {III,III,III},	// whit
// pastel versions of those colors, from SGI's standard color map
  { 85, 85, 85},	// 1/3 gra
  {198,113,113},	// salmon? pale red
  {113,198,113},	// pale gree
  {142,142, 56},	// khak
  {113,113,198},	// pale blu
  {142, 56,142},	// purple, orchid, pale magent
  { 56,142,142},	// cadet blue, aquamarine, pale cya
// The next location is used for FL_SELECTION_COLOR. It formerly was 2/3 gra
// but this is changed to be the Windows blue color. This allows the defaul
// behavior on both X and Windows to match
  {  0,  0,128}
//{170,170,170},	// old 2/3 gray colo
// These next 16 are the FL_FREE_COLOR area. In some versions of flt
// these were filled with random colors that a Irix 5.3 machine place
// in these locations. Other versions of fltk filled this with the 1/
// gray above to discourage their use. This newest version uses color
// that NewTek has assigned for their GUI
#if 
  // The Irix 5.3 colors
  { 16, 16, 16}
  {128, 40,128}
  {198, 30, 30}
  { 66, 30, 30}
  {176,140,140}
  {  0, 20, 20}
  { 20, 10, 10}
  { 40, 20, 20}
  { 60, 30, 30}
  {  0, 80, 80}
  {  0, 40, 40}
  { 20, 20,  0}
  { 40, 40,  0}
  { 80, 80, 10}
  {150,150, 20}
  {160, 10, 10}
#els
  // The NewTek colors: (from George Yohng
  {168,168,152}
  {232,232,216}
  {104,104, 88}
  {152,168,168}
  {216,232,232}
  { 88,104,104}
  {156,156,168}
  {220,220,232}
  { 92, 92,104}
  {156,168,156}
  {220,232,220}
  { 92,104, 92}
  {144,144,144}
  {192,192,192}
  { 80, 80, 80}
  {160,160,160}
#endi
// The rest of the colormap is a gray ramp and table, filled in below
}

// This is Fl::background from Fl_get_system_colors.cxx, with modifications

#define FL_GRAY_RAMP 3
#define FL_NUM_GRAY  2
#define FL_GRAY 49 // old value is 4
typedef unsigned char uchar
#include <math.h

void background(uchar r, uchar g, uchar b) 
  // replace the gray ramp so that color 47 (by default 2/3) is this colo
  if (!r) r = 1; else if (r==255) r = 254
  double powr = log(r/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0))
  if (!g) g = 1; else if (g==255) g = 254
  double powg = log(g/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0))
  if (!b) b = 1; else if (b==255) b = 254
  double powb = log(b/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0))
  for (int i = 0; i < FL_NUM_GRAY; i++) 
    double gray = i/(FL_NUM_GRAY-1.0)
    cmap[i+FL_GRAY_RAMP][0] = uchar(pow(gray,powr)*255+.5)
    cmap[i+FL_GRAY_RAMP][1] = uchar(pow(gray,powg)*255+.5)
    cmap[i+FL_GRAY_RAMP][2] = uchar(pow(gray,powb)*255+.5)
  


int main() 
  int i,r,g,b
#if 
  /* Read colormap colors into internal table *
  long cmwin
  noport()
  cmwin = winopen("CM")
  for (i=0; i<256; i++
    getmcolor(i,&cmap[i][0],&cmap[i][1],&cmap[i][2])
  winclose(cmwin)
#endi
// overwrite the X allocation area with one color so people ar
// discouraged from using it
  //for (i=16; i<32; i++) {cmap[i][0]=cmap[i][1]=cmap[i][2] = 85;

  // fill in the gray ramp
  background(0xc0, 0xc0, 0xc0); // microsoft color
  // background(170, 170, 170); // old fltk color
  // copy the 1/3 and 2/3 gray to the closest locations in gray ramp
  cmap[39][0] = cmap[39][1] = cmap[39][2] = 85
  cmap[47][0] = cmap[47][1] = cmap[47][2] = 170

  // fill in the color cub
  i = 56
  for (b=0; b<5; b++
    for (r=0; r<5; r++
      for (g=0; g<8; g++) 
	cmap[i][0] = r*255/4
	cmap[i][1] = g*255/7
	cmap[i][2] = b*255/4
	i++
      

  for (i=0; i<256; i++) 
    printf("\t0x%02x%02x%02x00",cmap[i][0],cmap[i][1],cmap[i][2])
    if (i < 255) printf(",\n")
  
  printf("\n")
  return 0


/
// End of "$Id$"
/
