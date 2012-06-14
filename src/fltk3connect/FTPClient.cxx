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

#include <fltk3connect/FTPClient.h>


#if 0

#include <fltk3/run.h>
#include <fltk3/dirent.h>
#include <fltk3/draw.h>
#include <fltk3/filename.h>

#ifdef WIN32
namespace fltk3 {
  char TCPSocket::wsaStartup = 0;
}
typedef int socklen_t;
#else
# include <unistd.h>
# include <strings.h>
# include <netinet/in.h> 
# include <sys/types.h> 
# include <sys/stat.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netdb.h>
# include <time.h>
#endif

#if 0
# include <stdio.h>
# include <stdarg.h>
# define LOG(x) fprintf(stderr, x)
#else
# define LOG(x)
#endif


fltk3::FTPClient::FTPClient(int x, int y, int w, int h, const char *l)
: fltk3::Group(x, y, w, h, l),
  pCommand(0, 0, w, h, 0, this),
  pData(0, 0, w, h, 0, this),
  pEnd(),
  pServerDir(0L),
  pLastReply((char*)malloc(1024)),
  pMsg(0L),
  pStatus(FTP_CLOSED)
{
  box(fltk3::DOWN_BOX);
  pCommand.box(fltk3::FLAT_BOX);
  pData.box(fltk3::FLAT_BOX);
  my_resize(x, y, w, h);
}


fltk3::FTPClient::~FTPClient()
{
  if (pLastReply) free(pLastReply);
  if (pMsg) free(pMsg);
  message("");
}


void fltk3::FTPClient::my_resize(int xp, int yp, int wp, int hp)
{
  xp += fltk3::box_dx(box());
  yp += fltk3::box_dy(box());
  wp -= fltk3::box_dw(box());
  hp -= fltk3::box_dh(box());
  pCommand.resize(xp+wp-hp-hp, yp, hp, hp);
  pData.resize(xp+wp-hp, yp, hp, hp);
}


void fltk3::FTPClient::resize(int xp, int yp, int wp, int hp)
{
  fltk3::Group::resize(xp, yp, wp, hp);
  my_resize(xp, yp, wp, hp);
}


void fltk3::FTPClient::draw()
{
  fltk3::Group::draw();
  int xp =       fltk3::box_dx(box());
  int yp =       fltk3::box_dy(box());
  int wp = w() - fltk3::box_dw(box());
  int hp = h() - fltk3::box_dh(box());
  fltk3::color(labelcolor());
  fltk3::font(labelfont(), labelsize());
  if (pMsg)
    fltk3::draw(pMsg, xp, yp, wp-2*hp, hp, fltk3::ALIGN_LEFT|fltk3::ALIGN_INSIDE);
}


void fltk3::FTPClient::message(const char *pattern, ...)
{
  va_list ap;
  va_start(ap, pattern);
  free(pMsg);
  vasprintf(&pMsg, pattern, ap);
  va_end(ap);
  
  int n = strlen(pMsg);
  if (pMsg[n]=='\n') { pMsg[n] = 0; n--; }
  if (pMsg[n]=='\r') { pMsg[n] = 0; n--; }
  
  puts(pMsg);
  redraw();
}


int fltk3::FTPClient::close() 
{
  pCommand.close();
  pData.close();
  pStatus = FTP_CLOSED;
  return 0;
}


int fltk3::FTPClient::sync_close() 
{
  return close();
}


int fltk3::FTPClient::open(unsigned char ip0, unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned short port, const char *name, const char *passwd)
{
  pName = strdup(name);
  pPasswd = strdup(passwd);
  
  if (pStatus!=FTP_CLOSED)
    close();
  
  // establish the TCP connection
  if (!pCommand.connect(ip0, ip1, ip2, ip3, port)) {
    return -1;
  }
  pStatus = FTP_CONNECTING;
  message("Connecting...");
  return 0;
}


