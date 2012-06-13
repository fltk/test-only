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

#include <fltk3connect/SerialConnection.h>

#if 0 // TODO: FLTK3 Connect is not yet implemented 

#include "isel.h"

#include "Fl/Fl.H"
#include "Fl/Flmm_Message.H"

#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
# define M_PI 3.141592635
#endif
#ifndef M_PI
# define M_PI_2 (0.5*3.141592635)
#endif

#ifdef WIN32
static HANDLE handle = 0;
#endif

static unsigned char movingAxis = 0;
static int timeout = 200000;

const int moverate = 6000;
const int homerate = 3000;


/**
 * Allocate resources for milling class.
 */
Ic_Mill::Ic_Mill() {
  millspeed = 1000;
}


/**
 * Free resources of milling class.
 */
Ic_Mill::~Ic_Mill() {
  try {
    motorOff();
    stopSerial();
  }
  catch( IselEx ex ) { }
}


/**
 * Allocate resources for Isel mill.
 */
Ic_Isel_Mill::Ic_Isel_Mill() {
}


/**
 * Free Isel mill resources.
 */
Ic_Isel_Mill::~Ic_Isel_Mill() {
}


/**
 * Instruct the operator to flip the raw part over.
 *
 * \return 0 if the operator canceled, 1 to continue, and 2 to skip part of the program
 */
int Ic_Mill::operatorFlipsPart()
{
  return Flmm_Message::choice("Please flip the part over now.\n\n"
    "Click 'Skip' to skip this side of the part.\n"
    "Click 'OK' to run the program for this tool.\n"
    "Click 'Cancel' to stop the entire program.",
    "Cancel", "OK", "Skip");
}


/**
 * Instruct the operator to flip the raw part over.
 *
 * This methods stops the spindle and moves it out of the 
 * users way. The following code may restart and reposition the 
 * spindle.
 *
 * \return 0 if the operator canceled, 1 to continue, and 2 to skip part of the program
 */
int Ic_Isel_Mill::operatorFlipsPart()
{
  motorOff();
  gotoHomePosition();
  return Ic_Mill::operatorFlipsPart();
}


/**
 * Operator needs to change tool.
 *
 * \param n index of tool to use
 * \param q if this is set, quietly assume that the right tool is used
 * \return 0 if the operator canceled, 1 to continue, and 2 to skip part of the program
 */
int Ic_Isel_Mill::operatorToolChange(int n, int q) 
{
  if (q) return 1;
  motorOff();
  gotoToolChangePosition();
  return Flmm_Message::choice("Please change to tool #%d, %s.\n\n"
    "Click 'Skip' to skip this tool.\n"
    "Click 'OK' to run the program for this tool.\n"
    "Click 'Cancel' to stop the entire program.",
    "Cancel", "OK", "Skip", n, tool_name[n]);
}


static void contMoveAxisCB( void* )
{
  if ( movingAxis == 0 )
    return;
  
  int spd = 500, off = 100, xoff=0, yoff=0, zoff=0;
  
  if ( movingAxis &  1 ) xoff =  off;
  if ( movingAxis &  2 ) xoff = -off;
  if ( movingAxis &  4 ) yoff =  off;
  if ( movingAxis &  8 ) yoff = -off;
  if ( movingAxis & 16 ) zoff =  off;
  if ( movingAxis & 32 ) zoff = -off;
  
  mill->writeSerial( "@0a%d,%d,%d,%d,%d,%d,0,30", xoff, spd, yoff, spd, zoff, spd );
  
  //Fl::add_timeout( ((double)off/(double)spd)*0.8, contMoveAxisCB );
  Fl::add_timeout( 0.01, contMoveAxisCB );
}


/**
 * Enable motion of one or more axes.
 * 
 * \deprecated
 */
void Ic_Isel_Mill::moveAxis( unsigned char axis )
{
  movingAxis |= axis;
  //	Fl::add_timeout( 0.01, contMoveAxisCB );
}


