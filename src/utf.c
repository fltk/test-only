//
// "$Id: utf.c,v 1.8 2004/07/18 03:23:27 spitzak Exp $"
//
// Copyright 2004 by Bill Spitzak and others.
//
// Unlike the rest of FLTK, these UTF-8 functions are explicitly
// released into the public domain, with no restrictions on copying
// or reuse for any purpose, in open or closed-source software.
// These functions are completely original, written by Bill Spitzak
// from the UTF-8 RFC documents, and have no legal encumberances.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

// Modified to obey rfc3629, which limits unicode to 0-0x10ffff

#include <fltk/utf.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// Codes 0x80..0x9f from the Microsoft CP1252 character set, translated
// to Unicode:
static unsigned short cp1252[32] = {
  0x20ac, 0x0081, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
  0x02c6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008d, 0x017d, 0x008f,
  0x0090, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
  0x02dc, 0x2122, 0x0161, 0x203a, 0x0153, 0x009d, 0x017e, 0x0178
};

/*! Decode a single UTF-8 encoded character starting at \e p. The
    resulting Unicode value (in the range 0-0x10ffff) is returned,
    and \e len is set the the number of bytes in the UTF-8 encoding
    (adding \e len to \e p will point at the next character).

    If \a p points at an illegal UTF-8 encoding, including one that
    would go past \e end, or where a code is uses more bytes than
    necessary, then *(unsigned char*)p is translated as though it is
    in the Microsoft CP1252 character set and \e len is set to 1.
    Treating errors this way allows this to decode almost any
    ISO-8859-1 or CP1252 text that has been mistakenly placed where
    UTF-8 is expected, and has proven very useful.

    To distinguish the error result from the legal 1-byte UTF-8
    encodings you must also check *p&0x80.

    Calling code can be speeded up by checking the high bit and
    directly treating the common 1-byte case:

\code
    if (!(*p&0x80)) {
      code = *p;
      len = 1;
    } else {
      code = utf8decode(p,end,&len);
    }
\endcode

    If you want to test if \a p points at the start of a legal utf-8
    character, the following code will work:

\code
    if (!(*p&0x80)) {
      legal = true;
    } else {
      int len; utf8decode(p,end,&len);
      legal = len > 1;
    }
\endcode

    It is also useful to know that this will always set \a len to 1
    if *p is not in the range 0xc2 through 0xf4.
*/
unsigned utf8decode(const char* p, const char* end, int* len)
{
  unsigned char c = *(unsigned char*)p;
  if (c < 0x80) {
    *len = 1;
    return c;
  } else if (c < 0xa0) {
    *len = 1;
    return cp1252[c-0x80];
  } else if (c < 0xc2) {
    *len = 1;
    return c;
  }
  if (p+1 >= end || (p[1]&0xc0) != 0x80) goto FAIL;
  if (c < 0xe0) {
    *len = 2;
    return
      ((p[0] & 0x1f) << 6) +
      ((p[1] & 0x3f));
  } else if (c == 0xe0) {
    if (((unsigned char*)p)[1] < 0xa0) goto FAIL;
    goto UTF8_3;
#if 0
  } else if (c == 0xed) {
    // RFC 3629 says surrogate chars are illegal, this skips them.
    // I have disabled this so that utf8encode/utf8decode of 16-bit
    // values is the identity
    if (((unsigned char*)p)[1] >= 0xa0) goto FAIL;
    goto UTF8_3;
  } else if (c == 0xef) {
    // Supposedly 0xfffe and 0xffff are also illegal characters
    // I have disabled this test for the same reason
    if (((unsigned char*)p)[1]==0xbf &&
	((unsigned char*)p)[2]>=0xbe) goto FAIL;
    goto UTF8_3;
#endif
  } else if (c < 0xf0) {
  UTF8_3:
    if (p+2 >= end || (p[2]&0xc0) != 0x80) goto FAIL;
    *len = 3;
    return
      ((p[0] & 0x0f) << 12) +
      ((p[1] & 0x3f) << 6) +
      ((p[2] & 0x3f));
  } else if (c == 0xf0) {
    if (((unsigned char*)p)[1] < 0x90) goto FAIL;
    goto UTF8_4;
  } else if (c == 0xf4) {
    if (((unsigned char*)p)[1] > 0x8f) goto FAIL; // after 0x10ffff
    goto UTF8_4;
  } else if (c < 0xf5) {
  UTF8_4:
    if (p+3 >= end || (p[2]&0xc0) != 0x80 || (p[3]&0xc0) != 0x80) goto FAIL;
    *len = 4;
#if 0
    // supposedly xxfffe and xxffff are illegal:
    if ((p[1]&0xf)==0xf &&
	((unsigned char*)p)[2] == 0xbf &&
	((unsigned char*)p)[3] >= 0xbe) goto FAIL;
#endif
    return
      ((p[0] & 0x07) << 18) +
      ((p[1] & 0x3f) << 12) +
      ((p[2] & 0x3f) << 6) +
      ((p[3] & 0x3f));
  } else {
  FAIL:
    *len = 1;
    return c;
  }
}

