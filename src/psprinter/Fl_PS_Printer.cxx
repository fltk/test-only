/
// "$Id$
/
// Postscript device class for the Fast Light Tool Kit (FLTK)
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


#include <stdio.h
#include <math.h
#include <string.h

#include <FL/Fl_PS_Printer.H
#include <FL/Fl.H

#include <FL/Fl_Pixmap.H
#include <FL/Fl_Bitmap.H

#include <FL/Enumerations.H


//  Prolog string

static const char * prolog 
"%%%%BeginProlog\n
"/L { /y2 exch def\n
  "/x2 exch def\n
  "/y1 exch def\n
  "/x1 exch def\n
  "newpath   x1 y1 moveto x2 y2 lineto\n
  "stroke}\n
"bind def\n


"/R { /dy exch def\n
  "/dx exch def\n
  "/y exch def\n
  "/x exch def\n
  "newpath\n
  "x y moveto\n
  "dx 0 rlineto\n
  "0 dy rlineto\n
  "dx neg 0 rlineto\n
  "closepath stroke\n
"} bind def\n

"/CL {\n
  "/dy exch def\n
  "/dx exch def\n
  "/y exch def\n
  "/x exch def\n
  "newpath\n
  "x y moveto\n
  "dx 0 rlineto\n
  "0 dy rlineto\n
  "dx neg 0 rlineto\n
  "closepath\n
  "clip\n
"} bind def\n

"/FR { /dy exch def\n
  "/dx exch def\n
  "/y exch def\n
  "/x exch def\n
  "currentlinewidth 0 setlinewidth newpath\n
  "x y moveto\n
  "dx 0 rlineto\n
  "0 dy rlineto\n
  "dx neg 0 rlineto\n
  "closepath fill setlinewidth\n
"} bind def\n

"/GS { gsave } bind  def\n
"/GR { grestore } bind def\n

"/SP { showpage } bind def\n
"/LW { setlinewidth } bind def\n
"/CF /Courier def\n
"/SF { /CF exch def } bind def\n
"/fsize 12 def\n
"/FS { /fsize exch def fsize CF findfont exch scalefont setfont }def \n


"/GL { setgray } bind def\n
"/SRGB { setrgbcolor } bind def\n

//  color images

"/CI { GS /py exch def /px exch def /sy exch def /sx exch def\n
  "translate \n
  "sx sy scale px py 8 \n
  "[ px 0 0 py neg 0 py ]\n
  "currentfile /ASCIIHexDecode filter\n false 3
  " colorimage GR\n
"} bind def\n

//  gray images

"/GI { GS /py exch def /px exch def /sy exch def /sx exch def \n
  "translate \n
  "sx sy scale px py 8 \n


  "[ px 0 0 py neg 0 py ]\n
  "currentfile /ASCIIHexDecode filter\n
  "image GR\n
"} bind def\n

// single-color bitmask

"/MI { GS /py exch def /px exch def /sy exch def /sx exch def \n
  "translate \n
  "sx sy scale px py false \n
  "[ px 0 0 py neg 0 py ]\n
  "currentfile /ASCIIHexDecode filter\n
  "imagemask GR\n
"} bind def\n


//  path

"/BFP { newpath moveto }  def\n
"/BP { newpath } bind def \n
"/PL { lineto } bind def \n
"/PM { moveto } bind def \n
"/MT { moveto } bind def \n
"/LT { lineto } bind def \n
"/EFP { closepath fill } bind def\n"  //was:strok
"/ELP { stroke } bind def\n" 
"/ECP { closepath stroke } bind def\n"  // Closed (loop
"/LW { setlinewidth } bind def\n

// ////////////////////////// misc ///////////////
"/TR { translate } bind def\n
"/CT { concat } bind def\n
"/RCT { matrix invertmatrix concat} bind def\n
"/SC { scale } bind def\n
//"/GPD { currentpagedevice /PageSize get} def\n




static const char * prolog_2 =  // prolog relevant only if lang_level >

// color image dictionarie
"/CII {GS /inter exch def /py exch def /px exch def /sy exch def /sx exch def \n
  "translate \n
  "sx sy scale\n
  "/DeviceRGB setcolorspace\n
  "/IDD 8 dict def\n
  "IDD begin\n
    "/ImageType 1 def\n
    "/Width px def\n
    "/Height py def\n
    "/BitsPerComponent 8 def\n
    "/Interpolate inter def\n
    "/DataSource currentfile /ASCIIHexDecode filter def\n
    "/MultipleDataSources false def\n
    "/ImageMatrix [ px 0 0 py neg 0 py ] def\n
    "/Decode [ 0 1 0 1 0 1 ] def\n
  "end\n
"IDD image GR} bind def\n

// gray image dict


"/GII {GS /inter exch def /py exch def /px exch def /sy exch def /sx exch def \n
  "translate \n
  "sx sy scale\n
  "/DeviceGray setcolorspace\n
  "/IDD 8 dict def\n
  "IDD begin\n
    "/ImageType 1 def\n
    "/Width px def\n
    "/Height py def\n
    "/BitsPerComponent 8 def\n

    "/Interpolate inter def\n
    "/DataSource currentfile /ASCIIHexDecode filter def\n
    "/MultipleDataSources false def\n
    "/ImageMatrix [ px 0 0 py neg 0 py ] def\n
    "/Decode [ 0 1 ] def\n
  "end\n
