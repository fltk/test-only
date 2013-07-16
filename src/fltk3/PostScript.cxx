//
// "$Id$"
//
// PostScript device support for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2011 by Bill Spitzak and others.
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
// Please report all bugs and problems to:
//
//     http://www.fltk.org/str.php
//

#include <config.h>
#include <fltk3/run.h>
#include <fltk3/ask.h>
#include <fltk3/draw.h>
#include <stdio.h>
#include <fltk3/PostScript.h>
#include <fltk3/NativeFileChooser.h>
#if defined(USE_X11)
#include "font.h"
#if USE_XFT
#include <X11/Xft/Xft.h>
#endif
#endif

/** \brief Label of the PostScript file chooser window */
const char *fltk3::PostScriptFileDevice::file_chooser_title = "Select a .ps file";

/**
 @brief The constructor.
 */
fltk3::PostScriptGraphicsDriver::PostScriptGraphicsDriver(void)
{
  close_cmd_ = 0;
  //lang_level_ = 3;
  lang_level_ = 2;
  mask = 0;
  ps_filename_ = NULL;
  scale_x = scale_y = 1.;
  bg_r = bg_g = bg_b = 255;
}

/** \brief The destructor. */
fltk3::PostScriptGraphicsDriver::~PostScriptGraphicsDriver() {
  if(ps_filename_) free(ps_filename_);
}

/**
 @brief The constructor.
 */
fltk3::PostScriptFileDevice::PostScriptFileDevice(void)
{
#ifdef __APPLE__
  gc = fl_gc; // the display context is used by fltk3::text_extents()
#endif
  SurfaceDevice::driver( new fltk3::PostScriptGraphicsDriver() );
}

/**
 \brief Returns the PostScript driver of this drawing surface.
 */
fltk3::PostScriptGraphicsDriver *fltk3::PostScriptFileDevice::driver()
{
  return (PostScriptGraphicsDriver*)SurfaceDevice::driver();
}


/**
 @brief Begins the session where all graphics requests will go to a local PostScript file.
 *
 Opens a file dialog entitled with fltk3::PostScriptFileDevice::file_chooser_title to select an output PostScript file.
 @param pagecount The total number of pages to be created. Use 0 if this number is unknown when this function is called.
 @param format Desired page format.
 @param layout Desired page layout.
 @return 0 if OK, 1 if user cancelled the file dialog, 2 if fopen failed on user-selected output file.
 */
int fltk3::PostScriptFileDevice::start_job (int pagecount, enum fltk3::PagedDevice::Page_Format format, 
					  enum fltk3::PagedDevice::Page_Layout layout)
{
  fltk3::NativeFileChooser fnfc;
  fnfc.title(fltk3::PostScriptFileDevice::file_chooser_title);
  fnfc.type(fltk3::NativeFileChooser::BROWSE_SAVE_FILE);
  fnfc.options(fltk3::NativeFileChooser::SAVEAS_CONFIRM);
  fnfc.filter("PostScript\t*.ps\n");
  // Show native chooser
  if ( fnfc.show() ) return 1;
  fltk3::PostScriptGraphicsDriver *ps = driver();
  ps->output = fopen(fnfc.filename(), "w");
  if(ps->output == NULL) return 2;
  ps->ps_filename_ = strdup(fnfc.filename());
  ps->start_postscript(pagecount, format, layout);
  this->set_current();
  return 0;
}

static int dont_close(FILE *f) 
{
  return 0;
}

/**
 @brief Begins the session where all graphics requests will go to FILE pointer.
 *
 @param ps_output A writable FILE pointer that will receive PostScript output and that should not be closed
 until after end_job() has been called.
 @param pagecount The total number of pages to be created. Use 0 if this number is unknown when this function is called.
 @param format Desired page format.
 @param layout Desired page layout.
 @return always 0.
 */
int fltk3::PostScriptFileDevice::start_job (FILE *ps_output, int pagecount, 
    enum fltk3::PagedDevice::Page_Format format, enum fltk3::PagedDevice::Page_Layout layout)
{
  fltk3::PostScriptGraphicsDriver *ps = driver();
  ps->output = ps_output;
  ps->ps_filename_ = NULL;
  ps->start_postscript(pagecount, format, layout);
  ps->close_command(dont_close); // so that end_job() doesn't close the file
  this->set_current();
  return 0;
}

/**
 @brief The destructor.
 */
fltk3::PostScriptFileDevice::~PostScriptFileDevice() {
  fltk3::PostScriptGraphicsDriver *ps = driver();
  if (ps) delete ps;
}

#ifndef FLTK3_DOXYGEN

#if ! (defined(__APPLE__) || defined(WIN32) )
#  include "print_panel.cxx"
#endif

//  Prolog string 

static const char * prolog =
"%%BeginProlog\n"
"/L { /y2 exch def\n"
"/x2 exch def\n"
"/y1 exch def\n"
"/x1 exch def\n"
"newpath   x1 y1 moveto x2 y2 lineto\n"
"stroke}\n"
"bind def\n"


"/R { /dy exch def\n"
"/dx exch def\n"
"/y exch def\n"
"/x exch def\n"
"newpath\n"
"x y moveto\n"
"dx 0 rlineto\n"
"0 dy rlineto\n"
"dx neg 0 rlineto\n"
"closepath stroke\n"
"} bind def\n"

"/CL {\n"
"/dy exch def\n"
"/dx exch def\n"
"/y exch def\n"
"/x exch def\n"
"newpath\n"
"x y moveto\n"
"dx 0 rlineto\n"
"0 dy rlineto\n"
"dx neg 0 rlineto\n"
"closepath\n"
"clip\n"
"} bind def\n"

"/FR { /dy exch def\n"
"/dx exch def\n"
"/y exch def\n"
"/x exch def\n"
"currentlinewidth 0 setlinewidth newpath\n"
"x y moveto\n"
"dx 0 rlineto\n"
"0 dy rlineto\n"
"dx neg 0 rlineto\n"
"closepath fill setlinewidth\n"
"} bind def\n"

"/GS { gsave } bind  def\n"
"/GR { grestore } bind def\n"

"/SP { showpage } bind def\n"
"/LW { setlinewidth } bind def\n"
"/CF /Courier def\n"
"/SF { /CF exch def } bind def\n"
"/fsize 12 def\n"
"/FS { /fsize exch def fsize CF findfont exch scalefont setfont }def \n"


"/GL { setgray } bind def\n"
"/SRGB { setrgbcolor } bind def\n"

//  color images 

"/CI { GS /py exch def /px exch def /sy exch def /sx exch def\n"
"translate \n"
"sx sy scale px py 8 \n"
"[ px 0 0 py neg 0 py ]\n"
"currentfile /ASCIIHexDecode filter\n false 3"
" colorimage GR\n"
"} bind def\n"

//  gray images 

"/GI { GS /py exch def /px exch def /sy exch def /sx exch def \n"
"translate \n"
"sx sy scale px py 8 \n"


"[ px 0 0 py neg 0 py ]\n"
"currentfile /ASCIIHexDecode filter\n"
"image GR\n"
"} bind def\n"

// single-color bitmask 

"/MI { GS /py exch def /px exch def /sy exch def /sx exch def \n"
"translate \n"
"sx sy scale px py true \n"
"[ px 0 0 py neg 0 py ]\n"
"currentfile /ASCIIHexDecode filter\n"
"imagemask GR\n"
"} bind def\n"


//  path 

"/BFP { newpath moveto }  def\n"
"/BP { newpath } bind def \n"
"/PL { lineto } bind def \n"
"/PM { moveto } bind def \n"
"/MT { moveto } bind def \n"
"/LT { lineto } bind def \n"
"/EFP { closepath fill } bind def\n"  //was:stroke
"/ELP { stroke } bind def\n"  
"/ECP { closepath stroke } bind def\n"  // Closed (loop)
"/LW { setlinewidth } bind def\n"