/*! Return non-zero if the characters between \a p and \a end
    appear to be UTF-8 encoded. This will return non-zero if
    at least one legal UTF-8 encoded character is found.

    If at least one legal UTF-8 character is found, but also
    some errorneous UTF-8 is found, this will return a negative
    number. So is_utf8()>0 will return true for legal-only UTF-8.

    Most of FLTK will work with strings no matter what this returns,
    treating illegal UTF-8 sequences as individual bytes of ISO-8859-1
    encoded text. However it may be useful to use this test before
    passing a string to a system interface that either barfs on
    illegal UTF-8 or is is much slower than the byte interface.
*/
int is_utf8(const char* p, const char* end) {
  int ret = 0;
  int err = 0;
  while (p < end) {
    unsigned char c = *(unsigned char*)p;
    if (c < 0x80) {
      p++;
    } else if (c < 0xC2 || c > 0xf4) { // error byte
      p++;
      err = 1;
    } else {
      int len; utf8decode(p,end,&len);
      if (len > 1) ret = len; else err = 1;
      p += len;
    }
  }
  return err ? -ret : ret;
}

/*! Return the length of a legal UTF-8 encoding that starts with
    this byte. Returns 1 for illegal bytes (0xc0, 0xc1, 0xf5 to 0xff).

    <i>This function is depreciated. You should examine all the bytes
    in the UTF-8 character for legality, so that raw ISO-8859-1
    characters are less likely to be confused with UTF-8.</i>
*/
int utf8len(char cc) {
  unsigned char c = (unsigned char)cc;
  if (c < 0xc2) return 1;
  else if (c < 0xe0) return 2;
  else if (c < 0xf0) return 3;
  else if (c < 0xf5) return 4;
  else return 1;
}

/*! If p points into (not at) a utf-8 character, return
  a pointer to after the character. Otherwise return p. This will
  move p to a point that is at the start of a glyph.

  \e end is the end of the string and is assummed to be a break
  between characters. It is assummed to be greater than p.

  \e start is the start of the string and is used to limit the
  backwards search for the start of a utf8 character.

  If you wish to increment a random pointer into a utf8 string, pass
  p+1 to this. If you wish to step through a string starting at a
  known legal starting point you can do this somewhat faster code:

  \code
  for (const char* p = start; p < end;) {
    if (*(unsigned char*)p < 0xc2) {
      // fast handler for single-byte utf-8 character, or an error byte
      thecode = *(unsigned char*)p;
      ++p;
    } else {
      int len;
      thecode = utf8decode(p, end, &len);
      p += len;
    }
  }
  \endcode
*/
const char* utf8fwd(const char* p, const char* start, const char* end)
{
  const char* a;
  int len;
  // if we are not pointing at a continuation character, we are done:
  if ((*p&0xc0) != 0x80) return p;
  // search backwards for a 0xc0 starting the character:
  for (a = p-1; ; --a) {
    if (a < start) return p;
    if (!(a[0]&0x80)) return p;
    if ((a[0]&0x40)) break;
  }
  utf8decode(a,end,&len);
  a += len;
  if (a > p) return a;
  return p;
}

/*! If p points into (not at) a legal UTF-8 character, return a
  pointer to the character (a value less than p). Otherwise return
  p. This will move p to a point that is the start of a character.

  If you wish to decrement a UTF-8 pointer, pass p-1 to this.

  \e end is the end of the string and is assummed to be a break
  between characters. It is assummed to be greater than p.

  \e start is the start of the string and is used to limit the
  backwards search for the start of a UTF-8 character.
*/
const char* utf8back(const char* p, const char* start, const char* end)
{
  const char* a;
  int len;
  // if we are not pointing at a continuation character, we are done:
  if ((*p&0xc0) != 0x80) return p;
  // search backwards for a 0xc0 starting the character:
  for (a = p-1; ; --a) {
    if (a < start) return p;
    if (!(a[0]&0x80)) return p;
    if ((a[0]&0x40)) break;
  }
  utf8decode(a,end,&len);
  if (a+len > p) return a;
  return p;
}