"IDD image GR} bind def\n



static const char * prolog_3 = // prolog relevant only if lang_level >

// masked color images
"/CIM {GS /inter exch def /my exch def /mx exch def /py exch def /px exch def /sy exch def /sx exch def \n
  "translate \n
  "sx sy scale\n
  "/DeviceRGB setcolorspace\n
 
"/IDD 8 dict def\n

"IDD begin\n
    "/ImageType 1 def\n
    "/Width px def\n
    "/Height py def\n
    "/BitsPerComponent 8 def\n
  "/Interpolate inter def\n
    "/DataSource currentfile /ASCIIHexDecode filter def\n
    "/MultipleDataSources false def\n
    "/ImageMatrix [ px 0 0 py neg 0 py ] def\n

    "/Decode [ 0 1 0 1 0 1 ] def\n
"end\n

"/IMD 8 dict def\n
"IMD begin\n
    "/ImageType 1 def\n
    "/Width mx def\n"          
    "/Height my def\n
    "/BitsPerComponent 1 def\n
//  "/Interpolate inter def\n
    "/ImageMatrix [ mx 0 0 my neg 0 my ] def\n
    "/Decode [ 1 0 ] def\n
"end\n

"<<\n
  "/ImageType 3\n
  "/InterleaveType 2\n
  "/MaskDict IMD\n
  "/DataDict IDD\n
">> image GR\n
"} bind def\n


//  masked gray images
"/GIM {GS /inter exch def /my exch def /mx exch def /py exch def /px exch def /sy exch def /sx exch def \n
  "translate \n
  "sx sy scale\n
  "/DeviceGray setcolorspace\n

  "/IDD 8 dict def\n

  "IDD begin\n
    "/ImageType 1 def\n
    "/Width px def\n
    "/Height py def\n
    "/BitsPerComponent 8 def\n
    "/Interpolate inter def\n
    "/DataSource currentfile /ASCIIHexDecode filter def\n
    "/MultipleDataSources false def\n
    "/ImageMatrix [ px 0 0 py neg 0 py ] def\n

    "/Decode [ 0 1 ] def\n
  "end\n

  "/IMD 8 dict def\n

  "IMD begin\n
    "/ImageType 1 def\n
    "/Width mx def\n"          
    "/Height my def\n
    "/BitsPerComponent 1 def\n
    "/ImageMatrix [ mx 0 0 my neg 0 my ] def\n
    "/Decode [ 1 0 ] def\n
  "end\n

  "<<\n
    "/ImageType 3\n
    "/InterleaveType 2\n
    "/MaskDict IMD\n
    "/DataDict IDD\n
  ">> image GR\n
"} bind def\n


"\n


// end prolog



void Fl_PS_Printer::recover()
    color(cr_,cg_,cb_)
    line_style(linestyle_,linewidth_,linedash_)
    font(font_,size_)
}
 
void Fl_PS_Printer::reset()
  gap_=1
  clip_=0
  cr_=cg_=cb_=0
  font_=FL_HELVETICA
  size_=12
  linewidth_=0
  linestyle_=FL_SOLID
  strcpy(linedash_,"")
  Clip *c=clip_;   ////just not to have memory leaks for badly writen code (forgotten clip popping

  while(c)
    clip_=clip_->prev
    delete c
    c=clip_
  

}



// /////////////// destructor, finishes postscript, closes FILE  //////////////

