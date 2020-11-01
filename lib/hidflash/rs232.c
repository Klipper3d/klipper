/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017 Teunis van Beelen
*
* Email: teuniz@gmail.com
*
***************************************************************************
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************
*/


/* Last revision: November 22, 2017 */

/* For more info and how to use this library, visit: http://www.teuniz.net/RS-232/ */


#include "rs232.h"
#include <string.h>
#include <stdio.h>

#if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)  /* Linux & FreeBSD */

    int tty_fd;
    struct termios old_termios;
    struct termios new_termios;
    

 int RS232_OpenComport(char *comport) {

    char str[64]= "/dev/";
    strcat(str, comport);
    
    tty_fd = open(str, O_RDWR | O_NOCTTY);
    if (tty_fd < 0) {
        fprintf(stderr, "error, counldn't open [%s]\n", str);
        return 1;
    }
    if (tcgetattr(tty_fd, &old_termios) != 0) {
        fprintf(stderr, "tcgetattr(fd, &old_termios) failed: %s\n", strerror(errno));
        return 1;
    }
    memset(&new_termios, 0, sizeof(new_termios));
    new_termios.c_iflag = IGNPAR;
    new_termios.c_oflag = 0;
    new_termios.c_cflag = CS8 | CREAD | CLOCAL | HUPCL;
    new_termios.c_lflag = 0;
    new_termios.c_cc[VINTR]    = 0;
    new_termios.c_cc[VQUIT]    = 0;
    new_termios.c_cc[VERASE]   = 0;
    new_termios.c_cc[VKILL]    = 0;
    new_termios.c_cc[VEOF]     = 4;
    new_termios.c_cc[VTIME]    = 0;
    new_termios.c_cc[VMIN]     = 1;
    //new_termios.c_cc[VSWTC]    = 0;
    new_termios.c_cc[VSTART]   = 0;
    new_termios.c_cc[VSTOP]    = 0;
    new_termios.c_cc[VSUSP]    = 0;
    new_termios.c_cc[VEOL]     = 0;
    new_termios.c_cc[VREPRINT] = 0;
    new_termios.c_cc[VDISCARD] = 0;
    new_termios.c_cc[VWERASE]  = 0;
    new_termios.c_cc[VLNEXT]   = 0;
    new_termios.c_cc[VEOL2]    = 0;


    if (cfsetispeed(&new_termios, B1200) != 0) {
        fprintf(stderr, "cfsetispeed(&new_termios, B1200) failed: %s\n", strerror(errno));
        return 1;
    }
    if (cfsetospeed(&new_termios, B1200) != 0) {
        fprintf(stderr, "cfsetospeed(&new_termios, B1200) failed: %s\n", strerror(errno));
        return 1;
    }

    if (tcsetattr(tty_fd, TCSANOW, &new_termios) != 0) {
        fprintf(stderr, "tcsetattr(fd, TCSANOW, &new_termios) failed: %s\n", strerror(errno));
        return 1;
    }

    return 0;
//======================================================================================
 }


 int RS232_SendByte(unsigned char byte) {
   int n = write(tty_fd, &byte, 1);
   if(n < 0) {
     if(errno == EAGAIN) {
       return 0;
     } else {
       return 1;
     }
   }


   return(0);
  }

/*
  int RS232_ReadByte() {
    unsigned char c;
    if (read(tty_fd,&c,1)>0) {
        return c;
    }
    
    return -1;
  }
*/

 void RS232_CloseComport() {

   tcsetattr(tty_fd, TCSANOW, &old_termios);
   close(tty_fd);

 }


 void RS232_enableDTR() {
   int status;

   if(ioctl(tty_fd, TIOCMGET, &status) == -1) {
     perror("unable to get portstatus");
   }

   status |= TIOCM_DTR;    /* turn on DTR */

   if(ioctl(tty_fd, TIOCMSET, &status) == -1) {
     perror("unable to set portstatus");
   }
 }


 void RS232_disableDTR()
 {
   int status;

   if(ioctl(tty_fd, TIOCMGET, &status) == -1) {
     perror("unable to get portstatus");
   }

   status &= ~TIOCM_DTR;    /* turn off DTR */

   if(ioctl(tty_fd, TIOCMSET, &status) == -1) {
     perror("unable to set portstatus");
   }
 }


 void RS232_enableRTS()
 {
   int status;

   if(ioctl(tty_fd, TIOCMGET, &status) == -1) {
     perror("unable to get portstatus");
   }

   status |= TIOCM_RTS;    /* turn on RTS */

   if(ioctl(tty_fd, TIOCMSET, &status) == -1) {
     perror("unable to set portstatus");
   }
 }


 void RS232_disableRTS() {
   int status;

   if(ioctl(tty_fd, TIOCMGET, &status) == -1) {
     perror("unable to get portstatus");
   }

   status &= ~TIOCM_RTS;    /* turn off RTS */

   if(ioctl(tty_fd, TIOCMSET, &status) == -1) {
     perror("unable to set portstatus");
   }
 }

 void RS232_send_magic(){
   write(tty_fd,"1EAF",4);
 }


#else  /* windows */

HANDLE Cport;

char mode_str_2[] = "baud=9600 data=8 parity=n stop=1 dtr=off rts=off";

int RS232_OpenComport(char *comport)
{
	
	//printf("%s\n %s\n %s\n %p\n", mode_str_2, comports[comport_number], comport, Cport);
    
	char str[32] = "\\\\.\\";
	strcat(str, comport);
	//printf("%s\n", str);
	
  Cport = CreateFileA(str,
                      GENERIC_READ|GENERIC_WRITE,
                      0,                          /* no share  */
                      NULL,                       /* no security */
                      OPEN_EXISTING,
                      0,                          /* no threads */
                      NULL);                      /* no templates */

  if(Cport==INVALID_HANDLE_VALUE)
  {
    //printf("> unable to open comport\n");
    return(1);
  }

  DCB port_settings;
  memset(&port_settings, 0, sizeof(port_settings));  /* clear the new struct  */
  port_settings.DCBlength = sizeof(port_settings);

  if(!BuildCommDCBA(mode_str_2, &port_settings))
  {
    printf("> unable to set comport dcb settings\n");
    CloseHandle(Cport);
    return(1);
  }

  if(!SetCommState(Cport, &port_settings))
  {
    printf("> unable to set comport cfg settings\n");
    CloseHandle(Cport);
    return(1);
  }

  COMMTIMEOUTS Cptimeouts;

  Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
  Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
  Cptimeouts.ReadTotalTimeoutConstant    = 0;
  Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  Cptimeouts.WriteTotalTimeoutConstant   = 0;

  if(!SetCommTimeouts(Cport, &Cptimeouts))
  {
    printf("> unable to set comport time-out settings\n");
    CloseHandle(Cport);
    return(1);
  }

  return(0);
}


int RS232_SendByte(unsigned char byte)
{
  int n;

  WriteFile(Cport, &byte, 1, (LPDWORD)((void *)&n), NULL);

  if(n<0)  return(1);

  return(0);
}

void RS232_CloseComport()
{
  CloseHandle(Cport);
}

void RS232_enableDTR()
{
  EscapeCommFunction(Cport, SETDTR);
}


void RS232_disableDTR()
{
  EscapeCommFunction(Cport, CLRDTR);
}


void RS232_enableRTS()
{
  EscapeCommFunction(Cport, SETRTS);
}


void RS232_disableRTS()
{
  EscapeCommFunction(Cport, CLRRTS);
}

void RS232_send_magic()
{
  int n;

  WriteFile(Cport, "1EAF", 4, (LPDWORD)((void *)&n), NULL);
}

#endif