/*! Returns number of bytes that utf8encode() will use to encode the
  Unicode point \a ucs. */
int utf8bytes(unsigned ucs) {
  if (ucs < 0x000080U) {
    return 1;
  } else if (ucs < 0x000800U) {
    return 2;
  } else if (ucs < 0x010000U) {
    return 3;
  } else if (ucs < 0x10ffffU) {
    return 4;
  } else {
    return 3; // length of the illegal character encoding
  }
}

/*! Write the UTF-8 encoding of \e ucs into \e buf and return the
    number of bytes written. Up to 6 bytes may be written. If you know
    that \a ucs is less than 0x10000 then at most 3 bytes will be written.
    If you wish to speed this up, remember that anything less than 0x80
    is written as a single byte.

    If ucs is greater than 0x10ffff this is an illegal character
    according to RFC 3629. These are converted as though they are
    0xFFFD (REPLACEMENT CHARACTER).

    If \a ucs is in the range 0xd800 to 0xdfff are "surrogate pairs"
    and are supposed to be illegal in UTF-8. However I encode these
    in the standard way, so that utf8decode will return the original
    value unchanged.
*/
int utf8encode(unsigned ucs, char* buf) {
  if (ucs < 0x000080U) {
    buf[0] = ucs;
    return 1;
  } else if (ucs < 0x000800U) {
    buf[0] = 0xc0 | (ucs >> 6);
    buf[1] = 0x80 | (ucs & 0x3F);
    return 2;
  } else if (ucs < 0x010000U) {
    buf[0] = 0xe0 | (ucs >> 12);
    buf[1] = 0x80 | ((ucs >> 6) & 0x3F);
    buf[2] = 0x80 | (ucs & 0x3F);
    return 3;
  } else if (ucs < 0x0010ffffU) {
    buf[0] = 0xf0 | (ucs >> 18);
    buf[1] = 0x80 | ((ucs >> 12) & 0x3F);
    buf[2] = 0x80 | ((ucs >> 6) & 0x3F);
    buf[3] = 0x80 | (ucs & 0x3F);
    return 4;
  } else {
    // encode 0xfffd:
    buf[0] = 0xef;
    buf[1] = 0xbf;
    buf[2] = 0xbd;
    return 3;
  }
}

/*! Convert a UTF-8 sequence into an array of 32-bit Unicode indexes.

    <i>If the conversion is the identity (ie every byte turns into the
    matching 32-bit value) then null is returned</i>. You should then
    pass the string to the 8-bit interface, or copy it yourself.
    \e count is still set to the length. This appears to be a useful
    optimization, as such strings are very common.

    If conversion is needed, the return value is a pointer to a
    malloc() buffer, and the number of words written to the buffer are
    returned in \e count. The buffer has one more location allocated
    and a zero is put there so you can use it as a terminated string.
    You must call free() or utf8free() on the buffer.

    Errors in the UTF-8 are converted as individual bytes, same as
    utf8decode() does. This allows ISO-8859-1 text mistakenly identified
    as UTF-8 to be printed correctly.
*/
unsigned* utf8to32(const char* text, int n, int* charcount) {
  const char* p = text;
  const char* e = text+n;
  char sawutf8 = 0;
  int count = 0;
  unsigned* buffer;
  while (p < e) {
    if (*(unsigned char*)p < 0x80) p++; // ascii
    else if (*(unsigned char*)p < 0xa0) {sawutf8 = 1; p++;} //cp1252
    else if (*(unsigned char*)p < 0xC2) p++; // other bad code
    else {
      int len; utf8decode(p,e,&len);
      if (len > 1) sawutf8 = 1;
      else if (!len) len = 1;
      p += len;
    }
    count++;
  }
  *charcount = count;
  if (!sawutf8) return 0;
  buffer = (unsigned*)(malloc((count+1)*sizeof(unsigned)));
  count = 0;
  p = text;
  while (p < e) {
    if (!(*p & 0x80)) { // ascii
      buffer[count] = *p++;
    } else {
      int len;
      buffer[count] = utf8decode(p,e,&len);
      p += len;
    }
    count++;
  }
  return buffer;
}