Fl_PS_Printer::~Fl_PS_Printer() 
  if(nPages){  // for eps nPages is 0 so it is fine ...
    fprintf(output, "CR\nGR\nGR\nGR\nSP\n restore\n")
    if(!pages_)
      fprintf(output, "%%%%Trailer\n")
      fprintf(output, "%%%%Pages: %i\n" , nPages)
    }
  }els
    fprintf(output, "GR\n restore\n")
  fprintf(output, "%%%%EOF")
  reset()
  //fclose(output)

  while(clip_)
    Clip * c= clip_
    clip_= clip_->prev
    delete c
  
  if(close_cmd_
    (*close_cmd_)(output)




// /////////////// PostScript constructors ////////////////////////////////////


Fl_PS_Printer::Fl_PS_Printer(FILE *o, int lang_level, int pages):clip_(0),interpolate_(0)
  type_ = FL_PS_PRINTER
  close_cmd_=0
  lang_level_=lang_level
  output=o
  mask = 0


  fprintf(output, "%%!PS-Adobe-3.0\n")
  fprintf(output, "%%%%Creator: %s\n",doc_info())
  if(lang_level_>1
    fprintf(output, "%%%%LanguageLevel: %i\n" , lang_level_)
  if((pages_=pages)
    fprintf(output, "%%%%Pages: %i\n", pages)
  els
    fprintf(output, "%%%%Pages: (atend)\n")
  fprintf(output, "%%%%EndComments\n")
  fprintf(output, prolog)
  if(lang_level >1
      fprintf(output, prolog_2)
  if(lang_level >2
      fprintf(output, prolog_3)
  if(lang_level_>=3)
    fprintf(output, "/CS { clipsave } bind def\n")
    fprintf(output, "/CR { cliprestore } bind def\n")
  }else
    fprintf(output, "/CS { GS } bind def\n")
    fprintf(output, "/CR { GR } bind def\n")
  
  page_policy_=1


  fprintf(output, "%%%%EndProlog\n")
  if(lang_level_>=2
    fprintf(output,"<< /Policies << /Pagesize 1 >> >> setpagedevice\n")

  reset()
  nPages=0
  type_ = 0x100

}

void Fl_PS_Printer::page_policy(int p)
  page_policy_ = p
  if(lang_level_>=2
    fprintf(output,"<< /Policies << /Pagesize %i >> >> setpagedevice\n", p)
}



// /////////////////  eps constructor ///////////////////////////////////
 
Fl_PS_Printer::Fl_PS_Printer(FILE *o, int lang_level, int x, int y, int w, int h
  :clip_(0),interpolate_(0

  type_ = FL_PS_PRINTER
  close_cmd_=0
  output=o
  mask = 0
  pages_=0
  //clip_=0
  lang_level_=lang_level

  fprintf(output, "%%!PS-Adobe-3.0 EPSF-3.0\n")
  if(lang_level_>1
    fprintf(output, "%%%%LanguageLevel: %i\n" , lang_level_)
  fprintf(output, "%%%%BoundingBox: %i %i %i %i\n", x , y , x+w , y+h)
  //fprintf(output, "%%%%Orientation: %i\n" , orientation)
  width_ = w
  height_ = h

  fprintf(output, prolog)
  if(lang_level >1
      fprintf(output, prolog_2)
  if(lang_level >2
      fprintf(output, prolog_3)
     
  if(lang_level_>=3)
    fprintf(output, "/CS { clipsave } bind def\n")
    fprintf(output, "/CR { cliprestore } bind def\n")
  }else
    fprintf(output, "/CS { GS } bind def\n")
    fprintf(output, "/CR { GR } bind def\n")
  
  fprintf(output, "%%%%EndProlog\n")
  fprintf(output, "save\n")
  fprintf(output, "GS\n")

  reset()
  fprintf(output, "%i %i TR\n", x , y+h)
  fprintf(output, "1 -1  SC\n")
  fprintf(output, "GS\nCS\n")
  line_style(0)

  nPages=0;  //must be 0 also for eps
  type_= 0x100

}


// //////////////////// paging /////////////////////////////////////////



void Fl_PS_Printer::page(double pw, double ph, int media) 

  if (nPages)
    fprintf(output, "CR\nGR\nGR\nGR\nSP\nrestore\n")
  
  ++nPages
  fprintf(output, "%%%%Page: %i %i\n" , nPages , nPages)
  if (pw>ph)
    fprintf(output, "%%%%PageOrientation: Landscape\n")
  }else
    fprintf(output, "%%%%PageOrientation: Portrait\n")
  

  fprintf(output, "%%%%BeginPageSetup\n")
  if((media & MEDIA) &&(lang_level_>1))
      int r = media & REVERSED
      if(r) r = 2
      fprintf(output, "<< /PageSize [%i %i] /Orientation %i>> setpagedevice\n", (int)(pw+.5), (int)(ph+.5), r)
  
  fprintf(output, "%%%%EndPageSetup\n")
 
  pw_=pw
  ph_=ph
  reset()

  fprintf(output, "save\n")
  fprintf(output, "GS\n")
  fprintf(output, "%g %g TR\n", (double)0 /*lm_*/ , ph_ /* - tm_*/)
  fprintf(output, "1 -1 SC\n")
  line_style(0)
  fprintf(output, "GS\n")
 
  if(!((media & MEDIA) &&(lang_level_>1)))
    if(pw>ph
      if(media & REVERSED
        fprintf(output, "-90 rotate %i 0 translate\n", int(-pw))
      els
        fprintf(output, "90 rotate 0 %i translate\n", int(-ph))
    els
      if(media & REVERSED
        fprintf(output, "180 rotate %i %i translate\n", int(-pw), int(-ph))
  
  fprintf(output, "GS\nCS\n")
 

}


void Fl_PS_Printer::page(int format)


  if(format &  LANDSCAPE)
    ph_=Fl_Printer::page_formats[format & 0xFF][0]
    pw_=Fl_Printer::page_formats[format & 0xFF][1]
  }else
    pw_=Fl_Printer::page_formats[format & 0xFF][0]
    ph_=Fl_Printer::page_formats[format & 0xFF][1]
  
  page(pw_,ph_,format & 0xFF00);//,orientation only
}




void Fl_PS_Printer::place(double x, double y, double tx, double ty, double scale)

 fprintf(output, "CR\nGR\nGS\n")
 reset()
 fprintf(output, "%g %g TR\n", -x*scale + tx , -y*scale + ty)
 fprintf(output, "%g %g SC\n", scale , scale )
 fprintf(output, "CS\n")





/
// End of "$Id$"
/







 







