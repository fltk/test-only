namespace fltk {

class Picture {
 public:
  HBITMAP rgb;
  bool drawn;
  bool isbitmap;
  bool opaque;
  bool mask;
  Color fg;
  Picture() {
    rgb = 0;
    drawn = false;
    isbitmap = false;
    opaque = false;
    mask = false;
  }
  inline ~Picture();
};
}

extern fltk::Picture* fl_current_picture;

inline fltk::Picture::~Picture() {
  stop_drawing(rgb);
  if (fl_current_picture == this) fl_current_picture = 0;
  if (rgb) DeleteObject(rgb);
}
