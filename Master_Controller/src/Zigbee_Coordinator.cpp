//=====================================================================================
// Name        : Zigbee_Coordinator.cpp
// Author      : Armia Wagdy
// Version     : 1.5
// Description : This class simply has function that will take the 64-bit address
//				 of the destination, the length of data and the data you want to send it
//			     as inputs and makes the transmitted packet.
//=====================================================================================

#include <iostream>
#include "Zigbee_Coordinator.h"
#define DEBUG	0

using namespace std;

/*********************************************************************************************/

/*This function takes the 64-bit destination address and slices it in chunks of eight bits*/
void Coordinator :: setDestinationAddr64(char* address)
{
        //Temporary array
        char ary[16] = {0};

        //Conversion from ASCII to HEX
        for(int i = 0; i < 16; i ++)
        {
            // If Number (its ASCII is between 48 and 57 (Decimal))
            if(address[i] >= 48 && address[i] <= 57)
                ary[i] = address[i] - 48;
            // If Capital letter (its ASCII is between 65 and 70 (Decimal))
            else if(address[i] >= 65 && address[i] <= 70)
                ary[i] = address[i] - 55;
            // If Small letter (its ASCII is between 97 and 102 (Decimal))
            else if (address[i] >= 97 && address[i] <= 102)
                ary[i] = address[i] - 87;
        }

        //Initialize Addr64 to zeros
        for(register unsigned char i = 0; i < 8 ; i++)	Addr64[i] = 0;

        //Set Destination Address
        for(register unsigned char i = 0; i < 8 ; i++)
        {
            Addr64[i] |= (ary[2*i] << 4) | (ary[(2*i)+1]);
        }

#if DEBUG
        for(int i = 0; i < 8; i++) cout <<"ary: " << (int)Addr64[i] <<endl;
#endif

        return;
}

/*********************************************************************************************/

/* This function makes the transmitted packet of frame type 0x90 */
unsigned char* Coordinator :: makePacket(unsigned char* Data, unsigned char lengthOfData)
{
        lengthOfFrame = lengthOfData + 18; 		 //As the length of frame without data = 18
        packet[0] = START_BYTE;			   		 //Start delimeter
        packet[1] = 0x00;				   		 //MSB of the length is forced to zero
        packet[2] = lengthOfData + 14;	  	 	 //LSB of the length of frame
        packet[3] = TRANSMIT_REQUEST_FRAME_TYPE; //Frame type
        packet[4] = 0x00;						 //No response needed

        //Integrating the 64-bit destination address in the packet
        for(register int i = 0; i < 8; i++) packet[i+5] = Addr64[i];

        //Set the 16-bit destination address to that value as I don't know it
        //(will be changed in future work)
        packet[13] = 0xFF;
        packet[14] = 0xFE;

        packet[15] = 0x00;					     //Broadcast radius
        packet[16] = 0x00;						 //options

        //Integrating the 64-bit destination address in the packet
        for(register int i = 0; i < lengthOfData; i++)  packet[i+17] = *(Data+i);

        packet[16 + lengthOfData +1] = checksum(packet); //checksum byte

#if DEBUG
        for (int i = 0; i < lengthOfFrame; i++) cout << "byte [" << i << "] = " << hex << (int)packet[i] <<endl;
#endif
        return packet;
}

/*********************************************************************************************/

// This function calculates the checksum byte of the frame */
unsigned char Coordinator :: checksum(unsigned char* frame)
{
    unsigned long int checksum = 0x00;
    for(register unsigned int i = 3; i <= (lengthOfFrame - 2); i++)  checksum += frame[i];
    checksumByte = 0xFF - (checksum & 0xFF);
    return checksumByte;
}

/*********************************************************************************************/

// This function reads the received packet and display some results
void Coordinator :: readPacket(int cnt, unsigned char* received_data)
{
            //Check if the length of frame is shorter than 17
            //As the length of the receive packet with only one byte of data = 17 bytes

            if(cnt < 17) 			{
                cout << "Status: FAIL" << '\n';
                return;
            }
            // Check if it is a receive packet (with frame type 0x90)
            else if (*(received_data + 3) != 0x90)
            {
                cout << "Status: FAIL" << '\n';
                return;
            }

            // Check if the packet contains just an ACK
            else if (*(received_data + 15) == 0xAA)
            {
                if(*(received_data + 16) == 0xFF)
                {
                    cout << "Status: OK" << '\n';
                    return;
                }
                else if (*(received_data + 16) == 0x00)
                {
                    cout << "Status: FAIL" << '\n';
                    return;
                }

            }

            // Check if the packet contains DATA
            else if(*(received_data + 15) == 0xDD)
            {
                cout << "Status: OK" << '\n';
                cout << "Received data = " << (int)(*(received_data + 16)) << '\n';
            }
}
/*********************************************************************************************/