// ////////////////////////// misc ////////////////
"/TR { translate } bind def\n"
"/CT { concat } bind def\n"
"/RCT { matrix invertmatrix concat} bind def\n"
"/SC { scale } bind def\n"
//"/GPD { currentpagedevice /PageSize get} def\n"

// show at position with desired width
// usage:
// width (string) x y show_pos_width
"/show_pos_width {GS moveto dup dup stringwidth pop exch length 2 div dup 2 le {pop 9999} if "
"1 sub exch 3 index exch sub exch "
"div 0 2 index 1 -1 scale ashow pop pop GR} bind def\n" // spacing altered to match desired width
//"/show_pos_width {GS moveto dup stringwidth pop 3 2 roll exch div -1 matrix scale concat "
//"show GR } bind def\n" // horizontally scaled text to match desired width

;


static const char * prolog_2 =  // prolog relevant only if lang_level >1

// color image dictionaries
"/CII {GS /inter exch def /py exch def /px exch def /sy exch def /sx exch def \n"
"translate \n"
"sx sy scale\n"
"/DeviceRGB setcolorspace\n"
"/IDD 8 dict def\n"
"IDD begin\n"
"/ImageType 1 def\n"
"/Width px def\n"
"/Height py def\n"
"/BitsPerComponent 8 def\n"
"/Interpolate inter def\n"
"/DataSource currentfile /ASCIIHexDecode filter def\n"
"/MultipleDataSources false def\n"
"/ImageMatrix [ px 0 0 py neg 0 py ] def\n"
"/Decode [ 0 1 0 1 0 1 ] def\n"
"end\n"
"IDD image GR} bind def\n"

// gray image dict 
"/GII {GS /inter exch def /py exch def /px exch def /sy exch def /sx exch def \n"
"translate \n"
"sx sy scale\n"
"/DeviceGray setcolorspace\n"
"/IDD 8 dict def\n"
"IDD begin\n"
"/ImageType 1 def\n"
"/Width px def\n"
"/Height py def\n"
"/BitsPerComponent 8 def\n"

"/Interpolate inter def\n"
"/DataSource currentfile /ASCIIHexDecode filter def\n"
"/MultipleDataSources false def\n"
"/ImageMatrix [ px 0 0 py neg 0 py ] def\n"
"/Decode [ 0 1 ] def\n"
"end\n"
"IDD image GR} bind def\n"

// Create a custom PostScript font derived from PostScript standard text fonts
// The encoding of this custom font is as follows:
// 0000-00FF  coincides with Unicode, that is to ASCII + Latin-1
// 0100-017F  coincides with Unicode, that is to Latin Extended-A
// 0180-01A6  encodes miscellaneous characters present in PostScript standard text fonts

// use ISOLatin1Encoding for all text fonts
"/ToISO { dup findfont dup length dict copy begin /Encoding ISOLatin1Encoding def currentdict end definefont pop } def\n"
"/Helvetica ToISO /Helvetica-Bold ToISO /Helvetica-Oblique ToISO /Helvetica-BoldOblique ToISO \n"
"/Courier ToISO /Courier-Bold ToISO /Courier-Oblique ToISO /Courier-BoldOblique ToISO \n"
"/Times-Roman ToISO /Times-Bold ToISO /Times-Italic ToISO /Times-BoldItalic ToISO \n"

// define LatinExtA, the encoding of Latin-extended-A + some additional characters
// see http://partners.adobe.com/public/developer/en/opentype/glyphlist.txt for their names
"/LatinExtA \n"
"[ "
" /Amacron /amacron /Abreve /abreve /Aogonek /aogonek\n" // begin of Latin Extended-A code page
" /Cacute  /cacute  /Ccircumflex  /ccircumflex  /Cdotaccent  /cdotaccent  /Ccaron  /ccaron \n"
" /Dcaron  /dcaron   /Dcroat  /dcroat\n"
" /Emacron  /emacron  /Ebreve  /ebreve  /Edotaccent  /edotaccent  /Eogonek  /eogonek  /Ecaron  /ecaron\n"
" /Gcircumflex  /gcircumflex  /Gbreve  /gbreve  /Gdotaccent  /gdotaccent  /Gcommaaccent  /gcommaaccent \n"
" /Hcircumflex /hcircumflex  /Hbar  /hbar  \n"
" /Itilde  /itilde  /Imacron  /imacron  /Ibreve  /ibreve  /Iogonek  /iogonek /Idotaccent  /dotlessi  \n"
" /IJ  /ij  /Jcircumflex  /jcircumflex\n"
" /Kcommaaccent  /kcommaaccent  /kgreenlandic  \n"
" /Lacute  /lacute  /Lcommaaccent  /lcommaaccent   /Lcaron  /lcaron  /Ldotaccent /ldotaccent   /Lslash  /lslash \n"
" /Nacute  /nacute  /Ncommaaccent  /ncommaaccent  /Ncaron  /ncaron  /napostrophe  /Eng  /eng  \n"
" /Omacron  /omacron /Obreve  /obreve  /Ohungarumlaut  /ohungarumlaut  /OE  /oe \n"
" /Racute  /racute  /Rcommaaccent  /rcommaaccent  /Rcaron  /rcaron \n"
" /Sacute /sacute  /Scircumflex  /scircumflex  /Scedilla /scedilla /Scaron  /scaron \n"
" /Tcommaaccent  /tcommaaccent  /Tcaron  /tcaron  /Tbar  /tbar \n"
" /Utilde  /utilde /Umacron /umacron  /Ubreve  /ubreve  /Uring  /uring  /Uhungarumlaut  /uhungarumlaut  /Uogonek /uogonek \n"
" /Wcircumflex  /wcircumflex  /Ycircumflex  /ycircumflex  /Ydieresis \n"
" /Zacute /zacute /Zdotaccent /zdotaccent /Zcaron /zcaron \n"
" /longs \n" // end of Latin Extended-A code page
" /florin  /circumflex  /caron  /breve  /dotaccent  /ring \n" // remaining characters from PostScript standard text fonts
" /ogonek  /tilde  /hungarumlaut  /endash /emdash \n"
" /quoteleft  /quoteright  /quotesinglbase  /quotedblleft  /quotedblright \n"
" /quotedblbase  /dagger  /daggerdbl  /bullet  /ellipsis \n"
" /perthousand  /guilsinglleft  /guilsinglright  /fraction  /Euro \n"
" /trademark /partialdiff  /Delta /summation  /radical \n"
" /infinity /notequal /lessequal /greaterequal /lozenge \n"
" /fi /fl /apple \n"
" ] def \n"
// deal with alternative PostScript names of some characters
" /mycharstrings /Helvetica findfont /CharStrings get def\n"
" /PSname2 { dup mycharstrings exch known {LatinExtA 3 -1 roll 3 -1 roll put}{pop pop} ifelse } def \n"
" 16#20 /Gdot PSname2 16#21 /gdot PSname2 16#30 /Idot PSname2 16#3F /Ldot PSname2 16#40 /ldot PSname2 16#7F /slong PSname2 \n"

// proc that gives LatinExtA encoding to a font
"/ToLatinExtA { findfont dup length dict copy begin /Encoding LatinExtA def currentdict end definefont pop } def\n"
// create Ext-versions of standard fonts that use LatinExtA encoding \n"
"/HelveticaExt /Helvetica ToLatinExtA \n"
"/Helvetica-BoldExt /Helvetica-Bold ToLatinExtA /Helvetica-ObliqueExt /Helvetica-Oblique ToLatinExtA  \n"
"/Helvetica-BoldObliqueExt /Helvetica-BoldOblique ToLatinExtA  \n"
"/CourierExt /Courier ToLatinExtA /Courier-BoldExt /Courier-Bold ToLatinExtA  \n"
"/Courier-ObliqueExt /Courier-Oblique ToLatinExtA /Courier-BoldObliqueExt /Courier-BoldOblique ToLatinExtA \n"
"/Times-RomanExt /Times-Roman ToLatinExtA /Times-BoldExt /Times-Bold ToLatinExtA  \n"
"/Times-ItalicExt /Times-Italic ToLatinExtA /Times-BoldItalicExt /Times-BoldItalic ToLatinExtA \n"

