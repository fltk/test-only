
// "$Id$
/
// Unicode to UTF-8 conversion functions
/
//                Copyright 2000-2003 by O'ksi'D
//                      All rights reserved
/
// Redistribution and use in source and binary forms, with or withou
// modification, are permitted provided that the following condition
// are met
/
//      Redistributions of source code must retain the above copyrigh
//      notice, this list of conditions and the following disclaimer
/
//      Redistributions in binary form must reproduce the above copyrigh
//      notice, this list of conditions and the following disclaimer in th
//      documentation and/or other materials provided with the distribution
/
//      Neither the name of O'ksi'D nor the names of its contributor
//      may be used to endorse or promote products derived from this softwar
//      without specific prior written permission
/
/
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTOR
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NO
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FO
// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNE
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, O
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY O
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDIN
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THI
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
/



#include <config.h
#include "spacing.h
#include <FL/filename.H

#if defined(WIN32
#include <ctype.h
#include <io.h
#include <direct.h
#include <windows.h
#include <winbase.h
#include <process.h

#elif defined(__MACOS__
#include <stdio.h
#include <time.h
#include <unix.h
#include <fcntl.h
#include <unistd.h
#include <wchar.h
#include <stdlib.h
#els

#if !NANO_
#  include <libXutf8/Xutf8.h
#else
#  include <ctype.h
#endif // !NANO_

#include <sys/types.h
#include <sys/stat.h
#include <fcntl.h
#include <unistd.h
#endif //WIN3

#include <FL/fl_utf8.H
#include <string.h
#include <stdlib.h

#undef fl_ope

