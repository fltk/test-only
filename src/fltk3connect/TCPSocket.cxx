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

#include <fltk3connect/TCPSocket.h>


#include <fltk3/run.h>
#include <fltk3/draw.h>

#ifdef WIN32
char fltk3::TCPSocket::wsaStartup = 0;
typedef int socklen_t;
#else
# include <unistd.h>
# include <strings.h>
# include <sys/types.h> 
# include <netinet/in.h> 
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netdb.h>
#endif

#if 0
# include <stdio.h>
# include <stdarg.h>
# define LOG(x) fprintf(stderr, x)
#else
# define LOG(x)
#endif


fltk3::TCPSocket::TCPSocket(int x, int y, int w, int h, const char *l)
: fltk3::Widget(x, y, w, h, l),
  pStatus(CLOSED),
  pActive(0),
#ifdef WIN32
  sListen(INVALID_SOCKET),
  sData(INVALID_SOCKET),
#else
  fdListen(-1),
  fdData(-1),
#endif
  pPort(-1)
{
  box(fltk3::THIN_DOWN_BOX);
  align(fltk3::ALIGN_LEFT);
  LOG("fltk3::TCPSocket: constructor\n");
}


fltk3::TCPSocket::~TCPSocket()
{
  LOG("fltk3::TCPSocket: destructor\n");
  close();
}


void fltk3::TCPSocket::close()
{
  LOG("fltk3::TCPSocket: close\n");
#ifdef WIN32
  if (sData!=INVALID_SOCKET) {
    LOG("fltk3::TCPSocket:   close sData\n");
    closesocket(sData);
    fltk3::remove_fd(sData);
    sData = INVALID_SOCKET;
  }
  if (sListen!=INVALID_SOCKET) {
    LOG("fltk3::TCPSocket:   close sListen\n");
    closesocket(sListen);
    fltk3::remove_fd(sListen);
    sListen = INVALID_SOCKET;
  }
#else
  if (fdData!=-1) {
    LOG("fltk3::TCPSocket:   close fdData\n");
    ::close(fdData);
    fltk3::remove_fd(fdData);
    fdData = -1;
  }
  if (fdListen!=-1) {
    LOG("fltk3::TCPSocket:   close fdListen\n");
    ::close(fdListen);
    fltk3::remove_fd(fdListen);
    fdListen = -1;
  }
#endif
  pStatus = CLOSED;
  pPort = -1;
  redraw();
}


int fltk3::TCPSocket::connect(unsigned char ip0, unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned short port)
{
  redraw();
#ifdef WIN32
  if (sData==INVALID_SOCKET)
#else
    if (fdData==-1)
#endif
    {
#ifdef WIN32
      if (!wsaStartup) {
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;
        wVersionRequested = MAKEWORD( 2, 2 );
        err = WSAStartup( wVersionRequested, &wsaData );
        if (err) {
          LOG("fltk3::TCPSocket: listen - WSAStartup failed!\n");
        }
        wsaStartup = 1;
      }
      pStatus = CLOSED;
      
      // create the socket
      LOG("fltk3::TCPSocket: connect - create socket\n");
      sData = socket(AF_INET, SOCK_STREAM, 0);
      if (sData==INVALID_SOCKET) {
        LOG("fltk3::TCPSocket: connect - can't open socket\n");
        return 0;
      }
#else
      // create the socket
      LOG("fltk3::TCPSocket: connect - create socket\n");
      fdData = socket(AF_INET, SOCK_STREAM, 0);
      if (fdData==-1) {
        LOG("fltk3::TCPSocket: connect - can't open socket\n");
        return 0;
      }
#endif
    }
  // try to connect to the other side
  LOG("fltk3::TCPSocket: connect\n");
  struct sockaddr_in host_addr;
  socklen_t addr_len = sizeof(host_addr);
  memset(&host_addr, 0, addr_len);
  host_addr.sin_family = AF_INET;
  host_addr.sin_port = htons(port);
  host_addr.sin_addr.s_addr = htonl((ip0<<24)|(ip1<<16)|(ip2<<8)|ip3);
#ifdef WIN32
  if (::connect(sData, (struct sockaddr*)&host_addr, addr_len) == SOCKET_ERROR) 
#else
    if (::connect(fdData, (struct sockaddr*)&host_addr, addr_len) == -1) 
#endif
    {
      LOG("fltk3::TCPSocket: connect - can't connect socket\n");
#ifdef WIN32
      closesocket(sData);
      sData = INVALID_SOCKET;
#else
      ::close(fdData);
      fdData = -1;
#endif
      return 0;
    }
  // all went well, we listen for connections now
  pStatus = CONNECTED;
  pPort = port;
#ifdef WIN32
  fltk3::add_fd(sData, fltk3::READ, pDataReadCB, this);
  fltk3::add_fd(sData, fltk3::EXCEPT, pDataExceptCB, this);
#else
  fltk3::add_fd(fdData, fltk3::READ, pDataReadCB, this);
  fltk3::add_fd(fdData, fltk3::EXCEPT, pDataExceptCB, this);
#endif
  return 1;
}


