//
// "$Id: Fl_Ps.cxx,v 1.1.2.1 2003/11/07 04:02:04 easysw Exp $"
//
// Postscript printing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 2002 by Bill Spitzak and others.
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
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <FL/Fl_Ps.H>
#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/fl_utf8.H>
#include <FL/Fl_Gdi.H>

#ifdef WIN32
#include "Fl_Gdi.cxx"
#elif __MACOS__
#include "Fl_Cpm.cxx"
#endif 

void FL_EXPORT Fl_PrintingGroup::print(Fl_Device *d){
		Fl_Device *t=fl;
		fl=d;
		redraw();
		fltk.push_no_clip();
		fl->clip(x(),y(),w(),h());
		draw();
		fl->pop_clip();
		fltk.pop_clip();
		fl=t;
		Fl::redraw();

}

static int  PageFormat[30][2]={

	// A* //
	{2384, 3370},
	{1684, 2384},
	{1191, 1684},
	{842, 1191},
	{595, 842},
	{420, 595},
	{297, 420},
	{210, 297},
	{148, 210},
	{105, 148},
	
	// B* //
	{2920, 4127},
	{2064, 2920},
	{1460, 2064},
	{1032, 1460},
	{729, 1032},
	{516, 729},
	{316, 516},
	{258, 516},
	{181, 258},
	{127, 181},
	{91,127},
	
	// others (look at Fl_Ps.H} //
	{462, 649},
	{312, 623},
	{541, 719},
	{595, 935},
	{1224, 790},
	{612, 1009},
	{612, 790},
	{791, 1224},
	{297, 683}

};

static char Dashes[5][7]={
	{0,0,0,0,0,0,0},
	{8,8,0,0,0,0,0},
	{1,8,0,0,0,0,0},
	{8,8,1,8,0,0,0},
	{8,8,1,8,1,8,0}
};


enum SHAPE{NONE=0, LINE, LOOP, POLYGON, PSPOINTS};


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

"/CL { \n"
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

"/SP { showpage } bind def\n"
"/LW { setlinewidth } bind def\n"
"/CF /Courier def\n"
"/SF { /CF exch def } bind def\n"
"/fsize 12 def\n"
"/FS { /fsize exch def fsize CF findfont exch scalefont setfont }def \n"
"/GL { setgray } bind def\n"
"/SRGB { setrgbcolor } bind def\n"
// Circle
"/CI { gsave /py exch def /px exch def /sy exch def /sx exch def\n"
	"/picstr px 3 mul string def\n"
	"translate \n"
	"sx sy scale px py 8 \n"
	"[ px 0 0 py neg 0 py ]\n"
	"{ currentfile picstr readhexstring pop } false 3 colorimage grestore\n"
"} bind def\n"

"/GI { gsave /py exch def /px exch def /sy exch def /sx exch def \n"
	"/picstr px string def\n"
	"translate \n"
	"sx sy scale px py 8 \n"
	"[ px 0 0 py neg 0 py ]\n"
	"{ currentfile picstr readhexstring pop } image grestore\n"
"} bind def\n"

"/BFP { newpath moveto }  def\n"
"/BP { newpath } bind def \n"
"/PL { lineto } bind def \n"
"/PM { moveto } bind def \n"
"/MT { moveto } bind def \n"
"/LT { lineto } bind def \n"
"/r { rectfill } bind def \n"
"/EFP { closepath fill } bind def\n"	//was:stroke
"/ELP { stroke } bind def\n"	
"/ECP { closepath stroke } bind def\n"	// Closed (loop)
"/LW { setlinewidth } bind def\n"
"/GS { gsave } bind  def\n"
"/GR { grestore } bind def\n"
"/TR { translate } bind def\n"
"/CT { concat } bind def\n"
"/RCT { matrix invertmatrix concat} bind def\n"
"/SC { scale } bind def\n"
"/fontstr 2 string def\n"
;



/////////////////////////////////////////////////////////////////
void Fl_Ps::concat(){
	double a,b,c,d,x,y;
	fl_matrix(a,b,c,d,x,y);
	fprintf(output, "[%g %g %g %g %g %g] CT\n", a, b, c, d, x, y);
}

void Fl_Ps::reconcat(){
	double a,b,c,d,x,y;
	fl_matrix(a,b,c,d,x,y);
	fprintf(output, "[%g %g %g %g %g %g] RCT\n", a, b, c, d, x, y);
}