/**
 * Disable motion of one or more axes.
 * 
 * \deprecated
 */
void Ic_Isel_Mill::stopAxis( unsigned char axis )
{
  movingAxis &= ~axis;
}


/**
 * Start communication with mill.
 *
 * \return true, if connection could be established.
 */
bool Ic_Isel_Mill::startSerial()
{
#ifdef WIN32
  DCB arg;
  char *port = "COM6";
  
  handle = CreateFile( port, GENERIC_READ|GENERIC_WRITE, 0, 0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);
  if (handle == 0) 
  {
    //++ Flmm_Message::alert( "Can't open serial port!" );
    return false;
  }
  PurgeComm( handle, PURGE_TXCLEAR|PURGE_RXCLEAR );
  arg.DCBlength = sizeof( arg );
  if (GetCommState(handle, &arg)==0) 
  {
    Flmm_Message::alert( "Can't get status of serial port!" );
    return false;
  }
  arg.BaudRate = CBR_9600;
  arg.fBinary = TRUE;
  arg.fParity = FALSE;
  arg.fOutxCtsFlow = FALSE;
  arg.fOutxDsrFlow = FALSE;
  arg.fDtrControl = FALSE;
  arg.fDsrSensitivity = FALSE;
  arg.fRtsControl = RTS_CONTROL_DISABLE;
  arg.fAbortOnError = TRUE;
  arg.ByteSize = 8;
  arg.Parity = NOPARITY;
  arg.StopBits = ONESTOPBIT;
  if (SetCommState(handle, &arg)==0) 
  {
    Flmm_Message::alert( "Can't set status of serial port!" );
    return false;
  }
  COMMTIMEOUTS timeout = { 10, 5, 10, 5, 10 };
  //COMMTIMEOUTS timeout = { MAXDWORD, 0, 100, 500, 500 };
  if (SetCommTimeouts(handle, &timeout)==0) 
  {
    Flmm_Message::alert( "Can't set timeout of serial port!" );
    return false;
  }
  PurgeComm(handle, PURGE_TXCLEAR|PURGE_RXCLEAR);
  SetCommMask( handle, EV_TXEMPTY );

  setTimeout( 10 );
  try {
    writeSerial( "@07" );
    writeSerial( "@0d%d,%d,%d", homerate, homerate, homerate );
    writeSerial( "@0z1" );
  }
  catch ( IselEx ) { }
  setTimeout( );
#endif
  return true;
}


/**
 * Stop communication with mill.
 */
void Ic_Isel_Mill::stopSerial() 
{
#ifdef WIN32
  if ( handle == 0 )
    return;
  CloseHandle( handle );
  handle = 0;
#endif
}


char dbg( char reply )
{
  PRINTF1("<%c>\n", reply );
  return reply;
}


/**
 * Set the timeout for communication via the serial line.
 *
 * \param[in] msec milliseconds to wait before giving timout error
 */
void Ic_Isel_Mill::setTimeout( int msec )
{
  if ( msec == 0 )
    msec = 20000;
  timeout = msec;
}


/**
 * Get the current position of the spindle.
 *
 * \param[out] x, y, z spindle coordinates
 * \return error code character
 */
char Ic_Isel_Mill::getPosition( int &x, int &y, int &z )
{
#ifdef WIN32
  resendCmd:
  char ret = writeSerial( "@0P" ); // this should receive a zero for confirmation
  if ( ret != '0' ) return dbg( ret );

  if ( dontSend )
  {
    x = 0; y = 0; z = 0;
    return '0';
  }

  DWORD res = 0, comErr = 0;
  char rcv[20] = "000000000000000000";
  for ( int j=0; j<18; j++ )
  {
    for ( int i=0; i<10; i++ )
    {
      Fl::check();
      ClearCommBreak( handle );
      BOOL ret = ReadFile( handle, (LPVOID)(rcv+j), 1, &res, 0L );
      ClearCommError( handle, &comErr, 0L );
      if ( ret == 0 ) 
	throw( IselEx('e') );
      if ( res )
	break;
      if ( stopped() )
	throw( IselEx('s') );
      if ( resend() )
	goto resendCmd;
      if ( skip() )
	return dbg('0');
    }
    if ( i == 10 ) throw( IselEx('t') );
  }
  rcv[18] = 0;	
  int X, Y, Z;
  sscanf( rcv, "%06x%06x%06x", &X, &Y, &Z );
  x = X; y = Y; z = Z;
  PRINTF3( "--> getPosition found %d,%d,%d\n", x, y, z );
#endif
  return dbg('0');
}


