//
// "$Id: utf.c,v 1.9 2004/07/27 07:03:08 spitzak Exp $"
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
    // RFC 3629 says surrogate chars are illegal.
    // I don't check this so that all 16-bit values are preserved
    // when going through utf8encode/utf8decode.
    if (((unsigned char*)p)[1] >= 0xa0) goto FAIL;
    goto UTF8_3;
  } else if (c == 0xef) {
    // 0xfffe and 0xffff are also illegal characters
    // Again I don't check this so 16-bit values are preserved
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
  } else if (c < 0xf4) {
  UTF8_4:
    if (p+3 >= end || (p[2]&0xc0) != 0x80 || (p[3]&0xc0) != 0x80) goto FAIL;
    *len = 4;
#if 0
    // All codes ending in fffe or ffff are illegal:
    if ((p[1]&0xf)==0xf &&
	((unsigned char*)p)[2] == 0xbf &&
	((unsigned char*)p)[3] >= 0xbe) goto FAIL;
#endif
    return
      ((p[0] & 0x07) << 18) +
      ((p[1] & 0x3f) << 12) +
      ((p[2] & 0x3f) << 6) +
      ((p[3] & 0x3f));
  } else if (c == 0xf4) {
    if (((unsigned char*)p)[1] > 0x8f) goto FAIL; // after 0x10ffff
    goto UTF8_4;
  } else {
  FAIL:
    *len = 1;
    return c;
  }
}

/*! Return the length of a legal UTF-8 encoding that starts with
    this byte. Returns 1 for illegal bytes (0xc0, 0xc1, 0xf5 to 0xff).

    <i>This function is depreciated. If the following bytes are
    not legal UTF-8 then using this to step forward to the next
    character will produce different positions than utf8decode()
    will produce.
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
    number of bytes written. Up to 4 bytes may be written. If you know
    that \a ucs is less than 0x10000 then at most 3 bytes will be written.
    If you wish to speed this up, remember that anything less than 0x80
    is written as a single byte.

    If ucs is greater than 0x10ffff this is an illegal character
    according to RFC 3629. These are converted as though they are
    0xFFFD (REPLACEMENT CHARACTER).

    \a ucs in the range 0xd800 to 0xdfff, or ending with 0xfffe or
    0xffff are also illegal according to RFC 3629. However I encode
    these as though they are legal, so that utf8encode/utf8decode will
    be the identity for all codes between 0 and 0x10ffff.
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
    buf[0] = 0xefU;
    buf[1] = 0xbfU;
    buf[2] = 0xbdU;
    return 3;
  }
}

/*! Convert a UTF-8 sequence into an array of 32-bit Unicode indexes.

    \a src points at the UTF-8, and \a srclen is the number of bytes to
    convert.

    \a dst points at an array to write the Unicode to, and \a dstlen
    is the number of locations in this array. At most \a dstlen-1
    32-bit words will be written there, plus a 0 terminating
    character. Thus this function will never overwrite the buffer and
    will always return a zero-terminated string. If \a dstlen is zero
    then \a dst can be null and no data is written, but the length is
    returned.

    The return value is the number of words that \e would be written
    to \a dst if it were long enough, not counting the terminating
    zero. If the return value is greater or equal to \a dstlen it
    indicates truncation, you can then allocate a new array of size
    return+1 and call this again.

    Errors in the UTF-8 are converted as though each byte in the
    erroneous string is in the Microsoft CP1252 encoding. This allows
    ISO-8859-1 text mistakenly identified as UTF-8 to be printed
    correctly.  */
unsigned utf8to32(const char* src, unsigned srclen,
		  unsigned* dst, unsigned dstlen)
{
  const char* p = src;
  const char* e = src+srclen;
  unsigned count = 0;
  if (dstlen) for (;;) {
    if (p >= e) {dst[count] = 0; return count;}
    if (!(*p & 0x80)) { // ascii
      dst[count] = *p++;
    } else {
      int len;
      dst[count] = utf8decode(p,e,&len);
      p += len;
    }
    if (++count >= dstlen) {dst[count-1] = 0; break;}
  }	
  // we filled dst, measure the rest:
  while (p < e) {
    if (!(*p & 0x80)) p++;
    else {
      int len;
      utf8decode(p,e,&len);
      p += len;
    }
    ++count;
  }
  return count;
}