void Fl_Ps::recover(){
	if (colored_) color(cr_,cg_,cb_);
	if (line_styled_) line_style(linestyle_,linewidth_,linedash_);
	if (fonted_) font(font_,size_);
	colored_=line_styled_=fonted_=0;
};
	
void Fl_Ps::reset(){
	gap_=1;
	clip_=0;
	cr_=cg_=cb_=0;
	font_=FL_HELVETICA;
	linewidth_=0;
	linestyle_=FL_SOLID;
	strcpy(linedash_,Dashes[FL_SOLID]);
	Clip *c=clip_;   ////just not to have memory leaks for badly writen code (forgotten clip popping)
	while(c){
		clip_=clip_->prev;
		delete c;
		c=clip_;
	}
	colored_=1;
	line_styled_=1;
	fonted_=1;
	while (nb_fonts > 0) {
		nb_fonts--;
		if (fonts[nb_fonts]) free(fonts[nb_fonts]);
	}
	if (fonts) free(fonts);
	fonts = NULL;

	if (bit) fl_delete_offscreen(bit);
	pw = ph = 0;
};

 
Fl_Ps::~Fl_Ps() {
	if(nPages){  // for eps nPages is 0 so it is fine ....
		fprintf(output, "CR\n SP\n GR\n restore\n");
		fprintf(output, "%%%%Trailer\n");
		fprintf(output, "%%%%Pages: %d\n", nPages);
	}else
		fprintf(output, "CR\n GR\n restore\n");
	reset();
}


Fl_Ps::Fl_Ps(FILE *o, int lang_level, int orientation, int w,  int h){
	type = FL_PS_DEVICE;
	fonts = NULL;
	nb_fonts = 0;
	bit = 0;
#ifdef WIN32
	gc = NULL;
#endif
	lang_level_=lang_level;
	output=o;
	g_orientation_=orientation;
	g_format=-1;
	if(!h){
		g_format=w;
		if (orientation&1){
			h=PageFormat[w][0];
			w=PageFormat[w][1];
		}else{
			h=PageFormat[w][1];
			w=PageFormat[w][0];
		}
	}
	fprintf(output, "%%!PS-Adobe-3.0\n");
	if(lang_level_>1)
		fprintf(output, "%%LanguageLevel: %d\n", lang_level_);
	fprintf(output, "%%%%Pages: (atend)\n");
	fprintf(output, "%%%%BoundingBox: 0 0 %d %d\n", w, h);
	g_width_= width_ = w;
	g_height_= height_ = h;
	if(orientation)
		fprintf(output, "%%%%Orientation: %d\n", orientation);
	fprintf(output, "%s", prolog);
	if(lang_level_>=3){
		fprintf(output, "/CS { clipsave } bind def\n");
		fprintf(output, "/CR { cliprestore } bind def\n");
	}else{
		fprintf(output, "/CS { gsave } bind def\n");
		fprintf(output, "/CR { grestore } bind def\n");
	}
	fprintf(output, "%%%%EndProlog\n");

	reset();
	
	nPages=0;

};



///////////////////  eps constructor ////////////////////////////////////
	
