//
// "$Id: utf.c,v 1.4 2004/06/24 07:05:22 spitzak Exp $"
//
// Copyright 2004 by Bill Spitzak and others.
//
// Unlike the rest of FLTK, these utf8 functions are explicitly
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

#include <fltk/utf.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! Decode a single UTF-8 encoded character starting at \e p. The
    resulting Unicode value (in the range 0-0x7fffffff) is returned,
    and \e len is set the the number of bytes in the UTF-8 encoding
    (adding \e len to \e p will point at the next character).

    If \a p points at an illegal UTF-8 encoding, including one that
    would go past \e end, then *(unsigned char*)p is returned and \e
    len is set to 1. Treating errors this way allows this to decode
    almost any ISO-8859-1 text that has been mistakenly placed where
    UTF-8 is expected, and has proven very useful.

    To distinguish the error result from the legal 1-byte UTF-8
    encodings you must also check *p&0x80.  FLTK is very strict about
    legal UTF-8 encodings, characters encoded in too many bytes are
    considered illegal.

    Calling code can be speeded up by treating *p as an unsigned char
    value. If it is less than 0xc2 then *(unsigned char*)p is returned
    and \e len is set to 1.
*/
unsigned utf8decode(const char* p, const char* end, int* len)
{
  unsigned char c = *(unsigned char*)p;
  if (c < 0xc2) {
  FAIL:
    *len = 1;
    return c;
  }
  if (p+1 >= end || (p[1]&0xc0) != 0x80) goto FAIL;
  if (!(c & 0x20)) {
    *len = 2;
    return
      ((p[0] & 0x1f) << 6) +
      ((p[1] & 0x3f));
  }
  if (p+2 >= end || (p[2]&0xc0) != 0x80) goto FAIL;
  if (!(c & 0x10)) {
    if (c==0xE0 && *(unsigned char*)(p+1) < 0xa0) goto FAIL;
    *len = 3;
    return
      ((p[0] & 0x0f) << 12) +
      ((p[1] & 0x3f) << 6) +
      ((p[2] & 0x3f));
  }
  if (p+3 >= end || (p[3]&0xc0) != 0x80) goto FAIL;
  if (!(c & 0x8)) {
    if (c==0xF0 && *(unsigned char*)(p+1) < 0x90) goto FAIL;
    *len = 4;
    return
      ((p[0] & 0x07) << 18) +
      ((p[1] & 0x3f) << 12) +
      ((p[2] & 0x3f) << 6) +
      ((p[3] & 0x3f));
  }
  if (p+4 >= end || (p[4]&0xc0) != 0x80) goto FAIL;
  if (!(c & 0x4)) {
    if (c==0xF8 && *(unsigned char*)(p+1) < 0x88) goto FAIL;
    *len = 5;
    return
      ((p[0] & 0x03) << 24) +
      ((p[1] & 0x3f) << 18) +
      ((p[2] & 0x3f) << 12) +
      ((p[3] & 0x3f) << 6) +
      ((p[4] & 0x3f));
  }
  if (p+5 >= end || (p[5]&0xc0) != 0x80) goto FAIL;
  if (!(c & 0x2)) {
    if (c==0xFC && *(unsigned char*)(p+1) < 0x84) goto FAIL;
    *len = 6;
    return
      ((p[0] & 0x01) << 30) +
      ((p[1] & 0x3f) << 24) +
      ((p[2] & 0x3f) << 18) +
      ((p[3] & 0x3f) << 12) +
      ((p[4] & 0x3f) << 6) +
      ((p[5] & 0x3f));
  }
  goto FAIL; // 0xFE and 0xFF are illegal
}

/*! Similar to {int len; utf8decode(p,end,&len); return len;}
    but faster because it skips the actual extraction of the
    decoded character, and it returns zero (not 1) for illegal
    bytes. So this can be used to test if \a p points at the
    start of a legal UTF-8 character.
*/
int utf8valid(const char* p, const char* end) {
  int len,i;
  unsigned char c = *(unsigned char*)p;
  if (c < 0x80) return 1;
  if (c < 0xc2) return 0;
  if (p+1 >= end) return 0; // avoid looking at *end
  if (!(c & 0x20)) {
    len = 2;
  } else if (!(c & 0x10)) {
    if (c==0xE0 && *(unsigned char*)(p+1) < 0xa0) return 0;
    len = 3;
  } else if (!(c & 0x8)) {
    if (c==0xF0 && *(unsigned char*)(p+1) < 0x90) return 0;
    len = 4;
  } else if (!(c & 0x4)) {
    if (c==0xF8 && *(unsigned char*)(p+1) < 0x88) return 0;
    len = 5;
  } else if (!(c & 0x2)) {
    if (c==0xFC && *(unsigned char*)(p+1) < 0x84) return 0;
    len = 6;
  } else {
    return 0; // 0xFE and 0xFF are illegal
  }
  // Make sure all the rest of the bytes exist and start with 0x80:
  if (p+len > end) return 0;
  for (i = 1; i < len; i++) if ((p[i]&0xc0) != 0x80) return 0;
  return len;
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
    } else if (c < 0xC2) { // error byte
      p++;
      err = 1;
    } else {
      int len = utf8valid(p,end);
      if (len > 1) {
	ret = len;
	p += len;
      } else if (len) {
	p++;
      } else {
	err = 1;
	p++;
      }
    }
  }
  return err ? -ret : ret;
}