/*! Convert a UTF-8 sequence into UTF-16 (the encoding used on Windows
    for the "W" versions of calls).

    \a src points at the UTF-8, and \a srclen is the number of bytes to
    convert.

    \a dst points at an array to write the UTF-16 to, and \a dstlen is
    the number of locations in this array. At most \a dstlen-1 16-bit
    words will be written there, plus a 0 terminating character. Thus
    this function will never overwrite the buffer and will always
    return a zero-terminated string. If \a dstlen is zero then \a dst
    can be null and no data is written, but the length is returned.

    \a dst is of type "unsigned short*" rather than "wchar_t*" due to
    the unfortunate decision on Unix to define wchar_t to be 32 bits.
    It appears that you need to use a cast on WindowsCE to send these
    results to some Windows functions.

    The return value is the number of words that \e would be written
    to \a dst if it were long enough, not counting the terminating
    zero. If the return value is greater or equal to \a dstlen it
    indicates truncation, you can then allocate a new array of size
    return+1 and call this again.

    Errors in the UTF-8 are converted as though each byte in the
    erroneous string is in the Microsoft CP1252 encoding. This allows
    ISO-8859-1 text mistakenly identified as UTF-8 to be printed
    correctly.

    Codes in the range 0x10000 to 0x10ffff are converted to "surrogate
    pairs" which take two words each. Not all Windows functions expect
    this.
*/
unsigned utf8to16(const char* src, unsigned srclen,
		  unsigned short* dst, unsigned dstlen)
{		  
  const char* p = src;
  const char* e = src+srclen;
  unsigned count = 0;
  if (dstlen) for (;;) {
    if (p >= e) {dst[count] = 0; return count;}
    if (!(*p & 0x80)) { // ascii
      dst[count] = *p++;
    } else {
      int len; unsigned ucs = utf8decode(p,e,&len);
      p += len;
      if (ucs < 0x10000) {
	dst[count] = ucs;
      } else {
	// make a surrogate pair:
	if (count+2 >= dstlen) {dst[count] = 0; count += 2; break;}
	dst[count] = (((ucs-0x10000u)>>10)&0x3ff) | 0xd800;
	dst[++count] = (ucs&0x3ff) | 0xdc00;
      }
    }
    if (++count == dstlen) {dst[count-1] = 0; break;}
  }
  // we filled dst, measure the rest:
  while (p < e) {
    if (!(*p & 0x80)) p++;
    else {
      int len; unsigned ucs = utf8decode(p,e,&len);
      p += len;
      if (ucs >= 0x10000) ++count;
    }
    ++count;
  }
  return count;
}

/*! Convert a UTF-8 sequence into an array of 1-byte characters.

    If the UTF-8 decodes to a character greater than 0xff then it is
    replaced with '¿' (0xbf).

    Errors in the UTF-8 are converted as individual bytes, same as
    utf8decode() does. This allows ISO-8859-1 text mistakenly identified
    as UTF-8 to be printed correctly (and possibly CP1512 on Windows).

    \a src points at the UTF-8, and \a srclen is the number of bytes to
    convert.

    \a dst points at a char array to write, and \a dstlen is the
    number of characters in this array. At most \a dstlen-1 chars
    will be written there, plus a NUL terminating character. Thus
    this function will never overwrite the buffer and will always
    return a nul-terminated string. If \a dstlen is zero then \a dst
    can be null and no data is written, but the length is returned.

    The return value is the number of chars that \e would be written
    to \a dst if it were long enough, not counting the terminating
    nul. If the return value is greater or equal to \a dstlen it
    indicates truncation, you can then allocate a new array of size
    return+1 and call this again.
*/
unsigned utf8toa(const char* src, unsigned srclen,
		 char* dst, unsigned dstlen)
{
  const char* p = src;
  const char* e = src+srclen;
  unsigned count = 0;
  if (dstlen) for (;;) {
    unsigned char c;
    if (p >= e) {dst[count] = 0; return count;}
    c = *(unsigned char*)p;
    if (c < 0xC2) { // ascii or bad code
      dst[count] = c;
      p++;
    } else {
      int len; unsigned ucs = utf8decode(p,e,&len);
      p += len;
      if (ucs < 0x100) dst[count] = ucs;
      else dst[count] = 0xbfU;
    }
    if (++count >= dstlen) {dst[count-1] = 0; break;}
  }	
  // we filled dst, measure the rest:
  while (p < e) {
    if (!(*p & 0x80)) p++;
    else {
      int len;
      utf8decode(p,e,&len);
      p += len;
    }
    ++count;
  }
  return count;
}

