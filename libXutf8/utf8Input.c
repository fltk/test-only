/******************************************************************************
              Copyright (c) 2000-2002 by O'ksi'D
 *                      All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *      Neither the name of O'ksi'D nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER 
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *

******************************************************************************/

#include "Xutf8.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>


int
XUtf8LookupString(
    XIC                 ic,
    XKeyPressedEvent*   event,
    char*               buffer_return,
    int                 bytes_buffer,
    KeySym*             keysym,
    Status*             status_return)
{
#if HAVE_X11_UTF8
	return Xutf8LookupString(ic, event, buffer_return, bytes_buffer,
		keysym, status_return);
#else 
        long ucs = -1;
        int len;
        len = XmbLookupString(ic, event, buffer_return, bytes_buffer / 5,
                         keysym, status_return);
	if (*status_return == XBufferOverflow) {
		return len * 5;
	}
	if (*keysym > 0 && *keysym < 0x100) {
		ucs = *keysym;
	} else  if (((*keysym >= 0x100 && *keysym <= 0xf000) ||
                              (*keysym & 0xff000000U) == 0x01000000))
        {
                ucs = XKeysymToUcs(*keysym);
        } else {
                ucs = -2;
        }
        //if (ucs == -1) len = 0;
        if (ucs > 0) {
                len = XConvertUcsToUtf8(ucs, (char *)buffer_return);
#  if 0 // ONLY SUPPORT Unicode + UTF-8!
        } else if (len > 0) {
		len = XConvertEucToUtf8(XLocaleOfIM(XIMOfIC(ic)), 
			buffer_return, len, bytes_buffer);	
#  endif // 0
	}
        return len;
#endif
}


