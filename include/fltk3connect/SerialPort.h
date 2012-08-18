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

#ifndef FLTK3_SERIAL_PORT_H
#define FLTK3_SERIAL_PORT_H

#include <fltk3connect/Socket.h>

/* \file
 fltk3::SerialPort widget . */

namespace fltk3 {

/**
 * A serial port communication widget for FLTK3.
 *
 * This widget manages communication over a serial port
 * by providing a send and receive callback mechanism.
 * It draws indicators of the connection state, but the
 * Widget may as well stay hidden or not be part of a widget
 * tree at all.
 *
 * Line indicators are two LED-style circles that are gray
 * if the connection is closed, green if there is no data
 * sent or received, and red if there is traffic.
 *
 * We are using a 2kByte ring buffer to implement buffering.
 * If that seems to be too little, NRing in the constructor
 * can be changed accordingly.
 *
 * \todo We have yet to stress test the threading mechanism for multiple open/close operations!
 * \todo No buffer should be allocated until we actually open the line
 * \todo We should implement some better error handling
 * \todo We should provide an error message
 * \todo We could provide a tooltip indicating the number of bytes transmitted
 */
class SerialPort : public Socket
{
public:
  
  /**
   * Standard widget interface constructor.
   */
  SerialPort(int X, int Y, int W, int H, const char *L=0L);
  
  /**
   * Constructor for superwidgets.
   */
  SerialPort(Socket *super);
  
  /**
   * The destructor closes any open connections.
   */
  virtual ~SerialPort();
  
  /**
   * Open a connection.
   *
   * All connections are opened with 8N1 for simplicity.
   *
   * \param[in] OS-specific port name, for example "\\.\COM1", or "/dev/ttyS0"
   * \param[in] transfer rate in bits per second (38400bps)
   * \return 0 for success, -1 if failed
   */
  virtual int open(const char *port, int bps);
  
  /**
   * Write a block of binary data.
   *
   * This function returns immediatly.
   *
   * \param data[in] address of data block to send
   * \param n[in] size of data block
   * \return -1 if failed
   */
  int write(const unsigned char *data, int n);
  
  /**
   * Return the number of bytes available.
   *
   * \return number of bytes available in the buffer
   */
  virtual int available() const;
  
  /**
   * Read bytes from the buffer.
   *
   * This function reads at most n bytes from its internal
   * buffer and makes room for new bytes to receive.
   *
   * \param dest[in] address of memory destination
   * \param n[in] number of bytes to read at max
   * \return number of bytes actually read
   */
  int read(unsigned char *dest, int n);
  
  /**
   * Close the serial connection.
   */
  virtual void close();
  
  /**
   * Check if the serial line is open.
   *
   * \return 0 if the connection is closed
   */
  int is_open();
  
  /**
   * This will be called whenever data arrives.
   *
   * Alternative interface to callbacks. Available bytes
   * are returned by available() and can then be read using
   * read().
   *
   * \return return 1, if callback should not be called anymore
   */
  virtual int on_read();
  
  //virtual int on_error();
  
  virtual void draw();
  void redraw();
  
protected:
  
  virtual int on_read_();
  //virtual int on_error_();
  
private:
  
  static void on_read_cb(void *);
  //static void on_error_cb(void *);
  static void lights_cb(void *);
  
  unsigned char *ring_;
  int NRing_;
  int ringHead_;
  int ringTail_;
  char *portname_;
  char rxActive_, pRxActive_;
  char txActive_, pTxActive_;
  
  int available_to_end();
  int free_to_end();
  
#ifdef WIN32
  void reader_thread();
  static void __cdecl reader_thread_(void*);
  HANDLE port_;
  HANDLE event_;
  unsigned long thread_;
  OVERLAPPED overlapped_;
#else
  int port_;
  static void reader_cb(int, void*);
  void reader();    
#endif
};

}; // namespace

#endif

//
// End of "$Id$".
//

