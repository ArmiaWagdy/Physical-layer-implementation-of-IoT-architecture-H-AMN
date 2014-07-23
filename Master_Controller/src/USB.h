//=====================================================================================
// Name        : USB.h
// Author      : Armia Wagdy
// Version     : 1.5
// Description : This file contains the declaration of the class used in the
//               interface with the serial port
//=====================================================================================

#ifndef USB_H_
#define USB_H_

#include    <termios.h>
#define  	USBPort_Path	"/dev/ttyACM0"
#define 	Baudrate  		B9600

using namespace std;

class USB {
    private:
        int file; // File descriptor
        unsigned char Time; 	  //time between the arrival of two characters.
        unsigned char Min;		  //min number of characters to make the read function return
        struct termios options;   // Set up the control structure
    public:
        int  OpenPort (void);
        void Init     (void);
        int  Write    (int f,unsigned char* ptr, int length);
        int  Read     (int f, unsigned char* buf, int count);
};


#endif /* USB_H_ */