/*! Return the length of a legal UTF-8 encoding that starts with
    this byte. Returns 1 for illegal bytes (anthing less than 0xc2
    and 0xfe and 0xff).

    <i>This function is depreciated. You should examine all the bytes
    in the UTF-8 character for legality, so that raw ISO-8859-1
    characters are less likely to be confused with UTF-8.</i>
*/
int utf8len(char c) {
  if ((unsigned char)c < 0xc2) return 1;
  if (!(c & 0x20)) return 2;
  if (!(c & 0x10)) return 3;
  if (!(c & 0x08)) return 4;
  if (!(c & 0x04)) return 5;
  if (!(c & 0x02)) return 6;
  return 1; // 0xFE and 0xFF are illegal
}

/*! If p points into (not at) a utf8valid() character, return
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
  if (utf8valid(p,end)) return p;
  // search backwards for a 0xc0 starting the character:
  for (a = p; ; --a) {
    if ((a[0]&0xc0)==0xc0) break;
    if (!(a[0]&0x80)) return p;
    if (a <= start) return p;
  }
  a += utf8valid(a,end);
  if (a > p) return a;
  return p;
}

/*! If p points into (not at) a utf8valid() character, return a
  pointer to the character (a value less than p). Otherwise return
  p. This will move p to a point that is the start of a character.

  If you wish to decrement a utf8 pointer, pass p-1 to this.

  \e end is the end of the string and is assummed to be a break
  between characters. It is assummed to be greater than p.

  \e start is the start of the string and is used to limit the
  backwards search for the start of a utf8 character.
*/
const char* utf8back(const char* p, const char* start, const char* end)
{
  const char* a;
  if (utf8valid(p,end)) return p;
  // search backwards for a 0xc0 starting the character:
  for (a = p; ; --a) {
    if ((a[0]&0xc0)==0xc0) break;
    if (!(a[0]&0x80)) return p;
    if (a <= start) return p;
  }
  if (a + utf8valid(a,end) > p) return a;
  return p;
}

/*! Write the UTF-8 encoding of \e ucs into \e buf and return the
    number of bytes written. Up to 6 bytes may be written. If you know
    that \a ucs is less than 0x10000 then at most 3 bytes will be written.
    If you wish to speed this up, remember that anything less than 0x80
    is written as a single byte.

    If ucs has the high (31st) bit set, it is not clear what to do, as there
    is no legal UTF-8 encoding. The current version writes it as though
    the high bit was not set, but this may change in the future.

    Warning: if your input is 16-bit, you may very well have "surrogate
    pairs" in there, especially if this is a buffer of data returned
    by Windows. You should call utf16decode on that buffer. However
    calling this directly will replicate a bug that is in many Windows
    programs and may be necessary for interoperability.
*/
int utf8encode(unsigned ucs, char* buf) {
  if (ucs < 0x000080U) {
    buf[0] = ucs;
    return 1;
  } else if (ucs < 0x000800U) {
    buf[0] = 0xC0 | (ucs >> 6);
    buf[1] = 0x80 | (ucs & 0x3F);
    return 2;
  } else if (ucs < 0x010000U) {
    buf[0] = 0xE0 | (ucs >> 12);
    buf[1] = 0x80 | ((ucs >> 6) & 0x3F);
    buf[2] = 0x80 | (ucs & 0x3F);
    return 3;
  } else if (ucs < 0x00200000U) {
    buf[0] = 0xF0 | (ucs >> 18);
    buf[1] = 0x80 | ((ucs >> 12) & 0x3F);
    buf[2] = 0x80 | ((ucs >> 6) & 0x3F);
    buf[3] = 0x80 | (ucs & 0x3F);
    return 4;
  } else if (ucs < 0x04000000U) {
    buf[0] = 0xF8 | (ucs >> 24);
    buf[1] = 0x80 | ((ucs >> 18) & 0x3F);
    buf[2] = 0x80 | ((ucs >> 12) & 0x3F);
    buf[3] = 0x80 | ((ucs >> 6) & 0x3F);
    buf[4] = 0x80 | (ucs & 0x3F);
    return 5;
  } else {
    buf[0] = (ucs&0x40000000) ? 0xFD : 0xFC;
    buf[1] = 0x80 | ((ucs >> 24) & 0x3F);
    buf[2] = 0x80 | ((ucs >> 18) & 0x3F);
    buf[3] = 0x80 | ((ucs >> 12) & 0x3F);
    buf[4] = 0x80 | ((ucs >> 6) & 0x3F);
    buf[5] = 0x80 | (ucs & 0x3F);
    return 6;
  }
}