/**
 * Start the spindle and wait for it to rev up.
 */
void Ic_Isel_Mill::motorOn()
{
#ifdef WIN32
  writeSerial( "@0B65529,1" );
  Sleep( 1000 );
#endif
}


/**
 * Stop the spindle and wait for it to rev down.
 */
void Ic_Isel_Mill::motorOff()
{
#ifdef WIN32
  writeSerial( "@0B65529,0" );
  Sleep( 1000 );
#endif
}


/**
 * Move the spindle to the home position in one or more axes.
 * 
 * Move z to home first, then y, then x.
 */
void Ic_Isel_Mill::gotoHomePosition()
{
  writeSerial( "@0d%d,%d,%d", homerate, homerate, homerate );
  writeSerial( "@0R7" );
}


/**
 * Move the spindle to a safe position for convenient tool change.
 * 
 * Stop the motor, go home, go to a good position to change the tool.
 */
void Ic_Isel_Mill::gotoToolChangePosition()
{
  PRINTF( "--> gotoToolChangePosition\n" );
  motorOff();
  gotoHomePosition();
  writeSerial( "@0a%d,%d,%d,%d,0,%d,0,30", MAX_X, moverate, MAX_Y/2, moverate, moverate );
}

/**
 * Safely move to a new position.
 *
 * Move from the current position to the given position at max speed.
 * Move up to z-home, move to x, y, z-home, move down to new z.
 *
 * \param[in] x, y, z new position 
 */
void Ic_Isel_Mill::gotoPositionSave( int x, int y, int z )
{
  int X, Y, Z;
  writeSerial( "@0R4" );	// home Z
  getPosition( X, Y, Z );
  writeSerial( "@0M%d,%d,%d,%d,%d,%d,0,30", x, moverate, y, moverate, Z, moverate );
  writeSerial( "@0M%d,%d,%d,%d,%d,%d,0,30", x, moverate, y, moverate, z, moverate );
}

/**
 * Move spindle to a new position fast.
 *
 * \param[in] x, y, z are 1/100th of a milimeter
 */
void Ic_Isel_Mill::gotoPosition( int x, int y, int z )
{
  writeSerial( "@0M%d,%d,%d,%d,%d,%d,0,30", x, moverate, y, moverate, z, moverate );
}


/**
 * Move spindle to a new position at feed rate.
 *
 * \param[in] x, y, z are 1/100th of a milimeter
 */
void Ic_Isel_Mill::milltoPosition( int x, int y, int z, int spd )
{
  if (!spd) spd = millspeed;
  writeSerial( "@0M%d,%d,%d,%d,%d,%d,0,30", x, spd, y, spd, z, spd );
}


/**
 * Mill an arc (circular curve) from the current position.
 *
 * \param[in] a starting agle in deg
 * \param[in] b final angle in deg
 * \param[in] r radius of arc in Isel coordintes
 * \param[in] spd feed speed
 */
