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
#include <config.h>

/*
        int getconf_sections(const char *configfile, const char *section,
                             conf_list *list)

        description:
                gets the list of subsections that are in the specified section
        arguments:
                configfile: path of config file
                section: section in config file (must be unique in file)
                list: pointer to a conf_list (same as pointer to pointer
                      to conf_entry) of subsections
        return values:
                returns 0 for OK or error code defined in conf.h
                returns a pointer to first in list of entries in "list"
*/
int
getconf_sections(const char *configfile, const char *section, conf_list *list)
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

        if (section && *section)                                                /* if we are not looking in top-level section */
        {
                sprintf(section2, "[%s]", section);                             /* add [] to section name */

                while ((p = fgets(line, sizeof(line), ifp)))                    /* keep reading lines until section found */
		{
			if ((p2 = strchr(line, conf_comment_sep)))              /* if there is a comment */
				*p2 = (char)0;					/* kill it */

            		if (!strcasecmp(trim(line), section2))			/* if this is the section we are loooking for */
		               break;						/* stop looking */
		}
		
                if (!p)
                {                                                               /* section was not found */
                        fclose(ifp);
                        return CONF_ERR_SECTION;
                } /* if (!fp) */

                sprintf(section2, "[%s%c", section, conf_level_sep);            /* beginning of subsection name */
        }
        else
        {
                strcpy(section2, "[");                                          /* beginning of subsection name */
        }


        while (fgets(line, sizeof(line), ifp))                                  /* while there are still lines in the file */
        {
		if ((p2 = strchr(line, conf_comment_sep)))
			*p2 = (char)0;
		
                trim(line);                                                     /* remove unnecessary whitespace */
		
                if (*line != '[')                                               /* if this is not a section name */
                        continue;                                               /* go to the next line */

                if (strncmp(line, section2, strlen(section2)))                  /* if this is a different section */
                        break;                                                  /* we are done */

                if ((p = strrchr(line, ']')))                                   /* find trailing ']' in section name */
                        *p = (char)0;                                           /* remove it */

                p = line + strlen(section2);                                    /* first character of subsection name */

                if (strchr(p, conf_level_sep))                                  /* if this is a sub-subsection */
                        continue;                                               /* go to next line */

                (*current) = (conf_entry *)malloc(sizeof(conf_entry));          /* allocate memory for this entry */
                if (*current)
                        (*current)->data = strdup(p);                           /* duplicate subsection name for this entry */
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
} /* getconf_sections() */