int fltk3::TCPSocket::available() const
{
  unsigned long n;
#ifdef WIN32
  DWORD rcvd;
  int ret = WSAIoctl(sData, FIONREAD, 0, 0, &n, sizeof(n), &rcvd, 0, 0);
  if (ret) ret = -1;
#else
  int ret = ioctl(fdData, FIONREAD, &n);
#endif
  //if (ret==0 && n>0) printf("%d bytes available\n", n);
  return (ret==-1) ? ret : n;
}


void fltk3::TCPSocket::pListenCB(int, void *user_data)
{
  fltk3::TCPSocket *t = (fltk3::TCPSocket*)user_data;
  LOG("fltk3::TCPSocket: listen callback\n");
#ifdef WIN32
  fltk3::remove_fd(t->sListen);
  fltk3::e_number = CONNECT;
  if (t->on_connect()==0) {
    if (t->callback()) 
      t->do_callback();
  }
  if (t->status()==CONNECTED) {
    closesocket(t->sListen);
    t->sListen = INVALID_SOCKET;
  } else if (t->status()==CLOSED || t->pPort==-1) {
    t->close();
  } else {
    t->listen(t->pPort);
  }
#else
  fltk3::remove_fd(t->fdListen);
  fltk3::e_number = CONNECT;
  if (t->on_connect()==0) {
    if (t->callback()) 
      t->do_callback();
  }
  if (t->status()==CONNECTED) {
    ::close(t->fdListen);
    t->fdListen = -1;
  } else if (t->status()==CLOSED || t->pPort==-1) {
    t->close();
  } else {
    t->listen(t->pPort);
  }
#endif
}


void fltk3::TCPSocket::pDataReadCB(int p, void *user_data)
{
  fltk3::TCPSocket *t = (fltk3::TCPSocket*)user_data;
  if (t->available()==0) {
    pDataExceptCB(p, user_data);
    return;
  }
  LOG("fltk3::TCPSocket: data read callback\n");
  fltk3::e_number = RECEIVE;
  if (t->on_receive()==0) {
    if (t->callback())
      t->do_callback();
  }
}


void fltk3::TCPSocket::pDataExceptCB(int, void *user_data)
{
  fltk3::TCPSocket *t = (fltk3::TCPSocket*)user_data;
  LOG("fltk3::TCPSocket: data except callback\n");
  fltk3::e_number = CLOSE;
  if (t->on_close()==0) {
    if (t->callback())
      t->do_callback();
  }
}