Fl_Ps::Fl_Ps(FILE *o, int lang_level, int orientation, int x, int y, int w, int h, int dx, int dy){
	type = FL_PS_DEVICE;
	output=o;
	bit = 0;
	fonts = NULL;
	nb_fonts = 0;
#ifdef WIN32
	gc = NULL;
#endif
	lang_level_=lang_level;
	fprintf(output, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	if(lang_level_>1)
		fprintf(output, "%%%%LanguageLevel: %d\n", lang_level_);
	fprintf(output, "%%%%BoundingBox: %d %d %d %d\n", dx, dy, w+dx, h+dy);
	fprintf(output, "%%%%Orientation: %d\n", orientation);
	width_ = w;
	height_ = h;
	fprintf(output, prolog);
	if(lang_level_>=3){
		fprintf(output, "/CS { clipsave } bind def\n");
		fprintf(output, "/CR { cliprestore } bind def\n");
	}else{
		fprintf(output, "/CS { gsave } bind def\n");
		fprintf(output, "/CR { grestore } bind def\n");
	}
	fprintf(output,"%%%%EndProlog\n");
	fprintf(output, "save\n");
	fprintf(output, "GS\n");

	reset();
	fprintf(output, "%d %d TR\n", -x+dx, h+y+dy);
	fprintf(output, "%d %d SC\n", 1, -1);
	fprintf(output, "%d %d %d %d CL\n", x, y, width_, height_);
	fprintf(output, "CS\n");
	clip_=new Clip();
	clip_->prev=0;
	clip_->x=c_x_=x; clip_->y=c_y_=y; clip_->w=c_w_=width_; clip_->h=c_h_=height_;
	nPages=0;  //must be even for eps!

};


void Fl_Ps::page( int lm, int tm, int rm, int bm, int orientation,  int w, int h) {

	if (nPages){
		fprintf(output, "CR\nSP\nGR\nrestore\n");
	}

	++nPages;
	fprintf(output, "%%%%Page: %d %d\n", nPages, nPages);

	if((w!=g_width_)||(h!=g_height_))
		fprintf(output,"%%%%PageBoundingBox: 0 0 %d %d\n", w, h);
	
	width_=w-lm-rm;
	height_=h-tm-bm;
	if (orientation!=g_orientation_){ 
		fprintf(output, "%%%%PageOrientation: %d\n", orientation);
	}
	reset();
	fprintf(output, "save\n");
	fprintf(output, "%d %d TR\n", lm, (height_+bm));
	fprintf(output, "1 -1 SC\n");
	fprintf(output, "GS\n");
	fprintf(output, "%d %d %d %d CL\n", 0, 0, width_, height_);
	fprintf(output, "CS\n");
	clip_=new Clip();
	clip_->prev=0;
	clip_->x=c_x_=0; clip_->y=c_y_=0; clip_->w=c_w_=width_; clip_->h=c_h_=height_;

};

void Fl_Ps::fit(int x, int y, int w, int h, int align){
	if (w == 0) w = 1;
	if (h == 0) h = 1;
	double dx=0;
	double s=double(width_)/double(w);
	double dy=(height_ - s*h)/2;
	if(((double)height_/(double)h)<s){
		s=(double)height_/(double)h;
		dy=0;
		dx=(width_- s*w)/2;
	}
	if(align & 3)
		if(align & FL_ALIGN_TOP)
			dy=0;
		else
			dy *=2;
	if(align & 12)
		if(align & FL_ALIGN_LEFT)
			dx=0;
		else
			dx *=2;
	fprintf(output,"CR\nGR\n");
	reset();
	fprintf(output, "%g %g TR\n", -x+dx, -y+dy);
	fprintf(output, "%g %g SC\n", s, s);
	fprintf(output, "%d %d %d %d CL\n", x, y, w, h);
	fprintf(output, "CS\nGS\n");
	clip_=new Clip();
	clip_->prev=0;
	clip_->x=x; clip_->y=y; clip_->w=w; clip_->h=h;
	c_x_=x; c_y_=y; c_w_=w; c_h_=h;
};



void Fl_Ps::page(int lm, int tm, int rm, int bm, int orientation, int format){
	int w,h;
	if(format < 0 && orientation == PS_UNCHANGED){
		w=g_width_;
		h=g_height_;
	}else{
		if(format < 0)
			format=g_format;
		if(orientation &1){
			h=PageFormat[format][0];
			w=PageFormat[format][1];
		}else{
			w=PageFormat[format][0];
			h=PageFormat[format][1];
		}
	}
	if (orientation==PS_UNCHANGED)
		orientation = g_orientation_;
	page(lm,tm,rm,bm,orientation,w,h);
};




//////////////////////////////// Primitives: Colors  ////////////////////////////////////////////

void Fl_Ps::color(Fl_Color c) {
	colored_=1;
	color_=c;
	Fl::get_color(c, cr_, cg_, cb_);
	if (cr_==cg_ && cg_==cb_) {
		double gray = cr_/255.0;
		fprintf(output, "%g GL\n", gray);
	} else {
		double fr, fg, fb;
		fr = cr_/255.0;
		fg = cg_/255.0;
		fb = cb_/255.0;
		fprintf(output, "%g %g %g SRGB\n", fr, fg, fb);
	}
}

void Fl_Ps::color(unsigned char r, unsigned char g, unsigned char b) {
	
	colored_=1;
	cr_=r;cg_=g;cb_=b;
	if (r==g && g==b) {
		double gray = r/255.0;
		fprintf(output, "%g GL\n", gray);
	} else {
		double fr, fg, fb;
		fr = r/255.0;
		fg = g/255.0;
		fb = b/255.0;
		fprintf(output, "%g %g %g SRGB\n", fr, fg, fb);
	}
}

/////////////////////////////   Clipping /////////////////////////////////////////////

void Fl_Ps::clip(double x, double y, double w, double h) {
	Clip * c=new Clip();
	clip_box(x,y,w,h,c->x,c->y,c->w,c->h);
	c->prev=clip_;
	clip_=c;
	fprintf(output, "CR\nCS\n");
	if(lang_level_<3)
		recover();
	fprintf(output, "%g %g %g %g CL\n", clip_->x, clip_->y, clip_->w , clip_->h);
}
void Fl_Ps::push_no_clip() {
	Clip * c = new Clip();
	c->prev=clip_;
	clip_=c;
	clip_->x=c_x_;clip_->y=c_y_;clip_->w=c_w_;clip_->h=c_h_;
	fprintf(output, "CR\nCS\n");
	if(lang_level_<3)
		recover();
}

void Fl_Ps::pop_clip() {
	if(!clip_)return;
	Clip * c=clip_;
	clip_=clip_->prev;
	delete c;
	fprintf(output, "GR\nGS\n");
	if(clip_){
		fprintf(output, "%g %g %g %g CL\n", clip_->x, clip_->y, clip_->w, clip_->h);
	}
	if(lang_level_<3)
		recover();

}

int Fl_Ps::not_clipped(double x, double y, double w, double h){
	if(!clip_) return 1;
	if(clip_->w<0) return 1;
	if ((x+w) < clip_->x) return 0;
	if ((y+h) < clip_->y) return 0;
	if (x > (clip_->x+clip_->w)) return 0;
	if (y > (clip_->y+clip_->h)) return 0;
	return 1;
};

int Fl_Ps::clip_box(double x, double y, double w, double h, double &X, double &Y, double &W, double &H){
	if(!clip_){
		X=x;Y=y;W=w;H=h;
		return 1;
	}
	if(clip_->w<0){
		X=x;Y=y;W=w;H=h;
		return 1;
	}
	int ret=0;
	if (x > (X=clip_->x)) {X=x; ret=1;}
	if (y > (Y=clip_->y)) {Y=y; ret=1;}
	if ((x+w) < (clip_->x+clip_->w)) {W=w; ret=1;}
	else W = clip_->x + clip_->w - x;
	if ((y+h) < (clip_->y+clip_->h)) {H=h; ret=1;}
	else H = clip_->h + clip_->h-y;
	return ret;
};

int Fl_Ps::clip_box(int x, int y, int w, int h, int& X, int& Y, int& W, int& H){
	double _x, _y, _w, _h;
	int ret = clip_box((double)x, (double)y, (double)w, (double)h,
		 _x, _y, _w, _h);
	X=(int)(_x+.5);
	Y=(int)(_y+.5);
	W=(int)(_w+.5);
	H=(int)(_h+.5);
	return ret;
};

///////////////////////////////// rect  /////////////////////////////////////////





void Fl_Ps::rect(int x, int y, int w, int h) {
	fprintf(output, "GS\n");
	fprintf(output,"%d %d %d %d R\n", x, y, w, h);
	fprintf(output, "GR\n");
}

void Fl_Ps::rectf(int x, int y, int w, int h) {
	fprintf(output, "%d %d %d %d FR\n", x, y, w, h);
}
void Fl_Ps::rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b) {

	fprintf(output,"GS\n");
	double fr = r/255.0;
	double fg = g/255.0;
	double fb = b/255.0;
	fprintf(output, "%g %g %g SRGB\n", fr, fg, fb);
	fprintf(output, "%d %d %d %d FR\n", x, y, w, h);
	fprintf(output, "GR\n");
}

