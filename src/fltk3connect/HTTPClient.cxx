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
#include <fltk3/run.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "../fltk3/flstring.h" // snprintf


static char GET_request[] = "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n";


/*
 Initialize all variables
 */
fltk3::HTTPClient::HTTPClient(int x, int y, int w, int h, const char *l)
: fltk3::TCPSocket(x, y, w, h, l),
  pHost(0),
  pFile(0L),
  pFileSize(0),
  pState(HTTP_CLOSED)
{
}


/*
 Return all resources.
 */
fltk3::HTTPClient::~HTTPClient()
{
  free_file();
  if (pHost)
    free(pHost);
}


/*
 Return the resources associated with the last file received.
 */
void fltk3::HTTPClient::free_file()
{
  if (pFile) {
    free(pFile);
    pFile = 0L;
  }
  pFileSize = 0;
  pChunk = 0L;
  pChunkSize = 0;
}


/*
 Add a block of data to the current pFile.
 */
void fltk3::HTTPClient::add_chunk(void *chunk, int chunk_size)
{
  if (chunk_size==-1)
    chunk_size = strlen((char*)chunk);
  pFile = realloc(pFile, pFileSize+chunk_size+1);
  memcpy(((char*)pFile)+pFileSize, chunk, chunk_size);
  pChunk = ((char*)pFile)+pFileSize;
  pChunkSize = chunk_size;
  pFileSize += chunk_size;
  ((char*)pFile)[pFileSize] = 0; // trailing 'NUL' for convenience
}


/*
 Separate the host name into host and port. If no port is set, use the default
 HTTP port 80.
 */
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


static void awake_cb(fltk3::Widget*, void*) 
{
  fltk3::awake(); // make sure that the event loop ends
}


int fltk3::HTTPClient::GET(const char *url)
{
  int ret = 0;
  if (url) {
    if (strncmp(url, "http://", 7)==0) 
      url+=7;
    char *buf = strdup(url);
    char *path = strchr(buf, '/');
    if (path) {
      *path++ = 0;
    } else {
      path = (char*)"";
    }
    ret = GET(buf, path);
    free(buf);
  }
  return ret;
}


/*
 This function may do a long busy-wait! Use the async method instead!
 */
int fltk3::HTTPClient::GET(const char *server, const char *filename, void *&data, int &size)
{
  fltk3::CallbackPtr cb = callback();
  callback(awake_cb);
  int ret = GET(server, filename);
  if (ret==-1) {
    callback(cb);
    return ret;
  }
  for (;;) {
    fltk3::wait();
    if (pState>=HTTP_CLOSED)
      break;
  }
  callback(cb);
  if (callback()) {
    do_callback();
  }
  data = pFile;
  size = pFileSize;
  return pState;
}


/*
 Asynchronously grab a file from an HTTP server.
 */
int fltk3::HTTPClient::GET(const char *server, const char *filename)
{
  // prepare for return
  int ret = -1;
  pState = HTTP_CLOSED;
  free_file();

  // connect to the server
  if (status()==CLOSED) {
    ret = connect(server);
    if (ret==-1) {
      // TODO: improve error handling
      return ret;
    }
  }
  
  // remove a leading '/' because we will add a mandatory '/' later
  if (filename[0]=='/') filename++;
  
  // prepare and send the request to the server
  int len = sizeof(GET_request)+strlen(server)+strlen(filename)+1;
  char *msg = (char*)malloc(len);
  snprintf(msg, len, GET_request, filename, server);
  ret = send(msg);
  free(msg);
  
  // tell the state engine that we now expect a response
  pState = WAITING_FOR_HEADER;
  ret = 0;  
  return ret;
}


char fltk3::HTTPClient::on_chunk_received()
{
  // signal the class, that we don;t want the callback called by default
  return 1;
}


char fltk3::HTTPClient::on_file_received()
{
  return 0;
}


char fltk3::HTTPClient::on_command_aborted()
{
  return 0;
}


/*
 Clear all attributes.
 */
void fltk3::HTTPClient::clear_attributes()
{
  pIsChunkedTransfer = 0;
}


/*
 Find the next CR/LF and return the beginning of the next line.
 */
char *fltk3::HTTPClient::find_end_of_line(char *src)
{
  char *eol = strstr(src, "\r\n");
  if (eol) {
    return eol+2;
  } else {
    return 0L;
  }
}


/*
 Find the next CR/LF/CR/LF and return the beginning of the next line.
 */
char *fltk3::HTTPClient::find_end_of_header(char *src)
{
  char *eol = strstr(src, "\r\n\r\n");
  if (eol) {
    return eol+4;
  } else {
    return 0L;
  }
}