// proc to create a Type 0 font with 2-byte encoding 
// that merges a text font with ISO encoding + same font with LatinExtA encoding
"/To2byte { 6 dict begin /FontType 0 def \n"
"/FDepVector 3 1 roll findfont exch findfont 2 array astore def \n"
"/FontMatrix [1  0  0  1  0  0] def /FMapType 6 def /Encoding [ 0 1 0 ] def\n"
// 100: Hexa count of ISO array; A7: hexa count of LatinExtA array
"/SubsVector < 01 0100 00A7 > def\n" 
"currentdict end definefont pop } def\n"
// create Type 0 versions of standard fonts
"/Helvetica2B /HelveticaExt /Helvetica To2byte \n"
"/Helvetica-Bold2B /Helvetica-BoldExt /Helvetica-Bold To2byte \n"
"/Helvetica-Oblique2B /Helvetica-ObliqueExt /Helvetica-Oblique To2byte \n"
"/Helvetica-BoldOblique2B /Helvetica-BoldObliqueExt /Helvetica-BoldOblique To2byte \n"
"/Courier2B /CourierExt /Courier To2byte \n"
"/Courier-Bold2B /Courier-BoldExt /Courier-Bold To2byte \n"
"/Courier-Oblique2B /Courier-ObliqueExt /Courier-Oblique To2byte \n"
"/Courier-BoldOblique2B /Courier-BoldObliqueExt /Courier-BoldOblique To2byte \n"
"/Times-Roman2B /Times-RomanExt /Times-Roman To2byte \n"
"/Times-Bold2B /Times-BoldExt /Times-Bold To2byte \n"
"/Times-Italic2B /Times-ItalicExt /Times-Italic To2byte \n"
"/Times-BoldItalic2B /Times-BoldItalicExt /Times-BoldItalic To2byte \n"
;

static const char * prolog_2_pixmap =  // prolog relevant only if lang_level == 2 for pixmaps/masked color images
"/pixmap_mat {[ pixmap_sx 0 0 pixmap_sy neg 0 pixmap_sy ]}  bind def\n"

"/pixmap_dict {"
"<< /PatternType 1 "
"/PaintType 1 "
"/TilingType 2 "
"/BBox [0  0  pixmap_sx  pixmap_sy] "
"/XStep pixmap_sx "
"/YStep pixmap_sy\n"
"/PaintProc "
"{ begin "
"pixmap_w pixmap_h scale "
"pixmap_sx pixmap_sy 8 "
"pixmap_mat "
"currentfile /ASCIIHexDecode filter "
"false 3 "
"colorimage "
"end "
"} bind "
">>\n"
"} bind def\n"

"/pixmap_plot {"
"GS "
"/pixmap_sy exch def /pixmap_sx exch def\n"
"/pixmap_h exch def /pixmap_w exch def\n"
"translate\n"
"pixmap_dict matrix makepattern setpattern\n"
"pixmap_w pixmap_h scale\n"
"pixmap_sx pixmap_sy\n"
"true\n"
"pixmap_mat\n"
"currentfile /ASCIIHexDecode filter\n"
"imagemask\n"
"GR\n"
"} bind def\n"
;

static const char * prolog_3 = // prolog relevant only if lang_level >2

// masked color images 
"/CIM {GS /inter exch def /my exch def /mx exch def /py exch def /px exch def /sy exch def /sx exch def \n"
"translate \n"
"sx sy scale\n"
"/DeviceRGB setcolorspace\n"

"/IDD 8 dict def\n"

"IDD begin\n"
"/ImageType 1 def\n"
"/Width px def\n"
"/Height py def\n"
"/BitsPerComponent 8 def\n"
"/Interpolate inter def\n"
"/DataSource currentfile /ASCIIHexDecode filter def\n"
"/MultipleDataSources false def\n"
"/ImageMatrix [ px 0 0 py neg 0 py ] def\n"

"/Decode [ 0 1 0 1 0 1 ] def\n"
"end\n"

"/IMD 8 dict def\n"
"IMD begin\n"
"/ImageType 1 def\n"
"/Width mx def\n"           
"/Height my def\n"
"/BitsPerComponent 1 def\n"
//  "/Interpolate inter def\n"
"/ImageMatrix [ mx 0 0 my neg 0 my ] def\n"
"/Decode [ 1 0 ] def\n"
"end\n"

"<<\n"
"/ImageType 3\n"
"/InterleaveType 2\n"
"/MaskDict IMD\n"
"/DataDict IDD\n"
">> image GR\n"
"} bind def\n"


//  masked gray images 
"/GIM {GS /inter exch def /my exch def /mx exch def /py exch def /px exch def /sy exch def /sx exch def \n"
"translate \n"
"sx sy scale\n"
"/DeviceGray setcolorspace\n"

"/IDD 8 dict def\n"

"IDD begin\n"
"/ImageType 1 def\n"
"/Width px def\n"
"/Height py def\n"
"/BitsPerComponent 8 def\n"
"/Interpolate inter def\n"
"/DataSource currentfile /ASCIIHexDecode filter def\n"
"/MultipleDataSources false def\n"
"/ImageMatrix [ px 0 0 py neg 0 py ] def\n"

"/Decode [ 0 1 ] def\n"
"end\n"

"/IMD 8 dict def\n"

"IMD begin\n"
"/ImageType 1 def\n"
"/Width mx def\n"           
"/Height my def\n"
"/BitsPerComponent 1 def\n"
"/ImageMatrix [ mx 0 0 my neg 0 my ] def\n"
"/Decode [ 1 0 ] def\n"
"end\n"

"<<\n"
"/ImageType 3\n"
"/InterleaveType 2\n"
"/MaskDict IMD\n"
"/DataDict IDD\n"
">> image GR\n"
"} bind def\n"


"\n"
;

// end prolog 

int fltk3::PostScriptGraphicsDriver::start_postscript (int pagecount, 
    enum fltk3::PagedDevice::Page_Format format, enum fltk3::PagedDevice::Page_Layout layout)
//returns 0 iff OK
{
  int w, h, x;
  if (format == fltk3::PagedDevice::A4) {
    left_margin = 18;
    top_margin = 18;
  }
  else {
    left_margin = 12;
    top_margin = 12;
  }
  page_format_ = (enum fltk3::PagedDevice::Page_Format)(format | layout);
  if (layout & fltk3::PagedDevice::LANDSCAPE){
    ph_ = fltk3::PagedDevice::page_formats[format].width;
    pw_ = fltk3::PagedDevice::page_formats[format].height;
  } else {
    pw_ = fltk3::PagedDevice::page_formats[format].width;
    ph_ = fltk3::PagedDevice::page_formats[format].height;
  }
  
  fputs("%!PS-Adobe-3.0\n", output);
  fputs("%%Creator: FLTK\n", output);
  if (lang_level_>1)
    fprintf(output, "%%%%LanguageLevel: %i\n" , lang_level_);
  if ((pages_ = pagecount))
    fprintf(output, "%%%%Pages: %i\n", pagecount);
  else
    fputs("%%Pages: (atend)\n", output);
  fprintf(output, "%%%%BeginFeature: *PageSize %s\n", fltk3::PagedDevice::page_formats[format].name );
  w = fltk3::PagedDevice::page_formats[format].width;
  h = fltk3::PagedDevice::page_formats[format].height;
  if (lang_level_ == 3 && (layout & fltk3::PagedDevice::LANDSCAPE) ) { x = w; w = h; h = x; }
  fprintf(output, "<</PageSize[%d %d]>>setpagedevice\n", w, h );
  fputs("%%EndFeature\n", output);
  fputs("%%EndComments\n", output);
  fputs(prolog, output);
  if (lang_level_ > 1) {
    fputs(prolog_2, output);
    }
  if (lang_level_ == 2) {
    fputs(prolog_2_pixmap, output);
    }
  if (lang_level_ > 2)
    fputs(prolog_3, output);
  if (lang_level_ >= 3) {
    fputs("/CS { clipsave } bind def\n", output);
    fputs("/CR { cliprestore } bind def\n", output);
  } else {
    fputs("/CS { GS } bind def\n", output);
    fputs("/CR { GR } bind def\n", output);
  }
  page_policy_ = 1;
  
  
  fputs("%%EndProlog\n",output);
  if (lang_level_ >= 2)
    fprintf(output,"<< /Policies << /Pagesize 1 >> >> setpagedevice\n");
  
  reset();
  nPages=0;
  return 0;
}

