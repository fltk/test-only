/*
   "$Id: conf_strcasecmp.c,v 1.3 1999/08/11 10:20:30 carl Exp $"

    Configuration file routines for the Fast Light Tool Kit (FLTK).

    Carl Thompson's config file routines version 0.21
    Copyright 1995-1999 Carl Everard Thompson (clip@home.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
    USA.
*/

#include <FL/conf.h>
#include <config.h>

/*
        long conf_strcascmp(const char *s1, const char *s2)

        description:
                this function compares the strings s1 and s2 ignoring case
        arguments:
		s1: string 1
		s2: string 2
        return value:
                returns 0 if the strings are equal, a negative integer if
		s1 < s2, or a positive interger if s1 > s2.
*/
int
conf_strcasecmp(const char *s1, const char *s2)
{
	for (; *s1 && *s2 && tolower(*s1) == tolower(*s2); s1++, s2++) ;        /* loop while chars equal & neither string ended */
	if (!(*s1) && !(*s2)) return 0;                                         /* if both strings ended must be equal */
	return *s1 - *s2;                                                       /* must be 1st different char, return comparison */
}

/*
    End of "$Id: conf_strcasecmp.c,v 1.3 1999/08/11 10:20:30 carl Exp $".
*/