/*
 1.Any response message which "MUST NOT" include a message-body (such as the 1xx, 204, and 304 responses and any response to a HEAD request) is always terminated by the first empty line after the header fields, regardless of the entity-header fields present in the message.
 
 2.If a Transfer-Encoding header field (section 14.41) is present and has any value other than "identity", then the transfer-length is defined by use of the "chunked" transfer-coding (section 3.6), unless the message is terminated by closing the connection.
 
 3.If a Content-Length header field (section 14.13) is present, its decimal value in OCTETs represents both the entity-length and the transfer-length. The Content-Length header field MUST NOT be sent if these two lengths are different (i.e., if a Transfer-Encoding
 
 header field is present). If a message is received with both a
 Transfer-Encoding header field and a Content-Length header field,
 the latter MUST be ignored.
 4.If the message uses the media type "multipart/byteranges", and the transfer-length is not otherwise specified, then this self- delimiting media type defines the transfer-length. This media type MUST NOT be used unless the sender knows that the recipient can parse it; the presence in a request of a Range header with multiple byte- range specifiers from a 1.1 client implies that the client can parse multipart/byteranges responses.
 
 A range header might be forwarded by a 1.0 proxy that does not
 understand multipart/byteranges; in this case the server MUST
 delimit the message using methods defined in items 1,3 or 5 of
 this section.
 5.By the server closing the connection. (Closing the connection cannot be used to indicate the end of a request body, since that would leave no possibility for the server to send back a response.)
 
 For compatibility with HTTP/1.0 applications, HTTP/1.1 requests containing a message-body MUST include a valid Content-Length header field unless the server is known to be HTTP/1.1 compliant. If a request contains a message-body and a Content-Length is not given, the server SHOULD respond with 400 (bad request) if it cannot determine the length of the message, or with 411 (length required) if it wishes to insist on receiving a valid Content-Length.
 
 All HTTP/1.1 applications that receive entities MUST accept the "chunked" transfer-coding (section 3.6), thus allowing this mechanism to be used for messages when the message length cannot be determined in advance.
 
 Messages MUST NOT include both a Content-Length header field and a non-identity transfer-coding. If the message does include a non- identity transfer-coding, the Content-Length MUST be ignored.
 
 When a Content-Length is given in a message where a message-body is allowed, its field value MUST exactly match the number of OCTETs in the message-body. HTTP/1.1 user agents MUST notify the user when an invalid length is received and detected.
 */
char *fltk3::HTTPClient::read_attributes(char *src, char *last)
{
  // loop through all lines
  for (;;) {
    // find important places in this line
    char *sep = strchr(src, ':');
    char *eol = find_end_of_line(src);
    if (!eol)
      break;
    
    if (strncmp(src, "Transfer-Encoding:", 18)==0) {
      if (strncmp(sep+2, "chunked", 7)==0) { // or "identity"
        pIsChunkedTransfer = 1;
      }
    }
    // or "Content-Length", followed by the number of bytes in decimal
    
    src = eol;
    if (src>=last) 
      break;
  }
  return src;
}


/*
 The HTTP state machine for asynchronously receiving messages.
 */
char fltk3::HTTPClient::on_receive()
{
  int n;
  char *data = 0L;
  //puts("----- on receive");
  while ( (n=available()) ) { 
    //printf("----- %d bytes available\n", n);
    if (data) free(data);
    data = (char*)malloc(n+1);
    peek(data, n); // leave the data in the buffer!
    data[n] = 0;
    
    if (pState==WAITING_FOR_HEADER) 
    {
      // we need an entire line before we can do anything
      char *eol = find_end_of_line(data);
      if (!eol)
        break;
      // enough data, but is it an HTTP header?
      if (strncmp(data, "HTTP/1.1 ", 9)!=0) {
        pState = HTTP_ERROR;
        add_chunk((void*)"\nUnsupported HTTP reply.\n");
        do_on_command_aborted();
        close();
        pState = HTTP_CLOSED;
        break;
      }
      // good, so we have an eror code and message next
      int err = atoi(data+9);
      if (err==200) { // OK: payload follows
        flush_input(eol-data);
        pState = WAITING_FOR_ATTRIBUTES;
      } else {
        // FIXME: Handle more result codes!
        pState = HTTP_UNSUPPORTED;
        add_chunk(data+9, eol-data-9-2);
        add_chunk((void*)"\n(result code unsupported by fltk3::HTTPClient)\n");
        do_on_command_aborted();
        close();
        pState = HTTP_CLOSED;
        break;
      }
    } 
    else if (pState==WAITING_FOR_ATTRIBUTES) 
    {
      // we need the entire header before we continue
      char *eoh = find_end_of_header(data);
      if (!eoh)
        break;
      // read all the attribute that we are intersetd in
      read_attributes(data, eoh-2);
      // decide what we do next, based on the attributes
      if (pIsChunkedTransfer) {
        flush_input(eoh-data);
        pState = WAITING_FOR_CHUNKS;
      } else {
        pState = HTTP_UNSUPPORTED;
        add_chunk((void*)"\nfltk3::HTTPClient: unsupported combination of attributes in reply\n");
        do_on_command_aborted();
        close();
        pState = HTTP_CLOSED;
        break; //throw 0L;
      }
    } 
    else if (pState==WAITING_FOR_CHUNKS) 
    {
      // we need the chunk header to find the chunk size
      char *eol = find_end_of_line(data);
      if (!eol) 
        break;
      // now get the chunk size
      int chunk_size = 0;
      sscanf(data, "%x", &chunk_size);
      int data_required = (eol-data)+chunk_size+2;
      if (n<data_required) 
        break;
      flush_input(data_required);
      if (chunk_size) {
        add_chunk(eol, chunk_size);
        do_on_chunk_received();
      } else {
        pState = HTTP_COMPLETE;
        do_on_file_received();
        close();
        pState = HTTP_CLOSED;
        break;
      }      
    } else {
      flush_input();
    }
  }
  if (data) free(data);
  return 1;
}


char fltk3::HTTPClient::on_close()
{
  // TODO: depending on the state, we can now copy whatever is still in the buffer into the file
  pState = HTTP_COMPLETE;
  return 0;
}


void fltk3::HTTPClient::do_on_file_received()
{
  char handled = on_file_received();
  if (!handled && callback()) {
    fltk3::e_number = FILE_RECEIVED;
    do_callback();
  }
}


void fltk3::HTTPClient::do_on_command_aborted()
{
  char handled = on_command_aborted();
  if (!handled && callback()) {
    fltk3::e_number = COMMAND_ABORTED;
    do_callback();
  }
}


void fltk3::HTTPClient::do_on_chunk_received()
{
  char handled = on_chunk_received();
  if (!handled && callback()) {
    fltk3::e_number = CHUNK_RECEIVED;
    do_callback();
  }
}


//
// End of "$Id$".
//