void fltk3::PostScriptGraphicsDriver::recover(){
  color(cr_,cg_,cb_);
  line_style(linestyle_,linewidth_,linedash_);
  font(GraphicsDriver::font(), GraphicsDriver::size());
}

void fltk3::PostScriptGraphicsDriver::reset(){
  gap_=1;
  clip_=0;
  cr_=cg_=cb_=0;
  GraphicsDriver::font(fltk3::HELVETICA, 12);
  linewidth_=0;
  linestyle_=fltk3::SOLID;
  strcpy(linedash_,"");
  Clip *c=clip_;   ////just not to have memory leaks for badly writen code (forgotten clip popping)
  
  while(c){
    clip_=clip_->prev;
    delete c;
    c=clip_;
  }
  
}

void fltk3::PostScriptGraphicsDriver::page_policy(int p){
  page_policy_ = p;
  if(lang_level_>=2)
    fprintf(output,"<< /Policies << /Pagesize %i >> >> setpagedevice\n", p);
}

// //////////////////// paging //////////////////////////////////////////



void fltk3::PostScriptGraphicsDriver::page(double pw, double ph, int media) {
  
  if (nPages){
    fprintf(output, "CR\nGR\nGR\nGR\nSP\nrestore\n");
  }
  ++nPages;
  fprintf(output, "%%%%Page: %i %i\n" , nPages , nPages);
  fprintf(output, "%%%%PageBoundingBox: 0 0 %d %d\n", pw > ph ? (int)ph : (int)pw , pw > ph ? (int)pw : (int)ph);
  if (pw>ph){
    fprintf(output, "%%%%PageOrientation: Landscape\n");
  }else{
    fprintf(output, "%%%%PageOrientation: Portrait\n");
  }
  
  fprintf(output, "%%%%BeginPageSetup\n");
  if((media & fltk3::PagedDevice::MEDIA) &&(lang_level_>1)){
    int r = media & fltk3::PagedDevice::REVERSED;
    if(r) r = 2;
    fprintf(output, "<< /PageSize [%i %i] /Orientation %i>> setpagedevice\n", (int)(pw+.5), (int)(ph+.5), r);
  }
  fprintf(output, "%%%%EndPageSetup\n");
  
  //pw_ = pw;
  //ph_ = ph;
  reset();
  
  fprintf(output, "save\n");
  fprintf(output, "GS\n");
  fprintf(output, "%g %g TR\n", (double)0 /*lm_*/ , ph_ /* - tm_*/);
  fprintf(output, "1 -1 SC\n");
  line_style(0);
  fprintf(output, "GS\n");
  
  if (!((media & fltk3::PagedDevice::MEDIA) &&(lang_level_>1))){
    if (pw > ph) {
      if(media & fltk3::PagedDevice::REVERSED) {
        fprintf(output, "-90 rotate %i 0 translate\n", int(-pw));
	}
      else {
        fprintf(output, "90 rotate -%i -%i translate\n", (lang_level_ == 2 ? int(pw - ph) : 0), int(ph));
	}
      }
      else {
	if(media & fltk3::PagedDevice::REVERSED)
	  fprintf(output, "180 rotate %i %i translate\n", int(-pw), int(-ph));
	}
  }
  fprintf(output, "GS\nCS\n");
}

void fltk3::PostScriptGraphicsDriver::page(int format){
/*  if(format &  fltk3::PagedDevice::LANDSCAPE){
    ph_=fltk3::PagedDevice::page_formats[format & 0xFF].width;
    pw_=fltk3::PagedDevice::page_formats[format & 0xFF].height;
  }else{
    pw_=fltk3::PagedDevice::page_formats[format & 0xFF].width;
    ph_=fltk3::PagedDevice::page_formats[format & 0xFF].height;
  }*/
  page(pw_,ph_,format & 0xFF00);//,orientation only;
}

void fltk3::PostScriptGraphicsDriver::rect(int x, int y, int w, int h) {
  // Commented code does not work, i can't find the bug ;-(
  // fprintf(output, "GS\n");
  //  fprintf(output, "%i, %i, %i, %i R\n", x , y , w, h);
  //  fprintf(output, "GR\n");
  x += origin_x(); y += origin_y();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output, "%i %i MT\n", x , y);
  fprintf(output, "%i %i LT\n", x+w-1 , y);
  fprintf(output, "%i %i LT\n", x+w-1 , y+h-1);
  fprintf(output, "%i %i LT\n", x , y+h-1);
  fprintf(output, "ECP\n");
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::rectf(int x, int y, int w, int h) {
  x += origin_x(); y += origin_y();
  fprintf(output, "%g %g %i %i FR\n", x-0.5, y-0.5, w, h);
}

void fltk3::PostScriptGraphicsDriver::line(int x1, int y1, int x2, int y2) {
  x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  fprintf(output, "GS\n");
  fprintf(output, "%i %i %i %i L\n", x1 , y1, x2 ,y2);
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::line(int x0, int y0, int x1, int y1, int x2, int y2) {
  x0 += origin_x(); y0 += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output, "%i %i MT\n", x0 , y0);
  fprintf(output, "%i %i LT\n", x1 , y1);
  fprintf(output, "%i %i LT\n", x2 , y2);
  fprintf(output, "ELP\n");
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::xyline(int x, int y, int x1, int y2, int x3){
  x += origin_x(); y += origin_y(); x1 += origin_x(); y2 += origin_y(); x3 += origin_x();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output, "%i %i MT\n", x , y );
  fprintf(output, "%i %i LT\n", x1 , y );
  fprintf(output, "%i %i LT\n", x1 , y2);
  fprintf(output,"%i %i LT\n", x3 , y2);
  fprintf(output, "ELP\n");
  fprintf(output, "GR\n");
}