/*! Convert a UTF-8 sequence into UTF-16 (the encoding used on Windows
    for the "w" versions of calls).

    <i>If the conversion is the identity (ie every byte turns into the
    matching 16-bit value) then null is returned</i>. You should then
    pass the string to the 8-bit interface, or copy it yourself.
    \e count is still set to the length. This appears to be a useful
    optimization.

    If conversion is needed, the return value is a pointer to a
    malloc() buffer, and the number of words written to the buffer are
    returned in \e count. The buffer has one more location allocated
    and a zero is put there so it is a null-terminated string.
    You must call free() or utf8free() on the buffer.
 
    Errors in the UTF-8 are converted as individual bytes, same as
    utf8decode() does. This allows ISO-8859-1 text mistakenly identified
    as UTF-8 to be printed correctly.

    Codes in the range 0x10000 to 0x10ffff are converted to "surrogate
    pairs" which take two words each. Because of this lossage we
    strongly recommend you do all your work in UTF-8.

    Recommended code for calling a Windows interface function:

    \code
#ifdef _WIN32
  int ucslen;
  unsigned short* ucs = utf8to16(name, strlen(name), &ucslen);
  if (ucs) {
    fp = _wfopen(ucs, L"r");
    utf8free(ucs);
  } else
#endif
    fp = fopen(name, "r");
    \endcode

    The return type is not wchar_t, due to the unfortunate decision on
    Unix to define that as being bigger than 16 bits. This apparently
    requires you to cast the results on Windows CE?  */
unsigned short* utf8to16(const char* text, int n, int* charcount) {
  const char* p = text;
  const char* e = text+n;
  int sawutf8 = 0;
  int count = 0;
  unsigned short* buffer;
  while (p < e) {
    if (*(unsigned char*)p < 0x80) p++; // ascii
    else if (*(unsigned char*)p < 0xa0) {sawutf8 = 1; p++;} //cp1252
    else if (*(unsigned char*)p < 0xC2) p++; // other bad code
    else {
      int len; utf8decode(p,e,&len);
      if (len > 1) {
	sawutf8 = 1;
	// detect a code that will turn into a surrogate pair:
	if (len == 4) count++;
      } else if (!len)
	len = 1;
      p += len;
    }
    count++;
  }
  *charcount = count;
  if (!sawutf8) return 0;
  buffer = (unsigned short*)(malloc((count+1)*sizeof(unsigned short)));
  count = 0;
  p = text;
  while (p < e) {
    if (!(*p & 0x80)) { // ascii
      buffer[count] = *p++;
    } else {
      int len;
      unsigned ucs = utf8decode(p,e,&len);
      p += len;
      if (ucs < 0x10000U) {
	buffer[count] = (unsigned short)ucs;
      } else {
	// write a surrogate pair:
	buffer[count++] = (((ucs-0x10000u)>>10)&0x3ff) | 0xd800;
	buffer[count] = (ucs&0x3ff) | 0xdc00;
      }
    }
    count++;
  }
  buffer[count] = 0;
  return buffer;
}

/*! Convert a UTF-8 sequence into an array of 1-byte characters.

    If the conversion is the identity (ie every byte is unchanged)
    then null is returned. You should use the original buffer in this
    case.  \e count is still set to the length. This appears to be a
    useful optimization, as such strings are very common.

    If conversion is needed, the return value is a pointer to a
    malloc() buffer, and the number of bytes written to the buffer are
    returned in \e count. The buffer has one more location allocated
    and a null is put there so you can use it as a terminated string.
    You must call free() or utf8free() on the buffer.

    Errors in the UTF-8 are converted as individual bytes, same as
    utf8decode() does. This allows ISO-8859-1 text mistakenly identified
    as UTF-8 to be printed correctly. The cp1512 emulation is not done,
    unexpected codes in the range 0x80-0x9f are returned unchanged.

    If the UTF-8 decodes to a character greater than 0xff then it is
    replaced with '¿' (0xbf).
*/
char* utf8to8(const char* text, int n, int* charcount) {
  const char* p = text;
  const char* e = text+n;
  int sawutf8 = 0;
  int count = 0;
  char* buffer;
  while (p < e) {
    unsigned char c = *(unsigned char*)p;
    if (c < 0xC2) p++; // ascii letter or bad code
    else {
      int len; utf8decode(p,e,&len);
      if (len > 1) sawutf8 = 1;
      else if (!len) len = 1;
      p += len;
    }
    count++;
  }
  *charcount = count;
  if (!sawutf8) return 0;
  buffer = (char*)(malloc(count+1));
  count = 0;
  p = text;
  while (p < e) {
    unsigned char c = *(unsigned char*)p;
    if (c < 0xC2) { // ascii letter or bad code
      buffer[count] = c;
      p++;
    } else {
      int len;
      unsigned ucs = utf8decode(p,e,&len);
      if (ucs < 0x100) buffer[count] = ucs;
      else buffer[count] = 0xbf;
      p += len;
    }
    count++;
  }
  buffer[count] = 0;
  return buffer;
}

