/* Copyright (c) 1990,1991,1992,1993 Chris and John Downey */

/***

* program name:
    xvi
* function:
    Portable version of UNIX "vi" editor, with extensions.
* module name:
    mark.c
* module function:
    Routines to maintain and manipulate marks.
* history:
    STEVIE - ST Editor for VI Enthusiasts, Version 3.10
    Originally by Tim Thompson (twitch!tjt)
    Extensive modifications by Tony Andrews (onecom!wldrdg!tony)
    Heavily modified by Chris & John Downey
    Last modified by Martin Guy

***/

#include "xvi.h"

#ifdef	MEGAMAX
overlay "mark"
#endif

/*
 * A new buffer - initialise it so there are no marks.
 */
void
init_marks(buffer)
Buffer	*buffer;
{
    Mark	*mlist = buffer->b_mlist;
    int	i;

    /* Set all m_line to NULL */
    memset(mlist, sizeof(*mlist) * NMARKS, 0);
    buffer->b_pcmark.m_line = NULL;
    buffer->b_pcmark.m_deleted = FALSE;
}

/*
 * setmark(c) - set mark 'c' at the specified line.
 *
 * Returns TRUE on success, FALSE if no room for mark or bad name given.
 */
bool_t
setmark(c, buffer, line)
int	c;
Buffer	*buffer;
Line	*line;
{
    if (c == '\'' || c == '`') {
	buffer->b_pcmark.m_line = line;
	return(TRUE);
    }
    if (is_lower(c)) {
	Mark *m = &(buffer->b_mlist[c - 'a']);
	m->m_line = line;
	m->m_deleted = FALSE;
	return(TRUE);
    }
    return(FALSE);
}

/*
 * setpcmark() - set the previous context mark to the current position
 */
void
setpcmark()
{
    curbuf->b_pcmark.m_line = curwin->w_cursor->p_line;
}

/*
 * getmark(c) - find mark for char 'c' in given buffer
 *
 * Return pointer to Line or NULL if no such mark.
 */
Line *
getmark(c, buffer)
int	c;
Buffer	*buffer;
{
    Mark *m;

    if (c == '\'' || c == '`') {
	m = &(buffer->b_pcmark);
    } else if (is_lower(c)) {
	m = &(buffer->b_mlist[c - 'a']);
	if (m->m_deleted) return(NULL);
    } else {
	return(NULL);
    }
    return(m->m_line);
}

/*
 * clrmark(line) - clear any marks for 'line'
 *
 * Used any time a line is deleted.
 */
void
clrmark(line, buffer)
Line	*line;
Buffer	*buffer;
{
    Mark	*mlist = buffer->b_mlist;
    int		 i;

    for (i = 0; i < NMARKS; i++) {
	if (mlist[i].m_line == line) {
	    mlist[i].m_deleted = TRUE;
	}
    }
    if (buffer->b_pcmark.m_line == line) {
	buffer->b_pcmark.m_line = NULL;
    }
}

/*
 * A line has just been undeleted. See if any marks referred to it
 * and restore them if so.
 */
void
restoremarks(line, buffer)
Line *line;
Buffer *buffer;
{
    Mark	*mlist = buffer->b_mlist;
    int		 i;

    for (i = 0; i < NMARKS; i++) {
	if (mlist[i].m_line == line) {
	    mlist[i].m_deleted = FALSE;
	}
    }
}