void fltk3::PostScriptGraphicsDriver::xyline(int x, int y, int x1, int y2){
  x += origin_x(); y += origin_y(); x1 += origin_x(); y2 += origin_y();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output, "%i %i MT\n", x , y);
  fprintf(output,"%i %i LT\n", x1 , y);
  fprintf(output, "%i %i LT\n", x1 , y2 );
  fprintf(output, "ELP\n");
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::xyline(int x, int y, int x1){
  x += origin_x(); y += origin_y(); x1 += origin_x();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output, "%i %i MT\n", x , y);
  fprintf(output, "%i %i LT\n", x1 , y );
  fprintf(output, "ELP\n");
  
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::yxline(int x, int y, int y1, int x2, int y3){
  x += origin_x(); y += origin_y(); y1 += origin_y(); x2 += origin_x(); y3 += origin_y();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output,"%i %i MT\n", x , y);
  fprintf(output, "%i %i LT\n", x , y1 );
  fprintf(output, "%i %i LT\n", x2 , y1 );
  fprintf(output , "%i %i LT\n", x2 , y3);
  fprintf(output, "ELP\n");
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::yxline(int x, int y, int y1, int x2){
  x += origin_x(); y += origin_y(); y1 += origin_y(); x2 += origin_x();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output, "%i %i MT\n", x , y);
  fprintf(output, "%i %i LT\n", x , y1);
  fprintf(output, "%i %i LT\n", x2 , y1);
  fprintf(output, "ELP\n");
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::yxline(int x, int y, int y1){
  x += origin_x(); y += origin_y(); y1 += origin_y();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output, "%i %i MT\n", x , y);
  fprintf(output, "%i %i LT\n", x , y1);
  fprintf(output, "ELP\n");
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::loop(int x0, int y0, int x1, int y1, int x2, int y2) {
  x0 += origin_x(); y0 += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output, "%i %i MT\n", x0 , y0);
  fprintf(output, "%i %i LT\n", x1 , y1);
  fprintf(output, "%i %i LT\n", x2 , y2);
  fprintf(output, "ECP\n");
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
  x0 += origin_x(); y0 += origin_y(); x1 += origin_x(); y1 += origin_y(); 
  x2 += origin_x(); y2 += origin_y(); x3 += origin_x(); y3 += origin_y();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output, "%i %i MT\n", x0 , y0);
  fprintf(output, "%i %i LT\n", x1 , y1);
  fprintf(output, "%i %i LT\n", x2 , y2);
  fprintf(output, "%i %i LT\n", x3 , y3);
  fprintf(output, "ECP\n");
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::polygon(int x0, int y0, int x1, int y1, int x2, int y2) {
  x0 += origin_x(); y0 += origin_y(); x1 += origin_x(); y1 += origin_y(); x2 += origin_x(); y2 += origin_y();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output, "%i %i MT\n", x0 , y0);
  fprintf(output,"%i %i LT\n", x1 , y1);
  fprintf(output, "%i %i LT\n", x2 , y2);
  fprintf(output, "EFP\n");
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
  x0 += origin_x(); y0 += origin_y(); x1 += origin_x(); y1 += origin_y(); 
  x2 += origin_x(); y2 += origin_y(); x3 += origin_x(); y3 += origin_y();
  fprintf(output, "GS\n");
  fprintf(output,"BP\n");
  fprintf(output, "%i %i MT\n", x0 , y0 );
  fprintf(output, "%i %i LT\n", x1 , y1 );
  fprintf(output, "%i %i LT\n", x2 , y2 );
  fprintf(output, "%i %i LT\n", x3 , y3 );
  fprintf(output, "EFP\n");
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::point(int x, int y){
  rectf(x + origin_x(), y + origin_y(), 1, 1);
}

static int dashes_flat[5][7]={
{-1,0,0,0,0,0,0},
{3,1,-1,0,0,0,0},
{1,1,-1,0,0,0,0},
{3,1,1,1,-1,0,0},
{3,1,1,1,1,1,-1}
};


//yeah, hack...
static double dashes_cap[5][7]={
{-1,0,0,0,0,0,0},
{2,2,-1,0,0,0,0},
{0.01,1.99,-1,0,0,0,0},
{2,2,0.01,1.99,-1,0,0},
{2,2,0.01,1.99,0.01,1.99,-1}
};


void fltk3::PostScriptGraphicsDriver::line_style(int style, int width, char* dashes){
  //line_styled_=1;
  
  linewidth_=width;
  linestyle_=style;
  //dashes_= dashes;
  if(dashes){
    if(dashes != linedash_)
      strcpy(linedash_,dashes);
    
  }else
    linedash_[0]=0;
  char width0 = 0;
  if(!width){
    width=1; //for screen drawing compatibility
    width0=1;
  }
  
  fprintf(output, "%i setlinewidth\n", width);
  
  if(!style && (!dashes || !(*dashes)) && width0) //system lines
    style = fltk3::CAP_SQUARE;
  
  int cap = (style &0xf00) >> 8;
  if(cap) cap--;
  fprintf(output,"%i setlinecap\n", cap);
  
  int join = (style & 0xf000) >> 12;
  
  if(join) join--;
  fprintf(output,"%i setlinejoin\n", join);
  
  
  fprintf(output, "[");
  if(dashes && *dashes){
    while(*dashes){
      fprintf(output, "%i ", *dashes);
      dashes++;
    }
  }else{
    int * ds; 
    if(style & 0x200){ // round and square caps, dash length need to be adjusted
      double *dt = dashes_cap[style & 0xff];
      while (*dt >= 0){
	fprintf(output, "%g ",width * (*dt));
	dt++;
      }
    }else{
      
      ds = dashes_flat[style & 0xff];
      while (*ds >= 0){
	fprintf(output, "%i ",width * (*ds));
        ds++;
      }
    }
  }
  fprintf(output, "] 0 setdash\n");
}

static const char *_fontNames[] = {
"Helvetica2B", 
"Helvetica-Bold2B",
"Helvetica-Oblique2B",
"Helvetica-BoldOblique2B",
"Courier2B",
"Courier-Bold2B",
"Courier-Oblique2B",
"Courier-BoldOblique2B",
"Times-Roman2B",
"Times-Bold2B",
"Times-Italic2B",
"Times-BoldItalic2B",
"Symbol",
"Courier2B",
"Courier-Bold2B",
"ZapfDingbats"
};

void fltk3::PostScriptGraphicsDriver::font(int f, int s) {
  fltk3::GraphicsDriver *driver = fltk3::DisplayDevice::display_device()->driver();
  driver->font(f,s); // Use display fonts for font measurement
  GraphicsDriver::font(f, s);
  Fl_Font_Descriptor *desc = driver->font_descriptor();
  this->font_descriptor(desc);
  if (f < fltk3::FREE_FONT) {
    float ps_size = (float)s;
    fprintf(output, "/%s SF\n" , _fontNames[f]);
#if defined(USE_X11) 
#if USE_XFT
    // Xft font height is sometimes larger than the required size (see STR 2566).
    // Increase the PostScript font size by 15% without exceeding the display font height 
    int max = desc->font->height;
    ps_size = s * 1.15;
    if (ps_size > max) ps_size = max;
#else
    // Non-Xft fonts can be smaller than required.
    // Set the PostScript font size to the display font height 
    char *name = desc->font->font_name_list[0];
    char *p = strstr(name, "--");
    if (p) {
      sscanf(p + 2, "%f", &ps_size);
    }
#endif // USE_XFT
#endif // USE_X11
    fprintf(output,"%.1f FS\n", ps_size);
  }
}

double fltk3::PostScriptGraphicsDriver::width(const char *s, int n) {
  return fltk3::DisplayDevice::display_device()->driver()->width(s, n);
}

int fltk3::PostScriptGraphicsDriver::height() {
  return fltk3::DisplayDevice::display_device()->driver()->height();
}

int fltk3::PostScriptGraphicsDriver::descent() {
  return fltk3::DisplayDevice::display_device()->driver()->descent();
}

void fltk3::PostScriptGraphicsDriver::text_extents(const char *c, int n, int &dx, int &dy, int &w, int &h) {
  fltk3::DisplayDevice::display_device()->driver()->text_extents(c, n, dx, dy, w, h);
}


void fltk3::PostScriptGraphicsDriver::color(fltk3::Color c) {
  fltk3::get_color(c, cr_, cg_, cb_);
  color(cr_, cg_, cb_);
}

void fltk3::PostScriptGraphicsDriver::color(unsigned char r, unsigned char g, unsigned char b) {
  GraphicsDriver::color( fltk3::rgb_color(r, g, b) );
  cr_ = r; cg_ = g; cb_ = b;
  if (r == g && g == b) {
    double gray = r/255.0;
    fprintf(output, "%g GL\n", gray);
  } else {
    double fr, fg, fb;
    fr = r/255.0;
    fg = g/255.0;
    fb = b/255.0;
    fprintf(output, "%g %g %g SRGB\n", fr , fg , fb);
  }
}

