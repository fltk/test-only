/*
   "$Id: conf_get.c,v 1.7 1999/11/10 04:48:53 carl Exp $"

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
        int getconf(const char *configfile, const char *key, char *svalue,
                    int slen)

        description:
                gets the string associated with a key in a config file
        arguments:
                configfile: path of config file
                key: section/key to look for
                slen: length of passed string buffer
        return values:
                returns 0 for OK or error code defined in conf.h
                svalue: string associated with key
*/
int
getconf(const char *configfile, const char *k, char *svalue, int slen)
{
        FILE    *ifp;                                                           /* file pointer for config file */
        char    section2[CONF_MAX_SECT_LEN + 2];                                /* section with [] added */
        char    line[CONF_MAX_LINE_LEN];                                        /* line buffer */
        char    *p, *p2;                                                        /* miscelaneous char pointers */
        char    keysect[CONF_MAX_SECT_LEN], *key, *section;                     /* key, section, and both */


        if (!configfile || !k || !svalue)
                return CONF_ERR_ARGUMENT;                                       /* NULL pointer was passed */

        strcpy(keysect, k);                                                     /* copy key and section to buffer */
        if ((p = strrchr(keysect, conf_level_sep)))                             /* if a level separator found */
        {
            *p = (char)0;                                                       /* turn into two strings */
            key = p + 1;                                                        /* set key */
            section = keysect;                                                  /* set section */
        }
        else                                                                    /* no level separator found */
        {
            key = keysect;                                                      /* set key */
            section = "";                                                       /* set toplevel section */
        }
        
        ifp = fopen(configfile, "r");
        if (!ifp)
                return CONF_ERR_FILE;                                           /* could not open config file */

        if (*section)                                                           /* if user passed a section name */
        {
                if (strlen(section) > (CONF_MAX_SECT_LEN - 1))                  /* if section name too long */
                        return CONF_ERR_ARGUMENT;                               /* bail out */

                sprintf(section2, "[%s]", section);                             /* add [] to section name */

                while ((p = fgets(line, sizeof(line), ifp)))                    /* keep reading lines until section found */
		{
			if ((p2 = strchr(line, conf_comment_sep)))
				*p2 = (char)0;

                        if (!strcasecmp(trim(line), section2))
		               break;
		}
                if (!p)
                {                                                               /* section was not found */
                        fclose(ifp);
                        return CONF_ERR_SECTION;
                } /* if (!fp) */
        }

        while (fgets(line, sizeof(line), ifp))                                  /* while there are still lines in the file */
        {
                /*
                   hack so that comment seperator can occur in value
                   if not immediately followed by a whitespace
                */
		for (p2 = line; (p = strchr(p2, conf_comment_sep)); p2 = p + 1)
		{
		        if (strchr(CONF_WHITESPACE, *(p + 1)) ||
		            line + strspn(line, CONF_WHITESPACE) == p)
		        {
		                *p = (char)0;
		                break;
		        }
		}
		
                trim(line);                                                     /* remove unnecessary whitespace */
		    
                if (*line == '[')                                               /* if this is a different section */
                {
                        fclose(ifp);
                        return CONF_ERR_KEY;                                    /* key was not found */
                } /* if (*line == '[') */
                if ((p = strchr(line, conf_sep)))                               /* if there is a seperator character */
                        *p++ = '\0';                                            /* p points to value, line is now just key */
                else
                        p = NULL;

                trim(line); trim(p);                                            /* kill unnecessary whitespace */
                if (strcasecmp(line, key))                                      /* this isn't the entry we want */
                        continue;                                               /* go to the next line */

                /* hmmm, this must be the right entry? */
                if (!p)
                        *svalue = (char)0;
                else
                        strncpy(svalue, p, slen);                               /* copy value to svalue */
                svalue[slen - 1] = (char)0;                                     /* make sure string is null terminated */
                fclose(ifp);
                return p ? CONF_SUCCESS : CONF_ERR_NOVALUE;

        } /* while (fgets(line, sizeof(line), ifp)) */

        /* if we got here, then we didn't find the key */
        fclose(ifp);
        return CONF_ERR_KEY;                                                    /* key was not found */
} /* getconf() */

/*
    End of "$Id: conf_get.c,v 1.7 1999/11/10 04:48:53 carl Exp $".
*/
