#ifndef fl_file_chooser_H
#define fl_file_chooser_H

#include <fltk/file_chooser.h>

//char *fl_dir_chooser(const char *message,const char *fname,int relative=0);
inline char *fl_file_chooser(const char *message,const char *pat,const char *fname,int /*relative*/ = 0) {
  return fltk::file_chooser(message, pat, fname);
}

#endif
