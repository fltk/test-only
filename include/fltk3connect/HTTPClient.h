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
    
    /** Copy of the host name (not uesd yet, but needed if we keep the 
     connection alive). */
    char *pHost;
    
    /** A block of memory that will hold the entire payload. A trailing NUL is
     added to the buffer at all times. */
    void *pFile;
    
    /** Pointer into pFile, marking the start of the last receive chunk */
    void *pChunk;
    
    /** Current size of pFile. */
    int pFileSize;
    
    /** Size of the last chunk that was received. */
    int pChunkSize;
    
    /** This is set if the attributes indicate a payload in chunks. */
    char pIsChunkedTransfer;
    
    /** This is set if the payload immediatly follows the header. */
    char pIsInlineTransfer;
    
    /** The number of bytes we expect as a payload. */
    int pExpectedDataSize;
    
    /** Possible states of the HTTP transfer. */
    typedef enum { // State
      WAITING_FOR_HEADER,     ///< wait until the first line is received ("HTTP/1.1...")
      WAITING_FOR_ATTRIBUTES, ///< wait until the header is received entirley
      WAITING_FOR_CHUNKS,     ///< wait until the next chunk is received
      WAITING_FOR_INLINE_DATA,///< wait until the entire payload is received
      HTTP_CLOSED,            ///< the connection is closed
      HTTP_COMPLETE,          ///< we received al expected data; all following data is ignored and the connection will be closed
      HTTP_ERROR,             ///< FIXME: some error occured
      HTTP_UNSUPPORTED        ///< FIXME: we don't know how to react yet
    } TState;
    
    /** The state of the HHTP transfer. Not to be confused with status()
     which indicates the state of the TCP connection. */
    TState pState;
    
    /** Connect to a host. Hostname may be followed by a ':' and a port number.
     This is currently protected because we disconnect after every transfer. */
    int connect(const char *host);
    
    /** Release the memory allocated by pFile. */
    void free_file();
    
    /** Add a chunk of data to pFile. */
    void add_chunk(void *chunk, int size=-1);
    
    /** Handle every incoming block of data based on the curren pState. */
    char on_receive();
    
    /** Handle end of connection. */
    char on_close();
    
    /** Clear all flags associated with the attribute part of the header. */
    void clear_attributes();
    
    /** Read all attributes and set the flags accordingly. */
    char *read_attributes(char *src, char *last);
    
    /** Find the next EOL (\r\n) and return the start of the next line. */
    char *find_end_of_line(char *src);
    
    /** Find the end of the header (\r\n\r\n) or NULL if there is none. */
    char *find_end_of_header(char *src);
    
    void do_on_chunk_received();
    
    void do_on_file_received();
    
    void do_on_command_aborted();

  public:
    
    /** POssible reasons for calling the callback. Use fltk3::event() to read. */
    enum { // Event
      FILE_RECEIVED = 256,  ///< The complete file is available in pFile
      HEADER_RECEIVED,      ///< A message header was received (see attributes), (on_header_received must be overridden to return 0)
      CHUNK_RECEIVED,       ///< A chunk of data was received (in pChunk), more to follow (on_chunk_received must be overridden to return 0)
      COMMAND_ABORTED       ///< Somehow the transmission was aborted
    };


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
     \return -1 for a connection error, or 0. (404 = not found, etc.)
     */
    int GET(const char *server, const char *filename);
    
    /**
     Get a file using the full URL as a path.
     */
    int GET(const char *path);
    
    /**
     Pointer to the block of data that was received in the last GET transaction.
     
     \return NULL if an error occured
     */
    void *file_data() { return pFile; }
    
    /**
     Size of last data transfer.
     
     \return -1 if an error occured, -2 if no data was received yet, or the 
            number of bytes received
     */
    int file_size() { return pFileSize; }

    /**
     This function is called if a chunk of bytes is received.
     
     This method can be overridden by the user as an alternative to setting a callback.
     
     At this point, the file is not complete. The data received so far is in
     pFile and pFileSize, the chunk is at pChunk and pChunkSize.
     
     \return 0 if the callback should be called, 1 (default) if not.
     */
    virtual char on_chunk_received();
    
    /**
     This function is called after a complete file is received.

     This method can be overridden by the user as an alternative to setting a callback.
     
     The data received is in pFile and pFileSize.
     
     \return 0 (default) if the callback should be called, 1 if not.
     */
    virtual char on_file_received();

    /**
     This function is called if the data transfer was interrupted for some reason.
     
     This method can be overridden by the user as an alternative to setting a callback.
     
     An error message is put into pFile and pFileSize.
     
     \return 0 (default) if the callback should be called, 1 if not.
     */
    virtual char on_command_aborted();
  };
  
}


#endif

//
// End of "$Id$".
//