/*! Turn UTF-16 (such as Windows uses for "wide characters") into UTF-8.

    \a dst points at an array to write the UTF-8 to, and \a dstlen is
    the number of bytes in this array. At most \a dstlen-1 bytes
    will be written there, plus a 0 terminating character. Thus
    this function will never overwrite the buffer and will always
    return a zero-terminated string. If \a dstlen is zero then \a dst
    can be null and no data is written, but the length is returned.

    \a src points at the UTF-16, and \a srclen is the number of words
    to convert. \a src is of type "unsigned short*" rather than
    "wchar_t*" due to the unfortunate decision on Unix to define
    wchar_t to be 32 bits.  It appears that you need to use a cast on
    WindowsCE to send these results to some Windows functions.

    The return value is the number of bytes that \e would be written
    to \a dst if it were long enough, not counting the terminating
    zero. If the return value is greater or equal to \a dstlen it
    indicates truncation, you can then allocate a new array of size
    return+1 and call this again.

    Unmatched halves of "surrogate pairs" are converted directly to
    the same Unicode value. This is supposedly the wrong thing to do,
    but this way converting to UTF-8 and back again is lossless.
*/
unsigned utf8from16(char* dst, unsigned dstlen,
		    const unsigned short* src, unsigned srclen) {
  unsigned i = 0;
  unsigned count = 0;
  if (dstlen) for (;;) {
    unsigned ucs;
    if (i >= srclen) {dst[count] = 0; return count;}
    ucs = src[i++];
    if (ucs < 0x80U) {
      dst[count] = ucs;
      if (++count >= dstlen) {dst[count-1] = 0; break;}
    } else if (ucs < 0x800U) { // 2 bytes
      if (count+2 >= dstlen) {dst[count] = 0; count += 2; break;}
      dst[count] = 0xc0 | (ucs >> 6);
      dst[++count] = 0x80 | (ucs & 0x3F);
    } else if (ucs >= 0xd800 && ucs <= 0xdbff && i < srclen &&
	       src[i] >= 0xdc00 && src[i] <= 0xdfff) {
      // surrogate pair
      unsigned ucs2 = src[i++];
      ucs = 0x10000U + ((ucs&0x3ff)<<10) + (ucs2&0x3ff);
      // all surrogate pairs turn into 4-byte utf8
      if (count+4 >= dstlen) {dst[count] = 0; count += 4; break;}
      dst[count] = 0xf0 | (ucs >> 18);
      dst[++count] = 0x80 | ((ucs >> 12) & 0x3F);
      dst[++count] = 0x80 | ((ucs >> 6) & 0x3F);
      dst[++count] = 0x80 | (ucs & 0x3F);
    } else {
      // all others are 3 bytes:
      if (count+3 >= dstlen) {dst[count] = 0; count += 3; break;}
      dst[count] = 0xe0 | (ucs >> 12);
      dst[++count] = 0x80 | ((ucs >> 6) & 0x3F);
      dst[++count] = 0x80 | (ucs & 0x3F);
    }
  }	
  // we filled dst, measure the rest:
  while (i < srclen) {
    unsigned ucs = src[i++];
    if (ucs < 0x80U) {
      count++;
    } else if (ucs < 0x800U) { // 2 bytes
      count += 2;
    } else if (ucs >= 0xd800 && ucs <= 0xdbff && i < srclen-1 &&
	       src[i+1] >= 0xdc00 && src[i+1] <= 0xdfff) {
      // surrogate pair
      ++i;
      count += 4;
    } else {
      count += 3;
    }
  }
  return count;
}

