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
   This class implements an HTTP communication client.
   
   It can be used to manage network connections to HTTP servers. This is great 
   for fetching web pages and other resources from the world wide web.
   
   HTTP is a state-less protocol. Generally, every transfer implies a new 
   connection to a server, a request, a reply by the server, and a disconnect.
   There are options in the protocoll to keep a connection for multiple 
   transfers, but these are not in the scope of this class.
   
   It is possible to create multiple HTTPClient objects that access the same
   server simultaneously, which may or may not accelerate communications. Some
   leterature recommends four to six clients working in parallel.
   
   This class implements a minimal subset of the HTTP/1.1 standard.
   
   Implemented: GET
   
   Not Implemented: OPTIONS, HEAD, POST, PUT, DELETE, TRACE, CONNECT

   */
  class HTTPClient : public TCPSocket
  {
    
  protected:
    
    char *pHost;
    
    int connect(const char *host);
    
  public:
    
    /**
     Create a widget that communicates with a server using the HTTP protocol.
     
     This widget manages an HTTP network connection, displaying a graphic 
     representation of the connection state. If no visual feedback is 
     required, this widget may be hidden or not be put into the widget 
     hierarchy at all.
     */
    HTTPClient(int x, int y, int w, int h, const char *label=0L);
    
    /**
     Return resources.
     */
    ~HTTPClient();
    
    /**
     Get a file from an HTTP server.
     
     This method connects to an HTTP server and requests a file by name and 
     waits until the file is downloaded.
     
     \param server name of the HTTP server. An optional port number can be 
            appended using the ':' notation.
     \param filename the path and name of the file we want to download. A '/'
            is prepended to the path if needed.
     \param[out] data the class will allocate some memory to hold the 
            downloaded data; use free() to release memory. The allocated block
            receives a trailing uncounted 'NUL' for your convinience
     \param[out] size the size of the data block in bytes
     \return 1 for a connection error, or the HTTP error code 
            (404 = not found, etc.)
     */
    int GET(const char *server, const char *filename, void *&data, int &size);
    
    /**
     Get a file from an HTTP server asynchronously.
     
     This method connects to an HTTP server and requests a file by name. The
     downloaded file will trigger an on_chunk_receive, on_file_receive, 
     on_timeout, or the corresponding callbacks.
     
     \param server name of the HTTP server. An optional port number can be 
     appended using the ':' notation.
     \param filename the path and name of the file we want to download. A '/'
     is prepended to the path if needed.
     \return 1 for a connection error, or 0
     (404 = not found, etc.)
     */
    int GET(const char *server, const char *filename);
    
  };
  
}


#endif

//
// End of "$Id$".
//
