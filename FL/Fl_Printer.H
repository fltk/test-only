#ifndef Fl_Printer_H
#define Fl_Printer_H
#include <FL/Fl_Device.H>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

const double FL_POINT = 1;
const double FL_INCH = 72;
const double FL_MM = 2.835;
const double FL_CM = 28.35;
const double FL_PICA = 12;



const int NO_PAGE_FORMATS=30;


class FL_EXPORT Fl_Printer:public Fl_Device{
  //static const char * creator_;

public:

  enum PAGE_FORMAT{
    A0 = 0, A1, A2, A3, A4, A5, A6, A7, A8, A9,
    B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10,
    C5E, DLE, EXECUTIVE, FOLIO, LEDGER, LEGAL,
    LETTER, TABLOID, ENVELOPE,
    MEDIA = 0x1000
  };
  enum PAGE_LAYOUT {PORTRAIT = 0, LANDSCAPE = 0x100, REVERSED = 0x200};
  //void creator(const char * c){creator_=c;};


protected:

  int nPages;
  //double lm_,tm_,rm_,bm_;
  double pw_, ph_;
  //int orientation_;

public:
//  static const char * format_names;
  static const int page_formats[NO_PAGE_FORMATS][2];
  virtual void page(double pw, double ph, int media = 0)=0;
  virtual void page(int format)=0;
  double page_width(){return pw_;};
  double page_height(){return ph_;};
  //virtual   void page()=0;
  //virtual void margins(double left, double top, double right, double bottom)=0;

  //virtual void fit(double x, double y, double w, double h, int align)=0;
  //virtual void fit(double x, double y, double w, double h, double dpi, int align)=0;
  //void fit(Fl_Widget * w, int align);
  //void fit(Fl_Widget * w, double dpi, int align);

  virtual void place(double x, double y,  double tx, double ty,  double scale = 1)=0;
  virtual void place(double x, double y, double w, double h, double tx, double ty, double tw, double th, int align = FL_ALIGN_CENTER);
  void place(Fl_Widget * w, double tx, double ty, double tw, double th, int align = FL_ALIGN_CENTER ){
    place(w->x(), w->y(), w->w(), w->h(), tx, ty, tw, th, align);
  };
    //void place(Fl_Widget * w, double tx, double ty, double tw, double th, double dpi, int align) {place(w->x(), w->y(), w->w(), w->h(), tx, ty, tw, th, dpi, align);};
   // Fl_Printer():lm_(72), tm_(72), rm_(72), bm_(72)/*,orientation_(0)*/{};
};





#endif