void Ic_Isel_Mill::millArc(float a, float b, int r, int spd) 
{
  if (a==b || r<=0) return;
  if (!spd) spd = millspeed;
  if (a>b) {
    a += 180.0f; b += 180.0f;
  }
  a = a/180.0f*M_PI; b = b/180.0f*M_PI;
  float x = -sinf(a) * r, y = -cosf(a) * r;
  int ix = (int)x, iy = (int)y;
  int rx = 1, ry = 1;
  writeSerial( "@03" );
  if (a<b) { // clockwise
    writeSerial( "@0f1" );
         if (ix>=0  && iy>0) { rx= 1; ry=-1; }
    else if (ix>0 && iy<=0)  { rx=-1; ry=-1; }
    else if (ix<0 && iy>=0)  { rx= 1; ry= 1; }
    else if (ix<=0  && iy<0) { rx=-1; ry= 1; }
  } else {
    writeSerial( "@0f-1" );
         if (ix>0  && iy>=0) { rx=-1; ry= 1; }
    else if (ix>=0 && iy<0)  { rx= 1; ry= 1; }
    else if (ix<=0 && iy>0)  { rx=-1; ry=-1; }
    else if (ix<0  && iy<=0) { rx= 1; ry=-1; }
  }
  // calculate the circumference as described in the manual
  // this fails if we cross into a new quadrant!
//#if 0
  float dx = r*cosf(a)-r*cosf(b);
  float dy = r*sinf(a)-r*sinf(b);
  float len = fabsf(dx) + fabsf(dy);
//#else
  //const float M_PI_2 = M_PI*0.5f; 
  // make sure that a is less than b
  if (a>b) { 
    float c = a; a = b; b = c; 
  }
  // rotate forward until a is in the first quadrant
  if (a<0.0) { 
    int n = -a/M_PI_2+1; 
    a += n*M_PI_2; 
    b += n*M_PI_2; 
  }
  // rotate backward until a is in the first quadrant
  if (a>M_PI_2) { 
    int n = a/M_PI_2; 
    a -= n*M_PI_2; 
    b -= n*M_PI_2; 
  }
  // if b is in the first quadrant as well, this is easy:
  if (b<M_PI_2) {
    dx = r*cosf(a)-r*cosf(b);
    dy = r*sinf(a)-r*sinf(b);
    len = fabsf(dx) + fabsf(dy);
  } else {
    len = 0;
    // if b is in the third quadrant or further, add 2r for every quadrant
    if (b>M_PI) {
      int n = b/M_PI_2 - 1;
      len += n*2*r;
      b -= n*M_PI_2;
    }
    // at this point, a is in the first quadrant, and b is in the second quadrant
    len += r * (cosf(a) + 1.0f-sinf(a) + 1.0f-sinf(b) - cosf(b));
  }  
//#endif
  float err = 0.5 * ( (float)r*(float)r - x*(x-rx) - y*(y-ry) );
  writeSerial( "@0y%d,%d,%d,%d,%d,%d,%d", (int)(len+0.5), spd, (int)(err+0.5), ix, iy, rx, ry );
  writeSerial( "@07" );
}


/**
 * Mill a rectangle at a given depth ccw.
 *
 * \param[in] x1, y1 first corner
 * \param[in] x2, y2 second corner
 * \param[in] z depth
 */
void Ic_Mill::millRect(int x1, int y1, int x2, int y2, int z) 
{
  milltoPosition(x1, y1, z);
  if ((x1>x2 && y1>y2) || (x1>x2 && y1>y2)) {
    milltoPosition(x2, y1, z);
    milltoPosition(x2, y2, z);
    milltoPosition(x1, y2, z);
  } else {
    milltoPosition(x1, y2, z);
    milltoPosition(x2, y2, z);
    milltoPosition(x2, y1, z);
  }
  milltoPosition(x1, y1, z);
}


/**
 * Mill a full rectangle at a given depth ccw, outside to inside.
 *
 * \param[in] x1, y1 first corner
 * \param[in] x2, y2 second corner
 * \param[in] z depth
 *
 * \todo x1, y1 must be top left. We should allow any corner!
 */
void Ic_Mill::millFilledRect(int x1, int y1, int x2, int y2, int z, int zsafe, int tr) 
{
  int w = x2 - x1; if (w<0) w = -w;
  int h = y2 - y1; if (h<0) h = -h;
  int s = (w<h) ? w : h;
  int n = s/tr/2, i = n;
  gotoPosition(x1+i*tr, y1+i*tr, zsafe);
  for ( ; i>=0; i--) {
    millRect(x1+i*tr, y1+i*tr, x2-i*tr, y2-i*tr, z);
  }
  gotoPosition(x1, y1, zsafe);
}