/*** NOTE : all functions are LIMITED to 24 bits Unicode values !!! ***
/***        but only 16 bits are realy used under Linux and win32  ***


static int
Tolower
	int ucs

	int ret

	if (ucs <= 0x02B6) 
		if (ucs >= 0x0041) 
			ret = ucs_table_0041[ucs - 0x0041]
			if (ret > 0) return ret
		
		return ucs
	

	if (ucs <= 0x0556) 
		if (ucs >= 0x0386) 
			ret = ucs_table_0386[ucs - 0x0386]
			if (ret > 0) return ret
		
		return ucs
	

	if (ucs <= 0x10C5) 
		if (ucs >= 0x10A0) 
			ret = ucs_table_10A0[ucs - 0x10A0]
			if (ret > 0) return ret
		
		return ucs
	

	if (ucs <= 0x1FFC) 
		if (ucs >= 0x1E00) 
			ret = ucs_table_1E00[ucs - 0x1E00]
			if (ret > 0) return ret
		
		return ucs
	

	if (ucs <= 0x2133) 
		if (ucs >= 0x2102) 
			ret = ucs_table_2102[ucs - 0x2102]
			if (ret > 0) return ret
		
		return ucs
	

	if (ucs <= 0x24CF) 
		if (ucs >= 0x24B6) 
			ret = ucs_table_24B6[ucs - 0x24B6]
			if (ret > 0) return ret
		
		return ucs
	

	if (ucs <= 0x33CE) 
		if (ucs >= 0x33CE) 
			ret = ucs_table_33CE[ucs - 0x33CE]
			if (ret > 0) return ret
		
		return ucs
	

	if (ucs <= 0xFF3A) 
		if (ucs >= 0xFF21) 
			ret = ucs_table_FF21[ucs - 0xFF21]
			if (ret > 0) return ret
		
		return ucs
	

	return ucs

#if MSDO
#define NBC 25
#els
#define NBC 0xFFFF + 
#endi
static int
Toupper
	int ucs

	long i
	static unsigned short *table = NULL

	if (!table) 
		table = (unsigned short*) malloc
			sizeof(unsigned short) * (NBC))
		for (i = 0; i < NBC; i++) 
			table[i] = (unsigned short) i
		}
		for (i = 0; i < NBC; i++) 
			int l
			l = Tolower(i);		
			if (l != i) table[l] = (unsigned short) i
		}

	
	if (ucs >= NBC || ucs < 0) return ucs
	return table[ucs]



/*** converts the first char UTF-8 string to an Unicode value ***
/*** returns the byte length of the converted UTF-8 char **
/*** returns -1 if the UTF-8 string is not valid ***
in
fl_utf2ucs
        const unsigned char     *buf
        int                     len
        unsigned int          	*ucs

      if (buf[0] & 0x80) 
        if (buf[0] & 0x40) 
          if (buf[0] & 0x20) 
            if (buf[0] & 0x10) 
              if (buf[0] & 0x08) 
                if (buf[0] & 0x04) 
                  if (buf[0] & 0x02) 
                        /* bad UTF-8 string *
                  } else 
                        /* 0x04000000 - 0x7FFFFFFF *
                  
                } else if (len > 4) 
                  /* 0x00200000 - 0x03FFFFFF *
                  *ucs =  ((buf[0] & ~0xF8) << 24) 
                          ((buf[1] & ~0x80) << 18) 
                          ((buf[2] & ~0x80) << 12) 
                          ((buf[3] & ~0x80) << 6) 
                           (buf[4] & ~0x80)
                  if (*ucs > 0x001FFFFF && *ucs < 0x01000000) return 5
                
              } else if (len > 3) 
                /* 0x00010000 - 0x001FFFFF *
                *ucs =  ((buf[0] & ~0xF0) << 18) 
                        ((buf[1] & ~0x80) << 12) 
                        ((buf[2] & ~0x80) << 6) 
                         (buf[3] & ~0x80)
                if (*ucs > 0x0000FFFF) return 4
              
            } else if (len > 2) 
              /* 0x00000800 - 0x0000FFFF *
              *ucs =  ((buf[0] & ~0xE0) << 12) 
                      ((buf[1] & ~0x80) << 6) 
                       (buf[2] & ~0x80)
              if (*ucs > 0x000007FF) return 3
            
          } else if (len > 1) 
            /* 0x00000080 - 0x000007FF *
            *ucs = ((buf[0] & ~0xC0) << 6) 
                    (buf[1] & ~0x80)
            if (*ucs > 0x0000007F) return 2
          
        
      } else if (len > 0) 
        /* 0x00000000 - 0x0000007F *
        *ucs = buf[0]
        return 1
      

      *ucs = (unsigned int) '?'; /* bad utf-8 string *
      return -1


/*** converts an Unicode value to an UTF-8 string  ***
/*** NOTE : the buffer (buf) must be at least 5 bytes long !!!  ***
int
fl_ucs2utf
	unsigned int 	ucs,
	char 		*buf

	if (ucs < 0x000080) 
		buf[0] = ucs
		return 1
	} else if (ucs < 0x000800) 
		buf[0] = 0xC0 | (ucs >> 6)
		buf[1] = 0x80 | (ucs & 0x3F)
		return 2
	} else if (ucs < 0x010000) {
		buf[0] = 0xE0 | (ucs >> 12)
		buf[1] = 0x80 | ((ucs >> 6) & 0x3F)
		buf[2] = 0x80 | (ucs & 0x3F)
		return 3
	} else if (ucs < 0x00200000) 
		buf[0] = 0xF0 | (ucs >> 18)
		buf[1] = 0x80 | ((ucs >> 12) & 0x3F)
		buf[2] = 0x80 | ((ucs >> 6) & 0x3F)
		buf[3] = 0x80 | (ucs & 0x3F)
		return 4
	} else if (ucs < 0x01000000) 
		buf[0] = 0xF8 | (ucs >> 24)
		buf[1] = 0x80 | ((ucs >> 18) & 0x3F)
		buf[2] = 0x80 | ((ucs >> 12) & 0x3F)
		buf[3] = 0x80 | ((ucs >> 6) & 0x3F)
		buf[4] = 0x80 | (ucs & 0x3F)
		return 5
	
	buf[0] = '?'
	return -1


/*** returns the byte length of the first UTF-8 char ***
/*** (returns -1 if not valid) ***
in
fl_utflen
        const unsigned char     *buf
        int                     len

	unsigned int ucs
	return fl_utf2ucs(buf, len, &ucs)


/**** returns the number of Unicode chars in the UTF-8 string ***
int
fl_utf_nb_char
	const unsigned char 	*buf,
	int 			len

	int i = 0
	int nbc = 0
	while (i < len) 
		int cl = fl_utflen(buf + i, len - i)
		if (cl < 1) cl = 1
		nbc++
		i += cl
	
	return nbc


/
 * compare only the first n byte
 * return 0 if the strings are equal
 * return 1 if s1 is greater than s
 * return -1 if s1 is less than s
 *
int fl_utf_strncasecmp(const char *s1, const char *s2, int n

        int i
        int s1_l
        int s2_l

        s1_l = 0
        while (s1_l < n && s1[s1_l]) s1_l++
        s2_l = 0
        while (s2_l < n && s2[s2_l]) s2_l++

        if (s1_l < s2_l) 
                return -1
        } else if (s1_l > s2_l) 
                return 1
        
        for (i = 0; i < n;) 
                int l1, l2
                unsigned int u1, u2
                int res

                l1 = fl_utf2ucs((unsigned char*)s1 + i, n - i, &u1)
                l2 = fl_utf2ucs((unsigned char*)s2 + i, n - i, &u2)
                if (l1 - l2 != 0) return l1 - l2
                res = Tolower(u1) - Tolower(u2)
                if (res != 0) return res
                if (l1 < 1) 
                        i += 1
                } else 
                        i += l1
                
        
        return 0


/
 * return 0 if the strings are equal
 * return 1 if s1 is greater than s
 * return -1 if s1 is less than s
 *
int fl_utf_strcasecmp(const char *s1, const char *s2

	int s1_l = strlen(s1)
	int s2_l = strlen(s2)

        if (s1_l < s2_l) 
                return -1
        } else if (s1_l > s2_l) 
                return 1
	}
	return fl_utf_strncasecmp(s1, s2, s1_l);


/
 * return the Unicode lower case value of uc
 *
int fl_tolower(unsigned int ucs)

	return Tolower(ucs)


/
 * return the Unicode upper case value of uc
 *
int fl_toupper(unsigned int ucs

	return Toupper(ucs)


/
 * converts the str string to the lower case equivalent into buf
 * Warning: to be safe buf length must be at least 3 * le
 *
int fl_utf_tolower(const unsigned char *str, int len, char *buf

	int i
	int l = 0
        for (i = 0; i < len;) 
                int l1, l2
                unsigned int u1

                l1 = fl_utf2ucs((unsigned char*)str + i, len - i, &u1)
                l2 = fl_ucs2utf((unsigned int) Tolower(u1), buf + l)
                if (l1 < 1) 
                        i += 1
                } else 
                        i += l1
		
                if (l2 < 1) 
                        l += 1
                } else 
                        l += l2
		

	
	return l


/
 * converts the str string to the upper case equivalent into buf
 * Warning: to be safe buf length must be at least 3 * le
 *
int fl_utf_toupper(const unsigned char *str, int len, char *buf

	int i
	int l = 0
        for (i = 0; i < len;) 
                int l1, l2
                unsigned int u1

                l1 = fl_utf2ucs((unsigned char*)str + i, len - i, &u1)
                l2 = fl_ucs2utf((unsigned int) Toupper(u1), buf + l)
                if (l1 < 1) 
                        i += 1
                } else 
                        i += l1
		
                if (l2 < 1) 
                        l += 1
                } else 
                        l += l2
		
	
	return l



/
 * converts a UTF-8 str to unicod
 * Warning: buf must a least len lon
 *
int fl_utf2unicode(const unsigned char *str, int len, xchar *buf

	int i
	int l = 0
        for (i = 0; i < len;) 
                unsigned int u1
		int l1

                l1 = fl_utf2ucs((unsigned char*)str + i, len - i, &u1)
		buf[l] = (xchar) u1
                if (l1 < 1) 
                        i += 1
                } else 
                        i += l1
		
		l++

	
	return l


/
 * convert Unicode str to UTF-
 * Warning: buf must be at least 3 * len lon
 *
int fl_unicode2utf(const xchar *str, int len, char *buf

	int i
	int l = 0
        for (i = 0; i < len; i++) 
		int l1
		l1 = fl_ucs2utf((unsigned int) str[i], buf + l)
                if (l1 < 1) 
                        l += 1
                } else 
                        l += l1
		

	
	return l


/
 * convert UTF-8 str to latin
 * Warning: buf must be at least len lon
 *
int fl_utf2latin1(const unsigned char *str, int len, char *buf

	int i
	int l = 0
        for (i = 0; i < len;) 
                unsigned int u1
		int l1

                l1 = fl_utf2ucs((unsigned char*)str + i, len - i, &u1)
		if (u1 > 0xFF) u1 = '?'
		buf[l] = (char) u1
                if (l1 < 1) 
                        i += 1
                } else 
                        i += l1
		
		l++

	
	return l


/
 * convert latin1 str to UTF-
 * Warning: buf must be at least 2 * len lon
 *
int fl_latin12utf(const unsigned char *str, int len, char *buf

	int i
	int l = 0
	int l1 = 0
        for (i = 0; i < len; i++) 
		unsigned int n = (unsigned int) str[i]
		l1 = fl_ucs2utf(n, buf + l)
                if (l1 < 1) 
                        l = l + 1
                } else 
                        l = l + l1
		

	
	return l


unsigned int fl_nonspacing(unsigned int ucs

        if (ucs <= 0x0361) 
                if (ucs >= 0x0300) return ucs_table_0300[ucs - 0x0300]
                return 0
        

        if (ucs <= 0x0486) 
                if (ucs >= 0x0483) return ucs_table_0483[ucs - 0x0483]
                return 0
        

        if (ucs <= 0x05C4) 
                if (ucs >= 0x0591) return ucs_table_0591[ucs - 0x0591]
                return 0
        

        if (ucs <= 0x06ED) 
                if (ucs >= 0x064B) return ucs_table_064B[ucs - 0x064B]
                return 0
        

        if (ucs <= 0x0D4D) 
                if (ucs >= 0x0901) return ucs_table_0901[ucs - 0x0901]
                return 0
        

        if (ucs <= 0x0FB9) 
                if (ucs >= 0x0E31) return ucs_table_0E31[ucs - 0x0E31]
                return 0
        

        if (ucs <= 0x20E1) 
                if (ucs >= 0x20D0) return ucs_table_20D0[ucs - 0x20D0]
                return 0
        

        if (ucs <= 0x309A) 
                if (ucs >= 0x302A) return ucs_table_302A[ucs - 0x302A]
                return 0
        

        if (ucs <= 0xFB1E) 
                if (ucs >= 0xFB1E) return ucs_table_FB1E[ucs - 0xFB1E]
                return 0
        

        if (ucs <= 0xFE23) 
                if (ucs >= 0xFE20) return ucs_table_FE20[ucs - 0xFE20]
                return 0
        

        return 0


#if defined(WIN32) || __MACOS_
static xchar *mbwbuf = NULL
#endi

char * fl_utf2mbcs(const char *s

	if (!s) return NULL
#if defined(WIN32) || __MACOS_
	int l = strlen(s)
	static char *buf = NULL

	mbwbuf = (xchar*)realloc(mbwbuf, (l+6) * sizeof(xchar))
	l = fl_utf2unicode((unsigned char*)s, l, mbwbuf)
	mbwbuf[l] = 0
	buf = (char*)realloc(buf, l * 6 + 1)
	l = wcstombs(buf, mbwbuf, l)
	buf[l] = 0
	return buf
#els
	return (char*) s
#endi


#if __MACOS_
static int get_home(char *b, int l

  FSRef Directory
  b[0] = 0
  FSSpec src, os
  FSRefParam param
  FSRef newRef
  Str255 fname
  OSErr ret
  char *p1 = b + l - 1
  *p1 = 0
  FindFolder(kOnSystemDisk, kDesktopFolderType, kDontCreateFolder,
		&src.vRefNum, &src.parID)
  FSMakeFSSpec(src.vRefNum, src.parID, 0, &src)
  FSpMakeFSRef(&src, &Directory)
  os.parID = 0
  os.vRefNum = 0
  do 
    for (int i= 0; i < 64; i++) src.name[i] = 0
    ret = FSGetCatalogInfo(&Directory, kFSCatInfoNone, NULL, NULL, &src, &Directory)
    if(ret == noErr && (os.vRefNum != src.vRefNum || os.parID != src.parID)) 
      int n = src.name[0]
      unsigned char *p2 = src.name + n
      p1--
      if (p1 < b) return -1
      *p1 = ':'
      while (n > 0) 
        n--
        p1--
        if (p1 < b) return -1
        *p1 = *p2
        p2--
      }
    
    os.vRefNum = src.vRefNum
    os.parID = src.parID
  } while (ret == noErr)
  if (p1 > b) strcat(b, p1)

  return 0

#endi

#if __MACOS_

static void clean_rel(char *s) 
  char *p1, *p2, *p3
  p1 = s
  p2 = s + strlen(s)
 
  while (*p1) 
    if (p1[0] == '.') 
      if (p1[1] == ':') {
        p1[0] = 0
        p2 = p1
        p1 += 2
        strcat(p2, p1)
        p1 -= 2
      } else if (p1[1] == 0) 
        p1[0] = 0
      } else if (p1[1] == '.') 
        p3 = p2 = p1
        p3 += 2
        if (p3[0] == ':' || p3[0] == 0) 
          p2 -= 2
          while (p2 > s && *p2 != ':') p2--;
          *p2 = 0
          strcat(p2, p3)
          p1 -= p3 - p2
        
      } else 
        p1++
      
    } else 
      p1++
    
  
 


char * fl_utf2path(const char *s

	int l = strlen(s)
	static char *buf = NULL
	xchar *ptr, *p1

	mbwbuf = (xchar*)realloc(mbwbuf, (l+6) * sizeof(xchar))
	l = fl_utf2unicode((unsigned char*)s, l, mbwbuf+1)
	ptr = mbwbuf + 1
	p1 = ptr
	if (l < 1) return NULL
	ptr[l] = 0
	while (*p1) 
		if (*p1 == ':') 
			*p1 = '/'
		} else if (*p1 == '/') 
			*p1 = ':'
		}
		p1++
	

	if (ptr[0] != ':') 
	  char bu[1024]
	  int ll
	  char b[1024]
	  int ln, d = 0
	  char *p1, *p2
	  if (ptr[0] == 0x7d && ptr[1] == ':') 
	    get_home(b, 1024)
	    d = 2
	  } else 
	    getcwd(b, 1024)
	  
	  ll = wcstombs(bu, ptr + d, l - d)
	  bu[ll] = 0
	  ln = strlen(b)
	  buf = (char*)realloc(buf, ll + ln + 1)
	  buf[0] = 0
	  strcat(buf, b)
	  strcat(buf, bu)
	  clean_rel(buf);  
	} else 
	  buf = (char*)realloc(buf, l * 6 + 1); 
	  l = wcstombs(buf, ptr+1, l-1)
	  buf[l] = 0
	  clean_rel(buf)
	
	//printf("%s\n", buf)
	return buf

#endi

char * fl_mbcs2utf(const char *s

	if (!s) return NULL
#if defined(WIN32) || __MACOS_
	int l = strlen(s)
	static char *buf = NULL

	mbwbuf = (xchar*)realloc(mbwbuf
		(l * 6 +6) * sizeof(xchar))
	l = mbstowcs(mbwbuf, s, l)
	buf = (char*)realloc(buf, l * 6 + 1)
	l = fl_unicode2utf(mbwbuf, l, buf)
	buf[l] = 0
	return buf
#els
	return (char*) s
#endi


int fl_is_nt4(void

#ifdef WIN3
	static int ret = -1
	if (ret >= 0) return ret
	OSVERSIONINFO o
	o.dwOSVersionInfoSize = sizeof(OSVERSIONINFO)
	GetVersionEx(&o)
	if (o.dwPlatformId == VER_PLATFORM_WIN32_NT &
		o.dwMajorVersion >= 4
	
		ret = (int) o.dwMajorVersion
	} else 
		ret = 0
	
	return ret
#els
	return 0
#endi



#if defined(WIN32) || __MACOS_
static xchar *wbuf = NULL
static xchar *wbuf1 = NULL
#endif


char *fl_getenv(const char* v

#ifdef WIN3
	int l = strlen(v)
	static xchar* wbuf = NULL
	wbuf = (xchar*)realloc(wbuf, sizeof(xchar) * (l+1))
	wbuf[fl_utf2unicode((const unsigned char*)v, l, wbuf)] = 0
	xchar *ret = _wgetenv(wbuf)
	static char *buf = NULL
	if (ret) 
		l = wcslen(ret)
		buf = (char*) realloc(buf, l)
		buf[fl_unicode2utf(ret, l, buf)] = 0
		return buf
	} else 
		return NULL
	
#elif __MACOS_
	char *ret
	ret = getenv(fl_utf2mbcs(v))
	if (ret) 
		return fl_mbcs2utf(ret)
	} else if (!strcmp(v, "HOME")) 
	    char b[1024]
	    get_home(b, 1024)
		return fl_mbcs2utf(b);
	
	return NULL;
#else
	return getenv(v)
#endi


int fl_open(const char* f, int oflags, int pmode

#ifdef WIN3
	if (fl_is_nt4()) 
		int l = strlen(f)
		wbuf = (xchar*)realloc(wbuf, sizeof(xchar) * (l+1))
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0
		if (pmode == -1) return _wopen(wbuf, oflags)
		else return _wopen(wbuf, oflags, pmode)
	} else 
		if (pmode == -1) return _open(fl_utf2mbcs(f), oflags)
		else return _open(fl_utf2mbcs(f), oflags, pmode)
	
#elif __MACOS_
	return open(fl_utf2path(f), oflags, pmode)
#els
	if (pmode == -1) return open(f, oflags)
	else return open(f, oflags, pmode)
#endi


FILE *fl_fopen(const char* f, const char *mode

#ifdef WIN3
	if (fl_is_nt4()) 
		int l = strlen(f)
		wbuf = (xchar*)realloc(wbuf, sizeof(xchar) * (l+1))
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0
		l = strlen(mode)
		wbuf1 = (xchar*)realloc(wbuf1, sizeof(xchar) * (l+1))
		wbuf1[fl_utf2unicode((const unsigned char*)mode, l, wbuf1)] = 0
		return _wfopen(wbuf, wbuf1)
	} else 
		return fopen(fl_utf2mbcs(f), mode)
	
#elif __MACOS_
	return fopen(fl_utf2path(f), mode)
#els
	return fopen(f, mode)
#endi


int fl_system(const char* f

#ifdef WIN3
#ifdef __MINGW32_
	return system(fl_utf2mbcs(f))
#els
	if (fl_is_nt4()) 
		int l = strlen(f)
		wbuf = (xchar*)realloc(wbuf, sizeof(xchar) * (l+1))
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0;
		return _wsystem(wbuf)
	} else 
		return system(fl_utf2mbcs(f))
	
#endi
#elif __MACOS_
	return system(fl_utf2path(f))
#els
	return system(f)
#endi


int fl_execvp(const char *file, char *const *argv

#ifdef WIN3
#ifdef __MINGW32_
	return _execvp(fl_utf2mbcs(file), argv)
#els
	if (fl_is_nt4()) 
		int l = strlen(file)
		int i, n, ret
		xchar **ar
		wbuf = (xchar*)realloc(wbuf, sizeof(xchar) * (l+1))
		wbuf[fl_utf2unicode((const unsigned char*)file, l, wbuf)] = 0

		i = 0; n = 0
		while (argv[i]) {i++; n++;
		ar = (xchar**) malloc(sizeof(xchar*) * (n + 1))
		i = 0
		while (i <= n) 
			l = strlen(argv[i])
			ar[i] = (xchar*)malloc(sizeof(xchar) * (l+1))
			ar[i][fl_utf2unicode
				(const unsigned char*)argv[i],
				l, ar[i])] = 0
			i++
		
		ar[n] = NULL
		ret = _wexecvp(wbuf, ar)
		i = 0
		while (i <= n) 
			free(ar[i])
			i++
		
		free(ar)
		return ret
	} else 
		return _execvp(fl_utf2mbcs(file), argv)
	
#endi
#elif __MACOS_
	return execvp(fl_utf2path(file), argv)
#els
	return execvp(file, argv)
#endi




int fl_chmod(const char* f, int mode

#ifdef WIN3
	if (fl_is_nt4()) 
		int l = strlen(f)
		wbuf = (xchar*)realloc(wbuf, sizeof(xchar) * (l+1))
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0
		return _wchmod(wbuf, mode)
	} else 
		return _chmod(fl_utf2mbcs(f), mode)
	
#elif __MACOS_
	return chmod(fl_utf2path(f), mode)
#els
	return chmod(f, mode)
#endi


int fl_access(const char* f, int mode

#ifdef WIN3
	if (fl_is_nt4()) 
		int l = strlen(f)
		wbuf = (xchar*)realloc(wbuf, sizeof(xchar) * (l+1))
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0
		return _waccess(wbuf, mode)
	} else 
		return _access(fl_utf2mbcs(f), mode)
	
#elif __MACOS_
	return access(fl_utf2path(f), mode)
#els
	return access(f, mode)
#endi



int fl_stat(const char* f, struct stat *b

#ifdef WIN3
	if (fl_is_nt4()) 
		int l = strlen(f)
		wbuf = (xchar*)realloc(wbuf, sizeof(xchar) * (l+1))
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0
		return _wstat(wbuf, (struct _stat*)b)
	} else 
		return _stat(fl_utf2mbcs(f), (struct _stat*)b)
	
#elif __MACOS_
	char bu[1024]
	f = fl_utf2path(f)
	if (!f || f[0] == 0 || (f[0] == ':' && f[1] == 0)) 
		 // stat a random director
		fl_getcwd(bu, 1024)
		f = bu
	
	return stat(f, b)
#els
	return stat(f, b)
#endi


char *fl_getcwd(char* b, int l

	if (b == NULL) 
		b = (char*) malloc(l+1)
	
#ifdef WIN3
	if (fl_is_nt4()) 
		static xchar *wbuf = NULL
		wbuf = (xchar*)realloc(wbuf, sizeof(xchar) * (l+1))
		xchar *ret = _wgetcwd(wbuf, l / 5)
		if (ret) 
			l = wcslen(wbuf)
			b[fl_unicode2utf(wbuf, l, b)] = 0
			return b
		} else 
			return NULL
		
	} else 
		char *ret = _getcwd(b, l / 5)
		if (ret) 
			char *s = fl_mbcs2utf(b)
			l = strlen(s)
			for(int i= 0; i < l; i++) 
				b[i] = s[i]
			
			b[l+1] = 0
			return b
		} else 
			return NULL
		
	
#elif __MACOS_
	char *ret = getcwd(b, l / 5)
	if (ret) 
		char *s = fl_mbcs2utf(b)
		l = strlen(s)
		b[0] = '/'
		for(int i= 0; i < l; i++) 
			if (s[i] == '/') 
				b[i+1] = ':'
			} else if (s[i] == ':') 
				b[i+1] = '/'
			} else 
				b[i+1] = s[i]
			
		
		b[l+1] = 0
		return b
	} else 
		return NULL
	
#elif MSDO
	b[0] = 0
	return b
#els
	return getcwd(b, l)
#endi



int fl_unlink(const char* f

#ifdef WIN3
	if (fl_is_nt4()) 
		int l = strlen(f)
		wbuf = (xchar*)realloc(wbuf, sizeof(xchar) * (l+1))
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0
		return _wunlink(wbuf)
	} else 
		return _unlink(fl_utf2mbcs(f))
	
#elif __MACOS_
	return unlink(fl_utf2path(f))
#els
	return unlink(f)
#endi


int fl_mkdir(const char* f, int mode

#if defined(WIN32) && !defined(__CYGWIN__
	if (fl_is_nt4()) 
		int l = strlen(f)
		wbuf = (xchar*)realloc(wbuf, sizeof(short) * (l+1))
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0
		return _wmkdir(wbuf)
	} else 
		return _mkdir(fl_utf2mbcs(f))
	
#elif __MACOS_
	return mkdir(fl_utf2path(f), mode)
#elif MSDO
	return -1
#els
	return mkdir(f, mode)
#endi



int fl_rmdir(const char* f

#ifdef WIN3
	if (fl_is_nt4()) 
		int l = strlen(f)
		wbuf = (xchar*)realloc(wbuf, sizeof(xchar) * (l+1))
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0
		return _wrmdir(wbuf)
	} else 
		return _rmdir(fl_utf2mbcs(f))
	
#elif __MACOS_
	return rmdir(fl_utf2path(f))
#els
	return rmdir(f)
#endi


/
// End of "$Id$"
/

