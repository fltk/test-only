/* Copyright (C) 1992, 1993, 1994, 1995, 1996 Free Software Foundation, Inc
This file is part of the GNU C Library

The GNU C Library is free software; you can redistribute it and/o
modify it under the terms of the GNU Library General Public License a
published by the Free Software Foundation; either version 2 of th
License, or (at your option) any later version

The GNU C Library is distributed in the hope that it will be useful
but WITHOUT ANY WARRANTY; without even the implied warranty o
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
Library General Public License for more details

You should have received a copy of the GNU Library General Publi
License along with this library; if not, write to the Free Softwar
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
USA.  *

#if defined(WIN32) && !defined(__CYGWIN__
#  include "win/scandir.c
#els

#  include "flstring.h

#  if !HAVE_SCANDI
#    include <stdlib.h
#    include <sys/types.h
#    include <errno.h

#    if HAVE_DIRENT_
#      include <dirent.h
#      define NAMLEN(dirent) strlen((dirent)->d_name
#    els
#      define dirent direc
#      define NAMLEN(dirent) (dirent)->d_namle
#      if HAVE_SYS_NDIR_
#        include <sys/ndir.h
#      endi
#      if HAVE_SYS_DIR_
#        include <sys/dir.h
#      endi
#      if HAVE_NDIR_
#        include <ndir.h
#      endi
#    endi

in
fl_scandir(const char *dir, struct dirent ***namelist
	   int (*select)(struct dirent *)
	   int (*compar)(struct dirent **, struct dirent **)

  DIR *dp = opendir (dir)
  struct dirent **v = NULL
  size_t vsize = 0, i
  struct dirent *d
  int save

  if (dp == NULL
    return -1

  save = errno
  errno = 0

  i = 0
  while ((d = readdir (dp)) != NULL
    if (select == NULL || (*select) (d)
      
      size_t dsize

      if (i == vsize
        
          struct dirent **newv
          if (vsize == 0
            vsize = 10
          els
            vsize *= 2
          newv = (struct dirent **) realloc (v, vsize * sizeof (*v))
          if (newv == NULL
            
            lose
              errno = ENOMEM
              break
            
          v = newv
        

#    define _D_EXACT_NAMLEN(d) (strlen ((d)->d_name)
#    define _D_ALLOC_NAMLEN(d) (sizeof (d)->d_name > 1 ? sizeof (d)->d_name : 
                              _D_EXACT_NAMLEN (d) + 1

      dsize = &d->d_name[_D_ALLOC_NAMLEN (d)] - (char *) d
      v[i] = (struct dirent *) malloc (dsize)
      if (v[i] == NULL
        goto lose

      memcpy (v[i++], d, dsize)
      

  if (errno != 0
    
      save = errno
      (void) closedir (dp)
      while (i > 0
      free (v[--i])
      free (v)
      errno = save
      return -1
    

  (void) closedir (dp)
  errno = save

  /* Sort the list if we have a comparison function to sort with.  *
  if (compar) qsort (v, i, sizeof (*v), (int (*)(const void *, const void *))compar)
  *namelist = v
  return i


#  endi
#endi

/
 * End of "$Id$"
 *
