#ifndef Fl_PS_Printer_H
#define Fl_PS_Printer_H
#include <FL/Fl_Printer.H>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
//#include <string.h>
#include <stdio.h>



class FL_EXPORT Fl_PS_Printer:public Fl_Printer{

  enum SHAPE{NONE=0, LINE, LOOP, POLYGON, POINTS};

  class Clip{
  public:
    int x, y, w, h;
    Clip *prev;
  };
  Clip * clip_;

  int lang_level_;
  int font_;
  int size_;
  Fl_Color color_;
  FILE *output;
  //int nPages;
  int gap_;
  int pages_;

  double width_;
  double height_;

  int shape_;
  int linewidth_;// need for clipping, lang level 1-2
  int linestyle_;//
  int interpolate_; //interpolation of images
  unsigned char cr_,cg_,cb_;
  char  linedash_[256];//should be enought
  void concat();  // transform ror scalable dradings...
  void reconcat(); //invert
  void recover(); //recovers the state afrer grestore (such as line styles...)
  void reset();

  uchar * mask;
  int mx; // width of mask;
  int my; // mask lines
  Fl_Color bg_;
  int (*close_cmd_)(FILE *);
  int page_policy_;
  int nPages;

protected:
    int alpha_mask(const uchar * data, int w, int h, int D, int LD=0);

    void transformed_draw(const char* s, int n, double x, double y); //precise text placing
    void transformed_draw(const char* s, double x, double y);

    void draw_scalled_image(const uchar *data, double x, double y, double w, double h, int iw, int ih, int D=3, int LD=0);
    void draw_scalled_image_mono(const uchar *data, double x, double y, double w, double h, int iw, int ih, int D=3, int LD=0);
    void draw_scalled_image(Fl_Draw_Image_Cb call, void *data, double x, double y, double w, double h, int iw, int ih, int D);
    void draw_scalled_image_mono(Fl_Draw_Image_Cb call, void *data, double x, double y, double w, double h, int iw, int ih, int D);


public:
    void page_policy(int p);
    int page_policy(){return page_policy_;};


    void close_command( int (*cmd)(FILE *)){close_cmd_=cmd;};
    FILE * file(){return output;};

    Fl_PS_Printer(FILE *o, int lang_level, int pages = 0); // ps (aslo multi-page) constructor
    Fl_PS_Printer(FILE *o, int lang_level, int x, int y, int w, int h); //eps constructor
    ~Fl_PS_Printer();
    void interpolate(int i){interpolate_=i;};
    int interpolate(){return interpolate_;}

    void page(double pw, double ph, int media = 0);
    void page(int format);

    void Fl_PS_Printer::place(double x, double y, double tx, double ty, double scale = 1);
    
protected:  // implementation of drawing methods

    void color(Fl_Color c);
    void bg_color(Fl_Color bg);
    void color(uchar r, uchar g, uchar b);
    Fl_Color color(){return color_;};


    void push_clip(int x, int y, int w, int h);
    int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H);
    int not_clipped(int x, int y, int w, int h);
    void push_no_clip();
    void pop_clip();


    void point(int x, int y);

    void line_style(int style, int width=0, char* dashes=0);

    void rect(int x, int y, int w, int h);
    void rectf(int x, int y, int w, int h);

    void line(int x1, int y1, int x2, int y2);
    void line(int x1, int y1, int x2, int y2, int x3, int y3);

    void loop(int x1, int y1, int x2 ,int y2, int x3, int y3);
    void loop(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4);

    void polygon(int x1, int y1, int x2 ,int y2, int x3, int y3);
    void polygon(int x1 ,int y1, int x2, int y2, int x3, int y3, int x4, int y4);

    void xyline(int x, int y, int x1);
    void xyline(int x, int y, int x1, int y2);
    void xyline(int x, int y, int x1, int y2, int x3);

    void yxline(int x, int y, int y1);
    void yxline(int x, int y, int y1, int x2);
    void yxline(int x, int y, int y1, int x2, int y3);

    void arc(int x, int y, int w, int h, double a1, double a2);
    void pie(int x, int y, int w, int h, double a1, double a2);

    void begin_points();
    void begin_line();
    void begin_loop();
    void begin_polygon();
    void vertex(double x, double y);
    void curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3);
    void arc(double x, double y, double r, double start, double a);
    void circle(double x, double y, double r);
    void end_points();
    void end_line();
    void end_loop();
    void end_polygon();
    void begin_complex_polygon(){begin_polygon();};
    void gap(){gap_=1;};
    void end_complex_polygon(){end_polygon();};
    void transformed_vertex(double x, double y);

    void font(int face, int size);
    int font(){return font_;};
    int size(){return size_;};
    int height();
    double width(const char* s, int n);
    double width(unsigned c);
    int descent();

    void draw(const char* s, int n, int x, int y){transformed_draw(s,n,x,y);};

    void draw_image(const uchar* d, int x,int y,int w,int h, int delta=3, int ldelta=0){draw_scalled_image(d,x,y,w,h,w,h,delta,ldelta);};
    void draw_image_mono(const uchar* d, int x,int y,int w,int h, int delta=1, int ld=0){draw_scalled_image_mono(d,x,y,w,h,w,h,delta,ld);};
    void draw_image(Fl_Draw_Image_Cb call, void* data, int x,int y, int w, int h, int delta=3){draw_scalled_image(call,data, x, y, w, h, w, h, delta);};
    void draw_image_mono(Fl_Draw_Image_Cb call, void* data, int x,int y, int w, int h, int delta=1){draw_scalled_image_mono(call, data, x, y, w, h, w, h, delta);};

    void rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b);

    void draw(Fl_Pixmap * pxm,int XP, int YP, int WP, int HP, int cx, int cy);
    void draw(Fl_RGB_Image * rgb,int XP, int YP, int WP, int HP, int cx, int cy);
    void draw(Fl_Bitmap * bitmap,int XP, int YP, int WP, int HP, int cx, int cy);
};


//Fl_PS_Printer * fl_psfile_chooser(Fl_PsFile_Settings * s=0, int * begin = 0, int * end = 0);

#endif
