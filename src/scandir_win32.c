/*
 * "$Id: scandir_win32.c,v 1.11.2.4.2.5.2.2 2003/11/07 03:47:25 easysw Exp $"
 *
 * WIN32 scandir function for the Fast Light Tool Kit (FLTK).
 *
 * Copyright 1998-2004 by Bill Spitzak and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "fltk-bugs@fltk.org".
 */

#ifndef __CYGWIN__
/* Emulation of posix scandir() call */
#include <FL/fl_utf8.H>
#include <FL/filename.H>
#include "flstring.h"
#include <windows.h>
#include <stdlib.h>


//extern char *fl_locale2utf8(const char *s, UINT codepage = 0);

int fl_scandir(const char *dirname, struct dirent ***namelist,
	       int (*select)(struct dirent *),
	       int (*compar)(struct dirent **, struct dirent **)) {
  int len;
  char *findIn, *d;
  WIN32_FIND_DATA find;
  WIN32_FIND_DATAW findw;
  HANDLE h;
  int nDir = 0, NDir = 0;
  struct dirent **dir = 0, *selectDir;
  unsigned long ret;
  unsigned short *wbuf;

  len    = strlen(dirname);
  findIn = (char*) malloc(len + 10);
  if (!findIn) return -1;

  strcpy(findIn, dirname);
  for (d = findIn; *d; d++) if (*d=='/') *d='\\';
  if ((len==0)) { strcpy(findIn, ".\\*"); }
  if ((len==1)&& (d[-1]=='.')) { strcpy(findIn, ".\\*"); }
  if ((len>0) && (d[-1]=='\\')) { *d++ = '*'; *d = 0; }
  if ((len>1) && (d[-1]=='.') && (d[-2]=='\\')) { d[-1] = '*'; }
  
  if (fl_is_nt4()) {
	unsigned short * wbuf = (unsigned short*)malloc(sizeof(short) *(len + 10));
	wbuf[fl_utf2unicode(findIn, strlen(findIn), wbuf)] = 0;
	h = FindFirstFileW(wbuf, &findw);
	free(wbuf);
  } else {
    h=FindFirstFile(fl_utf2mbcs(findIn), &find);
  }
  if (h==INVALID_HANDLE_VALUE) {
    free(findIn);
    ret = GetLastError();
    if (ret != ERROR_NO_MORE_FILES) {
      nDir = -1;
    }
    *namelist = dir;
    return nDir;
  }
  do {
	int l;
	if (fl_is_nt4()) {
		l = wcslen(findw.cFileName);
	} else {
		l = strlen(find.cFileName);
	}
    selectDir=(struct dirent*)malloc(sizeof(struct dirent)+l * 5+1);
	if (fl_is_nt4()) {
	  l = fl_unicode2utf(findw.cFileName, l, selectDir->d_name);
	} else {
	  wbuf = (unsigned short*) malloc(sizeof(short) *(l+1));
	  l = mbstowcs(wbuf, find.cFileName, l);
	  l = fl_unicode2utf(wbuf, l, selectDir->d_name);
	  free(wbuf);
    }
	selectDir->d_name[l] = 0;
    if ((fl_is_nt4() &&  findw.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ||
		(!fl_is_nt4() && find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
	{
      // Append a trailing slash to directory names...
      strcat(selectDir->d_name, "/");
    }
    if (!select || (*select)(selectDir)) {
      if (nDir==NDir) {
	struct dirent **tempDir = calloc(sizeof(struct dirent*), NDir+33);
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
  } while ((fl_is_nt4() && FindNextFileW(h, &findw)) ||
	  (!fl_is_nt4() && FindNextFile(h, &find)));
  ret = GetLastError();
  if (ret != ERROR_NO_MORE_FILES) {
    /* don't return an error code, because the dir list may still be valid
       up to this point */ 
  }
  FindClose(h);

  free (findIn);

  if (compar) qsort (dir, nDir, sizeof(*dir),
		     (int(*)(const void*, const void*))compar);

  *namelist = dir;
  return nDir;
}

#endif

/*
 * End of "$Id: scandir_win32.c,v 1.11.2.4.2.5.2.2 2003/11/07 03:47:25 easysw Exp $".
 */