int fltk3::FTPClient::sync_wait()
{
  int ret = -1, n;
  for (;;) {
    n = pCommand.available();
    if (n>0) {
      if (n>1023) n=1023;
      pCommand.recv(pLastReply, n);
      pLastReply[n] = 0;
      ret = atoi(pLastReply);
      break;
    }
    if (n<0) {
      perror("fltk3::FTPClient::sync_wait");
    }
    fltk3::wait(10.0);
    // TODO: timeout!
    // TODO: user cancel
    // TODO: data socket closed
    // TODO: command socket closed
  }
  return ret;
}


int fltk3::FTPClient::sync_open(unsigned char ip0, unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned short port, const char *name, const char *passwd)
{
  int ret;
  char cmd[1024];
  pName = strdup(name);
  pPasswd = strdup(passwd);
  
  if (pStatus!=FTP_CLOSED)
    close();
  pStatus = FTP_SYNC;
  
  // establish the TCP connection
  if (!pCommand.connect(ip0, ip1, ip2, ip3, port)) {
    message("sync_open: cant connect\n");
    return -1;
  }
  // wait for reply: 220 127.0.0.1 FTP server (tnftpd 20080929) ready.
  ret = sync_wait();
  if (ret!=220) {
    message("sync_open - connect message: %s", pLastReply);
    return -1;
  }
  
  // send the user name
  sprintf(cmd, "USER %s\r\n", name);
  pCommand.send(cmd);
  // wait for reply: 331 User matt accepted, provide password.
  ret = sync_wait();
  if (ret==220) { // some servers send the 220 twice, so give it another chance...
    ret = sync_wait();
  }
  if (ret!=331 && ret!=230) {
    message("sync_open - USER (expected 331): %s", pLastReply);
    return -1;
  }

  // send the password
  sprintf(cmd, "PASS %s\r\n", passwd);
  pCommand.send(cmd);
  // wait for reply: 230 User matt logged in.
  ret = sync_wait();
  if (ret!=230) {
    message("sync_open - PASS (expected 230): %s", pLastReply);
    return -1;
  }

  // set transfer type to binary
  pCommand.send("TYPE I\r\n");
  // wait for reply: 200 Type set to I.
  ret = sync_wait();
  if (ret!=200) {
    message("sync_open - TYPE I (expected 200): %s", pLastReply);
    return -1;
  }
  
  return sync_pwd();
}
  

int fltk3::FTPClient::sync_open(const char *host, const char *name, const char *passwd)
{
  int err;
  struct addrinfo *addr;
  err = getaddrinfo(host, "ftp", 0, &addr);
  if (err!=0) {
    message("sync_open - getaddrinfo: can't find host %s", host);
    return -1;
  }
  err = sync_open(
                  addr->ai_addr->sa_data[2], 
                  addr->ai_addr->sa_data[3], 
                  addr->ai_addr->sa_data[4], 
                  addr->ai_addr->sa_data[5], 
                  addr->ai_addr->sa_data[0]*256+addr->ai_addr->sa_data[1], 
                  name, passwd);
  return err;
}


int fltk3::FTPClient::sync_pwd()
{
  int ret; 
  
  // get the current working directory
  pCommand.send("PWD\r\n");
  // wait for reply: 257 "/Users/matt" is the current directory.
  ret = sync_wait();
  if (ret!=257) {
    message("sync_pwd - PWD: %s", pLastReply);
    return -1;
  }
  char *a = strchr(pLastReply, '"');
  if (!a) return -1;
  char *b = strchr(a+1, '"');
  if (!b) return -1;
  *b = 0;
  if (pServerDir) free(pServerDir);
  pServerDir = strdup(a+1);
  
  return 0;
}