#ifdef _WIN32
# include <windows.h>
#endif

/*! Convert the UTF-8 used by FLTK to the locale-specific encoding
    used for filenames and some other calls on Windows. Unfortunatley
    due to stupid design you will have to do this as needed for
    filenames.

    Like the other calls, up to \a dstlen bytes are written to \a dst,
    including the null terminator. The return value is the number of bytes
    that would be written, not counting the null terminator. If greator
    or equal to \a dstlen then if you malloc a new array of size n+1 you
    will have the space needed for the entire string.

    On Unix or on Windows when a UTF-8 locale is in effect, this
    does not change the data. It is copied and truncated as necessary to
    the destination buffer and \a srclen is always returned.
*/
unsigned utf8tomb(const char* src, unsigned srclen,
		  char* dst, unsigned dstlen)
{
#ifdef _WIN32
  unsigned cp = GetACP();
  if (cp != CP_UTF8) {
    unsigned short lbuf[1024];
    unsigned short* buf = lbuf;
    unsigned length = utf8to16(src, srclen, buf, 1024);
    unsigned ret;
    if (length >= 1024) {
      buf = (unsigned short*)(malloc((length+1)*sizeof(unsigned short)));
      utf8to16(src, srclen, buf, length+1);
    }
    ret =
      WideCharToMultiByte(cp, 0, buf, length, dst, dstlen, 0, 0) - 1;
    // if it overflows, get the actual length:
    if (ret >= dstlen-1)
      ret =
	WideCharToMultiByte(cp, 0, buf, length, 0, 0, 0, 0) - 1;
    if (buf != lbuf) free((void*)buf);
    return ret;
  }
#endif
  // identity transform:
  if (srclen <= dstlen) {
    memcpy(dst, src, srclen);
    dst[srclen] = 0;
  } else {
    memcpy(dst, src, dstlen-1);
    dst[dstlen] = 0;
  }
  return srclen;
}
  
/*! Convert a filename from the locale-specific multibyte encoding
    used by Windows to UTF-8 as used by FLTK.

    Like the other calls, up to \a dstlen bytes are written to \a dst,
    including the null terminator. The return value is the number of bytes
    that would be written, not counting the null terminator. If greator
    or equal to \a dstlen then if you malloc a new array of size n+1 you
    will have the space needed for the entire string.

    On Unix or on Windows when a UTF-8 locale is in effect, this
    does not change the data. It is copied and truncated as necessary to
    the destination buffer and \a srclen is always returned.
*/
unsigned utf8frommb(char* dst, unsigned dstlen,
		    const char* src, unsigned srclen)
{
#ifdef _WIN32
  UINT cp = GetACP();
  if (cp != CP_UTF8) {
    unsigned short lbuf[1024];
    unsigned short* buf = lbuf;
    unsigned length;
    unsigned ret;
    length =
      MultiByteToWideChar(cp, 0, src, srclen, buf, 1024);
    if (length >= 1024) {
      length = MultiByteToWideChar(cp, 0, src, srclen, 0, 0);
      buf = (unsigned short*)(malloc(length*sizeof(unsigned short)));
      MultiByteToWideChar(cp, 0, src, srclen, buf, length);
    }
    ret = utf8from16(dst, dstlen, buf, length);
    if (buf != lbuf) free((void*)buf);
    return ret;
  }
#endif
  // identity transform:
  if (srclen <= dstlen) {
    memcpy(dst, src, srclen);
    dst[srclen] = 0;
  } else {
    memcpy(dst, src, dstlen-1);
    dst[dstlen] = 0;
  }
  return srclen;
}
  


#if 0 // These are nyi...

unsigned utf8from32(char* dst, unsigned dstlen,
		    const unsigned short* src, unsigned srclen);

// utf-8 cleanup, replaces errors with legal utf-8?
unsigned utf8toutf8(char* dst, unsigned dstlen,
		    const char* src, unsigned srclen);


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
#endif

#ifdef __cplusplus
}
#endif