void Fl_Ps::line(int x1, int y1, int x2, int y2) {
	fprintf(output, "GS\n");
	fprintf(output, "%d %d %d %d L\n", x1, y1, x2, y2);
	fprintf(output, "GR\n");
}

void Fl_Ps::line(int x0, int y0, int x1, int y1, int x2, int y2) {
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%d %d MT\n", x0, y0);
	fprintf(output, "%d %d LT\n", x1, y1);
	fprintf(output, "%d %d LT\n", x2, y2);
	fprintf(output, "ELP\n");
	fprintf(output, "GR\n");
}

void Fl_Ps::loop(int x0, int y0, int x1, int y1, int x2, int y2) {
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%d %d MT\n", x0, y0);
	fprintf(output, "%d %d LT\n", x1, y1);
	fprintf(output, "%d %d LT\n", x2, y2);
	fprintf(output, "ECP\n");
	fprintf(output, "GR\n");
}

void Fl_Ps::loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%d %d MT\n", x0, y0);
	fprintf(output, "%d %d LT\n", x1, y1);
	fprintf(output, "%d %d LT\n", x2, y2);
	fprintf(output, "%d %d LT\n", x3, y3);
	fprintf(output, "ECP\n");
	fprintf(output, "GR\n");
}

void Fl_Ps::polygon(int x0, int y0, int x1, int y1, int x2, int y2) {
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%d %d MT\n", x0, y0);
	fprintf(output, "%d %d LT\n", x1, y1);
	fprintf(output, "%d %d LT\n", x2, y2);
	fprintf(output, "EFP\n");
	fprintf(output, "GR\n");
}