/*! Turn UTF-16 (such as Windows uses for "wide characters") into UTF-8.

    The return value is a pointer to a malloc() buffer, and the number
    of bytes written to the buffer are returned in \e count. The
    buffer has one more location allocated, if you put a zero there
    you can use it as a terminated string.  You must call free() or
    utf8free() on the buffer.

    Unmatched halves of "surrogate pairs" are converted directly to
    the same Unicode value. This is supposedly the wrong thing to do,
    but this way converting to UTF-8 and back is lossless.
*/
char* utf8from16(const unsigned short* wtext, int n, int* bytes) {
  int count = 0;
  int i;
  char* buffer;
  for (i = 0; i < n; i++) {
    unsigned short ucs = wtext[i];
    if (ucs < 0x80U) {
      count++;
    } else if (ucs < 0x800U) {
      count += 2;
    } else {
      count += 3;
      // detect and skip a surrogate pair:
      if (ucs >= 0xd800 && ucs <= 0xdbff && i < n-1 &&
	  wtext[i+1] >= 0xdc00 && wtext[i+1] <= 0xdfff) {
	count++; // all surrogate pairs turn into 4-byte utf8
	i++; // skip second half of surrogate pair
      }
    }
  }
  *bytes = count;
  buffer = (char*)(malloc(count+1));
  count = 0;
  for (i = 0; i < n; i++) {
    unsigned ucs = wtext[i];
    if (ucs < 0x80U) {
      buffer[count++] = (char)(ucs);
    } else {
      // detect and decode a surrogate pair:
      if (ucs >= 0xd800 && ucs <= 0xdbff && i < n-1 &&
	  wtext[i+1] >= 0xdc00 && wtext[i+1] <= 0xdfff) {
	unsigned ucs2 = wtext[++i];
	ucs = 0x10000U + ((ucs&0x3ff)<<10) + (ucs2&0x3ff);
      }
      count += utf8encode(ucs, buffer+count);
    }
  }
  return buffer;
}

#if 0 // These are nyi...

/*! Convert the local multi-byte encoding to UTF-8. A multi-byte
    encoding depends on the "locale" and may be used to store text
    data in files or in network communications, and filenames. This
    is incresaingly obsolete, you should check if raw UTF-8 can be
    used!

    If a conversion is done, a pointer is returned to a malloc'd
    and null-terminated buffer of the resulting UTF-8 and \a bytes
    is set to the length of it (not counting the null).

    In many cases the multi-byte text is the same as the UTF-8.
    In this case NULL is returned, and you should use the original
    buffer. \a bytes is set to strlen(mb).
*/
char* utf8frommb(const char *mb, int* bytes) {
#ifdef _WIN32
  // ??
#else
  // Assumme that if no high bit is set, it is done. This is
  // wrong for some mb encodings like UTF-7, but I don't think
  // that is a problem:
  int n = 0;
  for (;;n++) {
    if (!mb[n]) {bytes = n; return;}
    if (mb[n]&0x80) break;
  }
  // get how many characters there are, allocate buffer, and convert:
  int n = mbstowcs(0, mb, 0);
  wchar_t* ucs = (wchar_t*)malloc((n+1)*sizeof(wchar_t));
  n = mbstowcs(ucs, mb, n+1);
  char* utf = utf8from32(ucs, n, bytes);
  free((void*)ucs);
  return utf;
#endif
}

/*! Convert null-terminated utf8 *to* the mb encoding used by locale.
  Used to turn a filename shown to the user in utf8 into a filename
  to open (on Windows at least, not clear if you want this on Unix)
*/
char* utf8tomb(const char* src, int* bytes) {
  NYI;
}
#endif

/*! This is a wrapper for free(p). It is provided so you can free the
    buffers returned by the utf8 converter functions without including
    stdlib.h.
*/
void utf8free(void* p) {
  free(p);
}

#ifdef __cplusplus
}
#endif