void fltk3::PostScriptGraphicsDriver::draw(int angle, const char *str, int n, int x, int y)
{
  fprintf(output, "GS %d %d translate %d rotate\n", x, y, - angle);
  this->transformed_draw(str, n, 0, 0);
  fprintf(output, "GR\n");
}


// computes the mask for the RGB image img of all pixels with color != bg
static uchar *calc_mask(uchar *img, int w, int h, fltk3::Color bg)
{
  uchar red, green, blue, r, g, b;
  uchar bit, byte, *q;
  fltk3::get_color(bg, red, green, blue);
  int W = (w+7)/8; // width of mask
  uchar* mask = new uchar[W * h];
  q = mask;
  while (h-- > 0) { // for each row
    bit = 0x80; // byte with last bit set
    byte = 0; // next mask byte to compute
    for (int j = 0; j < w; j++) { // for each column
      r = *img++; // the pixel color components
      g = *img++;
      b = *img++;
      // if pixel doesn't have bg color, put it in mask
      if (r != red || g != green || b != blue) byte |= bit;
      bit = bit>>1; // shift bit one step to the right
      if (bit == 0) { // single set bit has fallen out
	*q++ = byte; // enter byte in mask
	byte = 0; // reset next mask byte to zero
	bit = 0x80; // and this byte
	}
      }
    if (bit != 0x80) *q++ = byte; // enter last columns' byte in mask
    }
  return mask;
}

// write to PostScript a bitmap image of a UTF8 string
static void transformed_draw_extra(const char* str, int n, double x, double y, int w, 
      FILE *output, fltk3::GraphicsDriver *driver, bool rtl) {
  // scale for bitmask computation
#if defined(USE_X11) && !USE_XFT
  float scale = 1; // don't scale because we can't expect to have scalable fonts
#else
  float scale = 2;
#endif
  fltk3::Fontsize old_size = driver->size();
  fltk3::Font fontnum = driver->font();
  int w_scaled =  (int)(w * (scale + 0.5));
  int h = (int)(driver->height() * scale);
  // create an offscreen image of the string
  fltk3::Color text_color = driver->color();
  fltk3::Color bg_color = fltk3::contrast(fltk3::WHITE, text_color);
  fltk3::Offscreen off = fl_create_offscreen(w_scaled, (int)(h+3*scale) );
  fl_begin_offscreen(off);
  fltk3::color(bg_color);
  // color offscreen background with a shade contrasting with the text color
  fltk3::rectf(0, 0, w_scaled, (int)(h+3*scale) );
  fltk3::color(text_color);
#if defined(USE_X11) && !USE_XFT
  // force seeing this font as new so it's applied to the offscreen graphics context
  fltk3::graphics_driver->font_descriptor(NULL);
  fltk3::font(fontnum, 0);
#endif
  fltk3::font(fontnum, (fltk3::Fontsize)(scale * old_size) );
  int w2 = (int)fltk3::width(str, n);
  // draw string in offscreen
  if (rtl) fltk3::rtl_draw(str, n, w2, (int)(h * 0.8) );
  else fltk3::draw(str, n, 1, (int)(h * 0.8) );
  // read (most of) the offscreen image
  uchar *img = fltk3::read_image(NULL, 1, 1, w2, h, 0);
  fl_end_offscreen();
  driver->font(fontnum, old_size);
  fl_delete_offscreen(off);
  // compute the mask of what is not the background
  uchar *mask = calc_mask(img, w2, h, bg_color);
  delete[] img;
  // write the string image to PostScript as a scaled bitmask
  scale = w2 / float(w);
  fprintf(output, "%g %g %g %g %d %d MI\n", x, y - h*0.77/scale, w2/scale, h/scale, w2, h);
  uchar *di;
  int wmask = (w2+7)/8;
  for (int j = h - 1; j >= 0; j--){
    di = mask + j * wmask;
    for (int i = 0; i < wmask; i++){
      //if (!(i%80)) fprintf(output, "\n"); // don't have lines longer than 255 chars
      fprintf(output, "%2.2x", *di );
      di++;
    }
    fprintf(output,"\n");
  }
  fprintf(output,">\n");
  delete[] mask;
}

static int is_in_table(unsigned utf) {
  unsigned i;
  static unsigned extra_table_roman[] = { // unicodes/*names*/ of other characters from PostScript standard fonts
    0x192/*florin*/, 0x2C6/*circumflex*/, 0x2C7/*caron*/, 
    0x2D8/*breve*/, 0x2D9/*dotaccent*/, 0x2DA/*ring*/, 0x2DB/*ogonek*/, 0x2DC/*tilde*/, 0x2DD/*hungarumlaut*/,
    0x2013/*endash*/, 0x2014/*emdash*/, 0x2018/*quoteleft*/, 0x2019/*quoteright*/, 
    0x201A/*quotesinglbase*/, 0x201C/*quotedblleft*/, 0x201D/*quotedblright*/, 0x201E/*quotedblbase*/, 
    0x2020/*dagger*/, 0x2021/*daggerdbl*/, 0x2022/*bullet*/,
    0x2026/*ellipsis*/, 0x2030/*perthousand*/, 0x2039/*guilsinglleft*/, 0x203A/*guilsinglright*/, 
    0x2044/*fraction*/, 0x20AC/*Euro*/, 0x2122/*trademark*/, 
    0x2202/*partialdiff*/, 0x2206/*Delta*/, 0x2211/*summation*/, 0x221A/*radical*/,
    0x221E/*infinity*/, 0x2260/*notequal*/, 0x2264/*lessequal*/, 
    0x2265/*greaterequal*/, 
    0x25CA/*lozenge*/, 0xFB01/*fi*/, 0xFB02/*fl*/,
    0xF8FF/*apple*/
  };
  for ( i = 0; i < sizeof(extra_table_roman)/sizeof(int); i++) {
    if (extra_table_roman[i] == utf) return i + 0x180;
  }
  return 0;
}

// outputs in PostScript a UTF8 string using the same width in points as on display
void fltk3::PostScriptGraphicsDriver::transformed_draw(const char* str, int n, double x, double y) {
  int len, code;
  if (!n || !str || !*str) return;
  // compute display width of string
  int w = (int)width(str, n);
  if (w == 0) return;
  x += origin_x(); y += origin_y();
  if (GraphicsDriver::font() >= fltk3::FREE_FONT) {
    transformed_draw_extra(str, n, x, y, w, output, this, false);
    return;
    }
  fprintf(output, "%d <", w);
  // transforms UTF8 encoding to our custom PostScript encoding as follows:
  // extract each unicode character
  // if unicode <= 0x17F, unicode and PostScript codes are identical
  // if unicode is one of the values listed in extra_table_roman above
  //    its PostScript code is 0x180 + the character's rank in extra_table_roman
  // if unicode is something else, draw all string as bitmap image

  const char *last = str + n;
  const char *str2 = str;
  while (str2 < last) {
    // Extract each unicode character of string.
    unsigned utf = fltk3::utf8decode(str2, last, &len);
    str2 += len;
    if (utf <= 0x17F) { // until Latin Extended-A
      ;
      }
    else if ( (code = is_in_table(utf)) != 0) { // other handled characters
      utf = code;
      }
    else { // unhandled character: draw all string as bitmap image
      fprintf(output, "> pop pop\n"); // close and ignore the opened hex string
      transformed_draw_extra(str, n, x, y, w, output, this, false);
      return;
    }
    fprintf(output, "%4.4X", utf);
  }
  fprintf(output, "> %g %g show_pos_width\n", x, y);
}

void fltk3::PostScriptGraphicsDriver::rtl_draw(const char* str, int n, int x, int y) {
  int w = (int)width(str, n);
  transformed_draw_extra(str, n, x - w, y, w, output, this, true);
}

