//
// "$Id: win32.h,v 1.27 2004/07/27 07:03:07 spitzak Exp $"
//
// _WIN32 header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
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

// Do not directly include this file, instead use <fltk/x.h>.  It will
// include this file if _WIN32 is defined.  This is to encourage
// portability of even the system-specific code...

#ifndef FL_WIN32_H
#define FL_WIN32_H

#include <windows.h>
#include <winuser.h>
// In some of the distributions, the gcc header files are missing some stuff:
#ifndef LPMINMAXINFO
# define LPMINMAXINFO MINMAXINFO*
#endif
#ifndef VK_LWIN
# define VK_LWIN 0x5B
# define VK_RWIN 0x5C
# define VK_APPS 0x5D
#endif

extern "C" {
	
// Function pointer declarations

typedef HWND (WINAPI *pfCreateWindowExW)(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
typedef HMODULE (WINAPI *pfLoadLibraryW)(LPCWSTR lpFileName);
typedef BOOL (WINAPI *pfPeekMessage)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
typedef BOOL (WINAPI *pfGetMessage)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
typedef LRESULT (WINAPI *pfDispatchMessage)(const MSG *lpmsg);
typedef BOOL (WINAPI *pfSetWindowTextW)(HWND hWnd, LPCWSTR lpString);
typedef LRESULT (WINAPI *pfDefWindowProc)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
typedef BOOL (WINAPI *pfPostMessage)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
typedef int (WINAPI *pfMessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);

typedef HFONT (WINAPI *pfCreateFontIndirectW)(CONST LOGFONTW *);
typedef BOOL (WINAPI *pfGetTextMetricsW)(HDC, LPTEXTMETRICW);

extern pfCreateWindowExW __CreateWindowExW;
extern pfLoadLibraryW	 __LoadLibraryW;
extern pfPeekMessage	 __PeekMessage;
extern pfGetMessage	 __GetMessage;
extern pfDispatchMessage __DispatchMessage;
extern pfSetWindowTextW	 __SetWindowTextW;
extern pfDefWindowProc	 __DefWindowProc;
extern pfPostMessage	 __PostMessage;
extern pfMessageBoxW	 __MessageBoxW;

extern pfCreateFontIndirectW __CreateFontIndirectW;
extern pfGetTextMetricsW     __GetTextMetricsW;

}; /* extern "C" */

#ifdef __MINGW32__
// MinGW cannot convert ushort to wchar_t ...
# define US2WC(x) ((wchar_t*)x)
#else
# define US2WC(x) (x)
#endif

namespace fltk {

////////////////////////////////////////////////////////////////
// constant information about the display:

extern FL_API void	open_display();
extern FL_API void	close_display();
extern FL_API HINSTANCE	xdisplay;
extern FL_API HPALETTE	xpalette; // non-zero only on 8-bit displays!

////////////////////////////////////////////////////////////////
// event handling:

extern FL_API MSG msg;

////////////////////////////////////////////////////////////////
// drawing functions:

extern FL_API HDC	dc;
extern FL_API HDC	getDC();
extern FL_API HFONT	xfont();
extern FL_API TEXTMETRICW* textmetric();
extern FL_API COLORREF	current_xpixel;
extern FL_API COLORREF	xpixel(Color i);
extern FL_API HPEN	setpen();
extern FL_API HBRUSH	setbrush();
extern FL_API void	clip_region(HRGN);
extern FL_API HRGN	clip_region();

extern FL_API void	draw_into(HBITMAP);
extern FL_API void	stop_drawing(HBITMAP);
extern FL_API void	stop_drawing(HWND);

////////////////////////////////////////////////////////////////
#ifdef fltk_Window_h // only include this if <fltk/Fl_Window.h> was included

// When fltk tells X about a window, one of these objects is created.
// Warning: this object is highly subject to change!  It's definition
// is only here so that fl_xid can be declared inline:

class FL_API CreatedWindow {
public:
  HWND xid;
  HDC dc;
  HBITMAP backbuffer;
  HDC bdc;
  Window* window;
  HRGN region;
  void expose(int x, int y, int w, int h);
  CreatedWindow* next;
  bool wait_for_expose;
  bool backbuffer_bad; // used for XDBE
  bool overlay; // true if redraw_overlay was called
  HCURSOR cursor;
  const Widget* cursor_for;
  static CreatedWindow* first;
  static CreatedWindow* find(const Window* window) {return window->i;}
  static int borders(const Window* w, int& dx, int& dy, int& dw, int& dh);
  void set_minmax(LPMINMAXINFO minmax);
  static void create(Window*);
};

// convert xid <-> Window:
inline HWND xid(const Window*w) {return CreatedWindow::find(w)->xid;}
Window* find(HWND xid);

extern FL_API HCURSOR default_cursor;

#endif //Fl_Window_H
////////////////////////////////////////////////////////////////

}

#endif

//
// End of "$Id: win32.h,v 1.27 2004/07/27 07:03:07 spitzak Exp $".
//