/**
 * Mill a rectangle at a given depth with rounded inside corners.
 *
 * \param[in] x1, y1 first corner
 * \param[in] x2, y2 second corner
 * \param[in] z depth
 * \param[in] r radius of corners, please add the tool radius when calling this
 */
void Ic_Mill::millRoundedRect(int x1, int y1, int x2, int y2, int z, int r) 
{
  if ((x1>x2 && y1>y2) || (x1<x2 && y1<y2)) {
    float a = 180, rr = r;
    if (x1>x2 && y1>y2) { a = 360; rr = -r; }
    milltoPosition(x1+rr, y1, z);
    milltoPosition(x2-rr, y1, z);
    millArc(a, a-90, r); a -= 90;
    milltoPosition(x2, y2-rr, z);
    millArc(a, a-90, r); a -= 90;
    milltoPosition(x1+rr, y2, z);
    millArc(a, a-90, r); a -= 90;
    milltoPosition(x1, y1+rr, z);
    millArc(a, a-90, r); a -= 90;
    milltoPosition(x1+rr, y1, z);
  } else {
    float a = 270, rr = r;
    if (x1>x2 && y1<y2) { a = 90; rr = -r; }
    milltoPosition(x1, y1-rr, z);
    milltoPosition(x1, y2+rr, z);
    millArc(a, a-90, r); a -= 90;
    milltoPosition(x2-rr, y2, z);
    millArc(a, a-90, r); a -= 90;
    milltoPosition(x2, y1-rr, z);
    millArc(a, a-90, r); a -= 90;
    milltoPosition(x1+rr, y1, z);
    millArc(a, a-90, r); a -= 90;
    milltoPosition(x1, y1-rr, z);
  }
}


/**
 * move the tool over the Z switch and return the Z position.
 *
 * This can be used to find the precise length of the tool.
 *
 * \return z position when tool hit the switch.
 */
int Ic_Isel_Mill::getNewToolZ()
{
  PRINTF( "--> getNewToolZ\n" );
  if ( dontSend )	return calZT;
  int spd = 500, x, y, z;
  motorOff();
  // reset all
  writeSerial( "@0R7" ); // move all home
  writeSerial( "@0n7" ); // make this 0,0,0
  writeSerial( "@0z1" ); // 3d interpolation
  // hit the switch
  writeSerial( "@0M0,%d,0,%d,7300,%d,0,30", spd, spd, spd ); // go down on switch
  getPosition( x, y, z );
  writeSerial( "@0a0,%d,0,%d,-300,%d,0,30", moverate, moverate, moverate ); // move 3mm up
  writeSerial( "@0a0,%d,0,%d,-300,%d,0,30", moverate, moverate, moverate ); // move 3mm up
  // and reset again
  writeSerial( "@0R7" ); // move all home
  writeSerial( "@0n7" ); // make this 0,0,0
  writeSerial( "@0z1" ); // 3d interpolation
  return z;
}


/**
 * Write a commend to the serial port.
 * 
 * Write a command and wait for a single character reply.
 * Return the reply or 'e' for error or 't' for timeout.
 *
 * \param[in] cmd printf style command followed by varaible number of arguments
 * \return single character error code
 */
