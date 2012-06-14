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

#ifndef Fltk3_FTP_Connection_H
#define Fltk3_FTP_Connection_H

/* \file
 fltk3::FTPClient widget . */

#if 0

#include <fltk3connect/TCPSocket.h>


struct timespec;


namespace fltk3 {
  
  class FTPClient;
  
  /**
   For internal use.
   \todo This class definition should probably be moved inside FTPClient.
   */
  class FTPCommandSocket : public TCPSocket
  {
  public:
    FTPCommandSocket(int x, int y, int w, int h, const char *l, FTPClient *ftp);
  protected:
    virtual char on_receive();
    FTPClient *pFTP;
  };
  
  
  /**
   For internal use.
   \todo This class definition should probably be moved inside FTPClient.
   */
  class FTPDataSocket : public TCPSocket
  {
  public:
    FTPDataSocket(int x, int y, int w, int h, const char *l, FTPClient *ftp);
  protected:
    virtual char on_receive();
    FTPClient *pFTP;
  };
  

  /**
   This widget manages a basic FTP connection as a client.
   The current implementation is focused on synchronizing a local directory to
   a remote server.
   \todo There are still many commands not implemented.
   \todo Error handling counld be much better.
   */
  class FTPClient : public Group
  {
    friend class FTPCommandSocket;
    friend class FTPDataSocket;
    
  public:
    FTPClient(int x, int y, int w, int h, const char *l);
    virtual ~FTPClient();
    
    void message(const char *pattern, ...);
    
    int open(unsigned char ip0, unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned short port, const char *name, const char *passwd);
    int close();
    int get(const char *filename, void *&data, unsigned int &size);
    int put(const char *filename, void *data, unsigned int size);
    int dir(const char *pathname); // should return some structure?!
    int mkdir(const char *pathname);
    int abort();
    int status();
    
    int sync_open(unsigned char ip0, unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned short port, const char *name, const char *passwd);
    int sync_open(const char *host, const char *name, const char *passwd);
    int sync_close();
    int sync_get_time(const char *serverfile, struct timespec &timespec);
    int sync_send_file(const char *clientfile, const char *serverfile=0);
    int sync_send_file_if_newer(const char *clientfile, const char *serverfile=0);
    int sync_wait();
    int sync_pwd();
    int sync_chdir(const char *serverpath);
    int sync_mkdir(const char *serverpath);
    int sync_send_dir(const char *clientpath, const char *serverpath);
    // int abort();
    
    void draw();
    void resize(int x, int y, int w, int h);
    /*
     int send_file(const char *srcname, const char *dstname=0);
     int get_creation_time(const char *filename, struct timespec &t);
     int get_current_dir();
     int change_dir(const char *subdir);
     int up_dir();
     */
    
  protected:
    void my_resize(int x, int y, int w, int h);
    
    char on_receive_command();
    char on_receive_data();
    
    FTPCommandSocket pCommand;
    FTPDataSocket pData;
    End pEnd;
    char *pName, *pPasswd, *pServerDir, *pLastReply, *pMsg;
    int pStatus;
    enum {
      FTP_ERROR,
      FTP_SYNC,
      FTP_CLOSED,
      FTP_CONNECTING,
      FTP_SENDING_NAME,
      FTP_SENDING_PASS,
      FTP_SETTING_TYPE,
      FTP_SET_PASSIVE,
      FTP_CONNECTED,
    };
  };
  
  
} // namespace

#endif

#endif

//
// End of "$Id$".
//