char fltk3::TCPSocket::listen(unsigned short port) 
{
  redraw();
#ifdef WIN32
  if (sListen==INVALID_SOCKET)
#else
    if (fdListen==-1)
#endif
    {
#ifdef WIN32
      if (!wsaStartup) {
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;
        wVersionRequested = MAKEWORD( 2, 2 );
        err = WSAStartup( wVersionRequested, &wsaData );
        if (err) {
          LOG("fltk3::TCPSocket: listen - WSAStartup failed!\n");
        }
        wsaStartup = 1;
      }
#endif
      pStatus = CLOSED;
      
      // create the socket
      LOG("fltk3::TCPSocket: listen - create socket\n");
#ifdef WIN32
      sListen = socket(AF_INET, SOCK_STREAM, 0);
      if (sListen==INVALID_SOCKET) {
        LOG("fltk3::TCPSocket: listen - can't open socket\n");
        return 0;
      }
#else
      fdListen = socket(AF_INET, SOCK_STREAM, 0);
      if (fdListen==-1) {
        LOG("fltk3::TCPSocket: listen - can't open socket\n");
        return 0;
      }
#endif
      
      // bind the socket to a port
      LOG("fltk3::TCPSocket: listen - bind socket\n");
      struct sockaddr_in my_addr;
      socklen_t addr_len = sizeof(my_addr);
      memset(&my_addr, 0, addr_len);
      my_addr.sin_family = AF_INET;
      my_addr.sin_port = htons(port);
      my_addr.sin_addr.s_addr = INADDR_ANY;
#ifdef WIN32
      if (::bind(sListen, (struct sockaddr*)&my_addr, addr_len) == SOCKET_ERROR) {
        LOG("fltk3::TCPSocket: listen - can't bind socket\n");
        closesocket(sListen);
        sListen = INVALID_SOCKET;
        return 0;
      }
#else
      if (::bind(fdListen, (struct sockaddr*)&my_addr, addr_len) == -1) {
        LOG("fltk3::TCPSocket: listen - can't bind socket\n");
        ::close(fdListen);
        fdListen = -1;
        return 0;
      }
#endif
    }
  // now go ahead and tell the socket that we are listening for connectin attempts  
  LOG("fltk3::TCPSocket: listen\n");
#ifdef WIN32
  if (::listen(sListen, 0) == SOCKET_ERROR) {
    LOG("fltk3::TCPSocket: listen - can't listen!\n");
    pStatus = CLOSED;
    closesocket(sListen);
    sListen = INVALID_SOCKET;
    return 0;
  }
#else
  if (::listen(fdListen, 0) == -1) {
    LOG("fltk3::TCPSocket: listen - can't listen!\n");
    pStatus = CLOSED;
    ::close(fdListen);
    fdListen = -1;
    return 0;
  }
#endif
  // all went well, we listen for connections now
  pStatus = LISTENING;
  pPort = port;
#ifdef WIN32
  fltk3::add_fd(sListen, pListenCB, this);
#else
  fltk3::add_fd(fdListen, pListenCB, this);
#endif
  return 1;
}


char fltk3::TCPSocket::accept()
{
  redraw();
  LOG("fltk3::TCPSocket: accept\n");
#ifdef WIN32
  if (sListen==INVALID_SOCKET) 
    return -1;
#else
  if (fdListen==-1) 
    return -1;
#endif
  
  struct sockaddr_in their_addr;
  socklen_t addr_len = sizeof(their_addr);
  
  LOG("fltk3::TCPSocket: accept - accept now\n");
#ifdef WIN32
  sData = ::accept(sListen, (struct sockaddr*)&their_addr, &addr_len);
  if (sData==INVALID_SOCKET) {
    LOG("fltk3::TCPSocket: accept - failed\n");
    close();
    return 0;
  }
#else
  fdData = ::accept(fdListen, (struct sockaddr*)&their_addr, &addr_len);
  if (fdData==-1) {
    LOG("fltk3::TCPSocket: accept - failed\n");
    close();
    return 0;
  }
#endif
  pStatus = CONNECTED;
#ifdef WIN32
  fltk3::add_fd(sData, fltk3::READ, pDataReadCB, this);
  fltk3::add_fd(sData, fltk3::EXCEPT, pDataExceptCB, this);
#else
  fltk3::add_fd(fdData, fltk3::READ, pDataReadCB, this);
  fltk3::add_fd(fdData, fltk3::EXCEPT, pDataExceptCB, this);
#endif
  return 1;
}


char fltk3::TCPSocket::send(const char *text)
{
  return send(text, strlen(text));
}


char fltk3::TCPSocket::send(const void *d, int n)
{
  LOG("fltk3::TCPSocket: send\n");
  pActive++; redraw();
#ifdef WIN32
  if (sData==INVALID_SOCKET) {
    return 0;
  }
  if (::send(sData, (const char*)d, n, 0)!=n) {
    return 0;
  }
#else
  if (fdData==-1) {
    return 0;
  }
  if (::send(fdData, d, n, 0)!=n) {
    return 0;
  }
#endif
  return 1;
}


