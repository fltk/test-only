/*
    Carl Thompson's config file routines version 0.20
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

/*
        long conf_atol(const char *decstr)

        description:
                this function coverts the given decimal string to a long int
                on illegally formatted input, returns zero
        arguments:
                decstr: decimal string to be converted
        return value:
                returns the long integer represented by the decimal string
*/
long
conf_atol(const char *decstr)
{
        int     sign = 1;                                                       /* sign multiplyer of this number */
        int     r;                                                              /* intermediate result- val. of rightmost digit */
        char    dec[20];                                                        /* copy of this number string */
        char    *p = (char *)decstr;                                            /* pointer to the input number string */
        char    c;                                                              /* rightmost char in number string */

        if (!decstr)                                                            /* if null pointer passed */
                return 0L;                                                      /* return zero */

        if (*decstr == '-') {                                                   /* if this is a negative number */
                sign = -1;                                                      /* set multiplyer to -1 */
                p++;                                                            /* go to actual start of number */
        }

        strncpy(dec, p, 19);                                                    /* make copy of string */
        dec[19] = '\0';                                                         /* make sure number is NULL terminated */

        if (!strlen(dec))                                                       /* if empty string passed */
                return 0L;                                                      /* return zero */

        if (strspn(dec, "0123456789") != strlen(dec))                           /* if string has other than digits passed */
                return 0L;                                                      /* return zero */

        c = dec[strlen(dec) - 1];                                               /* char c is last digit in number */
        dec[strlen(dec) - 1] = '\0';                                            /* remove last digit from number */

        r = c - '0';                                                            /* r is the value of the last digit in number */

        return (10 * conf_atol(dec) + r) * sign;                                /* recursive funtion call to compute result */
}