int fltk3::FTPClient::sync_get_time(const char *serverfile, struct timespec &timespec)
{
  char cmd[1024];
  int ret;
  
  // get file modification time
  sprintf(cmd, "MDTM %s\r\n", serverfile);
  pCommand.send(cmd);
  // wait for reply: 213 20101217152000
  ret = sync_wait();
  // no such file or directory
  if (ret==550) {
    timespec.tv_nsec = 0;
    timespec.tv_sec = 0;
    return 0;
  }
  if (ret!=213) {
    printf("sync_get_time - MDTM %s: %s", serverfile, pLastReply);
    return -1;
  }
  // interprete result
  struct tm tm; memset(&tm, 0, sizeof(tm));
  sscanf(pLastReply, "%d %04d%02d%02d%02d%02d%02d", &ret,
         &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
         &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
  tm.tm_mon--;
  tm.tm_year -= 1900;
  timespec.tv_nsec = 0;
  //timespec.tv_sec = mktime(&tm);
  timespec.tv_sec = timegm(&tm);
  
  // printf("%s\n", asctime(&tm));
  return 0;
}


int fltk3::FTPClient::sync_chdir(const char *serverpath)
{
  /*
  if (strchr(serverpath, '/')) {
    // the path contains morethan a single element, split the chdir up
    char rel_path[FL_PATH_MAX];
    fl_filename_relative(rel_path, sizeof(rel_path), serverpath, pServerDir);
    serverpath = rel_path;
    
  }*/
  
  // we just slam the new directory at the ftp server. I am not sure if this 
  // will work on any server, or if we have to change a single dir at a time?!
  char cmd[1024];
  int ret;
  
  // get file modification time
  sprintf(cmd, "CWD %s\r\n", serverpath);
  pCommand.send(cmd);
  // wait for reply: 250 CWD command successful.
  // or: 550 tempftp: No such file or directory.
  ret = sync_wait();
  if (ret!=250) {
    printf("sync_chdir - CWD %s: %s", serverpath, pLastReply);
    return -1;
  }
  return sync_pwd();
}
  
      
int fltk3::FTPClient::sync_mkdir(const char *serverpath)
{
  char cmd[1024];
  int ret;
  
  // get file modification time
  sprintf(cmd, "MKD %s\r\n", serverpath);
  pCommand.send(cmd);
  // wait for reply: 257 "tempftp" directory created.
  // or: 550 tempftp: File exists.
  ret = sync_wait();
  if (ret!=257 && ret!=550) {
    printf("sync_mkdir - MKD %s: %s", serverpath, pLastReply);
    return -1;
  }
  return 0;
}


int fltk3::FTPClient::sync_send_file_if_newer(const char *clientfile, const char *serverfile)
{
  int ret;
  if (!serverfile) serverfile = clientfile;
  
  // get the modification time of the local file
  struct stat statbuf;
  ret = ::stat(clientfile, &statbuf);
  if (ret!=0) {
    printf("sync_send_file_if_newer %s: Can't get local modification\n", clientfile);
    return ret;
  }
  
  // get the modification time of the remote file
  struct timespec ts;
  ret = sync_get_time(serverfile, ts);
  if (ret!=0) {
    printf("sync_send_file_if_newer %s: Can't get remote modification time\n", serverfile);
    return ret;
  }

  // compare modification times
  if (statbuf.st_mtimespec.tv_sec > ts.tv_sec) {
    time_t a = statbuf.st_mtimespec.tv_sec, b = ts.tv_sec;
    printf("File:   %s\n", serverfile);
    printf("Local:  %s", ctime(&a));
    printf("Remote: %s", ctime(&b));
    printf("Copying...\n");
    return sync_send_file(clientfile, serverfile);
  }
  return 0;
}


int fltk3::FTPClient::sync_send_file(const char *clientfile, const char *serverfile)
{
  char cmd[1024], reply[1024];
  int ret, n;

  message("Sending \"%s\"", clientfile);  
  
  if (!serverfile)
    serverfile = clientfile;
  // does this file exist?
  FILE *f = fopen(clientfile, "rb");
  if (!f) {
    message("can't open client file");
    return -1;
  }
  // create passive connection
  pCommand.send("PASV\n");
  // wait for the connection data: 227 Entering Passive Mode (127,0,0,1,192,66)
  for (;;) {
    fltk3::wait(10.0);
    if ( (n=pCommand.available()) ) {
      if (n>1023) n=1023;
      pCommand.recv(reply, n);
      ret = atoi(reply);
      if (ret==227) break;
      message("sync_open: %s", reply);
      return -1;
    }
  }
  unsigned int ip0, ip1, ip2, ip3, portHi, portLo, port;
  const char *s = strchr(reply, '(');
  if (s==0) {
    message("Reply does not contain ip and port for PASV\n");
    return -1;
  }
  if (sscanf(s+1, "%d,%d,%d,%d,%d,%d", &ip0, &ip1, &ip2, &ip3, &portHi, &portLo)!=6) {
    message("Reply does not contain ip and port for PASV\n");
    return -1;
  }
  port = portHi * 256 + portLo;
  if (pData.connect(ip0, ip1, ip2, ip3, port)==0) {
    message("Can't establish PASV connection to %d.%d.%d.%d:%d\n", ip0, ip1, ip2, ip3, port);
    return -1;
  }
  // tell the server to receive a file
  sprintf(cmd, "STOR %s\r\n", serverfile);
  pCommand.send(cmd);
  // wait for reply: 150 Opening BINARY mode data connection for 'test.txt'.
  for (;;) {
    fltk3::wait(10.0);
    if ( (n=pCommand.available()) ) {
      if (n>1023) n=1023;
      pCommand.recv(reply, n);
      ret = atoi(reply);
      if (ret==150) break;
      message("sync_open: %s", reply);
      return -1;
    }
  }
  // now send the file
  for (;;) {
    char buffer[1024];
    int bn = fread(buffer, 1, 1024, f);
    if (bn==0) break;
    pData.send(buffer, bn);
    fltk3::wait(0.0);
  }
  // when we are done, we close the data connection
  pData.close();
  // wait for: 226 Transfer complete.  
  for (;;) {
    fltk3::wait(10.0);
    if ( (n=pCommand.available()) ) {
      if (n>1023) n=1023;
      pCommand.recv(reply, n);
      ret = atoi(reply);
      if (ret==226) break;
      message("sync_open: %s", reply);
    }
  }
  fclose(f);
  return 0;
}


int fltk3::FTPClient::sync_send_dir(const char *aClientpath, const char *serverpath)
{
  int ret;
  char clientpath[FLTK3_PATH_MAX];
  strcpy(clientpath, aClientpath);
  int len = strlen(clientpath); if (len>0 && clientpath[len-1]=='/') clientpath[len-1] = 0;
  
  message("sending folder \"%s\" to \"%s\"", aClientpath, serverpath);
  
  // change the remote dir (if it doesn't exist, do we want to create it?)
  ret = sync_chdir(serverpath);
  if (ret!=0) {
    message("sync_send_dir: can't open server path \"%s\".\n", serverpath);
    return -1;
  }
  
  // list the local dir
  dirent **dir;
  int i, n = fltk3::filename_list(clientpath, &dir);
  if (n<0) {
    message("sync_send_dir: can't list client path \"%s\".\n", clientpath);
    return -1;
  }
  
  // do this for each entry
  for (i=0; i<n; i++) {
    if (!dir[i]->d_name) continue;
    char filename[FLTK3_PATH_MAX];    
    char fullname[FLTK3_PATH_MAX];
    strcpy(filename, dir[i]->d_name);
    int len = strlen(filename); if (len>0 && filename[len-1]=='/') filename[len-1] = 0;
    sprintf(fullname, "%s/%s", clientpath, filename);
    if (   strcmp(filename, ".")==0 || strcmp(filename, "..")==0)
      continue;
    if (fltk3::filename_isdir(fullname)) {
      // it's a directory: create one on the remote end
      sync_mkdir(filename); // don't worry if this fails
      ret = sync_chdir(filename);
      if (ret!=0) {
        message("sync_send_dir: can't change into server dir \"%s\".\n", filename);
        return -1;
      }
      // recurse into the directory and send it to the client as well
      char new_serverpath[FLTK3_PATH_MAX], new_clientpath[FLTK3_PATH_MAX];
      sprintf(new_serverpath, "%s/%s", serverpath, filename);
      sprintf(new_clientpath, "%s/%s", clientpath, filename);
      ret = sync_send_dir(new_clientpath, new_serverpath);
      if (ret!=0) {
        message("sync_send_dir: aborting directory transfer \"%s\".\n", serverpath);
        return -1;
      }
      // clean up
      ret = sync_chdir("..");
      if (ret!=0) {
        message("sync_send_dir: can't change into parent dir.\n");
        return -1;
      }
    } else {
      // it's a file: copy to remote if local is newer
      ret = sync_send_file_if_newer(fullname, filename);
      if (ret!=0) {
        message("sync_send_dir: error sending file \"%s\".\n", filename);
        return -1;
      }
    }
  }
  
  // clean allocated resources
  fltk3::filename_free_list(&dir, n);
  return 0;
}



int fltk3::FTPClient::get(const char *filename, void *&data, unsigned int &size)
{
  return 0;
}


char fltk3::FTPClient::on_receive_command()
{
  if (pStatus==FTP_SYNC) return 0;
  char buffer[1024];
  int n = pCommand.available();
  int reply = -1;
  if (n>0) {
    if (n>1023) n = 1023;
    pCommand.recv(buffer, n);
    buffer[n] = 0;
    if (n>0 && buffer[n-1]=='\n') buffer[n-1] = 0;
    if (n>1 && buffer[n-2]=='\r') buffer[n-2] = 0;
    printf("(LOG: %s)\n", buffer);
    reply = atoi(buffer);
  }
  switch (pStatus) {
    case FTP_CLOSED: // ignore
      break;
    case FTP_CONNECTING: // TCP runs, we are waiting for 220 Ready
      if (reply/100==2) {
        printf("FTP_SENDING_NAME\n");
        pStatus = FTP_SENDING_NAME;
        sprintf(buffer, "USER %s\n", pName);
        puts(buffer);
        pCommand.send(buffer);
      } else {
        printf("CONNECTING error: %s\n", buffer);
        pStatus = FTP_ERROR;
      }
      break;
    case FTP_SENDING_NAME:
      if (reply==331) {
        printf("FTP_SENDING_PASS\n");
        pStatus = FTP_SENDING_PASS;
        sprintf(buffer, "PASS %s\n", pPasswd);
        puts(buffer);
        pCommand.send(buffer);
//      } else if (reply/100==2) { // no password required
//        goto set_type;
      } else {
        printf("SENDING_NAME error: %s\n", buffer);
        pStatus = FTP_ERROR;
      }
      break;
    case FTP_SENDING_PASS:
//set_type:
      if (reply/100==2) {
        printf("FTP_SETTING_TYPE\n");
        pStatus = FTP_SETTING_TYPE;
        puts(buffer);
        pCommand.send("TYPE I\n"); // I=binary, A=ASCII
      } else {
        printf("SENDING_PASS error: %s\n", buffer);
        pStatus = FTP_ERROR;
      }
      break;
    case FTP_SETTING_TYPE:
      if (reply/100==2) {
        printf("FTP_SET_PASSIVE\n");
        pStatus = FTP_SET_PASSIVE;
        puts(buffer);
        pCommand.send("PASV\n");
      } else {
        printf("SETTING_TYPE error: %s\n", buffer);
        pStatus = FTP_ERROR;
      }
      break;
    case FTP_SET_PASSIVE:
      if (reply/100==2) {
        printf("FTP_CONNECTED\n");
        unsigned int ip0, ip1, ip2, ip3, portHi, portLo, port;
        const char *s = strchr(buffer, '(');
        if (s==0) {
          printf("Reply does not contain ip and port for PASV\n");
          pStatus = FTP_ERROR;
          break;
        }
        if (sscanf(s+1, "%d,%d,%d,%d,%d,%d", &ip0, &ip1, &ip2, &ip3, &portHi, &portLo)!=6) {
          printf("Reply does not contain ip and port for PASV\n");
          pStatus = FTP_ERROR;
          break;
        }
        port = portHi * 256 + portLo;
        if (pData.connect(ip0, ip1, ip2, ip3, port)==0) {
          printf("Can't establish PASV connection to %d.%d.%d.%d:%d\n", ip0, ip1, ip2, ip3, port);
          pStatus = FTP_ERROR;
          break;
        }
        printf("Connection established!\n");
        pStatus = FTP_CONNECTED;
        //pCommand.send("MODE x\n"); // S=Stream, B=Block, C=Compressed
        //pCommand.send("MDTM test.txt\n");
        //pCommand.send("SIZE test.txt\n");
        //pCommand.send("RETR test.txt\n");
        //pCommand.send("RETR schild_nordbahnhof.dxf\n");
        pCommand.send("PWD\n");
        //pCommand.send("NLST\n"); // ls
        //pCommand.send("LIST\n"); // ls -asl
        //pCommand.send("STAT\n"); // -> long list of information abou server via control connection
      } else {
        printf("SET_PASSIVE error: %s\n", buffer);
        pStatus = FTP_ERROR;
      }
      break;
  }
  return 1;
}


/*
 pCommand.send("NLST\n");
 (LOG: 150 Opening BINARY mode data connection for 'file list'.)
 (LOG: 226 Transfer complete.)
 (DATA: .adobe
  .artofillusion ...)
 
 pCommand.send("MDTM test.txt\n");
 (LOG: 213 20101217152000)
           YYYYMMDDhhmmss
 or
 (LOG: 550 "test.txt": No such file or directory.)
 
 Syntax: RETR remote-filename
 Begins transmission of a file from the remote host. Must be preceded by either
 a PORT command or a PASV command to indicate where the server should send data.
 (LOG: 150 Opening BINARY mode data connection for 'test.txt' (39 bytes)
 
 Syntax: STOR remote-filename
 Begins transmission of a file to the remote site. Must be preceded by either a 
 PORT command or a PASV command so the server knows where to accept data from.

 Syntax: ABOR
 Aborts a file transfer currently in progress.
 
 Syntax: ALLO size [R max-record-size] Allocate storage.
 */


char fltk3::FTPClient::on_receive_data()
{
  char buffer[1024];
  int n = pData.available();
  printf("(DATA(%d))\n", n);
  while (n>0) {
    if (n>1024) {
      pData.recv(buffer, 1024);
      n -= 1024;
    } else {
      pData.recv(buffer, n);
      n = 0;
    }
  }
  return 1;
}



fltk3::FTPCommandSocket::FTPCommandSocket(int x, int y, int w, int h, const char *l, fltk3::FTPClient *ftp)
: fltk3::TCPSocket(x, y, w, h, l),
  pFTP(ftp)
{ }


char fltk3::FTPCommandSocket::on_receive() {
  return pFTP->on_receive_command();
}


fltk3::FTPDataSocket::FTPDataSocket(int x, int y, int w, int h, const char *l, fltk3::FTPClient *ftp)
: fltk3::TCPSocket(x, y, w, h, l),
  pFTP(ftp)
{ }


char fltk3::FTPDataSocket::on_receive() {
  return pFTP->on_receive_data();
}

#endif

//
// End of "$Id$".
//
