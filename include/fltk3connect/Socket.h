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

#ifndef Fltk3_Socket_H
#define Fltk3_Socket_H

/* \file
 fltk3::Socket widget . */

#include <fltk3/Group.h>

#ifdef WIN32
# include <winsock2.h>
#endif


namespace fltk3 {
  
  /**
   \brief A base class for TCP, Serial, or USB connections.

   Connections via networks or external ports tend to be much slower than
   most internal operations in a PC. The fltk3connect library manages 
   connections to external devices or other computers in a transparent and
   non-blocking manner.
   
   Connections can be managed via callbacks or by overriding the on_...
   methods in this class.
   
   
   All sockets in the fltk3connect library are implemented as widgets. They can
   be added to a GUI to show connection and transfer state. Invisible sockets
   and sockets that are not added to a GUI still provide full functionality
   minus visualisation.
   
   This class is derived from fltk3:Group because some sockets need multiple
   connections (FTP for example) which will be added as children.
   
   \todo There are still a lot of useful features missing in this class.
   */
  class Socket : public Group
  {
    
  protected:
    
    int pStatus;
    int pActive;
    double pFirstByteTimeout;
    double pNextByteTimeout;
    
    void draw();
    
  public:
    
    enum { // Status
      CLOSED, LISTENING, VERIFYING, CONNECTED
    };
    
    enum { // Event
      CONNECT=128, RECEIVE, TIMEOUT, CLOSE
    };
    
    enum Trigger {
      RECV_ANY_OF,  // followed by a string of bytes, any of them triggering the event
      RECV_TEXT,    // followed by a string that needs to be matched
      RECV_MIN      // minimum bytes needed to trigger event (followed by an int)
    };
    
    /**
     Create a widget that manages an outside world connection.
     
     This widget manages a network connection, displaying a graphic 
     representation of the connection state. If no visual feedback is 
     required, this widget can be hidden or not be put into the widget 
     hierarchy at all.
     */
    Socket(int x, int y, int w, int h, const char *l);
    
    /**
     Disconnect and return resources
     */
    virtual ~Socket();
    
    /**
     Connect to another side using a text to describe the connection parameters.
     
     \param device describes the connection; the format depends largely on the 
            connection class
     \return 0 for success, -1 for error
     */
    virtual int connect(const char *device);
    
    /**
     Send a block of data over the connection.
     
     Depending on the base class, this call sneds bytes, or a block, or adds 
     the data to some send buffer.
     
     \param data the bytes we want to send
     \parem size number of bytes to send
     \return number of bytes sent, or -1 for error
     */
    virtual int send(const void *data, int size);
    
    /**
     Send some 'NUL' terminated text.
     
     Send an ASCII or UTF-8 string of text as a sequence of bytes or as a 
     single block of data.
     
     \param text the text we want to send
     \return the number of bytes sent, or -1 for error
     */
    virtual int send(const char *text);
    
    /**
     Receive data until the buffer is filled or a timout occurs.
     
     This is a possibly blocking call!
     
     \param data a buffer that will hold the received data
     \param size maximum number of bytes to receive
     \return number of bytes received, or 0 for timeout, or -1 for error
     */
    virtual int recv_all(void *data, int size);
    
    /**
     Receive data until the buffer is filled or a data block ends.
     
     This is a possibly blocking call!
     
     \param data a buffer that will hold the received data
     \param size maximum number of bytes to receive
     \return number of bytes received, or 0 for timeout, or -1 for error
     */
    virtual int recv(void *data, int size);
    
    /**
     Receive the next block of data, but don't remove it from the queue.
     
     This is a possibly blocking call!
     
     \param data a buffer that will hold the received data
     \param size maximum number of bytes to receive
     \return number of bytes put into buffer, or 0 for timeout, or -1 for error
     */
    virtual int peek(void *data, int size);
    
    /**
     Close a server or client connection.
     
     Some protocols will notify the other side and cause an on_close call or 
     CLOSE callback there.
     */
    virtual void close();
    
    /**
     Override this method to get notified whenever a client attempts to connect.

     \return 1 if the event was handled, or 0 if it should be resent to the callback
     */
    virtual char on_connect();
    
    /**
     Override this method to get notified whenever the connection is terminated.
     
     \return 1 if the event was handled, or 0 if it should be resent to the callback
     */
    virtual char on_close();
    
    /**
     Override this method to get notified whenever the connection times out.
     
     \return 1 if the event was handled, or 0 if it should be resent to the callback
     */
    virtual char on_timeout();
    
    /**
     Override this method to get notified whenever data arrives.
     
     \return 1 if the event was handled, or 0 if it should be resent to the callback
     */
    virtual char on_receive();
    
    /**
     Set the kind of event that will trigger an on_receive.
     
     \param trigger the kind of event that triggers the receive action
     \return -1 for error
     */
    int monitor(int trigger, ...);      
    
    /**
     Return the current state of the connection.
     
     In an asynchronous environment, communication can be implemented using 
     a state machine. This call help with the implementation.
     */
    int status() const;
    
    /**
     Get the number of bytes available in the buffer.
     
     A call to recv or recv_all will not block if it is called with the
     number of bytes returned by this call.
     
     \return number of bytes buffered or -1 for error
     */
    virtual int available() const;
    
    /**
     Flush a number of bytes from the receive buffer.
     
     \param n some devices allow to flush only some maximum number of bytes; if
            n is -1, the entire buffer will be flushed.
     */
    virtual void flush_input(int n=-1);
    
    /**
     Flush the send buffer.
     */
    virtual void flush_output();
    
    /**
     Set a per-byte timeout and a per-block timeout.
     
     The parameters in the call are used to set the timouts for possibly 
     blocking calls such as recv and recv_all.
     
     \param first_byte wait for the given number of seconds for the first byte 
            in a block
     \param next_byte wait for the given number of seconds for all further bytes
            in a block
     */
    void timeout(double first_byte, double next_byte);
    
  };
  
  
} // namespace

#endif

//
// End of "$Id$".
//
