/*
   "$Id: conf_keys.c,v 1.5 1999/11/10 04:48:53 carl Exp $"

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
        int getconf_keys(const char *configfile, const char *section,
                         conf_list *list)

        description:
                gets the list of keys that are in the specified section
        arguments:
                configfile: path of config file
                section: section in config file (must be unique in file)
                list: pointer to a conf_list (same as pointer to pointer
                      to conf_entry) of keys
        return values:
                returns 0 for OK or error code defined in conf.h
                returns a pointer to first in list of entries in "list"
*/
int
getconf_keys(const char *configfile, const char *section, conf_list *list)
{
        FILE        *ifp;                                                       /* file pointer for config file */
        char        section2[CONF_MAX_SECT_LEN + 2];                            /* section with [] added */
        char        line[CONF_MAX_LINE_LEN];                                    /* line buffer */
        char        *p, *p2;                                                    /* miscelaneous char pointers */
        conf_entry  **current;                                                  /* pointer to pointer to current entry */

        if (!configfile || !list)
                return CONF_ERR_ARGUMENT;                                       /* NULL pointer was passed */

        conf_list_free(list);                                                   /* free list passed to us */

        current = list;                                                         /* pointer to current (first) entry */

        ifp = fopen(configfile, "r");
        if (!ifp)
                return CONF_ERR_FILE;                                           /* could not open config file */

        if (section && *section)                                                /* if user passed a section name */
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

                if (*line == (char)0)                                            /* if this is a blank line */
                        continue;                                               /* go to the next line */

                if (*line == '[')                                               /* if this is a different section */
                        break;                                                  /* we are done */

                if ((p = strchr(line, conf_sep)))                               /* if this key has a value associated */
                {
                        *p = (char)0;                                           /* get rid of it */
                        trim(line);                                             /* remove unnecessary whitespace */
                }

                (*current) = (conf_entry *)malloc(sizeof(conf_entry));          /* allocate memory for this entry */
                if (*current)
                        (*current)->data = strdup(line);                        /* duplicate the key for this entry */
                if ((*current == NULL) || ((*current)->data == NULL))           /* if we had a memory allocation problem */
                {
                        fclose(ifp);                                            /* close data file */
                        conf_list_free(list);                                   /* free what we did already */
                        return CONF_ERR_MEMORY;                                 /* and bail out */
                }
                 (*current)->next = NULL;                                        /* set this entry's next pointer to null */
                current = &((*current)->next);                                  /* point to next entry */
        } /* while (fgets(line, sizeof(line), ifp)) */

        /* all done! */
        fclose(ifp);
        return CONF_SUCCESS;                                                    /* key was not found */
} /* getconf_keys() */

/*
    End of "$Id: conf_keys.c,v 1.5 1999/11/10 04:48:53 carl Exp $".
*/
