#ifndef fl_filename_H
#define fl_filename_H

#include <fltk/filename.h>
#include <string.h>

#define fl_filename_name	filename_name
#define fl_filename_ext		filename_ext
#define fl_filename_match	filename_match
#define fl_filename_isdir	filename_isdir
#define fl_filename_list	filename_list

#define FL_PATH_MAX 1024 // all buffers are assummed to be at least this long
inline char *fl_filename_setext(char * name, const char *ext) {
  strcpy((char*)filename_ext(name), ext ? ext : "");
  return name;
}
FL_API bool fl_filename_expand(char *, const char *from);
FL_API bool fl_filename_absolute(char *, const char *from, const char* pwd=0);

#endif
