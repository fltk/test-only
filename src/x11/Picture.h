namespace fltk {

class Picture {
 public:
  XWindow rgb;
  XWindow alpha;
  bool drawn;
  bool mask;
  Color fg;
  Color bg;
  Picture() {
    rgb = alpha = 0;
    drawn = false;
    mask = false;
  }
  ~Picture();
};
}

extern fltk::Picture* fl_current_picture;

inline fltk::Picture::~Picture() {
  stop_drawing(rgb);
  if (fl_current_picture == this) fl_current_picture = 0;
  if (alpha) XFreePixmap(xdisplay, alpha);
  if (rgb) XFreePixmap(xdisplay, rgb);
}
