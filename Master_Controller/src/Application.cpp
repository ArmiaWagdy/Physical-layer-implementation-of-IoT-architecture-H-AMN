//===============================================================================
// Name        : Application.cpp
// Author      : Armia Wagdy
// Copyright   : 1.5
// Description : This code sends a packet to the serial port connected to a zigbee
//				 RF module (configured as a Coordinator in API mode) through USB and
//               reads data from it.
//================================================================================
#include <iostream>
#include "USB.h"
#include "Zigbee_Coordinator.h"
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "ProcessInput.h"
#include <cstring>

#define DEBUG 0

using namespace std;


int main(int argc, char *argv[])
{
      USB USB;
      Coordinator Master;
      int file; 				//file descriptor

      /* Taking Input from terminal */
      //char *argvv[] = {"6","0013A20040987839","13","HIGH","BOOLEAN","DIGITAL","0","W"};
      unsigned char* payload = process_input(argv);


      /*Set Destination Address */
      Master.setDestinationAddr64(argv[1]);

      /* Make Packet */
      Master.makePacket(payload,*(payload+1) + 2) ;

      /* open and initialize the serial port */
      file = USB.OpenPort();
      USB.Init();

      /* Send packet to the XBEE (Coordinator) through USB*/
      int numberOfBytes = USB.Write(file, Master.packet, Master.lengthOfFrame);
      cout << "Number of written bytes = " << numberOfBytes <<endl;

      /* Receive data from Router XBEE */
      int n = USB.Read(file, Master.receivedData, sizeof(Master.receivedData));
      cout << "Number of read bytes = " << n << '\n';

#if DEBUG
      for (int i = 0; i < n ; i++) cout << hex << (int)Master.receivedData[i] << '\n';
#endif

      Master.readPacket(n,Master.receivedData);

  return 0;
}
