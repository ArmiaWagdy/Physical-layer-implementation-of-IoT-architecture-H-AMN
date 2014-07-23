//=====================================================================================
// Name        : USB.cpp
// Author      : Armia Wagdy
// Version     : 1.5
// Description : This file contains the APIs which will interface with the serial port
//=====================================================================================

#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "USB.h"

using namespace std;

/*********************************************************************************************/

int USB::OpenPort(void) {
        /* open serial port */
        file = open(USBPort_Path, O_RDWR | O_NOCTTY);
        if(file > 0) cout <<"Opening USB file....	 \t \t OK " <<endl;
        else cout << "Opening USB file....	 \t \t FAIL "<<endl;
        return file;
}

/*********************************************************************************************/

void USB::Init(void) {

     /* Check first if you managed to open the file */
     if(file > 0)
     {
          cout << "Configuring the USB Port ... ";
          /* Set custom options */
          cfsetispeed(&options, B9600);
          cfsetospeed(&options, B9600);
          // 8N1 -> (8bits, No parity, 1 stop bit)
          // CLOCAL  : local connection, no modem contol
          // CREAD   : enable receiving characters
          options.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
          // IGNPAR  : ignore bytes with parity errors
          options.c_iflag = IGNPAR;
          options.c_oflag = 0;
          /* set input mode as non-canonical mode */
          options.c_lflag = 0;
          Time = 10;
          Min  = 0;
          options.c_cc[VTIME]    = Time;     /* inter-character timer = 1 Second */
          options.c_cc[VMIN]     = Min;      /* blocking read until 0 chars received */
          /* Set this options */
          tcsetattr(file, TCSANOW, &options);
          cout << "\t \t OK" << endl;
          return;
     }
     else
     {
         /* Failed to configure the port as it can't be even opened */
         cout << "Configuring the USB Port ...\t \t FAIL" << endl;
     }
}

/*********************************************************************************************/

int USB::Write(int f,unsigned char* ptr, int length) {

    int numberOfBytes = 0;

    /* Check first if you managed to open the file */
    if(file > 0)
        {
            cout << "Writing bytes to USB Port ...";
            /* Writing data to serial port */
            numberOfBytes = write(f, ptr, length);
            tcdrain(file);
            cout << "\t \t OK" << endl;
        }
    else
        {
            /* Failed to write to the port as it can't be even opened */
            cout << "Writing bytes to USB Port ...\t \t FAIL" << endl;
        }
      return numberOfBytes;
}

/*********************************************************************************************/

int USB::Read(int f, unsigned char* buf, int count) {

    int n1 = 0;
    int n2 = 0;

    //Flushes Input and output buffer
    tcflush(file, TCIOFLUSH);

    /* Reading data from serial port */
     for(register int i = 0; i < 10 && !(n1) ; i++)
         {
             n1 = read(f, buf, 1);
             usleep(100000);
         }

     if (n1)
     {
         Time = 1;
         Min = 255;
         options.c_cc[VTIME] = Time;      /* inter-character timer = 0.1 Seconds */
         options.c_cc[VMIN]  = Min;   	  /* blocking read until 255 chars received */
         /* Set this options */
         tcsetattr(file, TCSANOW, &options);
         n2 = read(f, buf+n1, count);
         return n1+n2;
     }
     else
         {
         //Flushes Input and output buffer
         //tcflush(file, TCIOFLUSH);
         return 0;
         }
}

/*********************************************************************************************/

