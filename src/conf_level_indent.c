/*
   "$Id: conf_level_indent.c,v 1.5 1999/11/27 00:58:24 carl Exp $"

    Configuration file routines for the Fast Light Tool Kit (FLTK).

    Carl Thompson's config file routines version 0.3
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

/* this function returns the proper amount of leading indentation for a line */
const char *
level_indent(int l)
{
        static char indent[CONF_MAX_LEVEL * CONF_INDENT + 1];

        memset(indent, 0, sizeof(indent));
        if (l <= CONF_MAX_LEVEL) memset(indent, ' ', l * CONF_INDENT);

        return indent;
}

/*
    End of "$Id: conf_level_indent.c,v 1.5 1999/11/27 00:58:24 carl Exp $".
*/
