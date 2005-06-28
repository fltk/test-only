namespace fltk {

class Picture {
 public:
  CGImageRef rgb;
  bool drawn;
  bool opaque;
  Picture() {
    rgb = 0;
    drawn = false;
    opaque = false;
  }
  ~Picture();
};
}

extern fltk::Picture* fl_current_picture;

inline fltk::Picture::~Picture() {
  //stop_drawing(rgb);
  if (fl_current_picture == this) fl_current_picture = 0;
  if (rgb) CGImageRelease(rgb);
}
