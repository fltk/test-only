//
// "$Id$"
//
// FLTK3 connect library source file for the Fast Light Tool Kit (FLTK).
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

#include <fltk3connect/HTTPClient.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/*
 Sample data exchange:
 
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

fltk3::HTTPClient::HTTPClient(int x, int y, int w, int h, const char *l)
: fltk3::TCPSocket(x, y, w, h, l),
  pHost(0)
{
}


fltk3::HTTPClient::~HTTPClient()
{
  if (pHost)
    free(pHost);
}


int fltk3::HTTPClient::connect(const char *host)
{
  int ret = -1;
  if (pHost) {
    free(pHost);
    pHost = 0;
  }
  if (host) {
    const char *cln = strchr(host, ':');
    if (cln) {
      ret = TCPSocket::connect(host);
    } else {
      ret = TCPSocket::connect(host, 80);
    }
    pHost = strdup(host);
  }
  return ret;
}


int fltk3::HTTPClient::GET(const char *server, const char *filename, void *&data, int &size)
{
  int ret = -1;
#if 0
  if (!filename || !*filename) 
    filename = "index.html";
  
  int size = 32+strlen(filename)+strlen(pHost);
  char *msg = (char*)malloc(size);
  snprintf(msg, size, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", filename, pHost);
  send(msg);
  
  // sync mode for testing: simply wait indefinetly for a reply
  char buf[10000]; buf[0] = 0;
  
  // grab the header
  int n = recv(buf, 10000);
  if (strncmp(buf, "HTTP", 4)!=0) {
    return ret;
  }
  if (strncmp(buf+4, "/1.1 ", 5)!=0) {
    return ret;
  }
  int err = atoi(buf+9);
  printf("ERROR: %d\n", err);
  
  char *len_key = strstr(buf, "Content-Length:");
  char *data_key = strstr(buf, "\r\n\r\n");
  if (len_key && data_key) {
    int len = atoi(len_key+15);
    const char *data = (char*)malloc(len);
    //int nd = n-data_key+4;
    //memcpy(data, data_key+4, n);    
    //free(data);//
  }
  puts(buf);
#endif  
  return ret;
}


int fltk3::HTTPClient::GET(const char *server, const char *filename)
{
  int ret = -1;
#if 0
  // empty
#endif  
  return ret;
}



//
// End of "$Id$".
//