void fltk3::PostScriptGraphicsDriver::concat(){
  fprintf(output,"[%g %g %g %g %g %g] CT\n", fl_matrix->a , fl_matrix->b , fl_matrix->c , fl_matrix->d , 
	  fl_matrix->x + origin_x() , fl_matrix->y + origin_y());
}

void fltk3::PostScriptGraphicsDriver::reconcat(){
  fprintf(output, "[%g %g %g %g %g %g] RCT\n" , fl_matrix->a , fl_matrix->b , fl_matrix->c , fl_matrix->d , 
	  fl_matrix->x + origin_x() , fl_matrix->y + origin_y());
}

/////////////////  transformed (double) drawings ////////////////////////////////


void fltk3::PostScriptGraphicsDriver::begin_points(){
  fprintf(output, "GS\n");
  concat();
  fprintf(output, "BP\n");
  gap_=1;
  shape_=POINTS;
}

void fltk3::PostScriptGraphicsDriver::begin_line(){
  fprintf(output, "GS\n");
  concat();
  fprintf(output, "BP\n");
  gap_=1;
  shape_=LINE;
}

void fltk3::PostScriptGraphicsDriver::begin_loop(){
  fprintf(output, "GS\n");
  concat();
  fprintf(output, "BP\n");
  gap_=1;
  shape_=LOOP;
}

void fltk3::PostScriptGraphicsDriver::begin_polygon(){
  fprintf(output, "GS\n");
  concat();
  fprintf(output, "BP\n");
  gap_=1;
  shape_=POLYGON;
}

void fltk3::PostScriptGraphicsDriver::vertex(double x, double y){
  if(shape_==POINTS){
    fprintf(output,"%g %g MT\n", x , y);
    gap_=1;
    return;
  }
  if(gap_){
    fprintf(output,"%g %g MT\n", x , y);
    gap_=0;
  }else
    fprintf(output, "%g %g LT\n", x , y);
}

void fltk3::PostScriptGraphicsDriver::curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3){
  if(shape_==NONE) return;
  if(gap_)
    fprintf(output,"%g %g MT\n", x , y);
  else
    fprintf(output, "%g %g LT\n", x , y);
  gap_=0;
  
  fprintf(output, "%g %g %g %g %g %g curveto \n", x1, y1, x2, y2, x3, y3);
}


void fltk3::PostScriptGraphicsDriver::circle(double x, double y, double r){
  if (shape_ == NONE){
    fprintf(output, "GS\n");
    concat();
    fprintf(output,"%g %g %g 0 360 arc\n", x , y , r);
    reconcat();
    fprintf(output, "GR\n");
  } else
    fprintf(output, "%g %g %g 0 360 arc\n", x , y , r);
}

void fltk3::PostScriptGraphicsDriver::arc(double x, double y, double r, double start, double a){
  if(shape_==NONE) return;
  gap_=0;
  if(start>a)
    fprintf(output, "%g %g %g %g %g arc\n", x , y , r , -start, -a);
  else
    fprintf(output, "%g %g %g %g %g arcn\n", x , y , r , -start, -a);
  
}

void fltk3::PostScriptGraphicsDriver::arc(int x, int y, int w, int h, double a1, double a2) {
  if (w <= 1 || h <= 1) return;
  fprintf(output, "GS\n");
  begin_line();
  fprintf(output, "%g %g TR\n", x + w/2.0 -0.5 , y + h/2.0 - 0.5);
  fprintf(output, "%g %g SC\n", (w-1)/2.0 , (h-1)/2.0 );
  arc(0,0,1,a2,a1);
  fprintf(output, "%g %g SC\n", 2.0/(w-1) , 2.0/(h-1) );
  fprintf(output, "%g %g TR\n", -x - w/2.0 +0.5 , -y - h/2.0 +0.5);
  end_line();
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::pie(int x, int y, int w, int h, double a1, double a2) {
  fprintf(output, "GS\n");
  begin_polygon();
  fprintf(output, "%g %g TR\n", x + w/2.0 -0.5 , y + h/2.0 - 0.5);
  fprintf(output, "%g %g SC\n", (w-1)/2.0 , (h-1)/2.0 );
  vertex(0,0);
  arc(0.0,0.0, 1, a2, a1);
  end_polygon();
  fprintf(output, "GR\n");
}

void fltk3::PostScriptGraphicsDriver::end_points(){
  gap_=1;
  reconcat();
  fprintf(output, "ELP\n"); //??
  fprintf(output, "GR\n");
  shape_=NONE;
}

void fltk3::PostScriptGraphicsDriver::end_line(){
  gap_=1;
  reconcat();
  fprintf(output, "ELP\n");
  fprintf(output, "GR\n");
  shape_=NONE;
}
void fltk3::PostScriptGraphicsDriver::end_loop(){
  gap_=1;
  reconcat();
  fprintf(output, "ECP\n");
  fprintf(output, "GR\n");
  shape_=NONE;
}

void fltk3::PostScriptGraphicsDriver::end_polygon(){
  
  gap_=1;
  reconcat();
  fprintf(output, "EFP\n");
  fprintf(output, "GR\n");
  shape_=NONE;
}

void fltk3::PostScriptGraphicsDriver::transformed_vertex(double x, double y){
  reconcat();
  if(gap_){
    fprintf(output, "%g %g MT\n", x , y);
    gap_=0;
  }else
    fprintf(output, "%g %g LT\n", x , y);
  concat();
}

/////////////////////////////   Clipping /////////////////////////////////////////////

void fltk3::PostScriptGraphicsDriver::push_clip(int x, int y, int w, int h) {
  Clip * c=new Clip();
  clip_box(x,y,w,h,c->x,c->y,c->w,c->h);
  c->prev=clip_;
  clip_=c;
  fprintf(output, "CR\nCS\n");
  if(lang_level_<3)
    recover();
  fprintf(output, "%g %g %i %i CL\n", clip_->x + origin_x() - 0.5 , clip_->y + origin_y() - 0.5 , clip_->w  , clip_->h);
  
}

void fltk3::PostScriptGraphicsDriver::push_no_clip() {
  Clip * c = new Clip();
  c->prev=clip_;
  clip_=c;
  clip_->x = clip_->y = clip_->w = clip_->h = -1;
  fprintf(output, "CR\nCS\n");
  if(lang_level_<3)
    recover();
}

void fltk3::PostScriptGraphicsDriver::pop_clip() {
  if(!clip_)return;
  Clip * c=clip_;
  clip_=clip_->prev;
  delete c;
  fprintf(output, "CR\nCS\n");
  if(clip_ && clip_->w >0)
    fprintf(output, "%g %g %i %i CL\n", clip_->x - 0.5, clip_->y - 0.5, clip_->w  , clip_->h);
  // uh, -0.5 is to match screen clipping, for floats there should be something beter
  if(lang_level_<3)
    recover();
}

int fltk3::PostScriptGraphicsDriver::clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H){
  if(!clip_){
    X=x;Y=y;W=w;H=h;
    return 1;
  }
  if(clip_->w < 0){
    X=x;Y=y;W=w;H=h;
    return 1;
  }
  int ret=0;
  if (x > (X=clip_->x)) {X=x; ret=1;}
  if (y > (Y=clip_->y)) {Y=y; ret=1;}
  if ((x+w) < (clip_->x+clip_->w)) {
    W=x+w-X;
    ret=1;
  }else
    W = clip_->x + clip_->w - X;
  if(W<0){
    W=0;
    return 1;
  }
  if ((y+h) < (clip_->y+clip_->h)) {
    H=y+h-Y;
    ret=1;
  }else
    H = clip_->y + clip_->h - Y;
  if(H<0){
    W=0;
    H=0;
    return 1;
  }
  return ret;
}

int fltk3::PostScriptGraphicsDriver::not_clipped(int x, int y, int w, int h){
  if(!clip_) return 1;
  if(clip_->w < 0) return 1;
  int X, Y, W, H;
  clip_box(x, y, w, h, X, Y, W, H);
  if(W) return 1;
  return 0;
}


