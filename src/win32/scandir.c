// "$Id: scandir.c 1339 2006-04-03 22:47:29Z spitzak $"
//
// Copyright 1998-2006 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".

// Emulation of posix scandir() call
// This source file is #include'd by scandir.c
// THIS IS A C FILE! DO NOT CHANGE TO C++!!!

#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <fltk/utf.h>

#ifdef __cplusplus
extern "C" {
#endif

struct dirent { char d_name[1]; };

int scandir(const char *dirname, struct dirent ***namelist,
    int (*select)(struct dirent *),
    int (*compar)(struct dirent **, struct dirent **)) {
  char *d;
  WIN32_FIND_DATA find;
  HANDLE h;
  int nDir = 0, NDir = 0;
  struct dirent **dir = 0, *selectDir;
  unsigned long ret;
  char findIn[MAX_PATH];

  strlcpy ( findIn, dirname, MAX_PATH );
  d = findIn+strlen(findIn);
  if (d==findIn) *d++ = '.';
  if (*(d-1)!='/' && *(d-1)!='\\') *d++ = '/';
  *d++ = '*';
  *d++ = 0;

  if ((h=FindFirstFile(findIn, &find))==INVALID_HANDLE_VALUE) {
    ret = GetLastError();
    if (ret != ERROR_NO_MORE_FILES) {
      // TODO: return some error code
    }
    *namelist = dir;
    return nDir;
  }
  do {
    selectDir=(struct dirent*)malloc(sizeof(struct dirent)+strlen(find.cFileName));
    strcpy(selectDir->d_name, find.cFileName);
    if (!select || (*select)(selectDir)) {
      if (nDir==NDir) {
	struct dirent **tempDir = (struct dirent **)calloc(sizeof(struct dirent*), NDir+33);
	if (NDir) memcpy(tempDir, dir, sizeof(struct dirent*)*NDir);
	if (dir) free(dir);
	dir = tempDir;
	NDir += 32;
      }
      dir[nDir] = selectDir;
      nDir++;
      dir[nDir] = 0;
    } else {
      free(selectDir);
    }
  } while (FindNextFile(h, &find));
  ret = GetLastError();
  if (ret != ERROR_NO_MORE_FILES) {
    // TODO: return some error code
  }
  FindClose(h);

  if (compar) qsort (dir, nDir, sizeof(*dir),
		     (int(*)(const void*, const void*))compar);

  *namelist = dir;
  return nDir;
}

#ifdef __cplusplus
}
#endif

//
// End of "$Id: scandir.c 1339 2006-04-03 22:47:29Z spitzak $".
//