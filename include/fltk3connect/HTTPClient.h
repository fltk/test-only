//
// "$Id$"
//
// FLTK3 connect library header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2002-2012 by Matthias Melcher and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#ifndef FLTK3_HTTP_CLIENT_H
#define FLTK3_HTTP_CLIENT_H

/* \file
 fltk3::fltk3::HTTPClient widget . */

#include <fltk3connect/TCPSocket.h>

#include <string.h>

namespace fltk3 {

/**
 This class implements an HTTP client widget.
 It can be used to manage network connections to HTTP servers. This is great 
 for fetching web pages and other resources from the world wide web.
 
 port 80 (or 8080, etc)
 
 request (header, body)
    "GET /index.html HTTP/1.1\r\nHost: www.example.net\r\n\r\n"
 
 response (header, body)
    "HTTP/1.1 200 OK\r\n"
    "Server: Apache/1.3.29 (Unix) PHP/4.3.4\r\n"
    "Content-Length: 1024\r\n"
    "Content-Language: en\r\n"
    "Connection: close\r\n"
    "Content-Type: text/html\r\n"
    "\r\n" - binary data follows
 */
  class HTTPClient : public TCPSocket
  {
    
  protected:
    
    char *pHost;
    
  public:
    
    HTTPClient(int x, int y, int w, int h, const char *label=0L);
    ~HTTPClient();
    int connect(const char *host);
    int GET(const char *filename);
    
  };
  
}


#endif

//
// End of "$Id$".
//