void fltk3::PostScriptFileDevice::margins(int *left, int *top, int *right, int *bottom) // to implement
{
  fltk3::PostScriptGraphicsDriver *ps = driver();
  if(left) *left = (int)(ps->left_margin / ps->scale_x + .5);
  if(right) *right = (int)(ps->left_margin / ps->scale_x + .5);
  if(top) *top = (int)(ps->top_margin / ps->scale_y + .5);
  if(bottom) *bottom = (int)(ps->top_margin / ps->scale_y + .5);
}

int fltk3::PostScriptFileDevice::printable_rect(int *w, int *h)
//returns 0 iff OK
{
  fltk3::PostScriptGraphicsDriver *ps = driver();
  if(w) *w = (int)((ps->pw_ - 2 * ps->left_margin) / ps->scale_x + .5);
  if(h) *h = (int)((ps->ph_ - 2 * ps->top_margin) / ps->scale_y + .5);
  return 0;
}

void fltk3::PostScriptFileDevice::origin(int *x, int *y)
{
  PagedDevice::origin(x, y);
}

void fltk3::PostScriptFileDevice::origin(int x, int y)
{
  x_offset = x;
  y_offset = y;
  fltk3::PostScriptGraphicsDriver *ps = driver();
  fprintf(ps->output, "GR GR GS %d %d TR  %f %f SC %d %d TR %f rotate GS\n", 
	  ps->left_margin, ps->top_margin, ps->scale_x, ps->scale_y, x, y, ps->angle);
}

void fltk3::PostScriptFileDevice::scale (float s_x, float s_y)
{
  if (s_y == 0.) s_y = s_x;
  fltk3::PostScriptGraphicsDriver *ps = driver();
  ps->scale_x = s_x;
  ps->scale_y = s_y;
  fprintf(ps->output, "GR GR GS %d %d TR  %f %f SC %f rotate GS\n", 
	  ps->left_margin, ps->top_margin, ps->scale_x, ps->scale_y, ps->angle);
}

void fltk3::PostScriptFileDevice::rotate (float rot_angle)
{
  fltk3::PostScriptGraphicsDriver *ps = driver();
  ps->angle = - rot_angle;
  fprintf(ps->output, "GR GR GS %d %d TR  %f %f SC %d %d TR %f rotate GS\n", 
	  ps->left_margin, ps->top_margin, ps->scale_x, ps->scale_y, x_offset, y_offset, ps->angle);
}

void fltk3::PostScriptFileDevice::translate(int x, int y)
{
  fprintf(driver()->output, "GS %d %d translate GS\n", x, y);
}

void fltk3::PostScriptFileDevice::untranslate(void)
{
  fprintf(driver()->output, "GR GR\n");
}

int fltk3::PostScriptFileDevice::start_page (void)
{
  fltk3::PostScriptGraphicsDriver *ps = driver();
  ps->page(ps->page_format_);
  x_offset = 0;
  y_offset = 0;
  ps->scale_x = ps->scale_y = 1.;
  ps->angle = 0;
  fprintf(ps->output, "GR GR GS %d %d translate GS\n", ps->left_margin, ps->top_margin);
  return 0;
}

int fltk3::PostScriptFileDevice::end_page (void)
{
  return 0;
}

void fltk3::PostScriptFileDevice::end_job (void)
// finishes PostScript & closes file
{
  fltk3::PostScriptGraphicsDriver *ps = driver();
  if (ps->nPages) {  // for eps nPages is 0 so it is fine ....
    fprintf(ps->output, "CR\nGR\nGR\nGR\nSP\n restore\n");
    if (!ps->pages_){
      fprintf(ps->output, "%%%%Trailer\n");
      fprintf(ps->output, "%%%%Pages: %i\n" , ps->nPages);
    };
  } else
    fprintf(ps->output, "GR\n restore\n");
  fputs("%%EOF",ps->output);
  ps->reset();
  fflush(ps->output);
  if(ferror(ps->output)) {
    fltk3::alert ("Error during PostScript data output.");
    }
  if (ps->close_cmd_) {
    (*ps->close_cmd_)(ps->output);
  } else {
    fclose(ps->output);
    }
  while (ps->clip_){
    fltk3::PostScriptGraphicsDriver::Clip * c= ps->clip_;
    ps->clip_= ps->clip_->prev;
    delete c;
  }
  fltk3::DisplayDevice::display_device()->set_current();
}

#if ! (defined(__APPLE__) || defined(WIN32) )
int fltk3::PostScriptPrinter::start_job(int pages, int *firstpage, int *lastpage) {
  enum fltk3::PagedDevice::Page_Format format;
  enum fltk3::PagedDevice::Page_Layout layout;

  // first test version for print dialog
  if (!print_panel) make_print_panel();
  print_load();
  print_selection->deactivate();
  print_all->setonly();
  print_all->do_callback();
  print_from->value("1");
  { char tmp[10]; snprintf(tmp, sizeof(tmp), "%d", pages); print_to->value(tmp); }
  print_panel->show(); // this is modal
  while (print_panel->shown()) fltk3::wait();
  
  if (!print_start) // user clicked cancel
    return 1;

  // get options
  switch (print_page_size->value()) {
    case 0:
      format = fltk3::PagedDevice::LETTER;
      break;
    case 2:
      format = fltk3::PagedDevice::LEGAL;
      break;
    case 3:
      format = fltk3::PagedDevice::EXECUTIVE;
      break;
    case 4:
      format = fltk3::PagedDevice::A3;
      break;
    case 5:
      format = fltk3::PagedDevice::A5;
      break;
    case 6:
      format = fltk3::PagedDevice::B5;
      break;
    case 7:
      format = fltk3::PagedDevice::ENVELOPE;
      break;
    case 8:
      format = fltk3::PagedDevice::DLE;
      break;
    default:
      format = fltk3::PagedDevice::A4;
  }
  { // page range choice
    int from = 1, to = pages;
    if (print_pages->value()) {
      sscanf(print_from->value(), "%d", &from);
      sscanf(print_to->value(), "%d", &to);
    }
    if (from < 1) from = 1;
    if (to > pages) to = pages;
    if (to < from) to = from;
    if (firstpage) *firstpage = from;
    if (lastpage) *lastpage = to;
    if (pages > 0) pages = to - from + 1;
  }
  
  if (print_output_mode[0]->value()) layout = fltk3::PagedDevice::PORTRAIT;
  else if (print_output_mode[1]->value()) layout = fltk3::PagedDevice::LANDSCAPE;
  else if (print_output_mode[2]->value()) layout = fltk3::PagedDevice::PORTRAIT;
  else layout = fltk3::PagedDevice::LANDSCAPE;

  int print_pipe = print_choice->value();	// 0 = print to file, >0 = printer (pipe)

  const char *media = print_page_size->text(print_page_size->value());
  const char *printer = (const char *)print_choice->menu()[print_choice->value()].user_data();
  if (!print_pipe) printer = "<File>";

  if (!print_pipe) // fall back to file printing
    return fltk3::PostScriptFileDevice::start_job (pages, format, layout);

  // Print: pipe the output into the lp command...

  char command[1024];
  snprintf(command, sizeof(command), "lp -s -d %s -n %d -t '%s' -o media=%s",
             printer, print_collate_button->value() ? 1 : (int)(print_copies->value() + 0.5),
	     "FLTK", media);

  fltk3::PostScriptGraphicsDriver *ps = driver();
  ps->output = popen(command, "w");
  if (!ps->output) {
    fltk3::alert("could not run command: %s\n",command);
    return 1;
  }
  ps->close_command(pclose);
  this->set_current();
  return ps->start_postscript(pages, format, layout); // start printing
}

#endif // ! (defined(__APPLE__) || defined(WIN32) )

#endif // FLTK3_DOXYGEN

//
// End of "$Id$".
//
