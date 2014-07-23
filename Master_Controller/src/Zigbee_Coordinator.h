//=====================================================================================
// Name        : Zigbee_Coordinator.h
// Author      : Armia Wagdy
// Version     : 1.5
// Description : This file contains the declaration of the class which can
//               make the transmitted packet (Frame type = 0x90)
// 				 This class simply has some functions which will take the 64-bit address
//				 of the destination, the length of data and the data you want to send it
//			     as inputs and makes the transmitted packet.
//=====================================================================================

#ifndef ZIGBEE_COORDINATOR_H_
#define ZIGBEE_COORDINATOR_H_

#define START_BYTE                      0x7E
#define TRANSMIT_REQUEST_FRAME_TYPE     0x10


class Coordinator
    {
        private:
            unsigned char Addr64[8];   		//The 64-bit Address of desitnation
            unsigned char Addr16[2];		//The 16-bit Address of desitnation
            unsigned char checksumByte;		//Checksum Byte

            // calcualate the checksum byte of the frame
            unsigned char checksum(unsigned char* frame);
        public:
            int lengthOfFrame;						// length of frame
            unsigned char packet[100];				//Transmitted Packet
            unsigned char receivedData[100];		//Received data
            unsigned char* makePacket(unsigned char* Data, unsigned char lengthOfData);
            void setDestinationAddr64(char* address);
            void readPacket(int cnt, unsigned char* received_data);
    };


#endif /* ZIGBEE_COORDINATOR_H_ */