void Fl_Ps::polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%d %d MT\n", x0, y0);
	fprintf(output, "%d %d LT\n", x1, y1);
	fprintf(output, "%d %d LT\n", x2, y2);
	fprintf(output, "%d %d LT\n", x3, y3);
	fprintf(output, "EFP\n");
	fprintf(output, "GR\n");
}

void Fl_Ps::xyline(int x, int y, int x1, int y2, int x3){
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%d %d MT\n", x, y);
	fprintf(output, "%d %d LT\n", x1, y);
	fprintf(output, "%d %d LT\n", x1, y2);
	fprintf(output, "%d %d LT\n", x3, y2);
	fprintf(output, "ELP\n");
	fprintf(output, "GR\n");
};

void Fl_Ps::xyline(int x, int y, int x1, int y2){
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%d %d MT\n", x, y);
	fprintf(output, "%d %d LT\n", x1, y);
	fprintf(output, "%d %d LT\n", x1, y2);
	fprintf(output, "ELP\n");
	fprintf(output, "GR\n");
};

void Fl_Ps::xyline(int x, int y, int x1){
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%d %d MT\n", x, y);
	fprintf(output, "%d %d LT\n", x1, y);
	fprintf(output, "ELP\n");
	fprintf(output, "GR\n");
};

void Fl_Ps::yxline(int x, int y, int y1, int x2, int y3){
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%d %d MT\n", x, y);
	fprintf(output, "%d %d LT\n", x, y1);
	fprintf(output, "%d %d LT\n", x2, y1);
	fprintf(output, "%d %d LT\n", x2, y3);
	fprintf(output, "ELP\n");
	fprintf(output, "GR\n");
};

void Fl_Ps::yxline(int x, int y, int y1, int x2){
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%d %d MT\n", x, y);
	fprintf(output, "%d %d LT\n", x, y1);
	fprintf(output, "%d %d LT\n", x2, y1);
	fprintf(output, "ELP\n");
	fprintf(output, "GR\n");
};

void Fl_Ps::yxline(int x, int y, int y1){
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%d %d MT\n", x, y);
	fprintf(output, "%d %d LT\n", x, y1);
	fprintf(output, "ELP\n");
	fprintf(output, "GR\n");
};



void Fl_Ps::arc(int x, int y, int w, int h, double a1, double a2) {

	if (!w || !h) return;
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%g %g TR\n", (x + w/2.0), (y + h/2.0));
	fprintf(output, "%g %g SC\n", w/2.0, h/2.0);
	fprintf(output, "0 0 1 %g %g arcn\n", -a1, -a2);
	fprintf(output, "%g setlinewidth\n",  2.0/sqrt(w*h));
	fprintf(output, "ELP\n");
	fprintf(output, "GR\n");
}

void Fl_Ps::pie(int x, int y, int w, int h, double a1, double a2) {
	fprintf(output, "GS\n");
	fprintf(output, "BP\n");
	fprintf(output, "%g %g TR\n", x + w/2.0, y + h/2.0);
	fprintf(output, "%g %g SC\n", w/2.0, h/2.0);
	fprintf(output, "0 0 MT\n");
	fprintf(output, "0 0 1 %g %g arcn\n", -a1, -a2);
	fprintf(output, "EFP\n");
	fprintf(output, "GR\n");
}