/*! Convert a UTF-8 sequence into an array of 32-bit Unicode indexes.

    If the conversion is the identity (ie every byte turns into the
    matching 32-bit value) then null is returned. You should then
    pass the string to the 8-bit interface, or copy it yourself.
    \e count is still set to the length. This appears to be a useful
    optimization, as such strings are very common.

    If conversion is needed, the return value is a pointer to a
    malloc() buffer, and the number of words written to the buffer are
    returned in \e count. The buffer has one more location allocated,
    if you put a zero there you can use it as a terminated string.
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
    unsigned char c = *(unsigned char*)p;
    if (c < 0xC2) p++; // ascii letter or bad code
    else {
      int len = utf8valid(p,e);
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
    unsigned char c = *(unsigned char*)p;
    if (c < 0xC2) { // ascii letter or bad code
      buffer[count] = c;
      p++;
    } else {
      int len;
      buffer[count] = utf8decode(p,e,&len);
      p += len;
    }
    count++;
  }
  return buffer;
}

/*! Convert a UTF-8 sequence into UTF-16 (the encoding used on Windows).

    If the conversion is the identity (ie every byte turns into the
    matching 16-bit value) then null is returned. You should then
    pass the string to the 8-bit interface, or copy it yourself.
    \e count is still set to the length. This appears to be a useful
    optimization, as such strings are very common.

    If conversion is needed, the return value is a pointer to a
    malloc() buffer, and the number of words written to the buffer are
    returned in \e count. The buffer has one more location allocated,
    if you put a zero there you can use it as a terminated string.
    You must call free() or utf8free() on the buffer.
 
    Errors in the UTF-8 are converted as individual bytes, same as
    utf8decode() does. This allows ISO-8859-1 text mistakenly identified
    as UTF-8 to be printed correctly.

    Codes in the range 0x10000 to 0x10ffff are converted to "surrogate
    pairs" which take two words each. Codes greater than 0x10ffff are
    converted to 0xFFFD (REPLACEMENT CHARACTER). Because of this lossage
    we strongly recommend you do all your work in UTF-8.

    Recommended code for calling a Windows inteface function:

    \code
#ifdef _WIN32
  int ucslen;
  unsigned short* ucs = utf8to16(name, strlen(name), &ucslen);
  if (ucs) {
    ucs[ucslen] = 0;
    fp = _wfopen(ucs, L"r");
    utf8free(ucs);
  } else
#endif
    fp = fopen(name, "r");
    \endcode

*/
unsigned short* utf8to16(const char* text, int n, int* charcount) {
  const char* p = text;
  const char* e = text+n;
  int sawutf8 = 0;
  int count = 0;
  unsigned short* buffer;
  while (p < e) {
    unsigned char c = *(unsigned char*)p;
    if (c < 0xC2) p++; // ascii letter or bad code
    else {
      int len = utf8valid(p,e);
      if (len > 1) {
	sawutf8 = 1;
	// detect a code that will turn into a surrogate pair:
	if (len == 4 && *(unsigned char*)(p+1)<0x90) count++;
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
    unsigned char c = *(unsigned char*)p;
    if (c < 0xC2) { // ascii letter or bad code
      buffer[count] = c;
      p++;
    } else {
      int len;
      unsigned ucs = utf8decode(p,e,&len);
      p += len;
      if (ucs < 0x10000U) {
	buffer[count] = (unsigned short)ucs;
      } else if (ucs > 0x10ffffU) {
	// error!
	buffer[count] = 0xFFFD;
      } else {
	// write a surrogate pair:
	buffer[count++] = (((ucs-0x10000u)>>10)&0x3ff) | 0xd800;
	buffer[count] = (ucs&0x3ff) | 0xdc00;
      }
    }
    count++;
  }
  return buffer;
}

/*! Convert a UTF-8 sequence into an array of 1-byte characters.

    If the conversion is the identity (ie every byte is unchanged)
    then null is returned. You should use the original buffer in this
    case.  \e count is still set to the length. This appears to be a
    useful optimization, as such strings are very common.

    If conversion is needed, the return value is a pointer to a
    malloc() buffer, and the number of bytes written to the buffer are
    returned in \e count. The buffer has one more location allocated,
    if you put a zero there you can use it as a terminated string.
    You must call free() or utf8free() on the buffer.

    Errors in the UTF-8 are converted as individual bytes, same as
    utf8decode() does. This allows ISO-8859-1 text mistakenly identified
    as UTF-8 to be printed correctly.

    Any UTF-8 codes greater than 0xff are turned into '?'. Obviously
    this is lossy so you only want to use this if there is no other
    way to send the text.
*/
char* utf8to8(const char* text, int n, int* charcount) {
  const char* p = text;
  const char* e = text+n;
  int sawutf8 = 0;
  int count = 0;
  unsigned char* buffer;
  while (p < e) {
    unsigned char c = *(unsigned char*)p;
    if (c < 0xC2) p++; // ascii letter or bad code
    else {
      int len = utf8valid(p,e);
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
      else buffer[count] = '?';
      p += len;
    }
    count++;
  }
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
    but this way converting to utf8 and back is lossless.
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
