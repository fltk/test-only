//
// "$Id: utf.c,v 1.1 2004/06/19 23:02:26 spitzak Exp $"
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

/*! Return the length of a legal UTF-8 encoding that starts with
    this byte. Returns 1 for illegal bytes (anthing less than 0xc2
    and 0xfe and 0xff).

    <i>Danger Will Robinson: this is not going to safely decode UTF-8
    unless you already know the string contains no errors. To actually
    detect errors you must examine all the bytes in the character.
    This function is provided for use in code that only has access
    to the first character (i.e. rewrites of ctype.h calls), calling
    this function is an indication that something in wrong!</i>
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

/*! Decode a UTF-16 character, as used on Windows, into actual Unicode.
    The number of locations at \e p used (1 or 2) is returned. \e end
    points at the end of the buffer, if \e p is \e end-1 then only
    one location is read.

    Many programs think this transformation is the identity. Unfortunatley
    because the number of assigned Unicode characters was obviously
    going to exceed 65535, the standard defines a foul invention called
    "surrogate pairs", where two 16-bit numbers encode one character.
    This removes the only plausable reason to use 16-bit characters
    instead of UTF-8 (the fixed size of characters) and is why we
    don't bother with them in FLTK. This function is only provided
    in the hopes that people will start to correctly translate this
    crap on Windows and we can safely convert it to UTF-8.

    Warning: as I don't have any real text with surrogate pairs to
    test, I am not really sure this is doing the correct thing...
*/
unsigned utf16decode(unsigned short* p, unsigned short* end, int* len) {
  unsigned ucs = p[0];
  if (ucs >= 0xd800 && ucs <= 0xdbff && p+1 < end) { // first of surrogate pair
    unsigned ucs2 = p[1];
    if (ucs2 >=0xdc00 && ucs2 <= 0xdfff) { // an actual pair
      *len = 2;
      return 0x10000U + ((ucs&0x3ff)<<10) + (ucs2&0x3ff);
    }
  }
  *len = 1;
  return ucs;
}

/*! Write the UTF-16 encoding of the Unicode \e ucs into \e buf and
    return the number of words written (1 or 2). This should be used
    to convert UTF-8 strings for use by Windows API calls.

    If \e ucs is greater than 0x10ffff it cannot be encoded in UTF-16.
    The current version will write 0xfffe (an illegal character) in
    this case, but this may change in the future.
*/
int utf16encode(unsigned ucs, unsigned short* buf) {
  if (ucs < 0x10000U) {
    buf[0] = (unsigned short)ucs;
    return 1;
  } else if (ucs > 0x10ffffU) {
    // error!
    buf[0] = 0xfffe;
    return 1;
  } else {
    // write a surrogate pair:
    buf[0] = (((ucs-0x10000u)>>10)&0x3ff) | 0xd800;
    buf[1] = (ucs&0x3ff) | 0xdc00;
    return 2;
  }
}

#ifdef __cplusplus
}
#endif