void Fl_Ps::begin_points(){
	fprintf(output, "GS\n");
	concat();
	fprintf(output, "BP\n");
	gap_=1;
	shape_=PSPOINTS;
};

void Fl_Ps::begin_line(){
	fprintf(output, "GS\n");
	concat();
	fprintf(output, "BP\n");
	gap_=1;
	shape_=LINE;
};

void Fl_Ps::begin_loop(){
	fprintf(output, "GS\n");
	concat();
	fprintf(output, "BP\n");
	gap_=1;
	shape_=LOOP;
};

void Fl_Ps::begin_polygon(){
	fprintf(output,"GS\n");
	concat();
	fprintf(output, "BP\n");
	gap_=1;
	shape_=POLYGON;
};

void Fl_Ps::vertex(double x, double y){
	if(shape_==PSPOINTS){
		fprintf(output, "%g %g MT\n", x, y);
		gap_=1;
		return;
	}
	if(gap_){
		fprintf(output, "%g %g MT\n", x, y);
		gap_=0;
	}else
		fprintf(output, "%g %g LT\n", x, y);
};

void Fl_Ps::curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3){


	if(shape_==NONE) return;
		if(gap_)
			fprintf(output, "%g %g MT\n" , x, y);
		else
			fprintf(output, "%g %g LT\n", x, y);
		gap_=0;

	fprintf(output, "%g %g %g %g %g %g curveto\n", x1, y1, x2, y2, x3, y3);

};
FL_EXPORT void Fl_Ps::circle(double x, double y, double r){
	if(shape_==NONE){
		fprintf(output, "GS\n");
		concat();
		fprintf(output, "BP\n");
		fprintf(output, "%g %g %g 0 350 arc\n", x, y, r);
		reconcat();
		fprintf(output, "ELP\n");
		fprintf(output, "GR\n");
	}else
		fprintf(output, "%g %g %g 0 360 arc\n", x, y, r);
};



	

void Fl_Ps::arc(double x, double y, double r, double start, double a){
	if(shape_==NONE) return;
	gap_=0;
	fprintf(output, "%g %g %g %g %g arc\n", x, y, r, -start, -a);

};

void Fl_Ps::end_points(){
	gap_=1;
	reconcat();
	fprintf(output, "ELP\n"); //??
	fprintf(output, "GR\n");
	shape_=NONE;
}

void Fl_Ps::end_line(){
	gap_=1;
	reconcat();
	fprintf(output, "ELP\n");
	fprintf(output, "GR\n");
	shape_=NONE;
}

void Fl_Ps::end_loop(){
	gap_=1;
	reconcat();
	fprintf(output, "ECP\n");
	fprintf(output,  "GR\n");
	shape_=NONE;
}

void Fl_Ps::end_polygon(){
	gap_=1;
	reconcat();
	fprintf(output, "EFP\n");
	fprintf(output, "GR\n");
	shape_=NONE;
}

void Fl_Ps::transformed_vertex(double x, double y){
	reconcat();
	if(gap_){
		fprintf(output, "%g %g MT\n", x, y);
		gap_=0;
	}else
		fprintf(output, "%g %g LT\n", x, y);
	concat();
};


void Fl_Ps::font(int f, int s) {
	fonted_=1;
	fltk.font(f,s); //Dirty Harry, he needs that for font measurement ;-(
	font_=f; size_=s;
};


void Fl_Ps::line_style(int style, int width, char* dashes){
	int i;
	line_styled_=1;
	if(!width)

		width=1; //for screen drawing compatability
	linewidth_=width;
	linestyle_=style;
	fprintf(output, "%d setlinewidth\n", width);
	if(!dashes)
		dashes=Dashes[style&0xff];
	strcpy(linedash_,dashes);
	fprintf(output, "[");
	while(*dashes){
		fprintf(output, "%d ", (int)(*dashes));;
		dashes++;
	}
	fprintf(output,"] 0 setdash\n");
	style>>=8;
	i = style&0xf;
	if(i)
		fprintf(output, "%d setlinecap\n", (i-1));
	else
		fprintf(output, "1 setlinecap\n"); //standard cap

	style>>=4;
	i = style&0xf;
	if(i)
		fprintf(output, "%d setlinejoin\n", (style&0xf-1));
	else
		fprintf(output,"1 setlinejoin\n");
};