char Ic_Isel_Mill::writeSerial( char *cmd, ... )
{
#ifdef WIN32
  if ( handle == 0 ) 
    throw( IselEx('e') );
  while ( paused() ) Fl::wait();
  if ( stopped() ) throw( IselEx('e') );
  
  // create the string that we want to send... 
  char buffer[1024];
  va_list args;
  va_start(args, cmd);
  vsprintf(buffer, cmd, args);
  va_end(args); 
  /*
  static int purgeCnt = 100;
  if ( purgeCnt-- == 0 )
  {
    PurgeComm(handle, PURGE_TXCLEAR|PURGE_RXCLEAR);
    purgeCnt = 100;
    PRINTF("Purging...\n");
  }
  
  static int reopenCnt = 1000;
  if ( reopenCnt-- == 0 )
  {
    stopSerial();
    startSerial();
    reopenCnt = 1000;
    PRINTF("Reopening...\n");
  }
  */
  // write the string to the serial line
  PRINTF1( "Sending '%s'\n", buffer );
  if ( dontSend ) return '0';
  int size = strlen( buffer );
  buffer[size++] = '\r';
  buffer[size] = 0;
resendCmd:
  DWORD comErr = 0;
  size = strlen( buffer );
  char *src = buffer;
  DWORD res = 0;
  while (size>0) {
    ClearCommBreak( handle );
    BOOL ret = WriteFile( handle, (LPCVOID)src, size, &res, 0L );
    DWORD ev = EV_TXEMPTY;
    WaitCommEvent( handle, &ev, 0L );
    ClearCommError( handle, &comErr, 0L );
    if ( comErr )
    {
      if ( comErr & CE_BREAK ) PRINTF( "*** COM_ERROR (W): break condition\n" );
      if ( comErr & CE_DNS ) PRINTF( "*** COM_ERROR (W): DNS\n" );
      if ( comErr & CE_FRAME ) PRINTF( "*** COM_ERROR (W): hardware framing error\n" );
      if ( comErr & CE_IOE ) PRINTF( "*** COM_ERROR (W): I/O error\n" );
      if ( comErr & CE_MODE ) PRINTF( "*** COM_ERROR (W): unsupported mode\n" );
      if ( comErr & CE_OOP ) PRINTF( "*** COM_ERROR (W): out of paper ;-)\n" );
      if ( comErr & CE_OVERRUN ) PRINTF( "*** COM_ERROR (W): buffer overrun\n" );
      if ( comErr & CE_PTO ) PRINTF( "*** COM_ERROR (W): time out (parallel)\n" );
      if ( comErr & CE_RXOVER ) PRINTF( "*** COM_ERROR (W): input buffer overflow\n" );
      if ( comErr & CE_RXPARITY ) PRINTF( "*** COM_ERROR (W): parity\n" );
      if ( comErr & CE_TXFULL ) PRINTF( "*** COM_ERROR (W): output buffer is full\n" );
      Flmm_Message::alert( "There was an error writing to COM1.\nPlease check the error log!" );
    }
    if ( ret == 0 ) 
      throw( IselEx('e') );
    if ( res == 0 ) 
      throw( IselEx('e') );
    size -= res;
    src += res;
  }
  
  // now wait for the reply from the client (a one byte error code...)
  char rcv[8];
  for ( int i=0; i<timeout; i++ )
  {
    Fl::check();
    ClearCommBreak( handle );
    BOOL ret = ReadFile( handle, (LPVOID)rcv, 1, &res, 0L );
    ClearCommError( handle, &comErr, 0L );
    if ( comErr )
    {
      if ( comErr & CE_BREAK ) PRINTF( "*** COM_ERROR (R): break condition\n" );
      if ( comErr & CE_DNS ) PRINTF( "*** COM_ERROR (R): DNS\n" );
      if ( comErr & CE_FRAME ) PRINTF( "*** COM_ERROR (R): hardware framing error\n" );
      if ( comErr & CE_IOE ) PRINTF( "*** COM_ERROR (R): I/O error\n" );
      if ( comErr & CE_MODE ) PRINTF( "*** COM_ERROR (R): unsupported mode\n" );
      if ( comErr & CE_OOP ) PRINTF( "*** COM_ERROR (R): out of paper ;-)\n" );
      if ( comErr & CE_OVERRUN ) PRINTF( "*** COM_ERROR (R): buffer overrun\n" );
      if ( comErr & CE_PTO ) PRINTF( "*** COM_ERROR (R): time out (parallel)\n" );
      if ( comErr & CE_RXOVER ) PRINTF( "*** COM_ERROR (R): input buffer overflow\n" );
      if ( comErr & CE_RXPARITY ) PRINTF( "*** COM_ERROR (R): parity\n" );
      if ( comErr & CE_TXFULL ) PRINTF( "*** COM_ERROR (R): output buffer is full\n" );
      Flmm_Message::alert( "There was an error writing to COM1.\nPlease check the error log!" );
    }
    if ( ret == 0 ) 
      throw( IselEx('e') );
    if ( res )
      return dbg(rcv[0]);
    if ( stopped() )
      throw( IselEx('s') );
    if ( resend() )
      goto resendCmd;
    if ( skip() )
      return dbg('0');
  }
  throw( IselEx('t') );
#endif
  return '0';
}