int fltk3::TCPSocket::recv_all(void *d, int n) 
{
#ifdef WIN32
  if (sData==-1) {
    return 0;
  }
  char *dst = (char*)d;
  int sum = 0;
  for (;;) {
    int r = ::recv(sData, dst+sum, n, 0);
    if (r<0) 
      return r;
    sum += r;
    n -= r;
    if (n==0 || r==0) 
      return sum;
  }
#else
  if (fdData==-1) {
    return 0;
  }
  return ::recv(fdData, d, n, MSG_WAITALL);
#endif
}


int fltk3::TCPSocket::recv(void *d, int n)
{
#ifdef WIN32
  if (sData==-1) {
    return 0;
  }
  return ::recv(sData, (char*)d, n, 0);
#else
  if (fdData==-1) {
    return 0;
  }
  int ret = ::recv(fdData, d, n, 0);
#if 0
  int i; 
  char *c = (char*)d;
  if (ret>0) for (i=0; i<ret; i++) {
    if (isprint(c[i])) 
      printf("%c", c[i]);
    else
      printf("<%02x>", c[i]);
  }
  printf("\n");
#endif
  return ret;
#endif
}


int fltk3::TCPSocket::peek(void *d, int n)
{
#ifdef WIN32
  if (sData==-1) {
    return 0;
  }
  return ::recv(sData, (char*)d, n, MSG_PEEK);
#else
  if (fdData==-1) {
    return 0;
  }
  return ::recv(fdData, d, n, MSG_PEEK);
#endif
}


void fltk3::TCPSocket::flush_input(int n)
{
  char buffer[256];
  if (n<1) 
    n = available();
  for (;;) {
    int r = n;
    if (r>256) r = 256;
    recv(buffer, r);
    n = n - r;
    if (n<=0) return;
  }
}


char fltk3::TCPSocket::on_connect()
{
  return 0;
}


char fltk3::TCPSocket::on_receive()
{
  pActive++; redraw();
  return 0;
}


char fltk3::TCPSocket::on_close()
{
  return 0;
}


char fltk3::TCPSocket::reject()
{
  return -1;
}


int fltk3::TCPSocket::status() const
{
  return pStatus;
}


void fltk3::TCPSocket::draw()
{
  static const char *symlut[] = {
    "@6reload", "@3reload", "@2reload", "@1reload", 
    "@4reload", "@7reload", "@8reload", "@9reload" };
  
  draw_box();
  // draw the "S:" to indicate a server
  fltk3::font(labelfont(), labelsize()-2);
  fltk3::color(labelcolor()); // should probably be textcolor, but for now...
  // fltk3::draw("S:", x(), y(), h(), h(), FL_ALIGN_CENTER);
  int px =       fltk3::box_dx(box());
  int py =       fltk3::box_dy(box());
  int pw = w() - fltk3::box_dw(box());
  int ph = h() - fltk3::box_dh(box());
  switch (pStatus) {
    case CLOSED:
      fltk3::color(fltk3::GRAY);
      fltk3::draw("@3+", px, py, pw, ph, fltk3::ALIGN_CENTER);
      break;
    case LISTENING:
      fltk3::color(fltk3::GREEN);
      fltk3::draw("@circle", px, py, pw, ph, fltk3::ALIGN_CENTER);
      break;
    case VERIFYING:
      fltk3::color(fltk3::YELLOW);
      fltk3::draw("@circle", px, py, pw, ph, fltk3::ALIGN_CENTER);
      break;
    case CONNECTED:
      fltk3::color(fltk3::GREEN);
      fltk3::draw(symlut[pActive&7], px, py, pw, ph, fltk3::ALIGN_CENTER);
      break;
    default:
      fltk3::color(fltk3::GREEN);
      fltk3::draw("?", px, py, pw, ph, fltk3::ALIGN_CENTER);
      break;
  }
  draw_label();
}


//
// End of "$Id$".
//