double Fl_Ps::width(const char* s){
	return fltk.width(s); //Dirty Harry
}

double Fl_Ps::width(unsigned int c){
	return fltk.width(c); //Dirty Harry
}

double Fl_Ps::width(const char* s, int n){;
	return fltk.width(s,n); //Very Dirty Harry
}
int Fl_Ps::descent(){
	return fltk.descent(); //A bit Dirty Harry
}
int Fl_Ps::height(){
	return fltk.height(); //Still Dirty Harry
}

int Fl_Ps::get_width(unsigned int ucs)
{
	int size;
	int width;
	int height;
#ifdef WIN32
#elif NANO_X
#elif DJGPP
#elif __MACOS__
 // FIXME
#else
	XImage *i = NULL;	
#endif
	char buf[1024];
	int *glyphs;

	if (fl_font_ >= nb_fonts) {
		fonts = (int**)realloc(fonts, sizeof(int*) *(fl_font_ + 1));
		while (nb_fonts <= fl_font_) {
			fonts[nb_fonts] = NULL;
			nb_fonts++;
		}
	}
	if (fonts[fl_font_] == NULL) {
		fonts[fl_font_] = (int*) malloc(sizeof(int) * 0x10000);
		glyphs = fonts[fl_font_];
		for (long i=0;i<0x10000;i++) glyphs[i] = 0;
	}
	glyphs = fonts[fl_font_];

	if (ucs > 0xFFFF || ucs < 0) return 0;
	if (glyphs[ucs]) return glyphs[ucs];

	size = fl_size_;
	fltk.font(fl_font_, 48);
	int l = fl_ucs2utf(ucs, buf);
	width = (int) fltk.width(buf, l);
	glyphs[ucs] = width;
	if (width < 1) {
		/* Non spacing glyph */
		width = (int) fltk.width((unsigned)ucs);
		glyphs[ucs] = - width;
	}
	height = 48 + fltk.descent();
	width *= 2;
	Fl::first_window()->make_current();
	if (width > 0) {
		if (pw < width || ph < height || bit == 0) {
			if (bit) fl_delete_offscreen(bit);
			bit = fl_create_offscreen(width, height);
			ph = height;
			pw = width;
		}
		fl_begin_offscreen(bit);
		fltk.font(fl_font_, 48);
		fltk.color(FL_WHITE);
		fltk.rectf(0, 0, pw, ph);
		fltk.color(FL_BLACK);

		if (glyphs[ucs] < 1) {
			/* non spacing glyph */
			fltk.draw(buf, l, -glyphs[ucs], 48);
		} else {
			fltk.draw(buf, l, 0, 48);
		}
		fprintf(output, "/f%d_%d {\n", fl_font_, ucs); 
#ifndef WIN32
#if !NANO_X
#if !DJGPP
#if !__MACOS__
		i = XGetImage(fl_display, fl_window, 0, 0, 
			width, height, 1, XYPixmap);
#endif
#endif
#endif
#endif	
			int l;
			for (l=0; l < height; l++) {
				int c;
				int lpix = 1;
				int pixc = 0;
				for (c=0; c < width; c++) {
#ifdef WIN32
					int pix = GetPixel(fl_gc, c, l) ? 1 : 0;
#elif DJGPP
					int pix = 0; // FIXME_DJGPP
#elif NANO_X
					int pix = 0; // FIXME
#elif __MACOS__
					int pix = 0; // FIXME
#else
					int pix = XGetPixel(i, c, l) ? 1 : 0;
#endif
					if (pix != lpix) {
						if (lpix == 0) {
							fprintf(output, 
								"%d %d %d 1 r\n",
								pixc, 
								(l - 48), 
								(c - pixc));
						}
						pixc = c;
					}		
					lpix = pix;
				}
			}
#ifndef WIN32
#if !NANO_X && !DJGPP && !__MACOS__
		XDestroyImage(i);
#endif
#endif
		fl_end_offscreen();
	} else {
		fprintf(output, "/f%d_%d {\n", fl_font_, ucs);
	}

	
	fprintf(output, "} bind def\n"); 
	fltk.font(fl_font_, size);
	return 	glyphs[ucs];
}