#ifdef IGNORE_THE_REST


/**
 * Read some bytes from the serial port.
 *
 * \param[in] buffer destination buffer
 * \param[in] size number of bytes expected
 * \return MoOK, if all bytes were received
 */
mResult MoSerialIO::read(void *buffer, mInt size) {
#ifdef WIN32
  if (!handle) return MoFAIL;
  if (noWaitMode) {
    COMMTIMEOUTS timeout = { 10, 5, 10, 5, 10 };
    /// don't fail, even if not supported
    SetCommTimeouts(handle, &timeout);					
    noWaitMode = MoFALSE;
  }
  mByte *dst = (mByte*)buffer;
  DWORD nb;
  while (size>0) {
    if (ReadFile(handle, (LPVOID)dst, size, &nb, 0L)==0) return MoFAIL;
    if (nb==0) return MoTIMEOUT;
    size -= nb;
    dst += nb;
  }
  return MoOK;
#else
  /// \todo +++UX
  return MoFAIL;
#endif
}

/**
 * Read bytes already in the buffer from the serial port.
 *
 * \param[in] buffer destination buffer
 * \param[in] size number of bytes expected
 * \return MoOK, if all bytes were received
 */
mResult MoSerialIO::readNow(void *buffer, mInt size) {
#ifdef WIN32
  if (!handle) return MoFAIL;
  if (!noWaitMode) {
    /// ok, it's not 'now', but it's fast
    COMMTIMEOUTS timeout = { 1, 1, 1, 5, 10 };			
    SetCommTimeouts(handle, &timeout);
    noWaitMode = MoTRUE;
  }
  mByte *dst = (mByte*)buffer;
  DWORD nb;
  while (size>0) {
    if (ReadFile(handle, (LPVOID)dst, size, &nb, 0L)==0) return MoFAIL;
    if (nb==0) return MoTIMEOUT;
    size -= nb;
    dst += nb;
  }
  return MoOK;
#else
  /// \todo +++UX
  return MoFAIL;
#endif
}

/**
 * Write some bytes to the serial port.
 *
 * \param[in] buffer source buffer
 * \param[in] size number of bytes to send
 * \return MoOK, if all bytes were sent
 */
mResult MoSerialIO::write(void *buffer, mInt size) {
#ifdef WIN32
  if (!handle) return MoFAIL;
  mByte *src = (mByte*)buffer;
  DWORD res = 0;
  while (size>0) {
    if (WriteFile(handle, (LPCVOID)src, size, &res, 0L)==0) return MoFAIL;
    if (res==0) return MoTIMEOUT;
    size -= res;
    src += res;
  }
  return MoOK;
#else
  /// \todo +++UX
  return MoFAIL;
#endif
}

/**
 * Empty the send and receive buffer.
 */
mResult MoSerialIO::purge() {
#ifdef WIN32
  if (!handle) return MoFAIL;
  if (PurgeComm(handle, PURGE_TXCLEAR|PURGE_RXCLEAR)==0) return MoFAIL;
  return MoOK;
#else
  /// \todo +++UX
  return MoFAIL;
#endif
}


#endif

#endif

//
// End of "$Id$".
//