void Fl_Ps::transformed_draw(const char* str, int n, double x, double y){
	int i;
	if (!n||!str||!*str)return;
	xchar *buf = (xchar *)malloc(n * sizeof(short));
	n = fl_utf2unicode((const unsigned char*)str, n, buf);
	for(i=0; i < n; i++) {
		get_width(buf[i]);
	}
	
	for(i=0; i < n; i++) {
		fprintf(output, "GS %g %g TR %g %g SC f%d_%d GR\n", 
			x, y,
			fl_size_ / 48.0, 
			fl_size_ / 48.0, 
			fl_font_, buf[i]); 
		x += get_width(buf[i]) * fl_size_ / 48 ;
	}
	
	free(buf);
}

void Fl_Ps::rtl_draw(const char *str, int n, int x, int y) {
	int i;
	if (!n||!str||!*str)return;
	xchar *buf = (xchar *)malloc(n * sizeof(short));
	n = fl_utf2unicode((const unsigned char*)str, n, buf);
	for(i=0; i < n; i++) {
		get_width(buf[i]);
	}
	i = 0;
	int lx = 0;
	while (i < n) {
		lx = get_width(buf[i]) * fl_size_ / 48;
		if (lx < 0) lx = -lx;
		x -= lx;
		fprintf(output, "GS %d %d TR %g %g SC f%d_%d GR\n", 
			x, y,
			fl_size_ / 48.0, 
			fl_size_ / 48.0, 
			fl_font_, buf[i]); 
		if (fl_nonspacing(buf[i])) {
			x += lx;
		}
		i++;
	}
	free(buf);	
}



void Fl_Ps::draw_scalled_image(const uchar *data, double x, double y, double w, double h, int iw, int ih, int D, int LD) {
		fprintf(output, "%g %g %g %g %d %d CI\n", x, y+h, w, -h, iw, ih);
		const uchar *curdata=data;
		if(!LD) LD=iw*D;
		for (int j=0; j<ih;j++){
			curdata=data+j*LD;
			for(int i=0 ; i<iw ; i++) {
				if (!(i%40))
					fprintf(output, "\n");;
				fprintf(output, "%.2x%.2x%.2x", curdata[0], curdata[1], curdata[2]);
				curdata +=D;
			}
		}
		fprintf(output, "\n");
};


void Fl_Ps::draw_scalled_image(Fl_Draw_Image_Cb call, void *data, double x, double y, double w, double h, int iw, int ih, int D) {
	
		fprintf(output, "%g %g %g %g %d %d CI\n", x, y+h, w, -h,  iw, ih);
		int LD=iw*D;
		uchar *rgbdata=new uchar[LD];
		for (int j=0; j<ih;j++){
			call(data,0,j,iw,rgbdata);
			uchar *curdata=rgbdata;
			for(int i=0 ; i<iw ; i++) {
				if (!(i%40))
					fprintf(output, "\n");
				fprintf(output, "%.2x%.2x%.2x", curdata[0], curdata[1], curdata[2]);
				curdata +=D;
			}
		}
		fprintf(output, "\n");;
		delete[] rgbdata;
}

void Fl_Ps::draw_scalled_image_mono(const uchar *data, double x, double y, double w, double h, int iw, int ih, int D, int LD) {

		fprintf(output, "%g %g %g %g %d %d GI\n", x, y+h,  w, -h, iw, ih);
		//int n = iw*ih*D;
		const uchar *curdata=data;
		if(!LD) LD=iw*D;
		for (int j=0; j<ih;j++){
			curdata=data+j*LD;
			for(int i=0 ; i<iw ; i++) {
				if (!(i%120))
					fprintf(output, "\n");;
				fprintf(output, "%.2x", *curdata);
				curdata +=D;
			}
		}
		fprintf(output, "\n");
};


void Fl_Ps::draw_scalled_image_mono(Fl_Draw_Image_Cb call, void *data, double x, double y, double w, double h, int iw, int ih, int D) {
	
	fprintf(output, "%g %g %g %g %d %d CI\n", x, y+h, w, -h, iw,  ih);
	int LD=iw*D;
	uchar *monodata=new uchar[LD];
	for (int j=0; j<ih;j++){
		call(data,0,j,iw,monodata);
		uchar *curdata=monodata;
		for(int i=0 ; i<iw ; i++) {
			if (!(i%120))
				fprintf(output, "\n");
			fprintf(output, "%.2x", *curdata);
			curdata +=D;
		}
	}
	fprintf(output, "\n");
	delete[] monodata;
};



